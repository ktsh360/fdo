//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "Pch.h"
#include "PgGeometryTest.h"
// EWKB<->FGF
#include <PgGeometry.h>
//std
#include <string>
// boost
#include <boost/cstdint.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(PgGeometryTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PgGeometryTest, "PgGeometryTest");

// Namespace defining EWKB geometry parser
using namespace fdo::postgis::ewkb;

PgGeometryTest::PgGeometryTest()
{
}

PgGeometryTest::~PgGeometryTest()
{
}

void PgGeometryTest::setUp()
{
}

void PgGeometryTest::tearDown()
{
}

void PgGeometryTest::testPoint()
{
    //POINT(1.234 5.678)

    ewkb_t ewkb;
    std::string original("POINT (1.234 5.678)");
    std::string ewkbhex("01010000005839B4C876BEF33F83C0CAA145B61640");

    hex_to_bytes(ewkbhex, ewkb);
    CPPUNIT_ASSERT(!ewkb.empty());
    FdoPtr<FdoIGeometry> g = CreateGeometryFromExtendedWkb(ewkb);
    CPPUNIT_ASSERT(NULL != g);

    FdoStringP tmp(g->GetText());
    CPPUNIT_ASSERT(tmp.GetLength() > 0);
    std::string output(static_cast<char const*>(tmp));
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPointSRID()
{
    //SRID=32632;POINT(1.234 5.678)

    ewkb_t ewkb;
    std::string original("POINT (1.234 5.678)");
    std::string ewkbhex("0101000020787F00005839B4C876BEF33F83C0CAA145B61640");

    hex_to_bytes(ewkbhex, ewkb);
    CPPUNIT_ASSERT(!ewkb.empty());
    FdoPtr<FdoIGeometry> g = CreateGeometryFromExtendedWkb(ewkb);
    CPPUNIT_ASSERT(NULL != g);

    FdoStringP tmp(g->GetText());
    CPPUNIT_ASSERT(tmp.GetLength() > 0);
    std::string output(static_cast<char const*>(tmp));
    CPPUNIT_ASSERT_EQUAL(original, output);
}
