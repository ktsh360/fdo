/*
* Copyright (C) 2007  Haris Kurtagic
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
#include "c_KgInfSelectCommand.h"
#include "c_KgInfFeatureReader.h"
#include "c_KgInfFilterProcessor.h"

#include "c_LogApi.h"

#include <malloc.h>

c_KgInfSelectCommand::c_KgInfSelectCommand (c_KgInfConnection* Conn) 
    : c_KgInfFdoFeatureCommand<FdoISelect> (Conn)
{
  m_OrderingOption = FdoOrderingOption_Ascending;
}



c_KgInfSelectCommand::~c_KgInfSelectCommand (void)
{
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing.</returns> 
void c_KgInfSelectCommand::Dispose ()
{
    delete this;
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of property names to 
/// return with the result. If empty all properties of the specified class
/// are returned.</summary>
/// <returns>Returns the list of property names.</returns> 
FdoIdentifierCollection* c_KgInfSelectCommand::GetPropertyNames ()
{
    if (m_PropertyNames == NULL)
        m_PropertyNames = FdoIdentifierCollection::Create();

    return (FDO_SAFE_ADDREF(m_PropertyNames.p));
}

/// <summary>Gets the FdoLockType value (see "Locking Commands").</summary>
/// <returns>Returns the lock type.</returns> 
FdoLockType c_KgInfSelectCommand::GetLockType ()
{
    throw FdoCommandException::Create (L"Locking not supported 'GetLocktTpe'.");
}

/// <summary>Sets the FdoLockType value (see "Locking Commands").</summary>
/// <param name="value">Input the lock type.</param> 
/// <returns>Returns nothing</returns> 
void c_KgInfSelectCommand::SetLockType (FdoLockType value)
{
    throw FdoCommandException::Create (L"Locking not supported 'SetLocktTpe'.");
}

/// <summary>Gets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <returns>Returns the lock strategy.</returns> 
FdoLockStrategy c_KgInfSelectCommand::GetLockStrategy ()
{
    throw FdoCommandException::Create (L"Locking not supported 'GetLockStrategy'.");
}

/// <summary>Sets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <param name="value">Input the lock strategy.</param> 
/// <returns>Returns nothing</returns> 
void c_KgInfSelectCommand::SetLockStrategy (FdoLockStrategy value)
{
    throw FdoCommandException::Create (L"Locking not supported 'SetLockStrategy'.");
}

/// <summary>Executes the select command and returns a reference to an FdoIFeatureReader.</summary>
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* c_KgInfSelectCommand::Execute ()
{
 
    FdoPtr<FdoIdentifier> classid = GetFeatureClassName ();
    FdoString* class_name = classid->GetText ();
    
    FdoPtr<c_KgInfSchemaDesc> schemadesc = m_Connection->GetSchemaDesc();
    
    D_KGORA_ELOG_WRITE1("c_KgInfSelectCommand::Execute class_name = '%s'",(const char*)FdoStringP(class_name));
    
    FdoPtr<FdoFeatureSchemaCollection> fschemas = schemadesc->GetFeatureSchema();
    FdoPtr<c_KgInfOverPhysicalSchemaMapping> phschemamapping = schemadesc->GetPhysicalSchemaMapping();
    
    FdoPtr<FdoClassDefinition> classdef = schemadesc->FindClassDefinition(classid);
    if( !classdef.p ) return NULL;
    
    int geom_sqlcol_index;
    FdoPtr<FdoStringCollection> sqlcols = FdoStringCollection::Create();
    
    c_KgInfSridDesc orasrid;
    {
      FdoPtr<FdoPropertyDefinition> propdef;        
      FdoPtr<FdoPropertyDefinitionCollection> propcol = classdef->GetProperties();
      int count = propcol->GetCount();
      for( int ind = 0; ind < count; ind++ )
      {
        propdef = propcol->GetItem(ind);
        FdoString* propname = propdef->GetName();
              
        if( propdef->GetPropertyType() == FdoPropertyType_GeometricProperty )
        {
          FdoGeometricPropertyDefinition* geomprop = (FdoGeometricPropertyDefinition*)propdef.p;
          
          m_Connection->GetOracleSridDesc(geomprop,orasrid);
          
          
        }
      }
    }
    
    c_KgInfFilterProcessor fproc(schemadesc,classid,orasrid);
    string sqlstr = CreateSqlString(fproc,geom_sqlcol_index,sqlcols);
    
    otl_stream* stream=NULL;
    c_KgOtlStreamReader* streamreader=NULL;
    
    try
    {
    /*
      occi_stm = m_Connection->OCCI_CreateStatement();
      D_KGORA_ELOG_WRITE1("Execute select: '%s",sqlstr.c_str());
      occi_stm->setSQL(sqlstr);
      occi_stm->setPrefetchRowCount(200);
    */  
      
      D_KGORA_ELOG_WRITE1("Execute select: '%s",sqlstr.c_str());
      
      stream = m_Connection->OTL_CreateStream(20,sqlstr.c_str());
      
      fproc.GetExpressionProcessor().ApplySqlParameters(stream);
      
      streamreader = new c_KgOtlStreamReader(stream);
      //occi_rset = occi_stm->executeQuery();
      //m_Connection->OCCI_TerminateStatement(occi_stm);
    }
    catch(c_KgOtlException& ea)
    {
      m_Connection->OTL_DeleteStream(stream);
      throw FdoCommandException::Create( ea.what() );    
    }

    
    return new c_KgInfFeatureReader(m_Connection,stream,streamreader,classdef,geom_sqlcol_index,sqlcols, m_PropertyNames);
    
}//end of c_KgInfSelectCommand::Execute 

