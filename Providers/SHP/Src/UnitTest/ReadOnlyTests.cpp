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

#include "Pch.h"
#include "ReadOnlyTests.h"



CPPUNIT_TEST_SUITE_REGISTRATION (ReadOnlyTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (ReadOnlyTests, "ReadOnlyTests");

FdoPtr<FdoIConnection> ReadOnlyTests::mConnection;

ReadOnlyTests::ReadOnlyTests (void)
{
}

ReadOnlyTests::~ReadOnlyTests (void)
{
}

void ReadOnlyTests::setUp ()
{
    mConnection = ShpTests::GetConnection ();
#ifdef _WIN32
    mConnection->SetConnectionString (L"DefaultFileLocation=a:");
#else
    mConnection->SetConnectionString (L"DefaultFileLocation=/mnt/floppy");
#endif
    CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
}

void ReadOnlyTests::tearDown ()
{
    mConnection->Close ();
    FDO_SAFE_RELEASE(mConnection.p);
}

void ReadOnlyTests::create_schema (FdoGeometricType type, bool elevation, bool measure)
{
    // delete the class, if its there:
    TestCommonSchemaUtil::CleanUpClass (mConnection, NULL, L"Test");

    // create the class
    FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (L"TheSchema", L" test schema");
    FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

    FdoPtr<FdoDataPropertyDefinition> featid = FdoDataPropertyDefinition::Create (L"FeatId", L"integer");
    featid->SetDataType (FdoDataType_Int32);
    featid->SetIsAutoGenerated (true);
    featid->SetNullable (false);

    FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"Id", L"integer");
    id->SetDataType (FdoDataType_Decimal);
    id->SetPrecision(10);
    id->SetScale(0);

    FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Street", L"text");
    street->SetDataType (FdoDataType_String);
    street->SetLength (64);

    FdoPtr<FdoDataPropertyDefinition> area = FdoDataPropertyDefinition::Create (L"Area", L"double");
    area->SetDataType (FdoDataType_Decimal);
    area->SetPrecision (20);
    area->SetScale (20);

    FdoPtr<FdoDataPropertyDefinition> vacant = FdoDataPropertyDefinition::Create (L"Vacant", L"boolean");
    vacant->SetDataType (FdoDataType_Boolean);

    FdoPtr<FdoDataPropertyDefinition> birthday = FdoDataPropertyDefinition::Create (L"Birthday", L"date");
    birthday->SetDataType (FdoDataType_DateTime);

    // build a location geometry property
    FdoPtr<FdoGeometricPropertyDefinition> location = FdoGeometricPropertyDefinition::Create (L"Geometry", L"geometry");
    location->SetGeometryTypes (type);
    location->SetHasElevation (elevation);
    location->SetHasMeasure (measure);

    //// assemble the feature class
    FdoPtr<FdoFeatureClass> feature = FdoFeatureClass::Create (L"Test", L"test class created with apply schema");
    FdoPtr<FdoPropertyDefinitionCollection> properties = feature->GetProperties ();
    FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
    properties->Add (featid);
    identities->Add (featid);
    properties->Add (id);
    properties->Add (street);
    properties->Add (area);
    properties->Add (vacant);
    properties->Add (birthday);
    properties->Add (location);
    feature->SetGeometryProperty (location);

    // submit the new schema
    classes->Add (feature);
    FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
    apply->SetFeatureSchema (schema);
    apply->Execute ();
}

bool ReadOnlyTests::contains (const wchar_t* pattern, const wchar_t* string)
{
    size_t length;
    wchar_t* p;
    wchar_t* q;
    wchar_t c;
    bool ret;

    ret = false;

    length = FdoCommonStringUtil::StringLength (pattern);
    p = (wchar_t*)alloca (sizeof (wchar_t) * (FdoCommonStringUtil::StringLength (string) + 1));
    wcscpy (p, string);
    while (NULL != (q = (wchar_t*)FdoCommonStringUtil::FindCharacter (p, pattern[0])))
        if (length < FdoCommonStringUtil::StringLength (q))
        {
            c = *(q + length);
            *(q + length) = L'\0';
            if (0 == FdoCommonStringUtil::StringCompareNoCase (pattern, q))
            {
                ret = true;
                break;
            }
            else
                p = q + 1;
            *(q + length) = c;
        }
        else
            break;

    return (ret);
}

