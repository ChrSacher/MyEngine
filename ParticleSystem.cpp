#include "ParticleSystem.h"




ParticleSystem::ParticleSystem(void)
{
	
}
void ParticleSystem::init()
{
	shader = new Shader();
	shader->addVertexShader("res/Shaders/particleShader.vert");
	shader->addFragmentShader( "res/Shaders/particleShader.frag");
	shader->addAttribute("position");
	shader->addAttribute("color");
	shader->linkShaders();

	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER,ParticleBuffer.size() * sizeof(ParticleBuffer[0].pos[0]), NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Particle),(void*)offsetof(Particle,pos));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(Particle),(void*)offsetof(Particle,color));
	
	
	glBindVertexArray(0);
	lastParticle=0;
	lastActive = 0;
}



ParticleSystem::~ParticleSystem(void)
{
	glDeleteBuffers(1,&particles_position_buffer);
	glDeleteVertexArrays(1,&vao);
	delete(shader);
	shader = NULL;
}

void ParticleSystem::draw()
{    
	shader->use();
    if (lastActive > 0)
    {
		shader->setUniform("eyePos",camera->getPos());
		camera->update(shader);
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, lastActive);
        glBindVertexArray(0);
    }
	shader->unuse();
}
void ParticleSystem::addParticle(Vector3 &Pos,Vector3 &Vel,Vector4 &Color)
{
	ParticleBuffer.insert(ParticleBuffer.begin(),Particle(Pos,Vel,Color));
	lastActive++;
}
void ParticleSystem::update()
{

	//
	for(int i = 0; i < lastActive;i++)
	{
		if(ParticleBuffer[i].lifeTime > 0)
		{
			ParticleBuffer[i].pos += ParticleBuffer[i].vel * Time::delta;
			ParticleBuffer[i].lifeTime =-Time::delta;
		}
		else
		{
			ParticleBuffer.push_back(ParticleBuffer[i]);
			ParticleBuffer.erase(ParticleBuffer.begin() + i);
			std::swap(ParticleBuffer[i],ParticleBuffer[lastActive]);
			while(ParticleBuffer[lastActive].lifeTime <= 0 && lastActive > 0)
			{
				lastActive --;
			}
		}
	}
	//do everything before this
	if(ParticleBuffer.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER,sizeof(Particle) *  ParticleBuffer.size(), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(Particle) *  lastActive,&ParticleBuffer[0]);		
		
	}

}
