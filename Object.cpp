#include "Object.h"

GLuint Object::id = 1;
Object::Object(std::string Name,std::string Objectpath,Vector3 pos,Vector3 rot,Vector3 skal ,std::string texturepath,Vector3 color,std::string NormalMap,bool autoCenter)
{

	ID = id;
	id++;
	material = new Material(texturepath,NormalMap,color,2,32);
	transform = new Transform(pos,rot,skal);
	mesh = new Mesh(Objectpath,autoCenter);
	renderable = true;
	objectName = Name;
}


Object::~Object(void)
{

	if(material )delete(material);
	if(mesh) delete(mesh);
	if(transform) delete(transform);
	transform = NULL;
	material = NULL;
	mesh = NULL;
}

void Object::drawMesh()
{
	mesh->draw();
};
void Object::draw(Shader* shader)
{
	
	if(shader != NULL)
	{
		transform->update(shader);
		shader->setMVP(transform->getMatrix());
		material->update(shader);
	}
	mesh->draw();
}
Matrix4& Object::getMatrix()
{
	return transform->getMatrix();
}

Object::Object(const Object& otherobject)
{
	ID = id;
	id++;
	material = new Material(*otherobject.material);
	transform = new Transform(*otherobject.transform);
	mesh = new Mesh(*otherobject.mesh);
	renderable = true;
}

bool Object::operator==(const Object& other)
{
	return (ID == other.ID);
}

std::string Object::toString()
{
	std::string returnS;
	returnS += objectName + " " + mesh->getPath() + " " + material->texture.texturepath +" ";
	returnS += std::to_string(transform->getPos().x) + " " +  std::to_string(transform->getPos().y) + " " +  std::to_string(transform->getPos().z) + " ";
	returnS += std::to_string(transform->getRot().x) + " " +  std::to_string(transform->getRot().y) + " " +  std::to_string(transform->getRot().z) + " ";
	returnS += std::to_string(transform->getPos().x) + " " +  std::to_string(transform->getPos().y) + " " +  std::to_string(transform->getPos().z) + " ";
	returnS += std::to_string(material->getColor().x) + " " +  std::to_string(material->getColor().y) + " " +  std::to_string(material->getColor().z) + " ";
	returnS += material->normalMap.texturepath;
	return returnS;
}

std::string Object::toStringNames()
{
	std::string returnS;
	returnS += objectName + " " + mesh->getPath() + " " + material->texture.texturepath +"\n";
	returnS += "Position" + std::to_string(transform->getPos().x) + " " +  std::to_string(transform->getPos().y) + " " +  std::to_string(transform->getPos().z) +"\n";
	returnS += "Rotation" + std::to_string(transform->getRot().x) + " " +  std::to_string(transform->getRot().y) + " " +  std::to_string(transform->getRot().z)+"\n";
	returnS += "Scale" + std::to_string(transform->getScale().x) + " " +  std::to_string(transform->getScale().y) + " " +  std::to_string(transform->getScale().z)+"\n";
	returnS += "Color" + std::to_string(material->getColor().x) + " " +  std::to_string(material->getColor().y) + " " +  std::to_string(material->getColor().z)+"\n";
	returnS += material->normalMap.texturepath +"\n";
	return returnS;
}

 ObjectInformation::ObjectInformation(Object* newObject, GLuint Offset = 0,GLuint Count =  0)
{
	object = newObject;
	offset = Offset;
	count = Count;
}
 ObjectInformation::~ObjectInformation()
{

}
ObjectInformation::ObjectInformation()
{
}



 ObjectBatch::ObjectBatch()
{
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	glGenBuffers(1,&vab);
	lastOffset = 0;
	maxSize = 2000000;
	remainingSize = maxSize;
		//initiliaze buffer with 2mb
	glBindBuffer(GL_ARRAY_BUFFER,vab);
	glBufferData(GL_ARRAY_BUFFER,maxSize,NULL,GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,pos));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,uv));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,normal));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,tangent));
	glBindVertexArray(0);
	countObjects = 0;
	lastDeleteObjectIndex = -1;
	

}

 void ObjectBatch::loadBuffer()
 {
	
	
 }
 void ObjectBatch::loadBufferLast()
 {
	glBindBuffer(GL_ARRAY_BUFFER,vab);
	glBufferSubData(GL_ARRAY_BUFFER,lastInformation.offset * sizeof(Vertex),lastInformation.count * sizeof(Vertex),&lastInformation.object->mesh->model.Vertices[0]);
 }


 void ObjectBatch::loadBufferIndexToLast()
 {
	 if(lastDeleteObjectIndex > 0)
	 {
		glBindBuffer(GL_ARRAY_BUFFER,vab);
		for(auto iter = objects.begin();iter != objects.end();iter++)
		{	
			glBufferSubData(GL_ARRAY_BUFFER,iter->second.offset * sizeof(Vertex),iter->second.count * sizeof(Vertex),&iter->second.object->mesh->model.Vertices[0]);
		}
		lastDeleteObjectIndex = -1;
	 }
 }
 ObjectBatch::~ObjectBatch()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1,&vab);
}

 ShaderObjectPipeLine::ShaderObjectPipeLine()
{
	batches.push_back(new ObjectBatch());
	countBatches = 1;
}
 ShaderObjectPipeLine::~ShaderObjectPipeLine()
{
	for(unsigned int i = 0; i < countBatches;i++)
	{
		delete(batches[i]);
		batches[i] = NULL;
	}
}

