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

#include "stdafx.h"
#define WFS_MESSAGE_DEFINE
#include <../Message/Inc/WFSMessage.h>

#include <malloc.h>
#include <string.h>
#include "FdoWfsDelegate.h"
#include "FdoWfsServiceMetadata.h"
#include <OWS/FdoOwsCapabilities.h>
#include <OWS/FdoOwsRequestMetadata.h>
#include "FdoWfsOgcFilterCapabilities.h"

// external access to connection for client services
extern "C" FDOWFS_API FdoIConnection* CreateConnection ()
{
   return (new FdoWfsConnection ());
}

FdoWfsConnection::FdoWfsConnection (void) :
    mConnectionString ((wchar_t*)NULL)
{
}

FdoWfsConnection::~FdoWfsConnection (void)
{
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing</returns> 
void FdoWfsConnection::Dispose ()
{
    delete this;
}

/// <summary>Gets an FdoIConnectionCapabilities interface describing the capabilities
/// of the connection.</summary>
/// <returns>Returns the connection capabilities</returns> 
FdoIConnectionCapabilities* FdoWfsConnection::GetConnectionCapabilities ()
{
    return (new FdoWfsConnectionCapabilities ());
}

/// <summary>Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.</summary>
/// <returns>Returns schema capabilities</returns> 
FdoISchemaCapabilities* FdoWfsConnection::GetSchemaCapabilities ()
{
    return (new FdoWfsSchemaCapabilities ());
}

/// <summary>Gets an FdoICommandCapabilities interface describing the commands a provider supports.</summary>
/// <returns>Returns the command capabilities</returns> 
FdoICommandCapabilities* FdoWfsConnection::GetCommandCapabilities ()
{
    return (new FdoWfsCommandCapabilities ());
}

/// <summary>Gets an FdoIFilterCapabilities interface describing the provider's support for filters.</summary>
/// <returns>Returns the filter capabilities</returns> 
FdoIFilterCapabilities* FdoWfsConnection::GetFilterCapabilities ()
{
    FdoPtr<FdoWfsOgcFilterCapabilities> ogcFilterCaps;
    if (mServiceMetadata != NULL)
        ogcFilterCaps = mServiceMetadata->GetOGCFilterCapabilities();
        
    return (new FdoWfsFilterCapabilities(ogcFilterCaps));
}

/// <summary>Gets an FdoIExpressionCapabilities interface describing the provider's support for expressions.</summary>
/// <returns>Returns the expression capabilities</returns> 
FdoIExpressionCapabilities* FdoWfsConnection::GetExpressionCapabilities ()
{
    return (new FdoWfsExpressionCapabilities ());
}

/// <summary>Gets an FdoIRasterCapabilities interface describing the provider's support for raster images.</summary>
/// <returns>Returns the raster capabilities</returns> 
FdoIRasterCapabilities* FdoWfsConnection::GetRasterCapabilities (void)
{
    return (new FdoWfsRasterCapabilities ());
}

/// <summary>Gets an FdoITopologyCapabilities interface describing the provider's support for topology.</summary>
/// <returns>Returns the topology capabilities</returns> 
FdoITopologyCapabilities* FdoWfsConnection::GetTopologyCapabilities ()
{
    return (new FdoWfsTopologyCapabilities ());
}

/// Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry
FdoIGeometryCapabilities* FdoWfsConnection::GetGeometryCapabilities()
{
    return (new FdoWfsGeometryCapabilities());
}

/// <summary>Gets the connection string used to open a DataStore.</summary>
/// <returns>Returns the connection string</returns> 
FdoString* FdoWfsConnection::GetConnectionString ()
{
    return (mConnectionString);
}

/// <summary>Sets the connection string used to open a DataStore. SetConnectionString can only be set while the
/// connection is closed.</summary>
/// <param name="value">Input the connection string</param> 
/// <returns>Returns nothing</returns> 
void FdoWfsConnection::SetConnectionString (FdoString* value)
{
    if ((GetConnectionState() == FdoConnectionState_Closed) || (GetConnectionState() == FdoConnectionState_Pending))
    {
        // Update the connection string:
        mConnectionString = value;

        // Update the connection property dictionary:
        FdoPtr<FdoWfsConnectionInfo> info = dynamic_cast<FdoWfsConnectionInfo*>(GetConnectionInfo ());
        FdoPtr<FdoCommonConnPropDictionary> dictionary = dynamic_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties ());
        dictionary->UpdateFromConnectionString (mConnectionString);
    }
    else
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_103_CONNECTION_ALREADY_OPEN)));
}

