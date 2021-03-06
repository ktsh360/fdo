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

#include "FDO\Commands\mgICommand.h"

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE

/// \ingroup (OSGeoFDOCommandsUnitOfMeasure)
/// \interface OSGeo::FDO::Commands::UnitOfMeasure::IDestroyMeasureUnit
/// \brief
/// The IDestroyMeasureUnit interface defines the DestroyMeasureUnit command,
/// which deletes a measurement unit. Input to the command is simply the
/// abbreviation of the measure unit to be destroyed.
public interface class IDestroyMeasureUnit : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Gets the abbreviation of the measure unit to destroy as a string
    /// 
    /// \return
    /// Returns the abbreviation of the measure unit to be deleted.
    /// 
    /// \brief
    /// Sets the abbreviation of the measure unit to destroy as a string
    /// 
    /// \param value 
    /// Input the abbreviation of the measure unit to be deleted.
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::String^ Abbreviation
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Executes the DestroyMeasureUnit command.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE


