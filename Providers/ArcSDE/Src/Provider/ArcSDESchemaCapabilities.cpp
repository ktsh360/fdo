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

#include "stdafx.h"

ArcSDESchemaCapabilities::ArcSDESchemaCapabilities()
{
}

ArcSDESchemaCapabilities::~ArcSDESchemaCapabilities()
{
}

void ArcSDESchemaCapabilities::Dispose()
{
    delete this;
}

/// <summary>Returns an array of the FdoClassType objects the feature provider supports.</summary>
/// <param name="length">Output the number of class types.</param> 
/// <returns>Returns the list of class types.</returns> 
FdoClassType* ArcSDESchemaCapabilities::GetClassTypes(FdoInt32& length)
{
    static FdoClassType classes[] =
    {
        FdoClassType_Class,
        FdoClassType_FeatureClass
    };

    length = sizeof (classes) / sizeof (FdoClassType);

    return (classes);
}

/// <summary>Returns an array of the FdoDataType objects the feature provider supports.</summary>
/// <param name="length">Output the number of data types.</param> 
/// <returns>Returns the list of data types</returns> 
FdoDataType* ArcSDESchemaCapabilities::GetDataTypes(FdoInt32& length)
{
    static FdoDataType types[] =
    {
        FdoDataType_DateTime,
        FdoDataType_Double,
        FdoDataType_Int16,
        FdoDataType_Int32,
        FdoDataType_Single,
        FdoDataType_String,
        FdoDataType_BLOB,

// NO SUPPORT IN R1, SINCE APPLYSCHEMA WON'T BE IN R1:
//        FdoDataType_Boolean,
//        FdoDataType_Decimal,
//        FdoDataType_Byte,
//        FdoDataType_Int64,

// NO PLAN TO SUPPORT:
//        FdoDataType_CLOB,
    };

    length = sizeof (types) / sizeof (FdoDataType);

    return (types);
}


/// <summary>Determines if the feature provider supports inheritance.</summary>
/// <returns>Returns true if the feature provider supports inheritance</returns> 
bool ArcSDESchemaCapabilities::SupportsInheritance()
{
    return (false);
}


/// <summary>Determines if the provider supports multiple schemas. Returns false if the provider supports only a single schema.</summary>
/// <returns>Returns true if the provider supports multiple schemas.</returns> 
bool ArcSDESchemaCapabilities::SupportsMultipleSchemas()
{
    return (true);
}


/// <summary>Determines if the provider supports object properties. Returns false if the provider supports only data and geometric properties.</summary>
/// <returns>Returns true if the provider supports object properties.</returns> 
bool ArcSDESchemaCapabilities::SupportsObjectProperties()
{
    return (false);
}


/// <summary>Determines if the provider supports Schema Mapping Overrides 
/// (overrides to the default rules for mapping Feature Schemas to provider-specific
/// physical schemas)</summary>
/// <returns>Returns true if the provider supports Schema Overrides</returns> 
bool ArcSDESchemaCapabilities::SupportsSchemaOverrides()
{
    return false;  // APPLYSHEMA NOT IN R1, SO OVERRIDES NOT IN R1
}


/// <summary>Determines if the provider supports association property.</summary>
/// <returns>Returns true if the provider support the association property.</returns> 
bool ArcSDESchemaCapabilities::SupportsAssociationProperties(void)
{
    return false;
}

/// <summary>Determines if the provider supports the network model.</summary>
/// <returns>Returns true if the provider supports the network model.</returns> 
bool ArcSDESchemaCapabilities::SupportsNetworkModel(void)
{
    return false;
}

/// <summary>Determines if the provider can auto-generate unique ID values for a class.</summary>
/// <returns>Returns true if the provider supports id autogeneration.</returns> 
bool ArcSDESchemaCapabilities::SupportsAutoIdGeneration()
{
    return true;
}

/// <summary>Determines if the provider can auto-generate unique ID values that are unique for the entire datastore, rather than just for a particular class.</summary>
/// <returns>Returns true if the provider supports id autogeneration unique across an datastore.</returns> 
bool ArcSDESchemaCapabilities::SupportsDataStoreScopeUniqueIdGeneration()
{
    return false;
}

/// <summary>Returns an array of the FdoDataType objects the feature provider supports for auto-generation.</summary>
/// <param name="length">Output the number of data types.</param> 
/// <returns>Returns the list of data types</returns> 
FdoDataType* ArcSDESchemaCapabilities::GetSupportedAutoGeneratedTypes(FdoInt32& length)
{
    static FdoDataType autogen_types[] =
    {
        FdoDataType_Int32,
    };

    length = sizeof (autogen_types) / sizeof (FdoDataType);

    return (autogen_types);
}

bool ArcSDESchemaCapabilities::SupportsSchemaModification()
{
    return false;
}

