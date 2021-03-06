/*
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
 */

#include <Inc/rdbi.h>					/* rdbi status values		*/
#include <Inc/ut.h>							/* ut_vm_malloc()			*/
#include <Inc/debugext.h>
#include "proto_p.h"
#include <time.h>
#ifdef _WIN32
#include "odbcss.h"
#else
#include <sqlucode.h>
#endif

#include <assert.h>

/************************************************************************
* Name																	*
*	odbcdr_execute_direct - Execute direct the SQL statement			*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	int odbcdr_execute_direct(cursor, count, offset, rows_processed)	*
*   odbcdr_context_def  *context,										*
*	rdbi_string_def	  *sql,												*
*	int    *rows_processed	 											*
*																		*
* Description															*
*		Execute  the  SQL  statement  using SQLExecDirect(). This		*
*		executes an sql statement with no prior cursor allocation		*
*		and statement preparation. It is the fastest way of executing	*
*		one-time statements. Another advantage is that the statement	*
*		is not built into a temporary procedure like a prepared			*
*		statement and therefore can be used for setting query			*
*		execution options or creating temprary tables.					*
*		Also, SQLExecDirect is good for executing DDL					*
*		statements and stored procedures.								*
*																		*
*
************************************************************************/

int local_odbcdr_execute_direct(
    odbcdr_context_def  *context,
	rdbi_string_def     *sql,
	int    *rows_processed
	)
{
	odbcdr_connData_def		*connData;
	SQLRETURN				rc;
	SQLHSTMT                hStmt = NULL;
	int 					rdbi_status = RDBI_GENERIC_ERROR;

	debug_on("odbcdr_execute_direct");

	*rows_processed = 0;
	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );
	SQLAllocHandle(SQL_HANDLE_STMT,	connData->hDbc, &hStmt);
    if (context->odbcdr_UseUnicode)
        rc = SQLExecDirectW(hStmt, (SQLWCHAR*)sql->cwString, SQL_NTSL);
    else
        rc = SQLExecDirect(hStmt, (SQLCHAR*)sql->ccString, SQL_NTSL);

	if ( rc != ODBCDR_SUCCESS &&
		rc != SQL_SUCCESS_WITH_INFO &&
		rc != SQL_NO_DATA )
	{
		debug0( "SQLExecute FAILED!" );
		rdbi_status = odbcdr_xlt_status( context, rc, SQL_HANDLE_STMT, hStmt);
		context->odbcdr_last_rc = rc;
		goto the_exit;
	}
	else
	{
		SQLRowCount(hStmt, (SQLLEN*)rows_processed);
        rdbi_status = RDBI_SUCCESS;
		debug0( "SQLExecuteDirect." );
	}

the_exit:
    if ( hStmt ) 
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	debug_return(NULL, rdbi_status );
}

int odbcdr_execute_direct(
    odbcdr_context_def  *context,
	const char	  *sql,
	int    *rows_processed
	)
{
    rdbi_string_def str;
    str.ccString = sql;
    return local_odbcdr_execute_direct(context, &str, rows_processed);
}

int odbcdr_execute_directW(
    odbcdr_context_def  *context,
	const wchar_t   *sql,
	int    *rows_processed
	)
{
    rdbi_string_def str;
    str.cwString = sql;
    return local_odbcdr_execute_direct(context, &str, rows_processed);
}
