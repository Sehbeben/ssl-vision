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


#include <iostream>
#include "plugin_brightnesscompensation.h"

PluginBrightnessCompensationWidget::PluginBrightnessCompensationWidget(PluginBrightnessCompensation *pbc,
                                                                       QWidget *parent, Qt::WindowFlags f) {
    layout_main = new QVBoxLayout();
    rgbColorLabel = new QLabel("RGB:");
    yuvColorLabel = new QLabel("YUV:");


    layout_main->addWidget(rgbColorLabel);
    layout_main->addWidget(yuvColorLabel);
    this->setLayout(layout_main);
}



PluginBrightnessCompensation::PluginBrightnessCompensation(FrameBuffer* fb) :  VisionPlugin(fb){
    widget = new PluginBrightnessCompensationWidget(this);
}

string PluginBrightnessCompensation::getName() {
        return "brightness compensation";
}

ProcessResult PluginBrightnessCompensation::process(FrameData *data, RenderOptions *options) {
    return ProcessingOk;
}

QWidget *PluginBrightnessCompensation::getControlWidget() {
    return widget;
}

void PluginBrightnessCompensation::mousePressEvent(QMouseEvent *event, pixelloc loc) {
    QTabWidget* tabw = (QTabWidget*) widget->parentWidget()->parentWidget();
    if (tabw->currentWidget() == widget) {
        if (event->buttons()==Qt::LeftButton) {
            firstPos = loc;
            std::cout<<"firstPos"<<"\nX: "<< firstPos.x<<"\nY: "<< firstPos.y <<std::endl;
            event->accept();
        }

    }
    else
        event->ignore();
}

void PluginBrightnessCompensation::mouseReleaseEvent(QMouseEvent *event, pixelloc loc) {
    QTabWidget* tabw = (QTabWidget*) widget->parentWidget()->parentWidget();
    if (tabw->currentWidget() == widget) {
        if (event->button()==Qt::LeftButton) {
            secondPos = loc;
            std::cout<<"secondPos:"<<"\nX: "<< secondPos.x<<"\nY: "<< secondPos.y <<std::endl;
            yuv avgColor = calculateAverageLightning(firstPos, secondPos);
            event->accept();
        }

    }
    else
        event->ignore();
}

void PluginBrightnessCompensation::mouseEvent(QMouseEvent *event, pixelloc loc) {
    QTabWidget* tabw = (QTabWidget*) widget->parentWidget()->parentWidget();
    if (tabw->currentWidget() == widget) {
        std::cout<<"Yaaaaaaayyyyy"<<std::endl;
        if (event->buttons()==Qt::LeftButton) {
//            FrameBuffer * rb=getFrameBuffer();
//            if (rb!=0) {
//                rb->lockRead();
//                int idx=rb->curRead();
//                FrameData * frame = rb->getPointer(idx);
//                if (loc.x < frame->video.getWidth() && loc.y < frame->video.getHeight() && loc.x >=0 && loc.y >=0) {
//                    if (frame->video.getWidth() > 1 && frame->video.getHeight() > 1) {
//                        yuv color;
//                        //if converting entire image then blanking is not needed
//                        ColorFormat source_format=frame->video.getColorFormat();
//                        if (source_format==COLOR_RGB8) {
//                            //plain copy of data
//                            rgbImage img(frame->video);
//                            color=Conversions::rgb2yuv(img.getPixel(loc.x,loc.y));
//                        } else if (source_format==COLOR_YUV444) {
//                            yuvImage img(frame->video);
//                            color=img.getPixel(loc.x,loc.y);
//                        } else if (source_format==COLOR_YUV422_UYVY) {
//                            uyvy color2 = *((uyvy*)(frame->video.getData() + (sizeof(uyvy) * (((loc.y * (frame->video.getWidth())) + loc.x) / 2))));
//                            color.u=color2.u;
//                            color.v=color2.v;
//                            if ((loc.x % 2)==0) {
//                                color.y=color2.y1;
//                            } else {
//                                color.y=color2.y2;
//                            }
//                        } else {
//                            //blank it:
//                            fprintf(stderr,"Unable to pick color from frame of format: %s\n",Colors::colorFormatToString(source_format).c_str());
//                            fprintf(stderr,"Currently supported are rgb8, yuv444, and yuv422 (UYVY).\n");
//                            fprintf(stderr,"(Feel free to add more conversions to plugin_colorcalib.cpp).\n");
//                        }
//                        lutw->samplePixel(color);
//                        //img.setPixel(loc.x,loc.y,rgb(255,0,0));
//
//                        if (event->modifiers()!=Qt::NoModifier) {
//                            if(event->modifiers() & Qt::ShiftModifier) {          //modified 2/1/16, 'control' alone consumed
//                                if(event->modifiers() & Qt::ControlModifier)        // by QT framework to move a window
//                                    lutw->add_del_Pixel(color, true, continuing_undo);
//                                else
//                                    lutw->add_del_Pixel(color, false, continuing_undo);
//                            }
//                            continuing_undo = true;
//                        }
//                    }
//                }
//                rb->unlockRead();
//            }
            event->accept();
        }

    }
    else
        event->ignore();
}

