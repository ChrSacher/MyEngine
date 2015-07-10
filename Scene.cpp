#include "Scene.h"
#define TERRAINSIZE 6
#define OBJECTSIZE 16
#define POINTLIGHTSIZE 12

std::vector<std::string> find_files()
{

  WIN32_FIND_DATA FindFileData;
  std::vector<std::string> temp;
  HANDLE hFind = FindFirstFile(L"models/*", &FindFileData);
  if(hFind == INVALID_HANDLE_VALUE){
    return temp;
  } 
  else while (FindNextFile(hFind, &FindFileData))
  {
	 
		  std::wstring ws(FindFileData.cFileName);
		   std::string temp2(ws.begin(),ws.end());
		   if(temp2 != ".." && temp2 != ".") temp.push_back(temp2);;
	
  };
  FindClose(hFind);
  return temp;
}

Scene::Scene(int Height,int Width,std::string path)
{
	__shader = new Shader();
	pipeline = new ShaderObjectPipeLine();
	__shader->addVertexShader("Shaders/textureShading.vert");
	__shader->addFragmentShader( "Shaders/textureShading.frag");
	__shader->linkShaders();
	__lightingCache.addLight(AmbientLight(Vector3(1,1,1)));
	__lightingCache.addLight(DirectionalLight(BaseLight(Vector3(1,1,1),0.0f),Vector3(1,1,1)));
	__lightingCache.addFog(Fog(0.05f,Vector4(0.5f,0.5f,0.5f,1.0f),500.0f,1000.0f,false));
	__camera = new Camera3d(Vector3(0,100,0),90,Width,Height,0.1f,500000.0f);
	__skybox = new Skybox(*__camera);
	__skybox->loadSkybox("Texture/Skybox/standard/");
	__terrain = NULL;
	if(path != "none")
	{
		readFile(path);
	}
	if(__terrain == NULL)
		__terrain = new Terrain("Texture/firstheightmap.jpg","Texture/Unbenannt.bmp",5.0f,1.0f,true);
}




void Scene::update()
{
	static int counter = 0;
	counter++;
	int i = 0;
}

void Scene::renderScene()
{
	__skybox->renderSkybox(); //temporär
	__shader->use();
	__camera->update(__shader);
	__lightingCache.getAmbientLight().update(__shader);
	__lightingCache.getDirectionalLight().update(__shader);
	//__shader->updateFog(&__lightingCache.getFog());
	PointLight::update(__shader,__lightingCache.getPointLights());
	SpotLight::update(__shader,__lightingCache.getSpotLights());
	pipeline->renderBatches(__shader);
	__terrain->render(__shader);
	__shader->unuse(); //theoretisch nicht benötigt 
};


Scene::~Scene(void)
{
	for(auto &j = __objects.begin();j != __objects.end();j++)
	{
		deleteObject(j->first);
		j = __objects.begin();
	}
	delete(__camera,__shader,__skybox,__terrain);
}

void Scene::saveFile(std::string name)
{ 
	std::ofstream myfile;
	myfile.open (name);
	myfile << "//File created by Simple Engine for Scenes//\n";
	myfile << "//Objects "<<__objects.size()<<std::endl;
	for(auto i = __objects.begin();i != __objects.end();i++)
	{
		myfile <<"o "<<i->second->getName()<< " " << i->second->mesh->getPath() << " "<< i->second->material->texture.texturepath<< " "
			<< i->second->transform->getPos().x << " "<< i->second->transform->getPos().y << " " << i->second->transform->getPos().z <<" " 		 
			<< i->second->transform->getRot().x << " "<< i->second->transform->getRot().y << " " << i->second->transform->getRot().z <<" "
			<< i->second->transform->getScale().x << " "<< i->second->transform->getScale().y << " " << i->second->transform->getScale().z <<" "
			<< i->second->material->getColor().x << " "<< i->second->material->getColor().y << " " << i->second->material->getColor().z <<" \n" ;
	}
	std::vector<PointLight> temp = __lightingCache.getPointLights();
	for(unsigned int i = 0 ; i < __lightingCache.getCount(Point);i++)
	{
		myfile	<< "l " << temp[i].pos.x<<" "<< temp[i].pos.y<<" " << temp[i].pos.z << " "
				<< temp[i].base.getColor().x <<" " << temp[i].base.getColor().y<< " "<<temp[i].base.getColor().z << " "<< temp[i].base.getIntensity()<<" " 
				<< temp[i].attenuation.getConstant() << " " << temp[i].attenuation.getLinear()<< " " <<temp[i]. attenuation.getExponent() << " " 
				<< temp[i].range<<   " \n";
	}
	myfile << "t "<< __terrain->getPath()<< " " <<__terrain->getMaterial()->texture.texturepath<<" "<< __terrain->getPixelSize().x<< " "<< __terrain->getPixelSize().y <<" "<< __terrain->isCentered()<< " \n";
	myfile.close();
}

