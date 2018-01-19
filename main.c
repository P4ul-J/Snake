#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdlib.h>

#define WINDOW_HEIGHT (480)
#define WINDOW_WIDTH (640)
// speed in pixel per second
#define SPEED (300)
#define COLLECTVALUE (32)
#define LEFT  1
#define UP    2
#define DOWN  3
#define RIGHT 4
#define QUIT  1337
#define PAUSE 155

int dir;
int dir_old;
int input (void);

int main(int argc, char* argv[]) {

  /* BEGINNING OF SDL INIT */
  // initialize SDLs video and timer sdubsystem
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
    printf("Fehler beim Initialisieren von SDL: %s\n", SDL_GetError());
    return 1;
  }

  // creating a window
  SDL_Window *win = SDL_CreateWindow("Snake - Score: 0", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if (win == NULL) {
    printf("Fenster konnte nicht erstellt werden: %s\n", SDL_GetError());
    return 1;
  }

  // creating a renderer which will draw to the assigned window
  uint32_t render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
  if (rend == NULL) {
    printf("Renderer konnte nicht erstellt werden: %s\n", SDL_GetError());
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

  // load image into memory
  // SDL_Surface* is created which we can then upload to a SDL_Texture
  SDL_Surface* surf_sheet = IMG_Load("resources/snake-sheet.png");
  if (surf_sheet  == NULL) {
    printf("Bild konnte nicht in den Speicher geladen werden: %s\n", SDL_GetError());
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

  // create texture from surface which the renderer can draw to the window
  SDL_Texture* tex_sheet = SDL_CreateTextureFromSurface(rend, surf_sheet);
  if (tex_sheet == NULL) {
    printf("Textur konnte nicht erstellt werden: %s\n", SDL_GetError());
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }
  // we can free the surface afterwards
  SDL_FreeSurface(surf_sheet);


  // structs for coordinates and dimensions of certain elements
  SDL_Rect head;
  SDL_Rect fruit;

  // set and scale  dimensions of these elements
  fruit.w = 64 / 2;
  fruit.h = 64 / 2;
  head.w = 64 / 2;
  head.h = 64 / 2;


  // declare and initialize array of structs for storing clipping dimensions
  // 13 more have to be added!
  // set the coordinates regarding to the position in snake-sheet.png
  SDL_Rect sprites[15];
  // height and width of texture clippets from sprite sheet
  int W_clip = 64, H_clip = 64;
  // snake head right
  sprites[0].x = 4 * W_clip;
  sprites[0].y = 0;
  sprites[0].w = W_clip;
  sprites[0].h = H_clip;

  // fruit
  sprites[1].x = 0;
  sprites[1].y = 3 * H_clip;
  sprites[1].w = W_clip;
  sprites[1].h = H_clip;
  /* END OF SDL INIT  */

  /* BEGINNING OF GAME INIT */
  // Koordinatenursprung ist oben links, positive y-Achse zeigt nach unten
  head.x = (WINDOW_WIDTH - head.w) / 2;
  head.y = (WINDOW_HEIGHT - head.h) / 2;

  fruit.x = (rand()  % (WINDOW_WIDTH - 2 * fruit.w)) + fruit.w;
  fruit.y = (rand()  % (WINDOW_HEIGHT - 2 * fruit.h)) + fruit.h;



  //Bewegungsrichtung
  float x_vel = 0;
  float y_vel = 0;

  int score = 0;
  char scorestr[20];

  dir_old = 0;
  x_vel = y_vel = 0;



  srand(time(NULL));
  /* END OF GAME INIT */

  /* BEGINNING OF GAME LOOP */
  while (input() != QUIT) {

    // process events
    dir = input();
    // determine velocity

        if (dir == UP) {
          y_vel = -SPEED;
          x_vel = 0;
        }
        if (dir == DOWN) {
          y_vel = SPEED;
          x_vel = 0;
        }
        if (dir == LEFT) {
          x_vel = -SPEED;
          y_vel = 0;
        }
        if (dir == RIGHT) {
          x_vel = SPEED;
          y_vel = 0;
        }


      // update positions
      head.x += x_vel / 60;
      head.y += y_vel / 60;

      // detecting of snake eating fruit and random spawning of new fruit
      if (head.x < fruit.x + COLLECTVALUE && head.x > fruit.x - COLLECTVALUE
          && head.y < fruit.y + COLLECTVALUE && head.y > fruit.y - COLLECTVALUE) {

            fruit.x = (rand()  % (WINDOW_WIDTH - 2 * fruit.w)) + fruit.w;
            fruit.y = (rand()  % (WINDOW_HEIGHT - 2 * fruit.h)) + fruit.h;
            score += 10;
      }


      // collision detection with bounds and "wrap around"
      if (head.x < 0) head.x = WINDOW_WIDTH - head.w;
      if (head.y < 0) head.y = WINDOW_HEIGHT - head.h;
      if (head.x > WINDOW_WIDTH - head.w) head.x = 0;
      if (head.y > WINDOW_HEIGHT - head.h) head.y = 0;

      // clear the window / renderer
      SDL_RenderClear(rend);


      // draw fruit
      SDL_RenderCopy(rend, tex_sheet, &sprites[1], &fruit);
      //draw head of snake
      SDL_RenderCopy(rend, tex_sheet, &sprites[0], &head);

      // swap current visible rendered window with buffered
      // (double buffering)
      SDL_RenderPresent(rend);

      // store integer "score" within string "scorestr"
      sprintf(scorestr, "Snake - Score: %d", score);
      // dynamically change window title in order to display current highscore
      SDL_SetWindowTitle(win, scorestr);

      // wait 1/60th of a second
      SDL_Delay(1000/60);
    } /* END OF GAME LOOP */



    // cleanup ressources
    SDL_DestroyTexture(tex_sheet);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;

} // end of main()


int input (void) {
  int input;
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        input = QUIT;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.scancode) {
          case SDL_SCANCODE_W:
          case SDL_SCANCODE_UP:
            input = UP;
            break;
          case SDL_SCANCODE_A:
          case SDL_SCANCODE_LEFT:
            input = LEFT;
            break;
          case SDL_SCANCODE_S:
          case SDL_SCANCODE_DOWN:
            input = DOWN;
            break;
          case SDL_SCANCODE_D:
          case SDL_SCANCODE_RIGHT:
            input = RIGHT;
            break;
          case SDL_SCANCODE_SPACE:
          input = PAUSE;
        }
        break;
      }
    }
  return input;
}
