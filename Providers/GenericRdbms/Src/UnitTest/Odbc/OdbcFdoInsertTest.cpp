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
#include "OdbcFdoInsertTest.h"
#include "UnitTestUtil.h"
#include "OdbcBaseSetup.h"

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcOracleFdoInsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoInsertTest, "FdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoInsertTest, "OdbcOracleFdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoInsertTest, "OdbcOracleTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcMySqlFdoInsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoInsertTest, "FdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoInsertTest, "OdbcMySqlFdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoInsertTest, "OdbcMySqlTests");

#ifdef _WIN32
CPPUNIT_TEST_SUITE_REGISTRATION( OdbcSqlServerFdoInsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoInsertTest, "FdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoInsertTest, "OdbcSqlServerFdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoInsertTest, "OdbcSqlServerTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcAccessFdoInsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoInsertTest, "FdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoInsertTest, "OdbcAccessFdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoInsertTest, "OdbcAccessTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcExcelFdoInsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcExcelFdoInsertTest, "FdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcExcelFdoInsertTest, "OdbcExcelFdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcExcelFdoInsertTest, "OdbcExcelTests");
#endif

void OdbcBaseFdoInsertTest::setUp ()
{
    set_provider();
    connect();
}

void OdbcBaseFdoInsertTest::tearDown ()
{
    if (mConnection != NULL)
        mConnection->Close();
    mConnection = NULL;
}

void OdbcBaseFdoInsertTest::connect ()
{
    try
    {
		mConnection = UnitTestUtil::GetProviderConnectionObject();
        if (DataBaseType_None != mSetup.GetTypeDB() )
        {
            // Set up databases that are not prefabricated.
            StringConnTypeRequest connectionType = Connection_NoDatastore;
            if (DataBaseType_Oracle == mSetup.GetTypeDB() )
                connectionType = Connection_OraSetup;
		    mConnection->SetConnectionString ( UnitTestUtil::GetConnectionString(connectionType, "") );
		    mConnection->Open();
		    mSetup.CreateDataStore(mConnection, "");
		    mSetup.CreateAcadSchema(mConnection);
		    mSetup.CreateNonAcadSchema(mConnection);

		    mConnection->Close();
        }
		mConnection->SetConnectionString ( UnitTestUtil::GetConnectionString(Connection_WithDSN, "") );
		mConnection->Open();
    }
    catch (FdoException *ex)
    {
        mConnection = NULL;
        TestCommonFail (ex);
    }
}



void OdbcBaseFdoInsertTest::insert()
{
	clock_t start, finish;
    if (mConnection != NULL) try
    {
        FdoPtr<FdoIInsert> insertCommand =
            (FdoIInsert *) mConnection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(mSetup.GetClassNameAcdb3dpolyline());
        FdoPtr<FdoPropertyValueCollection> propertyValues =
            insertCommand->GetPropertyValues();

		start = clock();
        printf("start insert non-feature class\n");
        FdoPtr<FdoDataValue> dataValue;
        FdoPtr<FdoPropertyValue> propertyValue;

        try {
            UnitTestUtil::Sql2Db(L"delete from acdb3dpolyline where featid = '10000'", static_cast<FdoIConnection *>(mConnection.p));
        }catch (FdoException *ex){ex->Release();}

        if (!isPkeyAutogeneratedAcdb3dpolyline())
        {
            dataValue = FdoDataValue::Create(10000);
            propertyValue = AddNewProperty( propertyValues, mSetup.GetPropertyNameAcdb3dpolylineFeatId());
            propertyValue->SetValue(dataValue);
        }

        dataValue = FdoDataValue::Create(11);
        propertyValue = AddNewProperty( propertyValues, mSetup.GetPropertyNameAcdb3dpolylineClassId());
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(0);
        propertyValue = AddNewProperty( propertyValues, mSetup.GetPropertyNameAcdb3dpolylineRevision());
        propertyValue->SetValue(dataValue);

		FdoDateTime dateTime;

		dateTime.year    = 2006;
		dateTime.month   = 10;
		dateTime.day     = 20;
		dateTime.hour    = 6;
		dateTime.minute  = 45;
		dateTime.seconds = 7;
		dataValue = FdoDataValue::Create(dateTime);
		propertyValue = AddNewProperty( propertyValues, mSetup.GetPropertyNameAcdb3dpolylineDateTime());
		propertyValue->SetValue(dataValue);

        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

		finish = clock();
		printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
    }
    catch (FdoException *ex)
    {
        TestCommonFail (ex);
    }
}


