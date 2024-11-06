#include "game.h"

game::game()
{
  srand(static_cast<unsigned int>(time(0)));
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
  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
      std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
  }
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
      std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
  }
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

  //begin the loading screen image display code****
  // Initialize SDL_image to load jpg
  if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)) {
      std::cerr << "SDL_image could not initialize JPG support! SDL_image Error: " << IMG_GetError() << std::endl;
      throw std::runtime_error("SDL_image initialization failed");
  }

  // Load the image
  SDL_Surface* imageSurface = IMG_Load("data/assets/at-my-side.jpg");
  if (!imageSurface) {
      std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
      throw std::runtime_error("Image loading failed");
  }

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Load the pixel data from SDL surface to the OpenGL texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageSurface->w, imageSurface->h, 0, 
               GL_RGB, GL_UNSIGNED_BYTE, imageSurface->pixels);

    // Set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glEnable(GL_TEXTURE_2D);
  // Free the SDL surface after generating the texture
  SDL_FreeSurface(imageSurface);

  // Display the image by rendering a textured quad
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Flip the image vertically and render to cover the screen dimensions
  // WARNING 2.4F IS A MAGIC NUMBER RESPONCIBLE FOR SCALING
  float aspectRatio = static_cast<float>(screenWidth) / screenHeight;
  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-aspectRatio, -1.0f, -2.4f); // Bottom left
    glTexCoord2f(1.0f, 1.0f); glVertex3f( aspectRatio, -1.0f, -2.4f); // Bottom right
    glTexCoord2f(1.0f, 0.0f); glVertex3f( aspectRatio,  1.0f, -2.4f); // Top right
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-aspectRatio,  1.0f, -2.4f); // Top left
  glEnd();

  SDL_GL_SwapWindow(window); // Display the image
  //end the loading screen image display code****

  snd.loadSound("1-can-laugh");
  snd.loadSound("1shot1kill");
  snd.loadSound("aghhh");
  snd.loadSound("bleh");
  snd.loadSound("dabest");
  snd.loadSound("fall-7-rise-8");
  snd.loadSound("gonna");
  snd.loadSound("grunt1");
  snd.loadSound("know-who");
  snd.loadSound("mom-get-the-camera");
  snd.loadSound("my-gorilla-pit");
  snd.loadSound("oof");
  snd.loadSound("pretty");


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

  std::vector<collisionplane> laloco;
  lance=obj.load("data/weapon-lance/lance-of-longinus.obj",&laloco);


  std::vector<collisionplane> mapcp;
  std::vector<vector3d> mapsp;
  mapsp.push_back(vector3d(3,4,5));
  //unsigned int map=obj.load("map.obj",&mapcp);
  //unsigned int map=obj.load("collision-test.obj",&mapcp);
  //unsigned int map=obj.load("data/the-box/the-box.obj",&mapcp);
  unsigned int map=obj.load("data/the-pit/the-pit.obj",&mapcp);

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
//  loadAnimation(anim, "data/weapon1/weapon",38);
//  std::cout << "anim size in game(): " << anim.size() << std::endl;
//  weapons.push_back(std::make_shared<weapon>(anim,anim[0],1,16,20,vector3d(-1.3,-1.63,6.7),vector3d(0,0,0),vector3d(0,0,0),vector3d(0,0,0),vector3d(0,0,0),100,1000,30,7,300,20,"weapon1",true));
//  anim.clear();

  loadAnimation(anim, "data/weapon-revolver/revolver",36);
  std::cout << "big iron size in game(): " << anim.size() << std::endl;
  weapons.push_back(
  std::make_shared<gun>(anim, anim[0], 1, 16, 20, vector3d(-1, -1.5, 4.5), vector3d(0, 0, 0), vector3d(0, 0, 0), vector3d(0, 0, 0), vector3d(0, 0, 0), 100, 1000, 75, 6, 100, 20, "big iron", false));
  anim.clear();

  loadAnimation(anim, "data/weapon-stoner/stoner",28);
  std::cout << "stoner anim size in game(): " << anim.size() << std::endl;
  weapons.push_back(std::make_shared<gun>(anim,anim[0],1,7,20,vector3d(-1,-1.5,4.5),vector3d(0,0,0),vector3d(0,0,0),vector3d(0,0,0),vector3d(0,0,0),100,1000,25,30,500,7,"stoner",true));
  anim.clear();

  loadAnimation(anim, "data/weapon-mosin/mosin-nagant",55);
  std::cout << "mosin nagant anim size in game(): " << anim.size() << std::endl;
  weapons.push_back(std::make_shared<gun>(anim,anim[0],1,30,25,vector3d(-1,-1.5,4.5),vector3d(0,0,0),vector3d(0,0,0),vector3d(0,0,0),vector3d(0,0,0),100,1000,200,5,60,30,"mosin-nagant",false));
  anim.clear();

  loadAnimation(anim,"data/weapon-lance/lance-of-longinus",10);
  weapons.push_back(std::make_shared<melee>(anim,1000,30,30,vector3d(-1.55,-1.11,2)));
  anim.clear();

  player1=std::make_unique<player>("player1",collisionsphere(vector3d(0,7,0),2.0),0.5,0.2,0.2,weapons[2]);
  player1->addWeapon(weapons[1]);
  player1->addWeapon(weapons[0]);
  player1->addWeapon(weapons[3]);
  anim.clear();
  loadAnimation(zombieAnim,"data/zombie1/zombie",60);
  //loadAnimation(zombieAnim,"data/octahedron/octahedron",60);
  std::cout << "zombie size in game(): " << zombieAnim.size() << std::endl;
  zombies.push_back(std::make_shared<zombie>(zombieAnim,30,20,10,100,5,0.1,collisionsphere(vector3d(20,20,0),2.0)));

  mobGenerate=std::make_unique<mobGen>(zombies,vector3d(-20,-20,1),vector3d(30,30,1));
  //zombies.push_back(std::make_shared<zombie>(anim,30,20,10,100,5,0.1,collisionsphere(vector3d(-20,-20,0),2.0)));
  //zombies.push_back(std::make_shared<zombie>(anim,30,20,10,100,5,0.1,collisionsphere(vector3d(20,-20,0),2.0)));

  // it's font time bby!
  fonts.push_back(TTF_OpenFont("ttf/Hack-Regular.ttf",24));
  fonts.push_back(TTF_OpenFont("ttf/Hack-Bold.ttf",24));
  fonts.push_back(TTF_OpenFont("ttf/Hack-Italic.ttf",24));
  fonts.push_back(TTF_OpenFont("ttf/Hack-BoldItalic.ttf",24));

  // opengl type fonts:
	std::vector<unsigned int> chars;
	char c[50];
	for(int i=0;i<94;i++)
	{
		sprintf(c,"data/font/%d.obj",i);
		unsigned int tmp=obj.load(c,NULL);
		chars.push_back(tmp);
	}
	tex=std::make_shared<text>(chars,0.8,0.8,screenWidth,screenHeight);

  // Cleanup for the loading screen image
  glDeleteTextures(1, &textureID);
  IMG_Quit();

  startTime = SDL_GetTicks();
  out << "game started" << std::endl;
}

