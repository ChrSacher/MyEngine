#include "SpecComponent.h"

//Component Manager needs to be a static thing inside the Engine ...done
//TODO IMPLEMENT A SYSTEM WHERE I CAN STORE RAW DATA IN THOSE VECTORS
//IF THE VECTOR GETS RESIZED ALL COMPONENTS SHOULD UPDATE THEIR PARRENT
//THIS IS DONE BY STORING THE COMPONENT ARRAY POSITION INSIDE THE THE ACTUAL COMPONENT ... done

//create Functions should be moved to Content Manager
//A component Pointer should be returned done
//delete Functions should be moved to Content Manager done
//creatXXX(resources for emplace back) to avoid copying 
//this will be  a singelton ..... done
//add this to ServiceLocator
//this gives me the freedom of setting actuall order of rendering
class ComponentManager
{
public:

	friend class GraphicComponentSystem;
	friend class TerrainComponentSystem;
	friend class SkyboxComponentSystem;
	friend class LightComponentSystem;
	friend class PhysicsComponentSystem;
	friend class AudioComponentSystem;
	friend class ScriptComponentSystem;
	//start the Manager
	void initialize();
	//destroy Manager and all components
	void destroy();
	//update all Components
	void update();
	//render all Components
	void render(Shader* shader, Camera3d* camera);
	ComponentManager()
	{
	}
	~ComponentManager()
	{
	}
	ComponentPosition* createGraphics(std::string texturePath, std::string normalMap, std::string ObjectPath, Vector3 color = Vector3(1.0f, 1.0f, 1.0f), bool autoCenter = false);
	ComponentPosition* createTerrain(std::string Path, std::string Texture, Vector3 Scale = Vector3(1.0f, 1.0f, 1.0f), bool Center = false);
	ComponentPosition* createAmbient(Vector3 Color);
	ComponentPosition* createDirectional(Vector3 Color = Vector3(1.0f, 1.0f, 1.0f), float Intensity = 0.2f, Vector3 Dir = Vector3(1.0f, 1.0f, 1.0f));
	ComponentPosition* createSkyBox(Vector3 color, std::string Directory, std::string posx, std::string negx, std::string posy, std::string negy, std::string posz, std::string negz);
	ComponentPosition* createPhysicComponent(PhysicsData& Data);
	ComponentPosition* create2DAudio(std::string audiopath = "", float Volume = 1.0f);
	ComponentPosition* create3DAudio(std::string audiopath = "", Vector3 position = Vector3(), float Volume = 1.0f);
	ComponentPosition* createScript(std::string path);

	void deleteComponent(ComponentPosition* Posi);
	void renderComponent(ComponentPosition* comp, Shader* shader, Camera3d* camera);
	void renderComponent(Component* comp, Shader* shader, Camera3d* camera);
	Component* findComponent(ComponentPosition* Posi);


	void addScriptListener(ScriptComponentSystem::Listener* r);
	void removeScriptListener(ScriptComponentSystem::Listener* r);
private:
	GraphicComponentSystem GCS;
	LightComponentSystem LCS;
	SkyBoxComponentSystem SCS;
	TerrainComponentSystem TCS;
	PhysicsComponentSystem PCS;
	AudioComponentSystem  ACS;
	ScriptComponentSystem SCCS;

	std::vector<GraphicsComponent> graphics;
	std::vector<TerrainComponent> terrains;
	std::vector<AmbientLightComponent> ambients;
	std::vector<DirectionalLightComponent> directionals;
	std::vector<PhysicsComponent> physics;
	std::vector<SkyBoxComponent> skies;
	std::vector<AudioComponent> audios;
	std::vector<ScriptComponent> scripts;
	std::map<ComponentType, std::map<unsigned int, ComponentPosition*>> positions; //positions begin at 0



};

class NullComponentManager : public ComponentManager
{
public:
	//start the Manager
	void startup() {};
	//destroy Manager and all components
	void shutdown() {};
	static ComponentManager& get() {};
	//update all Components
	void update() {};
	//render all Components
	void render(Shader* shader, Camera3d* camera) {};

	ComponentPosition* createGraphics(std::string texturePath, std::string normalMap, std::string ObjectPath, Vector3 color = Vector3(1.0f, 1.0f, 1.0f), bool autoCenter = false) {};
	ComponentPosition* createTerrain(std::string Path, std::string Texture, Vector3 Scale = Vector3(1.0f, 1.0f, 1.0f), bool Center = false) {};
	ComponentPosition* createAmbient(Vector3 Color) {};
	ComponentPosition* createDirectional(Vector3 Color = Vector3(1.0f, 1.0f, 1.0f), float Intensity = 0.2f, Vector3 Dir = Vector3(1.0f, 1.0f, 1.0f)) {};
	ComponentPosition* createSkyBox(Vector3 color, std::string Directory, std::string posx, std::string negx, std::string posy, std::string negy, std::string posz, std::string negz) {};
	void deleteComponent(ComponentPosition* Posi) {};
	Component* findComponent(ComponentPosition* Posi) {};
private:
};