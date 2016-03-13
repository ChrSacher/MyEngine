#include "Scene.h"
#include "ServiceLocator.h"
#include <libconfig\libconfig.hh>
#define TERRAINSIZE 6
#define OBJECTSIZE 17
#define POINTLIGHTSIZE 12

char Scene::seperatingCharacter = ',';

std::string BTS(bool temp)
{
	if (temp) return "true";
	return "false";
}

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
	return Vector3(std::stof(s1.c_str()), std::stof(s2.c_str()), std::stof(s3.c_str()));
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
		scene->entityCount = 0;
		if(path != "none")
		{
			scene->loadScene(path);
		}
		else
		{
			Entity* terrainEntity = ServiceLocator::getEM().create("Terrain");
			terrainEntity->addComponent(ServiceLocator::getCM().createTerrain("res/Texture/standardTerrain.png","res/Texture/White.png",Vector3(1,1,1),true));
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
	delete(deleteScene->camera,deleteScene->shader);
	deleteScene->camera =NULL;deleteScene->shader = NULL;
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

void Scene::update()
{
	static int counter = 0;
	counter++;
	
}

void Scene::renderScene()
{
	drawnEntities = 0;
	shader->use();
	camera->update(shader);
	ServiceLocator::getCM().render(shader,camera);
	ServiceLocator::getPE().world->debugDrawWorld();
	glFinish();
}; 

void Scene::saveFile(std::string name)
{
	using namespace libconfig;
	Config config;

	Setting& root = config.getRoot();
	root.add("numEntities", Setting::TypeInt) = (int)entities.size();
	int i = 0;
	for (auto &it = entities.begin(); it != entities.end(); it++)
	{
		Entity* ent = it->second;
		Setting &current = root.add("Entity" + std::to_string(i++), Setting::TypeGroup);

		current.add("Name", Setting::TypeString) = ent->getName();
		Setting& pos = current.add("Position", Setting::TypeArray);
		pos.add(Setting::TypeFloat) = ent->transform.getPos().x;
		pos.add(Setting::TypeFloat) = ent->transform.getPos().y;
		pos.add(Setting::TypeFloat) = ent->transform.getPos().z;
		Setting& rot = current.add("Rotation", Setting::TypeArray);
		rot.add(Setting::TypeFloat) = ent->transform.getRot().x;
		rot.add(Setting::TypeFloat) = ent->transform.getRot().y;
		rot.add(Setting::TypeFloat) = ent->transform.getRot().z;
		Setting& scale = current.add("Scale", Setting::TypeArray);
		scale.add(Setting::TypeFloat) = ent->transform.getScale().x;
		scale.add(Setting::TypeFloat) = ent->transform.getScale().y;
		scale.add(Setting::TypeFloat) = ent->transform.getScale().z;
		current.add("numComponents", Setting::TypeInt) = (int)ent->components.size();
		int j = 0;
		for (unsigned int i = 0; i < ent->components.size(); i++)
		{
			switch (ent->components[i]->type)
			{
			case GRAPHICS:
			{
				auto* s = ent->components[i]->getT<GraphicsComponent*>();
				Setting& comp = current.add("Component" + std::to_string(j++), Setting::TypeGroup);
				comp.add("Type", Setting::TypeString) = "Graphics";
				comp.add("Texturepath", Setting::TypeString) = s->material.texture.texturepath;
				comp.add("Normalpath" , Setting::TypeString) = s->material.normalMap.texturepath;
				comp.add("Meshpath", Setting::TypeString) = s->mesh.getPath();
				Setting& color = comp.add("Color", Setting::TypeArray);
				color.add(Setting::TypeFloat) = s->material.color.x;
				color.add(Setting::TypeFloat) = s->material.color.y;
				color.add(Setting::TypeFloat) = s->material.color.z;
			}break;
			case AMBIENT:
			{
				auto* s = ent->components[i]->getT<AmbientLightComponent*>();
				Setting& comp = current.add("Component" + std::to_string(j++), Setting::TypeGroup);
				comp.add("Type", Setting::TypeString) = "Ambient";
				Setting& color = comp.add("Color", Setting::TypeArray);
				color.add(Setting::TypeFloat) = s->ambient.getAmbient().x;
				color.add(Setting::TypeFloat) = s->ambient.getAmbient().y;
				color.add(Setting::TypeFloat) = s->ambient.getAmbient().z;
				
				
			}; break;
			case TERRAIN:
			{
				auto* s = ent->components[i]->getT<TerrainComponent*>();
				Setting& comp = current.add("Component" + std::to_string(j++), Setting::TypeGroup);
				comp.add("Type", Setting::TypeString) = "Terrain";
				comp.add("Terrainpath", Setting::TypeString) = s->terrain.getPath();
				comp.add("Texturepath",Setting::TypeString) = s->terrain.getMaterial()->texture.texturepath;
				Setting& r = comp.add("Scale", Setting::TypeArray);
				r.add(Setting::TypeFloat) = s->terrain.getTransform()->getScale().x;
				r.add(Setting::TypeFloat) = s->terrain.getTransform()->getScale().y;
				r.add(Setting::TypeFloat) = s->terrain.getTransform()->getScale().z;
				comp.add("Centered", Setting::TypeBoolean) = s->terrain.isCentered();
			}; break;
			case DIRECTIONAL:
			{
				auto* s = ent->components[i]->getT<DirectionalLightComponent*>();
				Setting& comp = current.add("Component" + std::to_string(j++), Setting::TypeGroup);
				comp.add("Type", Setting::TypeString) = "Directional";
				Setting& color = comp.add("Color", Setting::TypeArray);
				color.add(Setting::TypeFloat) = s->light.getBaseLight().getColor().x;
				color.add(Setting::TypeFloat) = s->light.getBaseLight().getColor().y;
				color.add(Setting::TypeFloat) = s->light.getBaseLight().getColor().z;
				comp.add("Intensity", Setting::TypeFloat) = s->light.getBaseLight().getIntensity();
				Setting& dir = comp.add("Direction", Setting::TypeArray);
				dir.add(Setting::TypeFloat) = s->light.getDirection().x;
				dir.add(Setting::TypeFloat) = s->light.getDirection().y;
				dir.add(Setting::TypeFloat) = s->light.getDirection().z;

			}; break;
			case SKYBOX:
			{
				auto* s = ent->components[i]->getT<SkyBoxComponent*>();
				auto r = s->skyBox.getDirAndFile();
				Setting& comp = current.add("Component" + std::to_string(j++), Setting::TypeGroup);
				comp.add("Type", Setting::TypeString) = "Skybox";
				Setting& color = comp.add("Color", Setting::TypeArray);
				color.add(Setting::TypeFloat) = s->skyBox.getColor().x;
				color.add(Setting::TypeFloat) = s->skyBox.getColor().y;
				color.add(Setting::TypeFloat) = s->skyBox.getColor().z;
				comp.add("Directory", Setting::TypeString) = r[0];
				Setting& files = comp.add("Files", Setting::TypeArray);
				for (int w = 1; w < 7;w++) files.add(Setting::TypeString) = r[w];
			}; break;
			case PHYSICS:
			{
				auto* s = ent->components[i]->getT<PhysicsComponent*>();
				Setting& comp = current.add("Component" + std::to_string(j++), Setting::TypeGroup);
				comp.add("Type", Setting::TypeString) = "Physics";
				comp.add("Physicstype", Setting::TypeString) = PhysicsData::typeToString(s->data.type);
				comp.add("Mass", Setting::TypeFloat) = std::to_string(s->data.mass);
			}break;
			case AUDIO:
			{
				auto* s = ent->components[i]->getT<AudioComponent*>();
				Setting& comp = current.add("Component" + std::to_string(j++), Setting::TypeGroup);
				comp.add("Type", Setting::TypeString) = "Audio";
				comp.add("Path", Setting::TypeString) = s->path;
				comp.add("Volume", Setting::TypeFloat) = s->sound.getVolume();
				comp.add("is2D", Setting::TypeBoolean) = s->is2D;
			}break;
			case SCRIPT:
			{
				auto* s = ent->components[i]->getT<ScriptComponent*>();
				Setting& comp = current.add("Component" + std::to_string(j++), Setting::TypeGroup);
				comp.add("Type", Setting::TypeString) = "Script";
				comp.add("Path", Setting::TypeString) = s->script.path;
			}; break;
			default:
			{
				
			}break;
			}
		}
	}//remove
	config.writeFile(name.c_str());





	/*std::ofstream myFile;
	myFile.open (name);
	for (auto &it = entities.begin(); it != entities.end(); it++)
	{
	
		Entity* ent = it->second;
		myFile << "E " + ent->getName() + Scene::seperatingCharacter + std::to_string(ent->transform.getPos().x) + Scene::seperatingCharacter + std::to_string(ent->transform.getPos().y) + Scene::seperatingCharacter + std::to_string(ent->transform.getPos().z) + Scene::seperatingCharacter
			+ std::to_string(ent->transform.getRot().x) + Scene::seperatingCharacter + std::to_string(ent->transform.getRot().y) + Scene::seperatingCharacter + std::to_string(ent->transform.getRot().z) + Scene::seperatingCharacter
			+ std::to_string(ent->transform.getScale().x) + Scene::seperatingCharacter + std::to_string(ent->transform.getScale().y) + Scene::seperatingCharacter + std::to_string(ent->transform.getScale().z) + "\n";
		for (unsigned int i = 0; i < ent->components.size(); i++)
		{
			switch (ent->components[i]->type)
			{
				case GRAPHICS:
				{
					auto* s = ent->components[i]->getT<GraphicsComponent*>();
					myFile <<  "G " + s->material.texture.texturepath + Scene::seperatingCharacter + s->material.normalMap.texturepath + Scene::seperatingCharacter + s->mesh.getPath() + Scene::seperatingCharacter + std::to_string(s->material.color.x) + Scene::seperatingCharacter + std::to_string(s->material.color.y) + Scene::seperatingCharacter + std::to_string(s->material.color.z) + "\n";
				}break;
				case AMBIENT:
				{
					auto* s = ent->components[i]->getT<AmbientLightComponent*>();
					myFile<< "A " + std::to_string(s->ambient.getAmbient().x) + Scene::seperatingCharacter + std::to_string(s->ambient.getAmbient().y) + Scene::seperatingCharacter + std::to_string(s->ambient.getAmbient().z) + "\n";
				}; break;
				case TERRAIN:
				{
					auto* s = ent->components[i]->getT<TerrainComponent*>();
					myFile << "T " + s->terrain.getPath() + Scene::seperatingCharacter + s->terrain.getMaterial()->texture.texturepath + Scene::seperatingCharacter + std::to_string(s->terrain.getTransform()->getScale().x) + Scene::seperatingCharacter + std::to_string(s->terrain.getTransform()->getScale().y) + Scene::seperatingCharacter + std::to_string(s->terrain.getTransform()->getScale().z) + Scene::seperatingCharacter + BTS(s->terrain.isCentered()) + "\n";
				}; break;
				case DIRECTIONAL:
				{
					auto* s = ent->components[i]->getT<DirectionalLightComponent*>();
					myFile<< "D " + std::to_string(s->light.getBaseLight().getColor().x) + Scene::seperatingCharacter + std::to_string(s->light.getBaseLight().getColor().y) + Scene::seperatingCharacter + std::to_string(s->light.getBaseLight().getColor().z) + Scene::seperatingCharacter + std::to_string(s->light.getBaseLight().getIntensity()) + Scene::seperatingCharacter
						+ std::to_string(s->light.getDirection().x) + Scene::seperatingCharacter + std::to_string(s->light.getDirection().y) + Scene::seperatingCharacter + std::to_string(s->light.getDirection().z) + "\n";
				}; break;
				case SKYBOX:
				{
					auto* s = ent->components[i]->getT<SkyBoxComponent*>();
					auto r = s->skyBox.getDirAndFile();
					myFile <<  "S " + std::to_string(s->skyBox.getColor().x) + Scene::seperatingCharacter + std::to_string(s->skyBox.getColor().y) + Scene::seperatingCharacter + std::to_string(s->skyBox.getColor().z) + Scene::seperatingCharacter + r[0] + Scene::seperatingCharacter + r[1] + Scene::seperatingCharacter + r[2] + Scene::seperatingCharacter + r[3] + Scene::seperatingCharacter + r[4] + Scene::seperatingCharacter + r[5] + Scene::seperatingCharacter + r[6] + "\n";
				}; break;
				case PHYSICS:
				{
					auto* s = ent->components[i]->getT<PhysicsComponent*>();
					myFile << "P " + PhysicsData::typeToString(s->data.type) + Scene::seperatingCharacter + std::to_string(s->data.mass) + "\n";
				}break;
				case AUDIO:
				{
					auto* s = ent->components[i]->getT<AudioComponent*>();
					myFile << "AU " + BTS(s->is2D) + "  " + s->path +Scene::seperatingCharacter + std::to_string(s->sound.getVolume())+ "\n";
				}break;
				case SCRIPT:
				{
					auto* s = ent->components[i]->getT<ScriptComponent*>();
					myFile << "SC " + s->script.path + "\n";
				}; break;
				default:
				{
					std::cout << "BROKEN SCNE SAVE COMPONENT \n";
				}break;
			}
		}
	}*/
	
}

void Scene::addEntity(Entity* object)
{
	entities.insert(std::make_pair(object->getID(),object)); //store objects in map
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
		entityCount--;
	}
	
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
	picker.pick(camera->getDirClick( x,y));
}













