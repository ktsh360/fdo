/*
 * 
* Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
* 
 */
#include "stdafx.h"
#include <malloc.h>

#include "FdoRdbmsUtil.h"
#include "FdoRdbmsFilterProcessor.h"
#include "FdoRdbmsSchemaUtil.h"
#include "FdoRdbmsException.h"
#include "LockConflict.h"
#include "LockUtility.h"
#include "DbiConnection.h"
#include "../../SchemaMgr/Ph/Mgr.h"
#include <Inc/Util/thread.h>

wchar_t* getComDir (); // in Rdbms.cpp

static char *unkownDbiError = "Unknown dbi error";

static void ThrowUnknownError()
{
    throw FdoRdbmsException::Create(NlsMsgGet(FDORDBMS_55, "Unknown dbi error"));
}

static void ThrowLastError (wchar_t* err_msg)
{
	throw FdoRdbmsException::Create(err_msg);
}

#define  SWITCH_CONTEXT   {\
    if( mOpen == FdoConnectionState_Closed ) \
            throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established")); \
	if( mContext->last_error_msg != NULL )	\
	{	\
		free( mContext->last_error_msg );	\
		mContext->last_error_msg = NULL;	\
	} }

#define   THROW_DBI_EXCEPTION { \
    if( mContext == NULL || mContext->last_error_msg == NULL )  \
    { \
        ThrowUnknownError(); \
    } else \
        throw FdoRdbmsException::Create( mContext->last_error_msg ); \
    }

static void ThrowNotSupportedError()
{
    throw FdoRdbmsException::Create(NlsMsgGet(FDORDBMS_55, "TODO TODO"));
}

#define   INVOKE_DBI_FUNC( dbifunc ) { \
        ThrowNotSupportedError(); \
        return 1; \
}

#define   TODO { \
        ThrowNotSupportedError(); \
        return 1; \
}


const wchar_t DbiConnection::DELIMITER[] = L";\"";
const wchar_t DbiConnection::DELIMITER2[] = L"\"";
const wchar_t DbiConnection::SEPARATOR = L'=';
const wchar_t DbiConnection::DATASOURCE[] = L"Service";
const wchar_t DbiConnection::DATASOURCE2[] = L"DataSourceName";
const wchar_t DbiConnection::USERID[] = L"Username";
const wchar_t DbiConnection::USERID2[] = L"UserId";
const wchar_t DbiConnection::PASSWORD[] = L"Password";
const wchar_t DbiConnection::SCHEMA[] = L"DataStore";
const wchar_t DbiConnection::CONNECTIONSTRING[] = L"ConnectionString";
const wchar_t DbiConnection::GENERATEDEFAULTGEOMETRYPROPERTY[] = L"GenerateDefaultGeometryProperty";


DbiConnection::DbiConnection( ):
    mOpen(FdoConnectionState_Closed),
    mDbiContextId(-1),
    mFilterProcessor( NULL ),
    mSchemaUtil( NULL ),
    mIndex(0),
    mContext(NULL)
{
    mConnectionString = NULL;

    // Set the flag that indicates whether or not the RDBMS user has a Workspace
    // Manager environment.
    // TODO: Execute a query to set the value. At this point Workspace Manager
    //       is not supported and hence the value is set to FALSE. The variable
    //       is required to allow functions to redirect their calls to either an
    //       Workspace Manager or Alternate API.

    mIsWorkspaceManagerEnvironment = FALSE;


}

DbiConnection::~DbiConnection(void)
{
    if ( mOpen != FdoConnectionState_Closed )
        Close();

    if( mConnectionString != NULL )
        delete [] mConnectionString;

    if( mFilterProcessor != NULL )
        delete mFilterProcessor;

//  dbi_destroy_context(mContext);
    delete (mGdbiConnection);

    rdbi_term (&mContext);
}

void DbiConnection::SetConnectionString( const wchar_t* connString )
{
    if( mConnectionString != NULL )
        delete [] mConnectionString;

    mConnectionString = new wchar_t[wcslen( connString  ) + 1];
    if( mConnectionString != NULL )
        wcscpy( mConnectionString, connString );
}

FdoConnectionState DbiConnection::Open (
    bool skipPending
    )
{
    ParseInfo* info = NULL;

    if ( mOpen != FdoConnectionState_Open )
    {
        info = parseConnectString ( mConnectionString );

        if ( mOpen == FdoConnectionState_Closed )
        {
            int rdbi_status = RDBI_GENERIC_ERROR;

            // We currently overload what RDBI sees as "connection string".  To most
            // drivers, it is just a data store name, and sometimes a little more
            // (e.g. port number).  For ODBC, a connection string is more formal, 
            // and quite distinct from the datasource/username/password case.
            // Ideally, RDBI would use all four parameters explicitly, but it is not worth
            // disrupting the other drivers just to get the special case.  Only
            // ODBC Provider currently supports a property actually called
            // "ConnectionString".
            if (NULL == info->mConnectionStringProperty)
                rdbi_status = rdbi_connect ( mContext, info->mDataSource, info->mUser, info->mPassword, &mDbiContextId );
            else
                rdbi_status = rdbi_connect ( mContext, info->mConnectionStringProperty, NULL, NULL, &mDbiContextId );

            if( rdbi_status == RDBI_SUCCESS ) {
                mOpen = FdoConnectionState_Pending;
            }
            else {
                rdbi_get_msg( mContext );
                THROW_DBI_EXCEPTION;
            }
        }

        if ( skipPending || info->mSchema )
        {
            if ( rdbi_set_schema( mContext, info->mSchema ) == RDBI_SUCCESS ) {
				if ( rdbi_autocommit_off(mContext) == RDBI_SUCCESS )
					mOpen = FdoConnectionState_Open;
            }
            else
            {
				wchar_t	err_msg[RDBI_MSG_SIZE+1];
                rdbi_get_msg( mContext );
				wcsncpy(err_msg, mContext->last_error_msg, RDBI_MSG_SIZE);
				err_msg[RDBI_MSG_SIZE] = '\0';
                Close();
				ThrowLastError(err_msg);
            }
        }
        if (this->mGdbiConnection != NULL)
            mGdbiConnection->SetIsGeometryFromOrdinatesWanted(info->mIsGeometryFromOrdinatesWanted);
    }
    
    mDbSchemaName = info->mSchema;

    if( NULL != info )
        delete info;

    return mOpen;
}

