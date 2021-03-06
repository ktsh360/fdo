/*
* 
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

#include "stdafx.h"
#include <Fdo\Commands\PropertyValueCollection.h>
#include <Fdo\Commands\LongTransaction\IRollbackLongTransaction.h>

#include "FDO\Commands\LongTransaction\mgIRollbackLongTransactionImp.h"

FdoIRollbackLongTransaction* NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IRollbackLongTransactionImp::GetImpObj()
{
    return static_cast<FdoIRollbackLongTransaction*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IRollbackLongTransactionImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IRollbackLongTransactionImp::Name::get()
{
    FdoString* result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetName())

    return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IRollbackLongTransactionImp::Name::set(System::String^ value)
{
    EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IRollbackLongTransactionImp::KeepLongTransaction::get()
{
    FdoBoolean result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetKeepLongTransaction())

    return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IRollbackLongTransactionImp::KeepLongTransaction::set(System::Boolean value)
{
    EXCEPTION_HANDLER(GetImpObj()->SetKeepLongTransaction(value))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IRollbackLongTransactionImp::Execute()
{
    EXCEPTION_HANDLER(GetImpObj()->Execute())
}
