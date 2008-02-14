
/******************************************************************************\
 *                                                                            *
 * Soothsayer, an extensible predictive text entry system                     *
 * ------------------------------------------------------                     *
 *                                                                            *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>             *
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


#include "pluginsTestMockObjects.h"

/////////////////////
// Mock Profile class

Profile::Profile(TiXmlDocument* doc)
{
    configuration = (Configuration*) doc;
}

Profile::~Profile()
{}

Value Profile::getConfig(const Variable& variable)
{
    std::map<std::string, std::string>* config = 
	( std::map<std::string, std::string>* ) configuration;

    std::string key = variable.string();

    return (*config)[key];
}


////////////////////////////
// Mock HistoryTracker class

ContextTracker::ContextTracker(Profile* profile,
			       const char wc[],
			       const char sc[],
			       const char bc[],
			       const char cc[])
    : logger("MockContextTracker", std::cerr)
{
    const char** history = (const char**) profile;
    wordChars       = history[2]; // getPrefix()
    separatorChars  = history[1]; // getToken(1)
    blankspaceChars = history[0]; // getToken(2)
}

ContextTracker::~ContextTracker()
{}

void ContextTracker::update(std::string update)
{}

std::string ContextTracker::getPrefix()
{
    return wordChars;
}

std::string ContextTracker::getToken(const int index)
{ 
    std::string result;
    switch (index) {
    case 0:
	result = wordChars;
	break;
    case 1:
	result = separatorChars;
	break;
    case 2:
	result = blankspaceChars;
	break;
    default:
	break;
    }
    return result;
}

std::string ContextTracker::getFutureStream() const
{ std::string result; return result; }

std::string ContextTracker::getPastStream() const
{ std::string result; return result; }

std::string ContextTracker::getWordChars()       const
{ std::string result; return result; }

std::string ContextTracker::getSeparatorChars()  const
{ std::string result; return result; }

std::string ContextTracker::getBlankspaceChars() const
{ std::string result; return result; }

std::string ContextTracker::getControlChars()    const
{ std::string result; return result; }

std::string ContextTracker::toString() const
{ std::string result; return result; }

int  ContextTracker::getMaxBufferSize() const
{ int result; return result; }

void ContextTracker::setMaxBufferSize(const int)
{}

bool ContextTracker::isWordChar      (const char) const
{ return true; }

bool ContextTracker::isSeparatorChar (const char) const
{ return true; }

bool ContextTracker::isControlChar   (const char) const
{ return true; }

bool ContextTracker::isBlankspaceChar(const char) const
{ return true; }


