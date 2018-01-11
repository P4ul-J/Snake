#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>



int main(int argc, char* argv[]) {

// Grafiksystem initialiseren
if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
  printf("Fehler beim Initialisieren von SDL: %s\n", SDL_GetError());
  return 1;
}

printf("Initialiserung erfolgreich!\n");

//Fenster erstellen
SDL_Window *win = SDL_CreateWindow("Fenster", 100, 100, 640, 480, 0);

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

if (surface == NULL) {
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

// Fenster "säubern" -> Schwarzer Hintergrund
SDL_RenderClear(rend);

// Bild in das Fenster zeichnen
SDL_RenderCopy(rend, tex, NULL, NULL); // Double Buffering
SDL_RenderPresent(rend);

// 5 Sekunden warten
SDL_Delay(5000);

// Resourcen vorm Beenden befreien
SDL_DestroyTexture(tex);
SDL_DestroyRenderer(rend);
SDL_DestroyWindow(win);

SDL_Quit();
return 0;
}
