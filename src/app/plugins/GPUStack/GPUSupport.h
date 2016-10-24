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
  \file    $FILENAME$.h
  \brief   C++ Interface: $MODULE$
  \author  $AUTHOR$, $YEAR$
*/
//========================================================================

#include <GL/glew.h>
#include <GL/glxew.h>
#include <framedata.h>


GLuint LoadShader();
GLuint LinkProgram(GLuint, GLuint);


class GPUSupport {
public:
  GPUSupport();
  ~GPUSupport();
  void render(FrameData *data);
  
private:
    Display *dpy;
  Window root;
    GLXContext glc;
  
   GLuint mapTexture, imageTexture, fb, targetTexture;
  
  GLuint mappingshader;
  
  int currentWidth, currentHeight;


  
  void initializeGPUFunction(int width, int height);
  
  
  
  
  
};