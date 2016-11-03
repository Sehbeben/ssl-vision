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
  \file    gpu_plugin_colorsthreshold.cpp
  \brief   C++ Implementation: plugin_visualize1
  \author  Florian Lehmann, 2016
*/
//========================================================================

#include <iostream>

#include "GPUStack/gpu_plugin_colorthreshold.h"
#include "GPUSupport.h"

GPUPluginColorThreshold::GPUPluginColorThreshold(FrameBuffer* fb) : VisionPlugin(fb){
  gpu = new GPUSupport();
  
};

ProcessResult GPUPluginColorThreshold::process(FrameData* data, RenderOptions* options)
{
  gpu->render(data);
return ProcessingOk;
}

void GPUPluginColorThreshold::postProcess(FrameData* data, RenderOptions* options)
{
}





//#include "GPUSupport.h"

// GPU
// 
// ProcessResult GPUPluginColorThreshold::process(FrameData* data, RenderOptions* options)
// {
//   
//   
//  return ProcessingOk; 
// }
// // GPUPluginColorThreshold::GPUPluginColorThreshold(FrameBuffer* _buffer): VisionPlugin(buffer){
// //   
// //   
// // }
// 
// string GPUPluginColorThreshold::getName()
// {
//   return "GPUSegmentation";
// }



