#include "UIrenderer.h"

int SE_UIButton::IDCounter = 0;


UIrenderer::UIrenderer()
{
	shader = new Shader();
	shader->addVertexShader("res/Shaders/2DShader.vert");
	shader->addFragmentShader( "res/Shaders/2DShader.frag");
	shader->linkShaders();
	ortho = Matrix4().identity().InitOrthographic(0.0f, 1000.0f, 0.0f, 1000.0f,-1.0f,1.0f);
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	glGenBuffers(NUMBUFFERS,vab);
}
UIrenderer::~UIrenderer()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(NUMBUFFERS,vab);
	delete(shader);
}

void UIrenderer::addButton(SE_UIButton& newbutton)
{
	buttons.push_back(newbutton);
	std::sort(buttons.begin(), buttons.end());
	loadBuffer();
}

void UIrenderer::addButton(Vector2 Start,Vector2 Size,Vector4 Color,bool Render,std::string texturepath,std::string Name,std::string text,SE_ButtonType type)
{
	buttons.push_back(SE_UIButton(Start,Size,Color,Render,texturepath,Name,text,type));
	std::sort(buttons.begin(), buttons.end());
	loadBuffer();
}

void UIrenderer::draw()
{
	if(!buttons.size() > 1) return;
	glDisable(GL_CULL_FACE);
	shader->use();
	shader->setUniform("ortho",ortho);
	shader->setUniform("diffuse",0);
	glBindVertexArray(vao);
	int offset = 0;
	std::vector<TextData> data;
	for(unsigned int i=0;i < buttons.size();i++)
	{
		if(buttons[i].render)
		{
			SE_UIButton& temp = buttons[i];
			shader->setbaseColor(temp.color);
			temp.texture.bind();
			glDrawArrays(GL_TRIANGLES,offset,6);	
			data.push_back(TextData(temp.name,temp.start.x,temp.start.y + temp.size.y/7,temp.size.x,temp.size.y,Vector3(1,1,1)));
			
		}
			offset += 6;
	}
	glBindVertexArray(0);	
	glEnable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);
	text.RenderText(data);
	
}

SE_UIButton::SE_UIButton(Vector2 Start,Vector2 Size,Vector4 Color,bool Render,std::string texturepath,std::string Name,std::string text,SE_ButtonType type)
{
	start=Start;
	size=Size;
	color = Color;
	render=Render;
	ID=IDCounter;
	texture = TextureCache::getTexture(texturepath);
	name = Name;
	this->text = text;
	this->type = type;
	SE_UIButton::IDCounter++;
	
}
void UIrenderer::loadBuffer()
{
	std::vector<Vector2> positions;
	positions.reserve(buttons.size() * 12);
	for(unsigned int i=0;i< buttons.size();i++)
	{
		
		SE_UIButton &c = buttons[i];
		switch(buttons[i].type)
		{
			case UP:
			{
				c.start.x =- (c.size.x/2);
				
			}break;
			case LEFT:
			{
				c.start.x =- (c.size.x);
				c.start.y =- (c.size.y/2);

			}break;
			case RIGHT:
			{
				c.start.y =- (c.size.y/2);
			}break;
			case DOWN:
			{
				c.start.x =- (c.size.x/2);
				c.start.y =- (c.size.y);
			}break;
			case LEFTUP:
			{
				c.start.x =- (c.size.x);
			}break;
			case RIGHTUP:
			{
				//normal behaviour
			}break;
			case LEFTDOWN:
			{
				c.start.x =- (c.size.x);
				c.start.y =- (c.size.y);
			}break;
			case RIGHTDOWN:
			{
				c.start.y =- (c.size.y);
			}break;
			case CENTER:
			{
				c.start.x =- (c.size.x/2);
				c.start.y =- (c.size.y/2);
			}break;
			default:
			{
				
			}
		}
			
			positions.push_back(buttons[i].start);
			positions.push_back(Vector2(0,0));
			positions.push_back(Vector2(buttons[i].start.x + buttons[i].size.x,buttons[i].start.y));
			positions.push_back(Vector2(1,0));
			positions.push_back(Vector2(buttons[i].start.x + buttons[i].size.x,buttons[i].start.y + buttons[i].size.y));
			positions.push_back(Vector2(1,1));
			positions.push_back(Vector2(buttons[i].start.x + buttons[i].size.x,buttons[i].start.y + buttons[i].size.y));
			positions.push_back(Vector2(1,1));
			positions.push_back(Vector2(buttons[i].start.x ,buttons[i].start.y + buttons[i].size.y));
			positions.push_back(Vector2(0,1));
			positions.push_back(buttons[i].start);
			positions.push_back(Vector2(0,0));
	}
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER,vab[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vector2) * 2,0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(Vector2) * 2 ,(void*)sizeof(Vector2));
	glBufferData(GL_ARRAY_BUFFER,sizeof(Vector2) * 6 * 2 * buttons.size(),&positions[0],GL_STATIC_DRAW);
	glBindVertexArray(0);
	

	glBindVertexArray(0);
}

void UIrenderer::updateOrtho(float width,float height)
{
	ortho = Matrix4().identity().InitOrthographic(0,width,0,height,-1,1);
	text.setProjection(width,height);
}