bool Scene::readFile(std::string Path)
{
	std::cout << "Loading Scene" << Path<<std::endl;
	std::ifstream encodefile(Path.c_str());
    std::vector<std::string> lines;
    if(!encodefile)
    {
        std::cout<<Path<< " could not be read"<<std::endl;
        return 0;
    }
    std::string line;
    while(std::getline(encodefile, line))
    {
		if(line.find("//") == std::string::npos)
		split(line,' ',lines);
    }
	try
	{
		if(validateScene(lines))
		{
			for(unsigned int i = 0; i < lines.size();i++)
			{
				if(lines[i] == "o")
				{
					if(i + 15 < lines.size())
					{
						std::vector<std::string> temp;
						for(int j = 1;j < OBJECTSIZE;j++)
						{
							temp.push_back(lines[i+j]);
						}
						parseObject(temp);
					}
					else
					{
						std::cout << "error in scene object loading \n";
					}
				}
				else if(lines[i] == "l")
				{
					if(i + 11 < lines.size())
					{
						std::vector<std::string> temp;
						for(int j = 1;j < POINTLIGHTSIZE;j++)
						{
							temp.push_back(lines[i+j]);
						}
						parsePointLight(temp);
					}
					else
					{
						std::cout << "error in scene light loading \n";
					}
				}
				else if(lines[i] == "t")
				{
					if(i + 4 < lines.size())
					{
						std::vector<std::string> temp;
						for(int j = 1;j < TERRAINSIZE;j++)
						{
							temp.push_back(lines[i+j]);
						}
						parseTerrain(temp);
					}
					else
					{
						std::cout << "error in scene terrain loading \n";
					}
				}
			}
			return 1;
		}
		else 
		{
			std::cout << "scene invalid"<<std::endl;
		}return 0;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
		return 0;
	}
	
};

void Scene::parseObject(std::vector<std::string>& lines)
{
	addObject(lines[0],lines[1],
						Vector3(atof(lines[3].c_str()),atof(lines[4].c_str()),atof(lines[5].c_str())),
						Vector3(atof(lines[6].c_str()),atof(lines[7].c_str()),atof(lines[8].c_str())),
						Vector3(atof(lines[9].c_str()),atof(lines[10].c_str()),atof(lines[11].c_str())),
						lines[2],
						Vector3(atof(lines[12].c_str()),atof(lines[13].c_str()),atof(lines[14].c_str())));
}
void Scene::parsePointLight(std::vector<std::string>& lines)
{
	__lightingCache.addLight(PointLight(
										Vector3(atof(lines[0].c_str()),atof(lines[1].c_str()),atof(lines[2].c_str())),
										BaseLight(Vector3(atof(lines[3].c_str()),atof(lines[4].c_str()),atof(lines[5].c_str())),atof(lines[6].c_str())),
										Attenuation(atof(lines[7].c_str()),atof(lines[8].c_str()),atof(lines[9].c_str())),
										atof(lines[10].c_str())
										));
}
void Scene::parseTerrain(std::vector<std::string>& lines)
{
	if(__terrain)
	{
		std::cout<<"Terrain allready built. Deleting and Replacing"<<std::endl;
		delete(__terrain);
	}
	__terrain = new Terrain(lines[0],lines[1],atof(lines[2].c_str()),atof(lines[3].c_str()),(bool)atoi(lines[4].c_str()));
}
Object* Scene::addObject(std::string Name,std::string Objectpath,Vector3 pos,Vector3 rot,Vector3 skal,std::string texturepath,Vector3 color,bool autoCenter)
{
	Object* temp = new Object(Name,Objectpath,pos,rot,skal,texturepath,color);
	__objects.insert(std::make_pair(temp->getID(),temp)); //store objects in vector
	pipeline->addObject(temp); //render object
	return temp;
};
void Scene::addObject(Object* object)
{
	__objects.insert(std::make_pair(object->getID(),object)); //store objects in vector
	pipeline->addObject(object); //render object
}
void Scene::deleteObject(int ID)
{
	auto temp = __objects.find(ID);
	if(temp != __objects.end())
	{
		pipeline->deleteObject(temp->second);
		delete(temp->second);
		__objects.erase(ID);
	}

}

void Scene::addPrimitive(Primitives newPrimitive)
{
	switch(newPrimitive)
	{
		case Box:
		{
			addObject("box","Models/Box.obj");
		}break;
		case Cone:
		{
			addObject("Cone","Models/Cone.obj");

		}break;
		case Cylinder:
		{
			addObject("Cylinder","Models/Cylinder.obj");
		}break;
		case Pyramide:
		{
			addObject("Pyramide","Models/Pyramide.obj");
		}break;
		default:
		{
			std::cout <<"unknown Primitive type"<<std::endl;
		}
	}
}

const Object* Scene::getObject(int ID)
{
	auto temp = __objects.find(ID);
	if(temp != __objects.end())
	{
		return temp->second;
	}
	else
	{
		return NULL;
	}
}


void Scene::split(const std::string& s, char c, std::vector<std::string>& v)
{
   std::string::size_type i = 0;
   std::string::size_type j = s.find(c);

   while (j != std::string::npos)
   {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);
	  if (j == std::string::npos && s.substr(i, s.length()).length() != 0)
         v.push_back(s.substr(i, s.length()));
   }
}

bool Scene::validateScene(std::vector<std::string> &temp)
{
	int counto = 0;
	int countl = 0;
	int countt = 0;
	for(unsigned int i = 0; i < temp.size();i++)
	{
		if(temp[i] == "o") counto++;
		if(temp[i] == "l") countl++;
		if(temp[i] == "t") countt++;
	};
	if((counto * OBJECTSIZE + countl * POINTLIGHTSIZE + countt * TERRAINSIZE) == temp.size())
	{
		return true;
	}
	return false;
}


