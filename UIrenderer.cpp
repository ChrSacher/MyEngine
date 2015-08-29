#include "UIrenderer.h"

int OverlayField::IDCounter = 0;


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

void UIrenderer::addOverlay(OverlayField& newbutton)
{
	buttons.push_back(newbutton);
	std::sort(buttons.begin(), buttons.end());
	loadBuffer();
}

void UIrenderer::addOverlay(Vector2 Start,Vector2 Size,Vector4 Color,bool Render,std::string texturepath,std::string Name,std::string text,SE_ButtonType type)
{
	buttons.push_back(OverlayField(Start,Size,Color,Render,texturepath,Name,text,type));
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
			OverlayField& temp = buttons[i];
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
	ServiceLocator::getText().renderText(data);
	
}

OverlayField::OverlayField(Vector2 Start,Vector2 Size,Vector4 Color,bool Render,std::string texturepath,std::string Name,std::string text,SE_ButtonType type)
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
	OverlayField::IDCounter++;
	
}
void UIrenderer::loadBuffer()
{
	std::vector<Vector2> positions;
	positions.reserve(buttons.size() * 12);
	for(unsigned int i=0;i< buttons.size();i++)
	{
		
		OverlayField &c = buttons[i];
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
	color = Vector3(1,1,1);
   glGenVertexArrays(1, &vao); 
   glBindVertexArray(vao); 
		Dir = Directory;
	fileNames[0] = posx;
	fileNames[1] = negx;
	fileNames[2] = posy;
	fileNames[3] = negy;
	fileNames[4] = posz;
	fileNames[5] = negz;
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
	Dir = Directory;
	fileNames[0] = posx;
	fileNames[1] = negx;
	fileNames[2] = posy;
	fileNames[3] = negy;
	fileNames[4] = posz;
	fileNames[5] = negz;
	cube.Load();
}

Skybox::Skybox(Vector3 Color)
{
	shader = new Shader();
	shader->addVertexShader("res/Shaders/Skybox.vert");
	shader->addFragmentShader( "res/Shaders/Skybox.frag");
	shader->addAttribute("position");
	shader->linkShaders();
	color = Color;
}

void Skybox::renderSkybox(Camera3d* camera)
{
	glDepthMask(0);
	
	glCullFace(GL_FRONT); //box vertices are from outside but you are looking from the inside
	glBindVertexArray(vao);
	shader->use();
	if(camera) transform.setPos(camera->getPos());
	shader->setMVP(camera->GetViewProjection(),*transform.getMatrix());
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

   void  Skybox::setColor(Vector3 Color)
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
  std::vector<std::string>  Skybox::getDirAndFile()
{
	std::vector<std::string> names;
	names.push_back(Dir);
	names.push_back(fileNames[0]);
	names.push_back(fileNames[1]);
	names.push_back(fileNames[2]);
	names.push_back(fileNames[3]);
	names.push_back(fileNames[4]);
	names.push_back(fileNames[5]);
	return names;
}


bool OverlayField::operator<(OverlayField &other)
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

#include <SDL/SDL_timer.h>

CEGUI::OpenGL3Renderer* GUI::renderer = nullptr;

void GUI::init(const std::string& resourceDirectory) 
{
    // Check if the renderer and system were not already initialized
    if (renderer == nullptr) {
        renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();
		// create CEGUI system object
        CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
        rp->setResourceGroupDirectory("imagesets", resourceDirectory + "/imagesets/");
        rp->setResourceGroupDirectory("schemes", resourceDirectory + "/schemes/");
        rp->setResourceGroupDirectory("fonts", resourceDirectory + "/fonts/");
        rp->setResourceGroupDirectory("layouts", resourceDirectory + "/layouts/");
        rp->setResourceGroupDirectory("looknfeels", resourceDirectory + "/looknfeel/");
        rp->setResourceGroupDirectory("lua_scripts", resourceDirectory + "/lua_scripts/");


        CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
        CEGUI::Scheme::setDefaultResourceGroup("schemes");
        CEGUI::Font::setDefaultResourceGroup("fonts");
        CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
        CEGUI::WindowManager::setDefaultResourceGroup("layouts");
        CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
		renderer->grabTextures();
		renderer->restoreTextures();
    }
	static int i = 0;
    context = &CEGUI::System::getSingleton().createGUIContext(renderer->getDefaultRenderTarget());
    root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root" + std::to_string(i++));
    context->setRootWindow(root);
}

void GUI::destroy() {
    CEGUI::System::getSingleton().destroyGUIContext(*context);
}

void GUI::draw() 
{
	renderer->enableExtraStateSettings(true);
    renderer->beginRendering();
	
    context->draw();
	
    renderer->endRendering();
	glDisable(GL_SCISSOR_TEST);
	renderer->enableExtraStateSettings(false);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void GUI::update(float delta) 
{
    context->injectTimePulse(delta / 1000.0f);
}

void GUI::setMouseCursor(const std::string& imageFile) 
{
    context->getMouseCursor().setDefaultImage(imageFile);
}

void GUI::showMouseCursor()
{
    context->getMouseCursor().show();
}

void GUI::hideMouseCursor() 
{
    context->getMouseCursor().hide();
}

CEGUI::Key::Scan SDLKeyToCEGUIKey(SDL_Keycode key) {
    using namespace CEGUI;
    switch (key) {
        case SDLK_BACKSPACE:    return Key::Backspace;
        case SDLK_TAB:          return Key::Tab;
        case SDLK_RETURN:       return Key::Return;
        case SDLK_PAUSE:        return Key::Pause;
        case SDLK_ESCAPE:       return Key::Escape;
        case SDLK_SPACE:        return Key::Space;
        case SDLK_COMMA:        return Key::Comma;
        case SDLK_MINUS:        return Key::Minus;
        case SDLK_PERIOD:       return Key::Period;
        case SDLK_SLASH:        return Key::Slash;
        case SDLK_0:            return Key::Zero;
        case SDLK_1:            return Key::One;
        case SDLK_2:            return Key::Two;
        case SDLK_3:            return Key::Three;
        case SDLK_4:            return Key::Four;
        case SDLK_5:            return Key::Five;
        case SDLK_6:            return Key::Six;
        case SDLK_7:            return Key::Seven;
        case SDLK_8:            return Key::Eight;
        case SDLK_9:            return Key::Nine;
        case SDLK_COLON:        return Key::Colon;
        case SDLK_SEMICOLON:    return Key::Semicolon;
        case SDLK_EQUALS:       return Key::Equals;
        case SDLK_LEFTBRACKET:  return Key::LeftBracket;
        case SDLK_BACKSLASH:    return Key::Backslash;
        case SDLK_RIGHTBRACKET: return Key::RightBracket;
        case SDLK_a:            return Key::A;
        case SDLK_b:            return Key::B;
        case SDLK_c:            return Key::C;
        case SDLK_d:            return Key::D;
        case SDLK_e:            return Key::E;
        case SDLK_f:            return Key::F;
        case SDLK_g:            return Key::G;
        case SDLK_h:            return Key::H;
        case SDLK_i:            return Key::I;
        case SDLK_j:            return Key::J;
        case SDLK_k:            return Key::K;
        case SDLK_l:            return Key::L;
        case SDLK_m:            return Key::M;
        case SDLK_n:            return Key::N;
        case SDLK_o:            return Key::O;
        case SDLK_p:            return Key::P;
        case SDLK_q:            return Key::Q;
        case SDLK_r:            return Key::R;
        case SDLK_s:            return Key::S;
        case SDLK_t:            return Key::T;
        case SDLK_u:            return Key::U;
        case SDLK_v:            return Key::V;
        case SDLK_w:            return Key::W;
        case SDLK_x:            return Key::X;
        case SDLK_y:            return Key::Y;
        case SDLK_z:            return Key::Z;
        case SDLK_DELETE:       return Key::Delete;
        case SDLK_KP_PERIOD:    return Key::Decimal;
        case SDLK_KP_DIVIDE:    return Key::Divide;
        case SDLK_KP_MULTIPLY:  return Key::Multiply;
        case SDLK_KP_MINUS:     return Key::Subtract;
        case SDLK_KP_PLUS:      return Key::Add;
        case SDLK_KP_ENTER:     return Key::NumpadEnter;
        case SDLK_KP_EQUALS:    return Key::NumpadEquals;
        case SDLK_UP:           return Key::ArrowUp;
        case SDLK_DOWN:         return Key::ArrowDown;
        case SDLK_RIGHT:        return Key::ArrowRight;
        case SDLK_LEFT:         return Key::ArrowLeft;
        case SDLK_INSERT:       return Key::Insert;
        case SDLK_HOME:         return Key::Home;
        case SDLK_END:          return Key::End;
        case SDLK_PAGEUP:       return Key::PageUp;
        case SDLK_PAGEDOWN:     return Key::PageDown;
        case SDLK_F1:           return Key::F1;
        case SDLK_F2:           return Key::F2;
        case SDLK_F3:           return Key::F3;
        case SDLK_F4:           return Key::F4;
        case SDLK_F5:           return Key::F5;
        case SDLK_F6:           return Key::F6;
        case SDLK_F7:           return Key::F7;
        case SDLK_F8:           return Key::F8;
        case SDLK_F9:           return Key::F9;
        case SDLK_F10:          return Key::F10;
        case SDLK_F11:          return Key::F11;
        case SDLK_F12:          return Key::F12;
        case SDLK_F13:          return Key::F13;
        case SDLK_F14:          return Key::F14;
        case SDLK_F15:          return Key::F15;
        case SDLK_RSHIFT:       return Key::RightShift;
        case SDLK_LSHIFT:       return Key::LeftShift;
        case SDLK_RCTRL:        return Key::RightControl;
        case SDLK_LCTRL:        return Key::LeftControl;
        case SDLK_RALT:         return Key::RightAlt;
        case SDLK_LALT:         return Key::LeftAlt;
        case SDLK_SYSREQ:       return Key::SysRq;
        case SDLK_MENU:         return Key::AppMenu;
        case SDLK_POWER:        return Key::Power;
        default:                return Key::Unknown;
    }
}

CEGUI::MouseButton SDLButtonToCEGUIButton(Uint8 sdlButton) 
{
    switch (sdlButton) 
	{
        case SDL_BUTTON_LEFT: return CEGUI::MouseButton::LeftButton;
        case SDL_BUTTON_MIDDLE: return CEGUI::MouseButton::MiddleButton;
        case SDL_BUTTON_RIGHT: return CEGUI::MouseButton::RightButton;
        case SDL_BUTTON_X1: return CEGUI::MouseButton::X1Button;
        case SDL_BUTTON_X2: return CEGUI::MouseButton::X2Button;
    }
    return CEGUI::MouseButton::NoButton;
}

void GUI::onSDLEvent(SDL_Event& evnt) 
{
    CEGUI::utf32 codePoint;
    switch (evnt.type)
	{
		case SDL_WINDOWEVENT:
		{
			switch (evnt.window.event)
			{
				case SDL_WINDOWEVENT_RESIZED:
				{
					renderer->setDisplaySize(CEGUI::Sizef(evnt.window.data1, evnt.window.data2));
					renderer->grabTextures();
					renderer->restoreTextures();
				}
			}
		}break;

        case SDL_MOUSEMOTION:
            context->injectMousePosition(evnt.motion.x, evnt.motion.y);
            break;
        case SDL_KEYDOWN:
            context->injectKeyDown(SDLKeyToCEGUIKey(evnt.key.keysym.sym));
            break;
        case SDL_KEYUP:
            context->injectKeyUp(SDLKeyToCEGUIKey(evnt.key.keysym.sym));
            break;
        case SDL_TEXTINPUT:
            codePoint = 0;
            for (int i = 0; evnt.text.text[i] != '\0'; i++) {
                codePoint |= (((CEGUI::utf32 )evnt.text.text[i]) << (i * 8));
            }
            context->injectChar(codePoint);
            break;
        case SDL_MOUSEBUTTONDOWN:
            context->injectMouseButtonDown(SDLButtonToCEGUIButton(evnt.button.button));
            break;
        case SDL_MOUSEBUTTONUP:
            context->injectMouseButtonUp(SDLButtonToCEGUIButton(evnt.button.button));
            break;
		
    }
}

void GUI::loadScheme(const std::string& schemeFile) 
{
    CEGUI::SchemeManager::getSingleton().createFromFile(schemeFile);
}

CEGUI::Window* GUI::createWidget(const std::string& type, const Vector4& destRectPerc, const Vector4& destRectPix, const std::string& name /*= ""*/) 
{
    CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().createWindow(type, name);
    root->addChild(newWindow);
    setWidgetDestRect(newWindow, destRectPerc, destRectPix);
    return newWindow;
}

void GUI::setWidgetDestRect(CEGUI::Window* widget, const Vector4& destRectPerc, const Vector4& destRectPix)
{
    widget->setPosition(CEGUI::UVector2(CEGUI::UDim(destRectPerc.x, destRectPix.x), CEGUI::UDim(destRectPerc.y, destRectPix.y)));
    widget->setSize(CEGUI::USize(CEGUI::UDim(destRectPerc.z, destRectPix.z), CEGUI::UDim(destRectPerc.w, destRectPix.w)));
}

void GUI::setFont(const std::string& fontFile) 
{
    CEGUI::FontManager::getSingleton().createFromFile(fontFile + ".font");
    context->setDefaultFont(fontFile);
}