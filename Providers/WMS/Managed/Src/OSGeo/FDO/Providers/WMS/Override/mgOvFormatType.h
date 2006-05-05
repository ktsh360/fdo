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
*
*/

#pragma once

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE

/// <summary>
/// The OvFormatType enumeration defines the list of image formats in which a WMS can be returned from the FDO WMS Provider.
/// </summary>
/// <param name="OvFormatType_Png">Portable Network Graphics (PNG).</param>
/// <param name="OvFormatType_Tif">Tagged Image File Format (TIFF).</param>
/// <param name="OvFormatType_Jpg">Joint Photographics Expert Group (JPEG).</param>
/// <param name="OvFormatType_Gif">Graphics Interchange Format (GIF).</param>
public __value enum OvFormatType 
{
	OvFormatType_Png = FdoWmsOvFormatType_Png,
    OvFormatType_Tif = FdoWmsOvFormatType_Tif,
	OvFormatType_Jpg = FdoWmsOvFormatType_Jpg,
    OvFormatType_Gif = FdoWmsOvFormatType_Gif
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS_OVERRIDE
