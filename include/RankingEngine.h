#ifndef RANKING_ENGINE_H
#define RANKING_ENGINE_H

#include "../../DS_Library/include/dynamicArray.h"

#include "SearchPosting.h"

class RankingEngine
{
public:

    RankingEngine();

    DynamicArray<SearchPosting> rank(DynamicArray<SearchPosting>& postings);

private:

    void sortResults(
        DynamicArray<SearchPosting>& results);
};

#endif