
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


#ifndef PRESAGE_ARPAPREDICTOR
#define PRESAGE_ARPAPREDICTOR

#include "predictor.h"
#include "../core/logger.h"
#include "../core/progress.h"
#include "../core/dispatcher.h"

#include <assert.h>
#include <fstream>
#include <iomanip>


class cmp {
 public:
  bool operator() (const float& f1, const float& f2 ) const {
    return f2 < f1;
  }
};

class ARPAData
{
  public:
    ARPAData() {};
    ARPAData(float lp,float la) : logProb(lp), logAlfa(la) {};
    float logProb;
    float logAlfa;
};

class TrigramKey
{
  public:
    TrigramKey(int wd1, int wd2, int wd3) : key1(wd1), key2(wd2), key3(wd3) {};

    bool operator<(const TrigramKey &right) const
    {
      if(key1 < right.key1)
        return true;

      if(key1 == right.key1)
        if(key2 < right.key2 )
          return true;

      if(key1 == right.key1 && key2 == right.key2)
        if(key3 < right.key3)
          return true;

      return false;
    }

    bool operator==(const TrigramKey &right) const
    {
      return (key1 == right.key1 && key2 == right.key2 && key3 == right.key3 );
    }
    int key1;
    int key2;
    int key3;
};

class BigramKey
{
  public:
    BigramKey(int wd1, int wd2) : key1(wd1), key2(wd2) {};

    bool operator<(const BigramKey &right) const
    {
      if(key1 < right.key1)
        return true;

      if(key1 == right.key1)
        if(key2 < right.key2 )
          return true;

      return false;
    }

    bool operator==(const TrigramKey &right) const
    {
      return (key1 == right.key1 && key2 == right.key2);
    }
    int key1;
    int key2;
};

/** Smoothed n-gram statistical predictor.
 *
 */
class ARPAPredictor : public Predictor, public Observer {

public:
    ARPAPredictor(Configuration*, ContextTracker*, const char*);
    ~ARPAPredictor();

    virtual Prediction predict(const size_t size, const char** filter) const;

    virtual void learn(const std::vector<std::string>& change);

    virtual void update (const Observable* variable);

    void set_vocab_filename (const std::string& value);
    void set_arpa_filename (const std::string& value);
    void set_timeout (const std::string& value);

private:
    std::string LOGGER;
    std::string ARPAFILENAME;
    std::string VOCABFILENAME;
    std::string TIMEOUT;

    std::string arpaFilename;
    std::string vocabFilename;
    int timeout;

    std::map<std::string,int> vocabCode;
    std::map<int,std::string> vocabDecode;

    std::map<int,ARPAData> unigramMap;
    std::map<BigramKey,ARPAData>bigramMap;
    std::map<TrigramKey,float>trigramMap;

    void loadVocabulary();
    void createARPATable();
    bool matchesPrefixAndFilter(std::string , std::string , const char**  ) const;

    void addUnigram(std::string);
    void addBigram(std::string);
    void addTrigram(std::string);

    inline float computeTrigramBackoff(int,int,int) const;
    inline float computeBigramBackoff(int,int) const;

    int unigramCount;
    int bigramCount;
    int trigramCount;

    int unigramTot;
    int bigramTot;
    int trigramTot;

    ProgressBar<char>* unigramProg;
    ProgressBar<char>* bigramProg;
    ProgressBar<char>* trigramProg;

    Dispatcher<ARPAPredictor> dispatcher;
};

#endif // PRESAGE_ARPAPREDICTOR
