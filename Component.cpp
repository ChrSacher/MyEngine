#include "Component.h"







	std::string BTS(bool temp)
	{
		if(temp) return "true";
		return "false";
	}
	inline Transform* Component::GetTransform()             { return (parent->getTransform()); }
	inline const Transform& Component::GetTransform() const { return *parent->getTransform(); }
	
	void Component::SetParent(Entity* Parent,int Position) 
	{ 
		parent = Parent;
		position = Position;
	}
	void Component::notify(int eventType,Entity* entityInteractedWith)
	{
		parent->receive(eventType,this,entityInteractedWith);
	};
	void Component::receive(int eventType,Component* sender,Entity* entityInteractedWith)
	{
	};
	void Component::updatePointer()
	{
		parent->updateComponentPointer(position,this);
	}

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
	PhysicsComponent* PhysicsComponent::create()
	{
		return new PhysicsComponent();
	}
	PhysicsComponent::~PhysicsComponent(){};
	PhysicsComponent::PhysicsComponent(PhysicsWorld *World){world=World;type = PHYSICS;}; //if Null no physics on object
	void PhysicsComponent::update(float delta)
	{
	};


	CollisionComponent* CollisionComponent::create()
	{
		return new CollisionComponent();
	}
	CollisionComponent::~CollisionComponent()
	{

	}
	CollisionComponent::CollisionComponent(CollisionWorld *World){world = World;type = COLLISIONS;}//if null then no collision
	void CollisionComponent::update(float delta)
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
		physics.reserve(50);
		collisions.reserve(50);
		directionals.reserve(5);
		terrains.reserve(2);
	}
	void ComponentManager::shutdown()
	{
		ambients.clear();
		graphics.clear();
		physics.clear();
		collisions.clear();
		terrains.clear();
	}
	ComponentManager& ComponentManager::get()
	{
		static ComponentManager man;
		return man;
	}
	void ComponentManager::update(float delta)
	{
		for(unsigned int i = 0;i < directionals.size();i++) directionals[i].update(delta);
		for(unsigned int i = 0;i < ambients.size();i++) ambients[i].update(delta);
		for(unsigned int i = 0;i < graphics.size();i++) graphics[i].update(delta);
		for(unsigned int i = 0;i < physics.size();i++) physics[i].update(delta);
		for(unsigned int i = 0;i < collisions.size();i++) collisions[i].update(delta);
		for(unsigned int i = 0;i < terrains.size();i++) terrains[i].update(delta);
	}
	void ComponentManager::render(Shader* shader,Camera3d* camera)
	{
		AmbientLightComponent::setColor(ambients,shader);
		for(unsigned int i = 0;i < skies.size();i++) skies[i].render(shader,camera);
		for(unsigned int i = 0;i < directionals.size();i++) directionals[i].render(shader,camera);
		for(unsigned int i = 0;i < graphics.size();i++) graphics[i].render(shader,camera);
		for(unsigned int i = 0;i < physics.size();i++) physics[i].render(shader,camera);
		for(unsigned int i = 0;i < collisions.size();i++) collisions[i].render(shader,camera);
		for(unsigned int i = 0;i < terrains.size();i++) terrains[i].render(shader,camera);
	}
	GraphicsComponent& ComponentManager::createGraphics(std::string texturePath,std::string normalMap,std::string ObjectPath, Vector3 color,bool autoCenter)
	{
		static unsigned int size = graphics.capacity(); //update if getting bigger
		graphics.emplace_back(texturePath,normalMap,ObjectPath,color,autoCenter);
		{
			for(unsigned int i = 0;i < graphics.size() - 1;i++) graphics[i].updatePointer(); //size - 1 because the last one doesn't have a parent yet and also it is automatically updated witht he returnS
			size = graphics.capacity();
		}
		return graphics.back();
	}
	void ComponentManager::deleteGraphics(Component *graphicsC)
	{
		for(unsigned int i = 0; i < graphics.size();i++)
		{
			if(graphicsC->getID() == graphics[i].getID())
			{
				graphics.erase(graphics.begin() + i);
				i--;
				for(unsigned int j = i;j < graphics.size();j++) graphics[i].updatePointer();
				return;
			}
		}
	}
	TerrainComponent& ComponentManager::createTerrain(std::string Path,std::string Texture,Vector3 Scale,bool Center)
	{
		static unsigned int size = terrains.capacity(); //update if getting bigger
		terrains.emplace_back(Path,Texture,Scale,Center);
		if(size <= terrains.size())
		{
			for(unsigned int i = 0;i < terrains.size() - 1;i++) terrains[i].updatePointer(); //size - 1 because the last one doesn't have a parent yet and also it is automatically updated witht he returnS
			size = terrains.capacity();
		}
		return terrains.back();
	}
	void ComponentManager::deleteTerrain(Component *terrain)
	{
		for(unsigned int i = 0; i < terrains.size();i++)
		{
			if(terrain->getID() == terrains[i].getID())
			{
				terrains.erase(terrains.begin() + i);
				i--;
				for(unsigned int j = i;j < terrains.size();j++) terrains[i].updatePointer();
				return;
			}
		}
	}
	AmbientLightComponent& ComponentManager::createAmbient(Vector3 Color)
	{
		static unsigned  int size = ambients.capacity(); //update if getting bigger
		ambients.emplace_back(Color);
		if(size <=  ambients.size())
		{
			for(unsigned int i = 0;i <  ambients.size() - 1;i++)  ambients[i].updatePointer(); //size - 1 because the last one doesn't have a parent yet and also it is automatically updated witht he returnS
			size =  ambients.capacity();
		}
		return ambients.back();
	}
	void ComponentManager::deleteAmbient(Component * ambient)
	{
		for(unsigned int i = 0; i < ambients.size();i++)
		{
			if(ambient->getID() ==  ambients[i].getID())
			{
				 ambients.erase( ambients.begin() + i);
				i--;
				for(int j = i;j <  ambients.size();j++)  ambients[i].updatePointer();
				return;
			}
		}
	}
	DirectionalLightComponent& ComponentManager::createDirectional(Vector3 Color,float Intensity,Vector3 Dir)
	{
		static unsigned int size = directionals.capacity(); //update if getting bigger
		directionals.emplace_back(BaseLight(Color,Intensity),Dir);
		if(size <=  directionals.size())
		{
			for(unsigned int i = 0;i <  directionals.size() - 1;i++)  directionals[i].updatePointer(); //size - 1 because the last one doesn't have a parent yet and also it is automatically updated witht he returnS
			size =  directionals.capacity();
		}
		return directionals.back();
	}
	void ComponentManager::deleteDirectional(Component * directional)
	{
		for(unsigned int i = 0; i < directionals.size();i++)
		{
			if(directional->getID() ==  directionals[i].getID())
			{
				 directionals.erase( directionals.begin() + i);
				i--;
				for(unsigned int j = i;j <  directionals.size();j++)  directionals[i].updatePointer();
				return;
			}
		}
	}
	SkyBoxComponent&  ComponentManager::createSkyBox(Vector3 color,std::string Directory, std::string posx, std::string negx, std::string posy, std::string negy, std::string posz, std::string negz)
	{
		static int unsigned  size = skies.capacity(); //update if getting bigger
		std::vector<std::string> paths;
		paths.push_back(Directory);
		paths.push_back(posx);
		paths.push_back(negx);
		paths.push_back(posy);
		paths.push_back(negy);
		paths.push_back(posz);
		paths.push_back(negz);
		skies.emplace_back(color,paths);//emplace back only accepts certtain number of variables for whatever reason
		if(size <=  skies.size())
		{
			for(int i = 0;i <  skies.size() - 1;i++)  skies[i].updatePointer(); //size - 1 because the last one doesn't have a parent yet and also it is automatically updated witht he returnS
			size =  skies.capacity();
		}
		return skies.back();

	}
	void  ComponentManager::deleteSkyBox(Component* sky)
	{
		for(unsigned int i = 0; i < skies.size();i++)
		{
			if(sky->getID() ==  skies[i].getID())
			{
				skies.erase( skies.begin() + i);
				i--;
				for(int j = i;j <  skies.size();j++)  skies[i].updatePointer();
				return;
			}
		}
	}