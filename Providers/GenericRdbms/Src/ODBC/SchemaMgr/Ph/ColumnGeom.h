#ifndef FDOSMPHODBCCOLUMNGEOM_H
#define FDOSMPHODBCCOLUMNGEOM_H      1
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

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnGeom.h>

// Represents a Odbc geometric type column.
class FdoSmPhOdbcColumnGeom :
    public FdoSmPhColumnGeom, public FdoSmPhOdbcColumn
{
public:
    FdoSmPhOdbcColumnGeom(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        FdoSmPhDbObject* parentObject,
        FdoSmPhScInfoP AssociatedSCInfo,
        bool bNullable = true,
        bool bHasElevation = true,
        bool bHasMeasure = false,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* reader = NULL
    ) :
        FdoSmPhOdbcColumn (reader),
        FdoSmPhColumn    ( columnName, L"IMAGE", elementState, parentObject, bNullable, rootColumnName),
        FdoSmPhColumnGeom( AssociatedSCInfo, bHasElevation, bHasMeasure )
    {}

    virtual ~FdoSmPhOdbcColumnGeom(void) {}

    virtual int GetRdbType()
    {
        return RDBI_GEOMETRY;
    }

};

typedef FdoPtr<FdoSmPhOdbcColumnGeom> FdoSmPhOdbcColumnGeomP;

#endif
