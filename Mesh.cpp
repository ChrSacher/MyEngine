#include "Mesh.h"

Model loadOBJ(std::string path);
std::map<std::string, Model> ModelCache::_modelMap;

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
Mesh::Mesh(std::string path,bool autoCenter)
{
	
	init();
	model = ModelCache::getModel(path,autoCenter);
	filePath = path;
	loadBufferVertex();
}

Mesh::Mesh()
{
	init();
}

Vertex::Vertex()
{
	pos=Vector3(0,0,0);
}
Vertex::Vertex(const Vector3& position,const Vector2& tcoord,const Vector3 &tnormal)
{
	pos=position;
	uv=tcoord;
	normal = tnormal;
}
Vertex::Vertex(float x, float y, float z)
{
	pos=Vector3(x,y,z);
}

void Mesh::loadOBJ(std::string path,bool autoCenter)
{
	model = OBJLoader::loadOBJ(path,autoCenter);
	loadBufferVertex();
	
};

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
	glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex) * model.Vertices.size(),&model.Vertices[0],GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void Mesh::clearData()
{
	model.Vertices.clear();
	loadBufferVertex();
}

Model OBJLoader::loadOBJ(std::string path,bool autoCenter)
{
	printf("Loading OBJ file %s...\n", path.c_str());

	std::vector<Vector3> temp_vertices; 
	std::vector<Vector2> temp_uvs;
	std::vector<Vector3> temp_normals;
	std::vector<GLuint> vertexIndices,uvIndices,normalIndices;
	

	FILE * file = fopen(path.c_str(), "r");
	if( file == NULL )
	{
		printf("Impossible to open the file %s ",path.c_str());
		Model model;
		model.Vertices.push_back(Vertex(1,1,0));
		model.Vertices.push_back(Vertex(0,1,1));
		model.Vertices.push_back(Vertex(1,0,1));
		model.Vertices.push_back(Vertex(1,1,0));
		model.Vertices.push_back(Vertex(1,0,1));
		model.Vertices.push_back(Vertex(0,1,1));
		bool valid = false;
		return model;
	}

	while( 1 )
	{

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 )
		{
			Vector3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}
		else if ( strcmp( lineHeader, "vt" ) == 0 )
		{
			Vector2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 )
		{
			Vector3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}
		else if ( strcmp( lineHeader, "f" ) == 0 )
		{
			std::string vertex1, vertex2, vertex3;
			GLuint vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9)
			{
				printf("File %s cannot be read",path.c_str());
				return Model();
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1] );
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}
	Model model;
	for(unsigned int i=0;i<temp_vertices.size();i++)
	{
		model.positions.push_back(temp_vertices[i]);
	}
	if(autoCenter)
	{
		model.center();
	}
	for(unsigned int i=0;i<temp_uvs.size();i++)
	{
		model.uvs.push_back(temp_uvs[i]);
	}
	for(unsigned int i=0;i<temp_normals.size();i++)
	{
		model.normals.push_back(temp_normals[i]);
	}
	for(unsigned int i = 0;i<vertexIndices.size();i++)
	{
		model.Indices.push_back(vertexIndices[i] - 1);
	}
	for(unsigned int i = 0;i<uvIndices.size();i++)
	{
		model.Indices.push_back(uvIndices[i] - 1);
	}
	for(unsigned int i = 0;i<normalIndices.size();i++)
	{
		model.Indices.push_back(normalIndices[i] - 1);
	}
	model.index.vertexIndex=vertexIndices.size();
	model.index.uvIndex=uvIndices.size();
	model.index.normalIndex=vertexIndices.size();

	for( unsigned int i=0; i<model.index.vertexIndex; i++ )
	{
		Vector3 vertex = model.positions[ vertexIndices[i] - 1 ];
		Vector2 uv = temp_uvs[ uvIndices[i] - 1 ];
		Vector3 normal = temp_normals[ normalIndices[i] - 1 ];
		model.Vertices.push_back(Vertex(vertex,uv,normal));
	}
	model.count = model.Vertices.size();
	bool valid = true;
	return model;
}

Model ModelCache::getModel(std::string modelPath,bool autoCenter) 
{

    //lookup the texture and see if its in the map
    auto mit = _modelMap.find(modelPath);
    
    //check if its not in the map
   if (mit == _modelMap.end()) 
	{
        //Load the texture
		 Model newModel = OBJLoader::loadOBJ(modelPath,autoCenter);
		 
        //Insert it into the map
		_modelMap.insert(make_pair(modelPath, newModel));

        return newModel;
    }
	printf("loaded cached Model\n");
   return mit->second;
}


void ModelCache::deleteCache()
{
	_modelMap.clear();
}

void Model::center()
{
	Vector3 center = Vector3(0,0,0);
	for(unsigned int i = 0;i < positions.size();i++)
	{
		center += positions[i];
	}
	center /= positions.size();
	for(unsigned int i = 0;i < positions.size();i++)
	{
		positions[i] -= center;
	}
}