yuv PluginBrightnessCompensation::calculateAverageLightning(pixelloc firstPos, pixelloc secondPos)
{
    FrameBuffer * rb= getFrameBuffer();
    if (rb!=0) {
        rb->lockRead();
        int idx=rb->curRead();
        FrameData * frame = rb->getPointer(idx);
        if (firstPos.x < frame->video.getWidth() && firstPos.y < frame->video.getHeight() && firstPos.x >=0 && firstPos.y >=0) {
            if (frame->video.getWidth() > 1 && frame->video.getHeight() > 1) {
               yuv color;
//                        //if converting entire image then blanking is not needed
//                        ColorFormat source_format=frame->video.getColorFormat();
//                        if (source_format==COLOR_RGB8) {
//                            //plain copy of data
//                            rgbImage img(frame->video);
//                            color=Conversions::rgb2yuv(img.getPixel(loc.x,loc.y));
//                        } else if (source_format==COLOR_YUV444) {
//                            yuvImage img(frame->video);
//                            color=img.getPixel(loc.x,loc.y);
//                        } else if (source_format==COLOR_YUV422_UYVY) {
//                            uyvy color2 = *((uyvy*)(frame->video.getData() + (sizeof(uyvy) * (((loc.y * (frame->video.getWidth())) + loc.x) / 2))));
//                            color.u=color2.u;
//                            color.v=color2.v;
//                            if ((loc.x % 2)==0) {
//                                color.y=color2.y1;
//                            } else {
//                                color.y=color2.y2;
//                            }
//                        } else {
//                            //blank it:
//                            fprintf(stderr,"Unable to pick color from frame of format: %s\n",Colors::colorFormatToString(source_format).c_str());
//                            fprintf(stderr,"Currently supported are rgb8, yuv444, and yuv422 (UYVY).\n");
//                            fprintf(stderr,"(Feel free to add more conversions to plugin_colorcalib.cpp).\n");
//                        }
//                        lutw->samplePixel(color);
//                        //img.setPixel(loc.x,loc.y,rgb(255,0,0));
//
//                        if (event->modifiers()!=Qt::NoModifier) {
//                            if(event->modifiers() & Qt::ShiftModifier) {          //modified 2/1/16, 'control' alone consumed
//                                if(event->modifiers() & Qt::ControlModifier)        // by QT framework to move a window
//                                    lutw->add_del_Pixel(color, true, continuing_undo);
//                                else
//                                    lutw->add_del_Pixel(color, false, continuing_undo);
//                            }
//                            continuing_undo = true;
//                        }
            }
        }
        rb->unlockRead();
    }

}