/// <summary>Determines if the provider supports inclusive value range constraints.</summary>
/// <returns>Returns true if the provider supports inclusive value range constraints.</returns> 
bool ArcSDESchemaCapabilities::SupportsInclusiveValueRangeConstraints()
{
    return false;
}

/// <summary>Determines if the provider supports exclusive value range constraints.</summary>
/// <returns>Returns true if the provider supports exclusive value range constraints.</returns>
bool ArcSDESchemaCapabilities::SupportsExclusiveValueRangeConstraints()
{
    return false;
}
/// <summary>Determines if the provider supports value constraints list.</summary>
/// <returns>Returns true if the provider supports value constraints list.</returns>
bool ArcSDESchemaCapabilities::SupportsValueConstraintsList()
{
    return false;
}

/// <summary>Determines if the provider supports nullable value constraint.</summary>
/// <returns>Returns true if the provider supports nullable value constraint.</returns>
bool ArcSDESchemaCapabilities::SupportsNullValueConstraints()
{
    return true;
}

/// <summary>Determines if the provider supports unique value constraint.</summary>
/// <returns>Returns true if the provider supports unique value constraint.</returns>
bool ArcSDESchemaCapabilities::SupportsUniqueValueConstraints()
{
    return true;
}

/// <summary>Determines if the provider supports composite unique value constraint.</summary>
/// <returns>Returns true if the provider supports composite unique value constraint.</returns>
bool ArcSDESchemaCapabilities::SupportsCompositeUniqueValueConstraints()
{
    return true;
}

/// <summary>Determines if the provider supports multiple identity properties per class.</summary>
/// <returns>Returns true if the provider supports multiple identity properties per class.</returns>
bool ArcSDESchemaCapabilities::SupportsCompositeId()
{
    return false;
}

FdoInt32 ArcSDESchemaCapabilities::GetNameSizeLimit (FdoSchemaElementNameType nameType)
{
    switch (nameType)
    {
        case FdoSchemaElementNameType_Datastore:   return 123;  // sql server's limit, which is higher than oracle's limit of 30
        case FdoSchemaElementNameType_Schema:      return SE_MAX_DATABASE_LEN + SE_MAX_OWNER_LEN + 1;
        case FdoSchemaElementNameType_Class:       return SE_MAX_TABLE_LEN;
        case FdoSchemaElementNameType_Property:    return SE_MAX_COLUMN_LEN;
        case FdoSchemaElementNameType_Description: return SE_MAX_DESCRIPTION_LEN;
    }
    return -1;
}

FdoString* ArcSDESchemaCapabilities::GetReservedCharactersForName()
{
    return L".:";
}

FdoInt64 ArcSDESchemaCapabilities::GetMaximumDataValueLength (FdoDataType dataType)
{
    switch (dataType)
    {
        case FdoDataType_String:   return (FdoInt64)2^32; // ArcSDE's limit itself
        case FdoDataType_BLOB:     return (FdoInt64)2^32; // Oracle's limit, which is higher than SQL Server's limit
        case FdoDataType_CLOB:     return (FdoInt64)2^32; // Oracle's limit, which is higher than SQL Server's limit
        case FdoDataType_Decimal:  return (FdoInt64)(GetMaximumDecimalScale() + GetMaximumDecimalPrecision());
        case FdoDataType_Boolean:  return (FdoInt64)sizeof(FdoBoolean);
        case FdoDataType_Byte:     return (FdoInt64)sizeof(FdoByte);
        case FdoDataType_DateTime: return (FdoInt64)sizeof(FdoDateTime);
        case FdoDataType_Double:   return (FdoInt64)sizeof(FdoDouble);
        case FdoDataType_Int16:    return (FdoInt64)sizeof(FdoInt16);
        case FdoDataType_Int32:    return (FdoInt64)sizeof(FdoInt32);
        case FdoDataType_Int64:    return (FdoInt64)sizeof(FdoInt64);
        case FdoDataType_Single:   return (FdoInt64)sizeof(FdoFloat);
    }
    return (FdoInt64)-1;
}

FdoInt32 ArcSDESchemaCapabilities::GetMaximumDecimalPrecision()
{
    return 38; // oracle's limit, since it's higher than sql server's limit
}

FdoInt32 ArcSDESchemaCapabilities::GetMaximumDecimalScale()
{
    return 127; // oracle's limit, since it's higher than sql server's limit
}

FdoDataType* ArcSDESchemaCapabilities::GetSupportedIdentityPropertyTypes(FdoInt32& length)
{
    static FdoDataType types[] =
    {
        FdoDataType_DateTime,
        FdoDataType_Double,
        FdoDataType_Int16,
        FdoDataType_Int32,
        FdoDataType_Single,
        FdoDataType_String,
        FdoDataType_BLOB
    };

    length = sizeof (types) / sizeof (FdoDataType);

    return types;
}

bool ArcSDESchemaCapabilities::SupportsDefaultValue()
{
    return false;  // this could be true if we supported ApplySchema
}