/// <summary>Executes the select command and returns a reference to an
/// IFeatureReader.</summary> 
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* c_KgInfSelectCommand::ExecuteWithLock ()
{
  throw FdoCommandException::Create (L"Locking not supported 'ExecuteWithLock'.");
}

/// <summary> When executing the operation ExecuteWithLock lock 
/// conflicts may occur. Those lock conflicts are reported. The 
/// function GetLockConflicts returns a lock conflict reader 
/// providing access to the list of lock conflicts that occurred 
/// during the execution of the operation.</summary>
/// <returns>Returns a lock conflict reader.</returns> 
FdoILockConflictReader* c_KgInfSelectCommand::GetLockConflicts ()
{
  throw FdoCommandException::Create (L"Locking not supported 'GetLockConflicts'.");
    
}


string c_KgInfSelectCommand::CreateSqlString(c_KgInfFilterProcessor& FilterProc,int& GeomSqlColumnIndex,FdoStringCollection* SqlColumns)
{
    FdoPtr<FdoIdentifier> classid = GetFeatureClassName ();
    FdoString* class_name = classid->GetText ();
    
    FdoPtr<c_KgInfSchemaDesc> schemadesc = m_Connection->GetSchemaDesc();
    
    FdoPtr<FdoFeatureSchemaCollection> fschemas = schemadesc->GetFeatureSchema();
    //FdoPtr<c_KgInfOverPhysicalSchemaMapping> phschemamapping = schemadesc->GetPhysicalSchemaMapping();
    
    FdoPtr<FdoClassDefinition> classdef;
    if( fschemas && (fschemas->GetCount() > 0) )
    {
      //FdoPtr<FdoFeatureSchema> schm = fschemas->GetItem(0);      
      //FdoPtr<FdoClassCollection> classes = schm->GetClasses();
      //classdef = classes->FindItem( classid->GetName() );
      
      FdoPtr<FdoIDisposableCollection> classes = fschemas->FindClass(classid->GetText());
      classdef = (FdoClassDefinition*)classes->GetItem( 0 );
      
      
      if( !classdef.p ) return "";
          
    }

    string sqlstr;
    
    FdoPtr<c_KgInfOverClassDefinition> phys_class = schemadesc->FindClassMapping(classid);
    FdoStringP fultablename = phys_class->GetSqlQueryFullName();
    FdoStringP table_alias = phys_class->GetOraTableAlias();
    
    /* Define properties to be included in SELECT statement */    
    FdoPtr<FdoPropertyDefinition> propdef;
    FdoStringP sql_select_columns_part;
    FdoStringP sep;
    
    GeomSqlColumnIndex=-1;
    FdoPtr<FdoPropertyDefinitionCollection> propcol = classdef->GetProperties();

    c_KgInfSridDesc orasrid;
    int count = propcol->GetCount();
    for( int ind = 0; ind < count; ind++ )
    {
      propdef = propcol->GetItem(ind);
      FdoString* propname = propdef->GetName();
      
      
      SqlColumns->Add(propname);
      if( propdef->GetPropertyType() == FdoPropertyType_GeometricProperty )
      {
        FdoGeometricPropertyDefinition* geomprop = (FdoGeometricPropertyDefinition*)propdef.p;
        
        m_Connection->GetOracleSridDesc(geomprop,orasrid);
        
        GeomSqlColumnIndex=ind;
        
        if( phys_class->GetIsPointGeometry() && (_wcsicmp(propname,phys_class->GetPoinGeometryPropertyName())==0) )
        {
        // this is geometry created as point from numeric columns
          
          FdoStringP pointstr;
          if( phys_class->GetPointZOraColumn() && (wcslen(phys_class->GetPointZOraColumn()) > 0) )
            pointstr = pointstr.Format(L" SDO_GEOMETRY(2001,NULL,SDO_POINT_TYPE(%s,%s,%s),NULL,NULL) as %s ",phys_class->GetPointXOraColumn(),phys_class->GetPointYOraColumn(),phys_class->GetPointZOraColumn(),propname);
          else
            pointstr = pointstr.Format(L" SDO_GEOMETRY(2001,NULL,SDO_POINT_TYPE(%s,%s,NULL),NULL,NULL) as %s ",phys_class->GetPointXOraColumn(),phys_class->GetPointYOraColumn(),propname);
                    
          sql_select_columns_part += sep + pointstr;  // this is for just column -> sql_select_columns_part += sep + table_alias + "." + propname;  
          sep = ",";
        }
        else
        {
        // this is normal geomerty property - oracle column
        // add just property name in select
          sql_select_columns_part += sep + "cast(ST_AsBinary("+table_alias + "." + propname + ") as lvarchar):#1<raw>";  sep = ",";       

        }
      }
      else
      {
      // add property name in select
        sql_select_columns_part += sep + table_alias + "." + propname;  sep = ",";
      }

    }
     
    //string wherestr;
    //CreateFilterSqlString(m_Filter,wherestr);
    
    
    const char* filtertext=NULL;
    
    if( m_Filter )
    {      
      m_Filter->Process( &FilterProc );
    
      filtertext = FilterProc.GetFilterText();
    }
    else filtertext = NULL;

    
    // So tu use it I need to add a geom filter to from clause
    // and to add inner join on table primary key
    
    FdoStringP pkeyname;
    
    FdoPtr<FdoDataPropertyDefinitionCollection> identcol = classdef->GetIdentityProperties();
    pkeyname = "oid";
    
    char* sbuff = new char[2048];
    
    const char* geomfilter = FilterProc.GetGeomFilterText();
    if( geomfilter && *geomfilter && (identcol->GetCount() > 0) ) // also identity column (primary key must exist to be able to use spatial filters
    {
    // SELECT t.* FROM ST.FilterQuery('GEOTABLE', 'GEOCOLUMN', ST.MakeEnvelope(1000,1000,2000,2000,-1)) AS q
    // INNER JOIN GEOTABLE AS t ON q.oid = t.oid
    // or SELECT * FROM ST.FilterQuery#dbo#GEOTABLE#GEOCOLUMN(ST.MakeEnvelope(1000,1000,2000,2000,-1))
      
      sprintf(sbuff, "SELECT %s FROM %s AS sf1 INNER JOIN %s AS %s ON ",(const char*)sql_select_columns_part
                                                          ,geomfilter
                                                          ,(const char*)fultablename,(const char*)table_alias
                                                          );
                                                          
     // Now add primary key ON      
      char buff2[512];    
      for(int indid = 0;indid<identcol->GetCount();indid++)
      {
        
        FdoPtr<FdoDataPropertyDefinition> propdef = identcol->GetItem(indid);
        pkeyname = propdef->GetName();
        sprintf(buff2," sf1.%s = %s.%s ",(const char*)pkeyname,(const char*)table_alias,(const char*)pkeyname);
        
        if( indid > 0 )
        {
          strcat(sbuff," AND ");
        }
        strcat(sbuff,buff2);
      }
                                                                
    }
    else
    {
    // no spatial condition generate sql standard way
      sprintf(sbuff, "SELECT %s FROM %s %s",(const char*)sql_select_columns_part,(const char*)fultablename,(const char*)table_alias);
    }
    
    
    sqlstr = sbuff;
    if( filtertext && *filtertext )
    {
      sqlstr += " WHERE ";
      //sqlstr += wherestr;
      sqlstr += filtertext;
    }
    
    delete[] sbuff ;
    
    FdoPtr<FdoIdentifierCollection> order_ident_col = GetOrdering();
    long order_count = order_ident_col->GetCount();
    if( order_count > 0 )
    {
      string sep;
      sqlstr += " ORDER BY ";
      for(long ind=0; ind<order_count; ind++)
      {
        FdoPtr<FdoIdentifier> order_ident = order_ident_col->GetItem(ind);
        FdoStringP fdostr = order_ident->GetName();
        sqlstr += sep + (const char*)fdostr;
        if( GetOrderingOption() == FdoOrderingOption_Ascending )
        {
          sqlstr += " ASC ";
        }
        else
        {
          sqlstr += " DESC ";
        }
        sep = ",";
      }
    }
    
    // Test vremena za fetch iz oracle
    #ifdef _DEBUG
      //TestArrayFetch(ClassId, Filter, Props);
    #endif
    
    
    return sqlstr;
}//end of c_KgInfSelectCommand::CreateSqlString