void brokenSceneLine(std::string &path)
{
	std::cout << "Broken Scene Line: " << path << std::endl;
}
bool Scene::loadScene(std::string Path)
{
	std::cout << "Loading Scene" << Path<<std::endl;
	std::ifstream encodefile(Path.c_str());
    
	static std::map<std::string, ComponentType> type{
	std::make_pair("E",ENTITY),
	std::make_pair("G",GRAPHICS),
	std::make_pair("C",COLLISIONS),
	std::make_pair("A",AMBIENT),
	std::make_pair("D",DIRECTIONAL),
	std::make_pair("T",TERRAIN),
	std::make_pair("S",SKYBOX),
	std::make_pair("P", PHYSICS),
	std::make_pair("SC", SCRIPT),
	std::make_pair("AU", AUDIO) };
	

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
		split(line, Scene::seperatingCharacter,lines);
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
					currentEntity = ServiceLocator::getEM().create(	lines[1],
													stV(lines[2],lines[3],lines[4]),
													stV(lines[5],lines[6],lines[7]),
													stV(lines[8],lines[9],lines[10]));
					std::cout<<"Loading Entity"<<std::endl;
				}
				else brokenSceneLine(line);
				
			}break;
			case GRAPHICS:
			{
				if(currentEntity != NULL)
				{
					if(lines.size() == 7)
					{
						currentEntity->addComponent(ServiceLocator::getCM().createGraphics(lines[1],lines[2],lines[3],stV(lines[4],lines[5],lines[6])));
						std::cout<<"Loading Graphic"<<std::endl;
					}
					else std::cout<<"Broken Scene Line"<<std::endl;
				}
			}break;
			case AMBIENT:
			{
					if(lines.size() == 4)
					{
						currentEntity->addComponent(ServiceLocator::getCM().createAmbient(stV(lines[1],lines[2],lines[3])));
						std::cout<<"Loading Ambient"<<std::endl;
					}
					else brokenSceneLine(line);
			};break;
			case TERRAIN:
			{
				if (lines.size() == 7)
				{
					currentEntity->addComponent(ServiceLocator::getCM().createTerrain(lines[1], lines[2], stV(lines[3], lines[4], lines[5]), STB(lines[6])));
					std::cout << "Loading Ambient" << std::endl;
				}
				else brokenSceneLine(line);
			}; break;
			case COLLISIONS:
			{

			}break;
			case DIRECTIONAL:
			{
					if(lines.size() == 8)
					{
						currentEntity->addComponent(ServiceLocator::getCM().createDirectional(stV(lines[1],lines[2],lines[3]),std::stof(lines[4].c_str()),stV(lines[5],lines[6],lines[7])));
						std::cout<<"Loading Directional"<<std::endl;
					}
					else brokenSceneLine(line);
			};break;
			case SKYBOX:
			{
				if(lines.size() == 11)
				{
					currentEntity->addComponent(ServiceLocator::getCM().createSkyBox(stV(lines[1],lines[2],lines[3]),lines[4],lines[5],lines[6],lines[7],lines[8],lines[9],lines[10]));
					std::cout<<"Loading Skybox"<<std::endl;
				}
				else brokenSceneLine(line);
			};break;
			case PHYSICS:
			{
				if (lines.size() == 3)
				{

					currentEntity->addPhysics(ServiceLocator::getCM().createPhysicComponent(PhysicsData(PhysicsData::stringToType(lines[1]), std::stof(lines[2].c_str()))));
				std::cout << "Loading Phyiscs" << std::endl;
				}
				else brokenSceneLine(line);
			}break;;
			case SCRIPT:
			{
				if (lines.size() == 2)
				{
					currentEntity->addComponent(ServiceLocator::getCM().createScript(lines[1]));
					std::cout << "Loading Script" << std::endl;
				}
				else brokenSceneLine(line);
			}; break;
			case AUDIO:
			{
				if(lines.size() < 3) brokenSceneLine(line);
				else
				{
					if (std::stoi(lines[1]) == 2)	currentEntity->addComponent(ServiceLocator::getCM().create2DAudio(lines[2], std::stof(lines[3])));
					else currentEntity->addComponent(ServiceLocator::getCM().create3DAudio(lines[2], Vector3(), std::stof(lines[3])));
					std::cout << "Loading Skybox" << std::endl;
				}
			}; break;
			default:
			{
				
			}break;
		}
	}
	if(currentEntity != NULL)
	{
		addEntity(currentEntity);
	}
		
	return true;
};










