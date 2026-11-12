#include "../include/Indexer.h"
#include "../include/RetrievalEngine.h"
#include "../include/PageStorage.h"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include <iostream>

using json = nlohmann::json;
using namespace httplib;

int main()
{
    //--------------------------------------------------
    // Load Index
    //--------------------------------------------------

    Indexer indexer;
    indexer.buildIndex();

    RetrievalEngine retrieval(indexer);

    PageStorage storage;


    //--------------------------------------------------
    // HTTP Server
    //--------------------------------------------------

    Server server;


    //--------------------------------------------------
    // GET /search?q=query
    //--------------------------------------------------

    server.Get("/search",
    [&](const Request& req, Response& res)
    {
        json response;


        //--------------------------------------------------
        // Missing query
        //--------------------------------------------------

        if(!req.has_param("q"))
        {
            response["success"] = false;
            response["message"] = "Missing query.";

            res.set_content(
                response.dump(),
                "application/json");

            return;
        }


        string query =
            req.get_param_value("q");


        //--------------------------------------------------
        // Retrieve + Rank Results
        //--------------------------------------------------

        DynamicArray<SearchPosting> results =
            retrieval.search(query);


        //--------------------------------------------------
        // Prepare JSON Response
        //--------------------------------------------------

        response["success"] = true;

        response["query"] = query;

        response["count"] =
            results.getSize();


        response["results"] =
            json::array();


        for(int i = 0; i < results.getSize(); i++)
        {
            json item;


            item["pageID"] =
                results[i].pageID;


            item["url"] =
                results[i].url;


            item["frequency"] =
                results[i].frequency;


            response["results"].push_back(item);
        }


        res.set_content(
            response.dump(4),
            "application/json");
    });


    //--------------------------------------------------
    // GET /page?id=pageID
    //--------------------------------------------------

    server.Get("/page",
    [&](const Request& req, Response& res)
    {
        //--------------------------------------------------
        // Missing page id
        //--------------------------------------------------

        if(!req.has_param("id"))
        {
            res.status = 400;

            res.set_content(
                "Missing page id.",
                "text/plain");

            return;
        }


        //--------------------------------------------------
        // Convert id
        //--------------------------------------------------

        int pageID =
            std::stoi(req.get_param_value("id"));


        //--------------------------------------------------
        // Load Stored HTML
        //--------------------------------------------------

        string html =
            storage.getPageByID(pageID);


        //--------------------------------------------------
        // Page not found
        //--------------------------------------------------

        if(html.empty())
        {
            res.status = 404;

            res.set_content(
                "Page not found.",
                "text/plain");

            return;
        }


        //--------------------------------------------------
        // Return HTML
        //--------------------------------------------------

        res.set_content(
            html,
            "text/html");
    });


    //--------------------------------------------------
    // Health Check
    //--------------------------------------------------

    server.Get("/",
    [](const Request&, Response& res)
    {
        res.set_content(
            "Retrieval Engine Running",
            "text/plain");
    });


    std::cout
        << "Search Server running on http://localhost:8080\n";


    server.listen(
        "0.0.0.0",
        8080);


    return 0;
}