void ReadOnlyTests::select ()
{
    double  area;
    double  length;
    int count;
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"ontario");
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        count = 0;
        while (reader->ReadNext ())
        {
            count++;
            reader->GetInt32 (L"FeatId");
            area = reader->GetDouble (L"AREA");
            length = reader->GetDouble (L"PERIMETER");
            reader->GetDouble (L"ONTARIO_");
            reader->GetDouble (L"ONTARIO_ID");
            FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");

            TestCommonGeomUtil::PrintGeometryAnalysis( L"ontario", count, geometry, length, area, VERBOSE);
        }
        CPPUNIT_ASSERT_MESSAGE ("no ontario features selected", 0 != count);
    }
    catch (FdoException* ge)
    {
        TestCommonFail(ge);
    }
    catch(...)
    {
        CPPUNIT_FAIL ("ReadOnlyTests::select() failed");
    }

}

void ReadOnlyTests::apply_schema ()
{
    try
    {
        create_schema (FdoGeometricType_Point, true, true);
        CPPUNIT_FAIL ("apply schema worked on a read-only volume");
    }
    catch (FdoException* ge)
    {
        // check for 'read-only' somwehere in the message
        bool ok = contains (L"read-only", ge->GetExceptionMessage ());
        ge->Release ();
        CPPUNIT_ASSERT_MESSAGE ("no read-only message from ApplySchema", ok);
    }
}

void ReadOnlyTests::insert ()
{
    try
    {

        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (L"ontario");
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)ShpTests::ParseByDataType (L"2400129.29", FdoDataType_Decimal);
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (L"AREA", expression);
        values->Add (value);
        expression = (FdoValueExpression*)ShpTests::ParseByDataType (L"72821.56", FdoDataType_Decimal);
        value = FdoPropertyValue::Create (L"PERIMETER", expression);
        values->Add (value);
        expression = (FdoValueExpression*)ShpTests::ParseByDataType (L"2", FdoDataType_Decimal);
        value = FdoPropertyValue::Create (L"ONTARIO_", expression);
        values->Add (value);
        expression = (FdoValueExpression*)ShpTests::ParseByDataType (L"2", FdoDataType_Decimal);
        value = FdoPropertyValue::Create (L"ONTARIO_ID", expression);
        values->Add (value);
        // add real geometry value:
        FdoPtr<FdoGeometryValue> geometry = (FdoGeometryValue*)FdoExpression::Parse (L"GeomFromText('POLYGON XY ((5108.8 5104.7, 5109 5104, 5109 5105, 5108.8 5104.7))')");
        value = FdoPropertyValue::Create (L"Geometry", geometry);
        values->Add (value);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        if (reader->ReadNext ())
            CPPUNIT_FAIL ("insert really worked on a read-only volume");
        reader->Close ();
        CPPUNIT_FAIL ("insert worked on a read-only volume");
    }
    catch (FdoException* ge)
    {
        // check for 'read-only' somwehere in the message
        bool ok = contains (L"read-only", ge->GetExceptionMessage ());
        ge->Release ();
        CPPUNIT_ASSERT_MESSAGE ("no read-only message from Insert", ok);
    }
}

void ReadOnlyTests::update ()
{
    try
    {
        FdoPtr<FdoIUpdate> update = (FdoIUpdate*)mConnection->CreateCommand (FdoCommandType_Update);
        update->SetFeatureClassName (L"ontario");
        FdoPtr<FdoPropertyValueCollection> values = update->GetPropertyValues ();
        FdoPtr<FdoValueExpression> expression = FdoDecimalValue::Create (99.9999);
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (L"ONTARIO_ID", expression);
        values->Add (value);
        if (0 != update->Execute ())
            CPPUNIT_FAIL ("update really worked on a read-only volume");
        CPPUNIT_FAIL ("update worked on a read-only volume");
    }
    catch (FdoException* ge)
    {
        // check for 'read-only' somwehere in the message
        bool ok = contains (L"read-only", ge->GetExceptionMessage ());
        ge->Release ();
        CPPUNIT_ASSERT_MESSAGE ("no read-only message from Update", ok);
    }
}

void ReadOnlyTests::del ()
{
    try
    {
        FdoPtr<FdoIDelete> del = (FdoIDelete*)mConnection->CreateCommand (FdoCommandType_Delete);
        del->SetFeatureClassName (L"ontario");
        FdoInt32 n = del->Execute ();
        if (0 != del->Execute ())
            CPPUNIT_FAIL ("delete really worked on a read-only volume");
        CPPUNIT_FAIL ("delete worked on a read-only volume");
    }
    catch (FdoException* ge)
    {
        // check for 'read-only' somwehere in the message
        bool ok = contains (L"read-only", ge->GetExceptionMessage ());
        ge->Release ();
        CPPUNIT_ASSERT_MESSAGE ("no read-only message from Delete", ok);
    }
}

