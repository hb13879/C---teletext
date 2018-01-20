#include "teletext.h"

int main(int argc, char** argv)
{
  if(argc != 2) {
    ON_ERROR("Incorrect usage - use the form ./prog teletextfilename.extension\n");
  }
  else {
    SDL_Simplewin sw;
    grid* g = grid_init();
    Neill_SDL_Init(&sw);
    read_in(g->data,argv[1]);
    process_render(g,&sw);
    free_grid(&g);
  }
  return 0;
}
