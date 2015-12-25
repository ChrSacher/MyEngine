#include "Terrain.h"

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


Terrain::~Terrain(void)
{
	delete(Index);
	Index = NULL;
	shader = NULL;//have no use yet
}

void Terrain::render(Shader* Nshader)
{
	if (heightMap.size() < 1) return;
	/*Nshader->use();
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
	glBindVertexArray(0); */
	
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
	unsigned int yOffset = 0;
	unsigned int xOffset = 0;
	int PatchSize = heightMap.size() / numPatches;
	Material m = Material();
	for ( yOffset = yOffset; yOffset < heightMap.size(); yOffset += PatchSize)
	{
		for ( xOffset = xOffset; xOffset < heightMap.size(); xOffset += PatchSize)
		{
			for (unsigned int j = yOffset; j < yOffset + PatchSize; j++)
			{

				subsetVec.push_back(std::vector<int>());
				subsetNormal.push_back(std::vector<Vector3>());
				for (unsigned int i = xOffset; i < xOffset + PatchSize; i++)
				{
					subsetVec[j - yOffset].push_back(heightMap[i][j]);
					subsetNormal[j - yOffset].push_back(normalMap[i][j]);
				}
			}
			calculateVertices(subsetVec, subsetNormal, vertices);
			patches.push_back(TerrainPatch(vertices, m)); //should use emplace back but doesn't work
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
	for (int i = 0; i < heightmap.size() - 2; i++)
	{
		for (int j = 0; j < heightmap[i].size(); j++)
		{

			tempV.pos = Vector3(j, heightmap[i][j], (i));
			tempV.uv = Vector2(((float)j / (float)width), ((float)i / (float)height));
			tempV.normal = normalmap[i][j];
			vertices.push_back(tempV);
			tempV.pos = Vector3(j, heightmap[i + 1][j], (i + 1));
			tempV.uv = Vector2(((float)j / (float)width), ((float)(i + 1) / (float)height));
			tempV.normal = normalmap[i + 1][j];
			vertices.push_back(tempV);

		}
		i++;
		for (int j = heightmap[i].size() - 1; j >= 0; j--)
		{

			tempV.pos = Vector3(j, heightmap[i + 1][j], (i + 1));
			tempV.uv = Vector2(((float)j / (float)width), ((float)(i + 1) / (float)height));
			tempV.normal = normalmap[i + 1][j];
			vertices.push_back(tempV);
			tempV.pos = Vector3(j, heightmap[i][j], (i));
			tempV.uv = Vector2((float)j / (float)width, (float)i / (float)height);
			tempV.normal = normalmap[i][j];
			vertices.push_back(tempV);
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
	unsigned char* data = stbi_load(Path.c_str(),&width,&height,&numComponents,4);
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
	stbi_image_free(data);
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