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
#include "FdoAssociationSchemaTest.h"
#include "UnitTestUtil.h"

#define DB_SUFFIX	L"_assoc"

FdoAssociationSchemaTest::FdoAssociationSchemaTest(void)
{
    
}

FdoAssociationSchemaTest::~FdoAssociationSchemaTest(void)
{
    
}

void FdoAssociationSchemaTest::setUp ()
{
    set_provider();
}

void FdoAssociationSchemaTest::tearDown ()
{
}

void FdoAssociationSchemaTest::createFreshDb()
{
	try
    {
		FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore, L"");
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        connection->SetConnectionString( userConnectString );
        connection->Open();
		FdoPtr<FdoIDestroyDataStore> pDelCmd = (FdoIDestroyDataStore*)connection->CreateCommand( FdoCommandType_DestroyDataStore );
		FdoPtr<FdoIDataStorePropertyDictionary> dictionary = pDelCmd->GetDataStoreProperties();
		dictionary->SetProperty( L"DataStore", UnitTestUtil::GetEnviron("datastore", DB_SUFFIX) );
		pDelCmd->Execute();
		connection->Close();
    }
    catch(FdoException* e) {e->Release();}
    catch(...) { }

	UnitTestUtil::CreateDB(false, false, DB_SUFFIX);
}

void FdoAssociationSchemaTest::TestCreate_NoIdent_MultiSchema ()
{ 
    TestCreate( false, false ); 
    FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX, false);

    FdoPtr<FdoIDescribeSchema> descCmd = (FdoIDescribeSchema*) connection->CreateCommand( FdoCommandType_DescribeSchema );

    FdoFeatureSchemasP schemas = descCmd->Execute();
    FdoFeatureSchemaP baseSchema = schemas->FindItem(L"AssociationSchema");
    CPPUNIT_ASSERT(baseSchema);

    FdoClassesP classes = baseSchema->GetClasses();
    FdoClassDefinitionP baseClass = classes->FindItem(L"TestFeatureClass");
    CPPUNIT_ASSERT(baseClass);

    FdoFeatureSchemaP newSchema = FdoFeatureSchema::Create(L"SubSchema", L"");
    classes = newSchema->GetClasses();

    FdoFeatureClassP newClass = FdoFeatureClass::Create(L"TestSubFeatureClass",L"");
    newClass->SetBaseClass(baseClass);
    classes->Add(newClass);

    FdoPtr<FdoIApplySchema> applyCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
    applyCmd->SetFeatureSchema(newSchema);
    applyCmd->Execute();

    descCmd = (FdoIDescribeSchema*) connection->CreateCommand( FdoCommandType_DescribeSchema );
    schemas = descCmd->Execute();
    FdoFeatureSchemaP schema = schemas->FindItem(L"SubSchema");
    CPPUNIT_ASSERT(schema);

    FdoClassesP classes2 = schema->GetClasses();
    FdoClassDefinitionP classDef = classes2->FindItem(L"TestSubFeatureClass");
    CPPUNIT_ASSERT(classDef);

    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = classDef->GetBaseProperties();

    int foundCount = 0;
    for ( int i = 0; i < baseProps->GetCount(); i++ ) 
    {
        FdoPropertyP prop = baseProps->GetItem(i);
        FdoStringP propName = prop->GetName();

        if ( (propName == L"Association Prop1") || (propName == L"Association Prop2") ) 
        {
            foundCount++;

            CPPUNIT_ASSERT(prop->GetPropertyType() == FdoPropertyType_AssociationProperty);
        }
    }

    CPPUNIT_ASSERT(foundCount == 2);
}

