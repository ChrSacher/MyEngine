#include "Shadow.h"


ShadowMapFBO::ShadowMapFBO(unsigned int ShadowWidth, unsigned int ShadowHeight,unsigned int WindowWidth, unsigned int WindowHeight,Vector3 LightDirection,bool enabled)
{
	fbo.init(Vector2((float)ShadowWidth,(float)ShadowHeight));
	lightDirection  = LightDirection;

	depthProjectionMatrix = Matrix4().identity().InitOrthographic(-10,10,-10,10,-10,10);

	depthViewMatrix = Matrix4().lookAt(lightDirection,Vector3(0,0,0), Vector3(0,1,0));

	depthModelMatrix = Matrix4().identity();

	biasMatrix = Matrix4(	0.5, 0.0, 0.0, 0.0,
							0.0, 0.5, 0.0, 0.0,
							0.0, 0.0, 0.5, 0.0,
							0.5, 0.5, 0.5, 1.0
							);
	calculateMatrices();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	shader = new Shader();
		shader->addVertexShader("Shaders/shadowShader.vert");
		shader->addFragmentShader( "Shaders/shadowShader.frag");
		shader->linkShaders();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return;
}

ShadowMapFBO::~ShadowMapFBO()
{
	fbo.release();
}


void ShadowMapFBO::BindForWriting()
{
	if(isEnabled)
	{
		fbo.activate();
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glCullFace(GL_FRONT);
		glBindTexture(GL_TEXTURE_2D,fbo.texture);
		glViewport(0,0,shadowWidth,shadowHeight);
		shader->use();
		shader->setUniform("depthVP",depthMVP);
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	}
}

void ShadowMapFBO::BindForReading()
{  
	if(isEnabled)
	{	
		shader->unuse();
		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		depthMVP = Matrix4().identity();
		glCullFace(GL_BACK);
		calculateMatrices();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0,0,windowWidth,windowHeight);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		util.bindTexture(fbo.texture,31);
	}
}

void ShadowMapFBO::calculateMatrices()
{
	depthMVP =  depthProjectionMatrix * depthViewMatrix *  depthModelMatrix;
}

void ShadowMapFBO::setLightDirection(Vector3 LightDirection)
{
	lightDirection = LightDirection;
	depthViewMatrix = Matrix4().lookAt(lightDirection , Vector3(0,0,0), Vector3(0,1,0));
	calculateMatrices();
}
