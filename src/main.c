#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */

#include <math.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

struct Luminol_AppState {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#define UNUSED(x) (void)(x)

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    UNUSED(argc);
    UNUSED(argv);

    SDL_SetAppMetadata("Luminol_SDL", "1.0", "com.luminol.luminol_sdl");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    *appstate = calloc(1, sizeof(struct Luminol_AppState));

    if (*appstate == nullptr) {
        SDL_Log("Couldn't allocate memory for appstate: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    const auto luminol_appstate = (struct Luminol_AppState*)*appstate;

    if (!SDL_CreateWindowAndRenderer(
            "Luminol_SDL",
            640,
            480,
            0,
            &luminol_appstate->window,
            &luminol_appstate->renderer
        )) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    UNUSED(appstate);

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    const auto elapsed_time_ms = SDL_GetTicks() / 1000.f;
    const auto red = (uint8_t)(sin(elapsed_time_ms) * 127 + 128);
    const auto green = (uint8_t)(sin(elapsed_time_ms + 2) * 127 + 128);
    const auto blue = (uint8_t)(sin(elapsed_time_ms + 4) * 127 + 128);

    const auto luminol_appstate = (const struct Luminol_AppState*)appstate;

    SDL_SetRenderDrawColor(luminol_appstate->renderer, red, green, blue, 255);
    SDL_RenderClear(luminol_appstate->renderer);
    SDL_RenderPresent(luminol_appstate->renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    UNUSED(result);

    free(appstate);
}
