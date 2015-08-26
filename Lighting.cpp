#include "Lighting.h"

const int MAXPOINTLIGHTS = 4;
const int MAXSPOTLIGHTS = 4;

AmbientLight::AmbientLight(Vector3 ambient)
{
	ambientLight=ambient;
	static GLuint ID = 0;
	__ID = ID++;
	__Identifier = "TEMP";
}
AmbientLight::~AmbientLight(void)
{

}
DirectionalLight::DirectionalLight(BaseLight Base ,Vector3 Dir)
{
	base=Base;direction=Dir;
	static GLuint ID = 0;
	__ID = ID++;
	__Identifier = "TEMP";
}
void DirectionalLight::update(Shader* shader)
{
	shader->setUniform("directionalLight.direction",direction);
	shader->setUniform("directionalLight.base.color",base.getColor());
	shader->setUniform("directionalLight.base.intensity",base.getIntensity());
}
	
void AmbientLight::setAmbient(Vector3 newambient)
{
	ambientLight=newambient;
}

Vector3 AmbientLight::getAmbient()
{
	return ambientLight;
}

void AmbientLight::update(Shader *shader)
{
	shader->setUniform("ambientLight",ambientLight);
}

PointLight::PointLight(Vector3 Pos,BaseLight Base,Attenuation Atten,float Range)
{
	pos=Pos;
	base=Base;
	attenuation=Atten;
	range=Range;
	static GLuint ID = 0;
	__ID = ID++;
	__Identifier = "TEMP";
}
void BaseLight::update(std::string uniformName,Shader* shader)
{
	shader->setUniform(uniformName + ".base.intensity",intensity);
	shader->setUniform(uniformName + ".base.color",color);
}
void PointLight::update(std::string uniformName,Shader *shader)
{

		base.update(uniformName,shader);
		shader->setUniform(uniformName + ".pos",pos);
		shader->setUniform(uniformName + ".atten.constant",attenuation.getConstant());
		shader->setUniform(uniformName + ".atten.exponent",attenuation.getExponent());
		shader->setUniform(uniformName + ".atten.linear",attenuation.getLinear());
		shader->setUniform(uniformName + ".range",range);
}

void PointLight::update(Shader *shader,std::vector<PointLight> lights)
{

		if(lights.size() > MAXPOINTLIGHTS)
		{
			fatalError("Too many PointLights passed in\n");
			return;
		}
		for(unsigned int i =0;i<lights.size();i++)
		{
			std::string string("pointLights[");
			string.append(std::to_string(i));
			string = string  + "]";
			lights[i].update(string,shader);
		}
}

SpotLight::SpotLight(PointLight point,Vector3 Dir,float Cutoff)
{
	pointLight = point;
	dir=Dir.normalize();
	cutoff=Cutoff;
	static GLuint ID = 0;
	__ID = ID++;
	__Identifier = "TEMP";
}
void SpotLight::update(std::string uniformname,Shader *shader)
{
		pointLight.update(uniformname + ".pointLight",shader);
		shader->setUniform(uniformname + ".cutoff",cutoff);
		shader->setUniform(uniformname + ".dir",dir);
}
void SpotLight::update(Shader* shader,std::vector<SpotLight> lights)
{

		if(lights.size() > MAXSPOTLIGHTS)
		{
			fatalError("Too many SpotLights passed in\n");
			return;
		}
		for(unsigned int i =0;i<lights.size();i++)
		{
			std::string string("spotLights[");
			string.append(std::to_string(i));
			string = string  + "]";
			lights[i].update(string,shader);
		}	
}
Fog::Fog(float Density,Vector4 Color ,float Start ,float End,bool Type)
{
	density=Density;
	if(Start == End)
	{
		std::cout<<"Fog start = Fog end \nstart = start -= 1"<<std::endl;
		Start -= 1;
	}
	start=Start;
	end=End;
	color=Color;
	type=Type;
	static GLuint ID = 0;
	__ID = ID++;
	__Identifier = "TEMP";
}
void Fog::setDensity(float Density)
{
	density=Density;
}
void Fog::setStart(float Start)
{
	if(Start == end)
	{
		std::cout<<"Fog start = Fog end \nstart = start -= 1"<<std::endl;
		Start -= 1;
	}
	start=Start;
}
void Fog::setEnd(float End)
{
	if(start == End)
	{
		std::cout<<"Fog start = Fog end \nend  = end + 1"<<std::endl;
		End += 1;
	}
	end=End;
}
void Fog::setColor(Vector4 Color)
{
	color=Color;
}
void Fog::setType(int Type)
{
	type=Type;
}
void Fog::update(Shader* shader)
{
	shader->setUniform("fog.density",density);
	shader->setUniform("fog.color",color);
	shader->setUniform("fog.start",start);
	shader->setUniform("fog.type",type);
	shader->setUniform("fog.end",end);
}
//////////////////////////////////////
//LIGHTING CACHE

	void LightingCache::addLight(PointLight light)
	{
		_pointLights.push_back(light);
	}
	void LightingCache::addLight(SpotLight light)
	{
		_spotLights.push_back(light);
	}
	void LightingCache::addLight(AmbientLight light)
	{
		_ambientLight = light;
	}
	void LightingCache::addLight(DirectionalLight light)
	{
		_directionalLight = light;
	}
	void LightingCache::addFog(Fog newFog)
	{
		_fog = newFog;
	}

	const PointLight& LightingCache::getPointLight(std::string Identifier)
	{
		for(unsigned int i = 0; i < _pointLights.size(); i++)
		{
			if(_pointLights[i].getIdentifier() == Identifier)
			{
				return _pointLights[i];
			}
		}
		std::cout<< "Could not find PointLight with Identifier" << Identifier<< std::endl << "Returning Standard PointLight" << std::endl;
		return _pointLights[0];
	}
	const PointLight& LightingCache::getPointLight(GLuint ID)
	{
		for(unsigned int i = 0; i < _pointLights.size(); i++)
		{
			if(_pointLights[i].getID() == ID)
			{
				return _pointLights[i];
			}
		}
		std::cout<< "Could not find PointLight with ID" << ID<< std::endl << "Returning Standard PointLight" << std::endl;
		return _pointLights[0];
	}
	const SpotLight& LightingCache::getSpotLight(std::string Identifier)
	{
		for(unsigned int i = 0; i < _spotLights.size(); i++)
		{
			if(_spotLights[i].getIdentifier() == Identifier)
			{
				return _spotLights[i];
			}
		}
		std::cout<< "Could not find SpotLight with Identifier" << Identifier<< std::endl << "Returning Standard SpotLight" << std::endl;
		return _spotLights[0];
	}
	const SpotLight& LightingCache::getSpotLight(GLuint ID)
	{
		for(unsigned int i = 0; i <  _spotLights.size(); i++)
		{
			if( _spotLights[i].getID() == ID)
			{
				return  _spotLights[i];
			}
		}
		std::cout<< "Could not find SpotLight with ID" << ID<< std::endl << "Returning Standard SpotLight" << std::endl;
		return  _spotLights[0];
	}

	unsigned int LightingCache::getCount(Lights counter)
	{
		switch(counter)
		{
			case Point:
			{
				return _pointLights.size();
			}break;
			case Directional:
			{
				return 1;
			}break;
			case Spot:
			{
				return _spotLights.size();
			}break;
			case Ambient:
			{
				return 1;
			}break;
			case Fogs:
			{
				return 1;
			}break;
			default:
			{
				return 0;
			}
		}
	}