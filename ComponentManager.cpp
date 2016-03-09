#include "ComponentManager.h"

void ComponentManager::initialize()
{
	graphics.reserve(50);
	directionals.reserve(5);
	physics.reserve(50);
	skies.reserve(5);
	terrains.reserve(2);
}
void ComponentManager::destroy()
{
	for (unsigned int i = 0; i < ambients.size(); i++);
	ambients.clear();
	for (unsigned int i = 0; i < graphics.size(); i++) GCS.unload(graphics[i]);
	graphics.clear();
	for (unsigned int i = 0; i < terrains.size(); i++) TCS.unload(terrains[i]);
	terrains.clear();
	for (unsigned int i = 0; i < directionals.size(); i++);
	directionals.clear();
	for (unsigned int i = 0; i < physics.size(); i++) PCS.unload(physics[i]);
	physics.clear();
	for (unsigned int i = 0; i < skies.size(); i++) ;
	skies.clear();
	for (unsigned int i = 0; i < audios.size(); i++) ACS.unload(audios[i]);
	audios.clear();
	for (unsigned int i = 0; i < scripts.size(); i++) SCCS.unload(scripts[i]);
	scripts.clear();
	for (auto &i = positions.begin(); i != positions.end(); i++)
	{
		for (auto &k = i->second.begin(); k != i->second.end(); k++)
		{
			delete(k->second);
		}
	}
}
void ComponentManager::update()
{
	SCS.update(skies);
	LCS.update(directionals);
	LCS.update(ambients);
	GCS.update(graphics);
	TCS.update(terrains);
	PCS.update(physics);
	ACS.update(audios);
	SCCS.update(scripts);
	
}
void ComponentManager::render(Shader* shader, Camera3d* camera)
{
	SCS.render(skies, camera);
	LCS.render(directionals,shader);
	LCS.render(ambients,shader);
	GCS.render(graphics,shader,camera);
	TCS.render(terrains,shader);
	
}
ComponentPosition* ComponentManager::createGraphics(std::string texturePath, std::string normalMap, std::string ObjectPath, Vector3 color, bool autoCenter)
{

	graphics.emplace_back();
	GCS.load(graphics[graphics.size() - 1], texturePath, normalMap, ObjectPath, color, autoCenter);
	ComponentPosition* r = new ComponentPosition(GRAPHICS, graphics.size() - 1);
	positions[GRAPHICS].insert(std::make_pair(r->ID, r));
	return r;
}
ComponentPosition* ComponentManager::createTerrain(std::string Path, std::string Texture, Vector3 Scale, bool Center)
{
	terrains.emplace_back();
	TCS.load(terrains[terrains.size() - 1], Path, Texture, Scale, Center, 2);
	ComponentPosition* r = new ComponentPosition(TERRAIN, terrains.size() - 1);
	positions[TERRAIN].insert(std::make_pair(r->ID, r));
	return r;
}
ComponentPosition* ComponentManager::createAmbient(Vector3 Color)
{
	ambients.emplace_back();
	LCS.load(ambients[ambients.size() - 1], Color);
	ComponentPosition* r = new ComponentPosition(AMBIENT, ambients.size() - 1);
	positions[AMBIENT].insert(std::make_pair(r->ID, r));
	return r;
}
ComponentPosition* ComponentManager::createPhysicComponent(PhysicsData& Data)
{
	physics.emplace_back();
	PCS.load(physics[physics.size() - 1],Data);
	ComponentPosition* r = new ComponentPosition(PHYSICS, physics.size() - 1);
	positions[PHYSICS].insert(std::make_pair(r->ID, r));
	return r;
}


