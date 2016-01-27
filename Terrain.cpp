#include "Terrain.h"
#include "ServiceLocator.h"

TerrainPatch::~TerrainPatch()
{
	glDeleteBuffers(1, &vab);
	glDeleteVertexArrays(1, &vao);
	ServiceLocator::getPE().world->removeRigidBody(object);
	delete terrainPhysics;
	delete groundMotionState;
	delete object;
}
void TerrainPatch::load()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vab);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vab);
	Vertex::loadSet();
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	btTriangleMesh *mesh = new btTriangleMesh();
	for (int i = 0; i < vertices.size() - 2; i++)
	{
		mesh->addTriangle(V3BF(vertices[i].pos), V3BF(vertices[i + 1].pos), V3BF(vertices[i + 2].pos));
	}
	terrainPhysics = new btBvhTriangleMeshShape(mesh, false);
	terrainPhysics->setLocalScaling(V3BF(scale));
	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(btVector3(0, 0, 0));
	groundMotionState = new btDefaultMotionState(tr);
	object = new btRigidBody(0, groundMotionState, terrainPhysics);
	ServiceLocator::getPE().world->addRigidBody(object);
}
TerrainPatch::TerrainPatch(std::vector<Vertex> &Vertices, Material &material2, Vector3& v) :vertices(Vertices)
{
	count = Vertices.size();
	load();
	material = material2;
	scale = v;
}
void TerrainPatch::operator=(const TerrainPatch &other)
{
	count = other.count;
	vertices = other.vertices;
	material = other.material;
	scale = other.scale;
	load();


}
TerrainPatch::TerrainPatch(const TerrainPatch &other) :vertices(other.vertices)
{
	material = other.material;
	count = other.count;
	scale = other.scale;
	load();

}
Vector3 calcNormal( const Vector3 &p1, const Vector3 &p2, const Vector3 &p3 )
{
	Vector3 V1= (p2 - p1);
	Vector3 V2 = (p3 - p1);
    Vector3 surfaceNormal;
    surfaceNormal.x = (V1.y*V2.z) - (V1.z-V2.y);
    surfaceNormal.y = - ( (V2.z * V1.x) - (V2.x * V1.z) );
    surfaceNormal.z = (V1.x*V2.y) - (V1.y*V2.x);


    // Dont forget to normalize if needed
    return surfaceNormal;
}

Terrain::Terrain(std::string Path,std::string Texture,Vector3 Scale,bool Center, int NumPatches) : material(Texture,"res/Texture/normal_up.jpg",Vector3(1,1,1),0,32)
{
	printf("Loading terrain %s\n",Path.c_str());
	path = Path;
	centered = Center;
	transform.setScale(Scale);
	calculateHeightMap(Path);
	numPatches = NumPatches;
	if(heightMap.size() > 1)
	{

		calculateNormalMap();
		loadTerrain();
	}
	
}
void Terrain::start(std::string Path, std::string Texture, Vector3 Scale, bool Center, int NumPatches)
{
	material = Material(Texture, "res/Texture/normal_up.jpg", Vector3(1, 1, 1), 0, 32);
	printf("Loading terrain %s\n", Path.c_str());
	path = Path;
	centered = Center;
	transform.setScale(Scale);
	calculateHeightMap(Path);
	numPatches = NumPatches;
	if (heightMap.size() > 1)
	{

		calculateNormalMap();
		loadTerrain();
	}
}
void Terrain::operator=(const Terrain& other)
{
	printf("Loading terrain %s\n", other.path.c_str());
	path = other.path;
	centered = other.centered;
	transform = other.transform;
	calculateHeightMap(path);
	numPatches = other.numPatches;
	if (heightMap.size() > 1)
	{
		calculateNormalMap();
		loadTerrain();
	}
}
Terrain::~Terrain(void)
{
	delete(Index);
	Index = NULL;
	shader = NULL;//have no use yet
	stbi_image_free(data);
}

void Terrain::render(Shader* Nshader)
{
	if (heightMap.size() < 1) return;
	Nshader->use();
	Nshader->setUniform("Texture", 20);
	transform.update(Nshader);
	Nshader->setMVP(*transform.getMatrix());
	for (int i = 0; i < patches.size(); i++)
	{
		
		patches[i].material.update(Nshader);
		patches[i].material.texture.bind(20);
		glBindVertexArray(patches[i].vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, patches[i].count);
	}
	glBindVertexArray(0);
	
		Nshader->use();
		material.update(Nshader);
		material.texture.bind(20);
		Nshader->setUniform("Texture",20);
		transform.update(Nshader);
		Nshader->setMVP(*transform.getMatrix());
		glBindVertexArray(Index->vao);
		glDrawArrays(GL_TRIANGLE_STRIP,0,Index->count);
		glBindVertexArray(0);
	
}