void FdoAssociationSchemaTest::TestCreate ( bool useIdent, bool useObjProp, bool useNestedObj, bool useTransaction, bool commitTransaction, bool associatedIsFeat, bool ownerIsFeat, bool addToSubclass, int circularType )
{
	createFreshDb();
    FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX, false);
    FdoPtr<FdoITransaction> featureTransaction;

    try
    {
        if( useTransaction )
            featureTransaction = connection->BeginTransaction();
/*
        try
        {
            FdoPtr<FdoIDestroySchema> pDelCmd = (FdoIDestroySchema*) connection->CreateCommand(FdoCommandType_DestroySchema);
	        pDelCmd->SetSchemaName( L"TestAssociation" );
            CPPUNIT_ASSERT(wcscmp(pDelCmd->GetSchemaName(),L"TestAssociation") == 0);
	        pDelCmd->Execute();
        }
        catch (FdoSchemaException* e )
	    {
		    UnitTestUtil::PrintException(e, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ) );
		    FDO_SAFE_RELEASE(e);
        }
*/
        //
        // (re)create the schema
       // FdoPtr<FdoFeatureSchemaCollection> pSchemas = FdoFeatureSchemaCollection::Create(NULL);
	    FdoPtr<FdoFeatureSchema> pTestSchema = FdoFeatureSchema::Create( L"AssociationSchema", L"Test Association schema" );
       // pSchemas->Add( pTestSchema );


        FdoPtr<FdoClassDefinition>    pfeatureclass;
        FdoPtr<FdoDataPropertyDefinition> pProp;
        // Create a feature class
        FdoPtr<FdoPropertyDefinitionCollection> propDefColl;
        FdoPtr<FdoDataPropertyDefinitionCollection> propDataDefColl;
        if( ownerIsFeat )
        {
            pfeatureclass = FdoFeatureClass::Create(L"TestFeatureClass", L"FeatureClass Desc");
            // Add identity property
            pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"FeatId Prop" );
	        pProp->SetDataType( FdoDataType_Int64 );
	        pProp->SetNullable(false);
            pProp->SetIsAutoGenerated(true);
            propDefColl = pfeatureclass->GetProperties();
            propDefColl->Add( pProp );
	        propDataDefColl = pfeatureclass->GetIdentityProperties();
            propDataDefColl->Add( pProp );
        }
        else
        {
            pfeatureclass = FdoClass::Create(L"TestFeatureClass", L"FeatureClass Desc");
            pProp = FdoDataPropertyDefinition::Create( L"Id", L"Id Prop" );
	        pProp->SetDataType( FdoDataType_Int64 );
	        pProp->SetNullable(false);
            propDefColl = pfeatureclass->GetProperties();
            propDefColl->Add( pProp );
	        propDataDefColl = pfeatureclass->GetIdentityProperties();
            propDataDefColl->Add( pProp );
        }
        // Add first name and last name properties
        pProp = FdoDataPropertyDefinition::Create( L"First Name", L"First Name" );
	    pProp->SetDataType( FdoDataType_String );
        pProp->SetLength(32);
	    pProp->SetNullable(false);
	    propDefColl = pfeatureclass->GetProperties();
        propDefColl->Add( pProp );
        pProp = FdoDataPropertyDefinition::Create( L"Last Name", L"Last Name" );
	    pProp->SetDataType( FdoDataType_String );
        pProp->SetLength(32);
	    pProp->SetNullable(false);
	    propDefColl = pfeatureclass->GetProperties();
        propDefColl->Add( pProp );
        pProp = FdoDataPropertyDefinition::Create( L"ParentId", L"FeatId Prop" );
        pProp->SetDataType( FdoDataType_Int64 );
        pProp->SetNullable(true);
        propDefColl = pfeatureclass->GetProperties();
        propDefColl->Add( pProp );


        // Create a class
        FdoPtr<FdoClassDefinition> pclass;
        if( associatedIsFeat )
        {
            pclass = FdoFeatureClass::Create(L"TestClass", L"Class Desc");
            pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"FeatId Prop" );
	        pProp->SetDataType( FdoDataType_Int64 );
	        pProp->SetNullable(false);
            pProp->SetIsAutoGenerated(true);
            propDefColl = pclass->GetProperties();
            propDefColl->Add( pProp );
	        propDataDefColl = pclass->GetIdentityProperties();
            propDataDefColl->Add( pProp );
        }
        else
        {
            pclass = FdoClass::Create(L"TestClass", L"Class Desc");
            // Add identity property
            pProp = FdoDataPropertyDefinition::Create( L"Id", L"Id Prop" );
	        pProp->SetDataType( FdoDataType_Int64 );
	        pProp->SetNullable(false);
            propDefColl = pclass->GetProperties();
            propDefColl->Add( pProp );
	        propDataDefColl = pclass->GetIdentityProperties();
            propDataDefColl->Add( pProp );
        }
        // Add name one and name two properties
        pProp = FdoDataPropertyDefinition::Create( L"Name One", L"Name One" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetNullable(false);
        pProp->SetLength(32);
	    propDefColl = pclass->GetProperties();
        propDefColl->Add( pProp );
        pProp = FdoDataPropertyDefinition::Create( L"Name Two", L"Name Two" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetNullable(false);
        pProp->SetLength(32);
	    propDefColl = pclass->GetProperties();
        propDefColl->Add( pProp );
        pProp = FdoDataPropertyDefinition::Create( L"ParentId", L"Id Prop" );
        pProp->SetDataType( FdoDataType_Int64 );
        pProp->SetNullable(true);
        propDefColl = pclass->GetProperties();
        propDefColl->Add( pProp );

        // Create a class
        FdoPtr<FdoClass> pclassLeafObj = FdoClass::Create(L"TestSuperObjClass", L"Obj Class Desc");
        // Add identity property
        pProp = FdoDataPropertyDefinition::Create( L"Id", L"ObjId Prop" );
	    pProp->SetDataType( FdoDataType_Int64 );
	    pProp->SetNullable(false);
        propDefColl = pclassLeafObj->GetProperties();
        propDefColl->Add( pProp );
	    propDataDefColl = pclassLeafObj->GetIdentityProperties();
        propDataDefColl->Add( pProp );
        // Add name one and name two properties
        pProp = FdoDataPropertyDefinition::Create( L"First Name", L"Name One" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetNullable(false);
        pProp->SetLength(32);
	    propDefColl = pclassLeafObj->GetProperties();
        propDefColl->Add( pProp );
        pProp = FdoDataPropertyDefinition::Create( L"Last Name", L"Name Two" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetNullable(false);
        pProp->SetLength(32);
	    propDefColl = pclassLeafObj->GetProperties();
        propDefColl->Add( pProp );

        // Create a class
        FdoPtr<FdoClass> pclassObj = FdoClass::Create(L"TestOC", L"Obj Class Desc");
        // Add identity property
        pProp = FdoDataPropertyDefinition::Create( L"Id", L"ObjId Prop" );
	    pProp->SetDataType( FdoDataType_Int64 );
	    pProp->SetNullable(false);
        propDefColl = pclassObj->GetProperties();
        propDefColl->Add( pProp );
	    propDataDefColl = pclassObj->GetIdentityProperties();
        propDataDefColl->Add( pProp );
        // Add name one and name two properties
        pProp = FdoDataPropertyDefinition::Create( L"First Name", L"Name One" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetNullable(false);
        pProp->SetLength(32);
	    propDefColl = pclassObj->GetProperties();
        propDefColl->Add( pProp );
        pProp = FdoDataPropertyDefinition::Create( L"Last Name", L"Name Two" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetNullable(false);
        pProp->SetLength(32);
	    propDefColl = pclassObj->GetProperties();
        propDefColl->Add( pProp );
        
        // Create a feature Sub class class
        FdoPtr<FdoClassDefinition> pSubFeatureclass;
        if( ownerIsFeat )
        {
            pSubFeatureclass = FdoFeatureClass::Create(L"TestSubFeatureClass", L"A subclass from a featureClass");

            pSubFeatureclass->SetBaseClass( pfeatureclass );
            // Add a regular property
            pProp = FdoDataPropertyDefinition::Create( L"Id", L"An Id Prop" );
	        pProp->SetDataType( FdoDataType_Int64 );
	        pProp->SetNullable(true);
            propDefColl = pSubFeatureclass->GetProperties();
            propDefColl->Add( pProp );
        }
        else
        {
            pSubFeatureclass = FdoClass::Create(L"TestSubFeatureClass", L"A subclass from a featureClass");
            pSubFeatureclass->SetBaseClass( pfeatureclass );
        }
        // Add the classes to the schema class collection
        FdoClassesP clsColl = pTestSchema->GetClasses();
        clsColl->Add( pfeatureclass );
        clsColl->Add( pclass );
        clsColl->Add( pclassLeafObj );
        clsColl->Add( pclassObj );
        clsColl->Add( pSubFeatureclass );

        
        // Create a valid association property
        FdoPtr<FdoAssociationPropertyDefinition> passprop = FdoAssociationPropertyDefinition::Create(L"Association Prop1", L"Association Prop Desc");
        
        passprop->SetLockCascade( true );
        passprop->SetDeleteRule( FdoDeleteRule_Break );
        passprop->SetReverseName( L"Reverse Name" );
        passprop->SetMultiplicity(L"m");
        passprop->SetReverseMultiplicity(L"1");
        if( useIdent )
        {
            propDefColl = pclass->GetProperties();
            propDataDefColl = passprop->GetIdentityProperties();
            FdoPtr<FdoDataPropertyDefinition> pprop = (FdoDataPropertyDefinition*)propDefColl->GetItem( L"Name One" );
            propDataDefColl->Add( pprop );
           
            propDataDefColl->Add( FdoPtr<FdoDataPropertyDefinition>( (FdoDataPropertyDefinition*)propDefColl->GetItem( L"Name Two" )));
            propDefColl = pfeatureclass->GetProperties();
            propDataDefColl = passprop->GetReverseIdentityProperties();
            propDataDefColl->Add( FdoPtr<FdoDataPropertyDefinition>( (FdoDataPropertyDefinition*)propDefColl->GetItem( L"First Name" )));
            propDataDefColl->Add( FdoPtr<FdoDataPropertyDefinition>( (FdoDataPropertyDefinition*)propDefColl->GetItem( L"Last Name" )));
        }
        passprop->SetAssociatedClass(pclass);

        

        
        if ( circularType > 0 ) 
        {
            propDefColl = pfeatureclass->GetProperties();
            propDefColl->Add( passprop );
        }
        else if( ! useObjProp )
        {

            // Add a second association property
            FdoPtr<FdoAssociationPropertyDefinition> passprop2 = FdoAssociationPropertyDefinition::Create(L"Association Prop2", L"Second Association Prop Desc");    
            passprop2->SetLockCascade( true );
            passprop2->SetDeleteRule( FdoDeleteRule_Break );
            //passprop2->SetReverseName( L"Reverse Name" );
            passprop2->SetMultiplicity(L"m");
            passprop2->SetReverseMultiplicity(L"0_1");
            passprop2->SetAssociatedClass(pclass);

            // Add the association property to the feature class property collection
            if( addToSubclass )
            {
                propDefColl = pSubFeatureclass->GetProperties();
                propDefColl->Add( passprop );
                propDefColl->Add( passprop2 );
            }
            else
            {
                propDefColl = pfeatureclass->GetProperties();
                propDefColl->Add( passprop );
                propDefColl->Add( passprop2 );
            }
        }
        else
        {
            if( useNestedObj )
            {
                // Add the association property to the leaf class of the obj class
                propDefColl = pclassLeafObj->GetProperties();
                propDefColl->Add( passprop );
                FdoPtr<FdoObjectPropertyDefinition>pLeafObjPropData = FdoObjectPropertyDefinition::Create( L"LeafObject", L"object property" );
                pLeafObjPropData->SetClass( pclassLeafObj );
                pLeafObjPropData->SetObjectType( FdoObjectType_Value );
                propDefColl = pclassObj->GetProperties();
                propDefColl->Add( pLeafObjPropData );
            }
            else
            {
                // Add the association property to the Obj class property collection
                propDefColl = pclassObj->GetProperties();
                propDefColl->Add( passprop );
            }
            // Add an object property that contain an association
            FdoPtr<FdoObjectPropertyDefinition>pObjPropData = FdoObjectPropertyDefinition::Create( L"Object", L"object property" );
            pObjPropData->SetClass( pclassObj );
            pObjPropData->SetObjectType( FdoObjectType_Value );
            propDefColl = pfeatureclass->GetProperties();
            propDefColl->Add( pObjPropData );
        }

        // Create a valid association property
        FdoPtr<FdoAssociationPropertyDefinition> circprop = FdoAssociationPropertyDefinition::Create(L"Association Circ Prop1", L"Association Prop Desc");
        circprop->SetMultiplicity(L"m");
        circprop->SetReverseMultiplicity(L"0");
        
        switch ( circularType ) 
        {
        // case 0 - no circular references.
        case 1:
            {
                // Circular FeatureClass to itself
                propDataDefColl = pfeatureclass->GetIdentityProperties();
                FdoPtr<FdoDataPropertyDefinition> pprop = (FdoDataPropertyDefinition*)propDataDefColl->GetItem( 0 );
                propDataDefColl = circprop->GetIdentityProperties();
                propDataDefColl->Add( pprop );
                propDefColl = pfeatureclass->GetProperties();
                propDataDefColl = circprop->GetReverseIdentityProperties();
                pprop = (FdoDataPropertyDefinition*)propDefColl->GetItem( L"ParentId" );
                propDataDefColl->Add( pprop );
                circprop->SetAssociatedClass(pfeatureclass);
                propDefColl = pfeatureclass->GetProperties();
	            propDefColl->Add( circprop );           
            }
            break;

        case 2:
            {
                // Circular TestClass to itself
                propDataDefColl = pclass->GetIdentityProperties();
                FdoPtr<FdoDataPropertyDefinition> pprop = (FdoDataPropertyDefinition*)propDataDefColl->GetItem( 0 );
                propDataDefColl = circprop->GetIdentityProperties();
                propDataDefColl->Add( pprop );
                propDefColl = pclass->GetProperties();
                propDataDefColl = circprop->GetReverseIdentityProperties();
                pprop = (FdoDataPropertyDefinition*)propDefColl->GetItem( L"ParentId" );
                propDataDefColl->Add( pprop );
                circprop->SetAssociatedClass(pclass);
                propDefColl = pclass->GetProperties();
	            propDefColl->Add( circprop );           
            }
            break;

        case 3:
            {
                // TestClass to FeatClass - creates circular reference
                propDataDefColl = pfeatureclass->GetIdentityProperties();
                FdoPtr<FdoDataPropertyDefinition> pprop = (FdoDataPropertyDefinition*)propDataDefColl->GetItem( 0 );
                propDataDefColl = circprop->GetIdentityProperties();
                propDataDefColl->Add( pprop );
                propDefColl = pclass->GetProperties();
                propDataDefColl = circprop->GetReverseIdentityProperties();
                pprop = (FdoDataPropertyDefinition*)propDefColl->GetItem( L"ParentId" );
                propDataDefColl->Add( pprop );
                circprop->SetAssociatedClass(pfeatureclass);
                propDefColl = pclass->GetProperties();
	            propDefColl->Add( circprop );           
            }
            break;
        }

        FdoPtr<FdoIApplySchema>pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

        pCmd->SetFeatureSchema( pTestSchema );

	    pCmd->Execute();

        if( useTransaction )
        {
            if( commitTransaction )
                featureTransaction->Commit();
            else
            {
                featureTransaction->Rollback();
            }
        }

    }
    catch ( FdoSchemaException* e )
	{
        printf("TestSchema Error: %ls\n", e->GetExceptionMessage() );
		UnitTestUtil::PrintException(e, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ) );
        e->Release();
		CPPUNIT_FAIL ( "Association schema TestCreate exception" );
	}
}

void FdoAssociationSchemaTest::TestLoad ( bool withObj )
{
    FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX, true);

    try
    {
        FdoPtr<FdoIDescribeSchema>pDescSchemaCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
        pDescSchemaCmd->SetSchemaName( L"AssociationSchema" );
        FdoPtr<FdoFeatureSchemaCollection>pfsc = pDescSchemaCmd->Execute();
       
        FdoPtr<FdoFeatureSchema> schema = pfsc->GetItem(L"AssociationSchema");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();
        FdoPtr<FdoClassDefinition> cls;
        FdoPtr<FdoClassDefinition> ascClass;

        cls = classes->GetItem(L"TestFeatureClass");
        if( cls )
        {
            FdoPtr<FdoAssociationPropertyDefinition> prop;
            FdoPtr<FdoPropertyDefinitionCollection> propCol = cls->GetProperties();
            if( withObj )
            {
                FdoPtr<FdoObjectPropertyDefinition> objProp = (FdoObjectPropertyDefinition*)propCol->GetItem(L"Object");
                cls = objProp->GetClass();
                propCol = cls->GetProperties();
            }
            prop = (FdoAssociationPropertyDefinition*)propCol->GetItem(L"Association Prop1");
            ascClass = prop->GetAssociatedClass();
            printf("Association: name(%ls) associated class(%ls) multiplicity(%ls)\n",
                prop->GetName(),
                ascClass->GetName(),
                prop->GetMultiplicity() );
            FdoPtr<FdoDataPropertyDefinitionCollection> identProps = prop->GetIdentityProperties();
            printf("Identity properties: \n");
            for(int i=0; i<identProps->GetCount(); i++ )
            {
                FdoPtr<FdoDataPropertyDefinition> idProp = identProps->GetItem(i);
                printf("\tIdent(%d): name(%ls) \n", i, idProp->GetName());
            }
            identProps = prop->GetReverseIdentityProperties();
            printf("Reverse Identity properties: \n");
            for(int i=0; i<identProps->GetCount(); i++ )
            {
                FdoPtr<FdoDataPropertyDefinition> idProp = identProps->GetItem(i);
                printf("\tRevIdent(%d): name(%ls) \n", i, idProp->GetName());
            }
        } 
        
        cls = classes->GetItem(L"TestSubFeatureClass");
        if( cls )
        {
            FdoPtr<FdoAssociationPropertyDefinition> prop;
            FdoPtr<FdoReadOnlyPropertyDefinitionCollection> propCol = cls->GetBaseProperties();
            if( withObj )
            {
                FdoPtr<FdoObjectPropertyDefinition> objProp = (FdoObjectPropertyDefinition*)propCol->GetItem(L"Object");
                cls = objProp->GetClass();
                FdoPtr<FdoPropertyDefinitionCollection> propsDefColl = cls->GetProperties();
                prop = (FdoAssociationPropertyDefinition*)propsDefColl->GetItem(L"Association Prop1");
            }
            else
                prop = (FdoAssociationPropertyDefinition*)propCol->GetItem(L"Association Prop1");
            ascClass = prop->GetAssociatedClass();
            printf("Association: name(%ls) associated class(%ls) multiplicity(%ls)\n",
                prop->GetName(),
                ascClass->GetName(),
                prop->GetMultiplicity() );
            FdoPtr<FdoDataPropertyDefinitionCollection> identProps = prop->GetIdentityProperties();
            printf("Identity properties: \n");
            for(int i=0; i<identProps->GetCount(); i++ )
            {
                FdoPtr<FdoDataPropertyDefinition> idProp = identProps->GetItem(i);
                printf("\tIdent(%d): name(%ls) \n", i, idProp->GetName());
            }
            identProps = prop->GetReverseIdentityProperties();
            printf("Reverse Identity properties: \n");
            for(int i=0; i<identProps->GetCount(); i++ )
            {
                FdoPtr<FdoDataPropertyDefinition> idProp = identProps->GetItem(i);
                printf("\tRevIdent(%d): name(%ls) \n", i, idProp->GetName());
            }
        } 
    }
    catch ( FdoSchemaException* e )
	{
        printf("TestLoad Error: %ls\n", e->GetExceptionMessage() );
		UnitTestUtil::PrintException(e, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ) );
        e->Release();
		CPPUNIT_FAIL ( "Association schema TestLoad exception" );
	}
}
#if 1
void FdoAssociationSchemaTest::TestDelete ( bool rollback )
{
    FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX, true);

    try
    {
        FdoPtr<FdoIDescribeSchema>pDescSchemaCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
        pDescSchemaCmd->SetSchemaName( L"AssociationSchema" );
        FdoPtr<FdoFeatureSchemaCollection>pfsc = pDescSchemaCmd->Execute();
       
        FdoPtr<FdoFeatureSchema> schema = pfsc->GetItem(L"AssociationSchema");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();
        FdoPtr<FdoClassDefinition> cls;

        cls = classes->FindItem(L"TestSubFeatureClass");
        if( cls )
        {
            /*FdoPtr<FdoAssociationPropertyDefinition> prop;
            FdoPtr<FdoPropertyDefinitionCollection> propCol = cls->GetProperties();
            prop = (FdoAssociationPropertyDefinition*)propCol->GetItem(L"Association Prop1");
            prop->Delete();
           */
            cls->Delete();
            FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
            // Update
            FdoPtr<FdoIApplySchema>pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
            pCmd->SetFeatureSchema( schema );
            pCmd->Execute();

            if( rollback )
                featureTransaction->Rollback();
            else
                featureTransaction->Commit();

        } 
    }
    catch ( FdoSchemaException* e )
	{
        printf("TestLoad Error: %ls\n", e->GetExceptionMessage() );
		UnitTestUtil::PrintException(e, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ) );
		e->Release();
		CPPUNIT_FAIL ( "Association schema TestDelete exception" );
	}
}