void OdbcMySqlFdoInsertTest::ConfigFileTest()
{
    // This test is just like insert() above, but uses a configuration document.
	clock_t start, finish;
    if (mConnection != NULL) try
    {
        // Re-open the connection with a configuration document in place.
        mConnection->Close();
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(GetConfigFile2(), L"r");
        mConnection->SetConfiguration(fileStream);
        try
        {
            mConnection->Open();
        }
        catch (FdoException *ex)
        {
            TestCommonFail (ex);
        }

        FdoPtr<FdoIInsert> insertCommand =
            (FdoIInsert *) mConnection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(L"Acdb:Polyline");
        FdoPtr<FdoPropertyValueCollection> propertyValues =
            insertCommand->GetPropertyValues();

		start = clock();
        printf("start insert with config file\n");
        FdoPtr<FdoDataValue> dataValue;
        FdoPtr<FdoPropertyValue> propertyValue;

        try {
            UnitTestUtil::Sql2Db(L"delete from acdb3dpolyline where featid = '10001'", static_cast<FdoIConnection *>(mConnection.p));
        }catch (FdoException *ex){ex->Release();}

        if (!isPkeyAutogeneratedAcdb3dpolyline())
        {
            dataValue = FdoDataValue::Create(10001);
            propertyValue = AddNewProperty( propertyValues, L"Id");
            propertyValue->SetValue(dataValue);
        }

        dataValue = FdoDataValue::Create(11);
        propertyValue = AddNewProperty( propertyValues, L"ClassId");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(0);
        propertyValue = AddNewProperty( propertyValues, L"RevisionNumber");
        propertyValue->SetValue(dataValue);

        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

		finish = clock();
		printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
    }
    catch (FdoException *ex)
    {
        TestCommonFail (ex);
    }
    if (mConnection != NULL) try
    {
        // Set the connection back to having no configuration document.
        // We do this in a separate block in order to ensure that the
        // Insert command above is released by falling out of scope.
        mConnection->Close();
        mConnection->SetConfiguration(NULL);
        mConnection->Open();
    }
    catch (FdoException *ex)
    {
        TestCommonFail (ex);
    }
}


void OdbcBaseFdoInsertTest::insertCities()
{
    if (mConnection != NULL) try
    {
        FdoPtr<FdoIInsert> insertCommand =
            (FdoIInsert *) mConnection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(GetClassNameCities());
        FdoPtr<FdoPropertyValueCollection> propertyValues =
            insertCommand->GetPropertyValues();

        printf("start insert non-feature class\n");
        FdoPtr<FdoDataValue> dataValue;
        FdoPtr<FdoPropertyValue> propertyValue;

        try {
            UnitTestUtil::Sql2Db(L"delete from cities where CITY = 'FakeVille'", static_cast<FdoIConnection *>(mConnection.p));
        }catch (FdoException *ex){ex->Release();}

        if (!isPkeyAutogeneratedCities())
        {
            dataValue = FdoDataValue::Create(50);
		    propertyValue = FdoInsertTest::AddNewProperty( propertyValues, GetPropertyNameCitiesCityId());
            propertyValue->SetValue(dataValue);
        }

        dataValue = FdoDataValue::Create(L"Carleton");
        propertyValue = FdoInsertTest::AddNewProperty( propertyValues, GetPropertyNameCitiesName());
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"FakeVille");
        propertyValue = FdoInsertTest::AddNewProperty( propertyValues, GetPropertyNameCitiesCity());
        propertyValue->SetValue(dataValue);

		dataValue = FdoDataValue::Create(L"2006-10-11");
		propertyValue = FdoInsertTest::AddNewProperty( propertyValues, GetPropertyNameCitiesDate());
		propertyValue->SetValue(dataValue);

        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
		FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);
		selectCmd->SetFeatureClassName(GetClassNameCities());
		FdoPtr<FdoIFeatureReader> myReader = selectCmd->Execute();
		if (myReader)
		{
			while (myReader->ReadNext())
			{
				if (!myReader->IsNull(GetPropertyNameCitiesCityId()))
				{
					FdoInt64 id = myReader->GetInt64(GetPropertyNameCitiesCityId());
					printf("\nCityId: %ld", id);
				}
				if (!myReader->IsNull(GetPropertyNameCitiesDate()))
				{
					FdoDateTime dateTime = myReader->GetDateTime(GetPropertyNameCitiesDate());
					printf(" date: %4d-%02d-%02d %02d:%02d:%02d",dateTime.year, 
																	dateTime.month,
																	dateTime.day,
																	dateTime.hour,
																	dateTime.minute,
																	dateTime.seconds);
				}
			}
		}
		mConnection->Close();
    }
    catch (FdoException *ex)
    {
        TestCommonFail (ex);
    }
}


void OdbcBaseFdoInsertTest::insertTable1()
{
	clock_t start, finish;
    if (mConnection != NULL) try
    {
        FdoPtr<FdoIInsert> insertCommand =
            (FdoIInsert *) mConnection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(mSetup.GetClassNameTable1());
        FdoPtr<FdoPropertyValueCollection> propertyValues =
            insertCommand->GetPropertyValues();

        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

        start = clock();
        printf("start insert feature class\n");
        FdoPtr<FdoDataValue> dataValue;
        FdoPtr<FdoPropertyValue> propertyValue;

        try {
            UnitTestUtil::Sql2Db(L"delete from TABLE1 where NAME = 'FakeName'", static_cast<FdoIConnection *>(mConnection.p));
        }catch (FdoException *ex){ex->Release();}

        if (!isPkeyAutogeneratedTable1())
        {
            dataValue = FdoDataValue::Create(50);
            propertyValue = AddNewProperty( propertyValues, mSetup.GetPropertyNameTable1FeatId());
            propertyValue->SetValue(dataValue);
        }

        dataValue = FdoDataValue::Create(L"FakeName");
        propertyValue = AddNewProperty( propertyValues, mSetup.GetPropertyNameCitiesName());
        propertyValue->SetValue(dataValue);

        propertyValue = AddNewProperty( propertyValues, L"Geometry" );
        FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXY(50, 60);
        FdoPtr<FdoIPoint> pt = gf->CreatePoint(pos);
        FdoPtr<FdoByteArray> byteArray = gf->GetFgf(pt);
        FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
        propertyValue->SetValue(geometryValue);

        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

		finish = clock();
		printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
    }
    catch (FdoException *ex)
    {
        TestCommonFail (ex);
    }
}


