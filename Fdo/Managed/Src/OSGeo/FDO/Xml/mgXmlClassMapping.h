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

#pragma  once

#include "FDO\Commands\Schema\mgPhysicalClassMapping.h"

class FdoXmlClassMapping;

BEGIN_NAMESPACE_OSGEO_FDO_XML
ref class XmlElementMappingCollection;

/// \ingroup (OSGeoFDOXml)
/// \brief
/// XmlClassMapping describes GML to FDO mappings for a particular FDO class
/// Only mappings that cannot be represented in FDO are described.
public ref class XmlClassMapping : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping
{
public:
    /// \brief
    /// Constructs an object describing the GML-FDO mappings for an FDO class.
    /// 
    /// \param name 
    /// Input class name
    /// \param schemaName 
    /// Input corresponding type name in GML version of the class's schema.
    /// \param wkBaseName 
    /// Input name of the most specialized well-known type. This is 
    /// set when the type represents geometries and indicates what kind of geometries the type
    /// represents. 
    /// 
	XmlClassMapping(System::String^ name, System::String^ schemaName, System::String^ wkBaseName);

    /// \brief
    /// Constructs an object describing the GML-FDO mappings for an FDO class.
    /// 
    /// \param name 
    /// Input class name
    /// \param schemaName 
    /// Input corresponding type name in GML version of the class's schema.
    /// \param wkBaseName 
    /// Input name of the most specialized well-known type. This is 
    /// set when the type represents geometries and indicates what kind of geometries the type
    /// represents. 
    /// \param wkSchemaName 
    /// Input name of the most specialized well-known schema base type. 
    /// 
	XmlClassMapping(System::String^ name, System::String^ schemaName, System::String^ wkBaseName, System::String^ wkSchemaName);
		
    /// \brief
    /// Gets the name of the class's corresponding GML complexType.
    /// 
    /// \return
    /// Returns System::String.
    /// 
    property System::String^ GmlName
    {
        System::String^ get();
    }

    /// \brief
    /// Gets the most specialized well-known base type
    /// 
    /// \return
    /// Returns System::String.
    /// 
    property System::String^ WkBaseName
    {
        System::String^ get();
    }

    /// \brief
    /// Gets the schema containing most specialized well-known base type
    /// 
    /// \return
    /// Returns System::String.
    /// 
    property System::String^ WkSchemaName
    {
        System::String^ get();
    }

    /// \brief
    /// Gets the mappings between GML elements for this class's properties and their
    /// Fdo Classes. 
    /// 
    /// \return
    /// Returns XmlElementMappingCollection^
    /// 
    property NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection^ ElementMappings
    {
        NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection^ get();
    }

internal:
    XmlClassMapping(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping(unmanaged, autoDelete)
	{
	}

	inline FdoXmlClassMapping* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_XML


