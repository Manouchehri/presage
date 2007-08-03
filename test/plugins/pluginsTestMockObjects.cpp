
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
    profile = doc;
}

Profile::~Profile()
{}

Value Profile::getConfig(Variable variable)
{
    std::map<std::string, std::string>* config = 
	( std::map<std::string, std::string>* ) profile;

    std::string key = variable[variable.size() - 1];

    return (*config)[key];
}


////////////////////////////
// Mock HistoryTracker class

HistoryTracker::HistoryTracker(Profile* profile,
			       const char wc[],
			       const char sc[],
			       const char bc[],
			       const char cc[])
{
    const char** history = (const char**) profile;
    wordChars       = history[2]; // getPrefix()
    separatorChars  = history[1]; // getToken(1)
    blankspaceChars = history[0]; // getToken(2)
}

HistoryTracker::~HistoryTracker()
{}

void HistoryTracker::update(std::string update)
{}

std::string HistoryTracker::getPrefix()
{
    return wordChars;
}

std::string HistoryTracker::getToken(const int index)
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

std::string HistoryTracker::getFutureStream() const
{ std::string result; return result; }

std::string HistoryTracker::getPastStream() const
{ std::string result; return result; }

std::string HistoryTracker::getWordChars()       const
{ std::string result; return result; }

std::string HistoryTracker::getSeparatorChars()  const
{ std::string result; return result; }

std::string HistoryTracker::getBlankspaceChars() const
{ std::string result; return result; }

std::string HistoryTracker::getControlChars()    const
{ std::string result; return result; }

std::string HistoryTracker::toString() const
{ std::string result; return result; }

int  HistoryTracker::getMaxBufferSize() const
{ int result; return result; }

void HistoryTracker::setMaxBufferSize(const int)
{}

bool HistoryTracker::isWordChar      (const char) const
{ return true; }

bool HistoryTracker::isSeparatorChar (const char) const
{ return true; }

bool HistoryTracker::isControlChar   (const char) const
{ return true; }

bool HistoryTracker::isBlankspaceChar(const char) const
{ return true; }


