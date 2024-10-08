#include "collisionplane.h"

collisionplane::collisionplane(float n1, float n2, float n3, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l)
{
  normal.change(n1,n2,n3);
  p[0].change(a,b,c);
  p[1].change(d,e,f);
  p[2].change(g,h,i);
  p[3].change(j,k,l);
}
