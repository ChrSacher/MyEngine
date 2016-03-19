#include "Scene.h"
#include "ServiceLocator.h"
#include <libconfig\libconfig.hh>
#include <cstdio>
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






void Scene::sceneChanged()
{

	for (auto &itr = _listeners.begin(); itr != _listeners.end(); ++itr)
	{

		itr->second->sceneChanged(this);
	}
}

void Scene::addListener(Scene::Listener* listener)
{
	_listeners.insert(std::make_pair(listener->ListenerID, listener));
}

void Scene::removeListener(Scene::Listener* listener)
{
	_listeners.erase(listener->ListenerID);

}
Ray Scene::getClick(int x,int y)
{
	
	return camera->getDirClick(x, y);
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



template <typename ValueType>
bool look(libconfig::Setting& setting,std::string place,ValueType &value)
{
	if (setting.exists(place))
	{
		return setting.lookupValue(place, value);

	}
	else std::cout << setting.getPath() + place << " does not exist. Value remains unchanged." << std::endl;
	return false;
}

bool look(libconfig::Setting& current, std::string place ,Vector3& vec,std::string ent)
{
	using namespace libconfig;
	if (current.exists(place))
	{
		Setting& posit = current.lookup(place);
		vec[0] = posit[0];
		vec[1] = posit[1];
		vec[2] = posit[2];
		return true;
	}
	else
	{
		std::cout << place<<" error in " << ent << std::endl;
		return false;
	}
}

void Scene::saveFile(std::string name)
{
	using namespace libconfig;
	Config config;

	Setting& root = config.getRoot();
	root.add("Version", Setting::TypeFloat) = 1.0f;
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

		if (ent->getPhysics() != NULL) current.add("numComponents", Setting::TypeInt) = (int)ent->components.size() + 1;
		else current.add("numComponents", Setting::TypeInt) = (int)ent->components.size();
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
				comp.add("Normalpath", Setting::TypeString) = s->material.normalMap.texturepath;
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
				color.add(Setting::TypeFloat) = s->getAmbient().x;
				color.add(Setting::TypeFloat) = s->getAmbient().y;
				color.add(Setting::TypeFloat) = s->getAmbient().z;


			}; break;
			case TERRAIN:
			{
				auto* s = ent->components[i]->getT<TerrainComponent*>();
				Setting& comp = current.add("Component" + std::to_string(j++), Setting::TypeGroup);
				comp.add("Type", Setting::TypeString) = "Terrain";
				comp.add("Terrainpath", Setting::TypeString) = s->terrain.getPath();
				comp.add("Texturepath", Setting::TypeString) = s->terrain.getMaterial()->texture.texturepath;
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
				color.add(Setting::TypeFloat) = s->getBaseLight().getColor().x;
				color.add(Setting::TypeFloat) = s->getBaseLight().getColor().y;
				color.add(Setting::TypeFloat) = s->getBaseLight().getColor().z;
				comp.add("Intensity", Setting::TypeFloat) = s->getBaseLight().getIntensity();
				Setting& dir = comp.add("Direction", Setting::TypeArray);
				dir.add(Setting::TypeFloat) = s->getDirection().x;
				dir.add(Setting::TypeFloat) = s->getDirection().y;
				dir.add(Setting::TypeFloat) = s->getDirection().z;

			}; break;
			case SKYBOX:
			{
				auto* s = ent->components[i]->getT<SkyBoxComponent*>();
				auto r = s->getDirAndFile();
				Setting& comp = current.add("Component" + std::to_string(j++), Setting::TypeGroup);
				comp.add("Type", Setting::TypeString) = "Skybox";
				Setting& color = comp.add("Color", Setting::TypeArray);
				color.add(Setting::TypeFloat) = s->getColor().x;
				color.add(Setting::TypeFloat) = s->getColor().y;
				color.add(Setting::TypeFloat) = s->getColor().z;
				Setting& files = comp.add("Files", Setting::TypeArray);
				for (int w = 0; w <6; w++) files.add(Setting::TypeString) = r[w];
			}; break;
			case PHYSICS:
			{

			}break;
			case AUDIO:
			{
				auto* s = ent->components[i]->getT<AudioComponent*>();
				Setting& comp = current.add("Component" + std::to_string(j++), Setting::TypeGroup);
				comp.add("Type", Setting::TypeString) = "Audio";
				comp.add("Path", Setting::TypeString) = s->path;
				comp.add("Volume", Setting::TypeFloat) = s->sound.getVolume();
				comp.add("is2D", Setting::TypeBoolean) = s->is2D;
				Setting& pos = comp.add("Position", Setting::TypeArray);
				pos.add(Setting::TypeFloat) = s->sound.getPosition().x;
				pos.add(Setting::TypeFloat) = s->sound.getPosition().y;
				pos.add(Setting::TypeFloat) = s->sound.getPosition().z;
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
		if (ent->getPhysics() != NULL)
		{
			auto* s = ent->getPhysics()->getT<PhysicsComponent*>();

			Setting& comp = current.add("Component" + std::to_string(j++), Setting::TypeGroup);
			comp.add("Type", Setting::TypeString) = "Physics";
			comp.add("Physicstype", Setting::TypeString) = PhysicsData::typeToString(s->data.type);
			comp.add("Mass", Setting::TypeFloat) = s->data.mass;
		}
	}//remove
	config.writeFile(name.c_str());

}

