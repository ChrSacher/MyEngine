#include "Shader.h"

GLuint Shader::currentActiveShader = -1;
GLuint Shader::lastActiveShader = -2;
Shader::Shader() : _numAttributes(0)
{
	/*create program */
	 _programID = glCreateProgram();
	 if(_programID == 0)
	 {
		 fatalError("failed to create programm");
	 }
}

Shader::~Shader()
{
	glUseProgram(0);
	glDeleteProgram(_programID);
	
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
	if(Shader::currentActiveShader == _programID) return;
	Shader::lastActiveShader = Shader::currentActiveShader;
	Shader::currentActiveShader =_programID;
    glUseProgram(_programID);
	
    //enable all the attributes we added with addAttribute
    for (int i = 0; i < _numAttributes; i++) 
	{
        glEnableVertexAttribArray(i);
    }
}

void Shader::setMVP(Matrix4 &view,Matrix4 &model)
{
	setUniform("MVP",view * model);
}
void Shader::setMVP(Matrix4 &model)
{
	setUniform("MVP",matrices.view * model);
}
void Shader::unuse() 
{
	//disable the shader
	if(Shader::lastActiveShader == Shader::currentActiveShader)
	{
		Shader::lastActiveShader = 0;
	}
    glUseProgram(Shader::lastActiveShader);
	Shader::currentActiveShader = Shader::lastActiveShader;
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
		uniforms.insert(make_pair(uniformName, UniformPosition(location,Vector4())));

        return location; 
    }
   return mit->second.position;
}

	void Shader::setUniform(std::string uniformName, int value)
	{
		glUniform1i(getUniformLocation(uniformName), value);
		auto &r = uniforms.find(uniformName);
		r->second.value.x = value;
		r->second.type = UTINT;
	
	}

	void Shader::setUniform(std::string uniformName, bool value)
	{
		glUniform1i(getUniformLocation(uniformName), value);
		auto &r = uniforms.find(uniformName);
		r->second.value.x = value;
		r->second.type = UTBOOL;
	}

	void Shader::setUniform(std::string uniformName, float value)
	{
		glUniform1f(getUniformLocation(uniformName), value);
		auto &r = uniforms.find(uniformName);
		r->second.value.x = value;
		r->second.type = UTFLOAT;
	}
	
	void Shader::setUniform(std::string uniformName, Vector3 value)
	{
		glUniform3f(getUniformLocation(uniformName), value[0],value[1],value[2]);
		auto &r = uniforms.find(uniformName);
		r->second.value = Vector4(value.x,value.y,value.z,0);
		r->second.type = UTVECTOR3;
	}
	void Shader::setUniform(std::string uniformName, Vector4 value)
	{
		glUniform4f(getUniformLocation(uniformName), value[0],value[1],value[2],value[3]);
		auto &r = uniforms.find(uniformName);
		r->second.value = value;
		r->second.type = UTVECTOR4;
	}
	
	void Shader::setUniform(std::string uniformName, Matrix4 value)
	{
		glUniformMatrix4fv(getUniformLocation(uniformName),1,GL_FALSE,&value[0]);
		auto &r = uniforms.find(uniformName);
		r->second.type = UTMATRIX;
	}
	//standard shader specific settings

	void Shader::setmodelMatrices(std::vector<Matrix4*> &modelMatrices) 
	{
		std::string string2 =  "modelMatrix[0]";
		for(unsigned int i = 0; i < modelMatrices.size();i++)
		{
			string2 = "modelMatrix[" + std::to_string(i) + "]";
			setUniform(string2, *modelMatrices[i]);
		}
	}
	

	void Shader::setbaseColor(Vector3 Color)
	{
		setUniform("baseColor",Color);
	}

	void Shader::setbaseColor(Vector4 Color)
	{
		setUniform("baseColor",Color);
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

	addProgram("Shaders/BasicShader.vert",GL_VERTEX_SHADER);
	addProgram("Shaders/BasicShader.frag",GL_FRAGMENT_SHADER);

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



void UniformPosition::get(bool &boolReturn)
{
	boolReturn = value.x > 0.5;
}
void UniformPosition::get(float &returnFloat)
{
	returnFloat = value.x;
}
void UniformPosition::get(int &returnInt)
{
	returnInt = (int) value.x;
}
void UniformPosition::get(Vector2 &returnV2)
{
	returnV2.x = value.x;
	returnV2.y = value.y;
}
void UniformPosition::get(Vector3 &returnV3)
{
	returnV3.x = value.x;
	returnV3.y = value.y;
	returnV3.z = value.z;
	
}
void UniformPosition::get(Vector4 &returnV4)
{
	returnV4.x = value.x;
	returnV4.y = value.y;
	returnV4.z = value.z;
	returnV4.w = value.w;
	
}

