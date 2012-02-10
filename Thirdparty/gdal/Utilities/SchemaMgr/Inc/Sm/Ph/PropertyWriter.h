#ifndef FDOSMPHPROPERTYWRITER_H
#define FDOSMPHPROPERTYWRITER_H		1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/Row.h>
#include <Sm/Ph/CommandWriter.h>

// This class writes attribute definitions 
// to the f_attributedefinition table.

class FdoSmPhPropertyWriter :
	public FdoSmPhWriter
{
public:

    /// Create an attribute definition writer.
	//
    /// Parameters:
    /// 	mgr: holds the connection for the database to query.
	FdoSmPhPropertyWriter(FdoSmPhMgrP mgr);

	~FdoSmPhPropertyWriter(void);

    /// Various getters for property attributes

    /// Property name
	FdoStringP GetName();

    /// Physical column name for the property
	FdoStringP GetColumnName();

    /// Physical column name for the property
	FdoStringP GetColumnType();

    /// true if property column has fixed name specified through 
    /// schema overrides
    bool GetIsFixedColumn();

    /// true if this class owns the column (column was created when class was created)
    /// false if this class attached itself to pre-existing created when it was created. 
    /// for object properties this setting can apply to multiple columns.
    bool GetIsColumnCreator();

    /// Property Description
	FdoStringP GetDescription();

    /// Owning class id
	FdoInt64  GetClassId();

    /// Data type ( string equivalent of FDO type )
	FdoStringP GetDataType();

    /// Gets the current geometry type.
    FdoStringP GetGeometryType();

    /// If the property is an identity property, this function returns its position
    /// within the class's identity property collection.
    /// Otherwise, it returns 0.
	int GetIdPosition();

    /// Nullable
	bool GetIsNullable();

    /// Read only
	bool GetIsReadOnly();

    /// Is FeatId column ( datastore-only property )
	bool GetIsFeatId();

    /// Is provider-specific System column
	bool GetIsSystem();

    /// Total length for string type.
    /// Total number of digits for number type.
	int GetLength();

    /// Number of digits to right of decimal point
    /// for number type.
	int GetScale();

    /// Table name
	FdoStringP GetDefaultValue();

    /// Table name
	FdoStringP GetTableName();

    /// Gets the root object name for this property.
    //
    /// For an Object Property whose table is foreign, GetTableName() actually
    /// returns the view around the foreign table. This function returns the name
    /// of the foreign table.
    //
    /// For Data or Geometric Properties whose column is in a foreign table, GetColumnName() 
    /// returns the corresponding column in the view around the foreign table. This
    /// function returns the name of the column in the foreign table.
    //
    /// Returns L"" if the table for this property is not foreign.
	FdoStringP GetRootObjectName();

    /// Table name
	FdoStringP GetUser();

    /// Has elevation dimension ( Geometric properties only )
	bool GetHasElevation();

    /// has measure dimension ( Geometric properties only )
	bool GetHasMeasure();

    /// are values autogenerated? ( Data properties only )
	bool GetIsAutoGenerated();

    /// is Revision Number column
    bool GetIsRevisionNumber();

    /// User sequnce name
    FdoStringP GetSequenceName();

    /// Corresponding setter functions

    /// Property name
	void SetName( FdoStringP sValue );

    /// Physical column name for the property
	void SetColumnName( FdoStringP sValue );

    /// Physical column name for the property
	void SetColumnType( FdoStringP sValue );

    void SetIsFixedColumn( bool bValue );

    void SetIsColumnCreator( bool bValue );

    /// Property Description
	void SetDescription( FdoStringP sValue );

    /// Owning class name
	void  SetClassId( FdoInt64 iValue );

    /// Data type ( string equivalent of FDO type )
	void SetDataType( FdoStringP sValue );

    /// Sets the current geometry type.
    void SetGeometryType( FdoStringP sValue );

	void SetIdPosition( int iValue );

    /// Nullable
	void SetIsNullable( bool bValue );

    /// Read only
	void SetIsReadOnly( bool bValue );

    /// Is FeatId column ( datastore-only property )
	void SetIsFeatId( bool bValue );

    /// Is provider-specific System column
	void SetIsSystem( bool bValue );

    /// Total length for string type.
    /// Total number of digits for number type.
	void SetLength( int iValue );

    /// Number of digits to right of decimal point
    /// for number type.
	void SetScale( int iValue );

    /// Table name
	void SetTableName( FdoStringP sValue );

    /// Root table or column name
	void SetRootObjectName( FdoStringP sValue );

	void SetUser( FdoStringP sValue );

    /// Set whether column has elevation dimension ( Geometric properties only )
    void SetHasElevation( bool bValue );

    /// Set whether column has measure dimension ( Geometric properties only )
    void SetHasMeasure( bool bValue );


    /// Set whether column values are autogenerated ( Data properties only )
    void SetIsAutoGenerated( bool bValue );

    /// Set sequence name
    void SetSequenceName( FdoStringP sValue );

    /// Set whether column is Revision Number column
    void SetIsRevisionNumber( bool bValue);

    /// Add the current row as a new attribute definition
	virtual void Add();

    /// Modify a row with the current field values
    //
    /// Parameters
    ///      classid, sName: primary key of the row to modify
    virtual void Modify( FdoInt64 classId, FdoStringP sName );

    /// Delete a row with the current field values
    //
    /// Parameters
    ///      classid, sName: primary key of the row to delete
    virtual void Delete( FdoInt64 classId, FdoStringP sName );

    /// Get a field list for reading from or writing to 
    /// f_attributedefinition.
	//
    /// Parameters:
    /// 	mgr: holds the connection for the current datastore to query.
    static FdoSmPhRowP MakeRow( FdoSmPhMgrP mgr );

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhPropertyWriter() {}

    FdoSmPhWriterP MakeWriter( FdoSmPhMgrP mgr );

};

typedef FdoPtr<FdoSmPhPropertyWriter> FdoSmPhPropertyWriterP;

#endif


