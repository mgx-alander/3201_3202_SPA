#pragma once
// Note 1
#include <cppunit/extensions/HelperMacros.h>
class TestProcTable : 
	public CPPUNIT_NS::TestFixture // Note 2 
{
	CPPUNIT_TEST_SUITE(TestProcTable); // Note 3 
	CPPUNIT_TEST(testInsertAndRetrieveProcs);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	// method to test the assigning and retrieval of procs
	void testInsertAndRetrieveProcs();
};
