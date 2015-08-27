#include "RenderUtil.h"


RenderUtil::RenderUtil(void)
{
	CurrentActiveTexture = -1;
	CurrentProgram = -1;
	CurrentVertexArray = -1;
}


RenderUtil::~RenderUtil(void)
{
}

void RenderUtil::clearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);
}
void RenderUtil::initGraphics()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	//glEnableClientState(GL_VERTEX_ARRAY);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glReadBuffer(GL_BACK);
	glDrawBuffer(GL_BACK);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE); //enable shader point operations for particle engine
	//glEnable(GL_FRAMEBUFFER_SRGB);
	glClearDepth(1.0f);
	glPointSize(5);
	glEnable( GL_POINT_SPRITE ); // GL_POINT_SPRITE_ARB if you're
    glLineWidth(12);                     // using the functionality as an extension
    glEnable( GL_POINT_SMOOTH );
	
    //Set the background color to blue
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::string RenderUtil::getOpenGLVersion()
{
	return "ok";
}

void RenderUtil::genBuffer(GLuint Num,GLuint &Buffer)
{
	
	glGenBuffers(Num,&Buffer);
	if(Num > 1)
	{
		for(unsigned int i = 0; i < Num;i++)
		{
			_Buffers.push_back(Buffer[&i]);
		}
	}
	else
	{
		_Buffers.push_back(Buffer);
	}
	
}

void RenderUtil::genVertexArray(GLuint Num,GLuint &Buffer)
{
	glGenVertexArrays(Num,&Buffer);
	if(Num > 1)
	{
		for(unsigned int i = 0; i < Num;i++)
		{
			_VertexArrays.push_back(Buffer[&i]);
		}
	}
	else
	{
		_VertexArrays.push_back(Buffer);
	}
}

void RenderUtil::genTexture(GLuint Num,GLuint &Buffer)
{ 
	glGenTextures(Num, &Buffer);
	if(Num > 1)
	{
		for(unsigned int i = 0; i < Num;i++)
		{
			_Textures.push_back(Buffer[&i]);
		}
	}
	else
	{
		_Textures.push_back(Buffer);
	}
}
//add Operations
void RenderUtil::addProgram(GLuint Program)//shader has to run this function when a program is completed
{ 
		_Programs.push_back(Program);
} 
//Binding Operations

void   RenderUtil::bindTexture(GLuint ID,GLuint unit,GLenum TextureType)
{
	_BT.bind(ID,unit,TextureType);
}

void   RenderUtil::unbindTexture(GLuint ID)
{
	_BT.unbind(ID);
}

void   RenderUtil::activeTexture(GLuint Unit)
{ 
	if(CurrentActiveTexture != Unit)
	{
		glActiveTexture(GL_TEXTURE0 + Unit);
		CurrentActiveTexture = Unit;
	}
}

void   RenderUtil::bindVertexArray(GLuint ID)
{ 
	
	if(CurrentVertexArray != ID)
	{
		glBindVertexArray(ID);
		CurrentVertexArray = ID;
	}
	

}

void RenderUtil::bindBuffer(GLenum e,GLuint ID)
{
	glBindBuffer(e,ID);
}
void   RenderUtil::useProgram(GLuint ID)
{ 
	if(CurrentProgram != ID)
	{
		glUseProgram(ID);
		CurrentProgram = ID;
	}
	

}

//Delete Operations

void RenderUtil::deleteVertexArrays(GLuint Num,GLuint &Buffer)
{  
	if(Num > 1)
	{
		for(GLuint i = 0; i < Num;i++)
		{
			for(GLuint j = 0; j < _VertexArrays.size();j++)
			{
				if(_VertexArrays[j] == Buffer[&i])
				{
					_VertexArrays.erase(_VertexArrays.begin() + j);
					j -= 1;
				}
			}
		}
	}
	glDeleteVertexArrays(Num,&Buffer);
}

void RenderUtil::deleteBuffers(GLuint Num,GLuint &Buffer)
{ 
	if(Num > 1)
	{
		for(GLuint i = 0; i < Num;i++)
		{
			for(GLuint j = 0; j < _Buffers.size();j++)
			{
				if(_Buffers[j] == Buffer[&i])
				{
					_Buffers.erase(_Buffers.begin() + j);
					j -= 1;
				}
			}
		}
	}
	glDeleteBuffers(Num,&Buffer);
}

void RenderUtil::deleteTextures(GLuint Num,GLuint &Texture)
{  
	if(Num > 1)
	{
		for(GLuint i = 0; i < Num;i++)
		{
			for(GLuint j = 0; j < _Textures.size();j++)
			{
				if(_Textures[j] == Texture)
				{
					_Textures.erase(_Textures.begin() + j);
					j -= 1;
					unbindTexture(Texture);
				}
			}
		}
	}
	glDeleteTextures(Num,&Texture);
}

void RenderUtil::deleteProgram(GLuint ID)
{  
		for(GLuint j = 0; j < _Programs.size();j++)
		{
			if(_Programs[j] == ID)
			{
				_Programs.erase(_Programs.begin() + j);
				j -= 1;
			}
		}
		glDeleteProgram(ID);
}






bool BoundTexture::_isBound(GLuint ID,GLuint unit)
{
	if(_units[unit] == ID) return true;
	return false;
	
}

void BoundTexture::unbind(GLuint ID,GLenum TextureType)
{
		for(unsigned int i = 0; i < 31 ; i++)
		{
			if(_units[i] == ID)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(TextureType,0);
				_units[i] = -1;
			}
		}
}

void BoundTexture::bind(GLuint ID,GLuint unit,GLenum TextureType)
{
		if(!(_isBound(ID,unit)))
		{
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(TextureType, ID);
			_units[unit] = ID;
		}

}

void RenderUtil::renderInWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	renderMode = GL_LINE;
}
void RenderUtil::renderInPolygone()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	renderMode = GL_FILL;
}

void RenderUtil::switchRender()
{
	if(renderMode != GL_LINE){ renderInWireframe();}
	else renderInPolygone();
}