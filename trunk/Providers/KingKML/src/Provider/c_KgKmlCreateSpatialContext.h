/*
* Copyright (C) 2010  SL-King d.o.o
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


#ifndef _c_KgKmlCreateSpatialContext_h
#define _c_KgKmlCreateSpatialContext_h

class c_KgKmlCreateSpatialContext : public c_KgKmlFdoCommand<FdoICreateSpatialContext>
{
public:
  c_KgKmlCreateSpatialContext(c_KgKmlConnection *Conn);
public:
  ~c_KgKmlCreateSpatialContext(void);
  
protected:  
    FdoStringP  m_Name;    
    FdoStringP  m_Description;    
    
    FdoStringP  m_CoordinateSystem;
    FdoStringP  m_CoordinateSystemWkt;
    
    FdoSpatialContextExtentType m_SpatialContextExtentType;
    
    FdoPtr<FdoByteArray> m_Extent;
    
    double m_XYTolerance;
    double m_ZTolerance;
    bool m_UpdateExisting;
    
    
public:  
  
    /// \brief
    /// Gets the name of the context to create as a string.
    /// 
    /// \return
    /// Returns the name of the spatial context
    /// 
    FDOKGKML_API virtual FdoString* GetName();

    /// \brief
    /// Sets the name of the context to create as a string.
    /// 
    /// \param value 
    /// Input the name of the spatial context
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGKML_API virtual void SetName(FdoString* value);

    /// \brief
    /// Gets the description of the context to create as a string.
    /// 
    /// \return
    /// Returns the description of the spatial context
    /// 
    FDOKGKML_API virtual FdoString* GetDescription();

    /// \brief
    /// Sets the description of the context to create as a string.
    /// 
    /// \param value 
    /// Input the description of the spatial context
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGKML_API virtual void SetDescription(FdoString* value);

    /// \brief
    /// Gets the coordinate system name as a string.
    /// 
    /// \return
    /// Returns the coordinate system
    /// 
    FDOKGKML_API virtual FdoString* GetCoordinateSystem();

    /// \brief
    /// Sets the coordinate system name as a string.
    /// 
    /// \param value 
    /// Input the coordinate system
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGKML_API virtual void SetCoordinateSystem(FdoString* value);

    /// \brief
    /// Gets the coordinate system definition in OGC SRS WKT format for the 
    /// spatial context currently being created. If the result is an empty string, 
    /// the coordinate system WKT description has not been set.
    /// 
    /// \return
    /// Returns the coordinate system description in WKT of of the spatial context.
    /// 
    FDOKGKML_API virtual FdoString* GetCoordinateSystemWkt();

    /// \brief
    /// Sets the coordinate system definition in OGC SRS WKT 
    /// format for the spatial context currently being created.
    /// 
    /// \param value 
    /// Input the coordinate system WKT
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGKML_API virtual void SetCoordinateSystemWkt(FdoString* value);

    /// \brief
    /// Gets the desired extent type of the context to create, either static or dynamic.
    /// 
    /// \return
    /// Returns the extent type
    /// 
    FDOKGKML_API virtual FdoSpatialContextExtentType GetExtentType();

    /// \brief
    /// Sets the desired extent type of the context to create, either static or dynamic.
    /// 
    /// \param value 
    /// Input the extent type
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGKML_API virtual void SetExtentType(FdoSpatialContextExtentType value);

    /// \brief
    /// Gets the extent of the context to create as a byte array in AGF format.
    /// If the extent type is dynamic, any value specified via this function is ignored.
    /// 
    /// \return
    /// Returns the extent of the spatial context
    /// 
    FDOKGKML_API virtual FdoByteArray* GetExtent();

    /// \brief
    /// Sets the extent of the context to create as a byte array in AGF format.
    /// If the extent type is dynamic this is optional and any value specified is ignored.
    /// 
    /// \param value 
    /// Input the extent of the spatial context
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGKML_API virtual void SetExtent(FdoByteArray* value);

    /// \brief
    /// Gets the tolerance value to use for X/Y ordinates. Tolerances are used
    /// in some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \return
    /// Returns the tolerance
    /// 
    FDOKGKML_API virtual const double GetXYTolerance();

    /// \brief
    /// Sets the tolerance value to use for X/Y ordinates. Tolerances are used
    /// in some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \param value 
    /// Input the tolerance
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGKML_API virtual void SetXYTolerance(const double value);

    /// \brief
    /// Gets the tolerance value to use for Z ordinates. Tolerances are used in
    /// some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \return
    /// Returns the tolerance
    /// 
    FDOKGKML_API virtual const double GetZTolerance();

    /// \brief
    /// Sets the tolerance value to use for Z ordinates. Tolerances are used in
    /// some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \param value 
    /// Input the tolerance
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGKML_API virtual void SetZTolerance(const double value);

    /// \brief
    /// Gets a Boolean flag that indicates how the CreateSpatialContext command
    /// should behave if the spatial context already exists. If update existing
    /// is true and a spatial context with the specified name already exists
    /// then it will be updated. If update existing is false and a spatial
    /// context with the specified name already exists, command execution
    /// will fail.
    /// 
    /// \return
    /// Returns true if command should update existing contexts
    /// 
    FDOKGKML_API virtual const bool GetUpdateExisting();

    /// \brief
    /// Sets a Boolean flag that indicates how the CreateSpatialContext command
    /// should behave if the spatial context already exists. If update existing
    /// is true and a spatial context with the specified name already exists
    /// then it will be updated. If update existing is false and a spatial
    /// context with the specified name already exists, command execution
    /// will fail.
    /// 
    /// \param value 
    /// Input true if existing contexts should be updated
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGKML_API virtual void SetUpdateExisting(const bool value);

    /// \brief
    /// Executes the CreateSpatialContext command. An exception is thrown if
    /// the spatial context already exists, or if the coordinate system or
    /// extent type is not supported by the provider.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGKML_API virtual void Execute();
};

#endif