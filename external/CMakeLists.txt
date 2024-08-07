

if(USE_EXTERNAL_JSON)
    message("Using installed Version of nlohmann_json")
    find_package(nlohmann_json 3.2.0 REQUIRED)
else()
    message("Using local install of nlohmann_json")
    set(JSON_BuildTests OFF CACHE INTERNAL "")
    add_subdirectory(nlohmann_json)
endif()