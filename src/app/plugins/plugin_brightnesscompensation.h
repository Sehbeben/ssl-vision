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
  \file    plugin_brightnesscompensation.h
  \brief   C++ Interface: plugin_brightnesscompensation
  \author  Florian Lehmann, 2016
*/
//========================================================================
#ifndef PLUGIN_BRIGHTNESSCOMPENSATION_H
#define PLUGIN_BRIGHTNESSCOMPENSATION_H



#include <visionplugin.h>

#include <QWidget>

class PluginBrightnessCompensation;

class PluginBrightnessCompensationWidget : public QWidget
{
public:
    PluginBrightnessCompensationWidget(PluginBrightnessCompensation * pbc, QWidget * parent = 0, Qt::WindowFlags f = 0);

    QVBoxLayout * layout_main;
private:

};


class PluginBrightnessCompensation : public VisionPlugin {
public:
    PluginBrightnessCompensation(FrameBuffer* fb);

    virtual ProcessResult process(FrameData * data, RenderOptions * options);

    //virtual VarList * getSettings();

    virtual string getName();

    virtual QWidget * getControlWidget();
protected:
    PluginBrightnessCompensationWidget * widget;

};


#endif
