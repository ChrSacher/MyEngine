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

bool STB(std::string temp)
{
	std::transform(temp.begin(), temp.end(), temp.begin(), toupper);
	if(temp.find("T") != temp.npos || temp.find("R") != temp.npos || temp.find("U") != temp.npos) return true;
	return false;
}
Vector3 stV(std::string s1,std::string s2,std::string s3)
{
	return Vector3(atof(s1.c_str()),atof(s2.c_str()),atof(s3.c_str()));
}
void  split(const std::string& s, char c, std::vector<std::string>& v)
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

Scene* Scene::createScene(int Height,int Width,std::string path)
{
		Scene* scene = new Scene();	
		scene->deferedShader = new GBuffer();
		scene->deferedShader->Init(Height,Width);
		scene->shader = new Shader();
		scene->shader->addVertexShader("res/Shaders/textureShading.vert");
		scene->shader->addFragmentShader( "res/Shaders/textureShading.frag");
		scene->shader->linkShaders();
		scene->instancedShader = new Shader();
		scene->instancedShader->addVertexShader("res/Shaders/textureInstancedShading.vert");
		scene->instancedShader->addFragmentShader( "res/Shaders/textureShading.frag");
		scene->instancedShader->linkShaders();
		scene->camera = new Camera3d(Vector3(0,0,0),70,Width,Height,0.1f,1000.0f);
		scene->skybox = new Skybox(scene->camera);
		scene->skybox->loadSkybox("res/Texture/Skybox/standard/");
		scene->entityCount = 0;
		if(path != "none")
		{
			scene->loadScene(path);
		}
		else
		{
			Entity* terrainEntity = Entity::create("Terrain");
			terrainEntity->addComponent(&ComponentManager::get().createTerrain("res/Texture/standardTerrain.png","res/Texture/White.png",Vector3(1,1,1),true));
			scene->addEntity(terrainEntity);
		}
	return scene;


		scene->fbo = new FBO();
		scene->fbo->init(Vector2(scene->camera->getSize().x,scene->camera->getSize().y));
		return scene;
}

void Scene::deleteScene(Scene* deleteScene)
{
	for(auto &j = deleteScene->entities.begin();j != deleteScene->entities.end();)
	{	
		
		deleteScene->deleteEntity(j->first);
		j = deleteScene->entities.begin();
	}
	delete(deleteScene->camera,deleteScene->shader,deleteScene->skybox);
	deleteScene->camera =NULL;deleteScene->shader = NULL;deleteScene->skybox =NULL;
	delete(deleteScene);
}
Ray Scene::getClick(int x,int y)
{
	/*int i = 0;
	fbo->activate();
	//i += pipeline->renderColor(shader,camera,Vector3(0.2,0.2,0.2));//render objects in color
	glFinish();
	//now read the stuff under pixel
	//glReadPixels(x,y,camera->getSize().x,camera->getSize().y,GL_RGB,GL_FLOAT,NULL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, camera->getSize().x, camera->getSize().y);*/
	return Ray();
}

void Scene::update(float delta)
{
	static int counter = 0;
	counter++;
	ComponentManager::get().update(delta);
}

void Scene::renderScene()
{
	drawnEntities = 0;
	skybox->renderSkybox(); //temporär
	shader->use();
	camera->update(shader);
	ComponentManager::get().render(shader,camera);
	glFinish();
}; 

void Scene::saveFile(std::string name)
{
	std::ofstream myFile;
	myFile.open (name);
	for(auto &it = entities.begin(); it != entities.end(); it++)
	{
		myFile << it->second->saveScene();
	}
}

void Scene::addEntity(Entity* object)
{
	entities.insert(std::make_pair(object->getID(),object)); //store objects in vecto
	entityCount++;
}

void Scene::deleteEntity(int ID)
{
	auto temp = entities.find(ID);
	if(temp != entities.end())
	{
		//delete(temp->second);
		temp->second = NULL;
		entities.erase(ID);
	}
	entityCount--;
}


const Entity* Scene::getEntity(int ID)
{
	auto temp = entities.find(ID);
	if(temp != entities.end())
	{
		return temp->second;
	}
	else
	{
		return NULL;
	}
}

Scene::~Scene(void)
{

}

