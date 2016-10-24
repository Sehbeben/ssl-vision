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
  \file    gpu_plugin_shadowremoval.cpp
  \brief   C++ Implementation: plugin_visualize
  \author  Florian Lehmann, 2016
*/
//========================================================================

#include <iostream>
#include <timer.h>
//#include <chrono>

#include "gpu_plugin_shadowremoval.h"
#include "GPUSupport.h"

PluginShadowRemoval::PluginShadowRemoval(FrameBuffer* fb) : VisionPlugin(fb), counter(0){
//TODO
/* Intitialize GPU functions
 * set up 3D world with rectangle and orthogonal camera
 * load and compile shaders
 * set up image texture
 * 
 * 
 * 
 * 
 * 
 */
 gpu = new GPUSupport();
 //glClearColor(1.0,1.0,1.0,1.0); 
};

ProcessResult PluginShadowRemoval::process(FrameData* data, RenderOptions* options)
{
  gpu->render(data);
  return ProcessingOk;
}

void PluginShadowRemoval::postProcess(FrameData * data, RenderOptions * options)
{
  
  //gpu->render(data);
}

VarList * PluginShadowRemoval::getSettings() {
  return 0;
}

string PluginShadowRemoval::getName() {
  return "TestPlugin";
}