void FdoAssociationSchemaTest::TestCreateMultiple()
{
    createFreshDb();
    FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX, false);
    FdoPtr<FdoITransaction> featureTransaction;

    try
    {
	    FdoPtr<FdoFeatureSchema>pTestSchema = FdoFeatureSchema::Create( L"AssociationSchema", L"Test Association schema" );


        // Create a feature class
        FdoPtr<FdoFeatureClass>pfeatureclass1 = FdoFeatureClass::Create(L"TestClassOne", L"FeatureClass Desc");
        FdoPtr<FdoFeatureClass>pfeatureclass2 = FdoFeatureClass::Create(L"TestClassTwo", L"FeatureClass Desc");
        FdoPtr<FdoFeatureClass>pfeatureclass3 = FdoFeatureClass::Create(L"TestClassThree", L"FeatureClass Desc");
        FdoPtr<FdoFeatureClass>pfeatureclass4 = FdoFeatureClass::Create(L"TestClassFour", L"FeatureClass Desc");
        
        // Add identity property
        FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"FeatId Prop" );
	    pProp->SetDataType( FdoDataType_Int64 );
	    pProp->SetNullable(false);
        pProp->SetIsAutoGenerated(true);
        FdoPtr<FdoPropertyDefinitionCollection> propsColl = pfeatureclass1->GetProperties();
        propsColl->Add( pProp );
	    FdoPtr<FdoDataPropertyDefinitionCollection> propsIdColl = pfeatureclass1->GetIdentityProperties();
        propsIdColl->Add( pProp );

        pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"FeatId Prop" );
	    pProp->SetDataType( FdoDataType_Int64 );
	    pProp->SetNullable(false);
        pProp->SetIsAutoGenerated(true);
        propsColl = pfeatureclass2->GetProperties();
        propsColl->Add( pProp );
	    propsIdColl = pfeatureclass2->GetIdentityProperties();
        propsIdColl->Add( pProp );

        pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"FeatId Prop" );
	    pProp->SetDataType( FdoDataType_Int64 );
	    pProp->SetNullable(false);
        pProp->SetIsAutoGenerated(true);
        propsColl = pfeatureclass3->GetProperties();
        propsColl->Add( pProp );
	    propsIdColl = pfeatureclass3->GetIdentityProperties();
        propsIdColl->Add( pProp );

        pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"FeatId Prop" );
	    pProp->SetDataType( FdoDataType_Int64 );
	    pProp->SetNullable(false);
        pProp->SetIsAutoGenerated(true);
        propsColl = pfeatureclass4->GetProperties();
        propsColl->Add( pProp );
	    propsIdColl = pfeatureclass4->GetIdentityProperties();
        propsIdColl->Add( pProp );

        FdoPtr<FdoClassCollection> clsColl = pTestSchema->GetClasses();
        clsColl->Add( pfeatureclass1 );
        clsColl->Add( pfeatureclass2 );
        clsColl->Add( pfeatureclass3 );
        clsColl->Add( pfeatureclass4 );

        FdoPtr<FdoFeatureClass> pclass = FdoFeatureClass::Create(L"AssoClass", L"Associated FeatureClass Desc");
        pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"FeatId Prop" );
	    pProp->SetDataType( FdoDataType_Int64 );
	    pProp->SetNullable(false);
        pProp->SetIsAutoGenerated(true);
        propsColl = pclass->GetProperties();
        propsColl->Add( pProp );
	    propsIdColl = pclass->GetIdentityProperties();
        propsIdColl->Add( pProp );
        clsColl->Add( pclass );

        FdoPtr<FdoAssociationPropertyDefinition> passprop = FdoAssociationPropertyDefinition::Create(L"AssocProp1", L"Association Prop Desc");
        passprop->SetAssociatedClass(pclass);
        propsColl = pfeatureclass1->GetProperties();
        propsColl->Add( passprop );

        passprop = FdoAssociationPropertyDefinition::Create(L"AssocProp1", L"Association Prop Desc");
        passprop->SetAssociatedClass(pclass);
        propsColl = pfeatureclass2->GetProperties();
        propsColl->Add( passprop );

        passprop = FdoAssociationPropertyDefinition::Create(L"AssocProp1", L"Association Prop Desc");
        passprop->SetAssociatedClass(pclass);
        propsColl = pfeatureclass3->GetProperties();
        propsColl->Add( passprop );

        passprop = FdoAssociationPropertyDefinition::Create(L"AssocProp1", L"Association Prop Desc");
        passprop->SetAssociatedClass(pclass);
        propsColl = pfeatureclass4->GetProperties();
        propsColl->Add( passprop );

        FdoPtr<FdoIApplySchema>pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

        pCmd->SetFeatureSchema( pTestSchema );

	    pCmd->Execute();
    }
    catch ( FdoSchemaException* e )
	{
        printf("TestCreateMultiple Error: %ls\n", e->GetExceptionMessage() );
		UnitTestUtil::PrintException(e, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ) );
		e->Release();
		CPPUNIT_FAIL ( "Association schema TestCreateMultiple exception" );
	}
}

