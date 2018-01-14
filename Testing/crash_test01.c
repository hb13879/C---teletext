#include "teletext.h"
#include <assert.h>

/*test read in function when passed invalid parameters*/

int main(void)
{
  read_in(NULL,NULL);
  printf("Crash test 01 failed");
  return 0;
}
