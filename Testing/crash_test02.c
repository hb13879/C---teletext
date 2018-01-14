#include "teletext.h"
#include <assert.h>

/*test read in function when passed invalid parameters*/

int main(int argc, char** argv)
{
  if(argc != 2) {
    ON_ERROR("Crash test requires filename in argv\n");
  }
  else {
    read_in(NULL,argv[1]);
    printf("Crash test 2 failed");
    return 0;
  }
}
