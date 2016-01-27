#include "Component.h"
#include "ServiceLocator.h"

Component* ComponentPosition::get() 
{ 
	return ServiceLocator::getCM().findComponent(this); 
}
void ComponentPosition::destroy()
{
	ServiceLocator::getCM().deleteComponent(this);
}

void Component::render(Shader* shader, Camera3d* camera)
{
	ServiceLocator::getCM().renderComponent(this,shader, camera);
}
AmbientLightComponent::~AmbientLightComponent() {};
DirectionalLightComponent::~DirectionalLightComponent() {};


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
		if(parent != NULL ) setTransform(*parent->getTransform());
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
	GraphicsComponent::GraphicsComponent()
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
	
	std::string  TerrainComponent::sceneSave()
	{
		return "T " + terrain.getPath() +" "+ terrain.getMaterial()->texture.texturepath +" " + std::to_string(terrain.getTransform()->getScale().x) + " " + std::to_string(terrain.getTransform()->getScale().y) + " " + std::to_string(terrain.getTransform()->getScale().z) +" " +BTS(terrain.isCentered()) + "\n";
	}
	TerrainComponent::TerrainComponent()
	{
		parent = NULL;
		type = TERRAIN;
	}
	
	
	
	std::string AmbientLightComponent::sceneSave()
	{
		return "A "+ std::to_string(ambient.getAmbient().x)+ " " + std::to_string(ambient.getAmbient().y)+ " " +std::to_string(ambient.getAmbient().z)+ "\n";
	}

	
	static void setDirects(std::vector<DirectionalLightComponent> &ambients,Shader *shader)
	{
		
	}
	std::string DirectionalLightComponent::sceneSave()
	{
		return "D "	+ std::to_string(light.getBaseLight().getColor().x) + " " + std::to_string(light.getBaseLight().getColor().y)+ " " +std::to_string(light.getBaseLight().getColor().z)+ " " + std::to_string(light.getBaseLight().getIntensity()) +" "
					+ std::to_string(light.getDirection().x) + " " + std::to_string(light.getDirection().y)+ " " +std::to_string(light.getDirection().z)+"\n";
	}


	//TODO move all loading of resources to the system and make Components relativly constructorless
	//GRAPHICS///////////////////////////////////////////////////////////////////////////////////
	void GraphicComponentSystem::load(GraphicsComponent &r, std::string &texturePath, std::string &normalMap, std::string &ObjectPath, Vector3 color, bool autoCenter)
	{
		r.material = Material(texturePath, normalMap, color, 2, 32);
		r.mesh = Mesh(ObjectPath, autoCenter);
		r.type = GRAPHICS;
		r.renderable = true;
	}
	void GraphicComponentSystem::unload(GraphicsComponent &r)
	{

	}
	void GraphicComponentSystem::update(std::vector<GraphicsComponent> &r)
	{
	}
	void GraphicComponentSystem::render(std::vector<GraphicsComponent> &r, Shader* shader, Camera3d* camera)
	{
		
		shader->use();
		for (int i = 0; i < r.size(); i++)
		{
			render(r[i], shader, camera);
		}
	
	}
	void GraphicComponentSystem::render(GraphicsComponent &r, Shader* shader, Camera3d* camera)
	{
		shader->use();
		
			bool canRender = true;
			if (shader != NULL)
			{
				r.transform.update(shader);
				if (camera != NULL) { shader->setMVP(*r.transform.getMatrix()); }
				else { shader->setMVP(camera->GetViewProjection(), *r.transform.getMatrix()); }
				r.material.update(shader);
			}
			if (camera != NULL)
			{
				if (r.transform.getPos().distance(camera->getPos()) > camera->getZ().y) canRender = false; //don't draw when out of range
				if (camera->isBehind(r.transform.getPos()))  canRender = false; //don't draw behind camera

			}
			if (canRender)
			{
				r.mesh.draw();
			}
	}
	std::string GraphicComponentSystem::sceneSave(GraphicsComponent &r)
	{
		return "G " + r.material.texture.texturepath + " " + r.material.normalMap.texturepath + " " + r.mesh.getPath() + " " + std::to_string(r.material.color.x) + " " + std::to_string(r.material.color.y) + " " + std::to_string(r.material.color.z) + "\n";
	};

	//SKYBOX//////////////////////////////////////////////////////////////////////////////////////////
	void SkyBoxComponentSystem::render(std::vector<SkyBoxComponent> &r, Camera3d* camera)
	{
		for (int i = 0; i < r.size();i++)
		r[i].skyBox.renderSkybox(camera);
	}

	void SkyBoxComponentSystem::update(std::vector<SkyBoxComponent> &r)
	{
	}







	//LIGHT//////////////////////////////////////////////////////////////////////////////////////////////////
	void LightComponentSystem::update(std::vector<DirectionalLightComponent> &r)
	{
		
	}

	void LightComponentSystem::update(std::vector<AmbientLightComponent> &rr)
	{
		
	}
	void  LightComponentSystem::load(AmbientLightComponent &light, Vector3 Ambient)
	{
		light.ambient = AmbientLight(Ambient);
	}
	void LightComponentSystem::render(std::vector<DirectionalLightComponent> &r, Shader* shader)
	{
		for (int i = 0; i < r.size(); i++)
		{
		
			//shader->setUniform("directionalLight[" + std::to_string(i) + "].direction", r[i].light.direction);
			//shader->setUniform("directionalLight[" + std::to_string(i) + "].base.color", r[i].light.base.getColor());
			//shader->setUniform("directionalLight[" + std::to_string(i) + "].base.intensity", r[i].light.base.getIntensity());
			shader->setUniform("directionalLight.direction", r[i].light.direction);
			shader->setUniform("directionalLight.base.color", r[i].light.base.getColor());
			shader->setUniform("directionalLight.base.intensity", r[i].light.base.getIntensity());
		}
	}

	void LightComponentSystem::render(std::vector<AmbientLightComponent> &r,Shader* shader)
	{
		Vector3 color = Vector3(0, 0, 0);
		if (r.size() > 0)
		{
			for (unsigned int i = 0; i < r.size(); i++)
			{
				color += r[i].ambient.getAmbient();
			}
			color /= (r.size());
			shader->setUniform("ambientLight", color);
		}
		else shader->setUniform("ambientLight", Vector3(0.5f, 0.5f, 0.5f));
	}







	//TERRAIN  /////////////////////////////////////////////////////////////////////////////////////////////////////
	void  TerrainComponentSystem::update(std::vector<TerrainComponent> &r)
	{
		
	}
	void  TerrainComponentSystem::render(std::vector<TerrainComponent> &r,Shader* shader)
	{
		for (int i = 0; i < r.size(); i++)
		r[i].terrain.render(shader);
	}

	void TerrainComponentSystem::update(TerrainComponent* r)
	{
	}
	void TerrainComponentSystem::render(TerrainComponent* r, Shader* shader)
	{
		r->terrain.render(shader);
	}
	void TerrainComponentSystem::load(TerrainComponent &r, std::string Path, std::string Texture, Vector3 Scale, bool Center , int NumPatches)
	{
		r.terrain.start(Path, Texture, Scale, Center, NumPatches);
		 
	}

	void TerrainComponentSystem::unload(TerrainComponent &r)
	{
		


	}
	///PHYSICS
	void PhysicsComponentSystem::load(PhysicsComponent& comp,float size)
	{
		//TODO add MASS 
		static int ID = 0;
		int Mass = 1;
		comp.groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0,0,0)));
		comp.shape = new btSphereShape(size); //radius
		btVector3 fallInertia;
		comp.shape->calculateLocalInertia(Mass, fallInertia);
		btRigidBody::btRigidBodyConstructionInfo
			groundRigidBodyCI(Mass, comp.groundMotionState, comp.shape, fallInertia);
		comp.object = new btRigidBody(groundRigidBodyCI);
		comp.object->setUserPointer(comp.parent);//each body has a userpointer which can be use to retrieve the entity
		ServiceLocator::getPE().world->addRigidBody(comp.object);
	};
	void  PhysicsComponent::set(Transform& transform)
	{
		object->activate(true);
		
		btQuaternion quat = btQuaternion(0,0,0,1);
		btTransform tr = btTransform(quat,btVector3(0,0,0));
		quat.setEuler(transform.getRot().x, transform.getRot().y, transform.getRot().z); //or quat.setEulerZYX depending on the ordering you want
		tr.setRotation(quat);
		object->setCenterOfMassTransform(tr);
		object->translate(btVector3(transform.getPos().x, transform.getPos().y, transform.getPos().z));
	}
	void PhysicsComponentSystem::unload(PhysicsComponent& comp)
	{
		ServiceLocator::getPE().world->removeRigidBody(comp.object);
		delete comp.shape;
		delete comp.object;
		delete comp.groundMotionState;
		
	};
	void PhysicsComponentSystem::update(PhysicsComponent& comp)
	{
		Transform transform;
		btTransform trans = comp.object->getWorldTransform();
		btVector3 y = trans.getOrigin();
		Vector3 x = Vector3(y.x(), y.y(), y.z());
		transform.setPos(x);

		btQuaternion z = trans.getRotation();
		Quaternion euler = Quaternion(z.x(), z.y(), z.z(), z.w());
		transform.setRot(euler.euler());
		comp.parent->setTransform(transform);
	}
	void PhysicsComponentSystem::update(std::vector<PhysicsComponent>& comp)
	{
		for (int i = 0; i < comp.size(); i++)
		{
			update(comp[i]);
		}
	}