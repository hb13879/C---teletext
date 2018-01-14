#include "teletext.h"

int main(int argc, char** argv)
{
  if(argc != 2) {
    ON_ERROR("Incorrect usage - use the form ./prog teletextfilename.extension\n");
  }
  else {
    grid* g;
    SDL_Simplewin sw;
    Neill_SDL_Init(&sw);
    g = grid_init();
    read_in(g->data,argv[1]);
    process_render(g,&sw);
    free_grid(&g);
  }
  return 0;
}
