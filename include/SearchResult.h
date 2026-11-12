#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include <string>

class SearchResult {
public:
    int pageID;
    std::string url;
    std::string title;
    std::string snippet;
    double score;

    SearchResult()
        : pageID(-1), score(0.0) {}

    SearchResult(int pageID,
                 const std::string& url,
                 const std::string& title,
                 const std::string& snippet,
                 double score)
        : pageID(pageID),
          url(url),
          title(title),
          snippet(snippet),
          score(score) {}
};

#endif