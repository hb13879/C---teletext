#include "teletext.h"
#include <assert.h>

void test_init_free(void);
void test_getset_data(void);

int main(void)
{
  test_init_free();
  test_getset_data();
  test_grid_parameters();
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

void test_getset_data(void)
{
  grid* g = grid_init();
  assert(get_data(g) == 0); /*calloc'ed array*/
  assert(get_heightmd(g) == sgl);
  g->x = 5;
  g->y = 7;
  /*test data functions*/
  set_data(g,100);
  assert(get_data(g) == 100);
  assert(g->data[7][5].val == 100);
  /*test height mode function*/
  g->data[7][5].height = dbltop;
  assert(get_heightmd(g) == dbltop);
  printf("Get and set data wrapper functions OK\n");
}