ComponentPosition* ComponentManager::createDirectional(Vector3 Color, float Intensity, Vector3 Dir)
{
	//TODO directional LCS loader
	directionals.emplace_back(BaseLight(Color, Intensity), Dir);
	ComponentPosition* r = new ComponentPosition(DIRECTIONAL, directionals.size() - 1);
	positions[DIRECTIONAL].insert(std::make_pair(r->ID, r));
	return r;
}
ComponentPosition*  ComponentManager::createSkyBox(Vector3 color, std::string Directory, std::string posx, std::string negx, std::string posy, std::string negy, std::string posz, std::string negz)
{
	std::vector<std::string> paths;
	paths.push_back(Directory);
	paths.push_back(posx);
	paths.push_back(negx);
	paths.push_back(posy);
	paths.push_back(negy);
	paths.push_back(posz);
	paths.push_back(negz);
	skies.emplace_back(color, paths);//emplace back only accepts certtain number of variables for whatever reason
	ComponentPosition* r = new ComponentPosition(SKYBOX, skies.size() - 1);
	positions[SKYBOX].insert(std::make_pair(r->ID, r));
	return r;

}
ComponentPosition* ComponentManager::create2DAudio(std::string audiopath, float Volume)
{
	audios.emplace_back();
	ACS.load(audios[audios.size() - 1], audiopath, Volume);
	ComponentPosition* r = new ComponentPosition(AUDIO, audios.size() - 1);
	positions[AUDIO].insert(std::make_pair(r->ID, r));
	return r;
}
ComponentPosition* ComponentManager::create3DAudio(std::string audiopath, Vector3 position, float Volume)
{
	audios.emplace_back();
	ACS.load(audios[audios.size() - 1], audiopath,position, Volume);
	ComponentPosition* r = new ComponentPosition(AUDIO, audios.size() - 1);
	positions[AUDIO].insert(std::make_pair(r->ID, r));
	return r;
}
ComponentPosition* ComponentManager::createScript(std::string path)
{
	scripts.emplace_back();
	SCCS.load(scripts[scripts.size() - 1], path);
	ComponentPosition* r = new ComponentPosition(SCRIPT, scripts.size() - 1);
	positions[SCRIPT].insert(std::make_pair(r->ID, r));
	return r;
}
Component* ComponentManager::findComponent(ComponentPosition* Posi)
{
	ComponentPosition &Pos = *Posi;
	switch (Pos.type)
	{
	case GRAPHICS:
	{
		if (Pos.position >= graphics.size()) return NULL;
		return &graphics[Pos.position];
	}break;
	case TERRAIN:
	{
		if (Pos.position >= terrains.size()) return NULL;
		return &terrains[Pos.position];
	}break;
	case AMBIENT:
	{
		if (Pos.position >= ambients.size()) return NULL;
		return &ambients[Pos.position];

	}break;
	case DIRECTIONAL:
	{
		if (Pos.position >= directionals.size()) return NULL;
		return &directionals[Pos.position];
	}break;
	case SKYBOX:
	{
		if (Pos.position >= skies.size()) return NULL;
		return &skies[Pos.position];
	}break;
	case PHYSICS:
	{
		if (Pos.position >= physics.size()) return NULL;
		return &physics[Pos.position];
	}break;
	case AUDIO:
	{
		if (Pos.position >= audios.size()) return NULL;
		return &audios[Pos.position];
	}break;
	case SCRIPT:
	{
		if (Pos.position >= scripts.size()) return NULL;
		return &scripts[Pos.position];
	}break;
	default:
	{
		return NULL;
	}
	}
}

void ComponentManager::deleteComponent(ComponentPosition* Posi)
{
	ComponentPosition &Pos = *Posi;
	switch (Pos.type)
	{
	case GRAPHICS:
	{
		if (Pos.position >= graphics.size()) return;
		GCS.unload(graphics[Pos.position]);
		graphics.erase(graphics.begin() + Pos.position);

	}break;
	case TERRAIN:
	{
		if (Pos.position >= terrains.size()) return;
		terrains.erase(terrains.begin() + Pos.position);
	}break;
	case AMBIENT:
	{
		if (Pos.position >= ambients.size()) return;
		ambients.erase(ambients.begin() + Pos.position);

	}break;
	case DIRECTIONAL:
	{
		if (Pos.position >= directionals.size()) return;
		directionals.erase(directionals.begin() + Pos.position);
	}break;
	case SKYBOX:
	{
		if (Pos.position >= skies.size()) return;
		skies.erase(skies.begin() + Pos.position);
	}break;
	case PHYSICS:
	{
		if (Pos.position >= physics.size()) return;
		PCS.unload(physics[Pos.position]);
		physics.erase(physics.begin() + Pos.position);
	}break;
	case AUDIO:
	{
		if (Pos.position >= audios.size()) return;
		ACS.unload(audios[Pos.position]);
		audios.erase(audios.begin() + Pos.position);
	}break;
	case SCRIPT:
	{
		if (Pos.position >= scripts.size()) return;
		SCCS.unload(scripts[Pos.position]);
		scripts.erase(scripts.begin() + Pos.position);
	}
	default:
	{
		return;
	}
	}
	auto r = positions.find(Pos.type);
	for (auto i = r->second.find(Pos.ID); i != r->second.end(); i++)
	{
		i->second->position -= 1;
	}

	r->second.erase(r->second.find(Pos.ID));
}

void ComponentManager::renderComponent(ComponentPosition* comp, Shader* shader, Camera3d* camera)
{
	renderComponent(comp->get(), shader, camera);
}
void ComponentManager::renderComponent(Component* comp, Shader* shader, Camera3d* camera)
{
	switch (comp->getType())
	{
		case GRAPHICS:
		{
			GCS.render(*static_cast<GraphicsComponent*>(comp), shader, camera);
		}break;
		case COLLISIONS:
		{

		}break;
		case TERRAIN:
		{

		}break;
		case AMBIENT:
		{


		}break;
		case DIRECTIONAL:
		{

		}break;
		case SKYBOX:
		{

		}break;
		default:
		{

		}
	}
}
void ComponentManager::addScriptListener(ScriptComponentSystem::Listener* r)
{
	SCCS.addListener(r);
}
void ComponentManager::removeScriptListener(ScriptComponentSystem::Listener* r)
{
	SCCS.removeListener(r);
}