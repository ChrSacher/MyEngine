#include "Material.h"


Material::Material(std::string path,Vector3 Color,float intensity ,float power)
{
	
	texture = TextureCache::getTexture(path);
	color=Color;
	specularIntensity=intensity;
	specularPower= power;
}


Material::~Material(void)
{
	TextureCache::lowerCount(texture.ID);
}
