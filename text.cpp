#include "text.h"

void text::drawText(vector3d pos,vector3d rot,vector3d scale,const char* tex)
{
  bool b1=glIsEnabled(GL_TEXTURE_2D);
  glDisable(GL_TEXTURE_2D);
  glColor3f(1,1,1);
  glPushMatrix();
  int g=0;
  if(!islocked)
  {
    changepos=pos;
    changerot=rot;
    changescale=scale;
    islocked=true;
  }
  if(istest)
  {
    test(changepos,changerot,changescale);
    glTranslatef(changepos.x,changepos.y,changepos.z);
    glRotatef(changerot.x,1,0,0);
    glRotatef(changerot.y,0,1,0);
    glRotatef(changerot.z,0,0,1);
  }else{
    glTranslatef(pos.x,pos.y,pos.z);
    glRotatef(rot.x,1,0,0);
    glRotatef(rot.y,0,1,0);
    glRotatef(rot.z,0,0,1);
  }
  for(int i=0;i<strlen(tex);i++)
  {
    if(tex[i]=='\n')
    {
      glTranslatef(-g*width,-height,0);
      g=0;
      continue;
    }
    if(tex[i]==' ')
    {
      glTranslatef(width*changescale.x,0,0);
      g++;
      continue;
    }
    if(((int)tex[i])<33 || ((int)tex[i])>126)
      continue;
    glPushMatrix();
      if(istest)
      {
        glScalef(changescale.x,changescale.y,changescale.z);
      }else{
        glScalef(scale.x,scale.y,scale.z);
      }
      glCallList(characters.at((int)tex[i]-33));
      g++;
    glPopMatrix();
    glTranslatef(width*changescale.x,0,0);
  }
  glPopMatrix();
  if(b1)
    glEnable(GL_TEXTURE_2D);
}

text::text(std::vector<unsigned int>& ch,float w,float h)
{
  characters=ch;
  width=w;
  height=h;
}

void text::setTest(bool b)
{
  istest=b;
}

void text::test(vector3d& pos,vector3d& rot,vector3d& scale)
{
  Uint8* key=SDL_GetKeyState(NULL);
  if(key[SDLK_f])
    pos.x-=0.01;
  else if(key[SDLK_h])
    pox.x+=0.01;
  else if(key[SDLK_t])
    pox.y+=0.01;
  else if(key[SDLK_g])
    pox.y-=0.01;
  else if(key[SDLK_z])
    pox.z-=0.01;
  else if(key[SDLK_u])
    pox.z+=0.01;
  else if(key[SDLK_x])
  {
    scale.x+=0.005;
    scale.y+=0.005;
    scale.z+=0.005;
  }
  else if(key[SDLK_y])
  {
    scale.x-=0.005;
    scale.y-=0.005;
    scale.z-=0.005;
  }
  std::cout << pos << rot << scale << std::endl;
}
