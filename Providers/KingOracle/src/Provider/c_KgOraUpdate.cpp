/*
* Copyright (C) 2009  SL-King d.o.o
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
#include "stdafx.h"
#include "c_KgOraFilterProcessor.h"

c_KgOraUpdate::c_KgOraUpdate(c_KgOraConnection *Conn)
  : c_KgOraFdoFeatureCommand<FdoIUpdate>(Conn)
{
}

c_KgOraUpdate::~c_KgOraUpdate(void)
{
}



/// \brief
/// Gets the FdoPropertyValueCollection that specifies the names and values
/// of the properties for the instance to be inserted.
/// 
/// \return
/// Returns the list of properties and their values.
/// 
FdoPropertyValueCollection* c_KgOraUpdate::GetPropertyValues()
{
  if( !m_PropertyValues.p )
  {
    m_PropertyValues = FdoPropertyValueCollection::Create();
  }

  return FDO_SAFE_ADDREF( m_PropertyValues.p );
}


/// \brief
/// Executes the insert command and returns a reference to an
/// FdoIFeatureReader. Some feature providers can generate automatic identity
/// values for features. This will happen automatically as the features are
/// inserted. The returned FdoIFeatureReader allows the client to obtain the
/// automatic identity property value(s) of newly inserted object(s). The
/// returned feature reader at a minimum will read the unique identity
/// properties of the objects just inserted. Multiple objects will be
/// returned through the reader in the case of a batch insert.
/// 
/// \return
/// Returns an FdoIFeatureReader
/// 
FdoInt32 c_KgOraUpdate::Execute()
{
  FdoPtr<c_KgOraSchemaDesc> schemadesc = m_Connection->GetSchemaDesc();
  
  FdoPtr<FdoIdentifier> classid = GetFeatureClassName();
  FdoPtr<FdoClassDefinition> classdef = schemadesc->FindClassDefinition(classid);
  if( !classdef.p ) return NULL;
  
  
  FdoPtr<FdoKgOraClassDefinition> phys_class = schemadesc->FindClassMapping(classid);
  FdoStringP fultablename = phys_class->GetOracleFullTableName();
  FdoStringP table_alias = phys_class->GetOraTableAlias();
  
  c_KgOraSridDesc orasrid;
  m_Connection->GetOracleSridDesc(classdef,orasrid);
  
  if( m_PropertyValues.p )
  {
    c_FilterStringBuffer strbuff;
    c_KgOraExpressionProcessor expproc(&strbuff,schemadesc,classdef,orasrid);
    
    
    FdoStringP colupdates;
    FdoStringP colvalues;
    FdoStringP sep;
    
    unsigned int count = m_PropertyValues->GetCount();
    for(unsigned int ind=0;ind<count;ind++)
    {
      FdoPtr<FdoPropertyValue> propval = m_PropertyValues->GetItem(ind);
      FdoPtr<FdoIdentifier> propid = propval->GetName();
      FdoPtr<FdoValueExpression> expr = propval->GetValue();
                       
      strbuff.ClearBuffer();
      
      c_KgOraSridDesc orasrid;
      if( m_Connection->GetOracleSridDesc(classdef,propid->GetName(),orasrid) )
      {
        expproc.SetOracleSrid(orasrid);
      }
      
      expr->Process( &expproc );
      
      colupdates += sep + propid->GetName() + L" = " + strbuff.GetString();      
      
      sep = L",";
    }
    
    c_FilterStringBuffer sqlstr;
    sqlstr.AppendString( L"UPDATE ");
    sqlstr.AppendString(fultablename);
    sqlstr.AppendString( L" ");
    sqlstr.AppendString(table_alias);
    sqlstr.AppendString( L" SET ");
    sqlstr.AppendString(colupdates);
    sqlstr.AppendString( L" ");
    
    // process filter
    const wchar_t* filtertext=NULL;
    c_KgOraFilterProcessor fproc(m_Connection,schemadesc,classdef,orasrid);
    fproc.GetExpressionProcessor().SetParamNumberOffset(expproc.GetSqlParametersCount());
    if( m_Filter )
    {      
      m_Filter->Process( &fproc );
    
      filtertext = fproc.GetFilterText();
    }
    else filtertext = NULL;
    
    if( filtertext && *filtertext )
    {
      sqlstr.AppendString( L" WHERE ");
      sqlstr.AppendString(filtertext);
    }
    
    int update_num=0;
    c_Oci_Statement* oci_stm=NULL;
    
    try
    {
      oci_stm = m_Connection->OCI_CreateStatement();
      
      oci_stm->Prepare(sqlstr.GetString());
      
      #ifdef _KGORA_EXTENDED_LOG
        FdoStringP s1 = sqlstr.GetString();
        D_KGORA_ELOG_WRITE1("Execute Update: '%s'",(const char*)s1);
      #endif
      
      // fist apply binds from update values
      expproc.ApplySqlParameters(oci_stm);
      // then apply sql binds from filter expresion
      fproc.GetExpressionProcessor().ApplySqlParameters(oci_stm,expproc.GetSqlParametersCount());
      
      
      update_num = oci_stm->ExecuteNonQuery();
      
      
      
      if( oci_stm ) m_Connection->OCI_TerminateStatement(oci_stm);
      
      
    }
    catch(c_Oci_Exception* ea)
    {
      
      if( oci_stm ) m_Connection->OCI_TerminateStatement(oci_stm);
      FdoStringP gstr = ea->what();
      
      D_KGORA_ELOG_WRITE2("c_KgOraUpdate::Execute%d Exception '%s'",m_Connection->m_ConnNo,(const char*)gstr);
      
      delete ea;
      throw FdoCommandException::Create( gstr );    
    }

    
    return update_num;
    
  }
  
  
  return 0;
}//end of c_KgOraUpdate::Execute

FdoILockConflictReader* c_KgOraUpdate::GetLockConflicts()
{
  return NULL;
}