/// <summary>Gets an FdoIConnectionInfo interface that can be used to interrogate and set connection properties.</summary>
/// <returns>Returns the connection info</returns> 
FdoIConnectionInfo* FdoWfsConnection::GetConnectionInfo ()
{
    if (mConnectionInfo == NULL)
        mConnectionInfo = new FdoWfsConnectionInfo (this);
    return FDO_SAFE_ADDREF(mConnectionInfo.p);
}

/// <summary>Gets the current state of the connection.</summary>
/// <returns>Returns the current state of the connection</returns> 
FdoConnectionState FdoWfsConnection::GetConnectionState ()
{
    // return "Open" only when the service metadata is available
    if (mServiceMetadata != NULL)
        return FdoConnectionState_Open;
    return FdoConnectionState_Closed;

}

/// <summary>Gets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability 0 will be returned</summary>
/// <returns>Returns the time to wait (in milliseconds)</returns> 
FdoInt32 FdoWfsConnection::GetConnectionTimeout ()
{
    return (0);
}

/// <summary>Sets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability then attempting to
/// set a timeout will result in an exception</summary>
/// <param name="value">Input the time to wait (in milliseconds)</param> 
/// <returns>Returns nothing</returns> 
void FdoWfsConnection::SetConnectionTimeout (FdoInt32 value)
{
    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_67_CONNECTION_TIMEOUT_NOT_SUPPORTED)));
}

/// <summary>Opens a feature connection with the settings specified by the
/// ConnectionString attribute of the provider-specific feature connection
/// object.</summary>
/// <returns>Returns nothing</returns> 
FdoConnectionState FdoWfsConnection::Open ()
{
    if (GetConnectionState() == FdoConnectionState_Open)
        return FdoConnectionState_Open;

    // parse the connection string
	FdoStringP connectionStr = mConnectionString;
	while (connectionStr.GetLength() > 0)
	{
		FdoStringP onePair = connectionStr.Left(L";");
		connectionStr = connectionStr.Right(L";");
		FdoStringP key = onePair.Left(L"=");
		FdoStringP value = onePair.Right(L"=");
        if (key == FdoWfsGlobals::FeatureServer)
			mFeatureServer = value;
        else if (key == FdoWfsGlobals::Username)
            mUserName = value;
        else if (key == FdoWfsGlobals::Password)
            mPassword = value;
	}

    // set up the WFS delegate
    mDelegate = FdoWfsDelegate::Create(mFeatureServer, mUserName, mPassword);

    // try to get the service metadata
    mServiceMetadata = mDelegate->GetCapabilities();
    FdoPtr<FdoOwsRequestMetadataCollection> requestMetadatas = 
        FdoPtr<FdoOwsCapabilities>(mServiceMetadata->GetCapabilities())->GetRequestMetadatas();
    mDelegate->SetRequestMetadatas(requestMetadatas);
    

    return (GetConnectionState ());
}

/// <summary>Closes the connection to the DataStore</summary>
/// <returns>Returns nothing</returns> 
void FdoWfsConnection::Close ()
{
    mServiceMetadata = NULL;
    mDelegate = NULL;
}

/// <summary>Begins a transaction and returns an object that realizes
/// FdoITransaction.</summary>
/// <returns>Returns the transaction</returns> 
FdoITransaction* FdoWfsConnection::BeginTransaction ()
{
    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_101_TRANSACTIONS_NOT_SUPPORTED)));
}

