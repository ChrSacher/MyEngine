#version 330                                                                        
                                                                                    
attribute vec3 position;                                             
                                                                                    
uniform mat4 MVP;                                                                  
                                                                                    
varying vec3 TexCoord0;                                                                 
                                                                                    
void main()                                                                         
{                                                                                   
     gl_Position = MVP * vec4(position, 1.0);                                                                                          
    TexCoord0   = position;                                                         
}