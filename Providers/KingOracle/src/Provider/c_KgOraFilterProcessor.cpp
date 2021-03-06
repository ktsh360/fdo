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
#include "c_FgfToSdoGeom.h"
#include "c_Ora_API2.h"

// 1SPATIAL START
#include "math.h"
// 1SPATIAL END


#define D_FILTER_OPEN_PARENTH L" ( "
#define D_FILTER_CLOSE_PARENTH L" ) "

#define D_FILTER_COMMA L" , "
#define D_FILTER_LOGICAL_AND L" AND "
#define D_FILTER_LOGICAL_OR L" OR "
#define D_FILTER_LOGICAL_NOT L" NOT "
#define D_FILTER_IN L" IN "
#define D_FILTER_IS_NULL L" IS NULL "

// For comparison operation
#define D_FILTER_EQUAL_OP               L" = "
#define D_FILTER_NOT_EQUAL_OP           L" <> "
#define D_FILTER_GREATER_THAN_OP        L" > "
#define D_FILTER_GREATER_OR_EQUAL_OP    L" >= "
#define D_FILTER_LESS_THAN_OP           L" < "
#define D_FILTER_LESS_OR_EQUAL_OP       L" <= "
#define D_FILTER_LIKE_OP                L" LIKE "





c_KgOraFilterProcessor::c_KgOraFilterProcessor(c_KgOraConnection* KgOraConn,c_KgOraSchemaDesc *KgOraSchemaDesc,FdoClassDefinition* ClassDef,const c_KgOraSridDesc& OraSrid)
  : m_ExpressionProcessor( &m_StringBuff,KgOraSchemaDesc,ClassDef,OraSrid ) // they will share same string buffer
{
  m_KgOraConn=KgOraConn;
  
  m_KgOraSchemaDesc = KgOraSchemaDesc;
  FDO_SAFE_ADDREF(m_KgOraSchemaDesc.p);
  
  
  m_ClassDef = FDO_SAFE_ADDREF(ClassDef);
  
  FdoPtr<FdoKgOraPhysicalSchemaMapping> phschemamapping;
  FdoPtr<FdoKgOraClassDefinition> phys_class;
  if( KgOraSchemaDesc && ClassDef )
  {
    phschemamapping = KgOraSchemaDesc->GetPhysicalSchemaMapping();
    m_phys_class = phschemamapping->FindByClassName( ClassDef->GetName() );

  }
  
}

c_KgOraFilterProcessor::~c_KgOraFilterProcessor(void)
{
  
}

void c_KgOraFilterProcessor::ProcessFilter(FdoFilter* Filter)
{
  Filter->Process(this);
}//end of c_KgOraFilterProcessor::ProcessFilter

void c_KgOraFilterProcessor::ProcessGeomExpresion( FdoExpression* Expr,c_KgOraSridDesc& OraSrid)
{  
  m_ExpressionProcessor.SetOracleSrid(OraSrid);
  Expr->Process( &m_ExpressionProcessor );   
}

void c_KgOraFilterProcessor::ProcessExpresion( FdoExpression* Expr )
{  
  //m_ExpressionProcessor.SetOracleSrid(OraSrid);
  Expr->Process( &m_ExpressionProcessor );   
}


//
// Add a string to the end of the buffer
void c_KgOraFilterProcessor::AppendString(const wchar_t *Str)
{
  m_StringBuff.AppendString(Str);
}

//
// Add a string to the biginning of the buffer
void c_KgOraFilterProcessor::PrependString(const wchar_t *Str)
{
  m_StringBuff.PrependString(Str);
}




