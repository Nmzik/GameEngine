#include "Game.h"

Game::Game() {

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

	window = SDL_CreateWindow(
		"SDL2 window",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		1280,                               // width, in pixels
		720,                               // height, in pixels
		SDL_WINDOW_OPENGL  //| SDL_WINDOW_FULLSCREEN                 // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return;
	}

	SDL_SetRelativeMouseMode(SDL_TRUE);

	rendering_system = std::make_unique<RenderingSystem>(window);
	gameWorld = std::make_unique<GameWorld>();
	input = new InputManager();

	StateManager::get().enter<InGameState>(this);
}


Game::~Game() {
	// Close and destroy the window
	//SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::updateFPS(float delta_time) {
	const auto smooth_factor = 0.1f;
	static auto delta_time_smoothed = 0.f;
	static auto time_since_last_fps_output = 0.f;

	delta_time_smoothed = (1.0f - smooth_factor) * delta_time_smoothed + smooth_factor * delta_time;

	time_since_last_fps_output += delta_time;
	if (time_since_last_fps_output >= 1.0f) {
		time_since_last_fps_output = 0.0f;
		std::ostringstream osstr;
		osstr << "SDL2 window" << " (" << (int((1.0f / delta_time_smoothed) * 10.0f) / 10.0f) << " FPS, ";
		osstr << (int(delta_time_smoothed * 10000.0f) / 10.0f) << " ms / frame) | ";
		SDL_SetWindowTitle(window, osstr.str().c_str());
	}
}

void Game::run() {

	SDL_Event event;

	bool running = true;
	auto current_time = std::chrono::steady_clock::now();
	//float current_time = 0.f;
	//float old_time;
	
	while (running) {
		while (SDL_PollEvent(&event)) {
			StateManager::get().currentState()->handleEvent(event);
			if (event.type == SDL_QUIT) running = false;
		}

		auto old_time = current_time;
		current_time = std::chrono::steady_clock::now();
		float delta_time = std::chrono::duration<float>(current_time - old_time).count();

		input->Update();
		if (!paused) {
			gameWorld->update(delta_time);
			StateManager::get().tick(delta_time);
			gameWorld->UpdateTraffic(&rendering_system->getCamera());
			rendering_system->render(gameWorld.get());
		}

		updateFPS(delta_time);
	}

	//if (event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT) break;
}