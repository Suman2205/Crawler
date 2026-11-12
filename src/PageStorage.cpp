#include "../include/PageStorage.h"
#include "../include/URLNormalizer.h"
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;

PageStorage::PageStorage(): storageDirectory("../CrawlerStorage"){
    createStorageDirectory();
    database.open(buildFilePath("crawler.db"));
    database.createURLTable();
}

void PageStorage::createStorageDirectory()
{
    if(!fs::exists(storageDirectory))
    {
        fs::create_directories(storageDirectory);
    }
}

string PageStorage::buildFilePath(
    const string& fileName) const
{
    return storageDirectory + "/" + fileName;
}

void PageStorage::recoverCrawlerState(
    Frontier& frontier,
    SeenStore& seenStore)
{
    DynamicArray<URLDepth> pendingURLs =
        database.loadPendingURLs();

    for(int i = 0; i < pendingURLs.getSize(); i++)
    {
        frontier.push(pendingURLs[i]);
    }

    DynamicArray<string> completedURLs =
        database.loadCompletedURLs();

    for(int i = 0; i < completedURLs.getSize(); i++)
    {
        seenStore.insert(completedURLs[i]);
    }
}

bool PageStorage::addPendingURL(const string& url,int depth)
{
    return database.insertPendingURL(
        url,
        depth
    );
}

bool PageStorage::storePage(
    const string& url,
    const string& html,
    int depth)
{
    int fileId =
        database.getNextFileId();

    if(fileId == -1)
    {
        return false;
    }

    string fileName =
        to_string(fileId) + ".page";

    ofstream pageFile(
        buildFilePath(fileName));

    if(!pageFile)
    {
        return false;
    }

    // Line 1 : URL
    pageFile << url << '\n';

    // Line 2 : Depth
    pageFile << depth << '\n';

    // Line 3 onwards : Raw HTML
    pageFile << html;

    pageFile.close();

    if(!database.markPageStored(
            url,
            fileId,
            fileName))
    {
        fs::remove(
            buildFilePath(fileName));

        return false;
    }

    return true;
}

// string PageStorage::getPage(
//     const string& url)
// {
//     string fileName =
//         database.getFileName(url);

//     if(fileName.empty())
//     {
//         return "";
//     }

//     ifstream pageFile(
//         buildFilePath(fileName));

//     if(!pageFile)
//     {
//         return "";
//     }

//     string line;

//     // Skip URL
//     getline(pageFile, line);

//     // Skip Depth
//     getline(pageFile, line);

//     stringstream buffer;
//     buffer << pageFile.rdbuf();

//     return buffer.str();
// }

// string PageStorage::getPage(
//     const string& url)
// {
//     string fileName =
//         database.getFileName(url);

//     if(fileName.empty())
//     {
//         return "";
//     }

//     ifstream pageFile(
//         buildFilePath(fileName));

//     if(!pageFile)
//     {
//         return "";
//     }

//     string line;

//     // Skip URL
//     getline(pageFile, line);

//     // Skip Depth
//     getline(pageFile, line);

//     stringstream buffer;
//     buffer << pageFile.rdbuf();

//     string html = buffer.str();

//     //--------------------------------------------------
//     // Inject <base> tag for proper resource loading
//     //--------------------------------------------------

//     size_t headPos = html.find("<head");

//     if(headPos != string::npos)
//     {
//         size_t endHeadTag =
//             html.find('>', headPos);

//         if(endHeadTag != string::npos)
//         {
//             string baseTag =
//                 "\n<base href=\"" +
//                 url +
//                 "/\">\n";

//             html.insert(
//                 endHeadTag + 1,
//                 baseTag
//             );
//         }
//     }

//     return html;
// }


