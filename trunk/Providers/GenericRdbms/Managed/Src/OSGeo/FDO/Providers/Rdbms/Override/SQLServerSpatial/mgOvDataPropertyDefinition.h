//
// Copyright (C) 2004-2008  Autodesk, Inc.
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
//

#pragma once

#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgIOvPropertyDefinition.h"

class FdoSqlServerOvDataPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
ref class OvColumn;

///<summary>Concrete class defining physical schema overrides for a data property definition.</summary>
public ref class OvDataPropertyDefinition : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvDataPropertyDefinition, public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::IOvPropertyDefinition
{
public:
    ///<summary>Constructs a default of an FdoSqlServerOvDataPropertyDefinition</summary>
	OvDataPropertyDefinition();

    ///<summary>Constructs an instance of an FdoSqlServerOvDataPropertyDefinition using the specified arguments</summary>
    /// <param name="name">Input name</param>
	OvDataPropertyDefinition(System::String^ name);

    ///<summary>Gets the SqlServer column</summary>
    /// <returns>Returns the SqlServer column</returns>
    ///<summary>Sets the SqlServer column</summary>
    /// <returns>Returns nothing</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvColumn^ Column
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvColumn^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvColumn^ column);
    }

internal:
	OvDataPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoSqlServerOvDataPropertyDefinition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
