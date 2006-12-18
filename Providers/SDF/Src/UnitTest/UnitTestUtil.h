 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
 // 
 // This library is free software; you can redistribute it and/or
 // modify it under the terms of version 2.1 of the GNU Lesser
 // General Public License as published by the Free Software Foundation.
 // 
 // This library is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 // Lesser General Public License for more details.
 // 
 // You should have received a copy of the GNU Lesser General Public
 // License along with this library; if not, write to the Free Software
 // Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#ifdef _WIN32
#pragma once
#endif

#ifndef CPP_UNIT_UNITTESTUTIL_H
#define CPP_UNIT_UNITTESTUTIL_H

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <Fdo.h>
#include <TestCommonMiscUtil.h>


/* 
 * A test case for ApplySchemaCommand.
 *
 * Loads some schemas into a database.
 */

class UnitTestUtil 
{

public:
    static FdoIConnection* OpenConnection( FdoString* fileName, bool re_create );

    static void ExportDb( 
        FdoIConnection* connection, 
        FdoIoStream* stream, 
        FdoXmlSpatialContextFlags* flags = NULL
    );

//    void DeleteSchema( FdoIConnection* connection, GisString* schemaName );
    
    // Fail the current Unit Test, showing the given exception's message.
    static void FailOnException( FdoException* e );

    static void Exception2String( FdoException* e, char* buffer );

    // Print an exception, and all it's cause exceptions to a file
    // or stdout by default.
    static void PrintException( FdoException* e, FILE* fp = stdout, FdoBoolean stripLineNo = false );

    // Print an exception, and all it's cause exceptions to the given file.
    static void PrintException( FdoException* e, const char* fileName, FdoBoolean stripLineNo = false );

    // Compare an output file against its master (expected) copy.
    // Fail if they are different.
    static void CheckOutput( const char* masterFileName, const char* outFileName );

    // Compare to files.
    //
    // Returns:
    //      0 - they are the same
    //      -1 - they are different.
    static int CompareFiles( const char* file1Name, const char* file2Name );

};

#endif
