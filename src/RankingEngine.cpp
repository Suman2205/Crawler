#include "../include/RankingEngine.h"


RankingEngine::RankingEngine()
{
}


DynamicArray<SearchPosting> RankingEngine::rank(
    DynamicArray<SearchPosting>& postings)
{
    DynamicArray<SearchPosting> results;


    // Copy postings
    for(int i = 0; i < postings.getSize(); i++)
    {
        results.push_back(postings[i]);
    }


    sortResults(results);


    return results;
}



void RankingEngine::sortResults(
    DynamicArray<SearchPosting>& results)
{
    for(int i = 0; i < results.getSize(); i++)
    {
        for(int j = i + 1; j < results.getSize(); j++)
        {
            if(results[j].frequency > results[i].frequency)
            {
                SearchPosting temp = results[i];

                results[i] = results[j];

                results[j] = temp;
            }
        }
    }
}