game::~game()
{
  zombieAnim.clear();
  for(int i=0;i<fonts.size();i++)
  {
    TTF_CloseFont(fonts[i]);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  snd.clean();
  Mix_CloseAudio();
  SDL_Quit();
}

void game::start()
{
	SDL_Event event;
	bool running=true;
	int menuoption=0;
  bool mousebuttondown=false;
  Uint32 loopStartTime;
	while(running)
	{
		loopStartTime=SDL_GetTicks();

    auto currentWeapon = player1->getCurrentWeapon();
    // Check if the weapon is a gun
    std::shared_ptr<gun> curWepIsGun = std::dynamic_pointer_cast<gun>(currentWeapon);
    auto meleePtr = std::dynamic_pointer_cast<melee>(currentWeapon);
		while(SDL_PollEvent(&event))
		{
      if(player1->getHealth()>0)
      {
        switch(event.type)
        {
          case SDL_QUIT:
            running=false;
            break;
          case SDL_MOUSEBUTTONDOWN:
            player1->cam.mouseIn(true);
            SDL_ShowCursor(SDL_DISABLE);
            if(event.button.button==SDL_BUTTON_LEFT)
            {
              if(curWepIsGun){
                mousebuttondown=true;
              }else{
                int victim = meleePtr->setKnife(player1->getCollisionSphere().center,player1->cam.getVector(),zombies);
                if(victim!=-1)
                  zombies[victim]->decreaseHealth(meleePtr->getStrength());
              }
            }else if(event.button.button==SDL_BUTTON_RIGHT)
            {
              if(curWepIsGun)
              {
                curWepIsGun->aim();
              }
            }
          case SDL_MOUSEWHEEL:
            if (event.type == SDL_MOUSEWHEEL)
            {
              if(event.wheel.y > 0)
              {
                player1->changeWeaponUp();
              } else if (event.wheel.y < 0) {
                player1->changeWeaponDown();
              }
            }
          case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
              case SDLK_1:
                player1->changeWeapon(0);
                break;
              case SDLK_2:
                player1->changeWeapon(1);
                break;
              case SDLK_r:
                if (curWepIsGun)
                {
                  curWepIsGun->reload();
                }
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
            if(event.button.button==SDL_BUTTON_LEFT)
            {
              if(curWepIsGun)
              {
                mousebuttondown=false;
                curWepIsGun->stopfire();
                break;
              }else{
                //handle melee stop attack
              }

            }
        }
        if(curWepIsGun && mousebuttondown) {
          if(curWepIsGun->fire(player1->cam.getVector()))
          {
            for(int i=0;i<zombies.size();i++)
              if(collision::raysphere(
              zombies[i]->getCollisionSphere()->center.x,
              zombies[i]->getCollisionSphere()->center.y,
              zombies[i]->getCollisionSphere()->center.z,
              curWepIsGun->getDirection().x,
              curWepIsGun->getDirection().y,
              curWepIsGun->getDirection().z,
              player1->getCollisionSphere().center.x,
              player1->getCollisionSphere().center.y,
              player1->getCollisionSphere().center.z,
              zombies[i]->getCollisionSphere()->r))
              {
                std::cout << "shot one" << std::endl;
                zombies[i]->decreaseHealth(player1->getCurrentWeapon()->getStrength());
              }
          }
        }
      } else if (player1->getHealth()<=0){
        switch(event.type)
        {
          case SDL_QUIT:
            running=false;
            break;
          case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
              case SDLK_ESCAPE:
                running=false;
                break;
            }
        }
      }
    }
		update();
		show();

		SDL_GL_SwapWindow(window);
    int FPS=60;
		if(1000/FPS>(SDL_GetTicks()-loopStartTime))
			SDL_Delay(1000/FPS-(SDL_GetTicks()-loopStartTime));
	}
}

