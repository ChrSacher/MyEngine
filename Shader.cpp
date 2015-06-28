#include "Shader.h"

GLuint Shader::currentActiveShader = -1;

Shader::Shader() : _numAttributes(0)
{
	/*create program */
	 _programID = glCreateProgram();
	 if(_programID == 0)
	 {
		 fatalError("failed to create programm");
	 }
	 pipeline = new ShaderObjectPipeLine();
}

Shader::~Shader()
{
	glUseProgram(0);
	glDeleteProgram(_programID);
	delete(pipeline);
	
}
void Shader::addVertexShader(std::string path)
{
	addProgram(path,GL_VERTEX_SHADER);
};

void Shader::addGeometryShader(std::string path)
{
	addProgram(path,GL_GEOMETRY_SHADER);
};

void Shader::addFragmentShader(std::string path)
{
	addProgram(path,GL_FRAGMENT_SHADER);
};

void Shader::addProgram(std::string path,int type)
{
	/* create shader to attach to program*/
	int shader = glCreateShader(type); 
	if (shader == 0) 
	{
        fatalError("shader failed to be created!");
    }
	//Open the file
    std::ifstream shaderFile(path);
	std::cout << "opening shader "  + path << std::endl;
    if (shaderFile.fail()) 
	{
        perror(path.c_str());
        fatalError("Failed to open " + path);
    }

    //File contents stores all the text in the file
    std::string fileContents = "";
    //line is used to grab each line of the file
    std::string line;

    //Get all the lines in the file and add it to the contents
    while (std::getline(shaderFile, line)) 
	{
        fileContents += line + "\n";
    }

    shaderFile.close();

    //get a pointer to our file contents c string;
    const char* contentsPtr = fileContents.c_str();
    //tell opengl that we want to use fileContents as the contents of the shader file
    glShaderSource(shader, 1, &contentsPtr, nullptr);

    //compile the shader
    glCompileShader(shader);

    //check for errors
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

        //Provide the infolog in whatever manor you deem best.
        //Exit with failure.
        glDeleteShader(shader); //Don't leak the shader.

        //Print error log and quit
        std::printf("%s\n", &(errorLog[0]));
        fatalError("Shader " + path + " failed to compile");
    }
	attachedShaders.push_back(shader);
	glAttachShader(_programID, shader);

	
};




void Shader::linkShaders()
{

    //Link our program
    glLinkProgram(_programID);

    //Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<char> errorLog(maxLength);
        glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		for(unsigned int i = 0;i<attachedShaders.size();i++) glDetachShader(_programID,attachedShaders[i]);
		for(unsigned int i = 0;i<attachedShaders.size();i++) glDeleteShader(attachedShaders[i]);
        //We don't need the program anymore.
        glDeleteProgram(_programID);
        //Don't leak shaders either.
        //print the error log and quit
        std::printf("%s\n", &(errorLog[0]));
        fatalError("Shaders failed to link!");
    }	
	for(unsigned int i = 0;i<attachedShaders.size();i++) glDetachShader(_programID,attachedShaders[i]);
	for(unsigned int i = 0;i<attachedShaders.size();i++) glDeleteShader(attachedShaders[i]);
	//add Program in RenderUtil now
	//util.addProgram
	
}

//Adds an attribute to our shader. SHould be called between compiling and linking.
void Shader::addAttribute(const std::string& attributeName) 
{
    glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
}

//enable the shader, and all its attributes
void Shader::bind() 
{
    glUseProgram(_programID);
    //enable all the attributes we added with addAttribute
}

void Shader::use() 
{
    glUseProgram(_programID);
	Shader::currentActiveShader =_programID;
    //enable all the attributes we added with addAttribute
    for (int i = 0; i < _numAttributes; i++) 
	{
        glEnableVertexAttribArray(i);
    }
}


void Shader::unuse() 
{
	//disable the shader
    glUseProgram(0);
	Shader::currentActiveShader = -1;
    for (int i = 0; i < _numAttributes; i++) 
	{
        glDisableVertexAttribArray(i);
    }
}

