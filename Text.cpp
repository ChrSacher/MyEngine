#include "Text.h"

Text::~Text()
{
	
}
void Text::destroy()
{
	delete(shader);
	for(auto i = Characters.begin(); i != Characters.end();i++)
	{
		glDeleteTextures(1,&i->second.TextureID);
	}
}

Text::Text(){}
void Text::initialize()
{
		shader = new Shader();
		shader->addVertexShader("res/Shaders/textShader.vert");
		shader->addFragmentShader( "res/Shaders/textShader.frag");
		shader->linkShaders();
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4 * 1000, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&library))
		{
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			valid = false;
			return;
		}

		// Load font as face
		projection = Matrix4().identity().InitOrthographic(0.0f, 1000.0f, 0.0f, 1000.0f,-1.0f,1.0f);
		if (FT_New_Face(library, "res/arial.ttf", 0, &face))
		{
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			valid = false;
			return;
		}
		// Set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
		for (GLubyte c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph "<<c << std::endl;
				continue;
			}
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = 
			{
				texture,
				Vector2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};
			Characters.insert(std::pair<GLchar, Character>(c, character));
		}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::renderText(std::string text, GLfloat x, GLfloat y, GLfloat SX, GLfloat SY, Vector3 color)
	{
		// Activate corresponding render state	
		shader->use();
		shader->setUniform("textColor", color);
		shader->setUniform("projection",projection);
		
		glBindVertexArray(vao);
		GLfloat ssx = SX / (text.size() * 25 );
		GLfloat ssy = SY / 50;
		// Iterate through all characters
		std::string::const_iterator c;
		std::vector<Vector4> vertices;
		vertices.reserve(text.size() * 6);
		for (c = text.begin(); c != text.end(); c++) 
		{
			Character &ch = Characters[*c];

			GLfloat xpos = x + ch.Bearing.x * ssx;
			GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * ssy;

			GLfloat w = ch.Size.x * ssx;
			GLfloat h = ch.Size.y * ssy;
			// Update VBO for each character
			
			vertices.push_back(Vector4( xpos,     ypos + h,   0.0, 0.0 ));            
			vertices.push_back(Vector4( xpos,     ypos,       0.0, 1.0 ));
			vertices.push_back(Vector4(xpos + w, ypos,       1.0, 1.0 ));
			vertices.push_back(Vector4( xpos,     ypos + h,   0.0, 0.0 ));
			vertices.push_back(Vector4( xpos + w, ypos,       1.0, 1.0 ));
			vertices.push_back(Vector4( xpos + w, ypos + h,   1.0, 0.0 ));           
			// Render glyph texture over quad
			
			
			
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x = x + (ch.Advance >> 6) * ssx; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector4) * vertices.size(), &vertices[0]); // Be sure to use glBufferSubData and not glBufferData
			// Render quad
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		shader->setUniform("text",28);
		glActiveTexture(GL_TEXTURE28);
		int offset = 0;
		glDisable(GL_CULL_FACE);
		for (c = text.begin(); c != text.end(); c++) 
		{
			
			glBindTexture(GL_TEXTURE_2D, Characters.find(*c)->second.TextureID);
			// Update content of VBO memory
			glDrawArrays(GL_TRIANGLES, offset * 6, 6);
			offset++;
		}
		glBindVertexArray(0);
		//shader->unuse();
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_CULL_FACE);
	}
void Text::renderText(std::vector<TextData> &data)
{
	for(unsigned int i = 0; i < data.size();i++)
	{
		renderText(data[i].text,data[i].x,data[i].y,data[i].sx,data[i].sy,data[i].color);
	}
}

void Text::renderText(std::string text, GLfloat x, GLfloat y,  GLfloat scale, Vector3 color)
{
	// Activate corresponding render state	
    shader->use();
    shader->setUniform("textColor", color);
	shader->setUniform("projection",projection);
    glActiveTexture(GL_TEXTURE28);
    glBindVertexArray(vao);

    // Iterate through all characters
	std::vector<Vector4> vertices;
	vertices.reserve(text.size() * 6);
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        vertices.push_back(Vector4( xpos,     ypos + h,   0.0, 0.0 ));            
		vertices.push_back(Vector4( xpos,     ypos,       0.0, 1.0 ));
		vertices.push_back(Vector4(xpos + w, ypos,       1.0, 1.0 ));
		vertices.push_back(Vector4( xpos,     ypos + h,   0.0, 0.0 ));
		vertices.push_back(Vector4( xpos + w, ypos,       1.0, 1.0 ));
		vertices.push_back(Vector4( xpos + w, ypos + h,   1.0, 0.0 ));           
			// Render glyph texture over quad
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector4) * vertices.size(), &vertices[0]); // Be sure to use glBufferSubData and not glBufferData
			// Render quad
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		shader->setUniform("text",28);
		glActiveTexture(GL_TEXTURE28);
		int offset = 0;
		glDisable(GL_CULL_FACE);
		for (c = text.begin(); c != text.end(); c++) 
		{
			
			glBindTexture(GL_TEXTURE_2D, Characters[*c].TextureID);
			// Update content of VBO memory
			glDrawArrays(GL_TRIANGLES, offset * 6, 6);
			offset++;
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_CULL_FACE);
}
void Text::update()
{
	for(unsigned int i = 0; i < data.size();i++)
	{
		if(data[i].timetolive > 0)
		{
			renderText(data[i].text,data[i].x,data[i].y,data[i].sx,data[i].sy,data[i].color);
			data[i].timetolive -= Time::delta/1000;
		}
		else
		{
			data.erase(data.begin() + i);
			i--;
		}
	}
}
void Text::addTimedText(std::string text, GLfloat x, GLfloat y,  GLfloat SX, GLfloat SY, Vector3 color,GLfloat time)
{
	data.push_back(TextData(text,x,y,SX,SY,color,time));
}