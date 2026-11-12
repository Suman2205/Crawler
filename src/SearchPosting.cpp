// ================================================================
// FILE: SearchPosting.cpp
// ================================================================

#include "../include/SearchPosting.h"

SearchPosting::SearchPosting()
{
    pageID = 0;
    frequency = 0;
    url = "";
}

SearchPosting::SearchPosting(
    int pageID,
    int frequency,
    const string& url)
{
    this->pageID = pageID;
    this->frequency = frequency;
    this->url = url;
}