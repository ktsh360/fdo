/*
* Copyright (C) 2006  SL-King d.o.o
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
*/

#pragma once
//  new 3.2 naming
#ifdef _FDO_3_1
  #define D_FDO_3_1
#endif

#ifdef D_FDO_3_1

#define FdoPtr GisPtr
#define FDO_SAFE_RELEASE GIS_SAFE_RELEASE
#define FDO_SAFE_ADDREF GIS_SAFE_ADDREF

#define FdoIReader GisIReader
#define FdoByte GisByte
#define FdoByteArray GisByteArray
#define FdoString GisString
#define FdoStringP GisStringP
#define FdoStringCollection GisStringCollection
#define FdoInt16 GisInt16
#define FdoInt32 GisInt32
#define FdoInt64 GisInt64
#define FdoBoolean GisBoolean
#define FdoIoStream GisIoStream
#define FdoDateTime GisDateTime
#define FdoIStreamReader GisIStreamReader

#define FdoIPolygon GisIPolygon
#define FdoIGeometry GisIGeometry

#define FdoFgfGeometryFactory GisAgfGeometryFactory
#define CreateGeometryFromFgf CreateGeometryFromAgf
#define FdoIEnvelope GisIEnvelope
#define FdoEnvelopeImpl GisEnvelopeImpl
#define GetFgf GetAgf
#define FdoILinearRing GisILinearRing
#define FdoGeometryType_Point GisGeometryType_Point
#define FdoGeometryType_LineString GisGeometryType_LineString
#define FdoGeometryType_Polygon GisGeometryType_Polygon
#define FdoGeometryType_MultiPoint GisGeometryType_MultiPoint
#define FdoGeometryType_MultiLineString GisGeometryType_MultiLineString
#define FdoGeometryType_MultiPolygon GisGeometryType_MultiPolygon
#define FdoGeometryType_MultiGeometry GisGeometryType_MultiGeometry
#define FdoGeometryType_CurveString GisGeometryType_CurveString
#define FdoGeometryType_CurvePolygon GisGeometryType_CurvePolygon
#define FdoGeometryType_MultiCurveString GisGeometryType_MultiCurveString
#define FdoGeometryType_MultiCurvePolygon GisGeometryType_MultiCurvePolygon
#define FdoGeometryComponentType_LinearRing GisGeometryComponentType_LinearRing
#define FdoGeometryComponentType_LineStringSegment GisGeometryComponentType_LineStringSegment
#define FdoGeometryComponentType_CircularArcSegment GisGeometryComponentType_CircularArcSegment
#define FdoGeometryComponentType_Ring GisGeometryComponentType_Ring
#define FdoGeometryType GisGeometryType
#define FdoGeometryComponentType GisGeometryComponentType
#define FdoDimensionality_XY GisDimensionality_XY
#define FdoDimensionality_Z GisDimensionality_Z
#define FdoDimensionality_M GisDimensionality_M

#define FdoNamedCollection GisNamedCollection

#define FdoXmlSaxContext GisXmlSaxContext
#define FdoXmlSaxHandler GisXmlSaxHandler
#define FdoXmlAttributeCollection GisXmlAttributeCollection
#define FdoXmlWriter GisXmlWriter
#define FdoDisposable GisDisposable
#define FdoXmlAttributeP GisXmlAttributeP
#define FdoXml GisXml
#define FDO_NLSID GIS_NLSID
//#define FdoException GisException

#define FDO_100_COMMAND_TIMEOUT_NOT_SUPPORTED GIS_100_COMMAND_TIMEOUT_NOT_SUPPORTED
#define FDO_103_CONNECTION_ALREADY_OPEN GIS_103_CONNECTION_ALREADY_OPEN
#define FDO_102_COMMAND_NOT_SUPPORTED GIS_102_COMMAND_NOT_SUPPORTED
#define FDO_68_COMMAND_PARAMETERS_NOT_SUPPORTED GIS_68_COMMAND_PARAMETERS_NOT_SUPPORTED

#endif
