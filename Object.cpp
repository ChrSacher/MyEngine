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
	filePath = Objectpath;
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
bool Object::draw(Shader* shader,Camera3d* cam)
{
	shader->use();
	if(shader != NULL)
	{
		transform->update(shader);
		if(cam != NULL){shader->setMVP(transform->getMatrix());}
		else{shader->setMVP(cam->GetViewProjection(),transform->getMatrix());}
		material->update(shader);
	}
	bool render = true;
	if(cam != NULL)
	{
		if(transform->getPos().distance(cam->getPos()) > cam->getZ().y) render = false; //don't draw when out of range
		if(cam->isBehind(transform->getPos()))  render = false; //don't draw behind camera
		if(!renderable)  render = false;
	}
	if(render) {mesh->draw();return true;}
	return false;
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