/// <summary>Creates and returns the specified type of command object associated with
/// the connection.</summary>
/// <param name="commandType">Input the command type to be created</param> 
/// <returns>Returns the command</returns> 
FdoICommand* FdoWfsConnection::CreateCommand (FdoInt32 commandType)
{
    FdoPtr<FdoICommand> ret;

    switch (commandType)
    {
        case FdoCommandType_Select:
            ret = new FdoWfsSelectCommand (this);
            break;
        case FdoCommandType_SelectAggregates:
            ret = new FdoWfsSelectAggregatesCommand (this);
            break;        
        case FdoCommandType_DescribeSchema:
            ret = new FdoWfsDescribeSchemaCommand (this);
            break;
        case FdoCommandType_GetSpatialContexts:
            ret = new FdoWfsGetSpatialContexts (this);
            break;
        default:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_102_COMMAND_NOT_SUPPORTED, "The command '%1$ls' is not supported.", (FdoString*)(FdoCommonMiscUtil::FdoCommandTypeToString (commandType))));
    }

    return (FDO_SAFE_ADDREF (ret.p));
}


/// <summary>Factory function that creates an empty Schema Override set specific
/// to this FDO Provider.</summary>
/// <returns>Returns FdoPhysicalSchemaMapping</returns> 
FdoPhysicalSchemaMapping* FdoWfsConnection::CreateSchemaMapping()
{
	throw FdoException::Create (NlsMsgGet (WFS_SCHEMA_OVERRIDES_NOT_SUPPORTED, "WFS Provider does not support schema overrides."));
}


/// <summary>Sets the XML configuration stream used to configure the Data Store. 
/// SetConfiguration can only be called while the connection is closed.</summary>
/// <param name="configStream">Input the XML configuration stream</param> 
/// <returns>Returns nothing</returns> 
void FdoWfsConnection::SetConfiguration(FdoIoStream* configStream)
{
    throw FdoException::Create(NlsMsgGet(WFS_CONNECTION_CONFIGURATION_FILE_NOT_SUPPORTED, "WFS Provider does not support configuration files."));
}

FdoWfsDelegate* FdoWfsConnection::GetWfsDelegate ()
{
	return (FDO_SAFE_ADDREF(mDelegate.p));
}

FdoWfsServiceMetadata* FdoWfsConnection::GetServiceMetadata()
{
	return FDO_SAFE_ADDREF(mServiceMetadata.p);
}

