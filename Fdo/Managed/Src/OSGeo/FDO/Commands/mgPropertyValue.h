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

class FdoPropertyValue;

BEGIN_NAMESPACE_OSGEO_COMMON
public __gc __interface IStreamReader;
END_NAMESPACE_OSGEO_COMMON

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
public __gc class Identifier;
public __gc class ValueExpression;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS

/// \brief
/// The PropertyValue class represents a property name/value pair. Instances of
/// this class are used to specify or hold onto a reference to a property name
/// and the properties value.
public __gc class PropertyValue : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Constructs a default instance of an PropertyValue.
    /// 
	PropertyValue();

    /// \brief
    /// Constructs an instance of an PropertyValue using the specified arguments.
    /// 
    /// \param name 
    /// Input property name as identifier
    /// \param value 
    /// Input the value of property as an expression
    /// 
	PropertyValue(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* name, NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression* value);

    /// \brief
    /// Constructs an instance of an PropertyValue using the specified arguments.
    /// 
    /// \param name 
    /// Input property name as a string
    /// \param value 
    /// Input the value of property as an expression
    /// 
    /// \return
    /// Returns PropertyValue
    /// 
	PropertyValue(System::String* name, NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression* value);

    /// \brief
    /// Gets the property name as an identifier.
    /// 
    /// \return
    /// Returns the PropertyValue Identifier
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* get_Name();

    /// \brief
    /// Sets the property name as an identifier.
    /// 
    /// \param value 
    /// Input the PropertyValue Identifier
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Name(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* value);

    /// \brief
    /// Sets the property name as a string.
    /// 
    /// \param value 
    /// Input property name as a string
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void SetName(System::String* value);

    /// \brief
    /// Gets the value of the property as an ValueExpression.
    /// 
    /// \return
    /// Returns the Property value as a ValueExpression
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression* get_Value();

    /// \brief
    /// Sets the value of the property as an ValueExpression.
    /// 
    /// \param value 
    /// Input the Property value as a ValueExpression
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Value(NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression* value);

    /// \brief
    /// Sets the value of the property as a string. The string content 
    /// will be parsed to produce the required ValueExpression.
    /// 
    /// \param value 
    /// Input the Property value as a String
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void SetValue(System::String* value);

    /// \brief
    /// Provides a Reader on a external data source to allow reading 
    /// in blocks of data.
    /// 
    /// \param stream 
    /// Input reference to a stream reader
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_StreamReader(NAMESPACE_OSGEO_COMMON::IStreamReader* stream); 

    /// \brief
    /// Gets the value of the property as an Stream Reader.
    /// 
    /// \return
    /// Returns the value of the propery as an IStreamReader
    /// 
	__property NAMESPACE_OSGEO_COMMON::IStreamReader* get_StreamReader();

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond

public private:
	PropertyValue(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

	inline FdoPropertyValue* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS


