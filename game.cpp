#include "game.h"

game::game()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface* screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE|SDL_OPENGL);
  cam.setLocation(vector3d(10,10,10));
  glClearColor(0.5,0.5,0.5,1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45,640.0/480.0,1.0,500.0);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);
  //cube=obj.load("untitled.obj",&collplane);
  initskybox();
}

game::~game()
{
  SDL_Quit();
  killskybox();
}

void game::start()
{
	SDL_Event event;
	Uint32 startTime;
	bool running=true;
	int menuoption=0;
  std::cout << "started the game" << std::endl;
	while(running)
	{
		startTime=SDL_GetTicks();

		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					running=false;
					std::cout << "quiting..." << std::endl;
					break;
        case SDL_MOUSEBUTTONDOWN:
          cam.mouseIn(true);
          SDL_ShowCursor(SDL_DISABLE);
          break;
				case SDL_KEYDOWN:
          if(event.key.keysym.sym==SDLK_ESCAPE)
          {
            running=false;
            break;
          }
			}
		}
		update();
		show();

		SDL_GL_SwapBuffers();
//		if(1000/FPS>(SDL_GetTicks()-startTime))
//			SDL_Delay(1000/FPS-(SDL_GetTicks()-startTime));
	}
}

void game::update()
{

}

void game::show()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  cam.Control();
  drawSkybox(50.0);
  cam.UpdateCamera();
}
