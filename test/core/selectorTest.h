
/******************************************************************************\
 *                                                                            *
 * Soothsayer, an extensible predictive text entry system                     *
 * ------------------------------------------------------                     *
 *                                                                            *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@tiscali.it>             *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software                *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*
 *                                                                            *
\******************************************************************************/        


#ifndef SOOTH_SELECTORTEST
#define SOOTH_SELECTORTEST

#include <cppunit/extensions/HelperMacros.h>

#include <core/selector.h>
//#include <core/historyTracker.h>
#include <core/prediction.h>
#include <tokenizer/testStringSuite.h>

class SelectorTest : public CppUnit::TestFixture { 
public:
    void setUp();
    void tearDown();
    
    // R = repeat
    // NR = no repeat
    // Tn = threshold set to n
    // Sn = suggestions set to n
    void testSelect_S6_R_T0();
    void testSelect_S6_NR_T0();
    void testSelect_S6_R_T3();
    void testSelect_S6_NR_T3();

private:

    class TestDataSuite {
    public:
	TestDataSuite();
	virtual ~TestDataSuite();
	virtual void init();

	virtual bool hasMoreTestData() const;
	virtual void nextTestData();

	virtual std::string getUpdateString()     const { return iter->updateString;     }
	virtual Prediction  getInputPrediction()  const { return iter->inputPrediction;  }
	virtual Prediction  getOutputPrediction() const { return iter->outputPrediction; }

    protected:
        struct TestData {
            std::string updateString;
            Prediction  inputPrediction;
            Prediction  outputPrediction;
        };

        std::vector<TestData> testData;
	std::vector<TestData>::const_iterator iter;
    };

    class TestDataSuite_S6_NR_T0 : public TestDataSuite {
    public:
	TestDataSuite_S6_NR_T0();
	virtual void init();
    };

    class TestDataSuite_S6_R_T0 : public TestDataSuite {
    public:
	TestDataSuite_S6_R_T0();
	virtual void init();
    };

    class TestDataSuite_S6_NR_T3 : public TestDataSuite {
    public:
	TestDataSuite_S6_NR_T3();
	virtual void init();
    };

    class TestDataSuite_S6_R_T3 : public TestDataSuite {
    public:
	TestDataSuite_S6_R_T3();
	virtual void init();
    };

    void testSelect(TestDataSuite*);

    TestStringSuite* testStringSuite;
    TestDataSuite*   tds_S6_NR_T0;
    TestDataSuite*   tds_S6_R_T0;
    TestDataSuite*   tds_S6_NR_T3;
    TestDataSuite*   tds_S6_R_T3;
    HistoryTracker*  historyTracker;
    Selector*        selector;

    CPPUNIT_TEST_SUITE( SelectorTest );
    CPPUNIT_TEST( testSelect_S6_R_T0 );
    CPPUNIT_TEST( testSelect_S6_NR_T0);
    CPPUNIT_TEST( testSelect_S6_R_T3 );
    CPPUNIT_TEST( testSelect_S6_NR_T3);
    CPPUNIT_TEST_SUITE_END();
};

#endif // SOOTH_SELECTORTEST
