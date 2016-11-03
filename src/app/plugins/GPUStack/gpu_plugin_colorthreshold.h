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
  \file    gpu_plugin_colorthreshold.h
  \brief   C++ Interface: plugin_visualize1
  \author  Florian Lehmann, 2016
*/
//========================================================================
#ifndef PLUGIN_GPUCOLORTHRESHOLD_H
#define PLUGIN_GPUCOLORTHRESHOLD_H

//#include "GPUStack/GPUSupport.h"
#include <visionplugin.h>

class GPUSupport;


class GPUPluginColorThreshold : public VisionPlugin {
public:
  GPUPluginColorThreshold(FrameBuffer* framebuffer);
  virtual ~GPUPluginColorThreshold(){};
  
  virtual ProcessResult process(FrameData* data, RenderOptions* options);
  virtual void postProcess(FrameData* data, RenderOptions* options);


private:
  int counter;
  GPUSupport* gpu;
};

// class GPUPluginColorThreshold : public VisionPlugin {
//   public:
//   GPUPluginColorThreshold(FrameBuffer* framebuffer);
//   ~GPUPluginColorThreshold(){};
//   string test();
// 
//   virtual void postProcess(FrameData * data, RenderOptions * options);
//   virtual ProcessResult process(FrameData* data, RenderOptions* options);
//   virtual VarList * getSettings();
//   virtual string getName();
//     
//   
//   private:
//     int counter;
//   
//   
// };

#endif
