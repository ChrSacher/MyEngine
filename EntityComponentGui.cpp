#include "EntityComponentGui.h"
#include "SpecComponent.h"

EntityComponentGui::EntityComponentGui() 
{ 
	currentEntity = NULL;
	pixelSizes.insert(std::make_pair(ENTITY, 400));
	pixelSizes.insert(std::make_pair(GRAPHICS, 400));
	pixelSizes.insert(std::make_pair(AMBIENT, 150));
	pixelSizes.insert(std::make_pair(SKYBOX, 625));
	pixelSizes.insert(std::make_pair(AUDIO, 430));
	pixelSizes.insert(std::make_pair(SCRIPT, 200));
	pixelSizes.insert(std::make_pair(TERRAIN, 625));
	pixelSizes.insert(std::make_pair(DIRECTIONAL, 625));
	pixelSizes.insert(std::make_pair(PHYSICS, 625));

	typeStrings.insert(std::make_pair(ENTITY, "Entity.layout"));
	typeStrings.insert(std::make_pair(GRAPHICS, "GraphicsComponent.layout"));
	typeStrings.insert(std::make_pair(AMBIENT, "AmbientLightComponent.layout"));
	typeStrings.insert(std::make_pair(SKYBOX, "SkyboxComponent.layout"));
	typeStrings.insert(std::make_pair(AUDIO, "AudioComponent.layout"));
	typeStrings.insert(std::make_pair(SCRIPT, "ScriptComponent.layout"));
	typeStrings.insert(std::make_pair(TERRAIN, "TerrainComponent.layout"));
	typeStrings.insert(std::make_pair(DIRECTIONAL, "DirectionalLight.layout"));
}
void EntityComponentGui::entityChanged(Entity* ent)
{
	//load stuff here into gui
	
	CEGUI::Window* panel = entityWindow; //gui.getChild(entityWindow, "Panel/__auto_container__");
	try
	{
		
		if (ent->getPhysics())
		{
			PhysicsComponent* w = ent->getPhysics()->getT<PhysicsComponent*>();
			gui.getChild(panel, "MassEditbox")->setText(std::to_string(w->getData().mass));
			static_cast<CEGUI::RadioButton*>(gui.getChild(panel, "PhysicsRadioButton"))->setSelected(true);
		}
		else
		{
			gui.getChild(panel, "MassEditbox")->disable();
			static_cast<CEGUI::RadioButton*>(gui.getChild(panel, "PhysicsRadioButton"))->setSelected(false);
		}
		Transform *trans = ent->getTransform();
		gui.getChild(panel, "PosXEditbox")->setText(std::to_string(trans->getPos().x));
		gui.getChild(panel, "PosYEditbox")->setText(std::to_string(trans->getPos().y));
		gui.getChild(panel, "PosZEditbox")->setText(std::to_string(trans->getPos().z));
		gui.getChild(panel, "RotXEditbox")->setText(std::to_string(trans->getRot().x));
		gui.getChild(panel, "RotYEditbox")->setText(std::to_string(trans->getRot().y));
		gui.getChild(panel, "RotZEditbox")->setText(std::to_string(trans->getRot().z));
		gui.getChild(panel, "ScaleXEditbox")->setText(std::to_string(trans->getScale().x));
		gui.getChild(panel, "ScaleYEditbox")->setText(std::to_string(trans->getScale().y));
		gui.getChild(panel, "ScaleZEditbox")->setText(std::to_string(trans->getScale().z));
		gui.getChild(panel, "NameEditbox")->setText(currentEntity->getName());
		

		//CHECK HERE WITH THE COMPONENTS ARE ALLREADY THERE AND THEN ONLY UPDATE THEM or IF THE NUMBER HAS CHANGED
		std::map<unsigned int, unsigned int> windows;
		for (unsigned int i = 0; i < currentEntity->getComponents().size(); i++)
		{

			auto&s = componentWindows.find(currentEntity->getComponents()[i]->getID());
			if (s == componentWindows.end())
			{
				CEGUI::Window* r = gui.loadLayoutFromFile(typeStrings[currentEntity->getComponents()[i]->type], std::to_string(i) + "_", Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 800.0f, pixelSizes[currentEntity->getComponents()[i]->type]));
				mainPanel->addChild(r);
				componentWindows.insert(std::make_pair(currentEntity->getComponents()[i]->getID(),r));
				r->setUserData(currentEntity->getComponents()[i]->get());
				createComponent(currentEntity->getComponents()[i]->get(), r);
				loadComponent(currentEntity->getComponents()[i]->get(), r);
				windows.insert(std::make_pair(currentEntity->getComponents()[i]->getID(),currentEntity->getComponents()[i]->getID()));
			}
			else
			{
				loadComponent(currentEntity->getComponents()[i]->get(), s->second);
				windows.insert(std::make_pair(currentEntity->getComponents()[i]->getID(), currentEntity->getComponents()[i]->getID()));
			}
			
		}
		for (auto& r = componentWindows.begin(); r != componentWindows.end(); r++)
		{
			auto& s = windows.find(r->first);
			if (s == windows.end())
			{
				r->second->destroy();
				componentWindows.erase(r);
			}
		}
		realign();
		
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
void  EntityComponentGui::createComponent(Component* pos, CEGUI::Window* window)
{
	//load functions and stuff

	ComponentType x = pos->getType();
	switch (x)
	{
		case GRAPHICS:
		{
			gui.getChild(window, "MeshpathEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "TexturepathEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "NormalpathEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "ColorXEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "ColorYEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "ColorZEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "SpecularIntensityEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "SpecularEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "EnabledRadioButton")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));


		}; break;
		case TERRAIN:
		{

		}; break;
		case AMBIENT:
		{
			gui.getChild(window, "ColorXEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "ColorYEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "ColorZEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "EnabledRadioButton")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
		}; break;
		case DIRECTIONAL:
		{
			gui.getChild(window, "ColorXEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "ColorYEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "ColorZEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "DirectionXEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "DirectionYEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "DirectionZEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "IntensityEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));

			gui.getChild(window, "EnabledRadioButton")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
		}; break;
		case SKYBOX:
		{

			gui.getChild(window, "PosXEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "NegXEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "PosYEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "NegYEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "PosZEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "NegZEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));

			gui.getChild(window, "ColorXEditBox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "ColorYEditBox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "ColorZEditBox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "EnabledRadioButton")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
		}; break;
		case PHYSICS:
		{

		}; break;
		case SCRIPT:
		{
			gui.getChild(window, "ScriptPathEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "EnabledRadioButton")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
		}; break;
		case AUDIO:
		{
			gui.getChild(window, "AudioPathEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "VolumeEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "SpeedEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "FadeoutEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "DopplerRadioButton")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "2DRadioButton")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "OcclusionRadioButton")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "LoopingRadioButton")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			gui.getChild(window, "EnabledRadioButton")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));

			gui.getChild(window, "ChannelCombobox")->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			static_cast<CEGUI::Combobox*>(gui.getChild(window, "ChannelCombobox"))->addItem(new CEGUI::ListboxTextItem("Channel::Master", CHMASTER));
			static_cast<CEGUI::Combobox*>(gui.getChild(window, "ChannelCombobox"))->addItem(new CEGUI::ListboxTextItem("Channel::FX", CHFX));
			static_cast<CEGUI::Combobox*>(gui.getChild(window, "ChannelCombobox"))->addItem(new CEGUI::ListboxTextItem("Channel::GUI", CHGUI));
			static_cast<CEGUI::Combobox*>(gui.getChild(window, "ChannelCombobox"))->addItem(new CEGUI::ListboxTextItem("Channel::VOICE", CHVOICE));
			static_cast<CEGUI::Combobox*>(gui.getChild(window, "ChannelCombobox"))->addItem(new CEGUI::ListboxTextItem("Channel::Music", CHMUSIC));
			static_cast<CEGUI::Combobox*>(gui.getChild(window, "ChannelCombobox"))->addItem(new CEGUI::ListboxTextItem("Channel::Master", CHMASTER));

		}; break;
	}
}

