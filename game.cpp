#include "game.h"

game::game()
{
  out.open("report.txt");
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL Initialization failed: " << SDL_GetError() << std::endl;
    throw std::runtime_error("SDL Initialization failed");
  }
  if (TTF_Init() == -1) {
    std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
  }
  window = SDL_CreateWindow("28 frames later", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
  if (!window) {
    std::cerr << "SDL SetVideoMode failed: " << SDL_GetError() << std::endl;
    throw std::runtime_error("Failed to set video mode");
  }
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //we need an SDL renderer for fonts and text rendering

  // Create an OpenGL context
  SDL_GLContext glContext = SDL_GL_CreateContext(window);
  if (!glContext) {
    std::cerr << "OpenGL context could not be created: " << SDL_GetError() << std::endl;
    throw std::runtime_error("Failed to create OpenGL context");
  }

  glClearColor(0.5,0.5,0.5,1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, static_cast<double>(screenWidth) / screenHeight, 1.0, 500.0);

  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    std::cerr << "OpenGL Error: " << gluErrorString(err) << std::endl;
    throw std::runtime_error("OpenGL Error");
  }
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);

//  std::vector<collisionplane> goApCp;
//  goldenApple=obj.load("data/golden-apple/golden-apple.obj",&goApCp);
  loadAnimation(goldenApples, "data/golden-apple/golden-apple", 1);
  std::vector<collisionplane> tegrco;
  testgreen=obj.load("data/testgreen/testgreen.obj",&tegrco);
  if (testgreen == 0)
  {
    std::cerr << "failed to load testgreen" << std::endl;
  }
  items.add(vector3d(0,0,0),vector3d(1,1,1),collisionsphere(vector3d(30,2,20),2.0),0,testgreen);


  std::vector<collisionplane> mapcp;
  std::vector<vector3d> mapsp;
  mapsp.push_back(vector3d(3,4,5));
  //unsigned int map=obj.load("map.obj",&mapcp);
  //unsigned int map=obj.load("collision-test.obj",&mapcp);
  unsigned int map=obj.load("data/the-box/the-box.obj",&mapcp);
  //unsigned int map=obj.load("data/egypt/egypt.obj",&mapcp);

  if (map == 0) {
    std::cerr << "Failed to load map.obj." << std::endl;
    throw std::runtime_error("Failed to load map.obj");
  }

  // it's skybox time bby!
  unsigned int left=loadTexture("data/skybox/left.bmp");
  unsigned int back=loadTexture("data/skybox/back.bmp");
  unsigned int right=loadTexture("data/skybox/right.bmp");
  unsigned int front=loadTexture("data/skybox/front.bmp");
  unsigned int top=loadTexture("data/skybox/top.bmp");
  unsigned int bottom=loadTexture("data/skybox/bottom.bmp");

  levels.push_back(std::make_shared<level>("test-level",map,mapcp,mapsp,left,back,right,front,top,bottom));
  std::vector<unsigned int> anim;
  loadAnimation(anim, "data/weapon1/weapon",38);
  std::cout << "anim size in game(): " << anim.size() << std::endl;
  weapons.push_back(std::make_shared<weapon>(anim,anim[0],1,16,20,vector3d(-1.3,-1.63,6.7),vector3d(0,0,0),vector3d(0,0,0),vector3d(0,0,0),vector3d(0,0,0),100,1000,30,7,300,20,"weapon1",true));
  anim.clear();

  loadAnimation(anim, "data/weapon-revolver/revolver",36);
  std::cout << "anim size in game(): " << anim.size() << std::endl;
  weapons.push_back(std::make_shared<weapon>(anim,anim[0],1,16,20,vector3d(-1.5,-3.6,8),vector3d(0,0,0),vector3d(0,0,0),vector3d(0,0,0),vector3d(0,0,0),100,1000,75,6,300,20,"rickGrimes",true));
  anim.clear();

  player1=std::make_unique<player>("player1",collisionsphere(vector3d(0,7,0),2.0),0.5,0.2,0.2,weapons[1]);
  anim.clear();
  loadAnimation(anim,"data/zombie1/zombie",60);
  zombies.push_back(std::make_shared<zombie>(anim,30,20,10,100,5,0.1,collisionsphere(vector3d(20,20,0),2.0)));

  // it's font time bby!
  fonts.push_back(TTF_OpenFont("ttf/Hack-Regular.ttf",24));
  fonts.push_back(TTF_OpenFont("ttf/Hack-Bold.ttf",24));
  fonts.push_back(TTF_OpenFont("ttf/Hack-Italic.ttf",24));
  fonts.push_back(TTF_OpenFont("ttf/Hack-BoldItalic.ttf",24));
}