bool Scene::loadScene(std::string Path)
{
	using namespace libconfig;
	FILE* file = fopen(Path.c_str(),"r");
	if (file == NULL)
	{
		perror("Error opening file");
		return false;
	}
	Config config;
	try
	{
		config.read(file);
	}
	catch (const FileIOException &fioex)
	{
		std::cerr << "I/O error while reading file." << fioex.what() << std::endl;
		return(EXIT_FAILURE);
	}
	catch (const ParseException &pex)
	{
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
			<< " - " << pex.getError() << std::endl;
		return(EXIT_FAILURE);
	}
	Setting& root = config.getRoot();

	static std::map<std::string, ComponentType> type{
		std::make_pair("Entity",ENTITY),
		std::make_pair("Graphics",GRAPHICS),
		std::make_pair("Ambient",AMBIENT),
		std::make_pair("Directional",DIRECTIONAL),
		std::make_pair("Terrain",TERRAIN),
		std::make_pair("Skybox",SKYBOX),
		std::make_pair("Physics", PHYSICS),
		std::make_pair("Script", SCRIPT),
		std::make_pair("Audio", AUDIO) };

	bool numEntitiesSpecified = true;
	int numEnt = 1000000;
	numEntitiesSpecified = look<int>(root, "numEntities", numEnt);
	

	for (int i = 0; i < numEnt; i++)
	{
		std::string currentEntityName = "Entity" + std::to_string(i);
		if (!config.exists(currentEntityName))
		{
			
			if (numEntitiesSpecified)
			{
				std::cout << "Entity" + std::to_string(i) << " does not exist. Loading next Entity" << std::endl;
				continue;
			}
			else
			{
				std::cout << "Entity" + std::to_string(i) << " does not exist. Aborting loading due to non specified numEntities" << std::endl;
				return false;
			}
		}
		Setting& current = root.lookup("Entity" + std::to_string(i));
		std::string name;
		Vector3 pos = Vector3(), rot = Vector3(), scale = Vector3();


		if (!(current.lookupValue("Name", name)))
		{
			std::cout << "Entity without name in sceneLoad" << std::endl;
			break;
		}
		
		look(current, "Position", pos, currentEntityName);
		look(current, "Rotation", rot, currentEntityName);
		look(current, "Scale", scale, currentEntityName);
	
		Entity* ent = ServiceLocator::getEM().create(name, pos, rot, scale);
		addEntity(ent);

		//load Components
		int numComponents = 1000;
		bool numComponentsSpecified = true;
		numComponentsSpecified = look<int>(current, "numComponents", numComponents);
		for (int i = 0; i < numComponents; i++)
		{
			std::string currentCompName = "Component" + std::to_string(i);
			if (!current.exists(currentCompName))
			{

				if (numComponentsSpecified)
				{
					std::cout << currentCompName << " does not exist. Loading next Component." << std::endl;
					continue;
				}
				else
				{
					std::cout << currentCompName << " does not exist. Aborting Component loading due to non specified numComponents." << std::endl;
					return false;
				}
			}

			Setting& comp = current.lookup(currentCompName);
			ComponentType temp = NUMCOMPONENT;
			auto it = type.find(comp.lookup("Type"));
			if (it != type.end()) temp = it->second;

			switch (temp)
			{
				case GRAPHICS:
				{
					std::string texturePath(""), normalPath(""), objectPath("");
					Vector3 color(1.0f,1.0f,1.0f);
					bool autocenter = false;
					look(comp,"Texturepath", texturePath);
					look(comp,"Normalpath", normalPath);
					look(comp,"Meshpath", objectPath);
					look(comp, "Color", color,currentEntityName +  currentCompName);
					ent->addComponent(ServiceLocator::getCM().createGraphics(texturePath, normalPath, objectPath,color,autocenter));
				}break;
				case AMBIENT:
				{
					Vector3 ambient(0.5f, 0.5f, 0.5f);
					look(comp, "Color", ambient, currentEntityName + currentCompName);
					ent->addComponent(ServiceLocator::getCM().createAmbient(ambient));
				}; break;
				case TERRAIN:
				{
					std::string path(""), texturepath("");
					Vector3 scale;
					bool centered = false;
					
					look(comp, "Terrainpath", path);
					look(comp, "Texturepath", texturepath);
					look(comp, "Scale", scale, currentEntityName + currentCompName);
					look(comp, "Centered", centered);

					ent->addComponent(ServiceLocator::getCM().createTerrain(path, texturepath, scale, centered));
				}; break;
				case DIRECTIONAL:
				{
					Vector3 color(1.0f, 1.0f, 1.0f), direction(0.0f, 1.0f, 0.0f);
					float intensity = 100.0f;

					look(comp, "Color", color, currentEntityName + currentCompName);
					look(comp, "Direction", direction, currentEntityName + currentCompName);
					look(comp, "Intensity", intensity);

					ent->addComponent(ServiceLocator::getCM().createDirectional(color, intensity, direction));

				}; break;
				case SKYBOX:
				{
					Vector3 color(1.0f, 1.0f, 1.0f);
					std::vector<std::string> files;
					if (comp.exists("Files"))
					{
						Setting& f = comp.lookup("Files");
						files.push_back(f[0]);
						files.push_back(f[1]);
						files.push_back(f[2]);
						files.push_back(f[3]);
						files.push_back(f[4]);
						files.push_back(f[5]);
					}
					look(comp, "Color", color, currentEntityName + currentCompName);
					ent->addComponent(ServiceLocator::getCM().createSkyBox(color, files[0], files[1], files[2], files[3], files[4], files[5]));
				}; break;
				case PHYSICS:
				{
					PhysicsData data;
					std::string type;
					look(comp, "Physicstype", type);
					data.type = PhysicsData::stringToType(type);
					look(comp, "Mass", data.mass);
					ent->getPhysics()->getT<PhysicsComponent*>()->reload(data);
				}break;
				case AUDIO:
				{
					
					
					bool is2D = false;
					Vector3 pos(0.0f,0.0f,0.0f);
					std::string path("");
					float Volume = 1.0f;
					look(comp, "is2D", is2D);
					look(comp, "Path", path);
					look(comp, "Volume", Volume);
					look(comp, "Position", pos, currentEntityName + currentCompName);
					if (is2D)
					{
						ent->addComponent(ServiceLocator::getCM().create2DAudio(path, Volume));
					}
					else
					{
						ent->addComponent(ServiceLocator::getCM().create3DAudio(path, pos, Volume));
					}
					
				}break;
				case SCRIPT:
				{
					std::string path("");
					look(comp, "Path", path);
					ent->addComponent(ServiceLocator::getCM().createScript(path));
				}; break;
				default:
				{
					std::cout << "BROKEN SCENE SAVE COMPONENT \n";
				}break;
				
			}
		}
	}
	return true;



























	/*std::cout << "Loading Scene" << Path<<std::endl;
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
		
	return true;*/
};










