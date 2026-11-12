#include "../include/RankedPage.h"

RankedPage::RankedPage()
{
    pageID = 0;
    score = 0.0;
}

RankedPage::RankedPage(
    int pageID,
    double score)
{
    this->pageID = pageID;
    this->score = score;
}