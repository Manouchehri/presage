
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


#include "defaultProfile.h"

#include "dirs.h"

#include <sstream>

const char*        DefaultProfile::DEFAULT_PROFILE_FILENAME            = "presage.xml";

DefaultProfile::DefaultProfile (const std::string& filename)
  : Profile (filename)
{
    build_xml_document (filename);
}

DefaultProfile::~DefaultProfile()
{
    // complete
}

void DefaultProfile::build_xml_document (const std::string& filename)
{
    const char* xml =
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
"<Presage>"
"    <PredictorRegistry>"
"        <LOGGER>ERROR</LOGGER>"
"        <!-- PREDICTORS"
"          Space separated list of predictors to use to generate predictions"
"        -->"
"        <PREDICTORS>DefaultAbbreviationExpansionPredictor DefaultSmoothedNgramPredictor UserSmoothedNgramPredictor DefaultRecencyPredictor</PREDICTORS>"
"    </PredictorRegistry>"
"    <ContextTracker>"
"        <LOGGER>ERROR</LOGGER>"
"        <!-- SLIDING_WINDOW_SIZE"
"          Size of buffer used by context tracker to detect context changes"
"        -->"
"        <SLIDING_WINDOW_SIZE>80</SLIDING_WINDOW_SIZE>"
"        <!-- LOWERCASE_MODE"
"          Instruct context tracker to track text as lowercase"
"        -->"
"         <LOWERCASE_MODE>yes</LOWERCASE_MODE>"
"    </ContextTracker>"
"    <Selector>"
"        <LOGGER>ERROR</LOGGER>"
"        <!-- SUGGESTIONS"
"          Controls how many suggestions are returned in each prediction."
"        -->"
"        <SUGGESTIONS>6</SUGGESTIONS>"
"        <!-- REPEAT_SUGGESTIONS"
"          Allow the same suggestion to be offered in subsequent"
"          predictions, even if no context change has been detected."
"        -->"
"        <REPEAT_SUGGESTIONS>no</REPEAT_SUGGESTIONS>"
"        <!-- GREEDY_SUGGESTION_THRESHOLD"
"          Select only tokens whose completion length is greater than"
"          the specified greedy suggestion threshold."
"          i.e. If this option is set to 2 and the current prefix is"
"               \"cu\", then the word \"cub\" will not offered as a"
"               suggestion, because the completion's length is only one"
"               character long. Tokens \"curb\" or \"cube\" or \"cubicle\" or"
"               \"cucumber\" will however be offered, because these"
"               words' completions are at least 2 characters long."
"        -->"
"        <GREEDY_SUGGESTION_THRESHOLD>0</GREEDY_SUGGESTION_THRESHOLD>"
"    </Selector>"
"    <PredictorActivator>"
"        <LOGGER>ERROR</LOGGER>"
"        <!-- PREDICT_TIME"
"          Maximum time allowed for predictors to return their prediction."
"        -->"
"        <PREDICT_TIME>1000</PREDICT_TIME>"
"        <!-- MAX_PARTIAL_PREDICTION_SIZE"
"          Desired size of each prediction prior to combination phase."
"        -->"
"        <MAX_PARTIAL_PREDICTION_SIZE>60</MAX_PARTIAL_PREDICTION_SIZE>"
"        <!-- COMBINATION_POLICY"
"          policy used by predictor to combine predictions returned"
"          by the active predictors into one prediction."
"        -->"
"        <COMBINATION_POLICY>Meritocracy</COMBINATION_POLICY>"
"    </PredictorActivator>"
"    <ProfileManager>"
"        <LOGGER>ERROR</LOGGER>"
"        <!-- AUTOPERSIST"
"             Automatically saves configuration to active profile."
"          -->"
"        <AUTOPERSIST>false</AUTOPERSIST>"
"    </ProfileManager>"
"    <Predictors>"
"        <DefaultSmoothedNgramPredictor>"
"            <PREDICTOR>SmoothedNgramPredictor</PREDICTOR>"
"            <LOGGER>ERROR</LOGGER>"
"            <DBFILENAME>" pkgdatadir "/database_en.db</DBFILENAME>"
"            <!-- $delta_0, $delta_1, ..., $delta_{n-1} -->"
"            <DELTAS>0.01 0.1 0.89</DELTAS>"
"            <LEARN>false</LEARN>"
"            <DatabaseConnector>"
"                <LOGGER>ERROR</LOGGER>"
"            </DatabaseConnector>"
"        </DefaultSmoothedNgramPredictor>"
"        <UserSmoothedNgramPredictor>"
"            <PREDICTOR>SmoothedNgramPredictor</PREDICTOR>"
"            <LOGGER>ERROR</LOGGER>"
"            <!-- ${HOME} is special. It expands to:"
"                  - $HOME on Unix"
"                  - %USERPROFILE% on Windows"
"              -->"
"            <DBFILENAME>${HOME}/.presage/lm.db</DBFILENAME>"
"            <!-- delta_0, delta_1, ..., delta_{n-1} -->"
"            <DELTAS>0.01 0.1 0.89</DELTAS>"
"            <LEARN>true</LEARN>"
"            <DatabaseConnector>"
"                <LOGGER>ERROR</LOGGER>"
"            </DatabaseConnector>"
"        </UserSmoothedNgramPredictor>"
"        <DefaultRecencyPredictor>"
"            <PREDICTOR>RecencyPredictor</PREDICTOR>"
"            <LOGGER>ERROR</LOGGER>"
"            <LAMBDA>1</LAMBDA>"
"            <N_0>1</N_0>"
"            <CUTOFF_THRESHOLD>20</CUTOFF_THRESHOLD>"
"        </DefaultRecencyPredictor>"
"        <DefaultDictionaryPredictor>"
"            <PREDICTOR>DictionaryPredictor</PREDICTOR>"
"            <DICTIONARY>/usr/share/dict/words</DICTIONARY>"
"            <!-- fixed probability assigned to prediction -->"
"            <PROBABILITY>0.000001</PROBABILITY>"
"        </DefaultDictionaryPredictor>"
"       <DefaultAbbreviationExpansionPredictor>"
"            <PREDICTOR>AbbreviationExpansionPredictor</PREDICTOR>"
"            <LOGGER>ERROR</LOGGER>"
"            <ABBREVIATIONS>" pkgdatadir "/abbreviations_en.txt</ABBREVIATIONS>"
"       </DefaultAbbreviationExpansionPredictor>"
"       <DefaultDejavuPredictor>"
"            <PREDICTOR>DejavuPredictor</PREDICTOR>"
"            <LOGGER>ERROR</LOGGER>"
"            <MEMORY>" pkgdatadir "/dejavu_memory_en.txt</MEMORY>"
"            <TRIGGER>3</TRIGGER>"
"       </DefaultDejavuPredictor>"
"        <DefaultARPAPredictor>"
"            <PREDICTOR>ARPAPredictor</PREDICTOR>"
"            <LOGGER>ERROR</LOGGER>"
"            <ARPAFILENAME>" pkgdatadir "/arpa_en.arpa</ARPAFILENAME>"
"            <VOCABFILENAME>" pkgdatadir "/arpa_en.vocab</VOCABFILENAME>"
"            <TIMEOUT>100</TIMEOUT>"
"        </DefaultARPAPredictor>"
"    </Predictors>"
"</Presage>";

  xmlProfileDoc->Parse (xml);

}
