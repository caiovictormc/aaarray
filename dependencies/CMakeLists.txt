set(CPM_DOWNLOAD_VERSION 0.36.0)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

message(STATUS ${CPM_DOWNLOAD_LOCATION})

message(STATUS "CPM.cmake - SETUP")

if (NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "CPM.cmake - Downloading")
    file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
    message(STATUS "CPM.cmake - Done")
else ()
    message(STATUS "CPM.cmake - Done - Using cached CPM")
endif ()

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

    set(SDL2_INCLUDE_DIR ${SDL2_SOURCE_DIR}/include)
endif ()

include_directories(${SDL2_INCLUDE_DIR})
string(TIMESTAMP AFTER "%s")
math(EXPR DELTASDL "${AFTER} - ${BEFORE}")
MESSAGE(STATUS "SDL2 TIME: ${DELTASDL}s")

#ImGui
string(TIMESTAMP BEFORE "%s")
CPMAddPackage(
        NAME IMGUI
        URL "https://github.com/ocornut/imgui/archive/refs/tags/v1.89.2.zip"
)

IF (IMGUI_ADDED)
    add_library(IMGUI STATIC)

    target_sources(IMGUI
            PRIVATE
            ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
            ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
            ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
            ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
            ${IMGUI_SOURCE_DIR}/imgui.cpp

            PRIVATE
            ${IMGUI_SOURCE_DIR}/backends/imgui_impl_sdlrenderer.cpp
            ${IMGUI_SOURCE_DIR}/backends/imgui_impl_sdl.cpp
            ${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
            )
    target_include_directories(IMGUI
            PUBLIC ${IMGUI_SOURCE_DIR}
            PUBLIC ${IMGUI_SOURCE_DIR}/backends
            )
    find_package(OpenGL REQUIRED)

    target_link_libraries(IMGUI PUBLIC ${OPENGL_gl_LIBRARY} SDL2 ${CMAKE_DL_LIBS})
ENDIF ()

include_directories(${IMGUI_SOURCE_DIR} ${IMGUI_SOURCE_DIR}/backends)
string(TIMESTAMP AFTER "%s")
math(EXPR DELTAIMGUI "${AFTER} - ${BEFORE}")
MESSAGE(STATUS "IMGUI TIME: ${DELTAIMGUI}s")
