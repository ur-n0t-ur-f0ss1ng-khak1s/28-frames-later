#include "camera.h"

void camera::lockCamera()
{
	if(camPitch>90)
		camPitch=90;
	if(camPitch<-90)
		camPitch=-90;
	if(camYaw<0.0)
		camYaw+=360.0;
	if(camYaw>360.0)
		camYaw-=360;
}

void camera::moveCamera(float dir)
{
	float rad=(camYaw+dir)*M_PI/180.0;
	loc.x-=sin(rad)*movevel;
	loc.z-=cos(rad)*movevel;
}

void camera::moveCameraUp(float dir)
{
	float rad=(camPitch+dir)*M_PI/180.0;
	loc.y+=sin(rad)*movevel;	
}

camera::camera()
{
  camPitch=0;
  camYaw=0;
  movevel=0.2;
  mousevel=0.2;
  mi=ismoved=false;
}

camera::camera(vector3d l)
{
  loc.change(l);
  camPitch=0;
  camYaw=0;
  movevel=0.2;
  mousevel=0.2;
  mi=ismoved=false;
}

camera::camera(vector3d loc,float yaw,float pitch)
{
  loc.change(loc);
  camPitch=pitch;
  camYaw=yaw;
  movevel=0.2;
  mousevel=0.2;
  mi=ismoved=false;
}

camera::camera(vector3d loc,float yaw,float pitch,float mv,float mov)
{
  loc.change(loc);
  camPitch=pitch;
  camYaw=yaw;
  movevel=mv;
  mousevel=mov;
  mi=ismoved=false;
}

void camera::control(SDL_Window* win)
{
	if(mi)
	{
		int MidX=320;
		int MidY=240;
		SDL_ShowCursor(SDL_DISABLE);
		int tmpx,tmpy;
		SDL_GetMouseState(&tmpx,&tmpy);
		camYaw+=mousevel*(MidX-tmpx);
		camPitch+=mousevel*(MidY-tmpy);
		lockCamera();
		SDL_WarpMouseInWindow(win,MidX,MidY);
		const Uint8* state=SDL_GetKeyboardState(NULL);
    ismoved=false;
		if(state[SDL_SCANCODE_W])
		{
      ismoved=true;
			if(camPitch!=90 && camPitch!=-90)
				moveCamera(0.0);
		}else if(state[SDL_SCANCODE_S])
		{
      ismoved=true;
			if(camPitch!=90 && camPitch!=-90)
				moveCamera(180.0);
		}		
		if(state[SDL_SCANCODE_A])
    {
      ismoved=true;
			moveCamera(90.0);
    }
		else if(state[SDL_SCANCODE_D])
    {
      ismoved=true;
			moveCamera(270);	
    }
	}
	glRotatef(-camPitch,1.0,0.0,0.0);
	glRotatef(-camYaw,0.0,1.0,0.0);
}

void camera::update()
{
  glTranslatef(-loc.x,-loc.y,-loc.z);
}

//change the spherical coordinate system to cartesian
vector3d camera::getVector()
{
  return (vector3d(-cos(camPitch*M_PI/180.0)*sin(camYaw*M_PI/180.0),sin(camPitch*M_PI/180.0),-cos(camPitch*M_PI/180.0)*cos(camYaw*M_PI/180.0)));
}

vector3d camera::getLocation()
{
  return loc;
}

float camera::getCamPitch()
{
  return camPitch;
}

float camera::getCamYaw()
{
  return camYaw;
}

float camera::getMovevel()
{
  return movevel;
}

float camera::getMousevel()
{
  return mousevel;
}

bool camera::isMouseIn()
{
  return mi;
}

void camera::setLocation(vector3d vec)
{
  loc.change(vec);
}

void camera::lookAt(float pitch,float yaw)
{
  camPitch=pitch;
  camYaw=yaw;
}

void camera::mouseIn(bool b)
{
  mi=b;
}

void camera::setSpeed(float mv,float mov)
{
  movevel=mv;
  mousevel=mov;
}

bool camera::isMoved()
{
  return ismoved;
}
