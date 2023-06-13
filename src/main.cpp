#define SDL_MAIN_HANDLED true

#include <iostream>
#include <algorithm>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include <SDL.h>

#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600

int main(int argc, char *argv[]) {
    // Unused argc, argv
    (void) argc;
    (void) argv;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized!" << std::endl
                  << "SDL_Error: " << SDL_GetError() << std::endl;
        return 0;
    }

#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
    // Disable compositor bypass
    if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"))
    {
        std::cout << "SDL can not disable compositor bypass!" << std::endl;
        return 0;
    }
#endif

    // Create window
    SDL_Window *window = SDL_CreateWindow("Basic C++ SDL project",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Window could not be created!" << std::endl
                  << "SDL_Error: " << SDL_GetError() << std::endl;
    } else {
        // Create renderer
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            std::cout << "Renderer could not be created!" << std::endl
                      << "SDL_Error: " << SDL_GetError() << std::endl;
        } else {

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            (void) io;
            ImGui::StyleColorsDark();

            ImGui_ImplSDL2_InitForOpenGL(window, renderer);
            ImGui_ImplSDLRenderer_Init(renderer);

            // Declare rect of square
            SDL_Rect squareRect;

            // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
            squareRect.w = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
            squareRect.h = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;

            // Square position: In the middle of the screen
            squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
            squareRect.y = SCREEN_HEIGHT / 2 - squareRect.h / 2;

            ImVec4 my_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            // Event loop exit flag
            bool quit = false;

            // Event loop
            while (!quit) {
                SDL_Event e;

                // Wait indefinitely for the next available event
                while (SDL_PollEvent(&e)) {
                    ImGui_ImplSDL2_ProcessEvent(&e);
                    if (e.type == SDL_QUIT)
                        quit = true;
                    if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE &&
                        e.window.windowID == SDL_GetWindowID(window))
                        quit = true;
                }

                // User requests quit
                if (e.type == SDL_QUIT) {
                    quit = true;
                }

                // Initialize renderer color white for the background
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Clear screen
                SDL_RenderClear(renderer);

                ImGui_ImplSDLRenderer_NewFrame();
                ImGui_ImplSDL2_NewFrame();
                ImGui::NewFrame();

                // ImGui Code
                ImGui::Begin("Hello, world!");
                ImGui::Text("Hi! :D");
                ImGui::ColorEdit4("Color", (float *) &my_color);

                float position_x = squareRect.x;
                float position_y = squareRect.y;
                ImGui::SliderFloat("X Position", &position_x, 0.0f, (float) SCREEN_WIDTH);
                ImGui::SliderFloat("Y Position", &position_y, 0.0f, (float) SCREEN_HEIGHT);

                ImGui::End();
                ImGui::Render();
                ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

                squareRect.x = position_x;
                squareRect.y = position_y;

                // Set renderer color red to draw the square
                SDL_SetRenderDrawColor(
                        renderer,
                        (Uint8) (my_color.x * 255),
                        (Uint8) (my_color.y * 255),
                        (Uint8) (my_color.z * 255),
                        (Uint8) (my_color.w * 255)
                );
                // Draw filled square
                SDL_RenderFillRect(renderer, &squareRect);

                // Update screen
                SDL_RenderPresent(renderer);
            }

            // Destroy renderer
            SDL_DestroyRenderer(renderer);
        }

        // Destroy window
        ImGui_ImplSDL2_Shutdown();
        ImGui_ImplSDLRenderer_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyWindow(window);
    }

    // Quit SDL
    SDL_Quit();

    return 0;
}