std::vector<Entity*> Scene::getEntityVector()
{
	std::vector<Entity*> returnV;
	for(auto &j = entities.begin();j != entities.end();j++)
	{
		returnV.push_back(j->second);
	}
	return returnV;
}

std::vector<Entity*> Scene::getEntityOnRay(Ray ray)
{
	return RayTracer::getEntitiesOnRay(getEntityVector(),camera,ray);
}
Entity* Scene::getFirstEntityOnRay(Ray ray)
{
	return RayTracer::getFirstEntityOnRay(getEntityVector(),camera,ray);
}

void Scene::pick(int x,int y )
{
	picker.pick(x,y,getEntityVector(),camera);
}














bool Scene::loadScene(std::string Path)
{
	std::cout << "Loading Scene" << Path<<std::endl;
	std::ifstream encodefile(Path.c_str());
    
	std::map<std::string,ComponentType> type;
	type.insert(std::make_pair("E",ENTITY));
	type.insert(std::make_pair("G",GRAPHICS));
	type.insert(std::make_pair("P",PHYSICS));
	type.insert(std::make_pair("C",COLLISIONS));
	type.insert(std::make_pair("A",AMBIENT));
	type.insert(std::make_pair("D",DIRECTIONAL));
	type.insert(std::make_pair("T",TERRAIN));
    if(!encodefile)
    {
        std::cout<<Path<< " could not be read"<<std::endl;
        return 0;
    }
    std::string line;
	Entity* currentEntity = NULL;
    while(std::getline(encodefile, line))
    {
		auto pos = line.rfind("//");
		if (pos != std::string::npos)
		{
			line.erase(pos);
		}
		std::vector<std::string> lines;
		split(line,' ',lines);
			ComponentType temp = NUMCOMPONENT;
			if(lines.size() == 0) continue;
		auto it = type.find(lines[0]);
		if(it != type.end()) temp = it->second; 
		switch(temp)
		{
			case ENTITY:
			{
				if(lines.size() == 11)
				{
					if(currentEntity  != NULL) addEntity(currentEntity);
					currentEntity = Entity::create(	lines[1],
													stV(lines[2],lines[3],lines[4]),
													stV(lines[5],lines[6],lines[7]),
													stV(lines[8],lines[9],lines[10]));
					std::cout<<"Loading Entity"<<std::endl;
				}
				else std::cout<<"Broken Scene Line"<<std::endl;
				
			}break;
			case GRAPHICS:
			{
				if(currentEntity != NULL)
				{
					if(lines.size() == 7)
					{
						currentEntity->addComponent(&ComponentManager::get().createGraphics(lines[1],lines[2],lines[3],stV(lines[4],lines[5],lines[6])));
						std::cout<<"Loading Graphic"<<std::endl;
					}
					else std::cout<<"Broken Scene Line"<<std::endl;
				}
			}break;
			case PHYSICS:
			{
			}break;
			case TERRAIN:
			{
				if(currentEntity != NULL)
				{
					if(lines.size() == 7)
					{
						currentEntity->addComponent(&ComponentManager::get().createTerrain(lines[1],lines[2],stV(lines[3],lines[4],lines[5]),STB(lines[6])));
						std::cout<<"Loading Terrain"<<std::endl;
					}
					else std::cout<<"Broken Scene Line"<<std::endl;
				}
			}break;
			case AMBIENT:
			{
					if(lines.size() == 4)
					{
						currentEntity->addComponent(&ComponentManager::get().createAmbient(stV(lines[1],lines[2],lines[3])));
						std::cout<<"Loading Ambient"<<std::endl;
					}
					else std::cout<<"Broken Scene Line"<<std::endl;
			};break;
			case COLLISIONS:
			{

			}break;
			case DIRECTIONAL:
			{
					if(lines.size() == 8)
					{
						currentEntity->addComponent(&ComponentManager::get().createDirectional(stV(lines[1],lines[2],lines[3]),atof(lines[4].c_str()),stV(lines[5],lines[6],lines[7])));
						std::cout<<"Loading Directional"<<std::endl;
					}
					else std::cout<<"Broken Scene Line"<<std::endl;
			};break;
			default:
			{

			}break;
		}
	}
	return true;
};