void DbiConnection::SetConnectionState (FdoConnectionState state)
{
    mOpen = state;
}

void DbiConnection::Close ()
{
    if( mSchemaUtil != NULL ) {
        delete mSchemaUtil;
        mSchemaUtil = NULL;
    }

    if( mOpen != FdoConnectionState_Closed )
    {
        SWITCH_CONTEXT
        rdbi_disconnect( mContext );
        mOpen = FdoConnectionState_Closed;
		mGdbiConnection->Close();
        mDbSchemaName = L"";
        mDbiContextId  = -1;
    }
}

bool DbiConnection::IsWorkspaceManagerEnvironment ()
{
    return mIsWorkspaceManagerEnvironment;
}

FdoRdbmsSchemaUtil* DbiConnection::GetSchemaUtil( )
{
    if( mSchemaUtil == NULL )
        mSchemaUtil = new FdoRdbmsSchemaUtil( this );

    return mSchemaUtil;
}

FdoRdbmsUtil* DbiConnection::GetUtility()
{
    return &mUtility;
}

bool DbiConnection::SetTransactionLock( const char *sqlStatement )
{
    int     rc;
    bool    ret = false;
    char    parm[128];
    int     rows_processed = 0;
    int     util_cursor;

    SWITCH_CONTEXT

    rdbi_est_cursor( mContext, &util_cursor);

    rc = rdbi_sql_va( mContext, RDBI_VA_DEFNAMES|RDBI_VA_EXEC,
                        util_cursor,
                        (char *)sqlStatement,
                        RDBI_VA_EOL,
                        "1", RDBI_STRING, 128,
                            (void *) parm,
                        RDBI_VA_EOL);
    if ( rc == RDBI_SUCCESS )
    {
        ret = true;
        rc = rdbi_fetch( mContext, util_cursor, 1, &rows_processed);
        if( rc == RDBI_RESOURCE_LOCK )
            ret = false;
    }

    rdbi_end_select( mContext, util_cursor);
    rdbi_fre_cursor( mContext, util_cursor);

    return ret;
}

char* DbiConnection::GetUser()
{

    ParseInfo* info;

    info = parseConnectString (mConnectionString);

    char *user = NULL;
    if (NULL != info->mUser)
    {
        user = new char[strlen(info->mUser)+1];
        strcpy(user, info->mUser);
    }
    else
    {
        user = new char[1];
        user[0] = '\0';
    }
    delete info;
    return user;

}

char* DbiConnection::GetPassword()
{
	ParseInfo* info;

	info = parseConnectString (mConnectionString);

	char *password = NULL;
	if (NULL != info->mPassword)
	{
		password = new char[strlen(info->mPassword)+1];
		strcpy(password, info->mPassword);
	}
	else
	{
		password = new char[1];
		password[0] = '\0';
	}
	delete info;
	return password;
}

char* DbiConnection::GetSchema()
{

    ParseInfo* info;

    info = parseConnectString (mConnectionString);

    char *schema = NULL;
    if (NULL != info->mSchema)
    {
        schema = new char[strlen(info->mSchema)+1];
        strcpy(schema, info->mSchema);
    }
	else
	{
		schema = new char[1];
		schema[0] = '\0';
	}
    delete info;
    return schema;

}

char* DbiConnection::GetDataSource()
{

    ParseInfo* info;

    info = parseConnectString (mConnectionString);

    char *datasource = NULL;
    if (NULL != info->mDataSource)
    {
        datasource = new char[strlen(info->mDataSource)+1];
        strcpy(datasource, info->mDataSource);
    }
	else
	{
		datasource = new char[1];
		datasource[0] = '\0';
	}
    delete info;
    return datasource;

}

