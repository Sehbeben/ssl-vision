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
#include <image.h>
#include "plugin_brightnesscompensation.h"

PluginBrightnessCompensationWidget::PluginBrightnessCompensationWidget(PluginBrightnessCompensation *pbc,
                                                                       QWidget *parent, Qt::WindowFlags f) {
    layout_main = new QHBoxLayout();
    leftPart = new QVBoxLayout();
    rightPart = new QVBoxLayout();

    calibrationButton = new QPushButton("Start Calibration");


    leftPart->addWidget(calibrationButton);
    list = new QListWidget();
    list->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    list->setFocusPolicy(Qt::NoFocus);


    QListWidgetItem* item;
    item = new QListWidgetItem("Blue");
    QPixmap p(20,20);
    p.fill(QColor(0,0,255,255));
    QPainter painter(&p);
    painter.setPen(QPen(QColor(0, 0, 0), 1));
    painter.drawRect(0,0,19,19);
    item->setIcon(p);
    list->addItem(item);


    item = new QListWidgetItem("Yellow");
    p.fill(QColor(255,255,0,255));
    painter.setPen(QPen(QColor(0, 0, 0), 1));
    painter.drawRect(0,0,19,19);
    item->setIcon(p);
    list->addItem(item);

    item = new QListWidgetItem("Field");
    p.fill(QColor(0,80,0,255));
    painter.setPen(QPen(QColor(0, 0, 0), 1));
    painter.drawRect(0,0,19,19);
    item->setIcon(p);
    list->addItem(item);

    item = new QListWidgetItem("Pink");
    p.fill(QColor(255,0,255,255));
    painter.setPen(QPen(QColor(0, 0, 0), 1));
    painter.drawRect(0,0,19,19);
    item->setIcon(p);
    list->addItem(item);

    item = new QListWidgetItem("Green");
    p.fill(QColor(0,255,0,255));
    painter.setPen(QPen(QColor(0, 0, 0), 1));
    painter.drawRect(0,0,19,19);
    item->setIcon(p);
    list->addItem(item);

    item = new QListWidgetItem("Orange");
    p.fill(QColor(255,128,0,255));
    painter.setPen(QPen(QColor(0, 0, 0), 1));
    painter.drawRect(0,0,19,19);
    item->setIcon(p);
    list->addItem(item);


    leftPart->addWidget(list);


    rgbColorLabel = new QLabel("RGB:");
    yuvColorLabel = new QLabel("YUV:");
    checkBox = new QCheckBox("Check");
    checkBox->setCheckState(Qt::Unchecked);

    rightPart->addWidget(rgbColorLabel);
    rightPart->addWidget(yuvColorLabel);
    rightPart->addWidget(checkBox);
    layout_main->addLayout(leftPart);
    layout_main->addLayout(rightPart);
    this->setLayout(layout_main);
}



PluginBrightnessCompensation::PluginBrightnessCompensation(FrameBuffer* fb) :  VisionPlugin(fb){
    widget = new PluginBrightnessCompensationWidget(this);
}

string PluginBrightnessCompensation::getName() {
        return "brightness compensation";
}

ProcessResult PluginBrightnessCompensation::process(FrameData *data, RenderOptions *options) {

    if (widget->checkBox->isChecked()){
    for(int i = 0; i < data->video.getWidth(); i++)
    {
        for(int j=0; j < data->video.getHeight(); j++)
        {
            //data->video.getData()[i,j]; //= rgb(i, j, 128);
            rgbImage img(data->video);
            yuv color=Conversions::rgb2yuv(img.getPixel(i,j));
            color.y = averageColor.y;

            img.setPixel(i,j, Conversions::yuv2rgb(color));
        }
    }
    } 
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

            QString text = QString("YUV: %1, %2, %3").arg(avgColor.u).arg(avgColor.v).arg(avgColor.y);
            widget->yuvColorLabel->setText(text);

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
    averageColor = yuv(0.0, 0.0, 0.0);
    FrameBuffer * rb= getFrameBuffer();
    if (rb!=0) {
        rb->lockRead();
        int idx=rb->curRead();
        FrameData * frame = rb->getPointer(idx);

        int maxX = (firstPos.x > secondPos.x) ? firstPos.x : secondPos.x;
        int minX = (firstPos.x < secondPos.x) ? firstPos.x : secondPos.x;
        int maxY = (firstPos.y > secondPos.y) ? firstPos.y : secondPos.y;
        int minY = (firstPos.y < secondPos.y) ? firstPos.y : secondPos.y;
        int width = 1+maxX-minX;
        int height = 1+maxY-minY;
        float size = width*height;

        float u=0;
        float v=0;
        float y=0;
        for (int i= minX; i < minX+width; i++)
        {
            for(int j= minY; j < minY+height; j++)
            {
                rgbImage img(frame->video);
                yuv color=Conversions::rgb2yuv(img.getPixel(i,j));
                u += color.u/size;
                v += color.v/size;
                y += color.y/size;
            }
        }
        averageColor = yuv((unsigned char) u, (unsigned char) v, (unsigned char) y);


        // for(int i = 0; i< )
        if (firstPos.x < frame->video.getWidth() && firstPos.y < frame->video.getHeight() && firstPos.x >=0 && firstPos.y >=0) {
            if (frame->video.getWidth() > 1 && frame->video.getHeight() > 1) {
//                        //if converting entire image then blanking is not needed
                ColorFormat source_format=frame->video.getColorFormat();
                if (source_format==COLOR_RGB8) {
//                            //plain copy of data
                    rgbImage img(frame->video);
                    //color=Conversions::rgb2yuv(img.getPixel(loc.x,loc.y));
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
                        }
            }
        }
        rb->unlockRead();
    }
    return averageColor;
}