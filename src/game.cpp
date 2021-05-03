#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      _grid_width(grid_width), _grid_height(grid_height){
  
  PlaceFood();
  PlaceExtraFood();
}


void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, food_extra);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}
void Game::PlaceExtraFood(){
  int x, y;
  while(true){
    x = random_w(engine);
    y = random_h(engine);
    //check that the location is not occupied by a snake item or by the first food before placing the second food.
    if (!snake.SnakeCell(x,y) && (x!=food.x)&& (y!=food.y))
      {
        food_extra_x = x;
        food_extra_y = y;
        
        food_extra.x = static_cast<int>(food_extra_x);
        food_extra.y = static_cast<int>(food_extra_y);
        return;
  
    }
      }
}
void Game::UpdateExtraFood(){
   
   food_extra_x += food_speed;
    food_extra_y += food_speed;
   food_extra.x = static_cast<int>(food_extra_x);
    food_extra.y = static_cast<int>(food_extra_y);
   food_extra.x = fmod ( food_extra.x + _grid_width, _grid_width);
   food_extra.y = fmod (food_extra.y + _grid_height, _grid_height);
   return;
  
  }
void Game::Update() {
  if (!snake.alive) return;
  UpdateExtraFood();
  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if ((food.x == new_x && food.y == new_y)) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
  
  if (food_extra.x == new_x && food_extra.y == new_y){
    score +=2;
    PlaceExtraFood();
    snake.GrowBody();
    snake.speed += 0.02;
    food_speed +=0.01;
   } 
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }