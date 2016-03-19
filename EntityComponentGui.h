#pragma once
#include "UIrenderer.h"
#include "Scene.h"



class EntityComponentGui :public Entity::Listener , public Picker::Listener
{
public:
	friend class Scene;
	friend class DirectionalLightComponent;
	friend class AmbientLightComponent;
	friend class PhysicsComponent;
	friend class PhysicsWorld;
	friend class GraphicsComponent;
	friend class TerrainComponent;

	EntityComponentGui();
	void setScene(Scene* sc);
	void start();
	~EntityComponentGui();
	void entityChanged(Entity* ent) override;
	void pickerChanged(Picker* pick) override;
	void loadComponent(Component* pos,CEGUI::Window* window);
	void createComponent(Component* pos, CEGUI::Window* window);
	void realign();
	float getFloat(CEGUI::Window* win, std::string place);


	GUI gui;
	Scene* scene;
	Entity* currentEntity;
	CEGUI::PushButton* updateButton;
	CEGUI::Window *entityWindow; //only one exists so we can preload
	CEGUI::Window *mainWindow;
	CEGUI::Window *mainPanel;
	CEGUI::Window *physicsWindow;

	std::map<ComponentType, std::string> typeStrings;
	std::map<ComponentType, float> pixelSizes;
	std::map<unsigned int ,CEGUI::Window*> componentWindows;
	void uploadToEnity();
	void uploadToComponent(const CEGUI::EventArgs &e);
	void destroy();
private:
	void onTransformButton(const CEGUI::EventArgs &e);
	void increment(const CEGUI::EventArgs &e);
	void decrement(const CEGUI::EventArgs &e);
	

};

