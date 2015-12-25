#include "Material.h"


Material::Material(std::string path,std::string normalPath,Vector3 Color,float intensity ,float power,bool fromCache)
{
	if(fromCache)
	{
		texture = TextureCache::getTexture(path);
		normalMap = NormalCache::getTexture(normalPath);
	}
	else
	{
		texture = TextureLoader::load(path);
		normalMap = TextureLoader::loadNormal(normalPath);
	}
	isCached = fromCache;
	color=Color;
	specularIntensity=intensity;
	specularPower= power;
}
Material::Material(const Material &other)
{
	if (other.isCached)
	{
		texture = TextureCache::getTexture(other.texture.texturepath);
		normalMap = NormalCache::getTexture(other.normalMap.texturepath);
	}
	else
	{
		texture = TextureLoader::load(other.texture.texturepath);
		normalMap = TextureLoader::loadNormal(other.normalMap.texturepath);
	}
	isCached = other.isCached;
	color = other.color;
	specularIntensity = other.specularIntensity;
	specularPower = other.specularPower;
}

void Material::operator=(const Material & other)
{
	if (other.isCached)
	{
		texture = TextureCache::getTexture(other.texture.texturepath);
		normalMap = NormalCache::getTexture(other.normalMap.texturepath);
	}
	else
	{
		texture = TextureLoader::load(other.texture.texturepath);
		normalMap = TextureLoader::loadNormal(other.normalMap.texturepath);
	}
	isCached = other.isCached;
	color = other.color;
	specularIntensity = other.specularIntensity;
	specularPower = other.specularPower;
}

Material::~Material(void)
{
	if(isCached) TextureCache::lowerCount(texture.texturepath);
}

void Material::update(Shader *shader,int textureSlot,int normalSlot)
{
	
		texture.bind(textureSlot);
		shader->setUniform("Texture",textureSlot);
		normalMap.bind(normalSlot);
		shader->setUniform("normalMap",normalSlot);
		shader->setUniform("specularIntensity",specularIntensity);
		shader->setUniform("specularPower",specularPower);
		shader->setbaseColor(color);
}