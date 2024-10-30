#include "zombie.h"

zombie::zombie(std::vector<unsigned int>& anim,unsigned int w, unsigned int a, unsigned int d,int h,int str,float sp,collisionsphere ccs)
{
  frames=anim;
  walk=w;
  attack=a;
  die=d;
  health=h;
  strength=str;
  speed=sp;
  cs=ccs;
  iswalk=true;
  isattack=isdead=false;
  curframe=0;
}

bool zombie::update(std::vector<collisionplane>& col,vector3d playerloc)
{
  if(!isdead)
  {
    direction.change(playerloc-cs.center);
    direction.normalize();
    rotation.y=std::acos(direction.z);
    if(direction.x>0)
      rotation.y=-rotation.y;
  }
  if(health<=0 && !isdead)
  {
    isdead=true;
    iswalk=isattack=false;
    curframe=walk+attack;
    return 1;
  }
  if(curframe < frames.size()-1)
  {
    curframe++;
  }
  if(iswalk && curframe>=walk)
    curframe=0;
  else if(isattack && curframe>=walk+attack)
    curframe=walk;
  else if(isdead && curframe>=walk+attack+die)
    curframe=walk+attack+die-1;
  if(!isdead)
  {
    vector3d newpos(cs.center);
    newpos.y-=0.3;
    if(!isattack)
      newpos+=direction*speed;
    for(int i=0;i<col.size();i++)
      collision::sphereplane(newpos,col[i].normal,col[i].p[0],col[i].p[1],col[i].p[2],col[i].p[3],cs.r);
    setLocation(newpos);
  }
  return 0;
}

void zombie::show()
{
  glEnable(GL_NORMALIZE);
  glPushMatrix();
    glTranslatef(cs.center.x,cs.center.y,cs.center.z);
    glRotatef(rotation.x,1,0,0);
    glRotatef(-rotation.y*(180.0/M_PI),0,1,0);
    glRotatef(rotation.z,0,0,1);
    glScalef(0.6,0.6,0.6);
    glCallList(frames[curframe]);
  glPopMatrix();
  glDisable(GL_NORMALIZE);
}

bool zombie::setAttack(collisionsphere player)
{
  if(!isdead && collision::spheresphere(player.center,player.r,cs.center,cs.r))
  {
    isattack=true;
    iswalk=false;
    return 1;
  }else{
    isattack=false;
    if(!isdead)
      iswalk=true;
    return 0;
  }
}

collisionsphere* zombie::getCollisionSphere()
{
  return &cs;
}

void zombie::setLocation(vector3d newloc)
{
  cs.center=newloc;
}

vector3d zombie::getLocation()
{
  return cs.center;
}

void zombie::decreaseHealth(int num)
{
  health-=num;
}

int zombie::getHealth()
{
  return health;
}

int zombie::getStrength()
{
  return strength;
}

bool zombie::isDead()
{
  return isdead;
}