void Skybox::loadSkybox(std::string Directory, std::string posx, std::string negx, std::string posy, std::string negy, std::string posz, std::string negz) 
{ 
	cube.addFiles(Directory,  posx, negx,  posy,  negy, posz, negz);
	cube.Load();
   glGenVertexArrays(1, &vao); 
   glBindVertexArray(vao); 
		
	GLfloat positions[] = {
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, 
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f}; //positions for a box 

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,36 * 3 * sizeof(positions[0]),NULL,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,36 * 3 *sizeof(positions[0]),&positions[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
	glBindVertexArray(0);
	transform.setScale(Vector3(200,200,200));
}

void Skybox::setSkyboxTexture(std::string Directory, std::string posx, std::string negx, std::string posy, std::string negy, std::string posz, std::string negz) 
{ 
	cube.addFiles(Directory,  posx, negx,  posy,  negy, posz, negz);
	cube.Load();
}

Skybox::Skybox(Camera3d *Camera,Vector4 Color)
{
	shader = new Shader();
	shader->addVertexShader("res/Shaders/Skybox.vert");
	shader->addFragmentShader( "res/Shaders/Skybox.frag");
	shader->addAttribute("position");
	shader->linkShaders();
	color = Color;
	camera = Camera;
}

void Skybox::renderSkybox()
{
	glDepthMask(0);
	
	glCullFace(GL_FRONT); //box vertices are from outside but you are looking from the inside
	glBindVertexArray(vao);
	shader->use();
	transform.setPos(camera->getPos());
	shader->setUniform("MVP", camera->GetViewProjection() * transform.getMatrix() );
	shader->setbaseColor(color);
	cube.bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	shader->unuse();
	glDepthMask(1);
	glCullFace(GL_BACK);
	glBindVertexArray(0);
}
 Skybox::~Skybox()
 {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1,&vbo);
	cube.releaseCubemap();
	delete(shader);
	shader = NULL;
 }

   void  Skybox::setCamera(Camera3d* Camera)
   {
	   camera=Camera;
   };
   void  Skybox::setColor(Vector4 Color)
   {
	   color=Color;
   }
   void  Skybox::setPos(Vector3 Pos)
   {
	   transform.setPos(Pos);
   };
   void  Skybox::setRot(Vector3 Rot)
   {
	   transform.setRot(Rot);
   }
   void  Skybox::setScale(Vector3 Scale)
   {
	   transform.setPos(Scale);
   }



bool SE_UIButton::operator<(SE_UIButton &other)
{
	return texture.ID < other.texture.ID;
}



LineRenderer::LineRenderer()
{
	shader = new Shader();
	shader->addVertexShader("res/Shaders/LineShader.vert");
	shader->addFragmentShader( "res/Shaders/LineShader.frag");
	shader->linkShaders();
	glGenVertexArrays(2,vao);
	glGenBuffers(2,vab);
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER,vab[1]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vector3),0);
	glBufferData(GL_ARRAY_BUFFER,200000,NULL,GL_DYNAMIC_DRAW);
	glBindVertexArray(0);
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER,vab[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vector3),0);
	glBufferData(GL_ARRAY_BUFFER,200000,NULL,GL_DYNAMIC_DRAW);
	glBindVertexArray(0);
	lineCount = 0;
	timedLineCount = 0;
}
LineRenderer::~LineRenderer()
{
	if(shader) delete(shader);
	glDeleteVertexArrays(2,vao);
	glDeleteBuffers(2,vab);
}
GLuint LineRenderer::addLine(Vector3 pos,Vector3 pos2)
{
	static int ID = 0;
	lines.insert(std::make_pair(ID++,Ray(pos,pos2)));
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER,vab[0]);
	std::vector<Vector3> r;
	r.push_back(pos);
	r.push_back(pos2);
	glBufferSubData(GL_ARRAY_BUFFER,lineCount * 2 * sizeof(Vector3),sizeof(Vector3) * 2,&r[0]);
	glBindVertexArray(0);
	lineCount++;
	return ID - 1;
}

void LineRenderer::render(Camera3d* camera)
{
	if(lines.size() < 1 && timedLineCount < 1) return;
	shader->use();
	shader->setUniform("MVP",camera->GetViewProjection());
	if(!lineCount < 1)
	{
		glBindVertexArray(vao[0]);
		glDrawArrays(GL_LINES,0,lineCount * 2);
		glBindVertexArray(0);
	}
	if(!timedLineCount < 1)
	{
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_LINES,0,timedLineCount * 2);
		glBindVertexArray(0);
	}

}

GLuint LineRenderer::addTimedLine(Vector3 pos,Vector3 pos2,float time)
{
	static int ID = 0;
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER,vab[1]);
	std::vector<Vector3> r;
	r.push_back(pos);
	r.push_back(pos2);
	glBufferSubData(GL_ARRAY_BUFFER,timedLineCount * 2 * sizeof(Vector3),sizeof(Vector3) * 2,&r[0]);
	glBindVertexArray(0);
	timedLines.insert(std::make_pair(ID++,TimeRay(Ray(pos,pos2),time,lines.size() * 2)));
	timedLineCount++;
	return ID - 1;
	
}

void LineRenderer::update(float timeinms)
{
	int first = -1;
	int i = 0;
	std::vector<GLuint> toErase;
	for(auto it = timedLines.begin();it != timedLines.end();it++)
	{
		it->second.TTL =- timeinms/1000;
		if(it->second.TTL < 0)
		{
			toErase.push_back(it->first);
			if(first < 0) first = i;
		}
		i++;
	}
	for(unsigned int k = 0;k < toErase.size();k++)
	{
		timedLines.erase(toErase[k]);
	}
	if(first < 0) return;
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER,vab[1]);
	i = 0;
	for(auto it = timedLines.find(toErase[0]);it != timedLines.end();it++)
	{
		
		std::vector<Vector3> r;
		r.push_back(it->second.ray.pos);
		r.push_back(it->second.ray.dir);
		glBufferSubData(GL_ARRAY_BUFFER,(first + i ) * 2 * sizeof(Vector3),sizeof(Vector3) * 2,&r[0]);
		i++;
	}
	glBindVertexArray(0);
}