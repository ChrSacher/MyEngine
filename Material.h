#pragma once
#include "Texture.h"
#include <glew.h>
 
 
#include <GL/GLu.h>
#include "Math/3DMath.h"

 
class Material
{
public:
	Material(std::string,Vector3 Color = Vector3(1,1,1),float intensity = 2,float exponent = 32);
	Material(){}
	~Material(void);

	Texture texture;
	Vector3 color;

	float specularIntensity; //size of the reflection
	float specularPower; //strength

	float getIntensity(){return specularIntensity;}
	float getPower(){return specularPower;}
	Vector3 getColor(){return color;}

	void setIntensity(float inten){specularIntensity=inten;}
	void setPower(float power){specularPower=power;}
	void setColor(Vector3 temp){color = temp;}

	void set(Texture &rtexture,Vector3 rcolor,float rintensity,float rpower){texture=rtexture;color=rcolor;specularIntensity=rintensity;specularPower=rpower;}
	Material& getMaterial(){return *this;};
};