void Terrain::loadTerrain()
{


	//todo UV
	std::vector<Vertex> vertices;
	
	int sizeV = sizeof(Vertex);
	//make terrain with width going
	//add lowe height boundary
	if(centered)
	{
		transform.setPos(Vector3(-width / 2,0,-height/ 2));	
	}
	std::vector < std::vector<int>> subsetVec;
	std::vector<  std::vector<Vector3>> subsetNormal;
	//calculate Patches
	/*
	x--->
	y
	|
	|
	*/
	int PatchSize = ceil((float)heightMap.size() / (float)numPatches);
	Material m = Material("","res/texture/normal_up.jpg",Vector3(1,1,1));
	for (int numY = 0; numY < numPatches; numY++)
	{
		for (int numX = 0; numX < numPatches; numX++)
		{
			unsigned int i = 0;
			unsigned int j = 0;
			for (j = 0; j <  PatchSize; j++)
			{

				subsetVec.push_back(std::vector<int>());
				subsetNormal.push_back(std::vector<Vector3>());
				for (i = 0; i <  PatchSize; i++)
				{
					subsetVec[j].push_back(heightMap[j + numY * PatchSize][i + numX * PatchSize]);
					subsetNormal[j].push_back(normalMap[j + numY * PatchSize][i + numX * PatchSize]);
				}
				if (numX < (numPatches - 1)) //this works now
				{
					subsetVec[j].push_back(heightMap[j + numY * PatchSize][i + numX * PatchSize]);
					subsetNormal[j].push_back(normalMap[j + numY * PatchSize][i  + numX * PatchSize]);
				}
				else
				{
					subsetVec[j].push_back(heightMap[j + numY * PatchSize][i - 1 + numX * PatchSize]);
					subsetNormal[j].push_back(normalMap[j + numY * PatchSize][i - 1 + numX * PatchSize]);
				}
			}
			subsetVec.push_back(std::vector<int>());
			subsetNormal.push_back(std::vector<Vector3>());
			if (numY < (numPatches - 1)) //this works 
			{
				for (int i = 0; i < PatchSize; i++)
				{
					subsetVec[j].push_back(heightMap[j + numY * PatchSize][i + numX * PatchSize]);
					subsetNormal[j].push_back(normalMap[j + numY * PatchSize][i + numX * PatchSize]);
				}
				subsetVec[j].push_back(heightMap[j + numY * PatchSize][i - 1 + numX * PatchSize]);
				subsetNormal[j].push_back(normalMap[j + numY * PatchSize][i - 1 + numX * PatchSize]);
			}
			else
			{
				for (int i = 0; i < PatchSize; i++)
				{
					subsetVec[j].push_back(heightMap[j - 1 + numY * PatchSize][i + numX * PatchSize]);
					subsetNormal[j].push_back(normalMap[j - 1 + numY * PatchSize][i + numX * PatchSize]);
				}
				subsetVec[j].push_back(heightMap[j - 1 + numY * PatchSize][i - 1 + numX * PatchSize]);
				subsetNormal[j].push_back(normalMap[j - 1 + numY * PatchSize][i - 1 + numX * PatchSize]);
			}

			calculateVertices(subsetVec, subsetNormal, vertices);
			for (int i = 0; i < vertices.size(); i++)
			{
				vertices[i].pos = vertices[i].pos + Vector3(numX * PatchSize, 0, numY * PatchSize);
			}
			patches.push_back(TerrainPatch(vertices, m,Vector3(transform.getScale()))); //should use emplace back but doesn't work
			patches[patches.size() - 1].object->translate(btVector3(numX * PatchSize / 2, 0, numY * PatchSize / 2));
			vertices.clear();
			subsetVec.clear();
			subsetNormal.clear();
		}
	}
	
	
	calculateVertices(heightMap, normalMap,vertices);
	Index = new TerrainIndex(vertices);
	
	vertices.clear();
}
void Terrain::calculateVertices(std::vector<std::vector<int>> &heightmap, std::vector<std::vector<Vector3>> &normalmap,std::vector<Vertex> &vertices)
{
	Vertex tempV(0,0,0);
	int swidth = heightmap[0].size();
	int sheight = heightmap.size();
	for (int i = 0; i < sheight - 2; i++)
	{
		for (int j = 0; j < swidth; j++)
		{

			vertices.push_back(Vertex(Vector3(j, heightmap[i][j], (i)), Vector2(((float)j / (float)swidth), ((float)i / (float)sheight)),normalmap[i][j]));
			vertices.push_back(Vertex(Vector3(j, heightmap[i + 1][j], (i + 1)), Vector2(((float)j / (float)swidth), ((float)(i + 1) / (float)sheight)), normalmap[i + 1][j]));

		}
		i++;
		for (int j = heightmap[i].size() - 1; j >= 0; j--)
		{

			vertices.push_back(Vertex(Vector3(j, heightmap[i + 1][j], (i + 1)), Vector2(((float)j / (float)swidth), ((float)(i + 1) / (float)sheight)), normalmap[i + 1][j]));
			vertices.push_back(Vertex(Vector3(j, heightmap[i][j], (i)), Vector2(((float)j / (float)swidth), ((float)i / (float)sheight)), normalmap[i][j]));
		}
	}
}
float Terrain::getHeight(float X,float Z)
{
	X = (X - transform.getPos().x) / transform.getScale().x;
	Z = (Z - transform.getPos().z ) / transform.getScale().z;
	int minX = (int)floor(Z);
	int maxX = (int)ceil(Z);
	int minY = (int)floor(X);
	int maxY = (int)ceil(X);
	//there is no correlation bewteen minX maxX and X, minx and max are for the map which so happens to be a bit different to actual screen
	if(minX  >= 0  && minY  >= 0 && maxX  < width && maxY < height)
	{
		int height1,height2,height3,height4;
		height1 = heightMap[minX][minY] * (1 - (Z - minX)) + heightMap[maxX][minY] * (Z - minX);
		height2 = heightMap[minX][maxY] * (1 - (X - minY)) + heightMap[minX][maxY] * (X - minY);
		height3 = heightMap[minX][maxY] * (1 - (Z - minX)) + heightMap[maxX][maxY] * (Z - minX);
		height4 = heightMap[maxX][maxY] * (1 - (X - minY)) + heightMap[maxX][maxY] * (X - minY);
		return ((height1+height2+height3+height4)/4+ 1) * transform.getScale().y ;
	}
	else
	{
		return 0;
	}
}