void EntityComponentGui::loadComponent(Component* pos,CEGUI::Window* window)
{
	//load in data
	ComponentType x = pos->getType();
	Component* comp = pos;
	switch (x)
	{
		case GRAPHICS:
		{
			GraphicsComponent* w = static_cast<GraphicsComponent*>(comp);
			gui.getChild(window, "MeshpathEditbox")->setText(w->mesh.getPath());
			gui.getChild(window, "TexturepathEditbox")->setText(w->material.texture.texturepath);
			gui.getChild(window, "NormalpathEditbox")->setText(w->material.normalMap.texturepath);
			gui.getChild(window, "ColorXEditbox")->setText(std::to_string(w->material.color.x));
			gui.getChild(window, "ColorYEditbox")->setText(std::to_string(w->material.color.y));
			gui.getChild(window, "ColorZEditbox")->setText(std::to_string(w->material.color.z));
			gui.getChild(window, "SpecularIntensityEditbox")->setText(std::to_string(w->material.specularIntensity));
			gui.getChild(window, "SpecularEditbox")->setText(std::to_string(w->material.specularPower));
			static_cast<CEGUI::RadioButton*>(gui.getChild(window, "EnabledRadioButton"))->setSelected(comp->getEnabled());

		}; break;
		case TERRAIN:
		{
					
		}; break;
		case AMBIENT:
		{
			AmbientLightComponent* w = static_cast<AmbientLightComponent*>(comp);
			gui.getChild(window, "ColorXEditbox")->setText(std::to_string(w->getAmbient().x));
			gui.getChild(window, "ColorYEditbox")->setText(std::to_string(w->getAmbient().y));
			gui.getChild(window, "ColorZEditbox")->setText(std::to_string(w->getAmbient().z));
			static_cast<CEGUI::RadioButton*>(gui.getChild(window, "EnabledRadioButton"))->setSelected(comp->getEnabled());
		}; break;
		case DIRECTIONAL:
		{
			DirectionalLightComponent* w = static_cast<DirectionalLightComponent*>(comp);
			gui.getChild(window, "ColorXEditbox")->setText(std::to_string(w->getBaseLight().getColor().x));
			gui.getChild(window, "ColorYEditbox")->setText(std::to_string(w->getBaseLight().getColor().y));
			gui.getChild(window, "ColorZEditbox")->setText(std::to_string(w->getBaseLight().getColor().z));
			gui.getChild(window, "DirectionXEditbox")->setText(std::to_string(w->getDirection().x));
			gui.getChild(window, "DirectionYEditbox")->setText(std::to_string(w->getDirection().y));
			gui.getChild(window, "DirectionYEditbox")->setText(std::to_string(w->getDirection().z));
			gui.getChild(window, "IntensityEditbox")->setText(std::to_string(w->getBaseLight().getIntensity()));

			gui.getChild(window, "EnabledRadioButton")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			static_cast<CEGUI::RadioButton*>(gui.getChild(window, "EnabledRadioButton"))->setSelected(comp->getEnabled());
		}; break;
		case SKYBOX:
		{
			
			SkyBoxComponent* w = static_cast<SkyBoxComponent*>(comp);
			auto& s = w->getDirAndFile();
			gui.getChild(window, "PosXEditbox")->setText(s[0]);
			gui.getChild(window, "NegXEditbox")->setText(s[1]);
			gui.getChild(window, "PosYEditbox")->setText(s[2]);
			gui.getChild(window, "NegYEditbox")->setText(s[3]);
			gui.getChild(window, "PosZEditbox")->setText(s[4]);
			gui.getChild(window, "NegZEditbox")->setText(s[5]);

			gui.getChild(window, "ColorXEditBox")->setText(std::to_string(w->getColor().x));
			gui.getChild(window, "ColorYEditBox")->setText(std::to_string(w->getColor().x));
			gui.getChild(window, "ColorZEditBox")->setText(std::to_string(w->getColor().x));
			static_cast<CEGUI::RadioButton*>(gui.getChild(window, "EnabledRadioButton"))->setSelected(comp->getEnabled());
		}; break;
		case PHYSICS:
		{
			static_cast<CEGUI::RadioButton*>(gui.getChild(window, "EnabledRadioButton"))->setSelected(comp->getEnabled());
		}; break;
		case SCRIPT:
		{
			ScriptComponent* w = static_cast<ScriptComponent*>(comp);
			gui.getChild(window, "ScriptPathEditbox")->setText(w->script.getPath());
			static_cast<CEGUI::RadioButton*>(gui.getChild(window, "EnabledRadioButton"))->setSelected(comp->getEnabled());
		}; break;
		case AUDIO:
		{
			AudioComponent* w = static_cast<AudioComponent*>(comp);
			gui.getChild(window, "AudioPathEditbox")->setText(w->path);
			gui.getChild(window, "VolumeEditbox")->setText(std::to_string(w->sound.getVolume()));
			gui.getChild(window, "SpeedEditbox")->setText(std::to_string(w->sound.getSpeed()));
			static_cast<CEGUI::RadioButton*>(gui.getChild(window, "DopplerRadioButton"))->setSelected(w->sound.getDoppler());
			static_cast<CEGUI::RadioButton*>(gui.getChild(window, "2DRadioButton"))->setSelected(w->sound.is2D());
			static_cast<CEGUI::RadioButton*>(gui.getChild(window, "OcclusionRadioButton"))->setSelected(w->sound.getOcclusion());
			static_cast<CEGUI::RadioButton*>(gui.getChild(window, "LoopingRadioButton"))->setSelected(w->sound.isLooping());
			static_cast<CEGUI::RadioButton*>(gui.getChild(window, "EnabledRadioButton"))->setSelected(comp->getEnabled());

			gui.getChild(window, "ChannelCombobox")->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::uploadToComponent, this));
			static_cast<CEGUI::RadioButton*>(gui.getChild(window, "EnabledRadioButton"))->setSelected(comp->getEnabled());
		}; break;
	}
}

