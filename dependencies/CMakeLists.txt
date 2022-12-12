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

# SDL_ttf
string(TIMESTAMP BEFORE "%s")
CPMAddPackage(GITHUB_REPOSITORY libsdl-org/SDL_ttf
        GIT_TAG main
        OPTIONS "SDL2TTF_INSTALL FALSE" "SDL2TTF_VENDORED TRUE") # vendor is required for mingw builds
find_package(SDL_ttf REQUIRED)
string(TIMESTAMP AFTER "%s")
math(EXPR DELTASDL_ttf "${AFTER} - ${BEFORE}")
MESSAGE(STATUS "SDL_ttf TIME: ${DELTASDL_ttf}s")

## SDL_image
string(TIMESTAMP BEFORE "%s")
CPMAddPackage(
    GITHUB_REPOSITORY libsdl-org/SDL_image
    GIT_TAG main)
find_package(SDL_image REQUIRED)
string(TIMESTAMP AFTER "%s")
math(EXPR DELTASDL_image "${AFTER} - ${BEFORE}")
MESSAGE(STATUS "SDL_image TIME: ${DELTASDL_image}s")

## SDL_mixer
string(TIMESTAMP BEFORE "%s")
CPMAddPackage(GITHUB_REPOSITORY libsdl-org/SDL_mixer
        GIT_TAG main
        OPTIONS "SDL2IMIXER_INSTALL FALSE" "SDL2MIXER_VENDORED TRUE") # vendor is required for mingw builds
find_package(SDL_mixer REQUIRED)
string(TIMESTAMP AFTER "%s")
math(EXPR DELTASDL_mixer "${AFTER} - ${BEFORE}")
MESSAGE(STATUS "SDL_mixer TIME: ${DELTASDL_mixer}s")
