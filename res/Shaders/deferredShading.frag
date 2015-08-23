#version 330


vec3 CalcBumpedNormal()
{
    vec3 normal = normalize(normal0);
    vec3 tangent = normalize(tangent0);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 Bitangent = cross(tangent, normal);
    vec3 BumpMapNormal = texture2D(normalMap, uv0).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(tangent, Bitangent, normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}

in vec2 uv0; 
in vec3 normal0; 
in vec3 worldPos0; 
in vec3 tangent0;

layout (location = 0) out vec3 WorldPosOut; 
layout (location = 1) out vec3 DiffuseOut; 
layout (location = 2) out vec3 NormalOut; 
layout (location = 3) out vec3 TexCoordOut; 

uniform sampler2D Texture;
uniform sampler2D normalMap;

void main() 
{ 
    WorldPosOut = worldPos0; 
    DiffuseOut = texture(Texture, uv0).xyz; 
    NormalOut = CalcBumpedNormal(); 
    TexCoordOut = vec3(uv0, 0.0); 
}
