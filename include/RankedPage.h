#ifndef RANKED_PAGE_H
#define RANKED_PAGE_H

class RankedPage
{
public:

    int pageID;

    double score;

    RankedPage();

    RankedPage(
        int pageID,
        double score
    );
};

#endif