void game::update()
{
  for(int i=0;i<levels.size();i++)
    levels[i]->update();
  player1->update(levels[0]->getCollisionPlanes());
  bool allZombiesAreDead=true;
  for(int i=0;i<zombies.size();i++)
  {
    if(zombies[i]->update(levels[0]->getCollisionPlanes(),player1->getCollisionSphere().center))
    {
      //zombie died logic:
      //items.add(vector3d(0,0,0),vector3d(0.02,0.02,0.02),collisionsphere(zombies[i]->getLocation(),2.0),0,lance);
      //srand(static_cast<unsigned int>(time(0)));

      int randomChance = rand() % 20;

      if (randomChance == 0)
      {
        int randomIndex = rand() % 8;

        switch (randomIndex)
        {
          case 0:
            //snd.playSound("1-can-laugh");
            break;
          case 1:
            snd.playSound("1shot1kill");
            break;
          case 2:
            snd.playSound("dabest");
            break;
          case 3:
            //snd.playSound("fall-7-rise-8");
            break;
          case 4:
            snd.playSound("gonna");
            break;
          case 5:
            snd.playSound("know-who");
            break;
          case 6: 
            //snd.playSound("mom-get-the-camera");
            break;
          case 7:
            //snd.playSound("my-gorilla-pit");
            break;
          case 8:
            snd.playSound("pretty");
            break;
        }
      }
      player1->addPoints(1);
    }
    if(zombies[i]->isDead()==false)
    {
      allZombiesAreDead=false;
    }
  }
  if(allZombiesAreDead)
  {
    mobGenerate->spawnMob(zombieAnim);
  }
  for(int i=0;i<zombies.size();i++)
    if(zombies[i]->setAttack(player1->getCollisionSphere()))
    {
      //the zombie is attacking the player
      int randomIndex = rand() % 8;

      switch (randomIndex)
      {
        case 0:
          snd.playSound("aghhh");
          break;
        case 1:
          snd.playSound("bleh");
          break;
        case 2:
          snd.playSound("grunt1");
          break;
        case 3:
          snd.playSound("oof");
          break;
      }
      player1->setHealth(player1->getHealth()-zombies[i]->getStrength());
    }
  int j=items.update(player1->getCollisionSphere());
  if(j==0)
  {
//    char tmp[200];
//    sprintf(tmp,"  lance aquired  ");
//    tex->fillScreenOrtho(tmp);
  }
  if(player1->getHealth()<=0 && player1->getLifeTime() == 0)
  {
    player1->setLifeTime(SDL_GetTicks()-startTime);
  }
}

