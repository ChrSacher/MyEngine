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
AmbientLightComponent::~AmbientLightComponent() {};
DirectionalLightComponent::~DirectionalLightComponent() {};
void ComponentPosition::transformChanged(Transform& transform) { ServiceLocator::getCM().findComponent(this)->setTransform(transform); };

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





