#include "text.h"

void text::sdlDrawText(SDL_Renderer* renderr,TTF_Font* font,int x,int y,const char* tex)
{
  if (!renderr || !font || !tex) {
    std::cerr << "Invalid parameters passed to sdlDrawText!" << std::endl;
    return;
  }
  SDL_Color textColor = {255, 255, 255};
  SDL_Surface* textSurface = TTF_RenderText_Solid(font, tex, textColor);
  if (!textSurface) {
    std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
    return;
  }
  std::cout << renderr << std::endl;
  SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderr, textSurface);
  if (!textTexture) {
    std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
    SDL_FreeSurface(textSurface);
    return;
  }
  SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
  SDL_FreeSurface(textSurface);

  SDL_RenderClear(renderr);
  if (SDL_RenderCopy(renderr, textTexture, NULL, &textRect) < 0) {
    std::cerr << "Failed to render text: " << SDL_GetError() << std::endl;
  }
  SDL_RenderPresent(renderr);
  SDL_Delay(50000); // Show for 5 seconds
  SDL_DestroyTexture(textTexture);
  std::cout << "nothing rendered" << std::endl;
}

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
    std::cout << ":-p" << std::endl;
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
  const Uint8* key=SDL_GetKeyboardState(NULL);
  if(key[SDLK_f])
    pos.x-=0.01;
  else if(key[SDLK_h])
    pos.x+=0.01;
  else if(key[SDLK_t])
    pos.y+=0.01;
  else if(key[SDLK_g])
    pos.y-=0.01;
  else if(key[SDLK_z])
    pos.z-=0.01;
  else if(key[SDLK_u])
    pos.z+=0.01;
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
