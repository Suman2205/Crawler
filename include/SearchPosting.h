#ifndef SEARCH_POSTING_H
#define SEARCH_POSTING_H

#include <string>

using std::string;

class SearchPosting
{
public:
    int pageID;
    int frequency;
    string url;

    SearchPosting();

    SearchPosting(
        int pageID,
        int frequency,
        const string& url
    );
};

#endif