#include "Object.h"

GLuint Object::__id = 0;
Object::Object(std::string Name,std::string Objectpath,Vector3 pos,Vector3 rot,Vector3 skal ,std::string texturepath,Vector3 color,std::string NormalMap,bool autoCenter)
{

	__ID = __id;
	__id++;
	material = new Material(texturepath,NormalMap,color,2,32);
	transform = Transform(pos,rot,skal);
	mesh = new Mesh(Objectpath,autoCenter);
	renderable = true;
	__objectName = Name;
}


Object::~Object(void)
{
	if(material  || mesh)
	{
		delete(material,mesh);
		material = NULL;
		mesh = NULL;
	}
}

void Object::draw()
{
	mesh->draw();
};

Matrix4& Object::getMatrix()
{
	return transform.getMatrix();
}

Object::Object(const Object& otherobject)
{
	__ID = __id;
	__id++;
	material = new Material(*otherobject.material);
	transform = Transform(otherobject.transform);
	mesh = new Mesh(*otherobject.mesh);
	renderable = true;
}

bool Object::operator==(const Object& other)
{
	return (__ID && other.__ID);
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

void  ShaderObjectPipeLine::renderBatches(Shader* shader)
{
	for(unsigned int i = 0; i < countBatches; i++)
	{
		batches[i]->render(shader);
	}
}

void  ShaderObjectPipeLine::renderShadowBatches(Shader* shader)
{
	for(unsigned int i = 0; i < countBatches; i++)
	{
		batches[i]->renderShadow(shader);
	}
}

void  ObjectBatch::render(Shader *shader)
{
	glBindVertexArray(vao);
	loadBufferIndexToLast();
	for(auto iter = objects.begin();iter != objects.end();iter++)
	{
			iter->second.object->transform.update(shader);
			shader->setMVP(iter->second.object->transform.getMatrix());
			iter->second.object->material->update(shader);
			glDrawArrays(GL_TRIANGLES,iter->second.offset,iter->second.count);
	}
	glBindVertexArray(0);
}

void  ObjectBatch::renderShadow(Shader *shader)
{
	glBindVertexArray(vao);
	loadBufferIndexToLast();
	for(auto iter = objects.begin();iter != objects.end();iter++)
	{
			shader->setUniform("modelMatrix[0]",iter->second.object->transform.getMatrix());
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