GLint Shader::getUniformLocation(const std::string& uniformName)
{
	auto mit = uniforms.find(uniformName);
    
    //check if its not in the map
   if (mit == uniforms.end()) 
   {
		GLint location =glGetUniformLocation(_programID, uniformName.c_str());
		if(location == GL_INVALID_INDEX)
		{
			fatalError("Uniform " + uniformName + " not found");
			location = -1;
		}	 
        //Insert it into the map
		uniforms.insert(make_pair(uniformName, location));

        return location; 
    }
   return mit->second;
}

	void Shader::setUniform(std::string uniformName, int value)
	{
		glUniform1i(getUniformLocation(uniformName), value);
	}

	void Shader::setUniform(std::string uniformName, bool value)
	{
		glUniform1i(getUniformLocation(uniformName), value);
	}

	void Shader::setUniform(std::string uniformName, float value)
	{
		glUniform1f(getUniformLocation(uniformName), value);
	}
	
	void Shader::setUniform(std::string uniformName, Vector3 value)
	{
		glUniform3f(getUniformLocation(uniformName), value[0],value[1],value[2]);
	}
	void Shader::setUniform(std::string uniformName, Vector4 value)
	{
		glUniform4f(getUniformLocation(uniformName), value[0],value[1],value[2],value[3]);
	}
	
	void Shader::setUniform(std::string uniformName, Matrix4 value)
	{
		glUniformMatrix4fv(getUniformLocation(uniformName),1,GL_FALSE,&value[0]);
	}

	//standard shader specific settings
	void Shader::setUniform(std::string uniformName,BaseLight value)
	{
		setUniform(uniformName + ".base.intensity",value.getIntensity());
		setUniform(uniformName + ".base.color",value.getColor());
	}

	void Shader::setmodelMatrix(Transform *transform) 
	{
		setUniform("modelMatrix[0]",transform->getMatrix());
		setUniform("MVP[0]",matrices.view * transform->getMatrix());
	}
	void Shader::setmodelMatrices(std::vector<Matrix4*> modelMatrices,std::vector<Matrix4*> MVPMatrices) 
	{
		std::string string =  "MVP[0]";
		std::string string2 =  "modelMatrix[0]";
		if(modelMatrices.size() == MVPMatrices.size())
		{
			for(unsigned int i = 0; i < modelMatrices.size();i++)
			{
				string2 = "modelMatrix[" + std::to_string(i) + "]";
				setUniform(string2,matrices.view * *modelMatrices[i]);
				string = "MVP[" + std::to_string(i) + "]";
				setUniform(string,matrices.view * *MVPMatrices[i]);
			}
		}
		else
			std::cout << "different ammount of matrices"<<std::endl;
	}
	void Shader::setviewMatrix(Camera3d *view)
	{
		matrices.view = view->GetViewProjection();
	};

	void Shader::setbaseColor(Vector3 Color)
	{
		setUniform("baseColor",Color);
	}
	void Shader::setbaseColor(Vector4 Color)
	{
		setUniform("baseColor",Color);
	}
	void Shader::setSpecular(Material *material)
	{
		setUniform("specularIntensity",material->getIntensity());
		setUniform("specularPower",material->getPower());
	}
	void Shader::setCameraPos(Camera3d *view)
	{
		setUniform("eyePos",view->getPos());
	}
	void Shader::updateCamera(Camera3d *view)
	{
		setCameraPos(view);
		setviewMatrix(view);
		
	}
	void Shader::updateMaterial(Material *material)
	{
		material->texture.bind();
		setUniform("Texture",0);
		setSpecular(material);
		setbaseColor(material->color);
		
	}
	void Shader::updateObject(Object *object)
	{
		setmodelMatrix(object->transform);
		updateMaterial(object->material);
		object->mesh->draw();
	}

	void Shader::updateDirectionLight(DirectionalLight *light)
	{
		setUniform("directionalLight.direction",light->getDirection());
		setUniform("directionalLight.base.color",light->getBaseLight().getColor());
		setUniform("directionalLight.base.intensity",light->getBaseLight().getIntensity());
	}
	void Shader::updateAmbientLight(AmbientLight *ambient)
	{
		setUniform("ambientLight",ambient->getAmbient());
	}

	void Shader::updatePointLight(std::string uniformname ,PointLight *point)
	{
		setUniform(uniformname		   ,point->base);
		setUniform(uniformname + ".pos",point->pos);
		setUniform(uniformname + ".atten.constant",point->attenuation.getConstant());
		setUniform(uniformname + ".atten.exponent",point->attenuation.getExponent());
		setUniform(uniformname + ".atten.linear",point->attenuation.getLinear());
		setUniform(uniformname + ".range",point->range);


	}
	void Shader::updateFog(Fog *fog)
	{
		setUniform("fog.density",fog->density);
		setUniform("fog.color",fog->color);
		setUniform("fog.start",fog->start);
		setUniform("fog.type",fog->type);
		setUniform("fog.end",fog->end);
	}

	void Shader::updatePointLights(std::vector<PointLight> &point)
	{
		if(point.size() > MAXPOINTLIGHTS)
		{
			fatalError("Too many PointLights passed in\n");
			return;
		}
		for(unsigned int i =0;i<point.size();i++)
		{
			std::string string("pointLights[");
			string.append(std::to_string(i));
			string = string  + "]";
			updatePointLight(string,&point[i]);
		}


	}

	void Shader::updateSpotLight(std::string uniformname ,SpotLight *spot)
	{
		setUniform(uniformname + ".pointLight"	   ,spot->getPointLight().base);
		setUniform(uniformname + ".pointLight.pos",spot->getPointLight().pos);
		setUniform(uniformname + ".pointLight.atten.constant",spot->getPointLight().attenuation.getConstant());
		setUniform(uniformname + ".pointLight.atten.exponent",spot->getPointLight().attenuation.getExponent());
		setUniform(uniformname + ".pointLight.atten.linear",spot->getPointLight().attenuation.getLinear());
		setUniform(uniformname + ".pointLight.range",spot->getPointLight().range);
		setUniform(uniformname + ".cutoff",spot->getcutoff());
		setUniform(uniformname + ".dir",spot->getDir());


	}

	void Shader::updateSpotLights(std::vector<SpotLight> &spot)
	{
		if(spot.size() > MAXPOINTLIGHTS)
		{
			fatalError("Too many PointLights passed in\n");
			return;
		}
		for(unsigned int i =0;i<spot.size();i++)
		{
			std::string string("spotLights[");
			string.append(std::to_string(i));
			string = string  + "]";
		}


	}

	void Shader::renderBatch()
	{
		pipeline->renderBatches(this);
	}

	void Shader::renderShadowBatch()
	{
		pipeline->renderShadowBatches(this);
	}

	void Shader::addObject(Object* object)
	{
		pipeline->addObject(object);
	}

	void Shader::deleteObject(Object* object)
	{
		pipeline->deleteObject(object);
	}

	void Shader::emptyBatch()
	{
		pipeline->emptyBatch();
	}

	//-------------------------------------------------------------------



	BasicShader::BasicShader()
	{
		_programID = glCreateProgram();
		 if(_programID == 0)
		 {
			 fatalError("failed to create programm");
		 }
		 _numAttributes=0;

	addProgram("BasicShader.vert",GL_VERTEX_SHADER);
	addProgram("BasicShader.frag",GL_FRAGMENT_SHADER);

	  glBindAttribLocation(_programID, _numAttributes++, "positon");
	  glBindAttribLocation(_programID, _numAttributes++, "normal");
	  //Link our program
    glLinkProgram(_programID);

    //Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<char> errorLog(maxLength);
        glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);



        //We don't need the program anymore.
        glDeleteProgram(_programID);
        //Don't leak shaders either.
        //print the error log and quit
        std::printf("%s\n", &(errorLog[0]));
        fatalError("Shaders failed to link!");
    }
}

	void BasicShader::addProgram(std::string path,int type)
{
	int shader = glCreateShader(type);
	if (shader == 0) 
	{
        fatalError("shader failed to be created!");
    }
	//Open the file
    std::ifstream shaderFile(path);
    if (shaderFile.fail()) 
	{
        perror(path.c_str());
        fatalError("Failed to open " + path);
    }

    //File contents stores all the text in the file
    std::string fileContents = "";
    //line is used to grab each line of the file
    std::string line;

    //Get all the lines in the file and add it to the contents
    while (std::getline(shaderFile, line)) 
	{
        fileContents += line + "\n";
    }

    shaderFile.close();

    //get a pointer to our file contents c string;
    const char* contentsPtr = fileContents.c_str();
    //tell opengl that we want to use fileContents as the contents of the shader file
    glShaderSource(shader, 1, &contentsPtr, nullptr);

    //compile the shader
    glCompileShader(shader);

    //check for errors
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

        //Provide the infolog in whatever manor you deem best.
        //Exit with failure.
        glDeleteShader(shader); //Don't leak the shader.

        //Print error log and quit
        std::printf("%s\n", &(errorLog[0]));
        fatalError("Shader " + path + " failed to compile");
    }
	 glAttachShader(_programID, shader);

	
};

	void BasicShader::use() 
{
    glUseProgram(_programID);
    //enable all the attributes we added with addAttribute
    for (int i = 0; i < _numAttributes; i++) 
	{
        glEnableVertexAttribArray(i);
    }
}