DbiConnection::ParseInfo* DbiConnection::parseConnectString ( const wchar_t *connStr )
{
    wchar_t     *connection_string;
    wchar_t     *rover;
    wchar_t     *token;
    wchar_t     *start;
    int         size;
    char        *user = NULL;
    char        *password = NULL;
    char        *schema = NULL;
    char        *remainder;
    char        *datasource = NULL;
    char        *connectionStringProperty = NULL;
    char        *defaultGeometryWanted = NULL;
    char        *temp;
    ParseInfo   *ret;

    size = (int)((1 + wcslen (connStr)) * sizeof (wchar_t));
    connection_string = (wchar_t*)alloca(size*sizeof(wchar_t));
    memset (connection_string, 0, size);
    wcscpy (connection_string, connStr);
    rover = connection_string;
    user = NULL;
    password = NULL;
    schema = NULL;
    remainder = NULL;
#ifdef _WIN32
    while (NULL != (token = wcstok (rover, DELIMITER)))
#else
    wchar_t *state;
    while (NULL != (token = wcstok (rover, DELIMITER, &state)))
#endif
    {
        while (iswspace(*token))
            token++;
#ifdef _WIN32
        if (0 == _wcsnicmp (token, USERID, wcslen (USERID)))
#else
        if (0 == wcsncasecmp (token, USERID, wcslen (USERID)))
#endif
        {
            token += wcslen (USERID);
            if (NULL != (start = wcschr (token, SEPARATOR)))
            {
                start += 1;
                while (iswspace(*start))
                    start++;
                size = (int)wcslen (start) * 3;
                if (0 != size)
                {
                    user = (char*) alloca((size + 1)*sizeof(char));
                    memset (user, 0, size + 1);
                    ut_utf8_from_unicode( start, user, size );
                }
            }
        }
#ifdef _WIN32
        else if (0 == _wcsnicmp (token, USERID2, wcslen (USERID2)))
#else
        else if (0 == wcsncasecmp (token, USERID2, wcslen (USERID2)))
#endif
        {
            token += wcslen (USERID2);
            if (NULL != (start = wcschr (token, SEPARATOR)))
            {
                start += 1;
                while (iswspace(*start))
                    start++;
                size = (int)wcslen (start) * 3;
                if (0 != size)
                {
                    user = (char*) alloca((size + 1)*sizeof(char));
                    memset (user, 0, size + 1);
                    ut_utf8_from_unicode( start, user, size );
                }
            }
        }
#ifdef _WIN32
        else if (0 == _wcsnicmp (token, PASSWORD, wcslen (PASSWORD)))
#else
        else if (0 == wcsncasecmp (token, PASSWORD, wcslen (PASSWORD)))
#endif
        {
            token += wcslen (PASSWORD);
            if (NULL != (start = wcschr (token, SEPARATOR)))
            {
                start += 1;
                while (iswspace(*start))
                    start++;
                size = (int)wcslen (start) * 3;
                if (0 != size)
                {
                    password = (char*) alloca((size + 1)*sizeof(char));
                    memset (password, 0, size + 1);
                    ut_utf8_from_unicode( start, password, size );
                }
            }
        }
#ifdef _WIN32
        else if (0 == _wcsnicmp (token, SCHEMA, wcslen (SCHEMA)))
#else
        else if (0 == wcsncasecmp (token, SCHEMA, wcslen (SCHEMA)))
#endif
        {
            token += wcslen (SCHEMA);
            if (NULL != (start = wcschr (token, SEPARATOR)))
            {
                start += 1;
                while (iswspace(*start))
                    start++;
                size = (int)wcslen (start) * 3;
                if (0 != size)
                {
                    schema = (char*) alloca((size + 1)*sizeof(char));
                    memset (schema, 0, size + 1);
                    ut_utf8_from_unicode( start, schema, size );
                }
            }
        }
#ifdef _WIN32
        else if (0 == _wcsnicmp (token, DATASOURCE, wcslen (DATASOURCE)))
#else
        else if (0 == wcsncasecmp (token, DATASOURCE, wcslen (DATASOURCE)))
#endif
        {
            token += wcslen (DATASOURCE);
            if (NULL != (start = wcschr (token, SEPARATOR)))
            {
                start += 1;
                while (iswspace(*start))
                    start++;
                size = (int)wcslen (start) * 3;
                if (0 != size)
                {
                    datasource = (char*) alloca((size + 1)*sizeof(char));
                    memset (datasource, 0, size + 1);
                    ut_utf8_from_unicode( start, datasource, size );
                }
            }
        }
#ifdef _WIN32
        else if (0 == _wcsnicmp (token, DATASOURCE2, wcslen (DATASOURCE2)))
#else
        else if (0 == wcsncasecmp (token, DATASOURCE2, wcslen (DATASOURCE2)))
#endif
        {
            token += wcslen (DATASOURCE2);
            if (NULL != (start = wcschr (token, SEPARATOR)))
            {
                start += 1;
                while (iswspace(*start))
                    start++;
                size = (int)wcslen (start) * 3;
                if (0 != size)
                {
                    datasource = (char*) alloca((size + 1)*sizeof(char));
                    memset (datasource, 0, size + 1);
                    ut_utf8_from_unicode( start, datasource, size );
                }
            }
        }
#ifdef _WIN32
        else if (0 == _wcsnicmp (token, GENERATEDEFAULTGEOMETRYPROPERTY, wcslen (GENERATEDEFAULTGEOMETRYPROPERTY)))
#else
        else if (0 == wcsncasecmp (token, GENERATEDEFAULTGEOMETRYPROPERTY, wcslen (GENERATEDEFAULTGEOMETRYPROPERTY)))
#endif
        {
            token += wcslen (GENERATEDEFAULTGEOMETRYPROPERTY);
            if (NULL != (start = wcschr (token, SEPARATOR)))
            {
                start += 1;
                while (iswspace(*start))
                    start++;
                size = (int)wcslen (start) * 3;
                if (0 != size)
                {
                    defaultGeometryWanted = (char*) alloca((size + 1)*sizeof(char));
                    memset (defaultGeometryWanted, 0, size + 1);
                    ut_utf8_from_unicode( start, defaultGeometryWanted, size );
                }
            }
        }
#ifdef _WIN32
        else if (0 == _wcsnicmp (token, CONNECTIONSTRING, wcslen (CONNECTIONSTRING)))
#else
        else if (0 == wcsncasecmp (token, CONNECTIONSTRING, wcslen (CONNECTIONSTRING)))
#endif
        {
            // The string tokenizer does not deal well with alternate ways of encoding
            // our parameter values.  While the other types are immediately tokenized with
            // their values (e.g. UserName=Fred) and only require searching for the 
            // separator, ConnectionString uses double-quotes as delimiters, because it
            // can encode complex and multiple values inside it -- and they can have the 
            // same delimiter as our own parameters.
            //
            // We'll only use the tokenizer where it is safe to do so.  Any cases which 
            // do not require the tokenizer represent an unset ConnectionString, and are skipped.
            // The cases to consider are:
            //
            //      ConnectionString=       Completely empty value at end of string.
            //      ConnectionString=;      Completely empty value anywhere in string.
            //      ConnectionString=""     Quoted empty value at end of string.
            //      ConnectionString="";    Quoted empty value anywhere in string.
            //      ConnectionString="parm1=value1;parm2=value2;...";   Value to be tokenized.
            //      ConnectionString="parm1=value1;parm2=value2;..."    Value to be tokenized (end of string).
            //
            // The empty cases involve delimiter characters, which will be skipped by the next iteration of 
            // the main loop as it looks for the next parameter/value pair.  End-of-string cases are also
            // handled by the tokenizer in the next iteration.  We'll only use it to look for a closing
            // quote here.

            // First, peek ahead using the original string.
            size = (int) wcslen(token);
            FdoInt64 offset = (token+size) - connection_string;
            wchar_t nextChar1 = L'\0';
            wchar_t nextChar2 = L'\0';
            nextChar1 = connStr[offset];
            if (nextChar1 != L'\0')
                nextChar2 = connStr[offset+1];

            if (nextChar1 == DELIMITER2[0] && nextChar2 != DELIMITER2[0] && nextChar2 != L'\0')
            {
                // There is a non-empty value to parse with the tokenizer.
#ifdef _WIN32
                if (NULL != (token = wcstok (NULL, DELIMITER2)))
#else
                if (NULL != (token = wcstok (NULL, DELIMITER2, &state)))
#endif
                {
                    size = (int)wcslen (token) * 3;
                    if ( 0 != size ) 
                    {
                        connectionStringProperty = (char*) alloca((size + 1)*sizeof(char));
                        memset (connectionStringProperty, 0, size + 1);
                        ut_utf8_from_unicode( token, connectionStringProperty, size );
                    }
                }
            }
        }
        else
        {
            size = (int)wcslen (token) * 3;
            if (0 != size)
            {
                if (NULL != remainder)
                    size += (int)strlen (remainder) + 1;
                temp = (char*) alloca((size + 1)*sizeof(char));
                memset (temp, 0, size + 1);
                if (NULL != remainder)
                {
                    strcpy (temp, remainder);
                    strcat (temp, ";");
                    remainder = temp;
                    temp = remainder + strlen (remainder);
                }
                else
                    remainder = temp;
                ut_utf8_from_unicode( token, temp, size );
            }
        }
        rover = NULL;
    }

    ret = new ParseInfo (datasource, user, password, schema, connectionStringProperty, defaultGeometryWanted, remainder);
    return (ret);
}

