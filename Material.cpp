#include "Material.h"


Material::Material(std::string path,std::string normalPath,Vector3 Color,float intensity ,float power)
{
	
	texture = TextureCache::getTexture(path);
	normalMap = NormalCache::getTexture(normalPath);
	color=Color;
	specularIntensity=intensity;
	specularPower= power;
}


Material::~Material(void)
{
	TextureCache::lowerCount(texture.ID);
}

void Material::update(Shader *shader)
{
	
		texture.bind();
		shader->setUniform("Texture",0);
		normalMap.bind(1);
		shader->setUniform("normalMap",1);
		shader->setUniform("specularIntensity",specularIntensity);
		shader->setUniform("specularPower",specularPower);
		shader->setbaseColor(color);
}