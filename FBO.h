#pragma once
#include "Math/3DMath.h"
#include <glew.h>
 
 
#include <GL/GLu.h>
#include <iostream>
#include <assert.h>
#include "errors.h"

struct FBO 
{
	Vector2 size;
	GLuint fbo;
    GLuint texture;
    GLuint depth;
 
    virtual ~FBO()
	{
        release();
    }
 
    void allocate() 
	{
        release();
        glGenRenderbuffers(1, &depth);
        assert(depth);
        glGenTextures(1, &texture);
        assert(texture);
        glGenFramebuffers(1, &fbo);
        assert(fbo);
    }
 
    void release() 
	{
        if (fbo)
		{
            glDeleteFramebuffers(1, &fbo);
            fbo = 0;
        }
        if (texture) 
		{
            glDeleteTextures(1, &texture);
            texture = 0;
        }
        if (depth) 
		{
            glDeleteRenderbuffers(1, &depth);
            depth = 0;
        }
    }
 
    static bool checkStatus(GLenum target = GL_FRAMEBUFFER) 
	{
        GLuint status = glCheckFramebufferStatus(target);
        switch (status) 
		{
        case GL_FRAMEBUFFER_COMPLETE:
            return true;
            break;
 
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cerr << "framebuffer incomplete attachment" << std::endl;
            break;
 
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cerr << "framebuffer missing attachment" << std::endl;
            break;
 
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cerr << "framebuffer incomplete draw buffer" << std::endl;
            break;
 
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cerr << "framebuffer incomplete read buffer" << std::endl;
            break;
 
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            std::cerr << "framebuffer incomplete multisample" << std::endl;
            break;
 
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            std::cerr << "framebuffer incomplete layer targets" << std::endl;
            break;
 
        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cerr << "framebuffer unsupported internal format or image" << std::endl;
            break;
 
        default:
            std::cerr << "other framebuffer error" << std::endl;
            break;
        }
 
        return false;
    }
 
    void init(const Vector2& Size)
	{
        size = Size;
        allocate();
 
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
 
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, (int)size.x,(int) size.y);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
 
        glBindRenderbuffer(GL_RENDERBUFFER, depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, (int)size.x, (int)size.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
 
        if (!checkStatus())
		{
           fatalError("Could not create a valid framebuffer");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
 
    void activate() 
	{
        assert(fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, (int)size.x, (int)size.y);
    }
};
