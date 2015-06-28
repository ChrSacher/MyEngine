#include "Object.h"

GLuint Object::__id = 0;
Object::Object(std::string Name,std::string Objectpath,Vector3 pos,Vector3 rot,Vector3 skal ,std::string texturepath,Vector3 color,bool autoCenter)
{

	__ID = __id;
	__id++;
	material = new Material(texturepath,color,2,32);
	transform =  new Transform(pos,rot,skal);
	mesh = new Mesh(Objectpath,autoCenter);
	renderable = true;
	__objectName = Name;
}


Object::~Object(void)
{
	if(material || transform || mesh)
	{
		delete(material,transform,mesh);
	}
}

void Object::draw()
{
	mesh->draw();
};

Matrix4& Object::getMatrix()
{
	return transform->getMatrix();
}

Object::Object()
{
	__ID = __id;
	__id++;
	material = new Material();
	transform = new Transform();
	mesh = new Mesh();
	renderable = true;
	__objectName = "Object";
}

Object::Object(const Object& otherobject)
{
	__ID = __id;
	__id++;
	material = new Material(*otherobject.material);
	transform = new Transform(*otherobject.transform);
	mesh = new Mesh(*otherobject.mesh);
	renderable = true;
}

bool Object::operator==(const Object& other)
{
	return (__ID && other.__ID);
}
