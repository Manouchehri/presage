
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


#include "dejavuPredictor.h"

#include <algorithm>

/*
 * Implementation idea: predictor remembers previously entered text (by
 * storing it in a config defined file); when current token is found
 * in the remembered text, the tokens following the current token are
 * suggested; this requires "learning" previously entered text.
 *
 */

DejavuPredictor::DejavuPredictor(Configuration* config, ContextTracker* ct, const char* name)
    : Predictor(config,
                ct,
                name,
                "DejavuPredictor, a parrot predictor",
                "DejavuPredictor is a parrot predictor.\n"
                "It always returns what it has heard before.\n"),
      dispatcher (this)
{
    LOGGER = PREDICTORS + name + ".LOGGER";
    MEMORY = PREDICTORS + name + ".MEMORY";
    TRIGGER = PREDICTORS + name + ".TRIGGER";

    // build notification dispatch map
    dispatcher.map (config->find (LOGGER), & DejavuPredictor::set_logger);
    dispatcher.map (config->find (MEMORY), & DejavuPredictor::set_memory);
    dispatcher.map (config->find (TRIGGER), & DejavuPredictor::set_trigger);
}

DejavuPredictor::~DejavuPredictor()
{}

void DejavuPredictor::set_memory (const std::string& filename)
{
    memory = filename;
    logger << INFO << "MEMORY: " << filename << endl;
}

void DejavuPredictor::set_trigger (const std::string& number)
{
    trigger = Utility::toInt (number);
    logger << INFO << "TRIGGER: " << number << endl;
}

Prediction DejavuPredictor::predict(const size_t max_partial_predictions_size, const char** filter) const
{
    Prediction result;

    std::list<std::string> memory_trigger;
    
    std::ifstream memory_file(memory.c_str());
    if (!memory_file) {
        logger << ERROR << "Error opening memory file: " << memory << endl;
    } else {
        if (init_memory_trigger(memory_trigger)) {
            logger << INFO << "Memory trigger init'ed" << endl;

            std::list<std::string> rolling_window;
            if (init_rolling_window(rolling_window, memory_file)) {
                logger << INFO << "Rolling window init'ed" << endl;

                std::string token;
                while (memory_file >> token) {
                    logger << INFO << "Analyzing token: " << token << endl;

                    if (match(memory_trigger, rolling_window)) {
                        logger << INFO << "Token triggered memory: " << token << endl;
                        if (token_satisfies_filter (token, contextTracker->getPrefix(), filter)) {
                            logger << INFO << "Adding suggestion: " << token << endl;
                            result.addSuggestion(Suggestion(token, 1.0));
                        }
                    }

                    update_rolling_window(rolling_window, token);
                }

            } else {
                logger << INFO << "Rolling window initialization failed." << endl;
            }
        } else {
            logger << INFO << "Memory trigger initialization failed." << endl;
        }

        memory_file.close();
    }

    return result;
}

void DejavuPredictor::learn(const std::vector<std::string>& change)
{
    // loop through all tokens in change vector
    for (std::vector<std::string>::const_iterator it = change.begin();
         it != change.end();
         ++it)
    {
        std::string new_token = *it;
        logger << INFO << "Committing new token to memory: " << new_token << endl;
        std::ofstream memory_file(memory.c_str(), std::ios::app);
        if (!memory_file) {
            logger << ERROR << "Error opening memory file: " << memory << endl;
        } else {
            memory_file << new_token << std::endl;
            memory_file.close();
        }
    }
}

/** Tests two list arguments match.
 *
 * @return true if lists contain the same tokens in the same order,
 * false otherwise
 */
bool DejavuPredictor::match(const std::list<std::string>& l1, const std::list<std::string>& l2) const
{
    return equal(l1.begin(), l1.end(), l2.begin());
}

/** Initialize memory trigger.
 *
 * @param memory_trigger contains tokens that will trigger a memory
 * recollection
 * 
 * @return true if memory trigger has been populated with enough
 * tokens to trigger a memory recollection, false otherwise
 *
 */
bool DejavuPredictor::init_memory_trigger(std::list<std::string>& memory_trigger) const
{
    bool result = false;

    // fill up the token window list that contains the tokens that
    // will trigger a recollection
    for (int i = trigger; i > 0; i--) {
        logger << INFO << "Memory trigger list: " << contextTracker->getToken(i) << endl;
        memory_trigger.push_back(contextTracker->getToken(i));
    }

    // check that the current context is rich enough to trigger a
    // recollection
    if (memory_trigger.end() == find(memory_trigger.begin(), memory_trigger.end(), "")) {
        result = true;
    }

    logger << INFO << "Memory trigger valid: " << result << endl;

    return result;
}

/** Initialize rolling window.
 *
 * @return true if initialized rolling window has been populated with
 * enough tokens to trigger a memory recollection, false otherwise
 *
 */
bool DejavuPredictor::init_rolling_window(std::list<std::string>& rolling_window, std::ifstream& memory_file) const
{
    std::string token;
    int count = 0;
    // following while test relies on the fact that if first condition
    // is true, then the second condition will not be evaluated: in
    // other words, a token will not be read from file if count is not
    // less than trigger.
    while (count < trigger && memory_file >> token) {
        logger << INFO << "Rolling window list: " << token << endl;
        rolling_window.push_back(token);
        count++;
    }

    return (count == trigger);
}

/** Update rolling window.
 *
 * Pop front token and push back new token to list.
 */
void DejavuPredictor::update_rolling_window(std::list<std::string>& rolling_window, const std::string& token) const
{
    rolling_window.pop_front();
    logger << INFO << "Pushing back on memory list: " << token << endl;
    rolling_window.push_back(token);
}

void DejavuPredictor::update (const Observable* var)
{
    logger << DEBUG << "About to invoke dispatcher: " << var->get_name () << " - " << var->get_value() << endl;
    dispatcher.dispatch (var);
}
