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

#include "FDO\Schema\mgPropertyDefinition.h"

class FdoRasterPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_RASTER
ref class RasterDataModel;
END_NAMESPACE_OSGEO_FDO_RASTER

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// RasterPropertyDefinition has the information needed to
/// create or completely describe a raster property. This class encapsulates
/// the information necessary to insert a 'new' raster, in the absence of
/// any other information, for the properties defined using this schema
/// element.
public ref class RasterPropertyDefinition : public NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition
{
public:
    /// \brief
    /// Constructs a default instance of an RasterPropertyDefinition.
    /// 
	RasterPropertyDefinition();

    /// \brief
    /// Constructs an instance of an RasterPropertyDefinition using the
    /// specified arguments.
    /// 
    /// \param name 
    /// Input name
    /// \param description 
    /// Input description
    /// 
	RasterPropertyDefinition(System::String^ name, System::String^ description);

    /// \brief
    /// Constructs an instance of an RasterPropertyDefinition using the
    /// specified arguments.
    /// 
    /// \param name 
    /// Input name
    /// \param description 
    /// Input description
    /// \param system 
    /// Input true if this is a system generated property, false otherwise.
    /// <p><b>Note:</b> A client would never set system to true, only a provider.
    /// 
	RasterPropertyDefinition(System::String^ name, System::String^ description, System::Boolean system);

    // 
    // PropertyDefinition interface
    // 

    /// \brief
    /// Returns a Boolean value that indicates if this property is read-only.
    /// 
    /// \return
    /// Returns a Boolean value
    /// 
    /// \brief
    /// Sets a Boolean value that indicates if this property is read-only.
    /// 
    /// \param value 
    /// Input a Boolean value that indicates if this property is read-only
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Boolean ReadOnly
    {
        System::Boolean get();
        System::Void set(System::Boolean value);
    }

    /// \brief
    /// Returns a Boolean value that indicates if this property's value can be
    /// null.
    /// 
    /// \return
    /// Returns a Boolean value
    /// 
    /// \brief
    /// Sets a Boolean value that indicates if this property's value can be
    /// null.
    /// 
    /// \param value 
    /// Input a Boolean value that indicates if this property's value can be
    /// null
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Boolean Nullable
    {
        System::Boolean get();
        System::Void set(System::Boolean value);
    }

    /// \brief
    /// Gets the default data model used by this raster property.
    /// 
    /// \return
    /// Returns the current default data model.
    /// 
    /// \brief
    /// Sets the default data model used by this raster property.
    /// Allowed values are only those data models that are acceptable to the
    /// SupportsDataModel capability.
    /// 
    /// \param dataModel 
    /// The datamodel to be used for newly created
    /// rasters, or the default datamodel to be used when returning raster data.
    /// 
    property NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel^ DefaultDataModel
    {
        NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel^ dataModel);
    }

    /// \brief
    /// Gets the default size of image file in the horizontal
    /// direction in pixels (number of columns).
    /// 
    /// \return
    /// Returns the current default horizontal image size in pixels
    /// (number of columns).
    /// 
    /// \brief
    /// Sets the default size of an image file in the horizontal
    /// direction in pixels (number of columns).
    /// 
    /// \param size 
    /// The desired default horizontal image size in pixels
    /// (number of columns).
    /// 
    property System::Int32 DefaultImageXSize
    {
        System::Int32 get();
        System::Void set(System::Int32 size);
    }

    /// \brief
    /// Gets the default size of an image file in the vertical
    /// direction in pixels (number of rows).
    /// 
    /// \return
    /// Returns the current default vertical image size in pixels
    /// (number of rows).
    /// 
    /// \brief
    /// Sets the default size of an image file in the vertical
    /// direction in pixels (number of rows).
    /// 
    /// \param size 
    /// The desired default vertical image size in pixels
    /// (number of rows).
    /// 
    property System::Int32 DefaultImageYSize
    {
        System::Int32 get();
        System::Void set(System::Int32 size);
    }

    //
    /// Spatial Context Support.
    //

    /// \brief
    /// Sets a Spatial Context association for this raster property.
    /// 
    /// \param spatialContextName 
    /// Input the Spatial Context name to be set. 
    /// Defaults to the active Spatial Context.
    /// 
    /// \return
    /// Returns nothing
    /// 
    /// \brief
    /// Gets the Spatial Context name associated to this raster property.
    /// 
    /// \return
    /// Returns a String value representing the Spatial Context name.
    /// 
    property System::String^ SpatialContextAssociation
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Constructs a RasterPropertyDefinition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	RasterPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(unmanaged, autoDelete)
	{

	}

internal:
	inline FdoRasterPropertyDefinition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


