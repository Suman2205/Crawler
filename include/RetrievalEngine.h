#ifndef RETRIEVALENGINE_H
#define RETRIEVALENGINE_H

#include <string>

#include "Indexer.h"
#include "URLNormalizer.h"
#include "Tokenizer.h"
#include "StopWordRemover.h"
#include "SearchPosting.h"
#include "RankedPage.h"
#include "RankingEngine.h"

#include "../../DS_Library/include/dynamicArray.h"

using std::string;

class RetrievalEngine
{
private:

    Indexer& indexer;

    URLNormalizer normalizer;

    Tokenizer tokenizer;

    StopWordRemover remover;

    RankingEngine rankingEngine;


    bool contains(
        DynamicArray<SearchPosting>& results,
        int pageID);


public:

    RetrievalEngine(
        Indexer& indexer);


    DynamicArray<SearchPosting>search(
    const string& query);
};

#endif