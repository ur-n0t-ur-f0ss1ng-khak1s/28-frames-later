#include "game.h"
//#include <memory>
#include "level.h"

game::game()
{
  out.open("report.txt");
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cerr << "SDL Initialization failed: " << SDL_GetError() << std::endl;
    throw std::runtime_error("SDL Initialization failed");
  }

  const int SCREEN_WIDTH = 640;
  const int SCREEN_HEIGHT = 480;
  const int SCREEN_BPP = 32;

  SDL_Surface* screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_OPENGL);
  if (!screen) {
    std::cerr << "SDL SetVideoMode failed: " << SDL_GetError() << std::endl;
    throw std::runtime_error("Failed to set video mode");
  }

  glClearColor(0.5,0.5,0.5,1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, static_cast<double>(SCREEN_WIDTH) / SCREEN_HEIGHT, 1.0, 500.0);

  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    std::cerr << "OpenGL Error: " << gluErrorString(err) << std::endl;
    throw std::runtime_error("OpenGL Error");
  }
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);

  initskybox();

  std::vector<collisionplane> mapcp;
  std::vector<vector3d> mapsp;
  mapsp.push_back(vector3d(3,4,5));

  unsigned int map=obj.load("map.obj",&mapcp);

  if (map == 0) {
    std::cerr << "Failed to load map.obj." << std::endl;
    throw std::runtime_error("Failed to load map.obj");
  }
  levels.push_back(std::make_unique<level>("test-level",map,mapcp,mapsp));
  std::vector<unsigned int> anim;
  std::vector<std::unique_ptr<weapon>> weapons;
  loadAnimation(anim, "data/weapon/weapon1",38);
  weapons.push_back(std::make_unique<weapon>(anim,anim[0],1,16,19,vector3d(0,0,0),vector3d(0,0,0),vector3d(0,0,0),vector3d(0,0,0),100,1000,10,13,300,20,"weapon1",1));
  player1=new player("player1",collisionsphere(vector3d(10,30,0),3.0),0.2,0.2,0.2,weapons[0].get());
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
          player1->cam.mouseIn(true);
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
  for(int i=0;i<levels.size();i++)
    levels[i]->update();
  player1->update(levels[0]->getCollisionPlanes());
}

void game::show()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  player1->show();
  player1->cam.Control();
  //drawSkybox(50.0);
  player1->cam.UpdateCamera();
  for(int i=0;i<levels.size();i++)
    levels[i]->show();
  player1->show();
}

void game::loadAnimation(std::vector<unsigned int>& anim,const std::string filename,int frames)
{
	char frame[8];
	char tmp[7];
	for(int i=1;i<=frames;i++)
	{
		std::string s(filename+'_');
		sprintf(frame,"%d",i);
		int len=strlen(frame);
		for(int j=0;j<len;j++)
			tmp[j]=frame[j];
		for(int j=0;j<6-len;j++)
			frame[j]='0';
		for(int j=6-len;j<6;j++)
			frame[j]=tmp[j-6+len];
		frame[6]=NULL;
		s+=frame;
		s+=".obj";
		out << s << std::endl;
		anim.push_back(obj.load(s,NULL));
	}

//	std::cout << "load animation" << std::endl;
}
