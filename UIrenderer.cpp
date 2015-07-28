#include "UIrenderer.h"

int SE_UIButton::IDCounter = 0;


UIrenderer::UIrenderer()
{
	shader = new Shader();
	shader->addVertexShader("res/Shaders/2DShader.vert");
	shader->addFragmentShader( "res/Shaders/2DShader.frag");
	shader->addAttribute("position");
	shader->addAttribute("uv");
	shader->linkShaders();
	ortho= Matrix4().identity().InitOrthographic(0,1000,0,1000,-1,1);
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
	glClear(GL_DEPTH_BUFFER_BIT);
	text.RenderText(data);
	glEnable(GL_CULL_FACE);
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
	std::vector<Vector2> uvs;
	positions.reserve(buttons.size() * 6);
	uvs.reserve(buttons.size() * 6);
	for(unsigned int i=0;i< buttons.size();i++)
	{
		
		SE_UIButton &c = buttons[i];
		switch(buttons[i].type)
		{
			case UP:
			{
				c.start.x =- (c.size.x/2);
				
			}
			case LEFT:
			{
				c.start.x =- (c.size.x);
				c.start.y =- (c.size.y/2);

			}
			case RIGHT:
			{
				c.start.y =- (c.size.y/2);
			}
			case DOWN:
			{
				c.start.x =- (c.size.x/2);
				c.start.y =- (c.size.y);
			}
			case LEFTUP:
			{
				c.start.x =- (c.size.x);
			}
			case RIGHTUP:
			{
				//normal behaviour
			}
			case LEFTDOWN:
			{
				c.start.x =- (c.size.x);
				c.start.y =- (c.size.y);
			}
			case RIGHTDOWN:
			{
				c.start.y =- (c.size.y);
			}
			case CENTER:
			{
				c.start.x =- (c.size.x/2);
				c.start.y =- (c.size.y/2);
			}
			default:
			{
				
			}
		}
			
			positions.push_back(buttons[i].start);
			positions.push_back(Vector2(buttons[i].start.x + buttons[i].size.x,buttons[i].start.y));
			positions.push_back(Vector2(buttons[i].start.x + buttons[i].size.x,buttons[i].start.y + buttons[i].size.y));
			positions.push_back(Vector2(buttons[i].start.x + buttons[i].size.x,buttons[i].start.y + buttons[i].size.y));
			positions.push_back(Vector2(buttons[i].start.x ,buttons[i].start.y + buttons[i].size.y));
			positions.push_back(buttons[i].start);

			uvs.push_back(Vector2(0,0));
			uvs.push_back(Vector2(1,0));
			uvs.push_back(Vector2(1,1));
			uvs.push_back(Vector2(1,1));
			uvs.push_back(Vector2(0,1));
			uvs.push_back(Vector2(0,0));
	}
	

	glBindBuffer(GL_ARRAY_BUFFER,vab[POSITIONVB]);
	glBufferData(GL_ARRAY_BUFFER,buttons.size() * 6 * sizeof(positions[0]),0,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,buttons.size() * 6  * sizeof(positions[0]),&positions[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);

	
	glBindBuffer(GL_ARRAY_BUFFER,vab[TEXTUREVB]);
	glBufferData(GL_ARRAY_BUFFER,buttons.size() * 6 * sizeof(uvs[0]),0,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,buttons.size() * 6  * sizeof(uvs[0]),&uvs[0]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);

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

Skybox::Skybox(Camera3d &Camera,Vector4 Color)
{
	shader = new Shader();
	shader->addVertexShader("res/Shaders/Skybox.vert");
	shader->addFragmentShader( "res/Shaders/Skybox.frag");
	shader->addAttribute("position");
	shader->linkShaders();
	color = Color;
	camera = &Camera;
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

