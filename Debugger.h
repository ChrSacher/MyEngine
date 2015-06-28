#pragma once

#include <string>
#include <iostream>
#include <glew.h>
#include <GL/GL.h>
#include <GL/GLu.h>
#include <assert.h>
#include <Windows.h>
namespace Core
{
 
 class DebugOutput
 {
  public:
    DebugOutput(){};
   ~DebugOutput(){};
    //if you want you can rename myCallback; This is just an example
   static void APIENTRY myCallback(GLenum source,
                                   GLenum type,
                                   GLuint id,
                                   GLenum severity,
                                   GLsizei length,
                                   const GLchar *msg,
                                   const void *data)
   {
      //display warnings/errors however you like
     std::cout << "\n**********Debug Output**************"                 <<  std::endl;
     std::cout << "source: "     << getStringForSource(source).c_str()     <<  std::endl;
      std::cout << "type: "       << getStringForType(type).c_str()         <<  std::endl;
      std::cout << "severity: "   << getStringForSeverity(severity).c_str() <<  std::endl;
      std::cout << "debug call: " << msg                                    <<  std::endl;
      std::cout << "\n************************" << std::endl;
   }
 
  private:
     //Parsing code from OpenGL Shader Language CookBook SE
     //https://github.com/daw42/glslcookbook
   static std::string getStringForType(GLenum type)
   {
     switch (type)
     {
        case GL_DEBUG_TYPE_ERROR:
              return"Error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
              return "Deprecated behavior";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
              return "Undefined behavior";
        case GL_DEBUG_TYPE_PORTABILITY:
              return "Portability issue";
        case GL_DEBUG_TYPE_PERFORMANCE:
              return "Performance issue";
        case GL_DEBUG_TYPE_MARKER:
              return "Stream annotation";
        case GL_DEBUG_TYPE_OTHER_ARB:
              return "Other";
        default:
              assert(false);
              return "";
     }
   }
 
   static std::string getStringForSource(GLenum source)
   {
     switch (source)
     {
       case GL_DEBUG_SOURCE_API:
            return "API";
       case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "Window system";
       case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "Shader compiler";
       case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "Third party";
       case GL_DEBUG_SOURCE_APPLICATION:
            return "Application";
       case GL_DEBUG_SOURCE_OTHER:
            return "Other";
       default:
            assert(false);
            return "";
     }
   }
 
   static std::string getStringForSeverity(GLenum severity)
   {
     switch (severity)
     {
      case GL_DEBUG_SEVERITY_HIGH:
           return "High";
      case GL_DEBUG_SEVERITY_MEDIUM:
           return "Medium";
      case GL_DEBUG_SEVERITY_LOW:
           return "Low";
      case GL_DEBUG_SEVERITY_NOTIFICATION:
           return "Notification";
      default:
           assert(false);
           return("");
      }
   }
 
 };
}