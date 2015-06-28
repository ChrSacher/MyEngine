#version 330 core
//The fragment shader operates on each pixel in a given polygon


const int MAXPOINTLIGHTS = 4;
const int MAXSPOTLIGHTS = 4;
struct BaseLight
{
	vec3 color;
	float intensity;
};
struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};
struct Attenuation
{
	float constant;
	float linear;
	float exponent;
};
struct PointLight
{
	BaseLight base;
	Attenuation atten;
	vec3 pos;
	float range;
};
struct SpotLight
{
	PointLight pointLight;
	vec3 dir;
	float cutoff;
};
struct Fog
{
   vec4 color;
   float start;
   float end;
   float density;
   int type;
};

varying vec2 uv0;
varying vec3 normal0;
varying vec4 worldPos0;
varying vec4 viewworldPos0;

uniform sampler2D Texture;
uniform vec3 baseColor;
uniform vec3 ambientLight;
uniform DirectionalLight directionalLight;
uniform float specularIntensity;
uniform float specularPower;
uniform vec3 eyePos;
uniform PointLight pointLights[MAXPOINTLIGHTS];
uniform SpotLight spotLights[MAXSPOTLIGHTS];
uniform Fog fog;

vec4 calcLight(BaseLight base,vec3 direction,vec3 normal)
{
	float diffuseFactor = dot(normal,-direction);
	vec4 diffuseColor = vec4(0,0,0,0);
	vec4 specularColor = vec4(0,0,0,0);
	if(diffuseFactor >0)
	{

		diffuseColor = vec4(base.color,1) * base.intensity * diffuseFactor;

		vec3 directionToEye = normalize(eyePos - worldPos0.xyz);
		vec3 reflectionDirection = normalize(reflect(direction,normal));

		float specularFactor = dot(directionToEye,reflectionDirection);
		specularFactor = pow(specularFactor,specularPower);

		if(specularFactor > 0 )
		{
			specularColor = vec4(base.color,1) * specularIntensity * specularFactor * base.intensity;
		}
	}
	return diffuseColor + specularColor;
}

vec4 calcDirectionalLight(DirectionalLight light,vec3 normal)
{
	return calcLight(light.base,-light.direction,normal);
}

vec4 calcPointLight(PointLight light,vec3 normal)
{
	vec3 lightDirection = worldPos0.xyz - light.pos;
	float distanceToPoint = length(lightDirection);
	if(distanceToPoint > light.range )
	{return vec4(0,0,0,0);}
	lightDirection = normalize(lightDirection);
	vec4 color = calcLight(light.base,lightDirection,normal);
	float attenuation = light.atten.constant + 
					   light.atten.linear * distanceToPoint +
					   light.atten.exponent * distanceToPoint + 0.0001f;
	return color/attenuation;
}

vec4 calcSpotLight(SpotLight spotLight,vec3 normal)
{
	vec3 lightDirection = normalize(worldPos0.xyz - spotLight.pointLight.pos);
	float spotFactor = dot(lightDirection , spotLight.dir);
	vec4 color=vec4(0,0,0,0);
	if(spotFactor > spotLight.cutoff)
	{	
		color = calcPointLight(spotLight.pointLight,normal) * (1.0 - (1.0 - spotFactor)/(1.0 - spotLight.cutoff));
	}

	return color;
}

float getFogFactor(Fog params, float dist) 
{ 
   	float fResult = 0.0;
   	if(params.type == 1 && (params.end != params.start))
   	{
		fResult = (params.end - dist)/(params.end - params.start);
		fResult =1.0 - clamp( fResult, 0.0, 1.0 );
		return fResult;	   
	}
	if(params.type == 2)
	{
		fResult = exp(-params.density*dist);       
		fResult = 1.0 - clamp(fResult, 0.0, 1.0);     
		return fResult; 
	}
	return fResult;
	
}

void main()
{
	vec4 totalLight = vec4(ambientLight,1);
	vec4 color = vec4(baseColor,1);
	vec4 textureColor = vec4(0,0,0,0);
	textureColor = texture2D(Texture,uv0);
	if(textureColor.a < 0.9) discard; //if texel has alpha below don't draw to z-buffer so things behind are still visible
	color*= textureColor;
	vec3 normal = normalize(normal0);
	totalLight += calcDirectionalLight(directionalLight,normal);
	for(int i = 0;i < MAXPOINTLIGHTS;i++)
	{
	if(pointLights[i].base.intensity > 0)
		totalLight += calcPointLight(pointLights[i],normal);
	}
	for(int i = 0;i < MAXSPOTLIGHTS;i++)
	{
	if(spotLights[i].pointLight.base.intensity > 0)
		totalLight += calcSpotLight(spotLights[i],normal);
	}
	gl_FragColor = color * totalLight;
	
	if(fog.type > 0.1)
	{
		float dist = length(viewworldPos0);
		gl_FragColor = mix(gl_FragColor, fog.color,getFogFactor(fog, dist) ); 
	}
	
}