#include "SpecComponent.h"
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


	
	SkyBoxComponent::SkyBoxComponent()
	{
		type = SKYBOX;
	}
	SkyBoxComponent::~SkyBoxComponent()
	{
	}




	void AudioComponent::reload(std::string audiopath , float Volume)
	{
		ServiceLocator::getCM().ACS.unload(*this);
		ServiceLocator::getCM().ACS.load(*this, audiopath, Volume);
	}
	void AudioComponent::reload(std::string audiopath , Vector3 position , float Volume)
	{
		ServiceLocator::getCM().ACS.unload(*this);
		ServiceLocator::getCM().ACS.load(*this, audiopath,position, Volume);
	}









	GraphicsComponent::~GraphicsComponent() 
	{

	}

	GraphicsComponent::GraphicsComponent()
	{
		renderable = true;
		type = GRAPHICS;
	}
	
	void GraphicsComponent::reload(std::string &texturePath, std::string &normalMap, std::string &ObjectPath, Vector3 color, bool autoCenter)
	{
		ServiceLocator::getCM().GCS.unload(*this);
		ServiceLocator::getCM().GCS.load(*this,texturePath, normalMap, ObjectPath, color, autoCenter);
	}

	
	TerrainComponent::~TerrainComponent()
	{

	}
	

	TerrainComponent::TerrainComponent()
	{
		parent = NULL;
		type = TERRAIN;
	}

	void TerrainComponent::load(Entity* Parent)
	{
		parent = Parent;
		for (unsigned int i = 0; i < terrain.patches.size(); i++)
		{
			terrain.patches[i].object->setUserPointer((void*)parent);
			terrain.patches[i].terrainPhysics->setUserPointer((void*)parent);//each body has a userpointer which can be use to retrieve the entity
		}
		
	}
	

	void ScriptComponent::load(Entity* Parent)
	{
		ChaiScript* sc = &script.state;
		sc->add(user_type<Entity>(), "Entity");
		sc->add(const_var(Parent),"This");
		sc->add(fun(&Entity::getTransform),"transform");
		sc->add(fun(&Entity::getPhysics), "physics");
		//TODO FINISH THIS
	}
	void ScriptComponent::reload(std::string &path)
	{
		ServiceLocator::getCM().SCCS.unload(*this);
		ServiceLocator::getCM().SCCS.load(*this,path);
	}

	static void setDirects(std::vector<DirectionalLightComponent> &ambients,Shader *shader)
	{
		
	}


	
	void PhysicsComponent::reload(PhysicsData &Data)
	{
		ServiceLocator::getCM().PCS.unload(*this);
		ServiceLocator::getCM().PCS.load(*this,Data);
		load(parent);
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
		for (unsigned int i = 0; i < r.size(); i++)
		{
			
			if(r[i].enabled) render(r[i], shader, camera);
		}
	
	}
	void GraphicComponentSystem::render(GraphicsComponent &r, Shader* shader, Camera3d* camera)
	{
		shader->use();
		if (!r.renderable) return;
			bool canRender = true;
			
			if (camera != NULL)
			{
				if (r.transform->getPos().distance(camera->getPos()) > camera->getZ().y) canRender = false; //don't draw when out of range
				if (camera->isBehind(r.transform->getPos()))  canRender = false; //don't draw behind camera

			}
			if (canRender)
			{
				if (shader != NULL)
				{
					r.transform->update(shader);
					if (camera != NULL) { shader->setMVP(*r.transform->getMatrix()); }
					else { shader->setMVP(camera->GetViewProjection(), *r.transform->getMatrix()); }
					r.material.update(shader);
				}
				r.mesh.draw();
			}
	}


	//SKYBOX//////////////////////////////////////////////////////////////////////////////////////////
	void SkyBoxComponentSystem::render(std::vector<SkyBoxComponent> &r, Camera3d* camera)
	{
		for (unsigned int i = 0; i < r.size();i++) if(r[i].enabled) r[i].renderSkybox(camera);
	}
	void SkyBoxComponentSystem::load(SkyBoxComponent& r,Vector3 Color, std::vector<std::string> &paths)
	{
		r.shader = new Shader();
		r.shader->addVertexShader("res/Shaders/Skybox.vert");
		r.shader->addFragmentShader("res/Shaders/Skybox.frag");
		r.shader->addAttribute("position");
		r.shader->linkShaders();
		r.color = Color;
		r.vao = 0;
		r.vbo = 0;
		r.loadSkybox(paths[0], paths[1], paths[2], paths[3], paths[4], paths[5]);
		r.setColor(Color);
		
	}
	void SkyBoxComponentSystem::unload(SkyBoxComponent& r)
	{
		glDeleteVertexArrays(1, &r.vao);
		glDeleteBuffers(1, &r.vbo);
		r.cube.releaseCubemap();
		delete(r.shader);
		r.shader = NULL;
	}
	void SkyBoxComponentSystem::update(std::vector<SkyBoxComponent> &r)
	{
	}

	//PHYSICS









	//LIGHT//////////////////////////////////////////////////////////////////////////////////////////////////
	void LightComponentSystem::update(std::vector<DirectionalLightComponent> &r)
	{
		
	}

	void LightComponentSystem::update(std::vector<AmbientLightComponent> &rr)
	{
		
	}
	void  LightComponentSystem::load(AmbientLightComponent &light, Vector3 Ambient)
	{
		light.setAmbient(Ambient);
	}
	void LightComponentSystem::load(DirectionalLightComponent &light, BaseLight &Base , Vector3 &Dir )
	{
		light.direction = Dir;
		light.base = Base;
	}
	void LightComponentSystem::render(std::vector<DirectionalLightComponent> &r, Shader* shader)
	{
		int p = 0;
		for (unsigned int i = 0; i < r.size(); i++)
		{
			if (!r[i].enabled) continue;
			//shader->setUniform("directionalLight[" + std::to_string(i) + "].direction", r[i].light.direction);
			//shader->setUniform("directionalLight[" + std::to_string(i) + "].base.color", r[i].light.base.getColor());
			//shader->setUniform("directionalLight[" + std::to_string(i) + "].base.intensity", r[i].light.base.getIntensity());
			shader->setUniform("directionalLight.direction", r[p].direction);
			shader->setUniform("directionalLight.base.color", r[p].base.getColor());
			shader->setUniform("directionalLight.base.intensity", r[p].base.getIntensity());
			p++;
		}
	}

	void LightComponentSystem::render(std::vector<AmbientLightComponent> &r,Shader* shader)
	{
		Vector3 color = Vector3(0, 0, 0);
		if (r.size() > 0)
		{
			int p = 0;
			for (unsigned int i = 0; i < r.size(); i++)
			{
				if (!r[i].enabled) continue;
				color += r[i].getAmbient();
				p++;
			}
			color /= (float)(p);
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
		for (unsigned int i = 0; i < r.size(); i++)
		if(r[i].enabled) r[i].terrain.render(shader);
	}

	void TerrainComponentSystem::update(TerrainComponent* r)
	{
	}
	void TerrainComponentSystem::render(TerrainComponent* r, Shader* shader)
	{
		r->terrain.render(shader);
	}
	void TerrainComponentSystem::load(TerrainComponent &r, std::string Path, std::string Texture, Vector3 Scale, bool Center, int NumPatches)
	{
		r.terrain.start(Path, Texture, Scale, Center, NumPatches);
		//move into PhysicsComponent
		for (unsigned int i = 0; i < r.terrain.patches.size(); i++)
		{
			TerrainPatch* patch = &r.terrain.patches[i];
			btTriangleMesh *mesh = new btTriangleMesh();
			for (unsigned int i = 0; i < patch->vertices.size() - 2; i++)
			{
				mesh->addTriangle(patch->V3BF(patch->vertices[i].pos), patch->V3BF(patch->vertices[i + 1].pos), patch->V3BF(patch->vertices[i + 2].pos));
			}
			patch->terrainPhysics = new btBvhTriangleMeshShape(mesh, false);
			patch->terrainPhysics->setLocalScaling(patch->V3BF(r.terrain.transform.getScale()));
			btTransform tr;
			tr.setIdentity();
			tr.setOrigin(btVector3(0,0,0));
			patch->groundMotionState = new btDefaultMotionState(tr);
			patch->object = new btRigidBody(0, patch->groundMotionState, patch->terrainPhysics);
			patch->object->translate(patch->V3BF(r.terrain.transform.getPos()));
			ServiceLocator::getPE().world->addRigidBody(patch->object);
		
		}
	}

	void TerrainComponentSystem::unload(TerrainComponent &r)
	{
		for (unsigned int i = 0; i < r.terrain.patches.size(); i++)
		{
			ServiceLocator::getPE().world->removeRigidBody(r.terrain.patches[i].object);
			delete r.terrain.patches[i].terrainPhysics;
			delete r.terrain.patches[i].groundMotionState;
			delete r.terrain.patches[i].object;
		}

	}

	void  TerrainComponentSystem::loadPhysics(TerrainPatch &patch)
	{
		
	}
	///PHYSICS
	void PhysicsComponentSystem::load(PhysicsComponent& comp, PhysicsData &Data)
	{
		//TODO add MASS 
		//TODO switch statement for different kindes of shapes
		static int ID = 0;
		int Mass = 1;
		comp.groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0,0,0)));
		comp.shape = new btSphereShape(1); //radius
		
		btVector3 fallInertia;
		comp.shape->calculateLocalInertia(Data.mass, fallInertia);
		btRigidBody::btRigidBodyConstructionInfo
			groundRigidBodyCI(Data.mass, comp.groundMotionState, comp.shape, fallInertia);
		comp.object = new btRigidBody(groundRigidBodyCI);
	
		ServiceLocator::getPE().world->addRigidBody(comp.object);
	};
	const float RAD2DEG = 180.0f / 3.141593f;
	const float DEG2RAD =  3.141593f /  180.0f ;
	void  PhysicsComponent::setTransform(Transform& Trans)
	{
		object->activate(true);
		btTransform Tr;
		object->getMotionState()->getWorldTransform(Tr);
		Tr.setOrigin(btVector3(Trans.getPos().x, Trans.getPos().y, Trans.getPos().z));  // or whatever
		btQuaternion quat = btQuaternion(0, 0, 0, 1);
		quat.setEuler(Trans.getRot().y * DEG2RAD, Trans.getRot().x * DEG2RAD, Trans.getRot().z * DEG2RAD); //or quat.setEulerZYX depending on the ordering you want
		shape->setLocalScaling(btVector3(Trans.getScale().x, Trans.getScale().y, Trans.getScale().z));
		Tr.setRotation(quat);
		object->getMotionState()->setWorldTransform(Tr);
		object->setCenterOfMassTransform(Tr);
		//ServiceLocator::getPE().world->updateAabb();

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
		
		Transform* w = comp.parent->getTransform();
		btTransform trans = comp.object->getCenterOfMassTransform();
		btVector3 y = trans.getOrigin();
		Vector3 x = Vector3(y.x(), y.y(), y.z());
		w->setPos(x);

		btQuaternion z = trans.getRotation();
		Quaternion euler = Quaternion(z.x(), z.y(), z.z(), z.w());
		Vector3 e = euler.euler();
		e *= RAD2DEG;
		w->setRot(e);
		Vector3 sca = PhysicsConversion::V3TBT3(comp.shape->getLocalScaling());
		w->setScale(sca);
		w->calculateMatrix();
		//comp.parent->setTransform(*w);
	}
	void PhysicsComponentSystem::update(std::vector<PhysicsComponent>& comp)
	{
		for (unsigned int i = 0; i < comp.size(); i++)
		{
			if(comp[i].enabled) update(comp[i]);
		}
	}
	void PhysicsComponent::load(Entity* Parent)
	{
		parent = Parent;
		if (parent != NULL) setTransform(*parent->getTransform());
		shape->setUserPointer(parent);
		object->setUserPointer(parent);//each body has a userpointer which can be use to retrieve the entity
		return;
	}


	
	void  AudioComponentSystem::update(std::vector<AudioComponent> &r)
	{
		for (unsigned int i = 0; i < r.size(); i++)
		{
			r[i].sound.setPosition(Vec(r[i].parent->getTransform()->getPos()));
		}
	}

	float occlusionTest(const YSE::Vec & source, const YSE::Vec & listener) 
	{
		
		
		// this code is made up. It won't work but is supposed to give you an
		// idea of how to implement an occlusion test function.
		auto found = ServiceLocator::getPE().rayCast(Vector3(source.x,source.y,source.z), Vector3(listener.x, listener.y, listener.z));
		if (found.m_collisionObjects.size() > 0) {
			return 1; //found[0]->density(); density is basically what is between
		}
		return 0;
	}
	AudioComponentSystem::AudioComponentSystem()
	{

		// enable occlusion globally
		YSE::System().occlusionCallback(occlusionTest);
	}
	void  AudioComponentSystem::load(AudioComponent &r, std::string audiopath, float Volume)
	{
		r.is2D = true;
		r.path = audiopath;
		ServiceLocator::getAudio().play2D(&r.sound, audiopath, Volume);
	}
	void AudioComponentSystem::load(AudioComponent &r, std::string audiopath, Vector3 position, float Volume)
	{
		r.is2D = false;
		r.path = audiopath;
		ServiceLocator::getAudio().play3D(&r.sound, audiopath, position,Volume);
	}
	void AudioComponentSystem::unload(AudioComponent &r)
	{
		r.sound.stop();
	}

	void ScriptComponentSystem::update(std::vector<ScriptComponent> &r)
	{
		for (unsigned int i = 0; i < r.size(); i++)
		{
			r[i].script.update();
		}
	}
	void ScriptComponentSystem::load(ScriptComponent &r, std::string path)
	{
		r.script.hasTestRun = false;
		r.script.hasUpdate = true;
		r.script.isValid = true;
		static unsigned int I = 0;
		r.script.ID = ++I;
		try
		{
			r.script.state.eval_file(path);
		}
		catch (const std::exception &e)
		{
			std::cout << e.what() << std::endl;
			r.script.isValid = false;
			r.script.hasUpdate = false;
			r.script.hasTestRun = true;
			return;
		}
		ScriptCreated(&r.script);
		for (int i = 0; i < numEvents; i++)
		{
			r.script.eventHandlers.push_back(std::map<int, std::string>());
		}

		ChaiScript* sc = &r.script.state;
		sc->add(user_type<GameState>(), "gameState");
		sc->add(fun(&GameState::cameramove), "cameraMove");
		sc->add(fun(&GameState::drawgrid), "drawGrid");
		sc->add(fun(&GameState::escapemenu), "escapeMenu");
		sc->add(fun(&GameState::mainmenu), "mainMenu");
		sc->add(fun(&GameState::paused), "paused");
		sc->add(fun(&GameState::playing), "playing");
		sc->add(fun(&GameState::ray), "ray");
		sc->add_global(var(&GameState::state), "state");


		//TODO ADD COMPONENTS HERE

		sc->add(user_type<Vector2>(), "vec2");
		sc->add(constructor<Vector2(float, float)>(), "vec2");
		sc->add(constructor<Vector2()>(), "vec2");
		sc->add(fun(&Vector2::x), "x");
		sc->add(fun(&Vector2::y), "y");
		sc->add(fun(&Vector2::distance), "distance");
		sc->add(fun(&Vector2::dot), "dot");
		sc->add(fun(&Vector2::set), "set");
		sc->add(fun(&Vector2::length), "length");
		sc->add(fun(&Vector2::normalize), "normalize");
		sc->add(fun(&Vector2::operator+), "+");
		sc->add(fun(&Vector2::operator/), "/");
		sc->add(fun(&Vector2::mul), "*");
		sc->add(fun(&Vector2::sub), "-");
		sc->add(fun(&Vector2::operator==), "==");
		sc->add(fun(&Vector2::operator!=), "!=");

		sc->add(user_type<Vector3>(), "vec3");
		sc->add(constructor<Vector3(float, float, float)>(), "vec3");
		sc->add(constructor<Vector3()>(), "vec3");
		sc->add(fun(&Vector3::x), "x");
		sc->add(fun(&Vector3::y), "y");
		sc->add(fun(&Vector3::z), "z");
		sc->add(fun(&Vector3::distance), "distance");
		sc->add(fun(&Vector3::dot), "dot");
		sc->add(fun(&Vector3::set), "set");
		sc->add(fun(&Vector3::length), "length");
		sc->add(fun(&Vector3::normalize), "normalize");
		sc->add(fun(&Vector3::operator+), "+");
		sc->add(fun(&Vector3::operator/), "/");
		sc->add(fun(&Vector3::mul), "*");
		sc->add(fun(&Vector3::sub), "-");
		sc->add(fun(&Vector3::operator==), "==");
		sc->add(fun(&Vector3::operator!=), "!=");

		sc->add(user_type<Vector4>(), "vec4");
		sc->add(constructor<Vector4(float, float, float, float)>(), "vec4");
		sc->add(constructor<Vector4()>(), "vec4");
		sc->add(fun(&Vector4::distance), "distance");
		sc->add(fun(&Vector4::x), "x");
		sc->add(fun(&Vector4::y), "y");
		sc->add(fun(&Vector4::z), "z");
		sc->add(fun(&Vector4::w), "w");
		sc->add(fun(&Vector4::dot), "dot");
		sc->add(fun(&Vector4::set), "set");
		sc->add(fun(&Vector4::length), "length");
		sc->add(fun(&Vector4::normalize), "normalize");
		sc->add(fun(&Vector4::operator+), "+");
		sc->add(fun(&Vector4::operator/), "/");
		sc->add(fun(&Vector4::mul), "*");
		sc->add(fun(&Vector4::sub), "-");
		sc->add(fun(&Vector4::operator==), "==");
		sc->add(fun(&Vector4::operator!=), "!=");

		sc->add(user_type<Transform>(), "Transform");
		sc->add(constructor<Transform(Vector3, Vector3, Vector3)>(), "Transform");
		sc->add(constructor<Transform()>(), "Transform");
		sc->add(chaiscript::const_var(&Transform::pos), "position");
		sc->add(chaiscript::const_var(&Transform::rot), "rotation");
		sc->add(chaiscript::const_var(&Transform::down), "down");
		sc->add(chaiscript::const_var(&Transform::up), "up");
		sc->add(chaiscript::const_var(&Transform::left), "left");
		sc->add(chaiscript::const_var(&Transform::right), "right");
		sc->add(chaiscript::const_var(&Transform::forward), "forward");
		sc->add(fun(&Transform::setPos), "setPos");
		sc->add(fun(&Transform::setRot), "setRot");
		sc->add(fun(&Transform::setScale), "setScale");
		sc->add(fun(&Transform::translate), "translate");
		sc->add(fun(&Transform::rotate), "rotate");
		sc->add(fun(&Transform::getMatrix), "getMatrix");


		sc->add(user_type<Matrix3>(), "Matrix3");

		sc->add(user_type<Matrix4>(), "Matrix4");

		sc->add(user_type<ComponentManager>(), "ComponentManager");
		sc->add(fun(&ServiceLocator::getCM), "getCM");


		sc->add(fun(&ComponentManager::create2DAudio), "create2DAudio");
		sc->add(fun(&ComponentManager::create3DAudio), "create3DAudio");
		sc->add(fun(&ComponentManager::createAmbient), "createAmbient");
		sc->add(fun(&ComponentManager::createDirectional), "createDirectional");
		sc->add(fun(&ComponentManager::createGraphics), "createGraphics");
		sc->add(fun(&ComponentManager::createPhysicComponent), "createPhysicComponent");
		sc->add(fun(&ComponentManager::createScript), "createScript");
		sc->add(fun(&ComponentManager::createSkyBox), "createSkyBox");
		sc->add(fun(&ComponentManager::createTerrain), "createTerrain");
		sc->add_global_const(chaiscript::const_var(ComponentType::AMBIENT), "AMBIENT");
		sc->add_global_const(chaiscript::const_var(ComponentType::AUDIO), "AUDIO");
		sc->add_global_const(chaiscript::const_var(ComponentType::DIRECTIONAL), "DIRECTIONAL");
		sc->add_global_const(chaiscript::const_var(ComponentType::ENTITY), "ENTITY");
		sc->add_global_const(chaiscript::const_var(ComponentType::GRAPHICS), "GRAPHICS");
		sc->add_global_const(chaiscript::const_var(ComponentType::PHYSICS), "PHYSICS)");
		sc->add_global_const(chaiscript::const_var(ComponentType::SCRIPT), "SCRIPT");
		sc->add_global_const(chaiscript::const_var(ComponentType::SKYBOX), "SKYBOX");
		sc->add_global_const(chaiscript::const_var(ComponentType::TERRAIN), "TERRAIN");

		sc->add(user_type<PhysicsComponent>(), "PhysicsComponent");
		sc->add(user_type<GraphicsComponent>(), "GraphicsComponent");
		sc->add(user_type<AudioComponent>(), "AudioComponent");
		sc->add(user_type<ScriptComponent>(), "ScriptComponent");
		sc->add(user_type<TerrainComponent>(), "TerrainComponent");
		sc->add(user_type<AmbientLightComponent>(), "AmbientLightComponent");
		sc->add(user_type<DirectionalLightComponent>(), "DirectionalLightComponent");
		sc->add(user_type<SkyBoxComponent>(), "SkyBoxComponent");


		sc->add(user_type<ComponentPosition>(), "ComponentPosition");
		sc->add(fun(&ComponentPosition::get), "get");
		sc->add(fun(&ComponentPosition::getT<PhysicsComponent*>), "getPhysics");
		sc->add(fun(&ComponentPosition::getT<GraphicsComponent*>), "getGraphics");
		sc->add(fun(&ComponentPosition::getT<AudioComponent*>), "getAudio");
		sc->add(fun(&ComponentPosition::getT<ScriptComponent*>), "getScript");
		sc->add(fun(&ComponentPosition::getT<TerrainComponent*>), "getTerrain");
		sc->add(fun(&ComponentPosition::getT<AmbientLightComponent*>), "getAmbientLight");
		sc->add(fun(&ComponentPosition::getT<DirectionalLightComponent*>), "getDirectionLight");
		sc->add(fun(&ComponentPosition::getT<SkyBoxComponent*>), "getSkyBox");



		sc->add_global_const(const_var(&Time::delta), "delta");
		sc->add_global_const(const_var(&Time::counter), "counter");
		sc->add_global_const(const_var(&Time::start), "timeSinceStart");
		r.script.updateFun = sc->eval<std::function<void()>>("update");



		r.script.begin();




	}
	void ScriptComponentSystem::unload(ScriptComponent &r)
	{

	}

	void ScriptComponentSystem::ScriptCreated(SeScript* SeScript)
	{
		for (std::list< ScriptComponentSystem::Listener*>::iterator &itr = _listeners.begin(); itr != _listeners.end(); ++itr)
		{
			ScriptComponentSystem::Listener* listener = (*itr);
			listener->ScriptCreated(SeScript);
		}
	}

	void ScriptComponentSystem::addListener(ScriptComponentSystem::Listener* listener)
	{
		_listeners.push_back(listener);
	}

	void ScriptComponentSystem::removeListener(ScriptComponentSystem::Listener* listener)
	{

		for (std::list< ScriptComponentSystem::Listener*>::iterator &itr = _listeners.begin(); itr != _listeners.end(); ++itr)
		{
			if ((*itr) == listener)
			{
				_listeners.erase(itr);
				break;
			}
		}
	}