void  ShaderObjectPipeLine::addObject(Object* newObject)
{
	for(unsigned int i = 0; i < countBatches;i++)
	{
		if(batches[i]->checkSize(newObject))
		{
			batches[i]->addObject(newObject);
			return;
		}
	}
	batches.push_back(new ObjectBatch());
	countBatches ++;
	batches[countBatches - 1]->addObject(newObject);
	
}
void  ShaderObjectPipeLine::deleteObject(Object* removeObject)
{
	for(unsigned int i = 0;i < countBatches;i++)
	{
		batches[i]->deleteObject(removeObject);
	}
	
}

void ObjectBatch::deleteObject(Object* removeObject)
{
		auto mit = objects.find(removeObject->getID());
		if(mit == objects.end())
		{
			
		}
		else
		{
			remainingSize += mit->second.count * sizeof(Vertex);
			objects.erase(removeObject->getID());
			countObjects -- ;
		};
	
}

bool  ObjectBatch::checkSize(Object* newObject)
{
	if((newObject->mesh->model.Vertices.size() * sizeof(Vertex)) <= remainingSize)
	{
		return true;
	}
	else return false;
}

void  ObjectBatch::addObject(Object* newObject)
{
	int i = newObject->mesh->model.Vertices.size();
	auto mit = objects.find(newObject->getID());
		if(mit == objects.end())
		{
			ObjectInformation x(newObject,lastOffset,i);
			objects.insert(std::make_pair(newObject->getID(),x));
			lastInformation = x ;
			remainingSize -= i * sizeof(Vertex);
			lastOffset += i;
			countObjects++;
			loadBufferLast();
		}
		else
		{
	
		}
}

GLuint   ShaderObjectPipeLine::renderBatches(Shader* shader,Camera3d *cam)
{
	int k = 0;
	for(unsigned int i = 0; i < countBatches; i++)
	{
		k += batches[i]->render(shader,cam);
	}
	objectsDrawn = k;
	return k;
}

void  ShaderObjectPipeLine::renderShadowBatches(Shader* shader)
{
	for(unsigned int i = 0; i < countBatches; i++)
	{
		batches[i]->renderShadow(shader);
	}
}

GLuint  ObjectBatch::render(Shader *shader,Camera3d *cam)
{
	glBindVertexArray(vao);
	loadBufferIndexToLast();
	int i = 0;
	for(auto iter = objects.begin();iter != objects.end();iter++)
	{
			
			if(cam != NULL)
			{
				if(iter->second.object->transform->getPos().distance(cam->getPos()) > cam->getZ().y) continue; //don't draw when out of range
				if(cam->isBehind(iter->second.object->transform->getPos())) continue; //don't draw behind camera
			}
			iter->second.object->transform->update(shader);
			shader->setMVP(iter->second.object->transform->getMatrix());
			iter->second.object->material->update(shader);
			glDrawArrays(GL_TRIANGLES,iter->second.offset,iter->second.count);
			i++;
	}
	objectsDrawn = i;
	glBindVertexArray(0);
	return i;
}