float EntityComponentGui::getFloat(CEGUI::Window* win, std::string place)
{
	return std::stof(std::string(gui.getChild(win, place)->getText().c_str()));
}
void EntityComponentGui::uploadToComponent(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs &w = static_cast<const CEGUI::WindowEventArgs&>(e);
	CEGUI::Window* frame = w.window->getParent();
	Component* currentComp = static_cast<Component*>(frame->getUserData());
	switch (currentComp->getType())
	{
	case GRAPHICS:
	{
		GraphicsComponent* w = static_cast<GraphicsComponent*>(currentComp);
		if (w->mesh.getPath() != gui.getChild(frame, "MeshpathEditbox")->getText()) w->mesh = Mesh(std::string(gui.getChild(frame, "MeshpathEditbox")->getText().c_str()));
		if (w->material.texture.texturepath != gui.getChild(frame, "TexturepathEditbox")->getText()) w->material.texture = TextureCache::getTexture(std::string(gui.getChild(frame, "TexturepathEditbox")->getText().c_str()));
		if (w->material.texture.texturepath != gui.getChild(frame, "NormalpathEditbox")->getText()) w->material.normalMap = NormalCache::getTexture(std::string(gui.getChild(frame, "NormalpathEditbox")->getText().c_str()));
		w->material.color.x = getFloat(frame, "ColorXEditbox");
		w->material.color.y = getFloat(frame, "ColorYEditbox");
		w->material.color.z = getFloat(frame, "ColorZEditbox");
		w->material.specularIntensity = getFloat(frame, "SpecularIntensityEditbox");
		w->material.specularPower = getFloat(frame, "SpecularEditbox");
		currentComp->setEnabled(static_cast<CEGUI::RadioButton*>(gui.getChild(frame, "EnabledRadioButton"))->isSelected());
	}; break;
	case TERRAIN:
	{
		
	}; break;
	case AMBIENT:
	{
		AmbientLightComponent* w = static_cast<AmbientLightComponent*>(currentComp);
		w->setAmbient(Vector3(getFloat(frame, "ColorXEditbox"),
										getFloat(frame, "ColorYEditbox"),
										getFloat(frame, "ColorZEditbox")));
		currentComp->setEnabled(static_cast<CEGUI::RadioButton*>(gui.getChild(frame, "EnabledRadioButton"))->isSelected());
	}; break;
	case DIRECTIONAL:
	{
		DirectionalLightComponent* w = static_cast<DirectionalLightComponent*>(currentComp);
		w->setBase(BaseLight(Vector3(getFloat(frame, "ColorXEditbox"),
			getFloat(frame, "ColorYEditbox"),
			getFloat(frame, "ColorZEditbox")), getFloat(frame, "IntensityEditbox")));

		w->setDirection(Vector3(getFloat(frame, "ColorXEditbox"),
			getFloat(frame, "ColorYEditbox"),
			getFloat(frame, "ColorZEditbox")));
		currentComp->setEnabled(static_cast<CEGUI::RadioButton*>(gui.getChild(frame, "EnabledRadioButton"))->isSelected());

	}; break;
	case SKYBOX:
	{
		SkyBoxComponent* w = static_cast<SkyBoxComponent*>(currentComp);
		std::vector<std::string> paths;
		paths.push_back(std::string(gui.getChild(frame, "PosXEditbox")->getText().c_str()));
		paths.push_back(std::string(gui.getChild(frame, "NegXEditbox")->getText().c_str()));
		paths.push_back(std::string(gui.getChild(frame, "PosYEditbox")->getText().c_str()));
		paths.push_back(std::string(gui.getChild(frame, "NegYEditbox")->getText().c_str()));
		paths.push_back(std::string(gui.getChild(frame, "PosZEditbox")->getText().c_str()));
		paths.push_back(std::string(gui.getChild(frame, "NegZEditbox")->getText().c_str()));

		w->setSkyboxTexture(paths[0], paths[1], paths[2], paths[3], paths[4], paths[5]);
		w->setColor(Vector3(getFloat(frame, "ColorXEditbox"),
			getFloat(frame, "ColorYEditbox"),
			getFloat(frame, "ColorZEditbox")));

		
		currentComp->setEnabled(static_cast<CEGUI::RadioButton*>(gui.getChild(frame, "EnabledRadioButton"))->isSelected());
	}; break;
	case PHYSICS:
	{

	}; break;
	case SCRIPT:
	{
		ScriptComponent* w = static_cast<ScriptComponent*>(currentComp);
		w->reload(std::string(gui.getChild(frame, "ScriptpathEditbox")->getText().c_str()));
		currentComp->setEnabled(static_cast<CEGUI::RadioButton*>(gui.getChild(frame, "EnabledRadioButton"))->isSelected());
	}; break;
	case AUDIO:
	{
		AudioComponent* w = static_cast<AudioComponent*>(currentComp);
		w->reload(std::string(gui.getChild(frame, "AudioPathEditbox")->getText().c_str()), getFloat(frame, "VolumeEditbox"));
		w->sound.setSpeed(getFloat(frame, "SpeedEditbox"));
		w->sound.setDoppler(static_cast<CEGUI::RadioButton*>(gui.getChild(frame, "DopplerRadioButton"))->isSelected());
		w->sound.set2D(static_cast<CEGUI::RadioButton*>(gui.getChild(frame, "2DRadioButton"))->isSelected());
		w->sound.setOcclusion(static_cast<CEGUI::RadioButton*>(gui.getChild(frame, "OcclusionRadioButton"))->isSelected());
		w->sound.setLooping(static_cast<CEGUI::RadioButton*>(gui.getChild(frame, "LoopingRadioButton"))->isSelected());

		currentComp->setEnabled(static_cast<CEGUI::RadioButton*>(gui.getChild(frame, "EnabledRadioButton"))->isSelected());
	}; break;
	}
	loadComponent(currentComp, frame);
}

