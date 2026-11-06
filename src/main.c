#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

const int WIDTH = 800, HEIGHT = 800;
const int CELL_WIDTH = 20;
const int COLUMNS = WIDTH / CELL_WIDTH;
const int ROWS = HEIGHT / CELL_WIDTH;
const Uint64 STEP_MS = 100;
const Uint32 GRAY = 0x2f2f2f2f;
const Uint32 WHITE = 0xffffffff;
const Uint32 BLACK = 0x00000000;
void draw_grid(SDL_Surface *surface, Uint32 color);
void draw_cell(SDL_Surface *surface, int cell_x, int cell_y, Uint32 color);
void populate(SDL_Surface *surface, bool game_matrix[ROWS][COLUMNS]);
void init_game(bool game_matrix[ROWS][COLUMNS], int probability);
void update_state(bool game_matrix[ROWS][COLUMNS]);
int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Window", WIDTH, HEIGHT, 0);
  SDL_Surface *window_surface = SDL_GetWindowSurface(window);
  bool game_matrix[ROWS][COLUMNS];
  memset(game_matrix, false, sizeof(game_matrix));
  int probability = 0;
  if (argc > 1) {
    probability = atoi(argv[1]);
    init_game(game_matrix, probability);
    populate(window_surface, game_matrix);
  }
  draw_grid(window_surface, GRAY);
  SDL_UpdateWindowSurface(window);
  bool start = false;
  bool painting = false;
  int last_cx = -1, last_cy = -1;
  while (!start) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {

      case SDL_EVENT_QUIT:
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;

      case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (event.button.button == SDL_BUTTON_LEFT) {
          painting = true;
          int cx = (int)(event.button.y / CELL_WIDTH);
          int cy = (int)(event.button.x / CELL_WIDTH);
          if (cx >= 0 && cx < COLUMNS && cy >= 0 && cy < ROWS) {
            game_matrix[cy][cx] = !game_matrix[cy][cx];
            last_cx = cx;
            last_cy = cy;
            populate(window_surface, game_matrix);
            SDL_UpdateWindowSurface(window);
          }
        }
        break;

      case SDL_EVENT_MOUSE_BUTTON_UP:
        if (event.button.button == SDL_BUTTON_LEFT) {
          painting = false;
          last_cx = last_cy = -1;
        }
        break;

      case SDL_EVENT_MOUSE_MOTION:
        if (painting) {
          int cx = (int)(event.motion.y / CELL_WIDTH);
          int cy = (int)(event.motion.x / CELL_WIDTH);
          if (cx >= 0 && cx < COLUMNS && cy >= 0 && cy < ROWS &&
              (cx != last_cx || cy != last_cy)) {
            game_matrix[cy][cx] = !game_matrix[cy][cx];
            last_cx = cx;
            last_cy = cy;
            populate(window_surface, game_matrix);
            SDL_UpdateWindowSurface(window);
          }
        }
        break;

      case SDL_EVENT_KEY_DOWN:
        if (event.key.key == SDLK_RETURN || event.key.key == SDLK_RETURN2)
          start = true;
        break;
      }
    }
  }

  Uint64 last_step = SDL_GetTicks();
  bool running = true;

  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }

    Uint64 now = SDL_GetTicks();
    if (now - last_step >= STEP_MS) {
      update_state(game_matrix);
      last_step = now;
    }

    populate(window_surface, game_matrix);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(16);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
void draw_grid(SDL_Surface *surface, Uint32 color) {
  for (int i = 0; i < ROWS; i++) {
    SDL_Rect line = (SDL_Rect){0, i * CELL_WIDTH, WIDTH, 1};
    SDL_FillSurfaceRect(surface, &line, color);
  }
  for (int i = 0; i < COLUMNS; i++) {
    SDL_Rect line = (SDL_Rect){i * CELL_WIDTH, 0, 1, HEIGHT};
    SDL_FillSurfaceRect(surface, &line, color);
  }
}
void draw_cell(SDL_Surface *surface, int cell_x, int cell_y, Uint32 color) {
  int pixel_x = cell_x * CELL_WIDTH + 1, pixel_y = cell_y * CELL_WIDTH + 1;
  SDL_Rect rect = (SDL_Rect){pixel_x, pixel_y, CELL_WIDTH - 1, CELL_WIDTH - 1};
  SDL_FillSurfaceRect(surface, &rect, color);
}
void populate(SDL_Surface *surface, bool game_matrix[ROWS][COLUMNS]) {
  for (int row = 0; row < ROWS; row++) {
    for (int column = 0; column < COLUMNS; column++) {
      if (game_matrix[row][column]) {
        draw_cell(surface, row, column, WHITE);
      } else {
        draw_cell(surface, row, column, BLACK);
      }
    }
  }
}
void init_game(bool game_matrix[ROWS][COLUMNS], int probability) {
  for (int row = 0; row < ROWS; row++) {
    for (int column = 0; column < COLUMNS; column++) {
      bool fill = rand() % 100 < probability;
      game_matrix[row][column] = fill;
    }
  }
}
void update_state(bool game_matrix[ROWS][COLUMNS]) {
  bool new_state[ROWS][COLUMNS];
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLUMNS; col++) {
      int neighbours = 0;
      // Check Up
      if (row > 0) {
        neighbours += game_matrix[row - 1][col] ? 1 : 0;
      }
      // Check Up Right
      if (row > 0 && col < COLUMNS - 1) {
        neighbours += game_matrix[row - 1][col + 1] ? 1 : 0;
      }
      // Check Right
      if (col < COLUMNS - 1) {
        neighbours += game_matrix[row][col + 1] ? 1 : 0;
      }
      // Check Down Right
      if (row < ROWS - 1 && col < COLUMNS - 1) {
        neighbours += game_matrix[row + 1][col + 1] ? 1 : 0;
      }
      // Check Down
      if (row < ROWS - 1) {
        neighbours += game_matrix[row + 1][col] ? 1 : 0;
      }
      // Check Down Left
      if (row < ROWS - 1 && col > 0) {
        neighbours += game_matrix[row + 1][col - 1] ? 1 : 0;
      }
      // Check Left
      if (col > 0) {
        neighbours += game_matrix[row][col - 1] ? 1 : 0;
      }
      // Check Up Left
      if (row > 0 && col > 0) {
        neighbours += game_matrix[row - 1][col - 1] ? 1 : 0;
      }

      bool alive = game_matrix[row][col];
      new_state[row][col] =
          alive ? (neighbours == 2 || neighbours == 3) : (neighbours == 3);
    }
  }
  memcpy(game_matrix, new_state, sizeof(new_state));
}