add_library(RouteSearcher
        core/RouteService.cpp
        cli/RouteCLIController.cpp
        api/RouteAPI.cpp
        core/Route.cpp
        core/RouteFactory.cpp
        util/ChronoConversion.cpp)

include(FetchContent)

# nlohmann_json
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)

FetchContent_MakeAvailable(json)

target_link_libraries(RouteSearcher PRIVATE nlohmann_json::nlohmann_json)

# cpr
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG dd967cb48ea6bcbad9f1da5ada0db8ac0d532c06)

FetchContent_MakeAvailable(cpr)

target_link_libraries(RouteSearcher PRIVATE cpr::cpr)