void EntityComponentGui::realign()
{
	float yMove = pixelSizes[ENTITY] + pixelSizes[PHYSICS];
	for (auto& r = componentWindows.begin(); r != componentWindows.end(); r++)
	{
		ComponentType x = static_cast<Component*>(r->second->getUserData())->getType();
		gui.setWidgetDestRect(r->second, Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, yMove, 800.0f, pixelSizes[x]));
		yMove += pixelSizes[x];
	}
}
void EntityComponentGui::uploadToEnity()
{
	if (currentEntity == NULL) return;
	CEGUI::Window* panel = entityWindow;// gui.getChild(entityWindow, "Panel/__auto_container__");
	Vector3 pos;
	Vector3 rot;
	Vector3 sca;

	pos.x = std::stof(std::string(gui.getChild(panel, "PosXEditbox")->getText().c_str()));
	pos.y = std::stof(std::string(gui.getChild(panel, "PosYEditbox")->getText().c_str()));
	pos.z = std::stof(std::string(gui.getChild(panel, "PosZEditbox")->getText().c_str()));
	rot.x = std::stof(std::string(gui.getChild(panel, "RotXEditbox")->getText().c_str()));
	rot.y = std::stof(std::string(gui.getChild(panel, "RotYEditbox")->getText().c_str()));
	rot.z = std::stof(std::string(gui.getChild(panel, "RotZEditbox")->getText().c_str()));
	sca.x = std::stof(std::string(gui.getChild(panel, "ScaleXEditbox")->getText().c_str()));
	sca.y = std::stof(std::string(gui.getChild(panel, "ScaleYEditbox")->getText().c_str()));
	sca.z = std::stof(std::string(gui.getChild(panel, "ScaleZEditbox")->getText().c_str()));
	
	currentEntity->setName(std::string(gui.getChild(panel, "NameEditbox")->getText().c_str()));
	Transform r(pos,rot,sca);
	currentEntity->setTransform(r);
}