DbiConnection::ParseInfo::ParseInfo (char *datasource, char *user, char *password, char *schema, char *connectionString, char * defaultGeometryWanted, char *remainder) :
    mDataSource (NULL),
    mUser (NULL),
    mPassword (NULL),
    mSchema (NULL),
    mConnectionStringProperty(NULL),
    mIsGeometryFromOrdinatesWanted(NULL),
    mRemainder (NULL)
{
    if (NULL != datasource)
    {
        mDataSource = new char[strlen (datasource) + 1];
        strcpy (mDataSource, datasource);
    }
    if (NULL != user)
    {
        mUser = new char[strlen (user) + 1];
        strcpy (mUser, user);
    }
    if (NULL != password)
    {
        mPassword = new char[strlen (password) + 1];
        strcpy (mPassword, password);
    }
    if (NULL != schema)
    {
        mSchema = new char[strlen (schema) + 1];
        strcpy (mSchema, schema);
    }
    if (NULL != connectionString)
    {
        mConnectionStringProperty = new char[strlen (connectionString) + 1];
        strcpy (mConnectionStringProperty, connectionString);
    }
    if (NULL != defaultGeometryWanted)
    {
        mIsGeometryFromOrdinatesWanted = new char[strlen (defaultGeometryWanted) + 1];
        strcpy (mIsGeometryFromOrdinatesWanted, defaultGeometryWanted);
    }
    if (NULL != remainder)
    {
        mRemainder = new char[strlen (remainder) + 1];
        strcpy (mRemainder, remainder);
    }
}

DbiConnection::ParseInfo::~ParseInfo ()
{
    if (NULL != mDataSource)
        delete [] (mDataSource);
    if (NULL != mUser)
        delete [] (mUser);
    if (NULL != mPassword)
        delete [] (mPassword);
    if (NULL != mSchema)
        delete [] (mSchema);
    if (NULL != mConnectionStringProperty)
        delete [] (mConnectionStringProperty);
    if (NULL != mIsGeometryFromOrdinatesWanted)
        delete [] (mIsGeometryFromOrdinatesWanted);
    if (NULL != mRemainder)
        delete [] (mRemainder);
}
/**
 * Perform a sql statement without going through the dbgql parser.
 * Copied from adb/do_sql.c
 * Return 0 if OK, non-zero is the Oracle error code.
 */
