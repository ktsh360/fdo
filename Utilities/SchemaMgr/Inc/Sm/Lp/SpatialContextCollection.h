#ifndef FDOSMLPSPATIALCONTEXTCOLLECTION_H
#define FDOSMLPSPATIALCONTEXTCOLLECTION_H		1
//
// Copyright (C) 2004-2006  Autodesk, Inc.
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Lp/SpatialContext.h>
#include <Sm/Lp/SpatialContextGeom.h>
#include <Sm/NamedCollection.h>

// Represents a collection of Spatial Contexts in Logical/Physical form.
class FdoSmLpSpatialContextCollection : public FdoSmNamedCollection<FdoSmLpSpatialContext>
{
public:
    /// Create an empty collection.
	FdoSmLpSpatialContextCollection(void);

    /// Create a populated collection.
	//
    /// Parameters:
    /// 	physicalSchema: Physical Schema Manager. All spatial contexts at 
    ///          the current connection are added to this collection.
	FdoSmLpSpatialContextCollection( FdoSmPhMgrP physicalSchema );
	~FdoSmLpSpatialContextCollection(void);

    FdoSmLpSpatialContextP CreateSpatialContext(
        FdoString* name,
        FdoString* description,
        FdoString* coordinateSystem,
        FdoString* coordinateSystemWkt,
        FdoSpatialContextExtentType extentType,
        FdoByteArray * extent,
        double xyTolerance,
        double zTolerance,
        bool bIgnoreStates
	);

    /// Post outstanding modifications to the datastore.
	virtual void Commit();

    /// Gather all errors for this element and child elements into a chain of exceptions.
    /// Adds each error as an exception, to the given exception chain and returns
    /// the chain.
	//
    /// parameters:
    /// 	pFirstException: a chain of exceptions.
	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    void Load();

	FdoSmLpSpatialContextGeomsP  GetSpatialContextGeoms();

protected:
    /// The following two functions create a generic FdoSmLpSpatialContext)
    /// by default. Providers can override to create provider-specific
    /// spatial contexts if necessary.

    /// Construct a spatial context from readers
    virtual FdoSmLpSpatialContextP NewSpatialContext(
        FdoSmPhSpatialContextReaderP scReader,
        FdoSmPhSpatialContextGroupReaderP scgReader,
        FdoSmPhMgrP physicalSchema);

    /// Construct a new spatial context from parameters.
    virtual FdoSmLpSpatialContextP NewSpatialContext(
        FdoString* name,
        FdoString* description,
        FdoString* coordinateSystem,
        FdoString* coordinateSystemWkt,
        FdoSpatialContextExtentType extentType,
        FdoByteArray * extent,
        double xyTolerance,
        double zTolerance,
        bool bIgnoreStates,
        FdoSmPhMgrP physicalSchema);

private:
	FdoInt32 FindExistingSC( FdoSmLpSpatialContextP sc );

private:
    bool        mAreLoaded;
	FdoSmPhMgrP mPhysicalSchema;

	FdoSmLpSpatialContextGeomsP mSpatialContextGeoms;
};

typedef FdoPtr<FdoSmLpSpatialContextCollection> FdoSmLpSpatialContextsP;


#endif


