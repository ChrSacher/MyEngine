#include "Component.h"

int Entity::id = 0;
	
	
	inline Transform* Component::GetTransform()             { return (parent->getTransform()); }
	inline const Transform& Component::GetTransform() const { return *parent->getTransform(); }
	
	void Component::SetParent(Entity* Parent) { parent = Parent; }

	GraphicsComponent* GraphicsComponent::create(std::string texturePath,std::string normalMap,std::string ObjectPath, Vector3 color,bool autoCenter)
	{
		return new GraphicsComponent(texturePath,normalMap,ObjectPath,color,autoCenter);
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
	}
	PhysicsComponent* PhysicsComponent::create()
	{
		return new PhysicsComponent();
	}
	PhysicsComponent::~PhysicsComponent(){};
	PhysicsComponent::PhysicsComponent(PhysicsWorld *World){world=World;}; //if Null no physics on object
	void PhysicsComponent::update(Entity* entity){};


	CollisionComponent* CollisionComponent::create()
	{
		return new CollisionComponent();
	}
	CollisionComponent::~CollisionComponent(){}
	CollisionComponent::CollisionComponent(CollisionWorld *World){world = World;}//if null then no collision
	void CollisionComponent::update(Entity* entity){}