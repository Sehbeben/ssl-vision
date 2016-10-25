#version 330 core						  
//in vec3 fragmentColor;					  
in vec2 UV;							  
out vec3 color;						  
								  
uniform sampler2D textureSampler;				  
uniform sampler2D mapSampler;				  
uniform int imagewidth;					  
uniform int imageheight;					  
								  
								  
void main(void)						  
{								  
  vec2 mapPosition = gl_FragCoord.xy;														
  mapPosition.x = mapPosition.x / float(imagewidth);											
  mapPosition.y = mapPosition.y / float(imageheight);											
																							
  float a = texture2D(mapSampler, mapPosition).x;										
  float b = texture2D(mapSampler, mapPosition).y;										
  float c = texture2D(mapSampler, mapPosition).z;              
	//color = vec3(0.1f,0.5f,0.0f);				  
								  
	color = texture2D(textureSampler, UV).rgb;		  
	//color = vec3(1.0f,0.0f,0.5f);							  
	if ((gl_FragCoord.x < 10)||(gl_FragCoord.x > imagewidth-10)) color = vec3(0.9f,0.0f,0.0f);							  
}