int DbiConnection::DoSql (char *sql_buf, int *rows_processed, bool bThrowException)
{

    SWITCH_CONTEXT
    int rc;
    int rdb_status;
    int     util_cursor;

    rc = RDBI_GENERIC_ERROR;
    rdb_status = RDBI_SUCCESS;
    *rows_processed = -1;
    rc = rdbi_est_cursor ( mContext, &util_cursor);
    rdb_status = rdbi_sql ( mContext, util_cursor, sql_buf);
    if (rdb_status == RDBI_SUCCESS)
        rdb_status = rdbi_execute ( mContext, util_cursor, 1, 0);
    if (rdb_status == RDBI_SUCCESS)
        *rows_processed = rdbi_crsr_nrows ( mContext, util_cursor);
    else
        rdbi_get_msg( mContext );

    (void)rdbi_fre_cursor ( mContext, util_cursor);
    if (rc!=RDBI_SUCCESS)
    {
        rdbi_get_msg( mContext );
        goto the_exit;
    }

the_exit:
    if (rdb_status == RDBI_SUCCESS)
        rdb_status = rc;
    else {
        if (bThrowException)
            THROW_DBI_EXCEPTION;
    }




    return (rdb_status);
}

 int DbiConnection::dbi_ver_dbase( char *db_name, int  *exists)
 {
     INVOKE_DBI_FUNC(dbi_ver_dbase( mContext, db_name, exists) );
 }

 long  DbiConnection::dbi_version_active_get(  )
 {
    // Can't use INVOKE_DBI_FUNC since 0 is a valid return value for this function.
    /*SWITCH_CONTEXT
    dbi_vn_id_t version = ::dbi_version_active_get(mContext);
    if( version < 0 ) THROW_DBI_EXCEPTION;

    return version;*/
     TODO
 }

 int DbiConnection::dbi_version_get_active_lt (char **active_lt)
 {
     INVOKE_DBI_FUNC( dbi_version_get_active_lt( mContext, active_lt ) );
 }


 int DbiConnection::dbi_bind(
                int gid,
                char *name,
                int datatype,
                int size,
                char *address,
                char *null_ind,
                int *ccode
            )
 {
     INVOKE_DBI_FUNC( dbi_bind(
                mContext,
                gid,
                name,
                datatype,
                size,
                address,
                (short *)null_ind,
                ccode ) );
 }


 int DbiConnection::dbi_col_in_tab(
                char *col_name,
                char *table_name,
                int *exists_flag
            )
 {
     INVOKE_DBI_FUNC( dbi_col_in_tab(
                mContext,
                col_name,
                table_name,
                exists_flag ) );
 }

 int DbiConnection::dbi_db_empty( int *db_empty )
 {
     INVOKE_DBI_FUNC( dbi_db_empty( mContext, db_empty ) );
 }

 int DbiConnection::dbi_deac_table( )
 {
     INVOKE_DBI_FUNC( dbi_deac_table(mContext ) );
 }

 int DbiConnection::dbi_define(
                int gid,
                char *name,
                int datatype,
                int size,
                char *address,
                char *null_ind,
                int *ccode
            )
 {
     INVOKE_DBI_FUNC( dbi_define(
                mContext,
                gid,
                name,
                datatype,
                size,
                address,
                null_ind,
                ccode ) );
 }

 int DbiConnection::dbi_desc_nslct( int gid, int *n )
 {
     INVOKE_DBI_FUNC( dbi_desc_nslct( mContext, gid, n ) );
 }

 int DbiConnection::dbi_execute( int gid,  int *rows_processed, int *ccode )
 {
     INVOKE_DBI_FUNC( dbi_execute( mContext, gid,  rows_processed, ccode ) );
 }

 int DbiConnection::dbi_fetch( int gid, int *ccode )
 {
     INVOKE_DBI_FUNC( dbi_fetch( mContext, gid, ccode ) );
 }



 int DbiConnection::dbi_flush(
            int gid,
            int *rows_processed,
            int *ccode
            )
 {
     INVOKE_DBI_FUNC( dbi_flush(
            mContext,
            gid,
            rows_processed,
            ccode
            ) );
 }

 // Only dbi_flush_all and dbi_free can be invoked after the connection is
 // closed. This may be called from cleanup code that get executed after
 // the connection is closed by the caller and cleanup code triggered by
 // a smart pointer going out of scope.
 int DbiConnection::dbi_flush_all( int *ccode )
 {
     if( mDbiContextId == -1 )
         return 1;
     INVOKE_DBI_FUNC( dbi_flush_all( mContext, ccode ) );
 }

 int DbiConnection::dbi_free( int gid, int *ccode )
 {
     if( mDbiContextId == -1 )
         return 1;
     INVOKE_DBI_FUNC( dbi_free( mContext, gid, ccode ) );
 }

 int DbiConnection::dbi_get_val_a(
            int gid,
            char *name,
            int ml,
            char *ascii,
            int  *isnull,
            int *ccode
            )
 {
     INVOKE_DBI_FUNC( dbi_get_val_a(
            mContext,
            gid,
            name,
            ml,
            ascii,
            isnull,
            ccode
            ) );
 }

 int DbiConnection::dbi_get_val_b(
            int gid,
            char *name,
            int ml,
            char *address,
            char *null_ind,
            int *ccode
            )
 {
     INVOKE_DBI_FUNC( dbi_get_val_b(
            mContext,
            gid,
            name,
            ml,
            address,
            null_ind,
            ccode
            ) );
 }

 int DbiConnection::dbi_gql( char *gql, int *gid )
 {
     INVOKE_DBI_FUNC( dbi_gql( mContext, gql, gid ) );
 }

 int DbiConnection::dbi_lock_act(char *lock_name,
                                 char *opened_by_conflict)
 {
     INVOKE_DBI_FUNC( dbi_lock_act( mContext,
                                    lock_name,
                                    opened_by_conflict ) );
 }

 int DbiConnection::dbi_lock_activate_lkd_query (char            *lock_owner,
                                                 char            *table_name,
                                                 dbi_lk_data_obj **lk_info)
 {
     INVOKE_DBI_FUNC( dbi_lock_activate_lkd_query ( mContext,
                                                    lock_owner,
                                                    table_name,
                                                    lk_info ));
 }

 int DbiConnection::dbi_lock_chk_lk_conf(char *table_name,
                                         char *sql_filter,
                                         char *current_user,
                                         int  operation_id,
                                         int  *conflict_counter)
 {
    INVOKE_DBI_FUNC( dbi_lock_chk_lk_conf ( mContext,
                                            table_name,
                                            sql_filter,
                                            current_user,
                                            operation_id,
                                            conflict_counter ) );
 }

 int DbiConnection::dbi_lock_create(char *lock_name,
                                    int  autogen,
                                    char *description,
                                    char *gen_lock_name,
                                    int  *exists)
 {
     INVOKE_DBI_FUNC( dbi_lock_create( mContext,
                                       lock_name,
                                       autogen,
                                       description,
                                       gen_lock_name,
                                       exists ) );
 }

 int DbiConnection::dbi_lock_deac()
 {
     INVOKE_DBI_FUNC( dbi_lock_deac(mContext) );
 }

 int DbiConnection::dbi_lock_def_conflict_init()
 {
     INVOKE_DBI_FUNC( dbi_lock_def_conflict_init(mContext) );
 }

 int DbiConnection::dbi_lock_del(char *lock_name,
                                 int  vers_act)
 {
    INVOKE_DBI_FUNC( dbi_lock_del( mContext,
                                   lock_name,
                                   vers_act ) );
 }

 int DbiConnection::dbi_lock_find_by_id(dbi_li_id_t       lock_id,
                                        dbi_lknm_spec_def *lock_spec,
                                        int               *found)
 {
    INVOKE_DBI_FUNC( dbi_lock_find_by_id( mContext,
                                          lock_id,
                                          lock_spec,
                                          found ) );
 }

 int DbiConnection::dbi_lock_get_lkd_query_data (dbi_lk_data_obj *lk_info,
                                                 int             *is_eof)
 {
   INVOKE_DBI_FUNC( dbi_lock_get_lkd_query_data ( mContext,
                                                  lk_info,
                                                  is_eof ));
 }

 int DbiConnection::dbi_lock_get_lock_info(char *table_name,
                                           char *sql_filter,
                                           char **sql_statement)
 {
    INVOKE_DBI_FUNC( dbi_lock_get_lock_info ( mContext,
                                              table_name,
                                              sql_filter,
                                              sql_statement ) );
 }

 int DbiConnection::dbi_lock_get_lk_types (int lock_types[],
                                           int array_size)
 {
    INVOKE_DBI_FUNC( dbi_lock_get_lk_types ( mContext,
                                             lock_types,
                                             array_size ));
 }

 int DbiConnection::dbi_lock_lock(char *table_name,
                                  char *sql_filter,
                                  int  lock_type,
                                  char *lock_conflicts_view_name,
                                  int  *err_code)
 {
    INVOKE_DBI_FUNC( dbi_lock_lock ( mContext,
                                     table_name,
                                     sql_filter,
                                     lock_type,
                                     lock_conflicts_view_name,
                                     err_code ) );
 }

 int DbiConnection::dbi_lock_terminate_lkd_query (dbi_lk_data_obj **lk_info)
 {
    INVOKE_DBI_FUNC( dbi_lock_terminate_lkd_query ( mContext,
                                                    lk_info) );
 }

 int DbiConnection::dbi_lock_transaction(char *table_name,
                                         char *sql_filter,
                                         int  do_sync_cm)
 {
    INVOKE_DBI_FUNC( dbi_lock_transaction ( mContext, table_name,
                                            sql_filter,
                                            do_sync_cm ) );
 }

 int DbiConnection::dbi_lock_unlock(char *table_name,
                                    char *sql_filter,
                                    int  is_admin_op,
                                    char *lock_conflicts_view_name)
 {
    INVOKE_DBI_FUNC( dbi_lock_unlock ( mContext,
                                       table_name,
                                       sql_filter,
                                       is_admin_op,
                                       lock_conflicts_view_name ) );
 }

 int DbiConnection::dbi_get_user_num()
 {
    // Can't use INVOKE_DBI_FUNC since the function returns a user number only.
    /*SWITCH_CONTEXT
    return (::dbi_get_user_num(mContext));*/
     TODO;
 }

 int DbiConnection::dbi_is_sys_tbl(char *table_name,
                                   int  *system_tbl)
 {
     INVOKE_DBI_FUNC( dbi_is_sys_tbl( mContext, table_name, system_tbl ) );
 }

