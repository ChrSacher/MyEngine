#include "Mesh.h"


Mesh::~Mesh(void)
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(2,vab);
}

void Mesh::draw()
{
	glBindVertexArray(vao);
	if(!indiced)
	{
		glDrawArrays(GL_TRIANGLES,0,model.countVertices);		
	}
	else
	{	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vab[1]);
		glDrawElements(GL_TRIANGLES,model.countIndices,GL_UNSIGNED_SHORT,0);
	}
	glBindVertexArray(0);
}
void Mesh::drawInstanced(int count)
{
	glBindVertexArray(vao);
	if(indiced)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vab[1]);
		glDrawElementsInstanced(GL_TRIANGLES,model.countVertices,GL_UNSIGNED_SHORT,0,count);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES,0,model.countVertices);
	}
	glBindVertexArray(0);
}
void Mesh::init()
{
	glGenVertexArrays(1,&vao);
	glGenBuffers(2,vab);
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

void Mesh::operator=(const Mesh& other)
{
	init();
	filePath = other.filePath;
	model = other.model;
	indiced = other.indiced;
	loadBufferVertex();
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
	indiced = true;
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER,vab[0]);
	Vertex::loadSet();
	glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex) * model.Vertices.size(),&model.Vertices[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vab[1]);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_UNSIGNED_SHORT, GL_FALSE,0, 0);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort) * model.Indices.size(),&model.Indices[0],GL_STATIC_DRAW);
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
			countIndices = 0;
		countVertices = 0;
	Assimp::Importer importer; 
    const aiScene* scene = importer.ReadFile( path,   
      aiProcess_Triangulate				| 
	  aiProcess_CalcTangentSpace		|
	  aiProcess_JoinIdenticalVertices	|
      aiProcess_SortByPType); 
	countIndices = countVertices = 0;
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
	  return;
   } 
   int r = 0;
   int offset = 0;
   for(unsigned int i = 0; i < scene->mNumMeshes;i++)
   {
		const aiMesh* mesh = scene->mMeshes[i];
		for(unsigned int j = 0;j < mesh->mNumVertices;j++)
		{
			const aiVector3D pos = (mesh->mVertices[j]);
			aiVector3D normal = normal = aiVector3D(1,1,1);
			aiVector3D texCoord= aiVector3D(0,0,0);
			aiVector3D tangent = aiVector3D(0,1,0);
				if(mesh->HasNormals())
			{
				normal = (mesh->mNormals[j]);
			}
			if(mesh->HasTangentsAndBitangents())
			{
				tangent = (mesh->mTangents[j]);
			}
			if(mesh->HasTextureCoords(0))
			{
				texCoord =  (mesh->mTextureCoords[0][j]);
			}
			Vertex v(Vector3(pos.x, pos.y, pos.z),
					Vector2(texCoord.x, texCoord.y),
					Vector3(normal.x, normal.y, normal.z),Vector3(tangent.x,tangent.y,tangent.z));

			Vertices.push_back(v);

		}
		GLuint temp = 0;
		
		for (unsigned int i = 0 ; i < mesh->mNumFaces ; i++) 
		{
			const aiFace& Face = mesh->mFaces[i];
			
			
			if(Face.mNumIndices >= 3)
			{
				for(unsigned int c = 0; c < 3;c++)
				{
					if(r < Face.mIndices[c]) r = Face.mIndices[c] + 1;
					Indices.push_back(offset + Face.mIndices[c]);
				}
			}
		}
		offset = r;
		countIndices += mesh->mNumFaces * 3;
		countVertices += mesh->mNumVertices;
   }
  
   importer.FreeScene();
   return;
}