//
// Copyright (C) 2006 Refractions Research, Inc.
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
//
#include "stdafx.h"
#include "ObjectPropertyClass.h"

FdoSmLpPostGisObjectPropertyClass::FdoSmLpPostGisObjectPropertyClass(
    FdoSmLpObjectPropertyDefinition* parent,
	FdoSmLpClassDefinition* parentType,
	FdoSmLpPropertyMappingDefinition* mapping,
	FdoSmLpPropertyMappingType mappingType,
    FdoPhysicalClassMapping* overrides)
    : FdoSmLpGrdObjectPropertyClass(parent, parentType, mapping,
        mappingType, overrides),

      FdoSmLpPostGisClassDefinition(MakeName(parent),
        L"Autogenerated Object Property Class",
        mapping, NULL, FdoSmOvTableMappingType_ConcreteTable,
        parent->GetContainingDbObjectName(),
        parent->GetContainingDbObject(),
        parent->GetElementState()),

      FdoSmLpClassBase(MakeName(parent),
        L"Autogenerated Object Property Class",
        mapping, NULL, FdoSmOvTableMappingType_ConcreteTable,
        parent->GetContainingDbObjectName(),
        parent->GetContainingDbObject(),
        parent->GetElementState(),
        parent->GetIsFromFdo())
{
    // idle
}

FdoSmLpPostGisObjectPropertyClass::FdoSmLpPostGisObjectPropertyClass(
	FdoSmLpObjectPropertyClassP base,
	FdoSmLpObjectPropertyDefinition* parent,
	FdoSmLpClassDefinition* parentType,
	FdoSmLpPropertyMappingDefinition* mapping,
	FdoSmLpPropertyMappingType mappingType,
    FdoPhysicalClassMapping* overrides)
    : FdoSmLpGrdObjectPropertyClass(base, parent, parentType, mapping,
        mappingType, overrides),

	  FdoSmLpPostGisClassDefinition(MakeName(parent),
        L"Autogenerated Object Property Class", mapping,
		base.p->SmartCast<FdoSmLpClassDefinition>(),
		FdoSmOvTableMappingType_ConcreteTable,
		parent->GetContainingDbObjectName(),
		parent->GetContainingDbObject(),
		parent->GetElementState()),

	  FdoSmLpClassBase(MakeName(parent),
        L"Autogenerated Object Property Class", mapping,
		base.p->SmartCast<FdoSmLpClassDefinition>(),
        FdoSmOvTableMappingType_ConcreteTable,
		parent->GetContainingDbObjectName(),
		parent->GetContainingDbObject(),
		parent->GetElementState(),
        parent->GetIsFromFdo())
{
    // idle
}

FdoSmLpPostGisObjectPropertyClass::~FdoSmLpPostGisObjectPropertyClass()
{
    // idle
}

void FdoSmLpPostGisObjectPropertyClass::Update(FdoClassDefinition* fdoClass,
    FdoSchemaElementState state,
    FdoPhysicalSchemaMapping* overrides,
    bool ignoreStates)
{
    FdoSmLpGrdClassDefinition::Update(fdoClass, state, overrides, ignoreStates);
    FdoSmLpGrdObjectPropertyClass::Update(fdoClass, state, overrides, ignoreStates);
}

void FdoSmLpPostGisObjectPropertyClass::PostFinalize()
{
    FdoSmLpGrdObjectPropertyClass::PostFinalize();
    FdoSmLpPostGisClassDefinition::PostFinalize();
}