//disable the shader
void BasicShader::unuse() 
{
    glUseProgram(0);
    for (int i = 0; i < _numAttributes; i++) 
	{
        glDisableVertexAttribArray(i);
    }
}

//-------------------------------------------




 Shader::ObjectInformation::ObjectInformation(Object* newObject, GLuint Offset = 0,GLuint Count =  0)
{
	object = newObject;
	offset = Offset;
	count = Count;
}
 Shader::ObjectInformation::~ObjectInformation()
{

}
Shader::ObjectInformation::ObjectInformation()
{
}



 Shader::ObjectBatch::ObjectBatch()
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
	glBindVertexArray(0);
	countObjects = 0;
	lastDeleteObjectIndex = -1;
	

}

 void Shader::ObjectBatch::loadBuffer()
 {
	
	
 }
 void Shader::ObjectBatch::loadBufferLast()
 {
	glBindBuffer(GL_ARRAY_BUFFER,vab);
	glBufferSubData(GL_ARRAY_BUFFER,lastInformation.offset * sizeof(Vertex),lastInformation.count * sizeof(Vertex),&lastInformation.object->mesh->model.Vertices[0]);
 }


 void Shader::ObjectBatch::loadBufferIndexToLast()
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
 Shader::ObjectBatch::~ObjectBatch()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1,&vab);
}

 Shader::ShaderObjectPipeLine::ShaderObjectPipeLine()
{
	batches.push_back(new ObjectBatch());
	countBatches = 1;
}
 Shader::ShaderObjectPipeLine::~ShaderObjectPipeLine()
{
	for(unsigned int i = 0; i < countBatches;i++)
	{
		delete(batches[i]);
	}
}

