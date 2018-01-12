/*#include "teletext.h"
#include <assert.h>

int main(void)
{
  grid* g;
  g = grid_init();
  assert(g->x == 0);
  assert(g->y == 0);
  assert(g->graphics == 0);
  assert(g->alpha == 1);
  assert(g->foreground == set_colour(white);
  assert(g->background == set_colour(black);
  assert(g->background->r);
  assert(g->dblheight == false);
  assert(g->held == false);
  assert(g->heldchar == ' ');
  free_grid(NULL);  Check free function copes with NULL value
  free_grid(&g);
  assert(g == NULL);
  return 0;
}
*/
#include "teletext.h"

int main(int argc, char** argv)
{
  if(argc != 2) {
    ON_ERROR("Incorrect usage - use the from ./prog teletextfilename.extension\n");
  }
  else {
    grid* g;
    SDL_Simplewin sw;
    Neill_SDL_Init(&sw);
    g = grid_init();
    read_in(g->data,argv[1]);
    print_array(g->data);
    process_render(g,&sw);
    free_grid(&g);
  }
  return 0;
}
