#include "Lighting.h"


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
void AmbientLight::setAmbient(Vector3 newambient)
{
	ambientLight=newambient;
}

Vector3 AmbientLight::getAmbient()
{
	return ambientLight;
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
SpotLight::SpotLight(PointLight point,Vector3 Dir,float Cutoff)
{
	pointLight = point;
	dir=Dir.normalize();
	cutoff=Cutoff;
	static GLuint ID = 0;
	__ID = ID++;
	__Identifier = "TEMP";
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