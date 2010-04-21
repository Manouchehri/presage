
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


#ifndef PRESAGE_DEFAULTPROFILE
#define PRESAGE_DEFAULTPROFILE

#include "profile.h"

/** Default profile.
 *
 */
class DefaultProfile : public Profile {
public:
    /** Profile constructor.
     *
     * \param filename where profile is saved to
     */
    DefaultProfile(const std::string& filename);

    /** Profile destructor.
     *
     * Destructor deallocates the Configuration object passed in costructor.
     *
     */
    ~DefaultProfile();

    static const char*        DEFAULT_PROFILE_FILENAME;
    static const std::string  DEFAULT_LOGGER_LEVEL;
    static const int          DEFAULT_PREDICT_TIME;
    static const int          DEFAULT_MAX_PARTIAL_PREDICTION_SIZE;
    static const std::string  DEFAULT_COMBINATION_POLICY;
    static const std::string  DEFAULT_PREDICTIVE_PREDICTORS;
    static const int          DEFAULT_SLIDING_WINDOW_SIZE;
    static const size_t       DEFAULT_SUGGESTIONS;
    static const bool         DEFAULT_REPEAT_SUGGESTION;
    static const size_t       DEFAULT_GREEDY_SUGGESTION_THRESHOLD;
    static const std::string  DEFAULT_PREDICTORS;
    static const char*        DEFAULT_AUTOPERSIST;

private:

    void build_xml_document (const std::string&);

};

#endif // PRESAGE_DEFAULTPROFILE