void DbiConnection::dbi_set_nnull(
            char *null_ind,
            int start,
            int end
            )
 {
     /*SWITCH_CONTEXT
     return ::dbi_set_nnull(
            mContext,
            null_ind,
            start,
            end
            );*/
     ThrowNotSupportedError();
 }

 int DbiConnection::dbi_object_find(
            char *db_name,
            char *object_name,
            char  *dblink,
            int *found,
            char *object_type
            )
 {
     INVOKE_DBI_FUNC( dbi_object_find(
            mContext,
            db_name,
            object_name,
            dblink,
            found,
            object_type
            ) );
 }

 int DbiConnection::dbi_option( int gid, char *option, int *ccode )
 {
     INVOKE_DBI_FUNC( dbi_option(  mContext, gid, option, ccode ) );
 }

 int DbiConnection::dbi_tran_begin( char *tran_id )
 {
     INVOKE_DBI_FUNC( dbi_tran_begin( mContext, tran_id ) );
 }

 int DbiConnection::dbi_tran_end( char *tran_id )
 {
     INVOKE_DBI_FUNC( dbi_tran_end( mContext, tran_id ) );
 }

int DbiConnection::dbi_tran_rollback( )
{
    TODO;
/*
    SWITCH_CONTEXT

    int rc = ::dbi_tran_rollback(mContext);
    if( !rc ) THROW_DBI_EXCEPTION;

    try
    {
        // Metaschema changes are rolled back but Table and Column changes aren't
        // (Oracle does not roll back DDL changes ).
        // The following "rolls back" the Tables and Column changes by
        // re-synchronizing them with the Metaschema.
//TODO:     GetSchemaUtil()->GetSchemaManager()->SynchPhysical();
    }
    catch ( ... )
    {
        // Throw away synchronization errors. Rollback likely happened because of
        // an error so don't overshadow original error with this one.
    }
    return rc;
    */
}