void  Shader::ShaderObjectPipeLine::addObject(Object* newObject)
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
void  Shader::ShaderObjectPipeLine::deleteObject(Object* removeObject)
{
	for(unsigned int i = 0;i < countBatches;i++)
	{
		batches[i]->deleteObject(removeObject);
	}
	
}

void Shader::ObjectBatch::deleteObject(Object* removeObject)
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

bool  Shader::ObjectBatch::checkSize(Object* newObject)
{
	if((newObject->mesh->model.Vertices.size() * sizeof(Vertex)) <= remainingSize)
	{
		return true;
	}
	else return false;
}

void  Shader::ObjectBatch::addObject(Object* newObject)
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

void  Shader::ShaderObjectPipeLine::renderBatches(Shader* shader)
{
	for(unsigned int i = 0; i < countBatches; i++)
	{
		batches[i]->render(shader);
	}
}

void  Shader::ShaderObjectPipeLine::renderShadowBatches(Shader* shader)
{
	for(unsigned int i = 0; i < countBatches; i++)
	{
		batches[i]->renderShadow(shader);
	}
}

void  Shader::ObjectBatch::render(Shader *shader)
{
	glBindVertexArray(vao);
	loadBufferIndexToLast();
	for(auto iter = objects.begin();iter != objects.end();iter++)
	{
			shader->setmodelMatrix(iter->second.object->transform);
			shader->updateMaterial(iter->second.object->material);
			glDrawArrays(GL_TRIANGLES,iter->second.offset,iter->second.count);
	}
	glBindVertexArray(0);
}

void  Shader::ObjectBatch::renderShadow(Shader *shader)
{
	glBindVertexArray(vao);
	loadBufferIndexToLast();
	for(auto iter = objects.begin();iter != objects.end();iter++)
	{
			shader->setUniform("modelMatrix[0]",iter->second.object->transform->getMatrix());
			glDrawArrays(GL_TRIANGLES,iter->second.offset,iter->second.count);
	}
	glBindVertexArray(0);
}

void Shader::ShaderObjectPipeLine::emptyBatch()
{
	for (unsigned int i = 0; i< countBatches; i++)
    {  
       batches[i]->emptyBuffer();
    }
}

 void Shader::ObjectBatch::emptyBuffer()
 {

		objects.clear();
		glBindBuffer(GL_ARRAY_BUFFER,vab);
		glBufferData(GL_ARRAY_BUFFER,maxSize,NULL,GL_STREAM_DRAW);
		lastOffset = 0;
		remainingSize = maxSize;
		countObjects = 0;
 }