/// \brief
/// Processes the FdoBinaryLogicalOperator passed in as an argument.
/// 
/// \param Filter 
/// Input the FdoBinaryLogicalOperator
/// 
/// \return
/// Returns nothing
/// 
void c_KgOraFilterProcessor::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& Filter)
{
  FdoPtr<FdoFilter>leftop = Filter.GetLeftOperand();
  FdoPtr<FdoFilter>rightop = Filter.GetRightOperand();
  AppendString(D_FILTER_OPEN_PARENTH);
  if( Filter.GetOperation() == FdoBinaryLogicalOperations_And )
  {
    
    ProcessFilter( leftop );  
    AppendString( D_FILTER_LOGICAL_AND );
    ProcessFilter( rightop );
  }
  else
  {   
    ProcessFilter( leftop );
    AppendString( D_FILTER_LOGICAL_OR );
    ProcessFilter( rightop );
  }

  AppendString(D_FILTER_CLOSE_PARENTH);
  
}//end of c_KgOraFilterProcessor::ProcessBinaryLogicalOperator

void c_KgOraFilterProcessor::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& Filter)
{
    FdoPtr<FdoFilter>unaryop = Filter.GetOperand();
    if( unaryop == NULL ) throw FdoFilterException::Create(L"FdoUnaryLogicalOperator is missing the operand");

    AppendString(D_FILTER_OPEN_PARENTH);
    if ( Filter.GetOperation() == FdoUnaryLogicalOperations_Not )
    {
        AppendString( D_FILTER_LOGICAL_NOT );
    }
    else
        throw FdoFilterException::Create(L"FdoUnaryLogicalOperator supports only the 'Not' operation");

    ProcessFilter( unaryop );
    AppendString(D_FILTER_CLOSE_PARENTH);
}//end of c_KgOraFilterProcessor::ProcessUnaryLogicalOperator

void c_KgOraFilterProcessor::ProcessComparisonCondition(FdoComparisonCondition& Filter)
{
    FdoPtr<FdoExpression> leftexp = Filter.GetLeftExpression();
    FdoPtr<FdoExpression> rightexp = Filter.GetRightExpression();
    if( leftexp == NULL  ) throw FdoFilterException::Create(L"FdoComparisonCondition is missing the left expression");
    if( rightexp == NULL ) throw FdoFilterException::Create(L"FdoComparisonCondition is missing the right expression");

    AppendString( D_FILTER_OPEN_PARENTH );
    ProcessExpresion( leftexp );

    switch ( Filter.GetOperation() )
    {
        case FdoComparisonOperations_EqualTo: 
          AppendString( D_FILTER_EQUAL_OP );
        break;

        case FdoComparisonOperations_NotEqualTo: 
          AppendString( D_FILTER_NOT_EQUAL_OP );
        break;

        case FdoComparisonOperations_GreaterThan: 
          AppendString( D_FILTER_GREATER_THAN_OP );
        break;

        case FdoComparisonOperations_GreaterThanOrEqualTo: 
          AppendString( D_FILTER_GREATER_OR_EQUAL_OP );
        break;

        case FdoComparisonOperations_LessThan: 
          AppendString( D_FILTER_LESS_THAN_OP );
        break;

        case FdoComparisonOperations_LessThanOrEqualTo: 
          AppendString( D_FILTER_LESS_OR_EQUAL_OP );
        break;

        case FdoComparisonOperations_Like: 
          AppendString( D_FILTER_LIKE_OP );
        break;
        default:
          throw FdoFilterException::Create(L"FdoComparisonCondition unkown comparison operation");
        break;
    }
    ProcessExpresion( rightexp );
    AppendString( D_FILTER_CLOSE_PARENTH );
}//end of c_KgOraFilterProcessor::ProcessComparisonCondition


void c_KgOraFilterProcessor::ProcessInCondition(FdoInCondition& Filter)
{
    int i;
    FdoPtr<FdoExpression> exp;
    FdoPtr<FdoIdentifier> id = Filter.GetPropertyName();
    if( id == NULL )
        throw FdoFilterException::Create(L"FdoInCondition is missing the property name");

    FdoPtr<FdoValueExpressionCollection>expressions = Filter.GetValues();
    if( expressions == NULL || expressions->GetCount() <= 0 )
        throw FdoFilterException::Create(L"FdoInCondition has an empty value list");

    AppendString(D_FILTER_OPEN_PARENTH);
    ProcessExpresion( id );
    AppendString( D_FILTER_IN );
    AppendString(D_FILTER_OPEN_PARENTH);
    for(i=0; i < expressions->GetCount()-1; i++ )
    {
        exp = expressions->GetItem(i);
        ProcessExpresion( exp );
        AppendString( D_FILTER_COMMA );
    }
    exp = expressions->GetItem( i );
    ProcessExpresion(exp);
    AppendString(D_FILTER_CLOSE_PARENTH);
    AppendString(D_FILTER_CLOSE_PARENTH);
}//end of c_KgOraFilterProcessor::ProcessInCondition

