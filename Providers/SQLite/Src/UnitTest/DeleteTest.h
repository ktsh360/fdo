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

#ifndef DELETE_TEST_H_
#define DELETE_TEST_H_

#ifdef _WIN32
#pragma once
#endif

#include "Fdo.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <map>

/* 
 * Tests spatial contexts.
 * This is just an initial version to test a defect regression.
 * More tests would need to be added to provide full coverage.
 */

class DeleteTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( DeleteTest );
  CPPUNIT_TEST( TestSimpleDelete );
  CPPUNIT_TEST( TestBindDelete );
  CPPUNIT_TEST( TestSpatialDelete );
  CPPUNIT_TEST( TestComplexDelete );
  CPPUNIT_TEST(TestComplexWithBindDelete);
  CPPUNIT_TEST_SUITE_END();

public:
    DeleteTest(void);
    virtual ~DeleteTest(void);
    void setUp ();
	void tearDown ();

    void TestSimpleDelete ();
    void TestBindDelete ();
    void TestSpatialDelete ();
    void TestComplexDelete ();
    void TestComplexWithBindDelete();
};

#endif