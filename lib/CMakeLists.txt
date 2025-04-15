add_library(WaySearcher WaySearcher.cpp)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)

FetchContent_MakeAvailable(json)

target_link_libraries(WaySearcher PRIVATE nlohmann_json::nlohmann_json)