void Terrain::calculateNormalMap()
{
	normalMap.clear();
	//sopel Filter for normals
	int left = 1;
	int right = 2;
	//scharr filter
	left = 3;
	right = 10;
	std::vector<Vector3> temp1;
	for(int r = 0; r < width;r++)
	{
		temp1.push_back(Vector3(1,1,1));
	}
	normalMap.push_back(temp1);
	temp1.clear();
	for(int i = 1; i < height -1;i++)
	{
		std::vector<Vector3> temp ;
		temp.push_back(Vector3(1,1,1));
		for(int j = 1; j < width - 1;j++)
		{
			Vector3 n;
			n.x = -(left* (heightMap[i+1][j+1]-heightMap[i+1][j-1])+right*(heightMap[i][j+1]-heightMap[i][j-1])+left *(heightMap[i-1][j+1]-heightMap[i-1][j-1]));
			n.y = -(left* (heightMap[i-1][j-1]-heightMap[i+1][j-1])+right*(heightMap[i-1][j]-heightMap[i+1][j])+left * (heightMap[i-1][j+1]-heightMap[i+1][j+1]));
			n.z = 1.0;
			n.normalize();
			temp.push_back(n);
		}
		temp.push_back(Vector3(1,1,1));
		normalMap.push_back(temp);
	}
	for(int r = 0; r < width;r++)
	{
		temp1.push_back(Vector3(1,1,1));
	}
	normalMap.push_back(temp1);
}

void Terrain::calculateHeightMap(std::string Path)
{
	data = stbi_load(Path.c_str(),&width,&height,&numComponents,4);
	if(data ==NULL)
	{
		std::cout<<"Could not load Terrain "<< Path<<std::endl;		
		return;
	}
	if(width < 6 || height < 6) 
	{
		std::cout<<"Heightmap too small"<<std::endl;
		return;
	}	
	
	int offset = 0;
	heightMap.clear();
	heightMap.reserve(height);
	int lowest = 255;
	for(int i = 0; i < height ;i++)
	{
		std::vector<int> temp ;
		temp.reserve(width);
		for(int j = 0;j < width * 4;j += 4)
		{

			temp.push_back(((data[offset + j]) +(data[offset + j + 1]) + (data[offset + j + 2] )) / 3);
			if(temp[j/4] < lowest)
			lowest = temp[j/4];
		}
		offset += width * 4;
		heightMap.push_back(temp);
		
	}
	for(unsigned int i = 0; i < heightMap.size();i++)
	{
		for(unsigned int j = 0; j < heightMap[i].size();j++)
		{
			heightMap[i][j] -= lowest;
		}
	}
	
	//box filter
	for(unsigned int i = 1; i < heightMap.size() -1;i++)
	{
		for(unsigned int j = 1; j < heightMap[i].size() -1;j++)
		{
			heightMap[i][j] = heightMap[i-1][j-1]/9 + heightMap[i-1][j]/9 + heightMap[i-1][j+1]/9 + heightMap[i][j-1]/9 + heightMap[i][j]/9 + heightMap[i][j+1]/9 + heightMap[i+1][j-1]/9 + heightMap[i+1][j]/9 + heightMap[i+1][j+1]/9;
		}
	}
}

void Terrain::resizeTerrain(Vector3 Scale)
{
	transform.setScale(Scale);
}