void FdoAssociationSchemaTest::TestLoadMultiple()
{
    FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_SUFFIX, true);

    try
    {
        FdoPtr<FdoIDescribeSchema>pDescSchemaCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
        pDescSchemaCmd->SetSchemaName( L"AssociationSchema" );
        FdoPtr<FdoFeatureSchemaCollection>pfsc = pDescSchemaCmd->Execute();
       
        FdoPtr<FdoFeatureSchema> schema = pfsc->GetItem(L"AssociationSchema");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();
        FdoPtr<FdoClassDefinition> cls;
        for(int i=0; i<classes->GetCount(); i++ )
        {
            cls = classes->GetItem(i);
            if( cls )
            {
                printf("Class: %ls\n", (FdoString *) cls->GetQualifiedName() );
                FdoPtr<FdoPropertyDefinition> prop;
                FdoPtr<FdoPropertyDefinitionCollection> propCol = cls->GetProperties();
                if( propCol )
                {
                    for(int j=0; j<propCol->GetCount(); j++ )
                    {
                        prop = propCol->GetItem( j );
                        printf("\tProp: %ls\n", prop->GetName() );
                    }
                }
            }
        }
    }
    catch ( FdoSchemaException* e )
	{
        printf("TestLoadMultiple Error: %ls\n", e->GetExceptionMessage() );
		UnitTestUtil::PrintException(e, UnitTestUtil::GetOutputFileName( L"TestSchema.txt" ) );
		e->Release();
		CPPUNIT_FAIL ( "Association schema TestLoadMultiple exception" );
	}
}
#endif