int DbiConnection::dbi_index_add( char *index_name_in, char *table_name, char * *column_list, int ncol, int type, char *table_space, char *index_name_out )
{
    INVOKE_DBI_FUNC( dbi_index_add( mContext, index_name_in, table_name, column_list, ncol, type, table_space, index_name_out ) );
}

int DbiConnection::dbi_index_act(
    char            *lookup_key,
    int             ltype
)
{
    INVOKE_DBI_FUNC( dbi_index_act( mContext, lookup_key, ltype, NULL, 0 ) );
}

int DbiConnection::dbi_index_get(
    char            *index_name,
    char            *table_name,
    char            *column_name,
    int             *itype,
    int             *start,
    int             *eof
)
{
    INVOKE_DBI_FUNC( dbi_index_get( mContext, index_name, table_name, column_name, itype, start, eof ) );
}

int DbiConnection::dbi_index_deac()
{
    INVOKE_DBI_FUNC( dbi_index_deac(mContext) );
}

FdoStringP DbiConnection::GetDbSchemaName()
{
    return mDbSchemaName;
}

// the following will be replaced with the new FindClass method
const FdoSmLpSchema* DbiConnection::GetSchema(const wchar_t *className)
{
    return GetSchemaUtil()->GetSchema( className );
}

const wchar_t* DbiConnection::Property2ColName(const wchar_t *className, const wchar_t *attribute)
{
    return GetSchemaUtil()->Property2ColName(className, attribute);
}

const wchar_t *DbiConnection::ColName2Property(const wchar_t *className, const wchar_t *columnName)
{
    return GetSchemaUtil()->ColName2Property( className, columnName);
}

