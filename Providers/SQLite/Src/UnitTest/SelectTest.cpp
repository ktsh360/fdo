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

#include "stdafx.h"
#include "SelectTest.h"
#include "UnitTestUtil.h"
#include <ctime>
#include <cppunit/extensions/HelperMacros.h>
#include "FdoCommonFile.h"

#ifdef _WIN32
static const wchar_t* SC_TEST_FILE = L"..\\..\\TestData\\SelectTest.sqlite";
static const wchar_t* SRC_TEST_FILE = L"..\\..\\TestData\\PARCEL_Source.sqlite";
#else
#include <unistd.h>
static const wchar_t* SC_TEST_FILE = L"../../TestData/SelectTest.sqlite";
static const wchar_t* SRC_TEST_FILE = L"../../TestData/PARCEL_Source.sqlite";
#endif


CPPUNIT_TEST_SUITE_REGISTRATION( SelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SelectTest, "SelectTest");

SelectTest::SelectTest(void)
{
    
}

SelectTest::~SelectTest(void)
{
    
}


void SelectTest::setUp ()
{

}

void SelectTest::tearDown ()
{
}

void SelectTest::TestSimpleSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoISelect> SelectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    SelectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < 30");
        SelectCmd->SetFilter(filter);

        FdoPtr<FdoIFeatureReader>reader = SelectCmd->Execute();
        int rez = 0;
        while(reader->ReadNext())rez++;
        printf ("Selectd features : %d\n", rez);
        CPPUNIT_ASSERT(rez == 29);
    }
    catch ( FdoException* e )
	{
		TestCommonFail( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		throw;
	}
   	catch (...)
   	{
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
	printf( "Done\n" );
}

void SelectTest::TestBindSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoISelect> SelectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    SelectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < :parm");
        SelectCmd->SetFilter(filter);

        FdoPtr<FdoParameterValueCollection>parms = SelectCmd->GetParameterValues();
        FdoPtr<FdoInt32Value> intval = FdoInt32Value::Create(30);
        FdoPtr<FdoParameterValue>parm = FdoParameterValue::Create(L"parm",intval);
        parms->Add(parm);
        FdoPtr<FdoIFeatureReader>reader = SelectCmd->Execute();
        int rez = 0;
        while(reader->ReadNext())rez++;
        printf ("Selectd features : %d\n", rez);
        CPPUNIT_ASSERT(rez == 29);
    }
    catch ( FdoException* e )
	{
		TestCommonFail( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		throw;
	}
   	catch (...)
   	{
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
	printf( "Done\n" );
}

void SelectTest::TestSpatialSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoISelect> SelectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    SelectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID > :lowerbound AND ID < :upperbound AND Data2 INSIDE GeomFromText('POLYGON XYZ ((7.1770013502456 43.7501967446194 0, 7.1770013502456 43.6912771493358 0, 7.27407112243824 43.6912771493358 0, 7.27407112243824 43.7501967446194 0, 7.1770013502456 43.7501967446194 0))')");
        SelectCmd->SetFilter(filter);
        
        FdoPtr<FdoParameterValueCollection>parms = SelectCmd->GetParameterValues();
        FdoPtr<FdoInt32Value> intval;
        FdoPtr<FdoParameterValue>parm;

        // Add the bound paramaters in the reverse order.
        intval = FdoInt32Value::Create(10000);
        parm = FdoParameterValue::Create(L"upperbound",intval);
        parms->Add(parm);

        intval = FdoInt32Value::Create(5000);
        parm = FdoParameterValue::Create(L"lowerbound",intval);
        parms->Add(parm);

        FdoPtr<FdoIFeatureReader>reader = SelectCmd->Execute();
        int rez = 0;
        while(reader->ReadNext())rez++;
        
        printf ("Selectd features : %d\n", rez);
        CPPUNIT_ASSERT(rez == 4954);
    }
    catch ( FdoException* e )
	{
		TestCommonFail( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		throw;
	}
   	catch (...)
   	{
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
	printf( "Done\n" );
}

void SelectTest::TestComplexSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoISelect> SelectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    SelectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < 10000 AND Data2 INSIDE GeomFromText('POLYGON XYZ ((7.1770013502456 43.7501967446194 0, 7.1770013502456 43.6912771493358 0, 7.27407112243824 43.6912771493358 0, 7.27407112243824 43.7501967446194 0, 7.1770013502456 43.7501967446194 0))')");
        SelectCmd->SetFilter(filter);

        FdoPtr<FdoIFeatureReader>reader = SelectCmd->Execute();
        int rez = 0;
        while(reader->ReadNext())rez++;
        printf ("Selectd features : %d\n", rez);
        CPPUNIT_ASSERT(rez == 9471);
    }
    catch ( FdoException* e )
	{
		TestCommonFail( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		throw;
	}
   	catch (...)
   	{
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
	printf( "Done\n" );
}

void SelectTest::TestComplexWithBindSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoISelect> SelectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    SelectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < :parm AND Data2 INSIDE GeomFromText('POLYGON XYZ ((7.1770013502456 43.7501967446194 0, 7.1770013502456 43.6912771493358 0, 7.27407112243824 43.6912771493358 0, 7.27407112243824 43.7501967446194 0, 7.1770013502456 43.7501967446194 0))')");
        SelectCmd->SetFilter(filter);

        FdoPtr<FdoParameterValueCollection>parms = SelectCmd->GetParameterValues();
        FdoPtr<FdoInt32Value> intval = FdoInt32Value::Create(10000);
        FdoPtr<FdoParameterValue>parm = FdoParameterValue::Create(L"parm",intval);
        parms->Add(parm);
        FdoPtr<FdoIFeatureReader>reader = SelectCmd->Execute();
        int rez = 0;
        while(reader->ReadNext())rez++;
        printf ("Selectd features : %d\n", rez);
        CPPUNIT_ASSERT(rez == 9471);
    }
    catch ( FdoException* e )
	{
		TestCommonFail( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		throw;
	}
   	catch (...)
   	{
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
	printf( "Done\n" );
}

void SelectTest::BooleanDataTest ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
		conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, true, true);
		 
        //apply schema
		FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
        FdoPtr<FdoFeatureSchemaCollection> schColl = FdoFeatureSchemaCollection::Create(NULL);
        schColl->ReadXml(L"SchBooleanTest.xml");
        CPPUNIT_ASSERT(schColl->GetCount() == 1);
        
        FdoPtr<FdoFeatureSchema> schema = schColl->GetItem(0);
		applyschema->SetFeatureSchema(schema);
		applyschema->Execute();

        {
            FdoPtr<FdoIInsert> insCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
            FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
            FdoPtr<FdoPropertyValue> propIns;
            
		    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
		    double coords[] = { 7.2068, 43.7556, 
							    7.2088, 43.7556, 
							    7.2088, 43.7574, 
							    7.2068, 43.7574, 
							    7.2068, 43.7556 }; 
		    FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 10, coords);
		    FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);
		    FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
		    FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);

            FdoPtr<FdoPropertyValue> propGeomIns = FdoPropertyValue::Create(L"Geometry", gv);
            vals->Add(propGeomIns);
            
            FdoPtr<FdoBooleanValue> boolVal = FdoBooleanValue::Create(true);
            FdoPtr<FdoPropertyValue> propBoolIns = FdoPropertyValue::Create(L"BoolProp", boolVal);
            vals->Add(propBoolIns);

            FdoPtr<FdoStringValue> strVal = FdoStringValue::Create(L"sky is blue");
            FdoPtr<FdoPropertyValue> propStrIns = FdoPropertyValue::Create(L"Name", strVal);
            vals->Add(propStrIns);

            insCmd->SetFeatureClassName(L"BoolTest");
            
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            rdr->Close();
            
            boolVal->SetBoolean(false);
            strVal->SetString(L"water is red");
            rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            rdr->Close();

            boolVal->SetNull();
            strVal->SetString(L"snow is white or black");
            rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            rdr->Close();
        }

        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

        FdoPtr<FdoIdentifierCollection> props =  selectCmd->GetPropertyNames();
        props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"FeatId")));
        props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Geometry")));
        props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"BoolProp")));
        props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name")));
        FdoPtr<FdoComputedIdentifier> cmpid = FdoComputedIdentifier::Create(L"JustTrue", FdoPtr<FdoExpression>(FdoExpression::Parse(L"TRUE")));
        props->Add(cmpid);

        FdoPtr<FdoComputedIdentifier> cmpid2 = FdoComputedIdentifier::Create(L"CanBe", FdoPtr<FdoExpression>(FdoExpression::Parse(L"concat(Name, ' and this can be ', TRUE)")));
        props->Add(cmpid2);
        
        FdoPtr<FdoComputedIdentifier> cmpid3 = FdoComputedIdentifier::Create(L"ConcText", FdoPtr<FdoExpression>(FdoExpression::Parse(L"concat(Name, ' and this is ', BoolProp)")));
        props->Add(cmpid3);

        selectCmd->SetFeatureClassName(L"BoolTest");
        FdoPtr<FdoIFeatureReader>reader = selectCmd->Execute();
        int idx = 0;
        while(reader->ReadNext())
        {
            if (!reader->IsNull(L"JustTrue"))
            {
                bool justTrue = reader->GetBoolean(L"JustTrue");
                CPPUNIT_ASSERT(justTrue);
            }
            else
            {
                CPPUNIT_FAIL ("Invalid result data");
            }
            if (!reader->IsNull(L"CanBe"))
            {
                FdoStringP canBe = reader->GetString(L"CanBe");
                switch(idx)
                {
                case 0:
                    CPPUNIT_ASSERT(canBe == L"sky is blue and this can be 1");
                    break;
                case 1:
                    CPPUNIT_ASSERT(canBe == L"water is red and this can be 1");
                    break;
                case 2:
                    CPPUNIT_ASSERT(canBe == L"snow is white or black and this can be 1");
                    break;
                default:
                    CPPUNIT_FAIL ("Invalid row index");
                }
            }
            else
            {
                CPPUNIT_FAIL ("Invalid result data");
            }
            if (!reader->IsNull(L"ConcText"))
            {
                FdoStringP concText = reader->GetString(L"ConcText");
                switch(idx)
                {
                case 0:// bool is TRUE
                    CPPUNIT_ASSERT(concText == L"sky is blue and this is 1");
                    break;
                case 1:// bool is FALSE
                    CPPUNIT_ASSERT(concText == L"water is red and this is 0");
                    break;
                case 2: // bool is NULL
                    CPPUNIT_ASSERT(concText == L"snow is white or black and this is ");
                    break;
                default:
                    CPPUNIT_FAIL ("Invalid row index");
                }
            }
            else
            {
                CPPUNIT_FAIL ("Invalid result data");
            }
            idx++;
        }
        reader->Close();
    }
    catch ( FdoException* e )
	{
		TestCommonFail( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		throw;
	}
   	catch (...)
   	{
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
		
	printf( "Done\n" );
}

void SelectTest::TestAggregatesSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
        FdoPtr<FdoISelectAggregates> selectCmd = static_cast<FdoISelectAggregates*>(conn->CreateCommand(FdoCommandType_SelectAggregates));
	    selectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID > 1");
        selectCmd->SetFilter(filter);
        
        FdoPtr<FdoIdentifierCollection> idfc = selectCmd->GetPropertyNames();
        FdoPtr<FdoIdentifier> idf = FdoIdentifier::Create(L"Name");
        idfc->Add(idf);
        FdoPtr<FdoExpression> exp1 = FdoExpression::Parse(L"ID * 12.44 - 4.5");
        FdoPtr<FdoComputedIdentifier> cidf = FdoComputedIdentifier::Create(L"dbVal", exp1);
        idfc->Add(cidf);

        FdoPtr<FdoIdentifierCollection> idfcg = selectCmd->GetGrouping();
        FdoPtr<FdoIdentifier> idfg = FdoIdentifier::Create(L"Name");
        idfcg->Add(idfg);
        FdoPtr<FdoFilter> grFilter = FdoFilter::Parse(L"Name='AB0026'");
        selectCmd->SetGroupingFilter(grFilter);
        
        FdoPtr<FdoIdentifierCollection> idfco = selectCmd->GetOrdering();
        idfco->Add(cidf);

        FdoPtr<FdoIDataReader> rdr = selectCmd->Execute();
        int idx = 0;
        while(rdr->ReadNext())
        {
            FdoDataType dt = rdr->GetDataType(L"dbVal");
            FdoString* name = rdr->GetString(L"Name");
            double dbVal = rdr->GetDouble(L"dbVal");
            printf("[%d] = '%ls' & %g", idx++, name, dbVal);
        }
        rdr->Close();
    }
    catch ( FdoException* e )
	{
		TestCommonFail( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		throw;
	}
   	catch (...)
   	{
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
	printf( "Done\n" );
}