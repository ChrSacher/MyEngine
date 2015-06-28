#include "ParticleSystem.h"




ParticleSystem::ParticleSystem(void)
{
	
}
void ParticleSystem::init()
{
	shader = new Shader();
	shader->addVertexShader("Shaders/particleShader.vert");
	shader->addFragmentShader( "Shaders/particleShader.frag");
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
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Particle),0);
	
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER,ParticleBuffer.size() * sizeof(ParticleBuffer[0].color[0]), NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(Particle),0);
	
	glBindVertexArray(0);
	lastParticle=0;
}



ParticleSystem::~ParticleSystem(void)
{
	delete(shader);
}

void ParticleSystem::draw()
{    
	shader->use();
    const size_t count = lastParticle;
    if (count > 0)
    {
		shader->setUniform("eyePos",camera->getPos());
		shader->setviewMatrix(camera);
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, count);
        glBindVertexArray(0);
    }
	shader->unuse();
}

void ParticleSystem::update(double deltatime)
{

	//
	float time = 0;
	int i = 0;
	for(auto &x : ParticleBuffer)
	{
		if(x.lifeTime >= 0)
		{
			x.pos += x.vel;
			x.lifeTime =- time;
		}
	}
	//do everything before this
	if(ParticleBuffer.size() > 0)
	{
		
		std::vector<Vector3> positions;
		std::vector<Vector4> colors;
		positions.reserve(ParticleBuffer.size());
		colors.reserve(ParticleBuffer.size());
		for(unsigned int i = 0;i< ParticleBuffer.size();i++)
		{
			if(ParticleBuffer[i].lifeTime >= 0)
			{
			positions.push_back(ParticleBuffer[i].pos);
			colors.push_back(ParticleBuffer[i].color);
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER,ParticleBuffer.size() * sizeof(ParticleBuffer[0].pos[0]) * 3, NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(ParticleBuffer[0].pos[0]) * 3 * ParticleBuffer.size(),&positions[0]);		
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glBufferData(GL_ARRAY_BUFFER,ParticleBuffer.size() * sizeof(ParticleBuffer[0].color[0]) * 4, NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(ParticleBuffer[0].color[0]) * 4 * ParticleBuffer.size(),&colors[0]);
		
	}

}
