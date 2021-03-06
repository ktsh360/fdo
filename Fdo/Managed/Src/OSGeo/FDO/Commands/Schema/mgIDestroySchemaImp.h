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

#pragma once

#include "FDO\Commands\mgICommandImp.h"
#include "FDO\Commands\Schema\mgIDestroySchema.h"

class FdoIDestroySchema;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

/// \ingroup (OSGeoFDOCommandsSchema)
/// \brief
/// The IDestroySchemaImp class is a concrete implementation of IDestroySchema.
/// The IDestroySchema interface defines the DestroySchema command, which
/// destroys a schema definition, including all class definitions, relationship
/// definitions, and instance data within it. If elements in other schemas refer
/// to the schema to be destroyed command, execution will fail. Input to the
/// delete schema command is the name of the schema to be destroyed.
private ref class IDestroySchemaImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, 
                                       public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IDestroySchema
{
public:
    /// \brief
    /// Gets the name of the schema to destroy as a string.
    /// 
    /// \return
    /// Returns the name of the schema to destroy
    /// 
    /// \brief
    /// Sets the name of the schema to destroy as a string.
    /// 
    /// \param value 
    /// Input the name of the schema to destroy
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property System::String^ SchemaName
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Executes the destroy schema command, which removes the schema, class
    /// definitions, relation definitions, and all instance data from the DataStore.
    /// If elements in other schemas refer to the schema to be destroyed
    /// an exception is thrown.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void Execute();

internal:
	IDestroySchemaImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIDestroySchema* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA


