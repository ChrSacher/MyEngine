#version 330                                                                        
                                                                                    
varying vec3 TexCoord0;                                                                  
                                                                                    
out vec4 FragColor;                                                                 
                                                                                    
uniform samplerCube gCubemapTexture;                                                
uniform vec3 baseColor;                                                                                   
void main()                                                                         
{          
	vec4 textureColor = vec4(0,0,0,0);  
	vec4 color = vec4(baseColor,1);  
	textureColor = texture(gCubemapTexture, TexCoord0);
	color *= textureColor;                                                                
    gl_FragColor = color;                              
}
