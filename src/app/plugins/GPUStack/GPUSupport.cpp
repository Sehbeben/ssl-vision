//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    $FILENAME$.cpp
  \brief   C++ Implementation: $MODULE$
  \author  $AUTHOR$, $YEAR$
*/
//========================================================================

#include "GPUSupport.h"


#include <vector>
#include <fstream>

#include <iostream>



using namespace std;

static const char* vertexShader = {
	"#version 330 core												\n"
	"layout(location = 0) in vec3 vertexPosition_modelspace;		\n"
	"layout(location = 1) in vec2 vertexUV;							\n"
	"out vec2 UV;													\n"
	"uniform mat4 MVP;												\n"
	"																\n"
	"void main(){													\n"
	"	//gl_Position = vec4(vertexPosition_modelspace,1);			\n"
	"																\n"
	" 	gl_Position = MVP * vec4(vertexPosition_modelspace,1);		\n"
	" 	UV = vertexUV;												\n"
	"}																\n"
};

static const char* fragmentShader = {
  "#version 330 core						  \n"
  "//in vec3 fragmentColor;					  \n"
  "in vec2 UV;							  \n"
  "out vec3 color;						  \n"
  "								  \n"
  "uniform sampler2D textureSampler;				  \n"
  "uniform sampler2D mapSampler;				  \n"
  "uniform int imagewidth;					  \n"
  "uniform int imageheight;					  \n"
  "								  \n"
  "								  \n"
  "void main(void)						  \n"
  "{								  \n"
  "  vec2 mapPosition = gl_FragCoord.xy;														\n"
  "  mapPosition.x = mapPosition.x / float(imagewidth);											\n"
  "  mapPosition.y = mapPosition.y / float(imageheight);											\n"
  "																							\n"
  "  float a = texture2D(mapSampler, mapPosition).x;										\n"
  "  float b = texture2D(mapSampler, mapPosition).y;										\n"
  "  float c = texture2D(mapSampler, mapPosition).z;              \n"
  "	//color = vec3(0.1f,0.5f,0.0f);				  \n"
  "								  \n"
  "	color = texture2D(textureSampler, UV).rgb;		  \n"
  "	//color = vec3(1.0f,0.0f,0.5f);							  \n"
  "	if ((gl_FragCoord.x < 10)||(gl_FragCoord.x > imagewidth-10)) color = vec3(0.9f,0.0f,0.0f);							  \n"
  
  "								  \n"
  "}								  \n"
};

GLuint LoadShader() {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	string vertex_shader_source = vertexShader;
	char const* sourcePointer = vertex_shader_source.c_str();
	glShaderSource(vertexShaderID, 1, &sourcePointer, NULL);
	glCompileShader(vertexShaderID);
	
	GLint status;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(vertexShaderID, 512, NULL, buffer);
		std::cout << buffer << std::endl;
	}
	else cout << "Vertexshader successfully compiled!\n";

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	string fragment_shader_source = fragmentShader;
	sourcePointer = fragment_shader_source.c_str();
	glShaderSource(fragmentShaderID, 1, &sourcePointer, NULL);
	glCompileShader(fragmentShaderID);
	
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE){
		char buffer[512];
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, buffer);
		cout << buffer << endl;
	}
	else cout << "Fragmentshader successfully compiled!\n";
	

	GLuint programID = LinkProgram(vertexShaderID, fragmentShaderID);

	return programID;
}