void game::show()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  if(player1->getHealth()>0) player1->getCamera()->control(window);
  levels[0]->getSkybox()->drawSkybox();
  player1->getCamera()->update();
  for(int i=0;i<levels.size();i++)
    levels[i]->show();
  player1->show();
  for(int i=0;i<zombies.size();i++)
    zombies[i]->show();
  items.show();
  	// Peter: I clear the depth buffer here so that nothing comes in front of the menu ...
	glClear(GL_DEPTH_BUFFER_BIT);
  renderCrosshair();
  if(player1->getHealth()<=0)
  {
    char tmp[200];
    sprintf(tmp,"----game_over!_zombies_smoked:_%d_in_%d_seconds", player1->getPoints(),player1->getLifeTime()/1000);
    tex->fillScreenOrtho(tmp,10,255);
  } else {
    auto currentWeapon = player1->getCurrentWeapon();
    // Check if the weapon is a gun
    std::shared_ptr<gun> curWepIsGun = std::dynamic_pointer_cast<gun>(currentWeapon);

    if(curWepIsGun)
    {
      drawMenu(player1->getHealth(),curWepIsGun->getAmmo(),curWepIsGun->getAllAmmo(),player1->getPoints(),currentWeapon->getName());
    }else{
      drawMenu(player1->getHealth(),666,777,player1->getPoints(),currentWeapon->getName());
    }
  }
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

void game::renderCrosshair() {
    // Switch to 2D rendering (orthographic projection)
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, 0, screenHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Draw crosshair
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
        glVertex2f(screenWidth / 2.0f, screenHeight / 2.0f);
    glEnd();
    // Restore original matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void game::drawMenu(int health,int ammo,int allammo,int point,const std::string& weaponName)
{
  //cam.getCamPitch(),cam.getCamYaw()
  char tmp[200];
  sprintf(tmp,"health: %d",health);
  //tex->sdlDrawText(renderer,fonts[0],0,0,tmp);
  //tex->drawText(player1->getCamera()->getLocation(),player1->getCamera()->getVector(),vector3d(0.3,0.3,0.3),tmp,player1->getCamera()->getCamYaw(),player1->getCamera()->getCamPitch());
  tex->drawOrtho(0,0,tmp);
  sprintf(tmp,"%s    %d / %d",weaponName.c_str(),ammo,allammo);
  tex->drawOrtho(0,20,tmp);
//  tex->drawText(vector3d(-0.54,-0.39,-1),vector3d(0,0,0),vector3d(0.035,0.035,0.035),tmp);
  sprintf(tmp,"points: %d",point);
  tex->drawOrtho(0,40,tmp);
//  tex->drawText(vector3d(0.22,0.35,-1),vector3d(0,0,0),vector3d(0.035,0.035,0.035),tmp);
}


