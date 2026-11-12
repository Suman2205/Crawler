#include "../include/RetrievalEngine.h"


RetrievalEngine::RetrievalEngine(
    Indexer& indexer)
    : indexer(indexer)
{
}


bool RetrievalEngine::contains(
    DynamicArray<SearchPosting>& results,
    int pageID)
{
    for(int i = 0; i < results.getSize(); i++)
    {
        if(results[i].pageID == pageID)
        {
            return true;
        }
    }

    return false;
}


DynamicArray<SearchPosting> RetrievalEngine::search(
    const string& query)
{
    //--------------------------------------------------
    // Convert query into DynamicArray<string>
    //--------------------------------------------------

    DynamicArray<string> queryText;

    queryText.push_back(query);


    //--------------------------------------------------
    // Tokenize query
    //--------------------------------------------------

    DynamicArray<string> tokens =
        tokenizer.tokenize(queryText);


    //--------------------------------------------------
    // Remove stop words
    //--------------------------------------------------

    DynamicArray<string> words =
        remover.removeStopWords(tokens);


    //--------------------------------------------------
    // Search every word
    //--------------------------------------------------

    DynamicArray<SearchPosting> results;


    for(int i = 0; i < words.getSize(); i++)
    {
        DynamicArray<SearchPosting> postings =
            indexer.search(words[i]);


        for(int j = 0; j < postings.getSize(); j++)
        {
            bool found = false;

            for(int k = 0; k < results.getSize(); k++)
            {
                if(results[k].pageID == postings[j].pageID)
                {
                    //--------------------------------------------------
                    // Same page matched another query word.
                    // Increase its total frequency.
                    //--------------------------------------------------

                    results[k].frequency += postings[j].frequency;

                    found = true;

                    break;
                }
            }

            if(!found)
            {
                results.push_back(postings[j]);
            }
        }
    }


    //--------------------------------------------------
    // Rank search results
    //--------------------------------------------------

    return rankingEngine.rank(results);
}