GLuint LinkProgram(GLuint vertexShaderID, GLuint fragmentShaderID)
{
	int infoLogLength;
	GLint result = GL_FALSE;
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	//Check program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength>0)
	{
		std::vector<char> ProgramErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
		//std::cout<<"Fehler beim Linken aufgetreten\n"<<"Log-Laenge: "<<infoLogLength;
	}
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

GPUSupport::GPUSupport() : currentWidth(0), currentHeight(0)
{
  
       

  GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
  XVisualInfo *vi;

  
   dpy = XOpenDisplay(0);
  if(!dpy) {
 	//printf("\n\tcannot connect to X server\n\n");
    }
  root = DefaultRootWindow(dpy);
  vi = glXChooseVisual(dpy, 0, att);
  
  glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
  glXMakeCurrent(dpy, root, glc); 
  
  //intitialize GLEW and Context
  //Initialize glew
  glewInit();
  glClearColor(0.0, 0.0, 0.0, 0.0);
  //std::cout<<glGetString(GL_VERSION);

//set storage alignment in memory
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


  glGenTextures(1, &mapTexture);
  glGenTextures(1, &imageTexture);				//input texture

  

  //os::SMatrix<float, 4, 4> mvp = ortho_projection * view;

  //define Quad
  static const GLfloat g_vertex_buffer_data[] = {
	  -1.0f, -1.0f, -1.0f,
	  -1.0f,  1.0f, -1.0f,
	  1.0f, -1.0f, -1.0f,
	  1.0f,  1.0f, -1.0f,
	  1.0f, -1.0f, -1.0f,
	  -1.0f,  1.0f, -1.0f
  };

  static const GLfloat g_uv_buffer_data[] = {
	  0.0f, 1.0f,
	  0.0f, 0.0f,
	  1.0f, 1.0f,
	  1.0f, 0.0f,
	  1.0f, 1.0f,
	  0.0f, 0.0f
  };

  float mvp[16] = { 1.0f, 0, 0, 0,
		    0,-1, 0, 0,
		    0, 0,-1, 0,
		    0, 0,-2, 1};
  //create quad
  GLuint vertexBuffer, uvBuffer;
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
  glGenBuffers(1, &uvBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  //create shader program
  mappingshader = LoadShader();
  glUseProgram(mappingshader);
  GLuint matrixID = glGetUniformLocation(mappingshader, "MVP");
  glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0]);  
  
 // glBindTexture(GL_TEXTURE_2D, targetTexture);
 // glGenTextures(1, &targetTexture);
}

GPUSupport::~GPUSupport()
{

}

void GPUSupport::initializeGPUFunction(int width, int height)
{
  std::cout<<"Test\n";
  
  glXMakeCurrent(dpy, root, glc); 
    glewInit();
	glGenTextures(1, &targetTexture);
  	//RGBA8 2D texture, 24 bit depth texture, 256x256
	glBindTexture(GL_TEXTURE_2D, targetTexture);
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//NULL means reserve texture memory, but texels are undefined
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	//-------------------------
	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	//Attach 2D texture to this FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, targetTexture, 0);



	glViewport(0, 0, width, height);
	GLuint glWidth = glGetUniformLocation(mappingshader, "imagewidth");
	GLuint glHeight = glGetUniformLocation(mappingshader, "imageheight");
	glClearColor(0.5, 0.0, 0.0, 0.0);
	glUniform1i(glWidth, width);
	glUniform1i(glHeight, height);

	//GLuint glInterpolation = glGetUniformLocation(mappingshader, "interpolation");

	glBindTexture(GL_TEXTURE_2D, imageTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

}

void GPUSupport::render(FrameData* data)
{
  int width = data->video.getWidth();
  int height = data->video.getHeight();
  if ((currentWidth != width) || (currentHeight != height))  {
    
    initializeGPUFunction(width, height);
 
  }
  currentWidth = width;
  currentHeight= height;
  
  
  glBindTexture(GL_TEXTURE_2D, imageTexture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data->video.getData());
  
  GLuint textureID = glGetUniformLocation(mappingshader, "textureSampler");

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, imageTexture);
  glUniform1i(textureID, 0);

  glDrawArrays(GL_TRIANGLES, 0, 6);
  int position=0;
  for(int i=0; i<width; i++){
    for(int j=0; j<height; j++){
	position=(3*i+j*3*width);
	//data->video.getData()[position]=0;
	//data->video.getData()[position+1]=0;
    }
  }
    
  
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data->video.getData());
}

