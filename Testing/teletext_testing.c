#include "teletext.h"
#include <assert.h>

void test_init_free(void);

int main(void)
{
  test_init_free();
  return 0;
}

void test_init_free(void)
{
  grid* g;
  grid* h;
  g = grid_init();
  assert(g->x == 0);
  assert(g->y == 0);
  assert(g->graphics == 0);
  assert(g->alpha == 1);
  assert(g->background->r == 0);
  assert(g->background->g == 0);
  assert(g->background->b == 0);
  assert(g->foreground->r == HUE);
  assert(g->foreground->g == HUE);
  assert(g->foreground->b == HUE);
  assert(g->dblheight == false);
  assert(g->held == false);
  assert(g->heldchar == ' ');
  /*test background free functions*/
  h = grid_init();
  free_data(&(h->data));
  assert(h->data == NULL);
  free_colour(&(h->foreground));
  assert(h->foreground == NULL);
  free_colour(&(h->background));
  assert(h->background == NULL);
  /*test main free function*/
  free_grid(&g);
  assert(g == NULL);
  /*Check free functions cope with NULL values*/
  free_data(NULL);
  free_colour(NULL);
  free_grid(NULL);
  printf("Init and free functions OK\n");
}

/*
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
*/