// just check for spatial filter
// other filters ignored
void c_KgInfSelectCommand::CreateFilterSqlString(FdoFilter* Filter,string& WhereBuff)
{
  
    if( !Filter ) return;
  
    FdoSpatialCondition* spatial_filter = NULL;
    
    
    FdoString* testsql = Filter->ToString();        
    
    spatial_filter = dynamic_cast<FdoSpatialCondition*>(Filter);
    if( !spatial_filter ) return;
    
        
    if (spatial_filter->GetOperation() == FdoSpatialOperations_EnvelopeIntersects)
    {
        FdoPtr<FdoExpression> expr = spatial_filter->GetGeometry();
        FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(expr.p);

        if (geomval)
        {
            
            FdoPtr<FdoIdentifier> geomprop = spatial_filter->GetPropertyName();
            FdoStringP gcolname = geomprop->GetName();
            
            
            FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);
            FdoPtr<FdoIEnvelope> envelope = fgfgeom->GetEnvelope();

            char* sbuff = new char[512];
            char* sbuff2= new char[256];
            
            sprintf(sbuff2,"MDSYS.SDO_GEOMETRY(2003, NULL, NULL, SDO_ELEM_INFO_ARRAY(1,1003,3),SDO_ORDINATE_ARRAY(%.6lf,%.6lf, %.6lf,%.6lf))"
                          ,envelope->GetMinX(),envelope->GetMinY(),envelope->GetMaxX(),envelope->GetMaxY());
            
            // TODO: for envelope interscet just use primary filter SDO_FILTER and compare perfomance
            //sprintf(sbuff,"SDO_FILTER(a.%s,%s)='TRUE'",(const char*)gcolname,sbuff2);
            sprintf(sbuff,"SDO_ANYINTERACT(a.%s,%s)='TRUE'",(const char*)gcolname,sbuff2);
            
            WhereBuff.assign(sbuff);
            
            delete [] sbuff;
            delete [] sbuff2;

        }
    }
    else if (spatial_filter->GetOperation() == FdoSpatialOperations_Intersects)
    {
        
        FdoPtr<FdoExpression> expr = spatial_filter->GetGeometry();
        FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(expr.p);

        if (geomval)
        {
            FdoPtr<FdoIdentifier> geomprop = spatial_filter->GetPropertyName();
            FdoStringP gcolname = geomprop->GetName();
            
            FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);
            FdoPtr<FdoIEnvelope> envelope = fgfgeom->GetEnvelope();
            
            char* sbuff = new char[512];
            char* sbuff2= new char[256];
            
            sprintf(sbuff2,"SDO_GEOMETRY(2003, NULL, NULL, SDO_ELEM_INFO_ARRAY(1,1003,3),SDO_ORDINATE_ARRAY(%.6lf,%.6lf, %.6lf,%.6lf))"
                          ,envelope->GetMinX(),envelope->GetMinY(),envelope->GetMaxX(),envelope->GetMaxY());
            
            sprintf(sbuff,"SDO_ANYINTERACT(a.%s,%s)='TRUE'",(const char*)gcolname,sbuff2);
            
            WhereBuff.assign(sbuff);
            
            delete[] sbuff;
            delete[] sbuff2;
        }
    }
    


}//end of c_KgInfSelectCommand::CreateFilterSqlString


