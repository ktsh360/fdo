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

#include "stdafx.h"
#include "TestCommonInsert.h"
#include "TestCommonMiscUtil.h"

#define SCHEMA_NAME L"default_values"

TestCommonInsert::TestCommonInsert(void)
{
}

TestCommonInsert::~TestCommonInsert(void)
{
}

void TestCommonInsert::setUp ()
{
}

void TestCommonInsert::TestDefaultValues ()
{
    Context context(L"_default_values");

    DoTestDefaultValues( context );
}

void TestCommonInsert::DoTestDefaultValues (Context& context)
{
    try {
        // delete, re-create and open the datastore
        printf( "Initializing Connection ... \n" );
        CreateConnection( context, true );

        DoSubtestDefaultValues(
            context.connection,
            L"test1",
            true,
            42,
            L"timestamp '2008-10-24 8:15:49'",
            L"DATE '2001-5-1'",
            2.8,
            3.14159,
            1000,
            12345678,
            54321,
            (FdoFloat) -2.3e20,
            L"orange(brown)((red))"
        );

        DoSubtestDefaultValues(
            context.connection,
            L"test2",
            true,
            43,
            L"Date '2000-01-02'",
            L"TIMESTAMP '2000-12-31 23:55:05'",
            1.8,
            4.14159,
            1000,
            11345678,
            55321,
            (FdoFloat) -2.3e20,
            L"o'clock"
        );
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
}

void TestCommonInsert::DoSubtestDefaultValues(
    FdoIConnection* connection,
    FdoString* testName,
    FdoBoolean boolValue,
    FdoByte byteValue,
    FdoString* dateTimeValue1,
    FdoString* dateTimeValue2,
    FdoDouble decimalValue,
    FdoDouble doubleValue,
    FdoInt16 int16Value,
    FdoInt32 int32Value,
    FdoInt64 int64Value,
    FdoFloat singleValue,
    FdoString* stringValue
)
{
    printf( "Doing SubTest %ls ...\n", testName );

    FdoPtr<FdoISchemaCapabilities> cap = connection->GetSchemaCapabilities();

    FdoPtr<FdoIDescribeSchema>  descCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
    FdoFeatureSchemasP schemas = descCmd->Execute();
    FdoFeatureSchemaP schema = schemas->FindItem(GetDefaultSchemaName());
    if ( !schema )
        schema = FdoFeatureSchema::Create( GetDefaultSchemaName(), L"" );

    FdoPtr<FdoIApplySchema>  applyCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
    FdoClassesP classes = schema->GetClasses();

    FdoFeatureClassP featClass = FdoFeatureClass::Create( testName, L"AutoCAD entity base class" );
    classes->Add(featClass);
    featClass->SetIsAbstract(false);
    FdoPropertiesP props = featClass->GetProperties();
    FdoDataPropertiesP idProps = featClass->GetIdentityProperties();

    FdoDataPropertyP dataProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
    dataProp->SetDataType( GetAutoIncrementType() );
    dataProp->SetNullable(false);
    dataProp->SetIsAutoGenerated(true);
    props->Add( dataProp );
    idProps->Add( dataProp );

    dataProp = FdoDataPropertyDefinition::Create( L"FeatId2", L"id" );
    dataProp->SetDataType( FdoDataType_Int32 );
    dataProp->SetNullable(false);
    props->Add( dataProp );

    if ( SupportsDataType(cap, FdoDataType_Boolean) ) {
        dataProp = FdoDataPropertyDefinition::Create( L"boolean", L"" );
        dataProp->SetDataType( FdoDataType_Boolean );
        dataProp->SetNullable(false);
        dataProp->SetDefaultValue(boolValue ? L"true" : L"false");
        props->Add( dataProp );
    }

    if ( SupportsDataType(cap, FdoDataType_Byte) ) {
        dataProp = FdoDataPropertyDefinition::Create( L"byte", L"" );
        dataProp->SetDataType( FdoDataType_Byte );
        dataProp->SetNullable(false);
        dataProp->SetDefaultValue(FdoStringP::Format(L"%d", byteValue));
        props->Add( dataProp );
    }

    if ( SupportsDataType(cap, FdoDataType_DateTime) ) {
        dataProp = FdoDataPropertyDefinition::Create( L"datetime1", L"" );
        dataProp->SetDataType( FdoDataType_DateTime );
        dataProp->SetNullable(false);
        dataProp->SetDefaultValue(dateTimeValue1);
        props->Add( dataProp );
 
        dataProp = FdoDataPropertyDefinition::Create( L"datetime2", L"" );
        dataProp->SetDataType( FdoDataType_DateTime );
        dataProp->SetNullable(false);
        dataProp->SetDefaultValue(dateTimeValue2);
        props->Add( dataProp );
    }

    if ( SupportsDataType(cap, FdoDataType_Decimal) ) {
        dataProp = FdoDataPropertyDefinition::Create( L"decimal", L"" );
        dataProp->SetDataType( FdoDataType_Decimal );
        dataProp->SetPrecision(10);
        dataProp->SetScale(2);
        dataProp->SetNullable(false);
        dataProp->SetDefaultValue(FdoStringP::Format(L"%lf", decimalValue));
        props->Add( dataProp );
    }

    if ( SupportsDataType(cap, FdoDataType_Double) ) {
        dataProp = FdoDataPropertyDefinition::Create( L"double", L"" );
        dataProp->SetDataType( FdoDataType_Double );
        dataProp->SetNullable(false);
        dataProp->SetDefaultValue(FdoStringP::Format(L"%lf", doubleValue));
        props->Add( dataProp );
    }

    if ( SupportsDataType(cap, FdoDataType_Int16) ) {
        dataProp = FdoDataPropertyDefinition::Create( L"int16", L"" );
        dataProp->SetDataType( FdoDataType_Int16 );
        dataProp->SetNullable(false);
        dataProp->SetDefaultValue(FdoStringP::Format(L"%d", int16Value));
        props->Add( dataProp );
    }

    dataProp = FdoDataPropertyDefinition::Create( L"int32", L"" );
    dataProp->SetDataType( FdoDataType_Int32 );
    dataProp->SetNullable(false);
    dataProp->SetDefaultValue(FdoStringP::Format(L"%d", int32Value));
    props->Add( dataProp );

    if ( SupportsDataType(cap, FdoDataType_Int64) ) {
        dataProp = FdoDataPropertyDefinition::Create( L"int64", L"" );
        dataProp->SetDataType( FdoDataType_Int64 );
        dataProp->SetNullable(false);
        dataProp->SetDefaultValue(FdoCommonStringUtil::Int64ToString(int64Value));
        props->Add( dataProp );
    }

    if ( SupportsDataType(cap, FdoDataType_Single) ) {
        dataProp = FdoDataPropertyDefinition::Create( L"single", L"" );
        dataProp->SetDataType( FdoDataType_Single );
        dataProp->SetNullable(false);
        dataProp->SetDefaultValue(FdoStringP::Format(L"%f", singleValue));
        props->Add( dataProp );
    }

    dataProp = FdoDataPropertyDefinition::Create( L"string", L"" );
    dataProp->SetDataType( FdoDataType_String );
    dataProp->SetLength(20);
    dataProp->SetNullable(false);
    dataProp->SetDefaultValue(stringValue);
    props->Add( dataProp );

    applyCmd->SetFeatureSchema( schema );
    applyCmd->Execute();
    applyCmd = NULL;

    FdoPtr<FdoIInsert> insertCmd = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
    insertCmd->SetFeatureClassName(featClass->GetQualifiedName());
    FdoPtr<FdoPropertyValueCollection> propertyValues = insertCmd->GetPropertyValues();

    // Feature 1 tests RDBMS level setting of default values. Property values
    // not specified at API level.

    FdoPtr<FdoDataValue> dataValue = FdoInt32Value::Create(1);
    FdoPtr<FdoPropertyValue> propertyValue = TestCommonMiscUtil::AddNewProperty( propertyValues, L"FeatId2");
    propertyValue->SetValue(dataValue);

    FdoPtr<FdoIFeatureReader> insRdr;

    if ( InsertsDefaultValues() ) {
        insRdr = insertCmd->Execute();
        insRdr->Close();
        insRdr = NULL;
    }

    // Feature 2 tests DescribeSchema retrieval of default values. Default values
    // described and then set explicitly.

    insertCmd = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
    insertCmd->SetFeatureClassName(featClass->GetQualifiedName());
    propertyValues = insertCmd->GetPropertyValues();

    descCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
  	schemas = descCmd->Execute();
    FdoFeatureSchemaP schema2 = schemas->FindItem(GetDefaultSchemaName());
    CPPUNIT_ASSERT(schema2);
    FdoClassesP classes2 = schema2->GetClasses();
    FdoClassDefinitionP classDef2 = classes2->FindItem(testName);
    FdoPropertiesP props2 = classDef2->GetProperties();

    dataValue = FdoInt32Value::Create(2);
    propertyValue = TestCommonMiscUtil::AddNewProperty( propertyValues, L"FeatId2");
    propertyValue->SetValue(dataValue);

    int defaultCount = 0;

    for ( int i = 0; i < props2->GetCount(); i++ ) {
        FdoPropertyP prop2 = props2->GetItem(i);
        dataValue = NULL;

        if ( prop2->GetPropertyType() == FdoPropertyType_DataProperty ) {
            FdoDataPropertyDefinition* dataProp =
                static_cast<FdoDataPropertyDefinition*>(prop2.p);

            FdoStringP defaultStr = dataProp->GetDefaultValue();

            if ( defaultStr != L"" ) {
                dataValue = FdoDataValue::Create(
                    dataProp->GetDataType(),
                    FdoPtr<FdoStringValue>(FdoStringValue::Create(defaultStr))
                );

                if ( dataValue ) {
                    propertyValue = TestCommonMiscUtil::AddNewProperty( propertyValues, prop2->GetName() );
                    propertyValue->SetValue(FixDataValueType(dataValue, dataProp->GetDataType()));
                    defaultCount++;
                }
            }
        }
    }

    CPPUNIT_ASSERT( defaultCount == (props2->GetCount() - 2) );

    insRdr = insertCmd->Execute();
    insRdr->Close();
    insRdr = NULL;

    FdoPtr<FdoISelect> selectCmd = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);
    selectCmd->SetFeatureClassName(featClass->GetQualifiedName());

    FdoPtr<FdoIFeatureReader> rdr = selectCmd->Execute();

    CPPUNIT_ASSERT(rdr);

    int count = 0;

    while ( rdr->ReadNext() ) {
        count++;

        if ( SupportsDataType(cap, FdoDataType_Boolean) )
            CPPUNIT_ASSERT( boolValue == rdr->GetBoolean(L"boolean") );
        if ( SupportsDataType(cap, FdoDataType_Byte) )
            CPPUNIT_ASSERT( byteValue == rdr->GetByte(L"byte") );

        if ( SupportsDataType(cap, FdoDataType_DateTime) ) {
            VldDateTime( dateTimeValue1, rdr, L"datetime1" );
            VldDateTime( dateTimeValue2, rdr, L"datetime2" );
        }

        if ( SupportsDataType(cap, FdoDataType_Decimal) )
            CPPUNIT_ASSERT( fabs(decimalValue - rdr->GetDouble(L"decimal")) < 0.001 );
        if ( SupportsDataType(cap, FdoDataType_Double) )
            CPPUNIT_ASSERT( doubleValue == rdr->GetDouble(L"double") );
        if ( SupportsDataType(cap, FdoDataType_Int16) )
            CPPUNIT_ASSERT( int16Value == rdr->GetInt16(L"int16") );
        CPPUNIT_ASSERT( int32Value == rdr->GetInt32(L"int32") );
        if ( SupportsDataType(cap, FdoDataType_Int64) )
            CPPUNIT_ASSERT( int64Value == rdr->GetInt64(L"int64") );
        if ( SupportsDataType(cap, FdoDataType_Single) )
            CPPUNIT_ASSERT( singleValue == rdr->GetSingle(L"single") );
        CPPUNIT_ASSERT( wcscmp(stringValue, rdr->GetString(L"string")) == 0 );
    }

    CPPUNIT_ASSERT( count == (InsertsDefaultValues() ? 2 : 1) );
}

