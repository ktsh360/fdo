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

using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

using OSGeo.FDO.Schema;
using OSGeo.FDO.Commands.Schema;
using OSGeo.FDO.Commands;
using OSGeo.FDO.Commands.Feature;
using OSGeo.FDO.Expression;

using unit_test.Framework;

namespace unit_test.ProviderTests
{
    class DeleteTests : BaseTestWithConnection
    {
        public DeleteTests(ShpTestProvider connectionProvider)
            : base(connectionProvider)
        { }

        private void create_schema(int type, bool elevation, bool measure)
        {
            // delete the class, if its there:
           ShpTests.CleanUpClass(base.ConnectionInitialized, null, "Test");

            // create the class
            FeatureSchema schema = new FeatureSchema("TheSchema", " test schema");
            ClassCollection classes = schema.Classes;

            DataPropertyDefinition featid = new DataPropertyDefinition("FeatId", "integer");
            featid.DataType = DataType.DataType_Int32;
            featid.IsAutoGenerated = true;
            featid.Nullable = false;

            DataPropertyDefinition id = new DataPropertyDefinition("Id", "integer");
            id.DataType = DataType.DataType_Decimal;
            id.Precision = 10;
            id.Scale = 0;

            DataPropertyDefinition street = new DataPropertyDefinition("Street", "text");
            street.DataType = DataType.DataType_String;
            street.Length = 64;

            DataPropertyDefinition area = new DataPropertyDefinition("Area", "double");
            area.DataType = DataType.DataType_Decimal;
            area.Precision = 20;
            area.Scale = 8;

            DataPropertyDefinition vacant = new DataPropertyDefinition("Vacant", "boolean");
            vacant.DataType = DataType.DataType_Boolean;

            DataPropertyDefinition birthday = new DataPropertyDefinition("Birthday", "date");
            birthday.DataType = DataType.DataType_DateTime;

            // build a location geometry property
            GeometricPropertyDefinition location = new GeometricPropertyDefinition("Geometry", "geometry");
            location.GeometryTypes = type;
            location.HasElevation = elevation;
            location.HasMeasure = measure;

            // assemble the feature class
            FeatureClass feature = new FeatureClass("Test", "test class created with apply schema");
            PropertyDefinitionCollection properties = feature.Properties;
            DataPropertyDefinitionCollection identities = feature.IdentityProperties;
            properties.Add(featid);
            identities.Add(featid);
            properties.Add(id);
            properties.Add(street);
            properties.Add(area);
            properties.Add(vacant);
            properties.Add(birthday);
            properties.Add(location);
            feature.GeometryProperty = location;

            // submit the new schema
            classes.Add(feature);
            IApplySchema apply = (IApplySchema)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_ApplySchema);
            apply.FeatureSchema = schema;
            apply.Execute();
        }