string PageStorage::getPage(const string& url)
{
    string fileName = database.getFileName(url);

    if(fileName.empty())
    {
        return "";
    }

    ifstream pageFile(buildFilePath(fileName));

    if(!pageFile)
    {
        return "";
    }

    string line;

    // Skip URL
    getline(pageFile, line);

    // Skip Depth
    getline(pageFile, line);

    stringstream buffer;
    buffer << pageFile.rdbuf();

    string html = buffer.str();

    //--------------------------------------------------
    // Compute proper base URL
    //--------------------------------------------------

    URLNormalizer normalizer;

    string baseURL = normalizer.getDirectory(url);

    //--------------------------------------------------
    // If URL itself is a directory (ends with '/')
    //--------------------------------------------------

    if(!url.empty() && url.back() == '/')
    {
        baseURL = url;
    }

    //--------------------------------------------------
    // Inject <base>
    //--------------------------------------------------

    size_t headPos = html.find("<head");

    if(headPos != string::npos)
    {
        size_t endHeadTag = html.find('>', headPos);

        if(endHeadTag != string::npos)
        {
            string baseTag =
                "\n<base href=\"" +
                baseURL +
                "\">\n";

            html.insert(endHeadTag + 1, baseTag);
        }
    }

    return html;
}


// string PageStorage::getPage(
//     const string& url)
// {
//     string fileName =
//         database.getFileName(url);

//     if(fileName.empty())
//     {
//         return "";
//     }

//     ifstream pageFile(
//         buildFilePath(fileName));

//     if(!pageFile)
//     {
//         return "";
//     }

//     //--------------------------------------------------
//     // Skip metadata
//     //--------------------------------------------------

//     string line;

//     getline(pageFile, line);   // URL
//     getline(pageFile, line);   // Depth

//     //--------------------------------------------------
//     // Read HTML
//     //--------------------------------------------------

//     stringstream buffer;
//     buffer << pageFile.rdbuf();

//     string html = buffer.str();

//     //--------------------------------------------------
//     // Build correct base directory
//     //--------------------------------------------------

//     URLNormalizer normalizer;

//     string baseURL =
//         normalizer.getDirectory(url);

//     //--------------------------------------------------
//     // Inject <base>
//     //--------------------------------------------------

//     size_t headPos =
//         html.find("<head");

//     if(headPos != string::npos)
//     {
//         size_t endHead =
//             html.find('>', headPos);

//         if(endHead != string::npos)
//         {
//             string baseTag =
//                 "\n<base href=\"" +
//                 baseURL +
//                 "\">\n";

//             html.insert(
//                 endHead + 1,
//                 baseTag);
//         }
//     }

//     //--------------------------------------------------
//     // Rewrite resource attributes
//     //--------------------------------------------------

//     auto rewriteAttribute =
//         [&](const string& attribute)
//     {
//         size_t pos = 0;

//         while(true)
//         {
//             pos = html.find(attribute + "=\"", pos);

//             if(pos == string::npos)
//                 break;

//             size_t start =
//                 pos + attribute.length() + 2;

//             size_t end =
//                 html.find('"', start);

//             if(end == string::npos)
//                 break;

//             string value =
//                 html.substr(start,
//                     end - start);

//             //--------------------------------------------------
//             // Ignore already absolute data/javascript
//             //--------------------------------------------------

//             if(value.empty() ||
//                value.find("data:") == 0 ||
//                value.find("javascript:") == 0 ||
//                value.find("#") == 0)
//             {
//                 pos = end;
//                 continue;
//             }

//             //--------------------------------------------------
//             // Convert to absolute URL
//             //--------------------------------------------------

//             string absolute =
//                 normalizer.normalize(
//                     url,
//                     value);

//             //--------------------------------------------------
//             // Replace with proxy endpoint
//             //--------------------------------------------------

//             string replacement =
//                 "/page?url=" + absolute;

//             html.replace(
//                 start,
//                 end - start,
//                 replacement);

//             pos =
//                 start + replacement.length();
//         }
//     };

//     rewriteAttribute("href");
//     rewriteAttribute("src");
//     rewriteAttribute("action");
//     rewriteAttribute("poster");

//     return html;
// }

bool PageStorage::hasPage(
    const string& url)
{
    string fileName =
        database.getFileName(url);

    return !fileName.empty()&& fs::exists(buildFilePath(fileName));
}

int PageStorage::pageCount()
{
    return database.getStoredPageCount();
}

string PageStorage::getURLByID(int id){
    return database.getURLByID(id);
}

string PageStorage::getPageByID(int pageID)
{
    string url = getURLByID(pageID);

    if(url.empty())
    {
        return "";
    }

    return getPage(url);
}