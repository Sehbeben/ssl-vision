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
  \file    plugin_brightnesscompensation.cpp
  \brief   C++ Implementation: plugin_brightnesscompensation
  \author  Florian Lehmann, 2016
*/
//========================================================================


#include "plugin_brightnesscompensation.h"

PluginBrightnessCompensation::PluginBrightnessCompensation(FrameBuffer* fb) :  VisionPlugin(fb){

}

string PluginBrightnessCompensation::getName() {
        return "brightness compensation";
}

ProcessResult PluginBrightnessCompensation::process(FrameData *data, RenderOptions *options) {
    return ProcessingOk;
}