#ifdef _WIN32

void OdbcAccessFdoInsertTest::insertLidar()
{
	clock_t start, finish;
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        connection->SetConnectionString(GetConnectStringLidar());
        connection->Open();

        try
        {
            FdoPtr<FdoIInsert> insertCommand =
                (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"LIDAR");
            FdoPtr<FdoPropertyValueCollection> propertyValues =
                insertCommand->GetPropertyValues();

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

			start = clock();
            printf("start insert feature class LIDAR\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            dataValue = FdoDataValue::Create(L"100");
            propertyValue = AddNewProperty( propertyValues, L"ELEVATION");
            propertyValue->SetValue(dataValue);

            propertyValue = AddNewProperty( propertyValues, L"Geometry" );
            FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXY(50, 60);
            FdoPtr<FdoIPoint> pt = gf->CreatePoint(pos);
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(pt);
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
            CPPUNIT_ASSERT( reader != NULL );

            FdoInt32 numInserted = 0;
            while ( reader->ReadNext() )
            {
                FdoInt64 id = reader->GetInt64(L"ID");
                numInserted++;
                // Test DB leaves off at 65535.  Also do sanity check for 100000.
                CPPUNIT_ASSERT( id > 65535 && id < 100000 );
            }
            CPPUNIT_ASSERT( numInserted == 1 );

			finish = clock();
			printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
        }
        catch (...)
        {
            throw;
        }
    }
    catch (FdoException *ex)
    {
        TestCommonFail (ex);
    }
}

void OdbcExcelFdoInsertTest::insertTable1()
{
	clock_t start, finish;
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(GetConfigFile(), L"r");
        connection->SetConfiguration(fileStream);

        // For Excel, we need a config file because Excel doesn't support primary keys.
        connection->SetConnectionString(GetConnectString());
        connection->Open();

        try
        {
            FdoPtr<FdoIInsert> insertCommand =
                (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"TABLE1");
            FdoPtr<FdoPropertyValueCollection> propertyValues =
                insertCommand->GetPropertyValues();

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

            start = clock();
            printf("start insert feature class\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            try {
                UnitTestUtil::Sql2Db(L"delete from TABLE1 where NAME = 'FakeName'", static_cast<FdoIConnection *>(connection));
            }catch (FdoException *ex){ex->Release();}

            dataValue = FdoDataValue::Create(50);
            propertyValue = AddNewProperty( propertyValues, L"FEATID1");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"FakeName");
            propertyValue = AddNewProperty( propertyValues, L"NAME");
            propertyValue->SetValue(dataValue);

            propertyValue = AddNewProperty( propertyValues, L"Geometry" );
            FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXY(50, 60);
            FdoPtr<FdoIPoint> pt = gf->CreatePoint(pos);
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(pt);
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

			finish = clock();
			printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
        }
        catch (...)
        {
            throw;
        }
    }
    catch (FdoException *ex)
    {
        TestCommonFail (ex);
    }
}

void OdbcExcelFdoInsertTest::insertPoints()
{
	clock_t start, finish;
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(GetConfigFile(), L"r");
        connection->SetConfiguration(fileStream);

        // For Excel, we need a config file because Excel doesn't support primary keys.
        connection->SetConnectionString(GetConnectString());
        connection->Open();

        try
        {
            FdoPtr<FdoIInsert> insertCommand =
                (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"POINTS");
            FdoPtr<FdoPropertyValueCollection> propertyValues =
                insertCommand->GetPropertyValues();

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

            start = clock();
            printf("start insert feature class\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            try {
                UnitTestUtil::Sql2Db(L"delete from POINTS where PNO = '3000'", static_cast<FdoIConnection *>(connection));
            }catch (FdoException *ex){ex->Release();}

            dataValue = FdoDataValue::Create(3000);
            propertyValue = AddNewProperty( propertyValues, L"PNO");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"Fake");
            propertyValue = AddNewProperty( propertyValues, L"DSC");
            propertyValue->SetValue(dataValue);

            propertyValue = AddNewProperty( propertyValues, L"Geometry" );
            FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXYZ(50, 60, 70);
            FdoPtr<FdoIPoint> pt = gf->CreatePoint(pos);
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(pt);
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

			finish = clock();
			printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
        }
        catch (...)
        {
            throw;
        }
    }
    catch (FdoException *ex)
    {
        TestCommonFail (ex);
    }
}

#endif
