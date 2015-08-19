#include "Mesh.h"


Mesh::~Mesh(void)
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1,&vab);
}

void Mesh::draw()
{
	glBindVertexArray(vao);
	if(!indiced)
	{
		glDrawArrays(GL_TRIANGLES,0,model.Vertices.size());		
	}
	else
	{	
		glDrawElementsBaseVertex(GL_TRIANGLES,model.Indices.size(),GL_UNSIGNED_INT,0,0);
	}
	glBindVertexArray(0);
}
void Mesh::drawInstanced(int count)
{
	glDrawArraysInstanced(GL_TRIANGLES,0,model.Vertices.size(),count);
}
void Mesh::init()
{
	glGenVertexArrays(1,&vao);
	glGenBuffers(1,&vab);
}
Mesh::Mesh(std::vector<Vertex> vertices)
{
	
	init();
	for(unsigned int i=0;i<vertices.size();i++)
	{
		model.Vertices.push_back(vertices[i]);
	}
	loadBufferVertex();
}
Mesh::Mesh(std::string path,bool autoCenter) : model(path)
{
	
	init();
	filePath = path;
	loadBufferVertex();
}

Mesh::Mesh()
{
	init();
}

Vertex::Vertex()
{
}
Vertex::Vertex(const Vector3& position,const Vector2& tcoord,const Vector3 &tnormal,const Vector3& ttangent)
{
	pos=position;
	uv=tcoord;
	normal = tnormal;
	tangent = ttangent;
}
Vertex::Vertex(float x, float y, float z)
{
	pos=Vector3(x,y,z);
}


void Mesh::loadBufferVertex()
{	
	indiced = false;
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER,vab);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,pos));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,uv));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,normal));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,tangent));
	glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex) * model.Vertices.size(),&model.Vertices[0],GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void Mesh::clearData()
{
	model.Vertices.clear();
	loadBufferVertex();
}

Model::Model()
{
	Vertices.push_back(Vertex());
	  Vertices.push_back(Vertex());
	  Vertices.push_back(Vertex());
	  Indices.push_back(0);
	  Indices.push_back(0);
	Indices.push_back(0);
	valid = false;
}
Model::Model(std::string &path)
{
	Assimp::Importer importer; 
    const aiScene* scene = importer.ReadFile( path,   
      aiProcess_Triangulate				| 
	  aiProcess_CalcTangentSpace		|
      aiProcess_JoinIdenticalVertices	| 
      aiProcess_SortByPType); 

   if(!scene) 
   { 
	  fatalError("Failed to load Mesh from "+ path);
	  Vertices.push_back(Vertex());
	  Vertices.push_back(Vertex());
	  Vertices.push_back(Vertex());
	  Indices.push_back(0);
	  Indices.push_back(0);
	  Indices.push_back(0);
	  valid = false;
   } 
   for(int i = 0; i < scene->mNumMeshes;i++)
   {
		const aiMesh* mesh = scene->mMeshes[i];
		for(int j = 0;j < mesh->mNumVertices;j++)
		{
			const aiVector3D* pos = &(mesh->mVertices[i]);
			const aiVector3D* normal = &(mesh->mNormals[i]);
			aiVector3D* texCoord;
			aiVector3D* tangent = &(mesh->mTangents[i]);
			if(mesh->HasTextureCoords(0)) const aiVector3D* pTexCoord =  &(mesh->mTextureCoords[0][i]);

			Vertex v(Vector3(pos->x, pos->y, pos->z),
					Vector2(texCoord->x, texCoord->y),
					Vector3(normal->x, normal->y, normal->z),Vector3(tangent->x,tangent->y,tangent->z));

			Vertices.push_back(v);

		}
		for (unsigned int i = 0 ; i < mesh->mNumFaces ; i++) 
		{
			const aiFace& Face = mesh->mFaces[i];
			assert(Face.mNumIndices == 3);
			Indices.push_back(Face.mIndices[0]);
			Indices.push_back(Face.mIndices[1]);
			Indices.push_back(Face.mIndices[2]);
		}
   }
   
}