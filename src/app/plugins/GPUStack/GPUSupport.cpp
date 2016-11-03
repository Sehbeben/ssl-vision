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
#include "shaderHandler.h"


#include <vector>
#include <fstream>

#include <iostream>


using namespace std;



GPUSupport::GPUSupport() : currentWidth(0), currentHeight(0)
{
  
  createOpenGLContext();
  
  glXMakeCurrent(dpy, root, glc);
  glewInit();  
    
  //Initialize glew
  configureOpenGL();
  build3DSpace();
  setupShader();


  glGenTextures(1, &lutTexture);
  glGenTextures(1, &mapTexture);
  glGenTextures(1, &imageTexture);
  
  glGenFramebuffers(1, &fb);

  //input texture
 // glBindTexture(GL_TEXTURE_2D, targetTexture);
 // glGenTextures(1, &targetTexture);
}

GPUSupport::~GPUSupport()
{

}

void GPUSupport::createOpenGLContext()
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
}

void GPUSupport::configureOpenGL()
{
  //set storage alignment in memory
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  //std::cout<<glGetString(GL_VERSION);  
  //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}


void GPUSupport::build3DSpace()
{
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

}


void GPUSupport::setupShader()
{
  float mvp[16] = { 1.0f, 0, 0, 0,
		    0,-1, 0, 0,
		    0, 0,-1, 0,
		    0, 0,-2, 1};


  //create shader program
  mappingshader = ShaderHandler::createShaderProgram("../src/app/plugins/GPUStack/vertex.vsh", "../src/app/plugins/GPUStack/fragment.fsh");
  glUseProgram(mappingshader);
  GLuint matrixID = glGetUniformLocation(mappingshader, "MVP");
  glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0]);  
  
}






void GPUSupport::initializeGPUFunction(int width, int height)
{
  //std::cout<<"Test\n";
  
  glXMakeCurrent(dpy, root, glc); 
    //glewInit();
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

	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cout << "INCOMPLETE";
	//Attach 2D texture to this FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, targetTexture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cout << "INCOMPLETE";


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


    
  
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data->video.getData());
}