/// <summary>Gets the FdoIdentifierCollection that holds the list of order by property names. If empty no ordering is used. This list is initially
/// empty and the caller need to add the property that the command should use as a order by criteria.</summary>
/// <returns>Returns the list of group by property names.</returns> 
FdoIdentifierCollection* c_KgInfSelectCommand::GetOrdering()
{ 
  if( m_OrderingIdentifiers.p == NULL )
    m_OrderingIdentifiers = FdoIdentifierCollection::Create();
    
  return FDO_SAFE_ADDREF(m_OrderingIdentifiers.p); 
}

/// <summary>Set the ordering option of the selection. This is only used if the ordering collection is not empty.</summary>
/// <param name="option">Is the ordering option and should be set to one of FdoOrderingOption_Ascending or FdoOrderingOption_Descending.
/// FdoOrderingOption_Ascending is the default value.</param> 
/// <returns>Returns nothing</returns> 
void c_KgInfSelectCommand::SetOrderingOption( FdoOrderingOption  Option ) 
{
  m_OrderingOption = Option;
}

/// <summary>Gets the ordering option.</summary>
/// <returns>Returns the ordering option.</returns> 
FdoOrderingOption c_KgInfSelectCommand::GetOrderingOption( )
{ 
  return m_OrderingOption; 
}