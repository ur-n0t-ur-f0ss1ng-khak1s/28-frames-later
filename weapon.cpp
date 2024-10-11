weapon::weapon(std::vector<unsigned int>& anim,unsigned int ol,unsigned int na,unsigned int fa,unsigned int ra,vector3d pos,vector3d rot,vector3d apos,vector3d arot,float prec,float aprec,int str,int maxb,int allbul,int speed,const char* name,bool isa)
{
  frames=anim;
  outerview=ol;
  normalanimation=na;
  reloadanimation=ra;
  fireanimation=fa;
  precision=(prec!=0 ? prec : 0.00001);
  aimprecision=(aprec!=0 ? aprec : 0.00001);

  position=pos;
  rotation=rot;

  aimposition=apos;
  aimrotation=arot;

  strength=str;
  allbullets=allbul;
  maxBulletsInMag=maxb;
  speed=sp;
  name=na;

  position_expected=position;
  position_expected=rotation;

  curpos=position;
  currot=rotation;

  isaim=false;
  isreloading=false;
  isautomatic=isa;
  isfired=false;
  istest=true;
  bulletsInMag=maxbulletsInMag;
  
  lastshot=1000;

  curframe=0;
  curmode=1;
}

weapon::~weapon()
{

}

void weapon::update()
{
  test();
  lastshot++;
}

void weapon::show()
{

}

bool weapon::fire(vector3d& direction,vector3d camdirection)
{

}

void weapon::stopfire()
{

}

void weapon::reload()
{

}

void weapon::test()
{

}

void weapon::addBullets(unsigned int num)
{
  allbullets+=num;
}

void weapon::setBullets(unsigned int num)
{
  allbullets=num;
}

int weapon::getStrength()
{
  return strength;
}

int weapon::getAmmo()
{
  return bulletsInMag;
}

int weapon::getAllAmmo()
{
  return allbullets;
}

std::string weapon::getName()
{
  return name;
}

std::vector<unsigned int>& weapon::getAnimation()
{
  return frames;
}

bool weapon::isAimed()
{
  return isaim;
}

unsigned int weapon::getOuterView()
{
  return outerview;
}

void weapon::aim()
{

}

