
/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
                                                                             *
                                                                **********(*)*/


#include "abbreviationExpansionPluginTest.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

CPPUNIT_TEST_SUITE_REGISTRATION( AbbreviationExpansionPluginTest );

AbbreviationExpansionPluginTest::AbbreviationExpansionPluginTest()
{
    STANDARD_ABBREVIATION_FILE = static_cast<std::string>(getenv("srcdir"))
	+ "/../../../resources/abbreviations_en.txt";
}

void AbbreviationExpansionPluginTest::setUp()
{}

void AbbreviationExpansionPluginTest::tearDown()
{}

void AbbreviationExpansionPluginTest::testSimpleExpansion()
{
    const char* config[] = { STANDARD_ABBREVIATION_FILE.c_str() };
    const char* history[] = { "foo", "bar", "2moro" };

    Prediction prediction = runPredict(config, history);

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), prediction.size());
    CPPUNIT_ASSERT_EQUAL(std::string("\b\b\b\b\btomorrow"), prediction.getSuggestion(0).getWord());
}

void AbbreviationExpansionPluginTest::testLongExpansion()
{
    const char* config[] = { STANDARD_ABBREVIATION_FILE.c_str() };
    const char* history[] = { "foo", "bar", "ianal" };

    Prediction prediction = runPredict(config, history);

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), prediction.size());
    CPPUNIT_ASSERT_EQUAL(std::string("\b\b\b\b\bI am not a lawyer"), prediction.getSuggestion(0).getWord());
}

void AbbreviationExpansionPluginTest::testUnknownAbbreviation()
{
    const char* config[] = { STANDARD_ABBREVIATION_FILE.c_str() };
    const char* history[] = { "foo", "bar", "foobar" };

    Prediction prediction = runPredict(config, history);

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), prediction.size());
}

Configuration* AbbreviationExpansionPluginTest::prepareConfiguration(const char* config[]) const
{
    std::cerr << "AbbreviationExpansionPluginTest::prepareConfigMap" << std::endl;

    Configuration* result = new Configuration();
    result->set(Variable("Presage.Plugins.AbbreviationExpansionPlugin.LOGGER"), "ALL");
    result->set(Variable("Presage.Plugins.AbbreviationExpansionPlugin.ABBREVIATIONS"), config[0]);

    return result;
}

Plugin* AbbreviationExpansionPluginTest::createPlugin(Configuration* configuration, ContextTracker* ct) const
{
    std::cerr << "AbbreviationExpansionPluginTest::createPlugin" << std::endl;
    return new AbbreviationExpansionPlugin(configuration, ct);
}
