#include "Scene.h"


Scene::Scene(int Height,int Width,std::string path)
{
	__shader = new Shader();
	__shader->addVertexShader("Shaders/textureShading.vert");
	__shader->addFragmentShader( "Shaders/textureShading.frag");
	__shader->linkShaders();
	addLight(AmbientLight(Vector3(1,1,1)));
	addLight(DirectionalLight(BaseLight(Vector3(1,1,1),0.0f),Vector3(1,1,1)));
	addFog(Fog(0.05f,Vector4(0.5f,0.5f,0.5f,1.0f),500.0f,1000.0f,false));
	__camera = new Camera3d(Vector3(0,2,0),90,Width,Height,0.1f,500.0f);
	__skybox = new Skybox(*__camera);
	__skybox->loadSkybox("Texture/Skybox/standard/");
	if(path != "none")
	{
		readFile(path);
	}
}


Scene::~Scene(void)
{
	for(auto &j = __objects.begin();j != __objects.end();j++)
	{
		deleteObject(j->first);
		j = __objects.begin();
	}
	delete(__camera,__shader,__skybox);
}

void Scene::saveFile(std::string name)
{ 
	std::ofstream myfile;
	myfile.open (name);
	myfile << "//File created by Simple Engine for Scenes//\n";
	for(auto i = __objects.begin();i != __objects.end();i++)
	{
		myfile <<"o "<<i->second->getName()<< " " << i->second->mesh->getPath() << " "<< i->second->material->texture.texturepath<< " "
			<< i->second->transform->getPos().x << " "<< i->second->transform->getPos().y << " " << i->second->transform->getPos().z <<" " 		 
			<< i->second->transform->getRot().x << " "<< i->second->transform->getRot().y << " " << i->second->transform->getRot().z <<" "
			<< i->second->transform->getScale().x << " "<< i->second->transform->getScale().y << " " << i->second->transform->getScale().z <<" "
			<< i->second->material->getColor().x << " "<< i->second->material->getColor().y << " " << i->second->material->getColor().z <<" \n" ;
	}
	std::vector<PointLight> temp = getPointLights();
	for(unsigned int i = 0 ; i < getCount(Point);i++)
	{
		myfile	<< "l " << temp[i].pos.x<<" "<< temp[i].pos.y<<" " << temp[i].pos.z << " "
				<< temp[i].base.getColor().x <<" " << temp[i].base.getColor().y<< " "<<temp[i].base.getColor().z << " "<< temp[i].base.getIntensity()<<" " 
				<< temp[i].attenuation.getConstant() << " " << temp[i].attenuation.getLinear()<< " " <<temp[i]. attenuation.getExponent() << " " 
				<< temp[i].range<<   " \n";
	}
	myfile.close();
}

bool Scene::readFile(std::string Path)
{
	printf("Loading Scene file %s...\n", Path.c_str());
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
					addObject(lines[i+1],lines[i+2],
						Vector3(atoi(lines[i+4].c_str()),atoi(lines[i+5].c_str()),atoi(lines[i+6].c_str())),
						Vector3(atoi(lines[i+7].c_str()),atoi(lines[i+8].c_str()),atoi(lines[i+9].c_str())),
						Vector3(atoi(lines[i+10].c_str()),atoi(lines[i+11].c_str()),atoi(lines[i+12].c_str())),
						lines[i + 3],
						Vector3(atoi(lines[i+13].c_str()),atoi(lines[i+14].c_str()),atoi(lines[i+15].c_str())));
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
					addLight(PointLight(
										Vector3(atoi(lines[i+1].c_str()),atoi(lines[i+2].c_str()),atoi(lines[i+3].c_str())),
										BaseLight(Vector3(atoi(lines[i+4].c_str()),atoi(lines[i+5].c_str()),atoi(lines[i+6].c_str())),atoi(lines[i+7].c_str())),
										Attenuation(atoi(lines[i+8].c_str()),atoi(lines[i+9].c_str()),atoi(lines[i+10].c_str())),
										atoi(lines[i+11].c_str())
										));
					}
					else
					{
						std::cout << "error in scene light loading \n";
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
void Scene::renderScene()
{
	__skybox->renderSkybox(); //temporär
	__shader->use();
	__shader->updateCamera(__camera);
	__shader->updateAmbientLight(&getAmbientLight());
	__shader->updateDirectionLight(&getDirectionalLight());
	__shader->updateFog(&getFog());
	__shader->updatePointLights(getPointLights());
	__shader->updateSpotLights(getSpotLights());
	__shader->renderBatch();
	__shader->unuse(); //theoretisch nicht benötigt 
};

void Scene::addObject(std::string Name,std::string Objectpath,Vector3 pos,Vector3 rot,Vector3 skal,std::string texturepath,Vector3 color,bool autoCenter)
{
	Object* temp = new Object(Name,Objectpath,pos,rot,skal,texturepath,color);
	__objects.insert(std::make_pair(temp->getID(),temp)); //store objects in vector
	__shader->addObject(temp); //render object
};

void Scene::deleteObject(int ID)
{
	auto temp = __objects.find(ID);
	if(temp != __objects.end())
	{
		__shader->deleteObject(temp->second);
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

void Scene::update()
{
	static int counter = 0;
	counter++;
	int i = 0;
	for(auto &j = __objects.begin();j != __objects.end();j++)
	{
		i++;
		j->second->transform->setPos(Vector3((float)sin(counter   * 3.14/180) * i,(float)i,0.0f)); //Draw objects
	};
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
	for(unsigned int i = 0; i < temp.size();i++)
	{
		if(temp[i] == "o") counto++;
		if(temp[i] == "l") countl++;
	};
	if((counto * 16 + countl * 12) == temp.size())
	{
		return true;
	}
	return false;
}