FdoFeatureSchemaCollection* FdoWfsConnection::GetSchemas()
{
	if (mSchemas == NULL) {
        // First get the raw schemas from the schema document from the server
		mSchemas = mDelegate->DescribeFeatureType(NULL);

        // And then we have to make some adjustments to the raw schemas
        // to make the schema more user friendly.
        // We will iterate all the schema elements and corresponding mapping objects to:
        // 1. Remove the GML schemas which are useless for the end user
        // 2. Remove all the useless properties inherited from GML schema and XLink schema
        // 3. For those feature classes without any identity properties, make all non-null data properties as identity properties
        // 4. For those object properties whose class definition only contains one or more geometry properties,
        //    change the property type from object to geometry cause it's a customized geometry property and thus shouldn't
        //    be recognized as object property.
        FdoPtr<FdoPhysicalSchemaMappingCollection> mappings = mSchemas->GetXmlSchemaMappings();
        FdoInt32 count = mSchemas->GetCount();
        for (int i = count - 1; i >= 0; i--) {
            FdoPtr<FdoFeatureSchema> schema = mSchemas->GetItem(i);
            
            // Doing #1
            if (wcscmp(schema->GetName(), FdoWfsGlobals::GMLSchemaName) == 0 ||
                wcscmp(schema->GetName(), FdoWfsGlobals::XLinkSchemaName) == 0) {
                    mSchemas->RemoveAt(i);
                    continue;
                }

		    FdoPtr<FdoXmlSchemaMapping> mapping = static_cast<FdoXmlSchemaMapping*>(mappings->GetItem(FdoXml::mGmlProviderName, schema->GetName()));
            FdoPtr<FdoClassCollection> classes = schema->GetClasses();
            FdoPtr<FdoXmlClassMappingCollection> classMappings = mapping->GetClassMappings();
            if (classes == NULL)
                continue;

            for (int j  = classes->GetCount() - 1; j >= 0; j--) {
                FdoPtr<FdoClassDefinition> classDef = classes->GetItem(j);

                // Prepare for implementation of #3
                FdoPtr<FdoClassDefinition> base = classDef->GetBaseClass();
                bool bNoIdentity = false;
                FdoPtr<FdoDataPropertyDefinitionCollection> ids;
                if (base != NULL && wcscmp(base->GetName(), FdoWfsGlobals::AbstractFeature) == 0) {
                    classDef->SetBaseClass(NULL);
                    ids = classDef->GetIdentityProperties();
                    if (ids->GetCount() == 0)
                        bNoIdentity = true;
                }

                // Prepare for implementation of #4
                FdoPtr<FdoXmlClassMapping> classMapping;
                for (int k = classMappings->GetCount() - 1; k >= 0; k--) {
                    classMapping = classMappings->GetItem(k);
                    if (wcscmp(classDef->GetName(), classMapping->GetName()) == 0)
                        break;
                    classMapping = NULL;
                }

                FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();
                FdoPtr<FdoXmlElementMappingCollection> elements;
                if (classMapping != NULL)
                    elements = classMapping->GetElementMappings();
                for (int k = props->GetCount() - 1; k >= 0; k--) {
                    FdoPtr<FdoPropertyDefinition> prop = props->GetItem(k);

                    // Doing #2 
                    FdoStringP name = prop->GetName();
                    if (name.Contains(L"/xlink") ||
                        name.Contains(L"/type") ||
                        name.Contains(L"/gml")) {
                        props->RemoveAt(k);
                        continue;
                    }

                    // Doing #3 
                    if (bNoIdentity && prop->GetPropertyType() == FdoPropertyType_DataProperty) {
                        FdoPtr<FdoDataPropertyDefinition> dataProp = static_cast<FdoDataPropertyDefinition*>(FDO_SAFE_ADDREF(prop.p));
                        if (!dataProp->GetNullable())
                            ids->Add(dataProp);
                        continue; // no need to do the left adjustments
                    }

                    // Doing #4
                    if (elements != NULL && prop->GetPropertyType() == FdoPropertyType_ObjectProperty) {
                    
                        FdoPtr<FdoXmlElementMapping> element;
                        for (int m = elements->GetCount() - 1; m >= 0; m--) {
                            element = elements->GetItem(m);
                            if (wcscmp(prop->GetName(), element->GetName()) == 0) 
                                break;
                            element = NULL;
                        }
                        if (element == NULL)
                            continue;
                        FdoObjectPropertyDefinition* objProp = static_cast<FdoObjectPropertyDefinition*>(prop.p);
                        FdoPtr<FdoClassDefinition> objClassDef = objProp->GetClass();
                        FdoPtr<FdoPropertyDefinitionCollection> objClassProps = objClassDef->GetProperties();
                        bool allGeoProp = true;
                        for (int m = objClassProps->GetCount() - 1; m >= 0; m--) {
                            FdoPtr<FdoPropertyDefinition> objClassProp = objClassProps->GetItem(m);
                            if (objClassProp->GetPropertyType() != FdoPropertyType_GeometricProperty) {
                                allGeoProp = false;
                                break;
                            }
                        }
                        if (allGeoProp) {
                            // first remove the original object property
                            props->RemoveAt(k);
                            // then add another new geometry property with the same name
                            FdoPtr<FdoGeometricPropertyDefinition> newGeoProp = FdoGeometricPropertyDefinition::Create(prop->GetName(), prop->GetDescription());
                            props->Add(newGeoProp);
                            // and we still have to change the element mapping's class mapping
                            FdoPtr<FdoXmlClassMapping> elementClass = FdoXmlClassMapping::Create(FdoGml212::mGeometryProperty,
                                                                                                    FdoGml212::mGeometryProperty,
                                                                                                    FdoGml212::mGeometryProperty);
                            element->SetClassMapping(elementClass);

                        }
                    }
                } // end of for each property

                /// NOTE:
                /// THIS IS TEMPORAL ADDED so that QA can go further
                //if (bNoIdentity && ids->GetCount() == 0) {
                //    FdoPtr<FdoDataPropertyDefinition> fakeId = FdoDataPropertyDefinition::Create(L"FeatId",L"");
                //    fakeId->SetDataType(FdoDataType_String);
                //    props->Add(fakeId);
                //    ids->Add(fakeId);
                //}


            } // end of for each class
        }// end of for each schema
	}
	return FDO_SAFE_ADDREF(mSchemas.p);
}