        public void Test_del ()
        {
            try
            {
                create_schema (0x01, true, true);
                IInsert insert = (IInsert)base.ConnectionInitialized.CreateCommand(CommandType.CommandType_Insert);
                insert.SetFeatureClassName("Test");
                PropertyValueCollection values = insert.PropertyValues;
                ValueExpression expression = (ValueExpression)ShpTests.ParseByDataType("24", DataType.DataType_Int32);
                PropertyValue value = new PropertyValue("Id", expression);
                values.Add (value);
                expression = (ValueExpression)Expression.Parse("'1147 Trafford Drive'");
                value = new PropertyValue("Street", expression);
                values.Add (value);
                // add NULL geometry value:
                GeometryValue geometry = new GeometryValue();
                geometry.SetNull();
                value = new PropertyValue("Geometry", geometry);
                values.Add (value);
                IFeatureReader reader = insert.Execute ();
                Int32 featid = -1;
                while (reader.ReadNext ())
                {
                    if (featid != -1)
                        Debug.Assert(false, "too many features inserted");
                    featid = reader.GetInt32 ("FeatId");
                }
                reader.Close ();
                if (featid == -1)
                    Debug.Assert(false, "too few features inserted");

                // delete all features
                IDelete del = (IDelete)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_Delete);
                del.SetFeatureClassName ("Test");
                int n = del.Execute ();

                // check by doing a select
                ISelect select = (ISelect)base.ConnectionInitialized.CreateCommand (CommandType.CommandType_Select);
                select.SetFeatureClassName ("Test");
                reader = select.Execute ();
                Debug.Assert(!reader.ReadNext(), "still features left");
                reader.Close ();
            }
             catch (OSGeo.FDO.Common.Exception ex)
            {
                Debug.Assert(false, ex.Message);
            }
            catch (System.Exception ex)
            {
                Debug.Assert(false, ex.Message);
            }
        }

        //
        // !!!!!!! TODO: Convert these unmsnsged tests to managed tests
        //
        //void DeleteTests::del_geometry ()
        //{
        //    try
        //    {
        //        create_schema (FdoGeometricType_Point, false, false);
        //        IInsert insert = (FdoIInsert*)mConnection.CreateCommand (FdoCommandType_Insert);
        //        insert.SetFeatureClassName ("Test");
        //        PropertyValueCollection values = insert.GetPropertyValues ();
        //        ValueExpression expression = (FdoValueExpression*)ShpTests::ParseByDataType ("24", FdoDataType_Decimal);
        //        PropertyValue value = FdoPropertyValue::Create ("Id", expression);
        //        values.Add (value);
        //        expression = (FdoValueExpression*)FdoExpression::Parse ("'1147 Trafford Drive'");
        //        value = FdoPropertyValue::Create ("Street", expression);
        //        values.Add (value);
        //        // add real geometry value:
        //        GeometryValue geometry = (FdoGeometryValue*)FdoExpression::Parse ("GEOMFROMTEXT ('POINT XY ( 999000 -999000 )')");
        //        value = FdoPropertyValue::Create ("Geometry", geometry);
        //        values.Add (value);
        //        IFeatureReader reader = insert.Execute ();
        //        GisInt32 featid;
        //        featid = -1;
        //        while (reader.ReadNext ())
        //        {
        //            if (-1 != featid)
        //                CPPUNIT_FAIL ("too many features inserted");
        //            featid = reader.GetInt32 ("FeatId");
        //        }
        //        reader.Close ();
        //        if (-1 == featid)
        //            CPPUNIT_FAIL ("too few features inserted");

        //        // delete all features
        //        IDelete del = (FdoIDelete*)mConnection.CreateCommand (FdoCommandType_Delete);
        //        del.SetFeatureClassName ("Test");
        //        GisInt32 n = del.Execute ();

        //        // check by doing a select
        //        ISelect select = (FdoISelect*)mConnection.CreateCommand (FdoCommandType_Select);
        //        select.SetFeatureClassName ("Test");
        //        reader = select.Execute ();
        //        CPPUNIT_ASSERT_MESSAGE ("still features left", !reader.ReadNext ());
        //        reader.Close ();
        //    }
        //    catch (GisException* ge) 
        //    {
        //        fail (ge);
        //    }
        //}

        //void DeleteTests::del_one ()
        //{
        //    try
        //    {
        //        create_schema (FdoGeometricType_Point, false, false);
        //        IInsert insert = (FdoIInsert*)mConnection.CreateCommand (FdoCommandType_Insert);
        //        insert.SetFeatureClassName ("Test");
        //        PropertyValueCollection values = insert.GetPropertyValues ();

        //        GisInt32 featids[3];
        //        int index = 0;

        //        // feat #1
        //        ValueExpression expression = (FdoValueExpression*)ShpTests::ParseByDataType ("24", FdoDataType_Decimal);
        //        PropertyValue value = FdoPropertyValue::Create ("Id", expression);
        //        values.Add (value);
        //        expression = (FdoValueExpression*)FdoExpression::Parse ("'1147 Trafford Drive'");
        //        value = FdoPropertyValue::Create ("Street", expression);
        //        values.Add (value);
        //        GeometryValue geometry = (FdoGeometryValue*)FdoExpression::Parse ("GEOMFROMTEXT ('POINT XY ( 999000 -999000 )')");
        //        value = FdoPropertyValue::Create ("Geometry", geometry);
        //        values.Add (value);
        //        IFeatureReader reader = insert.Execute ();
        //        while (reader.ReadNext ())
        //        {
        //            if (index  2)
        //                CPPUNIT_FAIL ("too many features inserted");
        //            featids[index++] = reader.GetInt32 ("FeatId");
        //        }
        //        reader.Close ();

        //        // feat #2
        //        expression = (FdoValueExpression*)ShpTests::ParseByDataType ("9191", FdoDataType_Decimal);
        //        value = FdoPropertyValue::Create ("Id", expression);
        //        values.Add (value);
        //        expression = (FdoValueExpression*)FdoExpression::Parse ("'2611 Misener Crescent'");
        //        value = FdoPropertyValue::Create ("Street", expression);
        //        values.Add (value);
        //        geometry = (FdoGeometryValue*)FdoExpression::Parse ("GEOMFROMTEXT ('POINT XY ( 324.65 7687.0 )')");
        //        value = FdoPropertyValue::Create ("Geometry", geometry);
        //        values.Add (value);
        //        reader = insert.Execute ();
        //        while (reader.ReadNext ())
        //        {
        //            if (index  2)
        //                CPPUNIT_FAIL ("too many features inserted");
        //            featids[index++] = reader.GetInt32 ("FeatId");
        //        }
        //        reader.Close ();

        //        // feat #3
        //        expression = (FdoValueExpression*)ShpTests::ParseByDataType ("46", FdoDataType_Decimal);
        //        value = FdoPropertyValue::Create ("Id", expression);
        //        values.Add (value);
        //        expression = (FdoValueExpression*)FdoExpression::Parse ("'99 Rockcliffe Place'");
        //        value = FdoPropertyValue::Create ("Street", expression);
        //        values.Add (value);
        //        geometry = (FdoGeometryValue*)FdoExpression::Parse ("GEOMFROMTEXT ('POINT XY ( 2828.26 78127.83 )')");
        //        value = FdoPropertyValue::Create ("Geometry", geometry);
        //        values.Add (value);
        //        reader = insert.Execute ();
        //        while (reader.ReadNext ())
        //        {
        //            if (index  2)
        //                CPPUNIT_FAIL ("too many features inserted");
        //            featids[index++] = reader.GetInt32 ("FeatId");
        //        }
        //        reader.Close ();

        //        if (2  index)
        //            CPPUNIT_FAIL ("too few features inserted");

        //        // delete feature # 2
        //        IDelete del = (FdoIDelete*)mConnection.CreateCommand (FdoCommandType_Delete);
        //        del.SetFeatureClassName ("Test");
        //        wchar_t buffer[1024];
        //        swprintf (buffer, ELEMENTS(buffer), "FeatId = %d", featids[1]);
        //        del.SetFilter (Filter(FdoFilter::Parse (buffer)));
        //        GisInt32 n = del.Execute ();

        //        // check by doing a select
        //        ISelect select = (FdoISelect*)mConnection.CreateCommand (FdoCommandType_Select);
        //        select.SetFeatureClassName ("Test");
        //        reader = select.Execute ();
        //        CPPUNIT_ASSERT_MESSAGE ("not enough features", reader.ReadNext ());
        //        CPPUNIT_ASSERT_MESSAGE ("wrong feat id", featids[0] == reader.GetInt32 ("FeatId"));
        //        CPPUNIT_ASSERT_MESSAGE ("not enough features", reader.ReadNext ());
        //        CPPUNIT_ASSERT_MESSAGE ("wrong feat id", featids[2] == reader.GetInt32 ("FeatId"));
        //        CPPUNIT_ASSERT_MESSAGE ("still features left", !reader.ReadNext ());
        //        reader.Close ();
        //    }
        //    catch (GisException* ge) 
        //    {
        //        fail (ge);
        //    }
        //}

    }
}

