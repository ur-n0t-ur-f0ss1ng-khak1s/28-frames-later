#include "collisionsphere.h"

collisionsphere::collisionsphere()
{
  r=0;
}

collisionsphere::collisionsphere(const vector3d& vec, float rad)
{
  center.change(vec);
  r=rad;
}
