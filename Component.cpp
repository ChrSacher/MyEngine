#include "Component.h"







	std::string BTS(bool temp)
	{
		if(temp) return "true";
		return "false";
	}
	inline Transform* Component::GetTransform()             { return (parent->getTransform()); }
	inline const Transform& Component::GetTransform() const { return *parent->getTransform(); }
	
	void Component::SetParent(Entity* Parent) 
	{ 
		parent = Parent;
	}
	void Component::notify(int eventType,Entity* entityInteractedWith)
	{
		parent->receive(eventType,this,entityInteractedWith);
	};
	void Component::receive(int eventType,Component* sender,Entity* entityInteractedWith)
	{
	};
	SkyBoxComponent::SkyBoxComponent(Vector3 color,std::vector<std::string> paths)
	{
		skyBox.loadSkybox(paths[0],paths[1],paths[2],paths[3],paths[4],paths[5],paths[6]);
		skyBox.setColor(color);
		type = SKYBOX;
	}
	SkyBoxComponent::~SkyBoxComponent()
	{
	}
	void SkyBoxComponent::render(Shader* shader,Camera3d* camera)
	{
		skyBox.renderSkybox(camera);
	}
	std::string SkyBoxComponent::sceneSave()
	{
		auto r = skyBox.getDirAndFile();
		return "S " + std::to_string(skyBox.getColor().x) + " " +std::to_string(skyBox.getColor().y )+ " " + std::to_string(skyBox.getColor().z) +" " + r[0] +" " +r[1] + " "+ r[2]+" "+ r[3]+" "+ r[4]+" " +r[5]+" " + r[6] +"\n" ;
	}













	GraphicsComponent::~GraphicsComponent() 
	{

	}
	std::string GraphicsComponent::sceneSave()
	{
		return "G " + material.texture.texturepath + " " + material.normalMap.texturepath +" " + mesh.getPath() + " " +  std::to_string(material.color.x) + " " +  std::to_string(material.color.y) + " " +  std::to_string(material.color.z) + "\n";
	}
	void GraphicsComponent::render(Shader* shader,Camera3d* camera)
	{
			bool canRender = true;
			shader->use();
			Transform transform = *parent->getTransform();
			if(shader != NULL)
			{
				transform.update(shader);
				if(camera != NULL){shader->setMVP(*transform.getMatrix());}
				else{shader->setMVP(camera->GetViewProjection(),*transform.getMatrix());}
				material.update(shader);
			}
			if(camera != NULL)
			{
				if(transform.getPos().distance(camera->getPos()) > camera->getZ().y) canRender = false; //don't draw when out of range
				if(camera->isBehind(transform.getPos()))  canRender = false; //don't draw behind camera
				
			}
			if(canRender) 
			{
				mesh.draw();
			}
	};
	GraphicsComponent::GraphicsComponent(std::string &texturePath,std::string &normalMap,std::string &ObjectPath, Vector3 color,bool autoCenter):material(texturePath,normalMap,color,2,32),mesh(ObjectPath,autoCenter)
	{
		renderable = true;
		type = GRAPHICS;
	}



	CollisionComponent* CollisionComponent::create()
	{
		return new CollisionComponent();
	}
	CollisionComponent::~CollisionComponent()
	{

	}
	CollisionComponent::CollisionComponent(CollisionWorld *World){world = World;type = COLLISIONS;}//if null then no collision
	void CollisionComponent::update()
	{

	}
	TerrainComponent::~TerrainComponent()
	{

	}
	void  TerrainComponent::render(Shader* shader,Camera3d* camera)
	{
		terrain.render(shader);
	}
	std::string  TerrainComponent::sceneSave()
	{
		return "T " + terrain.getPath() +" "+ terrain.getMaterial()->texture.texturepath +" " + std::to_string(terrain.getTransform()->getScale().x) + " " + std::to_string(terrain.getTransform()->getScale().y) + " " + std::to_string(terrain.getTransform()->getScale().z) +" " +BTS(terrain.isCentered()) + "\n";
	}
	TerrainComponent::TerrainComponent(std::string Path,std::string Texture,Vector3 Scale,bool Center):terrain(Path,Texture,Scale,Center)
	{
		parent = NULL;
		type = TERRAIN;
	}
	
	void AmbientLightComponent::render(Shader* shader,Camera3d* camera)
	{
		
	}
	void AmbientLightComponent::setColor(std::vector<AmbientLightComponent> &ambients,Shader *shader)
	{
		Vector3 color = Vector3(0,0,0); 
		if(ambients.size() > 0)
		{
			for(unsigned int i = 0; i < ambients.size();i++)
			{
				color += ambients[i].ambient.getAmbient();
			}
			color /= (ambients.size());
			shader->setUniform("ambientLight",color);
		}
		else shader->setUniform("ambientLight",Vector3(0.5f,0.5f,0.5f));
	}
	std::string AmbientLightComponent::sceneSave()
	{
		return "A "+ std::to_string(ambient.getAmbient().x)+ " " + std::to_string(ambient.getAmbient().y)+ " " +std::to_string(ambient.getAmbient().z)+ "\n";
	}

	void DirectionalLightComponent::render(Shader* shader,Camera3d* camera)
	{
		light.update(shader);
	}
	static void setDirects(std::vector<DirectionalLightComponent> &ambients,Shader *shader)
	{
		
	}
	std::string DirectionalLightComponent::sceneSave()
	{
		return "D "	+ std::to_string(light.getBaseLight().getColor().x) + " " + std::to_string(light.getBaseLight().getColor().y)+ " " +std::to_string(light.getBaseLight().getColor().z)+ " " + std::to_string(light.getBaseLight().getIntensity()) +" "
					+ std::to_string(light.getDirection().x) + " " + std::to_string(light.getDirection().y)+ " " +std::to_string(light.getDirection().z)+"\n";
	}





	void ComponentManager::startup()
	{
		graphics.reserve(50);
		collisions.reserve(50);
		directionals.reserve(5);
		collisions.reserve(50);
		skies.reserve(5);
		terrains.reserve(2);
	}
	void ComponentManager::shutdown()
	{
		ambients.clear();
		graphics.clear();
		collisions.clear();
		terrains.clear();
		directionals.clear();
		skies.clear();
		for (auto &i = positions.begin(); i != positions.end(); i++)
		{
			for (auto &k = i->second.begin(); k != i->second.end(); k++)
			{
				delete(k->second);
			}
		}
	}
	ComponentManager& ComponentManager::get()
	{
		static ComponentManager man;
		return man;
	}
	void ComponentManager::update()
	{
		for(unsigned int i = 0;i < directionals.size();i++) directionals[i].update();
		for(unsigned int i = 0;i < ambients.size();i++) ambients[i].update();
		for(unsigned int i = 0;i < graphics.size();i++) graphics[i].update();
		for(unsigned int i = 0;i < collisions.size();i++) collisions[i].update();
		for(unsigned int i = 0;i < terrains.size();i++) terrains[i].update();
	}
	void ComponentManager::render(Shader* shader,Camera3d* camera)
	{
		AmbientLightComponent::setColor(ambients,shader);
		for(unsigned int i = 0;i < skies.size();i++) skies[i].render(shader,camera);
		for(unsigned int i = 0;i < directionals.size();i++) directionals[i].render(shader,camera);
		for(unsigned int i = 0;i < graphics.size();i++) graphics[i].render(shader,camera);
		for(unsigned int i = 0;i < collisions.size();i++) collisions[i].render(shader,camera);
		for(unsigned int i = 0;i < terrains.size();i++) terrains[i].render(shader,camera);
	}
	ComponentPosition& ComponentManager::createGraphics(std::string texturePath,std::string normalMap,std::string ObjectPath, Vector3 color,bool autoCenter)
	{
		graphics.emplace_back(texturePath,normalMap,ObjectPath,color,autoCenter);
		ComponentPosition* r = new ComponentPosition(GRAPHICS, graphics.size() - 1);
		positions[GRAPHICS].insert(std::make_pair(r->ID,r));
		return *r;
	}
	ComponentPosition& ComponentManager::createTerrain(std::string Path,std::string Texture,Vector3 Scale,bool Center)
	{
		terrains.emplace_back(Path,Texture,Scale,Center);
		ComponentPosition* r = new ComponentPosition(TERRAIN, terrains.size() - 1);
		positions[TERRAIN].insert(std::make_pair(r->ID, r));
		return *r;
	}
	ComponentPosition& ComponentManager::createAmbient(Vector3 Color)
	{
		ambients.emplace_back(Color);
		ComponentPosition* r = new ComponentPosition(TERRAIN, ambients.size() - 1);
		positions[AMBIENT].insert(std::make_pair(r->ID, r));
		return *r;
	}
	ComponentPosition& ComponentManager::createDirectional(Vector3 Color,float Intensity,Vector3 Dir)
	{
		directionals.emplace_back(BaseLight(Color,Intensity),Dir);
		ComponentPosition* r = new ComponentPosition(DIRECTIONAL, directionals.size() - 1);
		positions[DIRECTIONAL].insert(std::make_pair(r->ID, r));
		return *r;
	}
	ComponentPosition&  ComponentManager::createSkyBox(Vector3 color,std::string Directory, std::string posx, std::string negx, std::string posy, std::string negy, std::string posz, std::string negz)
	{
		std::vector<std::string> paths;
		paths.push_back(Directory);
		paths.push_back(posx);
		paths.push_back(negx);
		paths.push_back(posy);
		paths.push_back(negy);
		paths.push_back(posz);
		paths.push_back(negz);
		skies.emplace_back(color,paths);//emplace back only accepts certtain number of variables for whatever reason
		ComponentPosition* r = new ComponentPosition(SKYBOX, skies.size() - 1);
		positions[SKYBOX].insert(std::make_pair(r->ID, r));
		return *r;

	}
Component* ComponentManager::findComponent(ComponentPosition Pos)
{
	switch (Pos.type)
	{
		case GRAPHICS:
		{
			if (Pos.position >= graphics.size()) return NULL;
			return &graphics[Pos.position];
		}break;
		case COLLISIONS:
		{
			if (Pos.position >= collisions.size()) return NULL;
			return &collisions[Pos.position];
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
		default:
		{
			return NULL;
		}
	}
}

void ComponentManager::deleteComponent(ComponentPosition& Pos)
{
	switch (Pos.type)
	{
		case GRAPHICS:
		{
			if (Pos.position >= graphics.size()) return;
			graphics.erase(graphics.begin() + Pos.position);

		}break;
		case COLLISIONS:
		{
			if (Pos.position >= collisions.size()) return;
			collisions.erase(collisions.begin() + Pos.position);
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