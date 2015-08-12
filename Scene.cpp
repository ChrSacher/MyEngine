#include "Scene.h"
#define TERRAINSIZE 6
#define OBJECTSIZE 17
#define POINTLIGHTSIZE 12

std::vector<std::string> find_files()
{

  WIN32_FIND_DATA FindFileData;
  std::vector<std::string> temp;
  HANDLE hFind = FindFirstFile(L"res/models/*", &FindFileData);
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
void Scene::parseData(SceneDetails &Data)
{
		objects = Data.objects;
		skybox = Data.skybox;
		camera = Data.camera;
		shader = Data.shader;
		terrain = Data.terrain;
		lightingCache = Data.lightingCache;
		pipeline  = Data.pipeline;
		objectCount = Data.objectCount;
}

Scene::Scene(int Height,int Width,std::string path)
{
		parseData(loader.loadScene(Height,Width,path));	
		fbo = new FBO();
		fbo->init(Vector2(camera->getSize().x,camera->getSize().y));
}

Ray Scene::getClick(int x,int y)
{
	int i = 0;
	fbo->activate();
	i += pipeline->renderColor(shader,camera,Vector3(0.2,0.2,0.2));
	glFinish();
	//now read the stuff under pixel
	//glReadPixels(x,y,camera->getSize().x,camera->getSize().y,GL_RGB,GL_FLOAT,NULL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, camera->getSize().x, camera->getSize().y);
	return Ray();
}







SceneDetails SceneLoader::loadScene(int Height,int Width,std::string path)
{
	data.shader = new Shader();
	data.pipeline = new ShaderObjectPipeLine();
	data.shader->addVertexShader("res/Shaders/textureShading.vert");
	data.shader->addFragmentShader( "res/Shaders/textureShading.frag");
	data.shader->linkShaders();
	data.lightingCache.init();
	data.lightingCache.addLight(AmbientLight(Vector3(1,1,1)));
	data.lightingCache.addLight(DirectionalLight(BaseLight(Vector3(1,1,1),0.0f),Vector3(1,1,1)));
	data.lightingCache.addFog(Fog(0.05f,Vector4(0.5f,0.5f,0.5f,1.0f),500.0f,1000.0f,false));
	data.camera = new Camera3d(Vector3(0,0,0),70,Width,Height,0.1f,1000.0f);
	data.skybox = new Skybox(data.camera);
	data.skybox->loadSkybox("res/Texture/Skybox/standard/");
	data.terrain = NULL;
	data.objectCount = 0;
	if(path != "none")
	{
		readFile(path);
	}
	if(data.terrain == NULL)
		data.terrain = new Terrain("res/Texture/standardTerrain.png","res/Texture/White.png",5.0f,1.0f,true);
	return data;
}


void Scene::update(float delta)
{
	static int counter = 0;
	counter++;
	
	lightingCache.getSpotLights()[0].getPointLight().pos = camera->getPos();
	lightingCache.getSpotLights()[0].setDir(camera->getDir().normalize());
}

void Scene::renderScene()
{
	skybox->renderSkybox(); //temporär
	
	shader->use();
	camera->update(shader);
	lightingCache.getAmbientLight().update(shader);
	lightingCache.getDirectionalLight().update(shader);
	//__shader->updateFog(&__lightingCache.getFog());
	PointLight::update(shader,lightingCache.getPointLights());
	SpotLight::update(shader,lightingCache.getSpotLights());
	pipeline->renderBatches(shader,camera);
	terrain->render(shader);
	shader->unuse(); //theoretisch nicht benötigt 
	lightingCache.draw(camera);
};


Scene::~Scene(void)
{
	for(auto &j = objects.begin();j != objects.end();j++)
	{
		deleteObject(j->first);
		j = objects.begin();
	}
	delete(camera,shader,skybox,terrain);
	camera =NULL;shader = NULL;skybox =NULL; terrain = NULL;
}

void Scene::saveFile(std::string name)
{
	SceneDetails temp;
	temp.objects = objects;
	temp.terrain = terrain;
	temp.lightingCache = lightingCache;
	loader.saveFile(name,temp);
}


std::vector<Object*> Scene::getObjectVector()
{
	std::vector<Object*> returnV;
	for(auto &j = objects.begin();j != objects.end();j++)
	{
		returnV.push_back(j->second);
	}
	return returnV;
}

std::vector<Object*> Scene::getObjectsOnRay(Ray ray)
{
	return RayTracer::getObjectsOnRay(getObjectVector(),camera,ray);
}
Object* Scene::getFirstObjectOnRay(Ray ray)
{
	return RayTracer::getFirstObjectOnRay(getObjectVector(),camera,ray);
}

void Scene::pick(int x,int y )
{
	picker.pick(x,y,getObjectVector(),camera);
}













void  SceneLoader::saveFile(std::string name,SceneDetails& Data)
{ 
	std::ofstream myfile;
	myfile.open (name);
	myfile << "//File created by Simple Engine for Scenes//\n";
	myfile << "//Objects "<<Data.objects.size()<<std::endl;
	for(auto i = Data.objects.begin();i != Data.objects.end();i++)
	{
		myfile <<"o "<<i->second->getName()<< " " << i->second->mesh->getPath() << " "<< i->second->material->texture.texturepath<< " "
			<< i->second->transform->getPos().x << " "<< i->second->transform->getPos().y << " " << i->second->transform->getPos().z <<" " 		 
			<< i->second->transform->getRot().x << " "<< i->second->transform->getRot().y << " " << i->second->transform->getRot().z <<" "
			<< i->second->transform->getScale().x << " "<< i->second->transform->getScale().y << " " << i->second->transform->getScale().z <<" "
			<< i->second->material->getColor().x << " "<< i->second->material->getColor().y << " " << i->second->material->getColor().z <<" "
			<< i->second->material->normalMap.texturepath <<" \n" ;
	}
	std::vector<PointLight> temp = Data.lightingCache.getPointLights();
	for(unsigned int i = 0 ; i < Data.lightingCache.getCount(Point);i++)
	{
		myfile	<< "l " << temp[i].pos.x<<" "<< temp[i].pos.y<<" " << temp[i].pos.z << " "
				<< temp[i].base.getColor().x <<" " << temp[i].base.getColor().y<< " "<<temp[i].base.getColor().z << " "<< temp[i].base.getIntensity()<<" " 
				<< temp[i].attenuation.getConstant() << " " << temp[i].attenuation.getLinear()<< " " <<temp[i]. attenuation.getExponent() << " " 
				<< temp[i].range<<   " \n";
	}
	myfile << "t "<< Data.terrain->getPath()<< " " <<Data.terrain->getMaterial()->texture.texturepath<<" "<< Data.terrain->getPixelSize().x<< " "<< Data.terrain->getPixelSize().y <<" "<< Data.terrain->isCentered()<< " \n";
	myfile.close();
}

bool  SceneLoader::readFile(std::string Path)
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


void  SceneLoader::parseObject(std::vector<std::string>& lines)
{
	addObject(lines[0],lines[1],
						Vector3(atof(lines[3].c_str()),atof(lines[4].c_str()),atof(lines[5].c_str())),
						Vector3(atof(lines[6].c_str()),atof(lines[7].c_str()),atof(lines[8].c_str())),
						Vector3(atof(lines[9].c_str()),atof(lines[10].c_str()),atof(lines[11].c_str())),
						lines[2],
						Vector3(atof(lines[12].c_str()),atof(lines[13].c_str()),atof(lines[14].c_str())),
						lines[15]);
}
void  SceneLoader::parsePointLight(std::vector<std::string>& lines)
{
	data.lightingCache.addLight(PointLight(
										Vector3(atof(lines[0].c_str()),atof(lines[1].c_str()),atof(lines[2].c_str())),
										BaseLight(Vector3(atof(lines[3].c_str()),atof(lines[4].c_str()),atof(lines[5].c_str())),atof(lines[6].c_str())),
										Attenuation(atof(lines[7].c_str()),atof(lines[8].c_str()),atof(lines[9].c_str())),
										atof(lines[10].c_str())
										));
}
void  SceneLoader::parseTerrain(std::vector<std::string>& lines)
{
	if(data.terrain)
	{
		std::cout<<"Terrain allready built. Deleting and Replacing"<<std::endl;
		delete(data.terrain);
		data.terrain = NULL;
	}
	data.terrain = new Terrain(lines[0],lines[1],atof(lines[2].c_str()),atof(lines[3].c_str()),(bool)atoi(lines[4].c_str()));
}
Object* Scene::addObject(std::string Name,std::string Objectpath,Vector3 pos,Vector3 rot,Vector3 skal,std::string texturepath,Vector3 color,std::string normalMap,bool autoCenter)
{
	Object* temp = new Object(Name,Objectpath,pos,rot,skal,texturepath,color,normalMap);
	objects.insert(std::make_pair(temp->getID(),temp)); //store objects in vector
	pipeline->addObject(temp); //render object
	objectCount++;
	return temp;
};
Object* SceneLoader::addObject(std::string Name,std::string Objectpath,Vector3 pos,Vector3 rot,Vector3 skal,std::string texturepath,Vector3 color,std::string normalMap,bool autoCenter)
{
	Object* temp = new Object(Name,Objectpath,pos,rot,skal,texturepath,color,normalMap);
	data.objects.insert(std::make_pair(temp->getID(),temp)); //store objects in vector
	data.pipeline->addObject(temp); //render object
	data.objectCount++;
	return temp;
};
void Scene::addObject(Object* object)
{
	objects.insert(std::make_pair(object->getID(),object)); //store objects in vector
	pipeline->addObject(object); //render object
	objectCount++;
}
void Scene::deleteObject(int ID)
{
	auto temp = objects.find(ID);
	if(temp != objects.end())
	{
		pipeline->deleteObject(temp->second);
		delete(temp->second);
		temp->second = NULL;
		objects.erase(ID);
	}
	objectCount--;
}

void Scene::addPrimitive(Primitives newPrimitive)
{
	switch(newPrimitive)
	{
		case Box:
		{
			addObject("box","res/Models/Box.obj");
		}break;
		case Cone:
		{
			addObject("Cone","res/Models/Cone.obj");

		}break;
		case Cylinder:
		{
			addObject("Cylinder","res/Models/Cylinder.obj");
		}break;
		case Pyramide:
		{
			addObject("Pyramide","res/Models/Pyramide.obj");
		}break;
		default:
		{
			std::cout <<"unknown Primitive type"<<std::endl;
		}
	}
}

const Object* Scene::getObject(int ID)
{
	auto temp = objects.find(ID);
	if(temp != objects.end())
	{
		return temp->second;
	}
	else
	{
		return NULL;
	}
}


void  SceneLoader::split(const std::string& s, char c, std::vector<std::string>& v)
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

bool  SceneLoader::validateScene(std::vector<std::string> &temp)
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