void EntityComponentGui::onTransformButton(const CEGUI::EventArgs &e)
{
	
	uploadToEnity();
}


void EntityComponentGui::pickerChanged(Picker* pick)
{
	if (currentEntity != NULL); //currentEntity->removeListener(this);

	currentEntity = pick->getPick(); 
	mainWindow->show();
	//close gui here
	if (currentEntity == NULL)
	{
		mainWindow->hide();
		for (auto& r = componentWindows.begin(); r != componentWindows.end(); r++)
		{
			r->second->getParent()->removeChild(r->second);
			CEGUI::WindowManager::getSingleton().destroyWindow(r->second);
		}
		componentWindows.clear();
		return;
	}
	
	//currentEntity->addListener(this);
	entityChanged(currentEntity);
}
void EntityComponentGui::setScene(Scene* sc)
{
	scene = sc;
	scene->picker.addListener(this);
}
void EntityComponentGui::start()
{
	try {
		gui.init("res/GUI");
		gui.loadScheme("TaharezLook.scheme");
		gui.loadScheme("WindowsLook.scheme");
		gui.loadScheme("GlossySerpentFHD.scheme");
		gui.setFont("Jura-13");
		gui.setMouseCursor("TaharezLook/MouseArrow");

		mainWindow = gui.createWidget("WindowsLook/FrameWindow", Vector4(), Vector4(0.0f, 0.0f, 850.0f, 600.0f), "mainWindow");
		mainPanel = gui.createWidget("WindowsLook/ScrollablePane", Vector4(0, 0, 1, 1), Vector4(), "Panel");
		mainWindow->addChild(mainPanel);
		entityWindow = gui.loadLayoutFromFile("Entity.layout", "Ent_", Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.f, 800.0f,pixelSizes[ENTITY]));
		physicsWindow = gui.loadLayoutFromFile("PhysicsComponent.layout", "Phy_", Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, pixelSizes[ENTITY], 800.0f, pixelSizes[PHYSICS]));

		CEGUI::Window* panel = entityWindow; //gui.getChild(entityWindow, "Panel/__auto_container__");
		gui.getChild(panel, "PosXEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::onTransformButton, this));
		gui.getChild(panel, "PosYEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::onTransformButton, this));
		gui.getChild(panel, "PosZEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::onTransformButton, this));
		gui.getChild(panel, "RotXEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::onTransformButton, this));
		gui.getChild(panel, "RotYEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::onTransformButton, this));
		gui.getChild(panel, "RotZEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::onTransformButton, this));
		gui.getChild(panel, "ScaleXEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::onTransformButton, this));
		gui.getChild(panel, "ScaleYEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::onTransformButton, this));
		gui.getChild(panel, "ScaleZEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::onTransformButton, this));
		gui.getChild(panel, "NameEditbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EntityComponentGui::onTransformButton, this));

		//static_cast<CEGUI::Window*>(entityWindow->getChild("Panel/__auto_container__/PostXEditBox"))->setText("test");

		mainPanel->addChild(entityWindow);
		mainPanel->addChild(physicsWindow);
	}
	catch (std::exception &e)
	{

	}
}

void EntityComponentGui::destroy()
{
	mainWindow->getParent()->removeChild(mainWindow);
	CEGUI::WindowManager::getSingleton().destroyWindow(mainWindow);
	gui.destroy();
}
EntityComponentGui::~EntityComponentGui()
{
	
}

void EntityComponentGui::increment(const CEGUI::EventArgs &e)
{
	

}
void EntityComponentGui::decrement(const CEGUI::EventArgs &e)
{

}
