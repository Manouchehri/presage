
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


#include "predictorsTestMockObjects.h"


////////////////////////////
// Mock HistoryTracker class

ContextTracker::ContextTracker(Configuration* config,
			       PredictorRegistry* predictorRegistry,
			       PresageCallback* callback,
			       const char wc[],
			       const char sc[],
			       const char bc[],
			       const char cc[])
    : logger ("MockContextTracker", std::cerr),
      dispatcher (this)
{
    const char** history = (const char**) config;
    wordChars       = history[2]; // getPrefix()
    separatorChars  = history[1]; // getToken(1)
    blankspaceChars = history[0]; // getToken(2)
}

ContextTracker::~ContextTracker()
{}

void ContextTracker::update()
{}

std::string ContextTracker::getPrefix() const
{
    return wordChars;
}

std::string ContextTracker::getToken(const int index) const
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

std::string ContextTracker::getExtraTokenToLearn(const int index, const std::vector<std::string>& change) const
{ std::string result; return result; }

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

bool ContextTracker::isWordChar      (const char) const
{ return true; }

bool ContextTracker::isSeparatorChar (const char) const
{ return true; }

bool ContextTracker::isControlChar   (const char) const
{ return true; }

bool ContextTracker::isBlankspaceChar(const char) const
{ return true; }

void ContextTracker::update (const Observable* variable)
{ /* intentionally empty */ }
