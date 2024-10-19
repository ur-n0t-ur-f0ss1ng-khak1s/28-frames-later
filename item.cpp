#include "item.h"

oneItem::oneItem(const vector3d& rot,const vector3d& sc,const collisionsphere& ccs,int idnum,unsigned int m)
{
  rotation=rot;
  scale=sc;
  cs=ccs;
  id=idnum;
  mesh=m;
}

void item::add(const vector3d& rot,const vector3d& sc,const collisionsphere& ccs,int idnum,unsigned int m)
{
  items.push_back(oneItem(rot,sc,ccs,idnum,m));
}

void item::del(int id)
{
  for(int i=0;i<items.size();i++)
  {
    if(items[i].id==id)
    {
      items.erase(items.begin()+i);
      break;
    }
  }
}

int item::update(collisionsphere playerpos)
{
  for(int i=0;i<items.size();i++)
  {
    items[i].rotation.y+=1;
    if(items[i].rotation.y>=360)
      items[i].rotation.y-=360;
    if(collision::spheresphere(playerpos.center,playerpos.r,items[i].cs.center,items[i].cs.r))
    {
      int tmp=items[i].id;
      items.erase(items.begin()+i);
      return tmp;
    }
  }
  return -1;
}

void item::show()
{
  for(int i=0;i<items.size();i++)
  {
    glPushMatrix();
      glTranslatef(items[i].cs.center.x,items[i].cs.center.y,items[i].cs.center.z);
      glRotatef(items[i].rotation.x,1,0,0);
      glRotatef(items[i].rotation.y,0,1,0);
      glRotatef(items[i].rotation.z,0,0,1);
      glScalef(items[i].scale.x,items[i].scale.y,items[i].scale.z);
      glCallList(items[i].mesh);
    glPopMatrix();
  }
}
