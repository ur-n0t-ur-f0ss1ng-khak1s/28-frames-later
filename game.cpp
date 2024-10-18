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

  const int SCREEN_WIDTH = 1024;
  const int SCREEN_HEIGHT = 768;
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
  levels.push_back(std::make_shared<level>("test-level",map,mapcp,mapsp));
  std::vector<unsigned int> anim;
  loadAnimation(anim, "data/weapon1/weapon",38);
  std::cout << "anim size in game(): " << anim.size() << std::endl;
  weapons.push_back(std::make_shared<weapon>(anim,anim[0],1,16,20,vector3d(-0.8,-1.13,6.7),vector3d(0,0,0),vector3d(0,0,0),vector3d(0,0,0),vector3d(0,0,0),100,1000,10,13,300,20,"weapon1",true));
  player1=std::make_unique<player>("player1",collisionsphere(vector3d(0,0,0),3.0),0.2,0.2,0.2,weapons[0]);
  anim.clear();
  loadAnimation(anim,"data/zombie1/zombie",60);
  zombies.push_back(std::make_shared<zombie>(anim,30,20,10,100,5,0.2,collisionsphere(vector3d(5,10,0),3.0)));
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
  vector3d direction;
  bool mousebuttondown=false;
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
          if(event.button.button==SDL_BUTTON_LEFT)
          {
            mousebuttondown=true;
            player1->getCurrentWeapon()->fire(direction,player1->cam.getVector());
          }else if(event.button.button==SDL_BUTTON_RIGHT)
          {
            player1->getCurrentWeapon()->aim();
          }else if(event.button.button==SDL_BUTTON_WHEELUP)
          {
            player1->changeWeaponUp();
          }else if(event.button.button==SDL_BUTTON_WHEELDOWN)
          {
            player1->changeWeaponDown();
          }
          break;
				case SDL_KEYDOWN:
          if(event.key.keysym.sym==SDLK_ESCAPE)
          {
            running=false;
            break;
          }
          switch(event.key.keysym.sym)
          {
            case SDLK_0:
              player1->changeWeapon(0);
              break;
            case SDLK_1:
              player1->changeWeapon(1);
              break;
            case SDLK_2:
              player1->changeWeapon(2);
              break;
            case SDLK_r:
              player1->getCurrentWeapon()->reload();
              break;
          }
          break;
        case SDL_MOUSEBUTTONUP:
          mousebuttondown=false;
          player1->getCurrentWeapon()->stopfire();
          break;
//        case SDL_MOUSEBUTTONUP:
//          switch(event.button.button)
//					{
//						case SDL_BUTTON_LEFT:
//              mousebuttondown=false;
//							player1->getCurrentWeapon()->stopfire();
//							break;
//					}
//					break;
	 
			}
		}

		update();

		show();

		SDL_GL_SwapBuffers();
    int FPS=60;
		if(1000/FPS>(SDL_GetTicks()-startTime))
			SDL_Delay(1000/FPS-(SDL_GetTicks()-startTime));
	}
}

void game::update()
{
  for(int i=0;i<levels.size();i++)
    levels[i]->update();
  player1->update(levels[0]->getCollisionPlanes());
  for(int i=0;i<zombies.size();i++)
    if(zombies[i]->update(levels[0]->getCollisionPlanes(),player1->getCollisionSphere().center))
    {
      //zombie is dead
    }
  for(int i=0;i<zombies.size();i++)
    if(zombies[i]->setAttack(player1->getCollisionSphere()))
    {
      
    }
}

void game::show()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  player1->getCamera()->control();
  drawSkybox(500.0);
  player1->getCamera()->update();
  for(int i=0;i<levels.size();i++)
    levels[i]->show();
  player1->show();
  for(int i=0;i<zombies.size();i++)
    zombies[i]->show();
}

//void game::loadAnimation(std::vector<unsigned int>& frames,std::string filename,unsigned int num)
//{
//  char tmp[200];
//  for(int i=1;i<=num;i++)
//  {
//    if(i<10)
//      sprintf(tmp,"00%d",i);
//    else if(i<100)
//      sprintf(tmp,"00%d",i);
//    else if(i<1000)
//      sprintf(tmp,"%d",i);
//    else if(i<10000)
//      sprintf(tmp,"%d",i);
//    else if(i<100000)
//      sprintf(tmp,"%d",i);
//    else if(i<1000000)
//      sprintf(tmp,"%d",i);
//    std::string tmp2(filename+tmp);
//    tmp2+=".obj";
//    std::cout << "loading " << tmp2 << std::endl;
//    unsigned int id=obj.load(tmp2,NULL);
//    frames.push_back(id);
//  }
//}
  
void game::loadAnimation(std::vector<unsigned int>& anim,const std::string filename,int frames)
{
	char frame[8];
	char tmp[7];
  std::cout << "frames: " << frames << std::endl;
  frames = 38;
	for(int i=1;i<=frames;i++)
	{
		std::string s(filename); //string misformatting here will cause a segfault 
    
		sprintf(frame,"%d",i);
		int len=strlen(frame);
		for(int j=0;j<len;j++)
			tmp[j]=frame[j];
		for(int j=0;j<4-len;j++)
			frame[j]='0';
		for(int j=4-len;j<4;j++)
			frame[j]=tmp[j-4+len];
		frame[4]=NULL;
		s+=frame;
		s+=".obj";
		out << s << std::endl;
    int tmpobj = obj.load(s,NULL);
		anim.push_back(tmpobj);
	}
}