const wchar_t *DbiConnection::GetTable(const wchar_t *className)
{
    return GetSchemaUtil()->GetTable(className);
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_act_dbase()
{
    INVOKE_DBI_FUNC( dbi_act_dbase(mContext) );
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_get_dbase( char *dbname, int  *eof)
{
    INVOKE_DBI_FUNC( dbi_get_dbase( mContext, dbname, eof ) );
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_deac_dbase()
{
    INVOKE_DBI_FUNC( dbi_deac_dbase(mContext ) );
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_index_add_sptl( char *geom_tbl_name, char *geom_col_name, char *index_name, int is_rtree, int num_dims )
{
    INVOKE_DBI_FUNC( dbi_index_add_sptl( mContext, geom_tbl_name, geom_col_name, index_name, is_rtree, num_dims ) );
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_index_drop_sptl( char *geom_tbl_name, char *geom_col_name, char *index_name )
{
    INVOKE_DBI_FUNC( dbi_index_drop_sptl( mContext, geom_tbl_name, geom_col_name, index_name ) );
}


///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_activate_query (dbi_version_info_def lt_info,
                                               int                  query_request,
                                               int                  *qid)
{
    INVOKE_DBI_FUNC( dbi_version_activate_query (mContext,
                                                 lt_info,
                                                 query_request,
                                                 qid) );
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_get_query_data (dbi_version_info_def *lt_info,
                                               int                  *is_eof,
                                               int                  qid)
{
    INVOKE_DBI_FUNC( dbi_version_get_query_data (mContext,
                                                 lt_info,
                                                 is_eof,
                                                 qid) );
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_terminate_query (int qid)
{
    INVOKE_DBI_FUNC( dbi_version_terminate_query (mContext,qid) );
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_active_set( dbi_version_info_def *version_info,
                                           int                  *err_code  )
{
    INVOKE_DBI_FUNC( dbi_version_active_set( mContext, version_info, err_code ) );
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_add (dbi_version_info_def *version_info,
                                    int                  ignore_dup,
                                    int                  *err_code)
{
    INVOKE_DBI_FUNC( dbi_version_add( mContext,
                                      version_info,
                                      ignore_dup,
                                      err_code ));
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_commit(char *lt_names,
                                      int  len,
                                      int  conflict_flag,
                                      char *tran_id,
                                      int  *err_code)
{
    INVOKE_DBI_FUNC( dbi_version_commit( mContext,
                                         lt_names,
                                         len,
                                         conflict_flag,
                                         tran_id,
                                         err_code ));
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_load_conflicts (char                 *lt_name,
                                               dbi_tab_cfl_list_obj **cfl_root,
                                               int                  *err_code)
{
    INVOKE_DBI_FUNC( dbi_version_load_conflicts (mContext,
                                                 lt_name,
                                                 cfl_root,
                                                 err_code ));
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_clear_conflicts (dbi_tab_cfl_list_obj **cfl_root)
{
    INVOKE_DBI_FUNC( dbi_version_clear_conflicts ( mContext, cfl_root ));
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_process_conflicts (char                 *lt_name,
                                                  dbi_tab_cfl_list_obj **cfl_root,
                                                  int                  *err_code)
{
    INVOKE_DBI_FUNC( dbi_version_process_conflicts ( mContext,
                                                     lt_name,
                                                     cfl_root,
                                                     err_code ));
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_rollback (char *lt_names,
                                         int  len,
                                         int  conflict_flag,
                                         char *tran_id,
                                         int  *err_code)
{
    INVOKE_DBI_FUNC( dbi_version_rollback ( mContext,
                                            lt_names,
                                            len,
                                            conflict_flag,
                                            tran_id,
                                            err_code ));
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_add_depend( dbi_vn_id_t parent_version )
{
    INVOKE_DBI_FUNC( dbi_version_add_depend( mContext, parent_version ));
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_sel_set_add( long *vers_list, int list_len )
{
    INVOKE_DBI_FUNC( dbi_version_sel_set_add( mContext, vers_list, list_len ));
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_ws_disable (char *table_name,
                                           int  force_discard,
                                           int  ignore_last_error,
                                           int  is_topology)
{
    INVOKE_DBI_FUNC( dbi_version_ws_disable( mContext,
                                             table_name,
                                             force_discard,
                                             ignore_last_error,
                                             is_topology));
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_ws_disable_all ()
{
    INVOKE_DBI_FUNC( dbi_version_ws_disable_all(mContext));
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_ws_enable (char *table_name,
                                          char *history_option,
                                          int  is_topology)
{
    INVOKE_DBI_FUNC( dbi_version_ws_enable (mContext,
                                            table_name,
                                            history_option,
                                            is_topology));
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_version_ws_is_enabled (char *owner,
                                              char *table_name,
                                              int  *is_version_enabled)
{
    INVOKE_DBI_FUNC( dbi_version_ws_is_enabled (mContext,
                                                owner,
                                                table_name,
                                                is_version_enabled));
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_stats_gather( char *geom_tbl_name )
{
    INVOKE_DBI_FUNC( dbi_stats_gather( mContext, geom_tbl_name ));
}


///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_lob_create_ref( int sqlid, void **lob_ref )
{
    INVOKE_DBI_FUNC( dbi_lob_create_ref( mContext, sqlid, lob_ref ) );
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_lob_destroy_ref( int sqlid, void *lob_ref )
{
    INVOKE_DBI_FUNC( dbi_lob_destroy_ref( mContext, sqlid, lob_ref ) );
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_lob_get_size( int sqlid, void *lob_ref, unsigned int *size )
{
    INVOKE_DBI_FUNC( dbi_lob_get_size( mContext, sqlid, lob_ref, size ));
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_lob_write_next( int sqlid, void *lob_ref, int lob_type, unsigned int chunk_size, char* chunk )
{
    INVOKE_DBI_FUNC( dbi_lob_write_next( mContext, sqlid, lob_ref, lob_type, chunk_size, chunk ));
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_lob_read_next( int sqlid, void *lob_ref, int lob_type, unsigned int chunk_size, char *chunk, unsigned int *chunk_size_out, int *eol )
{
    INVOKE_DBI_FUNC( dbi_lob_read_next( mContext, sqlid, lob_ref, lob_type, chunk_size, chunk, chunk_size_out, eol ));
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_lob_open( int sqlid, void *lob_ref, int for_read )
{
    INVOKE_DBI_FUNC( dbi_lob_open( mContext, sqlid, lob_ref, for_read ) );
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_lob_close( int sqlid, void *lob_ref )
{
    INVOKE_DBI_FUNC( dbi_lob_close( mContext, sqlid, lob_ref ) );
}

///////////////////////////////////////////////////////////////////////////////
int DbiConnection::dbi_csys_get_wkt( long srid, char *wkt )
{
    INVOKE_DBI_FUNC( dbi_csys_get_wkt( mContext, srid, wkt ) );
}

int DbiConnection::GetNextIndex()
{
    mIndex++;
    return mIndex;
}

int DbiConnection::dbi_set_lt_method(int lt_method)
{
    if ( mOpen == FdoConnectionState_Open ) {
        INVOKE_DBI_FUNC( dbi_set_lt_method(
            mContext,
            (lt_method == -1) ? DBI_WORKSPACE_MANAGER_LT_METHOD : lt_method
        ));
    }

    return(1);
}

int DbiConnection::dbi_col_has_values(char *table, char *column, int *has_values)
{
    INVOKE_DBI_FUNC( dbi_col_has_values(mContext, table, column, has_values) );
}

void DbiConnection::SetSchemaManager( FdoSchemaManagerP schemaManager )
{
    FdoSmPhMgrP phMgr = schemaManager->GetPhysicalSchema();
    FdoSmPhGrdMgr* phRdbmsMgr = (FdoSmPhGrdMgr*)(FdoSmPhMgr*) phMgr;

    phRdbmsMgr->SetRdbiContext( mContext );

    GetSchemaUtil()->SetSchemaManager(schemaManager);
}

FdoSchemaManagerP DbiConnection::GetSchemaManager()
{
    return GetSchemaUtil()->GetSchemaManager();
}

