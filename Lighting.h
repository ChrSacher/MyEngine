#pragma once

#include <glew.h>
 
 
#include <GL/GLu.h>

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "Math/3DMath.h"
#include "Errors.h"
#include "Mesh.h"
#include "Transform.h"
#include "RenderUtil.h"
struct BasicTechniques
{
public:
	GLuint getID(){return __ID;}
	const std::string& getIdentifier(){return __Identifier;} 
protected:
	std::string __Identifier;
	GLuint __ID;
	
};
struct AmbientLight :public BasicTechniques
{
public:
	AmbientLight(Vector3 ambient = Vector3(0.1f,0.1f,0.1f));
	~AmbientLight(void);
	
	void setAmbient(Vector3 newambient);
	Vector3 getAmbient();
private:
	Vector3 ambientLight;

};

struct BaseLight
{
public:
	BaseLight(Vector3 Color = Vector3(0.3f,0.4f,0.1f),float Intensity = 1.0f){color=Color;intensity=Intensity;}
	
	//setters
	void setColor(Vector3 Color){color=Color;};
	void setIntensity(float Intensity){intensity=Intensity;};
	//getters
	Vector3 getColor(){return color;};
	float getIntensity(){return intensity;};
private:
	Vector3 color;
	float intensity;
};
struct DirectionalLight:public BasicTechniques
{
public:
	DirectionalLight(BaseLight Base = BaseLight(),Vector3 Dir = Vector3(1,1,1));
	void setBase(BaseLight Base){base=Base;};
	void setDirection(Vector3 dir){direction=dir;}
	BaseLight getBaseLight(){return base;};
	Vector3 getDirection(){return direction;};
private:
	BaseLight base;
	Vector3 direction;

};

struct Attenuation
{
	public:
		Attenuation(float Constant = 0,float Linear = 0,float Exponent = 1){constant=Constant;linear=Linear;exponent=Exponent;}
		~Attenuation(void){};
		

		void setConstant(float newconstant){constant=newconstant;};
		void setLinear(float newlinear){linear=newlinear;};
		void setExponent(float newexponent){exponent=newexponent;}

		float getConstant(){return constant;};
		float getLinear(){return linear;};
		float getExponent(){return exponent;};
private:
		float constant;
		float linear;
		float exponent;

};

struct PointLight:public BasicTechniques
{
public:
	PointLight(Vector3 Pos = Vector3(0,0,0),BaseLight Base = BaseLight(),Attenuation Atten = Attenuation(),float Range = 0);
	Vector3 pos;
	float range;
	BaseLight base;
	Attenuation attenuation;

};

struct SpotLight:public BasicTechniques
{
public:
	
	SpotLight(PointLight point = PointLight(),Vector3 Dir = Vector3(1,1,1),float Cutoff = 0.4f);
	~SpotLight(){};
	//setters
	void setPointLight(PointLight &light){pointLight=light;}
	void setDir(Vector3 Dir){dir=Dir;}
	void setCutOff(float Cutoff){cutoff=Cutoff;}
	//getters
	PointLight& getPointLight(){return pointLight;}
	Vector3& getDir(){return dir;}
	float& getcutoff(){return cutoff;}
private:
	PointLight pointLight;
	Vector3 dir;
	float cutoff;

};

struct Fog:public BasicTechniques
{
public:
	Fog(float Density = 0.05,Vector4 Color = Vector4(0.5,0.5,0.5,1),float Start = 0,float End = 10,bool Type = true);

	float density; //exp fog
	float start; //linear fog only
	float end; //linear fog only
	Vector4 color;
	int type; //0 = fog disabled 1 = linear 2 = exp 

	void setDensity(float Density);
	void setStart(float Start);
	void setEnd(float End);
	void setColor(Vector4 Color);
	void setType(int Type);
};

enum Lights
{
	Point,
	Spot,
	Ambient,
	Directional,
	Fogs
};
struct LightingCache
{
public:
	LightingCache(){};
	~LightingCache(){};
	void addLight(PointLight light);
	void addLight(SpotLight light);
	void addLight(AmbientLight light);
	void addLight(DirectionalLight light);
	void addFog(Fog newFog);

	const PointLight& getPointLight(std::string Identifier);
	const PointLight& getPointLight(GLuint ID);
	const SpotLight& getSpotLight(std::string Identifier);
	const SpotLight& getSpotLight(GLuint ID);
	std::vector<PointLight>& getPointLights(){return _pointLights;};
	std::vector<SpotLight>& getSpotLights(){return _spotLights;};
	DirectionalLight getDirectionalLight(){return _directionalLight;};
	AmbientLight getAmbientLight(){return _ambientLight;};
	 Fog getFog(){return _fog;};
	unsigned int getCount(Lights counter);
	void forceDelete();
private:
	std::vector<PointLight> _pointLights;
	std::vector<SpotLight> _spotLights;
	AmbientLight _ambientLight;
	DirectionalLight _directionalLight;
	Fog _fog;
};