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

/************************************************************************
* Name                                                                  *
*   context.h														*
*                                                                       *
* Synopsis                                                              *
*   #include "context.h"												*
*                                                                       *
************************************************************************/
#ifndef ODBCDR_CONTEXT_H
#define ODBCDR_CONTEXT_H    1
#if defined(_MSC_VER)
#include <windows.h>
#endif
#include <Inc/rdbi.h>				/* RDBI_MAX_CONNECTS	*/
#include <sql.h>
#include <sqlext.h>

#include "structs.h"

typedef struct
{
    char name[ODBCDR_MAX_BUFF_SIZE];
    char type;
} odbcdr_NameListEntry_obj_def;

typedef struct
{
    char name[ODBCDR_MAX_BUFF_SIZE];
} odbcdr_NameListEntry_user_def;

typedef struct
{
    char name[ODBCDR_MAX_BUFF_SIZE];
} odbcdr_NameListEntry_store_def;

typedef struct
{
    char name[ODBCDR_MAX_BUFF_SIZE];
} odbcdr_NameListEntry_pkey_def;

typedef struct
{
    char name[ODBCDR_MAX_BUFF_SIZE];
    char type[ODBCDR_MAX_BUFF_SIZE];
	int  length;
	int  scale;
	int  isnullable;
    int  isautogenerated;
	int  position;
} odbcdr_NameListEntry_col_def;

typedef struct _odbcdr_context_def
{

/************************************************************************
*  RDBMS session-related structures and globals 						*
************************************************************************/
    odbcdr_environment_def odbcdr_env; 

/************************************************************************
*  RDBMS connect-related structures and globals 						*
************************************************************************/

    long	odbcdr_connect_count; 
    long	odbcdr_current_connect; /* Index into odbcdr_lda */

    int     odbcdr_ddlexec; 
    int     odbcdr_ccache_size; 

    odbcdr_connData_def *odbcdr_conns[RDBI_MAX_CONNECTS];	/* Login data areas 	*/

    char	odbcdr_automatic_logon_user[2]; 	/* For default logon	*/

    long	odbcdr_rowid;						/* g_rowid bind value */


/************************************************************************
*  We keep track of the last ODBC return code returned so we can		*
*  return the associated message on request.							*
************************************************************************/

    short   odbcdr_last_rc; 
	char	odbcdr_last_err_msg[ODBCDR_MAX_BUFF_SIZE];

	long	odbcdr_nameListNextPosition_cols;
	long	odbcdr_nameListNextPosition_pkeys;
	long	odbcdr_nameListNextPosition_objs;
	long	odbcdr_nameListNextPosition_stores;
	long	odbcdr_nameListNextPosition_users;

	bool	odbcdr_singletonNameInUse;
	bool	odbcdr_singletonNameReturned;

	ut_da_def odbcdr_nameList_objs;
	ut_da_def odbcdr_nameList_users;
	ut_da_def odbcdr_nameList_cols;
	ut_da_def odbcdr_nameList_stores;
	ut_da_def odbcdr_nameList_pkeys;

	/* Here is a small bit of re-entrance (in addition to per-connection thread-safety) for datastores.
	 * Sometimes, between calls to get the next item in a list, another query comes in for a specific 
	 * target (to verify its existence). We'll store a singleton query result separately 
	 * from the main list. This assumes that singleton queries are strictly nested
	 * inside full queries (not interleaved).
	 */
	odbcdr_NameListEntry_obj_def	odbcdr_singletonName_objects;

} odbcdr_context_def;


/************************************************************************
 *  Macros to temporarily cache the error code and message      		*
 ************************************************************************/
#define ODBCDR_ERRORINFO_VARS  short __odbcdr_last_rc; char __odbcdr_last_err_msg[ODBCDR_MAX_BUFF_SIZE]; __odbcdr_last_err_msg[0]='\0';
#define ODBCDR_ERRORINFO_GET(context) __odbcdr_last_rc=context->odbcdr_last_rc; strcpy(__odbcdr_last_err_msg,context->odbcdr_last_err_msg);
#define ODBCDR_ERRORINFO_SET(context) context->odbcdr_last_rc=__odbcdr_last_rc; strcpy(context->odbcdr_last_err_msg,__odbcdr_last_err_msg);

#endif

