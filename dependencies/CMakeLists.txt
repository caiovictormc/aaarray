set(CPM_DOWNLOAD_VERSION 0.36.0)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

message(STATUS ${CPM_DOWNLOAD_LOCATION})

message(STATUS "CPM.cmake - SETUP")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "CPM.cmake - Downloading")
    file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
    message(STATUS "CPM.cmake - Done")
else()
    message(STATUS "CPM.cmake - Done - Using cached CPM")
endif()

include(${CPM_DOWNLOAD_LOCATION})

# SDL
string(TIMESTAMP BEFORE "%s")
CPMAddPackage(
    NAME SDL2
    GITHUB_REPOSITORY libsdl-org/SDL
    GIT_TAG release-2.26.0
    OPTIONS "SDL2_DISABLE_INSTALL TRUE"
)

find_package(SDL2 REQUIRED)

if (SDL2_ADDED)
    file(GLOB SDL_HEADERS "${SDL_SOURCE_DIR}/include/*.h")

    # Create a target that copies headers at build time, when they change
    add_custom_target(sdl_copy_headers_in_build_dir
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${SDL_SOURCE_DIR}/include" "${CMAKE_BINARY_DIR}/SDLHeaders/SDL2"
            DEPENDS ${SDL_HEADERS})

    # Make SDL depend from it
    add_dependencies(SDL2 sdl_copy_headers_in_build_dir)

    # And add the directory where headers have been copied as an interface include dir
    target_include_directories(SDL2 INTERFACE "${CMAKE_BINARY_DIR}/SDLHeaders")

    set (SDL2_INCLUDE_DIR ${SDL2_SOURCE_DIR}/include)
endif()

include_directories(${SDL2_INCLUDE_DIR})
string(TIMESTAMP AFTER "%s")
math(EXPR DELTASDL "${AFTER} - ${BEFORE}")
MESSAGE(STATUS "SDL2 TIME: ${DELTASDL}s")