game::~game()
{
  for(int i=0;i<fonts.size();i++)
  {
    TTF_CloseFont(fonts[i]);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
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
            if(player1->getCurrentWeapon()->fire(direction,player1->cam.getVector()))
            {
              for(int i=0;i<zombies.size();i++)
                if(collision::raysphere(
                zombies[i]->getCollisionSphere()->center.x,
                zombies[i]->getCollisionSphere()->center.y,
                zombies[i]->getCollisionSphere()->center.z,
                direction.x,direction.y,direction.z,
                player1->getCollisionSphere().center.x,
                player1->getCollisionSphere().center.y,
                player1->getCollisionSphere().center.z,
                zombies[i]->getCollisionSphere()->r))
                {
                  zombies[i]->decreaseHealth(player1->getCurrentWeapon()->getStrength());
                }
            }
          }else if(event.button.button==SDL_BUTTON_RIGHT)
          {
            player1->getCurrentWeapon()->aim();
          }else if(event.button.button==SDL_MOUSEWHEEL && event.wheel.y > 0)
          {
            player1->changeWeaponUp();
          }else if(event.button.button==SDL_MOUSEWHEEL && event.wheel.y < 0)
          {
            player1->changeWeaponDown();
          }
          break;
				case SDL_KEYDOWN:
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
            case SDLK_SPACE:
              player1->setJump();
              break;
            case SDLK_LSHIFT:
              player1->setSprint();
              break;
            case SDLK_ESCAPE:
              running=false;
              break;
          }
          break;
        case SDL_KEYUP:
          switch(event.key.keysym.sym)
          {
            case SDLK_LSHIFT:
              player1->stopSprint();
              break;
          }
        case SDL_MOUSEBUTTONUP:
          mousebuttondown=false;
          player1->getCurrentWeapon()->stopfire();
          break;
			}
		}
		update();
		show();

		SDL_GL_SwapWindow(window);
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
      //zombie died logic:
      items.add(vector3d(0,0,0),vector3d(1,1,1),*zombies[i]->getCollisionSphere(),0,goldenApples[0]);
      
//      items.add(vector3d(0,0,0),vector3d(1,1,1),*zombies[i]->getCollisionSphere(),0,weapons[0]->getOuterView());
    }
  for(int i=0;i<zombies.size();i++)
    if(zombies[i]->setAttack(player1->getCollisionSphere()))
    {
      //the zombie is attacking the player
      player1->setHealth(player1->getHealth()-zombies[i]->getStrength());
    }
  int j=items.update(player1->getCollisionSphere());
  if(j==0)
  {
    std::cout << "weapon picked up" << std::endl;
  }
}

void game::show()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  player1->getCamera()->control(window);
  levels[0]->getSkybox()->drawSkybox();
  player1->getCamera()->update();
  for(int i=0;i<levels.size();i++)
    levels[i]->show();
  player1->show();
  for(int i=0;i<zombies.size();i++)
    zombies[i]->show();
  items.show();
  renderCrosshair();
  //drawMenu(player1->getHealth(),player1->getCurrentWeapon()->getAmmo(),player1->getCurrentWeapon()->getAllAmmo(),player1->getPoints(),player1->getCurrentWeapon()->getName());
}

void game::loadAnimation(std::vector<unsigned int>& anim,const std::string filename,int frames)
{
	char frame[8];
	char tmp[7];
  std::cout << "frames: " << frames << std::endl;
  frames = frames;
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

unsigned int game::loadTexture(const char* filename)
{
  unsigned int num; // The ID for the texture
  glGenTextures(1, &num); // Generate a unique texture ID

  // Load the image using SDL_image
  SDL_Surface* img = SDL_LoadBMP(filename); // Use IMG_Load instead of SDL_LoadBMP for multiple formats
  if (!img) {
      // Handle error (e.g., log the error message)
      std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
      return 0; // Return 0 to indicate failure
  }

	glBindTexture(GL_TEXTURE_2D,num);	//and use the texture, we have just generated
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//if the texture is smaller, than the image, we get the avarege of the pixels next to it
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //same if the image bigger
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);	//we repeat the pixels in the edge of the texture, it will hide that 1px wide line at the edge of the cube, which you have seen in the video
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);	//we do it for vertically and horizontally (previous line)

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img->w,img->h,0,GL_RGB,GL_UNSIGNED_SHORT_5_6_5,img->pixels);	//we make the actual texture
  SDL_FreeSurface(img); // Free the surface as it's no longer needed
  return num; // Return the texture ID
}

//renderCrosshair is courtesy of chatGPT :)
void game::renderCrosshair() {
    // Switch to 2D rendering (orthographic projection)
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    // Set up orthographic projection (for 2D rendering)
    gluOrtho2D(0, screenWidth, 0, screenHeight);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable depth testing and lighting for 2D rendering
    //glDisable(GL_DEPTH_TEST);
    //glDisable(GL_LIGHTING);

    // Set the color of the dot (white)
    glColor3f(1.0f, 1.0f, 1.0f);

    // Enable point size control 5.0f is size of the dot
    glPointSize(5.0f);

    // Draw the dot at the center of the screen
    glBegin(GL_POINTS);
        glVertex2f(screenWidth / 2.0f, screenHeight / 2.0f);
    glEnd();

    // Re-enable depth testing and lighting (for 3D rendering)
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LIGHTING);

    // Restore original projection and modelview matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void game::drawMenu(int health,int ammo,int allammo,int point,const std::string& weaponName)
{
  char tmp[200];
  sprintf(tmp,"health: %d",health);
  tex->sdlDrawText(renderer,fonts[0],0,0,tmp);
//  tex->drawText(vector3d(-0.5,0.35,-1),vector3d(0,0,0),vector3d(0.035,0.035,0.035),tmp);
//  sprintf(tmp,"%s    %d / %d",weaponName.c_str(),ammo,allammo);
//  tex->drawText(vector3d(-0.54,-0.39,-1),vector3d(0,0,0),vector3d(0.035,0.035,0.035),tmp);
//  sprintf(tmp,"Points: %d",point);
//  tex->drawText(vector3d(0.22,0.35,-1),vector3d(0,0,0),vector3d(0.035,0.035,0.035),tmp);
}


