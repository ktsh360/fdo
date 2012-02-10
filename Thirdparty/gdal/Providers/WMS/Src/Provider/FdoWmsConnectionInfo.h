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

#ifndef FDOWMSCONNECTIONINFO_H
#define FDOWMSCONNECTIONINFO_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoWmsConnection;

class FdoWmsConnectionInfo :  public FdoIConnectionInfo
{
    friend class FdoWmsConnection;

public:
    // <summary>Default Constructor.</summary>
    // <returns>Returns nothing</returns> 
    FdoWmsConnectionInfo ()  { FdoWmsConnectionInfo(NULL); }  // 0-argument constructor to please FdoPtr::operator->
    
    // <summary>Constructor used to initialize the FdoConnection.</summary>
    // <returns>Returns nothing</returns> 
    FdoWmsConnectionInfo (FdoWmsConnection* connection);

protected:
    // <summary>Virtual Destructor.</summary>
    // <returns>Returns nothing</returns> 
    virtual ~FdoWmsConnectionInfo (void);

    // <summary>Dispose this object.</summary>
    // <returns>Returns nothing</returns> 
    virtual void Dispose ();

    // <summary>Throws an exception if the connection is invalid.</summary>
    // <returns>Returns nothing</returns> 
    virtual void validate ();

public:
    /// <summary>Gets the name of the feature provider.</summary>
    /// <returns>Returns the provider name</returns>
    FdoString* GetProviderName ();

    /// <summary>Gets the display name of the feature provider.</summary>
    /// <returns>Returns the provider's display name</returns>
    FdoString* GetProviderDisplayName();

    /// <summary>Gets the description of the feature provider.</summary>
    /// <returns>Returns the provider description</returns>
    FdoString* GetProviderDescription ();

    /// <summary>Gets the version of the feature provider.</summary>
    /// <returns>Returns provider version</returns>
    FdoString* GetProviderVersion ();

    /// <summary>Gets the version of the Feature Data Objects specification to which this provider conforms.</summary>
    /// <returns>Returns FDO version supported.</returns>
    FdoString* GetFeatureDataObjectsVersion ();

    /// <summary>Gets the FdoIConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.</summary>
    /// <returns>Returns the property dictionary</returns>
    FdoIConnectionPropertyDictionary* GetConnectionProperties ();

    /// <summary>Returns the provider type. A provider can be a file-based, database-based or
    /// web-based provider. The valid values the function may return are defined in
    /// the enumeration FdoProviderDatastoreType. The enumeration includes the following
    /// values: FdoProviderDatastoreType_Unknown, FdoProviderDatastoreType_File,
    /// FdoProviderDatastoreType_DatabaseServer, FdoProviderDatastoreType_WebServer.</summary>
    /// <returns>Returns the provider data store type.</returns>
    FdoProviderDatastoreType GetProviderDatastoreType();

    /// <summary>File-based providers depend on a various files. This function returns a list
    /// of fully qualified dependend file names. The return parameter will be NULL if
    /// the provider is not a file-based provider.</summary>
    /// <returns>Returns the list of fully-qualified dependend file names if the provider is a
    /// file-based provider, NULL otherwise.</returns>
    FdoStringCollection* GetDependentFileNames();

private:
    /**
     * The connection that spawned us.
     */
    FdoWmsConnection* mConnection; // weak reference

    /**
     * The dictionary of connection parameters.
     */
    FdoPtr<FdoCommonConnPropDictionary> mPropertyDictionary;
};

#endif // FDOWMSCONNECTIONINFO_H
