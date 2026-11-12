cmake_minimum_required(VERSION 3.20)

project(Fetcher)

# ----------------------------
# C++ Settings
# ----------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ----------------------------
# Debug Build
# ----------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)

include_directories(include)

# ----------------------------
# External Libraries
# ----------------------------
find_package(CURL REQUIRED)
find_package(SQLite3 REQUIRED)

# ----------------------------
# Fetch Content
# ----------------------------

include(FetchContent)

FetchContent_Declare(
    cpp_httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.18.6
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(
    cpp_httplib
    nlohmann_json
)

# ----------------------------
# Fetcher Library
# ----------------------------
add_library(fetcher
    src/Fetcher.cpp
    src/Frontier.cpp
    src/SeenStore.cpp
    src/PageStorage.cpp
    src/LinkExtractor.cpp
    src/URLNormalizer.cpp
    src/crawler.cpp
    src/Database.cpp
    src/HTMLParser.cpp
    src/Tokenizer.cpp
    src/StopWordRemover.cpp
    src/InvertedIndex.cpp
    src/Posting.cpp
    src/PagePosting.cpp
    src/IndexStorage.cpp
    src/PostingRecord.cpp
    src/Indexer.cpp
    src/SearchPosting.cpp

    # Ranking Engine
    src/RankingEngine.cpp
    src/RankedPage.cpp

    ../../DS_Library/src/HashFunction.cpp
)

target_link_libraries(fetcher
    PUBLIC
        CURL::libcurl
        SQLite::SQLite3
)

# ----------------------------
# Main Executable
# ----------------------------
add_executable(testFetcher
    main.cpp
)

target_link_libraries(testFetcher
    PRIVATE
        fetcher
)

# ==========================================================
# Retrieval Engine HTTP Server
# ==========================================================

add_executable(SearchServer
    src/SearchServer.cpp
    src/RetrievalEngine.cpp
)

target_link_libraries(SearchServer
    PRIVATE
        fetcher
        httplib::httplib
        nlohmann_json::nlohmann_json
)

# ==========================================================
# Google Test
# ==========================================================

enable_testing()

find_package(GTest REQUIRED)

add_executable(URLNormalizerTesting
    # testing/URLNormalizerTest.cpp
    # testing/LinkExtractorTest.cpp
    # testing/FetcherTest.cpp
    # testing/FrontierTest.cpp
    # testing/SeenStoreTest.cpp
    # testing/PageStorageTest.cpp
    # testing/CrawlerTest.cpp
    testing/HTMLParserTest.cpp
    testing/TokenizerTest.cpp
    testing/StopWordRemoverTest.cpp
    testing/InvertedIndexTest.cpp
    testing/IndexStorageTest.cpp
    testing/IndexerTest.cpp
)

target_link_libraries(URLNormalizerTesting
    PRIVATE
        fetcher
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(URLNormalizerTesting)