void c_KgOraFilterProcessor::ProcessNullCondition(FdoNullCondition& Filter)
{
    FdoPtr<FdoIdentifier>id = Filter.GetPropertyName();
    if( id == NULL )
        throw FdoFilterException::Create(L"FdoNullCondition is missing the property name");

    AppendString(D_FILTER_OPEN_PARENTH);
    ProcessExpresion( id );
    AppendString( D_FILTER_IS_NULL );
    AppendString(D_FILTER_CLOSE_PARENTH);
    
}//end of c_KgOraFilterProcessor::ProcessNullCondition


void c_KgOraFilterProcessor::ProcessSpatialCondition(FdoSpatialCondition& Filter)
{

FdoPtr<FdoIdentifier> geomprop = Filter.GetPropertyName();

FdoPtr<FdoExpression> geomexp = Filter.GetGeometry();

c_KgOraSridDesc orasrid;
m_KgOraConn->GetOracleSridDesc(m_ClassDef,geomprop->GetName(),orasrid);

// If class is from SDE then need to apply only primary check of min max
if( m_phys_class.p && m_phys_class->GetIsSdeClass() )
{
  FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(geomexp.p);
  if (geomval)
  {
  
    // SELECT /*+ LEADING INDEX(S_ S2008_IX1) INDEX(SHAPE F2008_UK1) INDEX(MMY_AREA
    // A2008_IX1) */  OID,  TOID,  FEATCODE,  VERSION,  VERDATE,  THEME,  CALCAREA,
    // CHANGE,  DESCGROUP,  DESCTERM,  MAKE,  PHYSLEVEL,  PHYSPRES,  BROKEN, 
    // LOADDATE,  SHAPE  ,S_.eminx,S_.eminy,S_.emaxx,S_.emaxy ,SHAPE.fid,
    // SHAPE.numofpts,SHAPE.entity,SHAPE.points,SHAPE.rowid 
    // FROM
    //  (SELECT  /*+ INDEX(SP_ S2008_IX1) */ DISTINCT sp_fid, eminx, eminy, emaxx,
    //  emaxy FROM OSMASTERMAP.S2008 SP_  WHERE SP_.gx >= :1 AND SP_.gx <= :2 AND
    //  SP_.gy >= :3 AND SP_.gy <= :4 AND SP_.eminx <= :5 AND SP_.eminy <= :6 AND
    //  SP_.emaxx >= :7 AND SP_.emaxy >= :8) S_ ,  
    //  OSMASTERMAP.MMY_AREA ,
    // OSMASTERMAP.F2008 SHAPE  WHERE S_.sp_fid = SHAPE.fid AND S_.sp_fid =
    // OSMASTERMAP.MMY_AREA.SHAPE
    
    
    
    // create select statement for spatial index
    
    
    FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);
    FdoPtr<FdoIEnvelope> envelope = fgfgeom->GetEnvelope();
    double minx = envelope->GetMinX();
    double miny = envelope->GetMinY();

    double maxx = envelope->GetMaxX();
    double maxy = envelope->GetMaxY();
    
    // convert min max to sde integers
    minx = (minx - orasrid.m_SDE_FalseX) * orasrid.m_SDE_XYUnit;
    maxx = (maxx - orasrid.m_SDE_FalseX) * orasrid.m_SDE_XYUnit;
    
    miny = (miny - orasrid.m_SDE_FalseY) * orasrid.m_SDE_XYUnit;
    maxy = (maxy - orasrid.m_SDE_FalseY) * orasrid.m_SDE_XYUnit;

	// 1SPATIAL START
	// for index
	double gxmin = floor( minx / (orasrid.m_SDE_XYUnit * m_phys_class->GetSdeGSize1()));
	double gxmax = floor( maxx / (orasrid.m_SDE_XYUnit * m_phys_class->GetSdeGSize1()));
	double gymin = floor( miny / (orasrid.m_SDE_XYUnit * m_phys_class->GetSdeGSize1()));
	double gymax = floor( maxy / (orasrid.m_SDE_XYUnit * m_phys_class->GetSdeGSize1()));
	// 1SPATIAL END

    wstring indexname = m_phys_class->GetSdeIndexTableName();
    indexname += L"_IX1";
    
    
    FdoPtr<FdoDoubleValue> fval_gxmin = FdoDoubleValue::Create(gxmin);
    FdoStringP param_gxmin = m_ExpressionProcessor.PushParameter(*fval_gxmin);
    
    FdoPtr<FdoDoubleValue> fval_gxmax = FdoDoubleValue::Create(gxmax);
    FdoStringP param_gxmax = m_ExpressionProcessor.PushParameter(*fval_gxmax);
    
    FdoPtr<FdoDoubleValue> fval_gymin = FdoDoubleValue::Create(gymin);
    FdoStringP param_gymin = m_ExpressionProcessor.PushParameter(*fval_gymin);
    
    FdoPtr<FdoDoubleValue> fval_gymax = FdoDoubleValue::Create(gymax);
    FdoStringP param_gymax = m_ExpressionProcessor.PushParameter(*fval_gymax);
    
    
      	// 1SPATIAL START
      	FdoStringP szORFilter;
    	if (m_phys_class->GetSdeGSize2() > 0)
      	{
        		double gxmin2 = 16777216 + floor( minx / (orasrid.m_SDE_XYUnit * m_phys_class->GetSdeGSize2()));
        		double gxmax2 = 16777216 + floor( maxx / (orasrid.m_SDE_XYUnit * m_phys_class->GetSdeGSize2()));
        		double gymin2 = 16777216 + floor( miny / (orasrid.m_SDE_XYUnit * m_phys_class->GetSdeGSize2()));
        		double gymax2 = 16777216 + floor( maxy / (orasrid.m_SDE_XYUnit * m_phys_class->GetSdeGSize2()));
        
          		FdoPtr<FdoDoubleValue> fval_gxmin2 = FdoDoubleValue::Create(gxmin2);
        		FdoStringP param_gxmin2 = m_ExpressionProcessor.PushParameter(*fval_gxmin2);
        
          		FdoPtr<FdoDoubleValue> fval_gxmax2 = FdoDoubleValue::Create(gxmax2);
        		FdoStringP param_gxmax2 = m_ExpressionProcessor.PushParameter(*fval_gxmax2);
        
          		FdoPtr<FdoDoubleValue> fval_gymin2 = FdoDoubleValue::Create(gymin2);
        		FdoStringP param_gymin2 = m_ExpressionProcessor.PushParameter(*fval_gymin2);
        
          		FdoPtr<FdoDoubleValue> fval_gymax2 = FdoDoubleValue::Create(gymax2);
        		FdoStringP param_gymax2 = m_ExpressionProcessor.PushParameter(*fval_gymax2);
        
          		szORFilter = FdoStringP::Format(L" OR  (SP_.gx >= %s  AND  SP_.gx <= %s  AND  SP_.gy >= %s AND SP_.gy <= %s  /* GSize2=%.0lf */) "
          										,(const wchar_t*)param_gxmin2,(const wchar_t*)param_gxmax2,(const wchar_t*)param_gymin2,(const wchar_t*)param_gymax2
          										, m_phys_class->GetSdeGSize2() );
        
          //		szORFilter = FdoStringP::Format(L" OR  (SP_.gx >= %.0lf  AND  SP_.gx <= %.0lf  AND  SP_.gy >= %.0lf AND SP_.gy <= %.0lf "
          //										L" /* minx2=%.0lf  m_OraSridDesc.m_SDE_XYUnit=%.0lf  RES=%.0lf  GSize2=%.0lf */ "
          //										L" ) "
          //										,gxmin2,gxmax2,gymin2,gymax2
          //										,minx2,m_OraSridDesc.m_SDE_XYUnit, floor( minx2 / (m_OraSridDesc.m_SDE_XYUnit * m_ClassDef->GetSdeGSize2()))
          //										,m_ClassDef->GetSdeGSize2() );
          	}
    	else
      		szORFilter = FdoStringP::Format(L" ");
    	// 1SPATIAL END
      
      
      
    FdoPtr<FdoDoubleValue> fval_maxx = FdoDoubleValue::Create(maxx);
    FdoStringP param_maxx = m_ExpressionProcessor.PushParameter(*fval_maxx);
    
    FdoPtr<FdoDoubleValue> fval_maxy = FdoDoubleValue::Create(maxy);
    FdoStringP param_maxy = m_ExpressionProcessor.PushParameter(*fval_maxy);
    
    FdoPtr<FdoDoubleValue> fval_minx = FdoDoubleValue::Create(minx);
    FdoStringP param_minx = m_ExpressionProcessor.PushParameter(*fval_minx);
    
    FdoPtr<FdoDoubleValue> fval_miny = FdoDoubleValue::Create(miny);
    FdoStringP param_miny = m_ExpressionProcessor.PushParameter(*fval_miny);
    
    
    // this goes into FROM part of SQL
    FdoStringP sbuff = FdoStringP::Format(L"(SELECT  /*+ INDEX(SP_ %s) */ DISTINCT sp_fid, eminx, eminy, emaxx,"
        L" emaxy FROM %s SP_  WHERE "
        // 1SPATIAL START
               // L" SP_.gx >= 0 AND SP_.gy >= 0"
        		L" ("
        		L"  (SP_.gx >= %s AND SP_.gx <= %s"
        		L"  AND SP_.gy >= %s AND SP_.gy <= %s"
        		L"  /* XYUnit=%.0lf  GSize1=%.0lf  GSize2=%.0lf */) "
        		L" %s "
        		L" )"        
        // 1SPATIAL END
        L" AND SP_.eminx <= %s AND SP_.eminy <= %s AND"
        L" SP_.emaxx >= %s AND SP_.emaxy >= %s) S_",
        		indexname.c_str(),(const wchar_t*)m_phys_class->GetSdeIndexTableName()
        		// 1SPATIAL START
                // ,maxx,maxy,minx,miny
        		// ,minx,maxx,miny,maxy,maxx,maxy,minx,miny
        		,(const wchar_t*)param_gxmin,(const wchar_t*)param_gxmax,(const wchar_t*)param_gymin,(const wchar_t*)param_gymax
        		,orasrid.m_SDE_XYUnit,m_phys_class->GetSdeGSize1()
        		,m_phys_class->GetSdeGSize2()
        		,(const wchar_t*)szORFilter
        		,(const wchar_t*)param_maxx,(const wchar_t*)param_maxy,(const wchar_t*)param_minx,(const wchar_t*)param_miny
        		// 1SPATIAL END
       );
       
    m_SDE_SelectSpatialIndex = sbuff;   


    // this goes into WHERE part of SQL
    sbuff = FdoStringP::Format(L"S_.sp_fid = %s.fid",m_phys_class->GetSdeGeomTableAlias());
    m_SDE_WhereSpatialIndex = sbuff;
    
    AppendString(L"1=1"); // just to satisfy boolean operator. Spatial condition can be combined with other filters and 
                          // filter processor will generate some boolean operators
    
    /*
    FdoStringP buff;

    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString(L" NOT ");
    AppendString(D_FILTER_OPEN_PARENTH);

    AppendString(L"eminx");
    AppendString(L">");       
    buff = FdoStringP::Format(L"%.8lf",maxx);
    AppendString((FdoString*)buff);

    AppendString(L" or ");

    AppendString(L"emaxx");
    AppendString(L"<");        
    buff = FdoStringP::Format(L"%.8lf",minx);
    AppendString((FdoString*)buff);

    AppendString(L" or ");

    AppendString(L"eminy");
    AppendString(L">");        
    buff = FdoStringP::Format(L"%.8lf",maxy);
    AppendString((FdoString*)buff);

    AppendString(L" or ");

    AppendString(L"emaxy");
    AppendString(L"<");        
    buff = FdoStringP::Format(L"%.8lf",miny);
    AppendString((FdoString*)buff);

    AppendString(D_FILTER_CLOSE_PARENTH);        
    AppendString(D_FILTER_CLOSE_PARENTH);       
    */ 
  }    
  return;
}



