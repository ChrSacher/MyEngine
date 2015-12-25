#pragma once
#include "Texture.h"
#include <glew.h>
 
 
#include <GL/GLu.h>
#include "Math/3DMath.h"
#include "Shader.h"
 
/*This class is the visual looks of a Mesh
	It will hold all the Information necessary to make a Mesh look like it should
*/

class Material
{
public:
	/*
		Standard Constructor for Material
		#param path = Path to the Texture
		#param normalPath = Path to the normalmap
		#param Color = Color of the Material should be kept at 1,1,1 for exact Representation of Textures, usefull if no Texture is loaded
		#param intensity = Lighting information Strength of Reflection
		#param exponent = dunno
		#param fromCache = true forload the Texture from the Cache and false for loading the Texture without entry to TextureCache 
	*/
	Material(std::string path = "res/Texture/white.png" ,std::string normalPath = "res/texture/normal_up.jpg", Vector3 Color = Vector3(1,1,1),float intensity = 2,float power = 32,bool fromCache = true);
	Material(const Material &other);
	void operator=(const Material &other);
	~Material(void);

	Texture texture; 
	Texture normalMap;
	Vector3 color;
	bool isCached;
	float specularIntensity; //size of the reflection
	float specularPower; //strength
	/*
	Will load variables into the shader
	#param shader = Shader to render with
	#param textureSlot int for place
	#param normalSlot same
	*/
	void update(Shader *shader, int textureSlot = 0, int normalSlot = 1);


	float getIntensity(){return specularIntensity;}
	float getPower(){return specularPower;}
	Vector3 getColor(){return color;}

	void setIntensity(float inten){specularIntensity=inten;}
	void setPower(float power){specularPower=power;}
	void setColor(Vector3 temp){color = temp;}

	//same as Standard Construktor
	void set(Texture &rtexture, Texture &ntexture, Vector3 rcolor, float rintensity, float rpower) { texture = rtexture; color = rcolor; normalMap = ntexture; specularIntensity = rintensity; specularPower = rpower; }
	Material& getMaterial(){return *this;};
};

