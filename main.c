#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {

// Grafiksystem initialiseren
if (SDL_Init(SDL_INIT_VIDEO) != 0) {
  printf("Fehler beim Initialisieren von SDL: %s\n", SDL_GetError());
  return 1;
}

printf("Initialiserung erfolgreich!\n");


SDL_Quit();
return 0;
}