FdoPtr<FdoPropertyDefinitionCollection> props = m_ClassDef->GetProperties();
FdoPtr<FdoPropertyDefinition>  propdef = props->FindItem(geomprop->GetName());
if( propdef.p && propdef->GetPropertyType()==FdoPropertyType_GeometricProperty )
{
  FdoGeometricPropertyDefinition* geompropdef = (FdoGeometricPropertyDefinition*)propdef.p;
}

switch( Filter.GetOperation() )
{
  case FdoSpatialOperations_EnvelopeIntersects:
  {
    if( m_phys_class.p &&  m_phys_class->GetIsPointGeometry() )
    {
      FdoStringP str_xcol =  m_phys_class->GetPointXOraColumn();
      FdoStringP str_ycol =  m_phys_class->GetPointYOraColumn();
      
      FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(geomexp.p);
      if (geomval)
      {
        FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);
        FdoPtr<FdoIEnvelope> envelope = fgfgeom->GetEnvelope();

        double minx = envelope->GetMinX();
        double miny = envelope->GetMinY();
        
        double maxx = envelope->GetMaxX();
        double maxy = envelope->GetMaxY();

        FdoStringP buff;
        
        AppendString(D_FILTER_OPEN_PARENTH);
        
        AppendString(str_xcol);
        AppendString(L">=");       
        buff = FdoStringP::Format(L"%.8lf",minx);
        AppendString((FdoString*)buff);
        
        AppendString(L" and ");
        
        AppendString(str_xcol);
        AppendString(L"<=");        
        buff = FdoStringP::Format(L"%.8lf",maxx);
        AppendString((FdoString*)buff);
        
        AppendString(L" and ");
        
        AppendString(str_ycol);
        AppendString(L">=");        
        buff = FdoStringP::Format(L"%.8lf",miny);
        AppendString((FdoString*)buff);
        
        AppendString(L" and ");
        
        AppendString(str_ycol);
        AppendString(L"<=");        
        buff = FdoStringP::Format(L"%.8lf",maxy);
        AppendString((FdoString*)buff);
        
        AppendString(D_FILTER_CLOSE_PARENTH);        
      }      
    }
    else
    {
     
      AppendString(D_FILTER_OPEN_PARENTH);
      AppendString(L"SDO_FILTER(");
      ProcessGeomExpresion( geomprop,orasrid );
      AppendString(L",");
      
      FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(geomexp.p);
      if (geomval)
      {
        GetExpressionProcessor().ProcessGeometryValueRect(*geomval);
      }
      else
      {
        ProcessGeomExpresion( geomexp,orasrid );
      }
         
      if( m_KgOraConn->GetOracleMainVersion() < 10 )
      {
        AppendString(L",'querytype = WINDOW')='TRUE'");
      }
      else
      {
        AppendString(L")='TRUE'");
      }
      AppendString(D_FILTER_CLOSE_PARENTH);
    
    }    
  }
  break;
  case FdoSpatialOperations_Intersects:
  {
    if( m_phys_class.p &&  m_phys_class->GetIsPointGeometry() )
    {
      FdoStringP str_xcol =  m_phys_class->GetPointXOraColumn();
      FdoStringP str_ycol =  m_phys_class->GetPointYOraColumn();
      
      FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(geomexp.p);
      if (geomval)
      {
        FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);
        FdoPtr<FdoIEnvelope> envelope = fgfgeom->GetEnvelope();

        double minx = envelope->GetMinX();
        double miny = envelope->GetMinY();
        
        double maxx = envelope->GetMaxX();
        double maxy = envelope->GetMaxY();    

        FdoStringP buff;
        
        AppendString(D_FILTER_OPEN_PARENTH);
        
        AppendString(str_xcol);
        AppendString(L">=");       
        buff = FdoStringP::Format(L"%.8lf",minx);
        AppendString((FdoString*)buff);
        
        AppendString(L" and ");
        
        AppendString(str_xcol);
        AppendString(L"<=");        
        buff = FdoStringP::Format(L"%.8lf",maxx);
        AppendString((FdoString*)buff);
        
        AppendString(L" and ");
        
        AppendString(str_ycol);
        AppendString(L">=");        
        buff = FdoStringP::Format(L"%.8lf",miny);
        AppendString((FdoString*)buff);
        
        AppendString(L" and ");
        
        AppendString(str_ycol);
        AppendString(L"<=");        
        buff = FdoStringP::Format(L"%.8lf",maxy);
        AppendString((FdoString*)buff);
        
        AppendString(D_FILTER_CLOSE_PARENTH);        
      }
    }
    else
    {    
      AppendString(D_FILTER_OPEN_PARENTH);

	    
      if( m_KgOraConn->GetOracleMainVersion() >= 10 )
      {
        
        AppendString(L"SDO_ANYINTERACT(");
      }
      else
      {
        AppendString(L"SDO_RELATE(");
      }
      
	  
      ProcessGeomExpresion( geomprop,orasrid );
      AppendString(L",");
      
      ProcessGeomExpresion( geomexp,orasrid);
      
      
      if( m_KgOraConn->GetOracleMainVersion() >= 10 )
      {
        AppendString(L")='TRUE'");
      }
      else
      {
        AppendString(L",'mask=ANYINTERACT')='TRUE'");
      }
      
	 

      AppendString(D_FILTER_CLOSE_PARENTH);
    }
  }
  break;
  
  
  /// Test to see if the geometric property value spatially contains the
  /// literal geometric value.
  case FdoSpatialOperations_Contains:
  {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString(L"SDO_RELATE(");
    ProcessGeomExpresion( geomprop,orasrid );
    AppendString(L",");
    ProcessGeomExpresion( geomexp,orasrid );
    AppendString(L",'mask=CONTAINS')='TRUE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;

  /// Test to see if the geometric property value spatially crosses the given
  /// geometry.
  case FdoSpatialOperations_Crosses:
  {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString(L"SDO_RELATE(");
    ProcessGeomExpresion( geomprop,orasrid );
    AppendString(L",");
    ProcessGeomExpresion( geomexp,orasrid );
    AppendString(L",'mask=OVERLAPBDYDISJOINT')='TRUE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;

  case FdoSpatialOperations_Disjoint:
  {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString(L"SDO_RELATE(");
    ProcessGeomExpresion( geomprop,orasrid );
    AppendString(L",");
    ProcessGeomExpresion( geomexp,orasrid );
    AppendString(L",'mask=ANYINTERACT')='FALSE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;
  /// Test to see if the geometric property value spatially overlaps the given
  /// geometry
  case FdoSpatialOperations_Overlaps:
    {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString(L"SDO_RELATE(");
    ProcessGeomExpresion( geomprop,orasrid );
    AppendString(L",");
    ProcessGeomExpresion( geomexp,orasrid );
    AppendString(L",'mask=OVERLAPBDYINTERSECT')='TRUE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;

    /// Test to see if the geometric property value spatially touches the given
    /// geometry.
  case FdoSpatialOperations_Touches:
  {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString(L"SDO_RELATE(");
    ProcessGeomExpresion( geomprop,orasrid );
    AppendString(L",");
    ProcessGeomExpresion( geomexp,orasrid );
    AppendString(L",'mask=TOUCH')='TRUE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;

    /// Test to see if the geometric property value is spatially within the
    /// given geometry.
  case FdoSpatialOperations_Within:
  {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString(L"SDO_RELATE(");
    ProcessGeomExpresion( geomprop,orasrid );
    AppendString(L",");
    ProcessGeomExpresion( geomexp,orasrid );
    AppendString(L",'mask=COVERS')='TRUE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;

    /// Test to see if the geometric property value is covered by the interior
    /// and boundary of the given geometry.
  case FdoSpatialOperations_CoveredBy:
  {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString(L"SDO_RELATE(");
    ProcessGeomExpresion( geomprop,orasrid );
    AppendString(L",");
    ProcessGeomExpresion( geomexp,orasrid );
    AppendString(L",'mask=COVERDBY')='TRUE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;

  /// Test to see if the geometric property value is inside the interior
  /// of the given geometry, not touching the boundary.
  case FdoSpatialOperations_Inside:
  {
      AppendString(D_FILTER_OPEN_PARENTH);
      AppendString(L"SDO_RELATE(");
      ProcessGeomExpresion( geomprop,orasrid );
      AppendString(L",");
      ProcessGeomExpresion( geomexp,orasrid );
      AppendString(L",'mask=INSIDE')='TRUE'");
      AppendString(D_FILTER_CLOSE_PARENTH);
    }
  break;

  

    /// Test to see if the geometric property value is spatially equal to the
    /// given geometry.
    case FdoSpatialOperations_Equals:
    {
      AppendString(D_FILTER_OPEN_PARENTH);
      AppendString(L"SDO_RELATE(");
      ProcessGeomExpresion( geomprop,orasrid );
      AppendString(L",");
      ProcessGeomExpresion( geomexp,orasrid );
      AppendString(L",'mask=EQUAL')='TRUE'");
      AppendString(D_FILTER_CLOSE_PARENTH);
    }
    break;
    
    
  default:
    throw FdoFilterException::Create(L"c_KgOraFilterProcessor::ProcessSpatialCondition Unsupported Spatial Condition Operation");
  break;  
}
}//end of c_KgOraFilterProcessor::ProcessSpatialCondition

void c_KgOraFilterProcessor::ProcessDistanceCondition(FdoDistanceCondition& Filter)
{
FdoPtr<FdoIdentifier> geomprop = Filter.GetPropertyName();

c_KgOraSridDesc orasrid;
m_KgOraConn->GetOracleSridDesc(m_ClassDef,geomprop->GetName(),orasrid);

FdoPtr<FdoExpression> geomval = Filter.GetGeometry();
double dist = Filter.GetDistance();
switch( Filter.GetOperation() )
{
  case FdoDistanceOperations_Within:
  {
    // sprintf(sbuff,"SDO_ANYINTERACT(a.%s,%s)='TRUE'",(const char*)gcolname,sbuff2);
    
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString(L"SDO_WITHIN_DISTANCE(");
    ProcessExpresion( geomprop );
    AppendString(L",");
    ProcessGeomExpresion( geomval,orasrid );
    FdoStringP tmpbuff = FdoStringP::Format(L",'distance=%.6lf'",dist);
    AppendString((FdoString*)tmpbuff);
    AppendString(L")='TRUE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;
  case FdoDistanceOperations_Beyond:
  {
    AppendString(D_FILTER_OPEN_PARENTH);
    AppendString(L"SDO_WITHIN_DISTANCE(");
    ProcessExpresion( geomprop );
    AppendString(L",");
    ProcessGeomExpresion( geomval,orasrid );
    FdoStringP tmpbuff = FdoStringP::Format(L",'distance=%.6lf'",dist);
    AppendString((FdoString*)tmpbuff);
    AppendString(L")='FALSE'");
    AppendString(D_FILTER_CLOSE_PARENTH);
  }
  break;
  default:
    throw FdoFilterException::Create(L"c_KgOraFilterProcessor::ProcessDistanceCondition Unsupported Spatial Condition Operation");
  break;  
}
}//end of c_KgOraFilterProcessor::ProcessDistanceCondition

