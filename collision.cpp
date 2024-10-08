#include "collision.h"

bool raysphere(float xc,float yc,float zc,float xd,float yd,float zd,float xs,float ys,float zs,float r)
{
	float b=2*(xd*(xs-xc)+yd*(ys-yc)+zd*(zs-zc));
	float c=xs*xs-2*xs*xc+xc*xc+ys*ys-2*ys*yc+yc*yc+zs*zs-2*zs*zc+zc*zc-r*r;
	float disc=(b*b-4*c);
	if(disc<0)	//if the discriminant is less then 0, there is no intersection
		return false;
	else
		return true;
}

float trianglearea(vector3d p1,vector3d p2,vector3d p3)
{
	//area of the triangle with the heron fomula
	float a=sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y)+(p2.z-p1.z)*(p2.z-p1.z));
	float b=sqrt((p3.x-p2.x)*(p3.x-p2.x)+(p3.y-p2.y)*(p3.y-p2.y)+(p3.z-p2.z)*(p3.z-p2.z));
	float c=sqrt((p1.x-p3.x)*(p1.x-p3.x)+(p1.y-p3.y)*(p1.y-p3.y)+(p1.z-p3.z)*(p1.z-p3.z));
	float s=(a+b+c)/2.;
	return (sqrt(s*((s-a)*(s-b)*(s-c))));
}

bool rayplane(const float& nx,float ny,float nz,float x0,float y0,float z0,float xs,float ys,float zs,float xd,float yd,float zd,vector3d p1,vector3d p2,vector3d p3,vector3d p4)
{
	if((xd*nx+yd*ny+zd*nz)==0)	//if the two vector dot product is 0, then there is no intersection (we don't like to divide by 0)
		return false;
	float t=((x0*nx+y0*ny+z0*nz-nx*xs-ny*ys-nz*zs)/(xd*nx+yd*ny+zd*nz));
	if(t<0)	//if t<0, the intersction point is in the opposite direction
		return false;
	float x=xs+t*xd;	//calculate the 3 point vector3d
	float y=ys+t*yd;
	float z=zs+t*zd;
	vector3d i(x,y,z);
	if((abs(trianglearea(p1,p2,p3)-(trianglearea(p1,p2,i)+trianglearea(p2,p3,i)+trianglearea(p1,p3,i)))<0.001) || abs(trianglearea(p1,p3,p4)-(trianglearea(p1,p3,i)+trianglearea(p3,p4,i)+trianglearea(p1,p4,i)))<0.001)	//we divide the quad to 2 triangle, we divide one triangle to 3 (one point is the
	//intersection point), and if the area of the 3 triangle is equal to the main triangle, then the point is inside the triangle. We do the same with
	//the other triangle, and if one is true, then the point is in the quad
		return true;
	return false;	//else not
}

//we just give back the distance of the plane
float rayplanedist(float nx,float ny,float nz,float x0,float y0,float z0,float xs,float ys,float zs,float xd,float yd,float zd)
{
	if((xd*nx+yd*ny+zd*nz)==0)
		return 0;
	float t=((x0*nx+y0*ny+z0*nz-nx*xs-ny*ys-nz*zs)/(xd*nx+yd*ny+zd*nz));
	if(t<0)
		return 0;
	return t;
}

float pointdistacesquare(vector3d p1,vector3d p2)
{
	vector3d vec(p2.x-p1.x,p2.y-p1.y,p2.z-p1.z);
	return (vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
}

bool spheresphere(vector3d c1,float r1,vector3d c2,float r2)
{
	return (pointdistacesquare(c1,c2)<=(r1+r2)*(r1+r2));
}

//distance of 2 point (with 3d pythagoras formula)
float pointdistace(vector3d p1,vector3d p2)
{
	vector3d vec(p2.x-p1.x,p2.y-p1.y,p2.z-p1.z);
	return (sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z));
}

bool sphereplane(vector3d& sp,vector3d pn,vector3d p1,vector3d p2,vector3d p3,vector3d p4, float r)
{
		//if the distance is greater then the radius, there is no collision
		if(rayplanedist(pn.x,pn.y,pn.z,p1.x,p1.y,p1.z,sp.x,sp.y,sp.z,-pn.x,-pn.y,-pn.z)>r || rayplanedist(-pn.x,-pn.y,-pn.z,p1.x,p1.y,p1.z,sp.x,sp.y,sp.z,pn.x,pn.y,pn.z)>r)
			return false;
	//if the ray from the center of the sphere is intersect the plane, there is a collision
	if(rayplane(pn.x,pn.y,pn.z,p1.x,p1.y,p1.z,sp.x,sp.y,sp.z,-pn.x,-pn.y,-pn.z,p1,p2,p3,p4) || rayplane(-pn.x,-pn.y,-pn.z,p1.x,p1.y,p1.z,sp.x,sp.y,sp.z,pn.x,pn.y,pn.z,p1,p2,p3,p4))
	{
		float raycol=rayplanedist(pn.x,pn.y,pn.z,p1.x,p1.y,p1.z,sp.x,sp.y,sp.z,-pn.x,-pn.y,-pn.z);	//we calculate how far we are
		if(raycol)	//if not zero
		{
			sp.x=sp.x+pn.x*(r-raycol);	//calculate the point, and calculate our new location
			sp.y=sp.y+pn.y*(r-raycol);
			sp.z=sp.z+pn.z*(r-raycol);		
		}else	//else if we are in the opposite side, then the normal vector
		{
			raycol=rayplanedist(-pn.x,-pn.y,-pn.z,p1.x,p1.y,p1.z,sp.x,sp.y,sp.z,pn.x,pn.y,pn.z);
			sp.x=sp.x-pn.x*(r-raycol);
			sp.y=sp.y-pn.y*(r-raycol);
			sp.z=sp.z-pn.z*(r-raycol);				
		}

		return true;	//we return true, to indicate the collision, but the main thing is, that we changged the sp (so the sphere position) to the new position)
	}
	return false;
}