void TestCommonInsert::CreateConnection( Context&, FdoBoolean )
{
    throw FdoException::Create( L"base TestCommonInsert::CreateConnection called, need implementation on subclass" );
}

void TestCommonInsert::VldDateTime( FdoStringP expected, FdoIFeatureReader* rdr, FdoStringP propName )
{
    FdoPtr<FdoExpression> expr = FdoExpression::Parse(expected);
    FdoPtr<FdoDateTimeValue> expDtv = FDO_SAFE_ADDREF(dynamic_cast<FdoDateTimeValue*>(expr.p));
        
    FdoDateTime expDt = expDtv->GetDateTime();
    if ( DefaultsTime() ) {
        if ( expDt.hour == -1 ) {
            expDt.hour = 0;
            expDt.minute = 0;
            expDt.seconds = 0;
        }
    }
    else {
        if ( !SupportsTime() ) 
            expDt.hour = -1;
    }
    expDtv->SetDateTime(expDt);

    FdoDateTime dt = rdr->GetDateTime(propName);

    if ( (expDt.year == -1) && !SupportsTimeOnly() )
        dt.year = -1;

    CPPUNIT_ASSERT( wcscmp(expDtv->ToString(), FdoPtr<FdoDateTimeValue>(FdoDateTimeValue::Create(dt))->ToString()) == 0 );
}

