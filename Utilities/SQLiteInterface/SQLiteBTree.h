#ifndef _SQLITEBTREE_H
#define _SQLITEBTREE_H
// 
//  Copyright (C) 2004-2005  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#ifdef _WIN32
#pragma once
#endif

class SQLiteCursor;
struct Btree;

class SQLiteBTree
{
private:
    Btree       *m_pBt;
    bool        m_bOwner;


public:
    SQLiteBTree(void);

    SQLiteBTree( Btree *bt );

    ~SQLiteBTree(void);

    /*
    ** Open a new database
    */
    int open( const char* fileName, int nCache, int   flags );

    /*
    ** Close the given database.
    */
    int close();

    /*
    ** Create a new table in the database
    */
    int create_table( int flags, int *tableId );

	/*
    ** Drop an existing table in the database
    */
    int drop_table( int tableId );

    /*
    **
    ** Remove all entries from the given table but keep the table around.
    */
    int clear_table( int tableId );

    /*
    ** Create a new cursor.  Return the ID for the cursor.
    */
    int cursor( int iTable, SQLiteCursor **pCur_out, int wrFlag );
};

#endif

