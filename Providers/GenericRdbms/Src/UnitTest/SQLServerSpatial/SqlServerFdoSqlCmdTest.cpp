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
 *
 */

#include "Pch.h"
#include "SqlServerFdoSqlCmdTest.h"
#include "UnitTestUtil.h"
#ifndef _WIN32
#include <stdlib.h>
#endif

#define  SQLCMD_8BIT_TABLE_NAME  L"Fd�SqlTest"

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoSqlCmdTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoSqlCmdTest, "FdoSqlCmdTest");

void SqlServerFdoSqlCmdTest::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
}

void SqlServerFdoSqlCmdTest::CreateAndDrop8bitTable ()
{
    FdoISQLCommand *sqlCmd = NULL;

    if( mConnection != NULL )
    {
        try
        {
            sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );

            wchar_t *number_type = get_number_type();
            FdoStringP sql = FdoStringP::Format(L"create table %ls (Name varchar(64) not null, id %ls(10,0), amount %ls(38,8), bool_val %ls(1), date_val %ls )", 
                                SQLCMD_8BIT_TABLE_NAME, number_type, number_type, number_type, get_datetime_type());
            sqlCmd->SetSQLStatement( sql );
            sqlCmd->ExecuteNonQuery();

            if( sqlCmd)
                sqlCmd->Release();

            sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
            sql = FdoStringP::Format(L"drop table %ls", SQLCMD_8BIT_TABLE_NAME);
            sqlCmd->SetSQLStatement( (FdoString *)sql );
            sqlCmd->ExecuteNonQuery();

            if( sqlCmd)
                sqlCmd->Release();
        }
        catch( FdoException *ex )
        {
            if( sqlCmd)
                sqlCmd->Release();

			CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
            throw;
        }
    }

}

FdoString *SqlServerFdoSqlCmdTest::GetGeometrySelectStatement()
{
	return L"select geometry from acdb3dpolyline";
}

