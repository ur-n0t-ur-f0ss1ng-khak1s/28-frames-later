#include "text.h"

text::text(std::vector<unsigned int>& ch,float w,float h)
{
	characters=ch;
	istest=false;
	width=w;
	height=h;
	islocked=false;
  offset=vector3d(1.87,1.16,3.59);
}

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
  SDL_DestroyTexture(textTexture);
}

void text::drawOrtho(int screenWidth,int screenHeight, const char* text)
{
  test(changescale);
  // Convert text to a vector of model indices
  std::vector<unsigned int> textModels;
  for (int i = 0; text[i] != '\0'; i++) {
    std::cout << "inside loop" << std::endl;
    if(((int)text[i])<33 || ((int)text[i])>126)
      continue;

    int index = static_cast<int>(text[i]) - 33;  // Adjust based on your ASCII offset
    textModels.push_back(characters[index]);

//    if (index >= 0 && index < characters.size()) {
//      textModels.push_back(characters[index]);
//    }
  }

  // Save the current projection and modelview matrices
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, screenWidth, screenHeight, 0); // Set up orthographic projection

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  // Set the text position in screen coordinates, e.g., (10, 10) for the upper-left corner
  glTranslatef(screenWidth / 2, screenHeight / 2, 0.0f);

  // Render each letter in the converted text model list
  for (const auto& letterModel : textModels) {
    glPushMatrix();
    //glTranslatef(offset.x, offset.y, offset.z);  // Text offset adjustments (if needed)

    // Bind the display list or draw the letter object
    glCallList(letterModel);

    glPopMatrix();
  }

  // Restore the previous projection and modelview matrices
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

void text::drawText(vector3d pos,vector3d rot,vector3d scale,const char* tex,float yaw,float pitch)
{
  istest=true;
  bool b1=glIsEnabled(GL_TEXTURE_2D);
  glDisable(GL_TEXTURE_2D);
  glColor3f(1,1,1);
  glPushMatrix();
  int g=0;
  if(false)
  {
    changepos=pos + rot + vector3d(0,-1,0);
    changerot=rot;
    changescale=scale;
    islocked=true;
  }
//  vector3d rightVector(forwardVector.z, 0, -forwardVector.x);
//  rightVector.normalize();
//  curpos = curpos + forwardVector * offset.z //offset forward
//                  + rightVector * offset.x //offset to the right
//                  + vector3d(0,1,0) * offset.y; //offset slight down
  changescale=scale;

  if(istest)
  {
    //test(changescale);
    pos=pos + rot * offset.z
            + vector3d(rot.z,0,-rot.x) * offset.x
            + vector3d(0,1,0) * offset.y;
    glTranslatef(pos.x,pos.y,pos.z);
    glRotatef(yaw, 0.0f, 1.0f, 0.0f);
    glRotatef(pitch, 1.0f, 0.0f, 0.0f);

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
      if(!istest)
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

void text::assignFont(std::vector<unsigned int>& ch,float w,float h)
{
	characters=ch;
	width=w;
	height=h;
}

void text::setTest(bool b)
{
  istest=b;
}

void text::test(vector3d& scale)
{
  const Uint8* key=SDL_GetKeyboardState(NULL);
 if(key[SDL_SCANCODE_H])
    offset.x-=0.01;
  else if(key[SDL_SCANCODE_L])
    offset.x+=0.01;
  else if(key[SDL_SCANCODE_J])
    offset.y+=0.01;
  else if(key[SDL_SCANCODE_K])
    offset.y-=0.01;
  else if(key[SDL_SCANCODE_I])
    offset.z-=0.01;
  else if(key[SDL_SCANCODE_O])
    offset.z+=0.01;
  else if(key[SDL_SCANCODE_N])
  {
    scale.x+=0.005;
    scale.y+=0.005;
    scale.z+=0.005;
  }
  else if(key[SDL_SCANCODE_M])
  {
    scale.x-=0.005;
    scale.y-=0.005;
    scale.z-=0.005;
  }
  std::cout << "offset: " << offset << "scale: " << scale << std::endl;
}

std::vector<unsigned int>& text::getCharacters()
{
  return characters;
}
