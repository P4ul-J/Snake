#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdlib.h>


#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
// Geschwindigkeit in Pixel pro Sekunde
#define SPEED (200)
#define COLLECTVALUE (10)

int main(int argc, char* argv[]) {

// Grafiksystem initialiseren
if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
  printf("Fehler beim Initialisieren von SDL: %s\n", SDL_GetError());
  return 1;
}

printf("Initialiserung erfolgreich!\n");

//Fenster erstellen
SDL_Window *win = SDL_CreateWindow("Snake", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

if (win == NULL) {
  printf("Fenster konnte nicht erstellt werden: %s\n", SDL_GetError());
  return 1;
}

// Renderer erstellen
uint32_t render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

if (rend == NULL) {
  printf("Renderer konnte nicht erstellt werden: %s\n", SDL_GetError());
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 1;
}

// Bild in den Speicher laden
SDL_Surface* surface = IMG_Load("resources/snake.png");
SDL_Surface* fruitsurface = IMG_Load("resources/fruit.png");

if (surface  == NULL) {
  printf("Bild konnte nicht in den Speicher geladen werden: %s\n", SDL_GetError());
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 1;
}

SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
SDL_FreeSurface(surface);

if (tex == NULL) {
  printf("Textur konnte nicht erstellt werden: %s\n", SDL_GetError());
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 1;
}

surface = IMG_Load("resources/fruit.png");

if (surface  == NULL) {
  printf("2 Bild konnte nicht in den Speicher geladen werden: %s\n", SDL_GetError());
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 1;
}

SDL_Texture* texfruit = SDL_CreateTextureFromSurface(rend, surface);
SDL_FreeSurface(surface);

if (texfruit == NULL) {
  printf("2 Textur konnte nicht erstellt werden: %s\n", SDL_GetError());
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 1;
}

// Struct für Größe und Position des Sprites
SDL_Rect dest;
SDL_Rect fruits;

// Dimensionen der Textur wird in struct gespeichert
SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
SDL_QueryTexture(texfruit, NULL, NULL, &fruits.w, &fruits.h);

// Textur Skalieren
dest.w /= 4;
dest.h /= 4;

fruits.w /= 5;
fruits.h /= 5;

// Sprite in der Mitte platzieren
// Koordinatenursprung ist oben links, positive y-Achse zeigt nach unten
dest.x = (WINDOW_WIDTH - dest.w) / 2;
dest.y = (WINDOW_HEIGHT - dest.h) / 2;
fruits.x = 10;
fruits.y = 10;

int up = 0;
int down = 0;
int left = 0;
int right = 0;

//Bewegungsrichtung
float x_vel = 0;
float y_vel = 0;

int exitbutton = 0;

srand(time(NULL));
// gameloop
while (exitbutton == 0)
    {
        // process events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                exitbutton = 1;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = 1;
                    down = left = right = 0;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 1;
                    right = up = down = 0;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = 1;
                    up = left = right = 0;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 1;
                    left = up = down = 0;
                    break;
                }
                break;
                /*
            case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = 0;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 0;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = 0;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 0;
                    break;
                }
                break; */
            }
}

// determine velocity
x_vel = y_vel = 0;
if (up && !down) y_vel = -SPEED;
if (down && !up) y_vel = SPEED;
if (left && !right) x_vel = -SPEED;
if (right && !left) x_vel = SPEED;

// update positions
dest.x += x_vel / 60;
dest.y += y_vel / 60;


// fruit eating and new random fruit
if (dest.x < fruits.x + COLLECTVALUE && dest.x > fruits.x -COLLECTVALUE
 && dest.y < fruits.y + COLLECTVALUE && dest.y > fruits.y - COLLECTVALUE){
   fruits.x = rand() +1 % WINDOW_WIDTH ;
   fruits.y = rand() +1 % WINDOW_HEIGHT;
 }


// collision detection with bounds and "wrap around"
if (dest.x < 0) dest.x = WINDOW_WIDTH - dest.w;
if (dest.y < 0) dest.y = WINDOW_HEIGHT - dest.h;
if (dest.x > WINDOW_WIDTH - dest.w) dest.x = 0;
if (dest.y > WINDOW_HEIGHT - dest.h) dest.y = 0;

// clear the window
SDL_RenderClear(rend);

// draw the image to the window
SDL_RenderCopy(rend, texfruit, NULL, &fruits);
SDL_RenderCopy(rend, tex, NULL, &dest);
SDL_RenderPresent(rend);

// wait 1/60th of a second
SDL_Delay(1000/60);

}

// cleanup
SDL_DestroyTexture(tex);
SDL_DestroyRenderer(rend);
SDL_DestroyWindow(win);
SDL_Quit();

return 0;
} // Ende Main()