void SqlServerFdoSqlCmdTest::TestOutParamsStoreProcRetOnly()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        try
        {
            sqlCmd->SetSQLStatement( L"DROP PROCEDURE MyRandProc;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}

        sqlCmd->SetSQLStatement( L"CREATE PROCEDURE MyRandProc\nAS\nBEGIN\ndeclare @value bigint\nSET NOCOUNT ON;\nset @value = 100*rand();\nreturn (@value);\nEND" );
        sqlCmd->ExecuteNonQuery();

        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();

        FdoPtr<FdoInt32Value> idVal = FdoInt32Value::Create(0);
        FdoPtr<FdoParameterValue> pval = FdoParameterValue::Create(L"RetVal", idVal);
        pval->SetDirection(FdoParameterDirection_Output);
        params->Add(pval);

        sqlCmd->SetSQLStatement( L"MyRandProc" );
        int randVal = sqlCmd->ExecuteNonQuery();
        printf("Test1 RandVal = %d", randVal);
        CPPUNIT_ASSERT( idVal->GetInt32() == randVal);

        params->Clear();
        sqlCmd->SetSQLStatement( L"DROP PROCEDURE MyRandProc;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

void SqlServerFdoSqlCmdTest::TestOutParamsStoreProcRetAndIn()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        try
        {
            sqlCmd->SetSQLStatement( L"DROP PROCEDURE MyRandProc;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}

        sqlCmd->SetSQLStatement( L"CREATE PROCEDURE MyRandProc @initval bigint = 0\nAS\nBEGIN\ndeclare @value bigint\nSET NOCOUNT ON;\nset @value = 100*rand()+@initval;\nreturn (@value);\nEND" );
        sqlCmd->ExecuteNonQuery();

        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();

        FdoPtr<FdoInt32Value> idVal = FdoInt32Value::Create(0);
        FdoPtr<FdoParameterValue> pval = FdoParameterValue::Create(L"RetVal", idVal);
        pval->SetDirection(FdoParameterDirection_Output);
        params->Add(pval);
        
        FdoPtr<FdoInt32Value> initVal = FdoInt32Value::Create(300);
        FdoPtr<FdoParameterValue> pInitVal = FdoParameterValue::Create(L"InitVal", initVal);
        pInitVal->SetDirection(FdoParameterDirection_Input);
        params->Add(pInitVal);

        sqlCmd->SetSQLStatement( L"MyRandProc(:InitVal)" );
        int randVal = sqlCmd->ExecuteNonQuery();
        printf("Test1 RandVal = %d", randVal);
        CPPUNIT_ASSERT( idVal->GetInt32() == randVal);
        
        params->Clear();
        sqlCmd->SetSQLStatement( L"DROP PROCEDURE MyRandProc;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

void SqlServerFdoSqlCmdTest::TestOutParamsStoreProcRetAndOut()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        try
        {
            sqlCmd->SetSQLStatement( L"DROP PROCEDURE MyRandProc;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}

        sqlCmd->SetSQLStatement( L"CREATE PROCEDURE MyRandProc @value nvarchar(20) OUT\nAS\nBEGIN\nSET NOCOUNT ON;\nset @value = 'MyTest';\nreturn 11;\nEND" );
        sqlCmd->ExecuteNonQuery();

        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();

        FdoPtr<FdoInt32Value> idVal = FdoInt32Value::Create(0);
        FdoPtr<FdoParameterValue> pval = FdoParameterValue::Create(L"RetVal", idVal);
        pval->SetDirection(FdoParameterDirection_Output);
        params->Add(pval);
        
        FdoPtr<FdoStringValue> strVal = FdoStringValue::Create(L"");
        FdoPtr<FdoParameterValue> parStrVal = FdoParameterValue::Create(L"StringVal", strVal);
        parStrVal->SetDirection(FdoParameterDirection_Output);
        params->Add(parStrVal);

        sqlCmd->SetSQLStatement( L"MyRandProc(:StringVal)" );

        FdoPtr<FdoISQLDataReader> myReader = sqlCmd->ExecuteReader();
        int colCnt = myReader->GetColumnCount();
        int rIdx = myReader->GetColumnIndex(L"RetVal");
        int sIdx = myReader->GetColumnIndex(L"StringVal");

        while(myReader->ReadNext())
        {
            FdoInt32 cpRet = myReader->GetInt32(rIdx);
            FdoString* cStrRet;
            CPPUNIT_ASSERT(!myReader->IsNull(sIdx));
            if (myReader->IsNull(sIdx))
                cStrRet = L"NULL";
            else
                cStrRet = myReader->GetString(sIdx);
            printf ("RetValue[%d] = '%ls'\n", cpRet, cStrRet);
            CPPUNIT_ASSERT( idVal->GetInt32() == cpRet);
        }
        myReader->Close();
        
        params->Clear();
        sqlCmd->SetSQLStatement( L"DROP PROCEDURE MyRandProc;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

void SqlServerFdoSqlCmdTest::TestOutParamsStoreProcRetAndNullOut()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        try
        {
            sqlCmd->SetSQLStatement( L"DROP PROCEDURE MyRandProc;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}

        sqlCmd->SetSQLStatement( L"CREATE PROCEDURE MyRandProc @value nvarchar(20) OUT\nAS\nBEGIN\nSET NOCOUNT ON;\nset @value = null;\nreturn 11;\nEND" );
        sqlCmd->ExecuteNonQuery();

        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();

        FdoPtr<FdoInt32Value> idVal = FdoInt32Value::Create(0);
        FdoPtr<FdoParameterValue> pval = FdoParameterValue::Create(L"RetVal", idVal);
        pval->SetDirection(FdoParameterDirection_Output);
        params->Add(pval);
        
        FdoPtr<FdoStringValue> strVal = FdoStringValue::Create(L"");
        FdoPtr<FdoParameterValue> parStrVal = FdoParameterValue::Create(L"StringVal", strVal);
        parStrVal->SetDirection(FdoParameterDirection_Output);
        params->Add(parStrVal);

        sqlCmd->SetSQLStatement( L"MyRandProc(:StringVal)" );

        FdoPtr<FdoISQLDataReader> myReader = sqlCmd->ExecuteReader();
        int colCnt = myReader->GetColumnCount();
        int rIdx = myReader->GetColumnIndex(L"RetVal");
        int sIdx = myReader->GetColumnIndex(L"StringVal");

        while(myReader->ReadNext())
        {
            FdoInt32 cpRet = myReader->GetInt32(rIdx);
            FdoString* cStrRet;
            CPPUNIT_ASSERT(myReader->IsNull(sIdx));
            if (myReader->IsNull(sIdx))
                cStrRet = L"NULL";
            else
                cStrRet = myReader->GetString(sIdx);
            printf ("RetValue[%d] = '%ls'\n", cpRet, cStrRet);
            CPPUNIT_ASSERT( idVal->GetInt32() == cpRet);
        }
        myReader->Close();
        
        params->Clear();
        sqlCmd->SetSQLStatement( L"DROP PROCEDURE MyRandProc;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

void SqlServerFdoSqlCmdTest::TestOutParamsStoreProcRetAndInAndOut()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        try
        {
            sqlCmd->SetSQLStatement( L"DROP PROCEDURE MyRandProc;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}

        sqlCmd->SetSQLStatement( L"CREATE PROCEDURE MyRandProc @initval bigint = 0, @value nvarchar(20) OUT\nAS\nBEGIN\nSET NOCOUNT ON;\nset @value = 'MyTest';\nreturn 11+@initval;\nEND" );
        sqlCmd->ExecuteNonQuery();

        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();

        FdoPtr<FdoInt32Value> idVal = FdoInt32Value::Create(0);
        FdoPtr<FdoParameterValue> pval = FdoParameterValue::Create(L"RetVal", idVal);
        pval->SetDirection(FdoParameterDirection_Output);
        params->Add(pval);
        
        FdoPtr<FdoInt32Value> initVal = FdoInt32Value::Create(300);
        FdoPtr<FdoParameterValue> pInitVal = FdoParameterValue::Create(L"InitVal", initVal);
        pInitVal->SetDirection(FdoParameterDirection_Input);
        params->Add(pInitVal);

        FdoPtr<FdoStringValue> strVal = FdoStringValue::Create(L"");
        FdoPtr<FdoParameterValue> parStrVal = FdoParameterValue::Create(L"StringVal", strVal);
        parStrVal->SetDirection(FdoParameterDirection_Output);
        params->Add(parStrVal);

        sqlCmd->SetSQLStatement( L"MyRandProc(:InitVal, :StringVal)" );

        FdoPtr<FdoISQLDataReader> myReader = sqlCmd->ExecuteReader();
        int colCnt = myReader->GetColumnCount();
        int rIdx = myReader->GetColumnIndex(L"RetVal");
        int sIdx = myReader->GetColumnIndex(L"StringVal");

        while(myReader->ReadNext())
        {
            FdoInt32 cpRet = myReader->GetInt32(rIdx);
            FdoString* cStrRet = myReader->GetString(sIdx);
            printf ("RetValue[%d] = '%ls'\n", cpRet, cStrRet);
            CPPUNIT_ASSERT( idVal->GetInt32() == cpRet);
        }
        myReader->Close();
        
        params->Clear();
        sqlCmd->SetSQLStatement( L"DROP PROCEDURE MyRandProc;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

void SqlServerFdoSqlCmdTest::TestParamInGeom()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        try
        {
            sqlCmd->SetSQLStatement( L"DROP TABLE testTable;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}

        sqlCmd->SetSQLStatement( L"CREATE TABLE testTable(ID INT IDENTITY(1,1) NOT NULL, G GEOMETRY);" );
        sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement( L"INSERT INTO testTable (G) VALUES ('LINESTRING (4 2, 5 3)');" );
        sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement( L"INSERT INTO testTable (G) VALUES ('POLYGON ((0 0, 3 0, 3 3, 0 3, 0 0), (1 1, 1 2, 2 2, 2 1, 1 1))');" );
        sqlCmd->ExecuteNonQuery();

        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"G INSIDE GeomFromText('LINESTRING (3.5 0, 1 4)')");
        FdoSpatialCondition* spFilter = static_cast<FdoSpatialCondition*>(filter.p);
        FdoPtr<FdoGeometryValue> gVal = static_cast<FdoGeometryValue*>(spFilter->GetGeometry());
        FdoPtr<FdoParameterValue> pval = FdoParameterValue::Create(L"gVal", gVal);
        pval->SetDirection(FdoParameterDirection_Input);
        params->Add(pval);
        
        FdoPtr<FdoInt32Value> sridVal = FdoInt32Value::Create(0);
        FdoPtr<FdoParameterValue> srval = FdoParameterValue::Create(L"srid", sridVal);
        pval->SetDirection(FdoParameterDirection_Input);
        params->Add(srval);

        sqlCmd->SetSQLStatement( L"SELECT * FROM testTable WHERE G.STIntersects(:gVal)=1" );
        FdoPtr<FdoISQLDataReader> myReader = sqlCmd->ExecuteReader();
        int colCnt = myReader->GetColumnCount();
        int gIdx = myReader->GetColumnIndex(L"G");

        int cnt = 0;
        while(myReader->ReadNext())
        {
            FdoPtr<FdoByteArray> geom;
            if (!myReader->IsNull(gIdx))
                geom = myReader->GetGeometry(gIdx);
            cnt++;
        }
        myReader->Close();

        CPPUNIT_ASSERT( cnt == 1);

        params->Clear();

        sqlCmd->SetSQLStatement( L"DROP TABLE testTable;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

void SqlServerFdoSqlCmdTest::TestBindNotNullBlob()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        try
        {
            sqlCmd->SetSQLStatement( L"DROP TABLE testTable;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}
        
        sqlCmd->SetSQLStatement( L"CREATE TABLE testTable ([ID] [int] IDENTITY(1,1) NOT NULL, [G] [geometry] NULL, [vBlob] [varbinary](max) NULL)" );
        sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement( L"insert into testTable (vBlob) values (:bval);" );
        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();
        
        FdoInt64 int64Val = 0xDE0B6B3A763FFFF;
        FdoPtr<FdoByteArray> ba = FdoByteArray::Create((FdoByte*)&int64Val, sizeof(int64Val));
        FdoPtr<FdoBLOBValue> bVal = FdoBLOBValue::Create(ba);
        FdoPtr<FdoParameterValue> pParVal = FdoParameterValue::Create(L"bval", bVal);
        pParVal->SetDirection(FdoParameterDirection_Input);
        params->Add(pParVal);

        int val = sqlCmd->ExecuteNonQuery();

        params->Clear();
        sqlCmd->SetSQLStatement( L"select vBlob from testTable where ID=(select max(ID) from testTable);" );
        FdoPtr<FdoISQLDataReader> myReader = sqlCmd->ExecuteReader();
        int colCnt = myReader->GetColumnCount();
        int gIdx = myReader->GetColumnIndex(L"vBlob");
        FdoDataType dtype = myReader->GetColumnType(gIdx);

        int cnt = 0;
        while(myReader->ReadNext())
        {
            CPPUNIT_ASSERT(!myReader->IsNull(gIdx));
            FdoPtr<FdoLOBValue> blb = myReader->GetLOB(gIdx);
            FdoPtr<FdoByteArray> barr = blb->GetData();
            CPPUNIT_ASSERT(barr->GetCount() == ba->GetCount());
            CPPUNIT_ASSERT(*(FdoInt64*)barr->GetData() == *(FdoInt64*)ba->GetData());
            cnt++;
        }
        myReader->Close();

        CPPUNIT_ASSERT(cnt == 1);

        params->Clear();
        sqlCmd->SetSQLStatement( L"DROP TABLE testTable;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

void SqlServerFdoSqlCmdTest::TestBindNullBlob()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        
        try
        {
            sqlCmd->SetSQLStatement( L"DROP TABLE testTable;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}
        
        sqlCmd->SetSQLStatement( L"CREATE TABLE testTable ([ID] [int] IDENTITY(1,1) NOT NULL, [G] [geometry] NULL, [vBlob] [varbinary](max) NULL)" );
        sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement( L"insert into testTable (vBlob) values (:bval);" );
        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();
        
        FdoPtr<FdoBLOBValue> bVal = FdoBLOBValue::Create();
        bVal->SetNull();
        FdoPtr<FdoParameterValue> pParVal = FdoParameterValue::Create(L"bval", bVal);
        pParVal->SetDirection(FdoParameterDirection_Input);
        params->Add(pParVal);

        int val = sqlCmd->ExecuteNonQuery();

        params->Clear();
        sqlCmd->SetSQLStatement( L"select vBlob from testTable where ID=(select max(ID) from testTable);" );
        FdoPtr<FdoISQLDataReader> myReader = sqlCmd->ExecuteReader();
        int colCnt = myReader->GetColumnCount();
        int gIdx = myReader->GetColumnIndex(L"vBlob");
        FdoDataType dtype = myReader->GetColumnType(gIdx);

        int cnt = 0;
        while(myReader->ReadNext())
        {
            CPPUNIT_ASSERT(myReader->IsNull(gIdx));
            cnt++;
        }
        myReader->Close();

        CPPUNIT_ASSERT(cnt == 1);

        params->Clear();
        sqlCmd->SetSQLStatement( L"DROP TABLE testTable;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

void SqlServerFdoSqlCmdTest::TestBindBigBlob()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        
        try
        {
            sqlCmd->SetSQLStatement( L"DROP TABLE testTable;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}
        
        sqlCmd->SetSQLStatement( L"CREATE TABLE testTable ([ID] [int] IDENTITY(1,1) NOT NULL, [G] [geometry] NULL, [vBlob] [varbinary](max) NULL)" );
        sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement( L"insert into testTable (vBlob) values (:bval);" );
        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();

        int buffSize = 1024*1024;
        FdoByte* bff = new FdoByte[buffSize];
        int* tmpBuff = (int*)bff;
        for(int i = 0; i < buffSize; i+=4)
        {
            *tmpBuff++ = (int)(13.33*rand());
        }
        FdoPtr<FdoByteArray> ba = FdoByteArray::Create(bff, buffSize);
        delete[] bff;

        FdoPtr<FdoBLOBValue> bVal = FdoBLOBValue::Create(ba);
        FdoPtr<FdoParameterValue> pParVal = FdoParameterValue::Create(L"bval", bVal);
        pParVal->SetDirection(FdoParameterDirection_Input);
        params->Add(pParVal);

        int val = sqlCmd->ExecuteNonQuery();

        params->Clear();
        sqlCmd->SetSQLStatement( L"select vBlob from testTable where ID=(select max(ID) from testTable);" );
        FdoPtr<FdoISQLDataReader> myReader = sqlCmd->ExecuteReader();
        int colCnt = myReader->GetColumnCount();
        int gIdx = myReader->GetColumnIndex(L"vBlob");
        FdoDataType dtype = myReader->GetColumnType(gIdx);

        int cnt = 0;
        while(myReader->ReadNext())
        {
            CPPUNIT_ASSERT(!myReader->IsNull(gIdx));
            FdoPtr<FdoLOBValue> blb = myReader->GetLOB(gIdx);
            FdoPtr<FdoByteArray> barr = blb->GetData();
            CPPUNIT_ASSERT(barr->GetCount() == ba->GetCount());
            CPPUNIT_ASSERT(memcmp(barr->GetData(), ba->GetData(), buffSize) == 0);
            cnt++;
        }
        myReader->Close();

        CPPUNIT_ASSERT(cnt == 1);

        params->Clear();
        sqlCmd->SetSQLStatement( L"DROP TABLE testTable;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

void SqlServerFdoSqlCmdTest::TestGetMaxLenTexts()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        
        try
        {
            sqlCmd->SetSQLStatement( L"DROP TABLE testTable;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}
        
        sqlCmd->SetSQLStatement( L"CREATE TABLE testTable ([ID] [int] IDENTITY(1,1) NOT NULL, [G] [geometry] NULL, [nchar] [nvarchar](max) NULL, [vchar] [varchar](max) NULL, [txt] [text] NULL, [vval] [sql_variant] NULL)" );
        sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement( L"insert into testTable (nchar, vchar, txt, vval) values (:ncval, :vcval, :tval, 33);" );
        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();
        
        FdoPtr<FdoStringValue> ncVal = FdoStringValue::Create(L"nvarchar");
        FdoPtr<FdoParameterValue> pNcVal = FdoParameterValue::Create(L"ncval", ncVal);
        pNcVal->SetDirection(FdoParameterDirection_Input);
        params->Add(pNcVal);

        FdoPtr<FdoStringValue> vcVal = FdoStringValue::Create(L"varchar");
        FdoPtr<FdoParameterValue> pVcVal = FdoParameterValue::Create(L"vcval", vcVal);
        pVcVal->SetDirection(FdoParameterDirection_Input);
        params->Add(pVcVal);

        FdoPtr<FdoStringValue> tVal = FdoStringValue::Create(L"text");
        FdoPtr<FdoParameterValue> ptVal = FdoParameterValue::Create(L"tval", tVal);
        ptVal->SetDirection(FdoParameterDirection_Input);
        params->Add(ptVal);
        sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement( L"insert into testTable (nchar, vchar, txt, vval) values (:ncval, :vcval, :tval, 'sql_variant');" );
        ncVal->SetString(L"nvarchar1");
        vcVal->SetString(L"varchar1");
        tVal->SetString(L"text1");
        sqlCmd->ExecuteNonQuery();

        params->Clear();
        sqlCmd->SetSQLStatement( L"select * from testTable" );
        FdoPtr<FdoISQLDataReader> myReader = sqlCmd->ExecuteReader();
        int colCnt = myReader->GetColumnCount();
        // verify variant type (must be BLOB)
        int ivval = myReader->GetColumnIndex(L"vval");
        FdoDataType dtype = myReader->GetColumnType(ivval);
        CPPUNIT_ASSERT(dtype == FdoDataType_BLOB);

        int inchar = myReader->GetColumnIndex(L"nchar");
        int ivchar = myReader->GetColumnIndex(L"vchar");
        int itxt = myReader->GetColumnIndex(L"txt");

        int cnt = 0;
        while(myReader->ReadNext())
        {
            CPPUNIT_ASSERT(!myReader->IsNull(ivval));
            FdoPtr<FdoLOBValue> rvval = myReader->GetLOB(ivval);
            FdoPtr<FdoByteArray> barr = rvval->GetData();
            int len = barr->GetCount();
            FdoByte* pvData = barr->GetData();

            CPPUNIT_ASSERT(!myReader->IsNull(inchar));
            FdoString* rnchar = myReader->GetString(inchar);
            if (cnt == 0)
            {
                CPPUNIT_ASSERT(wcscmp(rnchar, L"nvarchar") == 0);
            }
            else
            {
                CPPUNIT_ASSERT(wcscmp(rnchar, L"nvarchar1") == 0);
            }

            CPPUNIT_ASSERT(!myReader->IsNull(ivchar));
            FdoString* rvchar = myReader->GetString(ivchar);
            if (cnt == 0)
            {
                CPPUNIT_ASSERT(wcscmp(rvchar, L"varchar") == 0);
            }
            else
            {
                CPPUNIT_ASSERT(wcscmp(rvchar, L"varchar1") == 0);
            }

            CPPUNIT_ASSERT(!myReader->IsNull(itxt));
            FdoString* rtxt = myReader->GetString(itxt);
            if (cnt == 0)
            {
                CPPUNIT_ASSERT(wcscmp(rtxt, L"text") == 0);
            }
            else
            {
                CPPUNIT_ASSERT(wcscmp(rtxt, L"text1") == 0);
            }

            cnt++;
        }
        myReader->Close();

        CPPUNIT_ASSERT(cnt == 2);

        params->Clear();
        sqlCmd->SetSQLStatement( L"DROP TABLE testTable;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

void SqlServerFdoSqlCmdTest::TestInParamsStoreProcNoRet()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        try
        {
            sqlCmd->SetSQLStatement( L"DROP PROCEDURE MyRandProc;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}

        sqlCmd->SetSQLStatement( L"CREATE PROCEDURE MyRandProc (@key nvarchar(255), @value bigint)\nAS\nBEGIN\nSET NOCOUNT ON;\nEND" );
        sqlCmd->ExecuteNonQuery();

        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();

        FdoPtr<FdoInt32Value> idVal = FdoInt32Value::Create();
        FdoPtr<FdoParameterValue> pval = FdoParameterValue::Create(L"RetVal", idVal);
        pval->SetDirection(FdoParameterDirection_Output);
        params->Add(pval);
        
        FdoPtr<FdoStringValue> keyVal = FdoStringValue::Create(L"Mumu");
        FdoPtr<FdoParameterValue> pKeyVal = FdoParameterValue::Create(L"key", keyVal);
        pKeyVal->SetDirection(FdoParameterDirection_Input);
        params->Add(pKeyVal);

        FdoPtr<FdoInt32Value> initVal = FdoInt32Value::Create(300);
        FdoPtr<FdoParameterValue> pInitVal = FdoParameterValue::Create(L"value", initVal);
        pInitVal->SetDirection(FdoParameterDirection_Input);
        params->Add(pInitVal);

        sqlCmd->SetSQLStatement( L"MyRandProc(:key, :value)" );
        int randVal = sqlCmd->ExecuteNonQuery();
        printf("Test1 RandVal = %d", randVal);
        CPPUNIT_ASSERT( idVal->GetInt32() == randVal);
        
        params->Clear();
        sqlCmd->SetSQLStatement( L"DROP PROCEDURE MyRandProc;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

void SqlServerFdoSqlCmdTest::TestInParamsStoreProcBigInt()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        try
        {
            sqlCmd->SetSQLStatement( L"DROP PROCEDURE MyRandProc;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}

        sqlCmd->SetSQLStatement( L"CREATE PROCEDURE MyRandProc @initval bigint = 0, @outval bigint OUT\nAS\nBEGIN\ndeclare @value bigint\nSET NOCOUNT ON;\n set @value = 100*rand()+@initval;  set @outval = 19999999980+@initval;\nreturn (@value);\nEND" );
        sqlCmd->ExecuteNonQuery();

        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();

        FdoPtr<FdoInt32Value> idVal = FdoInt32Value::Create(0);
        FdoPtr<FdoParameterValue> pval = FdoParameterValue::Create(L"RetVal", idVal);
        pval->SetDirection(FdoParameterDirection_Output);
        params->Add(pval);
        
        FdoPtr<FdoInt64Value> initVal = FdoInt64Value::Create(300);
        FdoPtr<FdoParameterValue> pInitVal = FdoParameterValue::Create(L"InitVal", initVal);
        pInitVal->SetDirection(FdoParameterDirection_Input);
        params->Add(pInitVal);
        
		FdoPtr<FdoInt64Value> outVal = FdoInt64Value::Create((FdoInt64)0);
        FdoPtr<FdoParameterValue> pOutVal = FdoParameterValue::Create(L"OutVal", outVal);
        pOutVal->SetDirection(FdoParameterDirection_Output);
        params->Add(pOutVal);

        sqlCmd->SetSQLStatement( L"MyRandProc(:InitVal, :OutVal)" );
        int randVal = sqlCmd->ExecuteNonQuery();
		printf("Test1 RandVal[%d] = %lld", randVal, outVal->GetInt64());
        CPPUNIT_ASSERT( outVal->GetInt64() == 20000000280);
        
        params->Clear();
        sqlCmd->SetSQLStatement( L"DROP PROCEDURE MyRandProc;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

void SqlServerFdoSqlCmdTest::TestInParamsStoreProcVariant()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        try
        {
            sqlCmd->SetSQLStatement( L"DROP PROCEDURE TestOutVar;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}

        // SET NOCOUNT ON;
        sqlCmd->SetSQLStatement( L"CREATE PROCEDURE TestOutVar @initval bigint = 0, @outval sql_variant OUT\n AS\n BEGIN \ndeclare @value bigint\n set @value = 100*rand()+@initval;\n set @outval = convert(nvarchar, 19999999980+@initval); \n return (@value);\n END" );
        sqlCmd->ExecuteNonQuery();

        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();

        FdoPtr<FdoInt32Value> idVal = FdoInt32Value::Create(0);
        FdoPtr<FdoParameterValue> pval = FdoParameterValue::Create(L"RetVal", idVal);
        pval->SetDirection(FdoParameterDirection_Output);
        params->Add(pval);
        
        FdoPtr<FdoBLOBValue> outVal = FdoBLOBValue::Create();
        FdoPtr<FdoParameterValue> pOutVal = FdoParameterValue::Create(L"varRet", outVal);
        pOutVal->SetDirection(FdoParameterDirection_Output);
        params->Add(pOutVal);

        sqlCmd->SetSQLStatement( L"TestOutVar(300, :varRet)" );
        int randVal = sqlCmd->ExecuteNonQuery();
        if (outVal->IsNull())
            throw FdoException::Create(L"Invalid return value!");

        FdoPtr<FdoByteArray> arr = outVal->GetData();
        std::wstring ret ((const wchar_t*)arr->GetData(), arr->GetCount()/sizeof(wchar_t));
        printf("\nValue = %ls \n", ret.c_str());
        CPPUNIT_ASSERT(ret == L"20000000280");
        
        params->Clear();
        sqlCmd->SetSQLStatement( L"DROP PROCEDURE TestOutVar;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

void SqlServerFdoSqlCmdTest::TestNewSequence()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        sqlCmd->SetSQLStatement( L"SELECT convert(nvarchar, SERVERPROPERTY('productversion')) as av" );
        FdoPtr<FdoISQLDataReader> myReader = sqlCmd->ExecuteReader();
        FdoStringP var = L"0";
        if (myReader->ReadNext())
        {
            if (!myReader->IsNull(0))
            {
                FdoStringP val = myReader->GetString(0);
                var = val.Left(L".");
            }
        }
        if (var.ToLong() < 11)
        {
            printf("\nTest can be run on a higher server version\n");
            return;
        }

        try
        {
            sqlCmd->SetSQLStatement( L"DROP SEQUENCE RangeSeq;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}

        sqlCmd->SetSQLStatement( L"CREATE SEQUENCE RangeSeq AS int START WITH 1 INCREMENT BY 1 MINVALUE 1 MAXVALUE 25 CYCLE CACHE 10;" );
        sqlCmd->ExecuteNonQuery();

        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();

        FdoPtr<FdoInt32Value> idVal = FdoInt32Value::Create(0);
        FdoPtr<FdoParameterValue> pval = FdoParameterValue::Create(L"RetVal", idVal);
        pval->SetDirection(FdoParameterDirection_Output);
        params->Add(pval);
        
        FdoPtr<FdoStringValue> sName = FdoStringValue::Create(L"RangeSeq");
        FdoPtr<FdoParameterValue> psName = FdoParameterValue::Create(L"sequence_name", sName);
        psName->SetDirection(FdoParameterDirection_Input);
        params->Add(psName);
        
		FdoPtr<FdoInt64Value> rSize = FdoInt64Value::Create((FdoInt64)4);
        FdoPtr<FdoParameterValue> prSize = FdoParameterValue::Create(L"range_size", rSize);
        prSize->SetDirection(FdoParameterDirection_Input);
        params->Add(prSize);

        FdoPtr<FdoBLOBValue> outVal = FdoBLOBValue::Create();
        FdoPtr<FdoParameterValue> pOutVal = FdoParameterValue::Create(L"range_first_value", outVal);
        pOutVal->SetDirection(FdoParameterDirection_Output);
        params->Add(pOutVal);

        sqlCmd->SetSQLStatement( L"sp_sequence_get_range(:sequence_name, :range_size, :range_first_value)" );
        int randVal = sqlCmd->ExecuteNonQuery();
        if (outVal->IsNull())
            throw FdoException::Create(L"Invalid sequence value!");

        FdoPtr<FdoByteArray> arr = outVal->GetData();
        FdoInt64 vv = 0;
        if (arr->GetCount() == 4)
            vv = *(FdoInt32*)arr->GetData();
        else
            vv = *(FdoInt64*)arr->GetData();
        
        printf("\nSequence = %lld\n", vv);
        CPPUNIT_ASSERT(vv == 1);

        params->Clear();
        sqlCmd->SetSQLStatement( L"DROP SEQUENCE RangeSeq;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

void SqlServerFdoSqlCmdTest::TestBigInt()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        
        try
        {
            sqlCmd->SetSQLStatement( L"DROP TABLE testTable;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}
        
        sqlCmd->SetSQLStatement( L"CREATE TABLE testTable ([ID] [int] IDENTITY(1,1) NOT NULL, [G] [geometry] NULL, [vBigInt] bigint NULL)" );
        sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement( L"insert into testTable (vBigInt) values (:bval);" );
        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();

        FdoPtr<FdoInt64Value> bVal = FdoInt64Value::Create(19999999980);
        FdoPtr<FdoParameterValue> pParVal = FdoParameterValue::Create(L"bval", bVal);
        pParVal->SetDirection(FdoParameterDirection_Input);
        params->Add(pParVal);

        int val = sqlCmd->ExecuteNonQuery();

        params->Clear();
        sqlCmd->SetSQLStatement( L"select * from testTable;" );
        FdoPtr<FdoISQLDataReader> myReader = sqlCmd->ExecuteReader();
        int colCnt = myReader->GetColumnCount();
        int gIdx = myReader->GetColumnIndex(L"vBigInt");
        FdoDataType dtype = myReader->GetColumnType(gIdx);
        CPPUNIT_ASSERT(dtype == FdoDataType_Int64);

        int cnt = 0;
        while(myReader->ReadNext())
        {
            CPPUNIT_ASSERT(!myReader->IsNull(gIdx));
            CPPUNIT_ASSERT(myReader->GetInt64(gIdx) == 19999999980);
            cnt++;
        }
        myReader->Close();

        CPPUNIT_ASSERT(cnt == 1);

        params->Clear();
        sqlCmd->SetSQLStatement( L"DROP TABLE testTable;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

void SqlServerFdoSqlCmdTest::TestUnknownTypeBindParameter()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );

        try
        {
            sqlCmd->SetSQLStatement( L"DROP TABLE TEST_TABLE;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}

        sqlCmd->SetSQLStatement( L"create table TEST_TABLE (ID bigint primary key, VMIN decimal (18, 10), VMAX decimal (18, 10));" );
        sqlCmd->ExecuteNonQuery();
        
        sqlCmd->SetSQLStatement( L"insert into TEST_TABLE (ID, VMIN, VMAX) values (1, 0, 10);");
        sqlCmd->ExecuteNonQuery();
        
        sqlCmd->SetSQLStatement( L"insert into TEST_TABLE (ID, VMIN, VMAX) values (2, 10, 20);");
        sqlCmd->ExecuteNonQuery();
        
        sqlCmd->SetSQLStatement( L"insert into TEST_TABLE (ID, VMIN, VMAX) values (3, 20, 30);");
        sqlCmd->ExecuteNonQuery();


        FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();
        FdoPtr<FdoDoubleValue> dVal = FdoDoubleValue::Create(0.05);
        FdoPtr<FdoParameterValue> pval = FdoParameterValue::Create(L"tol", dVal);
        pval->SetDirection(FdoParameterDirection_Input);
        params->Add(pval);
        
        sqlCmd->SetSQLStatement( L"select ID from TEST_TABLE where 19.97 between (VMIN - :tol) and (VMAX + :tol)" );
        FdoPtr<FdoISQLDataReader> rdr = sqlCmd->ExecuteReader();
        int cnt = 0;
        while(rdr->ReadNext())
        {
            FdoInt64 id = 0;
            if (!rdr->IsNull(0))
                id = rdr->GetInt64(0);
            cnt++;
        }
        rdr->Close();

        CPPUNIT_ASSERT(cnt == 2);

        params->Clear();
        sqlCmd->SetSQLStatement( L"DROP TABLE TEST_TABLE;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}

#define EPSILON 1e-10

void SqlServerFdoSqlCmdTest::TestGeometryGeographyLatLong()
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd;
        sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
        try
        {
            sqlCmd->SetSQLStatement( L"DROP TABLE testTableGeomLL;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}
        try
        {
            sqlCmd->SetSQLStatement( L"DROP TABLE testTableGeogLL;" );
            sqlCmd->ExecuteNonQuery();
        }
        catch(FdoException *e)
        {e->Release();}

        sqlCmd->SetSQLStatement( L"CREATE TABLE testTableGeomLL(ID INT IDENTITY(1,1) NOT NULL, G geometry);" );
        sqlCmd->ExecuteNonQuery();
        
        sqlCmd->SetSQLStatement( L"CREATE TABLE testTableGeogLL(ID INT IDENTITY(1,1) NOT NULL, G geography);" );
        sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement( L"INSERT INTO testTableGeomLL (G) VALUES (geometry::STGeomFromText('LINESTRING (4 2, 5 3)', 4326));" );
        sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement( L"INSERT INTO testTableGeogLL (G) VALUES (geography::STGeomFromText('LINESTRING (4 2, 5 3)', 4326));" );
        sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement( L"INSERT INTO testTableGeomLL (G) VALUES (geometry::STGeomFromText('POLYGON ((0.5 0.5, 3 0, 3 3, 0 3, 0.5 0.5), (1 1, 1 2, 2 2, 2 1, 1 1))', 4326));" );
        sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement( L"INSERT INTO testTableGeogLL (G) VALUES (geography::STGeomFromText('POLYGON ((0.5 0.5, 3 0, 3 3, 0 3, 0.5 0.5), (1 1, 1 2, 2 2, 2 1, 1 1))', 4326));" );
        sqlCmd->ExecuteNonQuery();

        FdoPtr<FdoIFeatureReader> myReaderG;
        FdoPtr<FdoISelect> selCmdG;

        selCmdG = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
        selCmdG->SetFeatureClassName(L"dbo:testTableGeomLL");
        myReaderG = selCmdG->Execute();

        FdoPtr<FdoIFeatureReader> myReaderH;
        FdoPtr<FdoISelect> selCmdH;

        selCmdH = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
        selCmdH->SetFeatureClassName(L"dbo:testTableGeogLL");
        myReaderH = selCmdH->Execute();

        while ( myReaderG->ReadNext() && myReaderH->ReadNext())
        {
            FdoPtr<FdoByteArray> gG = myReaderG->GetGeometry(L"G");
            FdoPtr<FdoByteArray> gH = myReaderH->GetGeometry(L"G");
            // it's OK just to compare first point only to see if it's switched
            int* ireaderG = (int*)gG->GetData();
            int* ireaderH = (int*)gH->GetData();
            // we know we have only a line and a polygon
            int geom_type = (FdoGeometryType) ireaderG[0];

            switch (geom_type)
            {
            case FdoGeometryType_LineString :
                {
                    ireaderG += 3; //skip past geom_type, dimensionality, and point cnt
                    double* dreaderG = (double*) ireaderG;
                    
                    ireaderH += 3; //skip past geom_type, dimensionality, and point cnt
                    double* dreaderH = (double*) ireaderH;
                    
                    double dx = *dreaderG - *dreaderH;
                    CPPUNIT_ASSERT (fabs(dx) <= EPSILON);
                    dreaderG++;
                    dreaderH++;
                    dx = *dreaderG - *dreaderH;
                    CPPUNIT_ASSERT (fabs(dx) <= EPSILON);
                }
                break;
            case FdoGeometryType_Polygon :
                {
                    //skip past geom_type, dimensionality, and point cnt
                    double* dreaderG = (double*)&ireaderG[4];
                    double* dreaderH = (double*)&ireaderH[4];
                    double dx = *dreaderG - *dreaderH;
                    CPPUNIT_ASSERT (fabs(dx) <= EPSILON);
                    dreaderG++;
                    dreaderH++;
                    dx = *dreaderG - *dreaderH;
                    CPPUNIT_ASSERT (fabs(dx) <= EPSILON);
                }
            }
        }
        sqlCmd->SetSQLStatement( L"DROP TABLE testTableGeomLL;" );
        sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement( L"DROP TABLE testTableGeogLL;" );
        sqlCmd->ExecuteNonQuery();
    }
    catch( FdoException *ex )
    {
		TestCommonFail (ex);
    }
}
