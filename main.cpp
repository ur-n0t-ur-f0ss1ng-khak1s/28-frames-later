#include "game.h"

// Peter: Redirect I/O to console ...
#ifdef main
#undef main
#endif

int main(int argc,char** argv)
{
  game g;
  g.start();
  return 0;
}