void  ObjectBatch::renderShadow(Shader *shader)
{
	glBindVertexArray(vao);
	loadBufferIndexToLast();
	shader->use();
	for(auto iter = objects.begin();iter != objects.end();iter++)
	{
			iter->second.object->transform->update(shader);
			glDrawArrays(GL_TRIANGLES,iter->second.offset,iter->second.count);
	}
	glBindVertexArray(0);
}

void ShaderObjectPipeLine::emptyBatch()
{
	for (unsigned int i = 0; i< countBatches; i++)
    {  
       batches[i]->emptyBuffer();
    }
}

 void ObjectBatch::emptyBuffer()
 {

		objects.clear();
		glBindBuffer(GL_ARRAY_BUFFER,vab);
		glBufferData(GL_ARRAY_BUFFER,maxSize,NULL,GL_STREAM_DRAW);
		lastOffset = 0;
		remainingSize = maxSize;
		countObjects = 0;
 }

GLuint ObjectBatch::renderColor(Shader *shader,Camera3d* cam, Vector3 color)
 {
	glBindVertexArray(vao);
	loadBufferIndexToLast();
	int i = 0;
	for(auto iter = objects.begin();iter != objects.end();iter++)
	{
			
			if(cam != NULL)
			{
				if(iter->second.object->transform->getPos().distance(cam->getPos()) > cam->getZ().y) continue; //don't draw when out of range
				if(cam->isBehind(iter->second.object->transform->getPos())) continue; //don't draw behind camera
			}
			shader->setMVP(cam->GetViewProjection(),iter->second.object->transform->getMatrix());
			shader->setUniform("color",color);
			glDrawArrays(GL_TRIANGLES,iter->second.offset,iter->second.count);
			i++;
	}
	objectsDrawn = i;
	glBindVertexArray(0);
	return i;
 }

GLuint ShaderObjectPipeLine::renderColor(Shader *shader,Camera3d* cam, Vector3 color)
 {
	int k = 0;
	for (unsigned int i = 0; i< countBatches; i++)
    {  
       k =+ batches[i]->renderColor(shader,cam,color);
    }
	return k;
 }


InstancedObject::InstancedObject(std::string Name,std::string Objectpath,std::vector<Vector3> pos,std::vector<Vector3> rot,std::vector<Vector3> skal,std::string texturepath,Vector3 color ,std::string NormalMap,bool autoCenter)
{
	ID = Object::id;
	Object::id++;
	material = new Material(texturepath,NormalMap,color,2,32);
	for(int i = 0; i < pos.size();i++)
	{
		transforms.push_back(new Transform(pos[i],rot[i],skal[i]));
	}
	
	mesh = new Mesh(Objectpath,autoCenter);
	renderable = true;
	objectName = Name;

	GLuint VAO = mesh->getVao();
	glBindVertexArray(VAO);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	
	// Vertex Attributes
	glEnableVertexAttribArray(5); //5 because 0 vertices 1 uv 2 normal 3 tangen 4 indices
	glVertexAttribPointer(5, 16, GL_FLOAT, GL_FALSE,sizeof(Matrix4), (GLvoid*)0);
	glVertexAttribDivisor(5, 1);
	glBindVertexArray(0);
	
}
void InstancedObject::drawMesh()
{
	if(transforms.size() == 0) return;

	for(GLuint i = 0; i < transforms.size(); i++)
	{
		if(transforms[i]->needsUpdate()) modelMatrices[i] = transforms[i]->getMatrix();
	};
	
	glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(Matrix4), &modelMatrices[0], GL_STATIC_DRAW);
	mesh->drawInstanced(modelMatrices.size());
};

void InstancedObject::draw(Shader* shader)
{
	if(shader != NULL)
	{
		material->update(shader);
		
	}
	drawMesh();
}

InstancedObject::~InstancedObject(void)
{

	if(material )delete(material);
	if(mesh) delete(mesh);
	for(int i = 0; i < transforms.size();i++)
	{
		if(transforms[i]) delete(transforms[i]);
	}
	transforms.clear();
	glDeleteBuffers(1,&buffer);
	material = NULL;
	mesh = NULL;
}