FdoPtr<FdoDataValue> TestCommonInsert::FixDataValueType( FdoPtr<FdoDataValue> val, FdoDataType propType )
{
	FdoInt32		valInt32;
	FdoInt64		valInt64;
	FdoDouble		valDouble;

	FdoPtr<FdoDataValue>	ret = val;

	// Fast return if nothing to do
	if ( val == NULL )
		return ret;

	FdoDataType	constrType = val->GetDataType();

	if ( constrType != propType ) 
	{
		FdoDataValue	*newData = NULL;

        switch (constrType) {

        case FdoDataType_Int32:
			valInt32 = ((FdoInt32Value*)(FdoDataValue*)val)->GetInt32();

            switch (propType) {
            case FdoDataType_Byte:
			    newData = FdoDataValue::Create( (FdoByte)valInt32 );
                break;

            case FdoDataType_Int16:
			    newData = FdoDataValue::Create( (FdoInt16)valInt32 );
                break;

            case FdoDataType_Int64:
			    newData = FdoDataValue::Create( (FdoInt64)valInt32);
                break;

            case FdoDataType_Single:
			    newData = FdoDataValue::Create( (FdoFloat)valInt32);
                break;

            case FdoDataType_Double:
            case FdoDataType_Decimal:
    			newData = FdoDataValue::Create( (FdoDouble) valInt32, propType );
                break;
            }

            break;

        case FdoDataType_Int64:
			valInt64 = ((FdoInt64Value*)(FdoDataValue*)val)->GetInt64();

            switch (propType) {
            case FdoDataType_Byte:
			    newData = FdoDataValue::Create( (FdoByte)valInt64 );
                break;

            case FdoDataType_Int16:
			    newData = FdoDataValue::Create( (FdoInt16)valInt64 );
                break;

            case FdoDataType_Int32:
			    newData = FdoDataValue::Create( (FdoInt32)valInt64);
                break;

            case FdoDataType_Single:
			    newData = FdoDataValue::Create( (FdoFloat)valInt64);
                break;

            case FdoDataType_Double:
            case FdoDataType_Decimal:
    			newData = FdoDataValue::Create( (FdoDouble) valInt64, propType );
                break;
            }

            break;

        case FdoDataType_Double:
			valDouble = ((FdoDoubleValue*)(FdoDataValue*)val)->GetDouble();

            switch (propType) {
            case FdoDataType_Byte:
			    newData = FdoDataValue::Create( (FdoByte)valDouble );
                break;

            case FdoDataType_Int16:
			    newData = FdoDataValue::Create( (FdoInt16)valDouble );
                break;

            case FdoDataType_Int32:
			    newData = FdoDataValue::Create( (FdoInt32)valDouble );
                break;

            case FdoDataType_Int64:
			    newData = FdoDataValue::Create( (FdoInt64)valDouble);
                break;

            case FdoDataType_Single:
			    newData = FdoDataValue::Create( (FdoFloat)valDouble);
                break;

            case FdoDataType_Decimal:
    			newData = FdoDataValue::Create( valDouble, propType );
                break;
            }

            break;

        }

        ret = newData;
	}
	return ret;
}


bool TestCommonInsert::SupportsDataType( FdoISchemaCapabilities* cap, FdoDataType dt )
{
    FdoInt32 len;
    FdoDataType* types = cap->GetDataTypes( len );

    for ( int i = 0; i < len; i++ ) {
        if ( types[i] == dt ) 
            return true;
    }

    return false;
}

bool TestCommonInsert::DefaultsTime()
{
    return false;
}

bool TestCommonInsert::SupportsTime()
{
    return true;
}

bool TestCommonInsert::SupportsTimeOnly()
{
    return true;
}

bool TestCommonInsert::InsertsDefaultValues()
{
    return true;
}

FdoDataType TestCommonInsert::GetAutoIncrementType()
{
    return FdoDataType_Int64;
}

TestCommonInsert::Context::Context( FdoString* suffix ) 
{
    mSuffix = suffix;
}

TestCommonInsert::Context::~Context() 
{
    if ( connection ) {
        try {
            connection->Close();
        }
        catch ( ... ) {
        }
    }
}

FdoString* TestCommonInsert::GetDefaultSchemaName(void)
{
    return SCHEMA_NAME;
}
