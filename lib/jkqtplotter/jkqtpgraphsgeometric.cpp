/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "jkqtplotter/jkqtpgraphsgeometric.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>

#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;

JKQTPgeoBaseLine::JKQTPgeoBaseLine(QColor color, double lineWidth, Qt::PenStyle style, JKQtBasePlotter* parent):
    JKQTPplotObject(parent)
{
    this->color=color;
    this->lineWidth=lineWidth;
    this->style=style;
    title="";
}

JKQTPgeoBaseLine::JKQTPgeoBaseLine(QColor color, double lineWidth, Qt::PenStyle style, JKQtPlotter* parent):
    JKQTPplotObject(parent)
{
    this->color=color;
    this->lineWidth=lineWidth;
    this->style=style;
    title="";
}

void JKQTPgeoBaseLine::setAlpha(float alpha)
{
    color.setAlphaF(alpha);
}

QPen JKQTPgeoBaseLine::getPen(JKQTPEnhancedPainter& painter) {
    QPen p;
    p.setColor(color);
    p.setStyle(style);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    return p;
}

void JKQTPgeoBaseLine::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    painter.setPen(getPen(painter));
    double y=rect.top()+rect.height()/2.0;
    if (rect.width()>0) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));
    painter.restore();
}

QColor JKQTPgeoBaseLine::getKeyLabelColor() {
    return color;
}


JKQTPgeoBaseFilled::JKQTPgeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, Qt::BrushStyle fillStyle, JKQtBasePlotter* parent):
    JKQTPgeoBaseLine(color, lineWidth, style, parent)
{
    this->fillColor=fillColor;
    this->fillStyle=fillStyle;
}

JKQTPgeoBaseFilled::JKQTPgeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, Qt::BrushStyle fillStyle, JKQtPlotter* parent):
    JKQTPgeoBaseLine(color, lineWidth, style, parent)
{
    this->fillColor=fillColor;
    this->fillStyle=fillStyle;
}

JKQTPgeoBaseFilled::JKQTPgeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, JKQtPlotter* parent):
    JKQTPgeoBaseLine(color, lineWidth, style, parent)
{
    this->fillColor=fillColor;
    this->fillStyle=Qt::SolidPattern;
}

JKQTPgeoBaseFilled::JKQTPgeoBaseFilled(QColor color, QColor fillColor, double lineWidth, JKQtPlotter* parent):
    JKQTPgeoBaseLine(color, lineWidth, Qt::SolidLine, parent)
{
    this->fillColor=fillColor;
    this->fillStyle=Qt::SolidPattern;
}
JKQTPgeoBaseFilled::JKQTPgeoBaseFilled(QColor color, QColor fillColor, JKQtPlotter* parent):
    JKQTPgeoBaseLine(color, 2.0, Qt::SolidLine, parent)
{
    this->fillColor=fillColor;
    this->fillStyle=Qt::SolidPattern;
}

void JKQTPgeoBaseFilled::setAlpha(float alpha)
{
    JKQTPgeoBaseLine::setAlpha(alpha);
    fillColor.setAlphaF(alpha);
}

void JKQTPgeoBaseFilled::setAlpha(float alphaLine, float alphaFill)
{
    JKQTPgeoBaseLine::setAlpha(alphaLine);
    fillColor.setAlphaF(alphaFill);
}
QBrush JKQTPgeoBaseFilled::getBrush(JKQTPEnhancedPainter &/*painter*/) {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

void JKQTPgeoBaseFilled::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    painter.setPen(getPen(painter));
    painter.setBrush(getBrush(painter));
    painter.drawRect(rect);
    painter.restore();
}





JKQTPgeoText::JKQTPgeoText(JKQtBasePlotter* parent, double x, double y, QString text, double fontSize, QColor color):
    JKQTPplotObject(parent)
{
    this->x=x;
    this->y=y;
    this->text=text;
    this->fontSize=fontSize;
    this->color=color;
}

JKQTPgeoText::JKQTPgeoText(JKQtPlotter* parent, double x, double y, QString text, double fontSize, QColor color):
    JKQTPplotObject(parent)
{
    this->x=x;
    this->y=y;
    this->text=text;
    this->fontSize=fontSize;
    this->color=color;
}
bool JKQTPgeoText::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=maxx=x;
    smallestGreaterZero=0;
    if (x>10.0*DBL_MIN) smallestGreaterZero=x;
    return true;
}

bool JKQTPgeoText::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=maxy=y;
    smallestGreaterZero=0;
    if (y>10.0*DBL_MIN) smallestGreaterZero=y;
    return true;
}

void JKQTPgeoText::draw(JKQTPEnhancedPainter& painter) {
    painter.save();
#ifdef USE_XITS_FONTS
    parent->get_mathText()->useXITS();
#endif
    parent->get_mathText()->set_fontSize(fontSize*parent->get_fontSizeMultiplier());
    parent->get_mathText()->set_fontColor(color);
    parent->get_mathText()->parse(text);
    parent->get_mathText()->draw(painter, transformX(x), transformY(y));
    painter.restore();
}

void JKQTPgeoText::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    painter.setPen(getPen(painter));
    double y=rect.top()+rect.height()/2.0;
    if (rect.width()>0) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));
    painter.restore();
}

QColor JKQTPgeoText::getKeyLabelColor() {
    return color;
}

QPen JKQTPgeoText::getPen(JKQTPEnhancedPainter &/*painter*/) {
    QPen p;
    p.setColor(color);
    return p;
}






JKQTPgeoLine::JKQTPgeoLine(JKQtBasePlotter* parent, double x1, double y1, double x2, double y2, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPgeoBaseLine(color, lineWidth, style, parent)
{
    this->x1=x1;
    this->y1=y1;
    this->x2=x2;
    this->y2=y2;
}

JKQTPgeoLine::JKQTPgeoLine(JKQtPlotter* parent, double x1, double y1, double x2, double y2, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPgeoBaseLine(color, lineWidth, style, parent)
{
    this->x1=x1;
    this->y1=y1;
    this->x2=x2;
    this->y2=y2;
}
bool JKQTPgeoLine::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=qMin(x1, x2);
    maxx=qMax(x1, x2);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=x1; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=x2; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPgeoLine::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=qMin(y1, y2);
    maxy=qMax(y1, y2);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=y1; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=y2; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

void JKQTPgeoLine::draw(JKQTPEnhancedPainter& painter) {
    painter.save();
    painter.setPen(getPen(painter));
    QLineF l(QPointF(transformX(x1), transformY(y1)), QPointF(transformX(x2), transformY(y2)));
    if (l.length()>0) painter.drawLine(l);
    painter.restore();
}






JKQTPgeoInfiniteLine::JKQTPgeoInfiniteLine(JKQtBasePlotter* parent, double x, double y, double dx, double dy, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPgeoBaseLine(color, lineWidth, style, parent)
{
    this->x=x;
    this->y=y;
    this->dx=dx;
    this->dy=dy;
    this->two_sided=false;
}

JKQTPgeoInfiniteLine::JKQTPgeoInfiniteLine(JKQtPlotter* parent, double x, double y, double dx, double dy, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPgeoBaseLine(color, lineWidth, style, parent)
{
    this->x=x;
    this->y=y;
    this->dx=dx;
    this->dy=dy;
    this->two_sided=false;
}

bool JKQTPgeoInfiniteLine::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=x;
    maxx=x;
    smallestGreaterZero=0;
    if (x>10.0*DBL_MIN) smallestGreaterZero=x;
    return true;
}

bool JKQTPgeoInfiniteLine::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=y;
    maxy=y;
    smallestGreaterZero=0;
    if (y>10.0*DBL_MIN) smallestGreaterZero=y;
    return true;
}

void JKQTPgeoInfiniteLine::draw(JKQTPEnhancedPainter& painter) {

    double xmin=parent->get_xAxis()->getMin();
    double xmax=parent->get_xAxis()->getMax();
    double ymin=parent->get_yAxis()->getMin();
    double ymax=parent->get_yAxis()->getMax();
    QRectF bbox(QPointF(xmin, ymin), QPointF(xmax, ymax));
    bool doDraw=false;
    double x2=x, y2=y;
    double x1=x, y1=y;

    //  normalize lengh of direction
    double dl=sqrt(dx*dx+dy*dy);
    dx=dx/dl;
    dy=dy/dl;

    // first catch cases where we are parallel to one coordinate axis
    if (dy==0) {
        doDraw=((y>=ymin)&&(y<=ymax));
        x1=xmin;
        x2=xmax;
        if (!two_sided) {
            if ((dx>0)&&(x>xmin)) {
                x1=x;
            } else if ((dx<0)&&(x<xmax)) {
                x2=x;
            }
        }
    } else if (dx==0) {
        doDraw=((x>=xmin)&&(x<=xmax));
        y1=ymin;
        y2=ymax;
        if (!two_sided) {
            if ((dy>0)&&(y>ymin)) {
                y1=y;
            } else if ((dy<0)&&(y<ymax)) {
                y2=y;
            }
        }
    } else {
        // first we find intersection points of line and y=ymin, as well as y=ymax
        double tymin=(ymin-y)/dy;
        double xymin=x+tymin*dx;
        double tymax=(ymax-y)/dy;
        double xymax=x+tymax*dx;
        double t1=0, t2=0;
        if ((xymin>xmin)&&(xymin<xmax)) {
            // (xymin,ymin) is on the rectangle circumference => use it
            t1=tymin;
            if (two_sided) {
                doDraw=true;
            } else if (t1>0) {
                doDraw=true;
            } else {
                t1=0;
            }
        } else if (xymin<xmin) {
            //(xymin,ymin) is on the left, next to the rectangle, so we have to intersect with x=xmin
            t1=(xmin-x)/dx;
            if (two_sided) {
                doDraw=true;
            } else if (t1>0) {
                doDraw=true;
            } else {
                t1=0;
            }
        } else if (xymin>xmax) {
            //(xymin,ymin) is on the right, next to the rectangle, so we have to intersect with x=xmax
            t1=(xmax-x)/dx;
            if (two_sided) {
                doDraw=true;
            } else if (t1>0) {
                doDraw=true;
            } else {
                t1=0;
            }
        }

        if ((xymax>xmin)&&(xymax<xmax)) {
            // (xymax,ymin) is on the rectangle circumference => use it
            t2=tymax;
            if (two_sided) {
                doDraw=true;
            } else if (t2>0) {
                doDraw=true;
            } else {
                t2=0;
            }
        } else if (xymax<xmin) {
            //(xymax,ymax) is on the left, next to the rectangle, so we have to intersect with x=xmin
            t2=(xmin-x)/dx;
            if (two_sided) {
                doDraw=true;
            } else if (t2>0) {
                doDraw=true;
            } else {
                t2=0;
            }
        } else if (xymax>xmax) {
            //(xymax,ymax) is on the right, next to the rectangle, so we have to intersect with x=xmax
            t2=(xmax-x)/dx;
            if (two_sided) {
                doDraw=true;
            } else if (t2>0) {
                doDraw=true;
            } else {
                t2=0;
            }
        }

        x1=x+t1*dx;
        y1=y+t1*dy;
        x2=x+t2*dx;
        y2=y+t2*dy;
    }

    if (doDraw) {
        painter.save();
        painter.setPen(getPen(painter));
        QLineF l(QPointF(transformX(x1), transformY(y1)), QPointF(transformX(x2), transformY(y2)));
        if (l.length()>0) painter.drawLine(l);
        painter.restore();
    }

}




JKQTPgeoPolyLines::JKQTPgeoPolyLines(JKQtBasePlotter* parent, const QVector<QPointF>& points, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPgeoBaseLine(color, lineWidth, style, parent)
{
    this->points=points;
}
JKQTPgeoPolyLines::JKQTPgeoPolyLines(JKQtPlotter* parent, const QVector<QPointF>& points, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPgeoBaseLine(color, lineWidth, style, parent)
{
    this->points=points;
}
JKQTPgeoPolyLines::JKQTPgeoPolyLines(JKQtBasePlotter *parent, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPgeoBaseLine(color, lineWidth, style, parent)
{

}

JKQTPgeoPolyLines::JKQTPgeoPolyLines(JKQtPlotter *parent, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPgeoBaseLine(color, lineWidth, style, parent)
{

}

bool JKQTPgeoPolyLines::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=0;
    maxx=0;
    smallestGreaterZero=0;
    if (points.size()>0) {
        minx=points[0].x();
        maxx=points[0].x();
        for (int i=1; i<points.size(); i++) {
            double x=points[i].x();
            if (x>maxx) maxx=x;
            if (x<minx) minx=x;
            double xvsgz;
            xvsgz=x; SmallestGreaterZeroCompare_xvsgz();
        }
        return true;
    }
    return false;
    //qDebug()<<"getXMinMax"<<minx<<maxx;
}

bool JKQTPgeoPolyLines::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=0;
    maxy=0;
    smallestGreaterZero=0;
    if (points.size()>0) {
        miny=points[0].y();
        maxy=points[0].y();
        for (int i=1; i<points.size(); i++) {
            double y=points[i].y();
            if (y>maxy) maxy=y;
            if (y<miny) miny=y;
            double xvsgz;
            xvsgz=y; SmallestGreaterZeroCompare_xvsgz();
        }
        return true;
    }
    return false;
    //qDebug()<<"getYMinMax"<<miny<<maxy;
}

void JKQTPgeoPolyLines::draw(JKQTPEnhancedPainter& painter) {
    QPainterPath path=transformToLinePath(points);
    painter.save();
    painter.setPen(getPen(painter));
    painter.drawPath(path);
    painter.restore();
}


JKQTPgeoRectangle::JKQTPgeoRectangle(JKQtBasePlotter* parent, double x, double y, double width, double height, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->x=x;
    this->y=y;
    angle=0;
    this->width=width;
    this->height=height;
}

JKQTPgeoRectangle::JKQTPgeoRectangle(JKQtPlotter* parent, double x, double y, double width, double height, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->x=x;
    this->y=y;
    angle=0;
    this->width=width;
    this->height=height;
}

JKQTPgeoRectangle::JKQTPgeoRectangle(JKQtBasePlotter *parent, double x, double y, double width, double height, double angle, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->x=x;
    this->y=y;
    this->angle=angle;
    this->width=width;
    this->height=height;
}

JKQTPgeoRectangle::JKQTPgeoRectangle(JKQtPlotter *parent, double x, double y, double width, double height, double angle, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->x=x;
    this->y=y;
    this->angle=angle;
    this->width=width;
    this->height=height;
}

JKQTPgeoRectangle::JKQTPgeoRectangle(JKQtBasePlotter *parent, QPointF bottomleft, QPointF topright, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->angle=0;
    this->width=fabs(topright.x()-bottomleft.x());
    this->height=fabs(topright.y()-bottomleft.y());
    this->x=bottomleft.x()+this->width/2.0;
    this->y=bottomleft.y()+this->height/2.0;
}

JKQTPgeoRectangle::JKQTPgeoRectangle(JKQtPlotter *parent, QPointF bottomleft, QPointF topright, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->angle=0;
    this->width=fabs(topright.x()-bottomleft.x());
    this->height=fabs(topright.y()-bottomleft.y());
    this->x=bottomleft.x()+this->width/2.0;
    this->y=bottomleft.y()+this->height/2.0;
}

QMatrix JKQTPgeoRectangle::getMatrix() {
    QMatrix trans;
    trans.rotate(angle);
    return trans;
}

bool JKQTPgeoRectangle::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    QRectF bound=getPolygon().boundingRect();
    //std::cout<<"JKQTPgeoRectangle::getXMinMax: b.left="<<bound.left()<<" b.right="<<bound.right()<<" b.top="<<bound.top()<<" b.bottom="<<bound.bottom()<<std::endl;
    minx=bound.left();
    maxx=bound.right();
    if (minx>maxx) std::swap(minx, maxx);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=minx; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxx; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPgeoRectangle::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    QRectF bound=getPolygon().boundingRect();
    miny=bound.bottom();
    maxy=bound.top();
    if (miny>maxy) std::swap(miny, maxy);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=miny; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxy; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

QPolygonF JKQTPgeoRectangle::getPolygon() {
    QMatrix m=getMatrix();
    QPolygonF rect;
    rect.append(m.map(QPointF(0-width/2.0, 0-height/2.0)));
    rect.append(m.map(QPointF(0-width/2.0, 0+height/2.0)));
    rect.append(m.map(QPointF(0+width/2.0, 0+height/2.0)));
    rect.append(m.map(QPointF(0+width/2.0, 0-height/2.0)));
    rect.translate(x,y);
    return rect;
}

void JKQTPgeoRectangle::draw(JKQTPEnhancedPainter& painter) {
    QPolygonF poly=getPolygon();
    QPolygonF rect;
    for (int i=0; i<poly.size(); i++) {
        rect.append(QPointF(transformX(poly[i].x()), transformY(poly[i].y())));
    }

    painter.save();
    painter.setPen(getPen(painter));
    painter.setBrush(getBrush(painter));
    painter.drawPolygon(rect);
    painter.restore();
}


void JKQTPgeoRectangle::set_bottomleftrectangle(double x, double y, double width, double height) {
    this->x=x+width/2.0;
    this->y=y+height/2.0;
    this->width=width;
    this->height=height;
}



JKQTPgeoPolygon::JKQTPgeoPolygon(JKQtBasePlotter* parent, const QVector<QPointF>& points, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->points=points;
}
JKQTPgeoPolygon::JKQTPgeoPolygon(JKQtPlotter* parent, const QVector<QPointF>& points, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->points=points;
}

JKQTPgeoPolygon::JKQTPgeoPolygon(JKQtBasePlotter* parent, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
}
JKQTPgeoPolygon::JKQTPgeoPolygon(JKQtPlotter* parent, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
}

bool JKQTPgeoPolygon::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=0;
    maxx=0;
    smallestGreaterZero=0;
    if (points.size()>0) {
        minx=points[0].x();
        maxx=points[0].x();
        for (int i=1; i<points.size(); i++) {
            double x=points[i].x();
            if (x>maxx) maxx=x;
            if (x<minx) minx=x;
            double xvsgz;
            xvsgz=x; SmallestGreaterZeroCompare_xvsgz();
        }
        return true;
    }
    return false;
}

bool JKQTPgeoPolygon::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=0;
    maxy=0;
    smallestGreaterZero=0;
    if (points.size()>0) {
        miny=points[0].y();
        maxy=points[0].y();
        for (int i=1; i<points.size(); i++) {
            double y=points[i].y();
            if (y>maxy) maxy=y;
            if (y<miny) miny=y;
            double xvsgz;
            xvsgz=y; SmallestGreaterZeroCompare_xvsgz();
        }
        return true;
    }
    return false;
}

void JKQTPgeoPolygon::draw(JKQTPEnhancedPainter& painter) {
    QPolygonF path=transformToPolygon(points);
    painter.save();
    painter.setPen(getPen(painter));
    painter.setBrush(getBrush(painter));
    painter.drawPolygon(path);
    painter.restore();
}



JKQTPgeoEllipse::JKQTPgeoEllipse(JKQtBasePlotter* parent, double x, double y, double width, double height, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoRectangle(parent, x, y, width, height, color, lineWidth, style, fillColor, fillStyle)
{
    controlPoints=180;
}

JKQTPgeoEllipse::JKQTPgeoEllipse(JKQtPlotter* parent, double x, double y, double width, double height, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoRectangle(parent, x, y, width, height, color, lineWidth, style, fillColor, fillStyle)
{
    controlPoints=180;
}

JKQTPgeoEllipse::JKQTPgeoEllipse(JKQtBasePlotter *parent, double x, double y, double width, double height, double angle, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoRectangle(parent, x, y, width, height, angle, color, lineWidth, style, fillColor, fillStyle)
{
    controlPoints=180;
}

JKQTPgeoEllipse::JKQTPgeoEllipse(JKQtPlotter *parent, double x, double y, double width, double height, double angle, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoRectangle(parent, x, y, width, height, angle, color, lineWidth, style, fillColor, fillStyle)
{
    controlPoints=180;
}

JKQTPgeoEllipse::JKQTPgeoEllipse(JKQtBasePlotter *parent, QPointF bottomleft, QPointF topright, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoRectangle(parent, bottomleft, topright, color, lineWidth, style, fillColor, fillStyle)
{
    controlPoints=180;
}

JKQTPgeoEllipse::JKQTPgeoEllipse(JKQtPlotter *parent, QPointF bottomleft, QPointF topright, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoRectangle(parent, bottomleft, topright, color, lineWidth, style, fillColor, fillStyle)
{
    controlPoints=180;
}


void JKQTPgeoEllipse::draw(JKQTPEnhancedPainter& painter) {
    QPainterPath rect;
    rect=transformToLinePath(JKQTPdrawEllipse(x,y,width/2.0, height/2.0,0,360,angle, controlPoints));
    rect.closeSubpath();


    painter.save();
    painter.setPen(getPen(painter));
    painter.setBrush(getBrush(painter));
    painter.drawPath(rect);
    painter.restore();
}



JKQTPgeoArc::JKQTPgeoArc(JKQtBasePlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPgeoBaseLine(color, lineWidth, style, parent)
{
    this->angleStart=angleStart;
    this->angleStop=angleStop;
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
    this->angle=0;
    this->controlPoints=180;
}

JKQTPgeoArc::JKQTPgeoArc(JKQtPlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPgeoBaseLine(color, lineWidth, style, parent)
{
    this->angleStart=angleStart;
    this->angleStop=angleStop;
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
    this->angle=0;
    this->controlPoints=180;
}


void JKQTPgeoArc::draw(JKQTPEnhancedPainter& painter) {
    QPainterPath rect;
    rect=transformToLinePath(JKQTPdrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));

    painter.save();
    painter.setPen(getPen(painter));
    painter.drawPath(rect);
    painter.restore();
}


bool JKQTPgeoArc::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    QPolygonF rect;
    rect=QPolygonF(JKQTPdrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    minx=rect.boundingRect().left();
    maxx=rect.boundingRect().right();
    if (minx>maxx) std::swap(minx, maxx);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=minx; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxx; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPgeoArc::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    QPolygonF rect;
    rect=QPolygonF(JKQTPdrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    miny=rect.boundingRect().bottom();
    maxy=rect.boundingRect().top();
    if (miny>maxy) std::swap(miny, maxy);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=miny; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxy; SmallestGreaterZeroCompare_xvsgz();
    return true;
}






JKQTPgeoPie::JKQTPgeoPie(JKQtBasePlotter* parent, double x, double y, double width, double height,double angleStart, double angleStop, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoEllipse(parent, x, y, width, height, color, lineWidth, style, fillColor, fillStyle)
{
    this->angleStart=angleStart;
    this->angleStop=angleStop;
}

JKQTPgeoPie::JKQTPgeoPie(JKQtPlotter* parent, double x, double y, double width, double height,double angleStart, double angleStop, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoEllipse(parent, x, y, width, height, color, lineWidth, style, fillColor, fillStyle)
{
    this->angleStart=angleStart;
    this->angleStop=angleStop;
}


void JKQTPgeoPie::draw(JKQTPEnhancedPainter& painter) {
    QPainterPath rect;
    rect=transformToLinePath(JKQTPdrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    rect.lineTo(transform(x,y));
    rect.closeSubpath();


    painter.save();
    painter.setPen(getPen(painter));
    painter.setBrush(getBrush(painter));
    painter.drawPath(rect);
    painter.restore();
}

bool JKQTPgeoPie::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    QPolygonF rect;
    rect=QPolygonF(JKQTPdrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    rect.append(QPointF(x,y));
    minx=rect.boundingRect().left();
    maxx=rect.boundingRect().right();
    if (minx>maxx) std::swap(minx, maxx);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=minx; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxx; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPgeoPie::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    QPolygonF rect;
    rect=QPolygonF(JKQTPdrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    rect.append(QPointF(x,y));
    miny=rect.boundingRect().bottom();
    maxy=rect.boundingRect().top();
    if (miny>maxy) std::swap(miny, maxy);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=miny; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxy; SmallestGreaterZeroCompare_xvsgz();
    return true;
}




JKQTPgeoChord::JKQTPgeoChord(JKQtBasePlotter* parent, double x, double y, double width, double height,double angleStart, double angleStop, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoPie(parent, x, y, width, height, angleStart, angleStop, color, lineWidth, style, fillColor, fillStyle)
{

}

JKQTPgeoChord::JKQTPgeoChord(JKQtPlotter* parent, double x, double y, double width, double height,double angleStart, double angleStop, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPgeoPie(parent, x, y, width, height, angleStart, angleStop, color, lineWidth, style, fillColor, fillStyle)
{

}


void JKQTPgeoChord::draw(JKQTPEnhancedPainter& painter) {
    QPainterPath rect;
    rect=transformToLinePath(JKQTPdrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    rect.closeSubpath();


    painter.save();
    painter.setPen(getPen(painter));
    painter.setBrush(getBrush(painter));
    painter.drawPath(rect);
    painter.restore();
}

bool JKQTPgeoChord::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    QPolygonF rect;
    rect=QPolygonF(JKQTPdrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    minx=rect.boundingRect().left();
    maxx=rect.boundingRect().right();
    if (minx>maxx) std::swap(minx, maxx);
    double xvsgz;
    xvsgz=minx; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxx; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPgeoChord::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    QPolygonF rect;
    rect=QPolygonF(JKQTPdrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    miny=rect.boundingRect().bottom();
    maxy=rect.boundingRect().top();
    if (miny>maxy) std::swap(miny, maxy);
    double xvsgz;
    xvsgz=miny; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxy; SmallestGreaterZeroCompare_xvsgz();
    return true;
}


JKQTPgeoSymbol::JKQTPgeoSymbol(JKQtBasePlotter *parent, double x, double y, JKQTPgraphSymbols symbol, double symbolSize, QColor color, QColor fillColor):
    JKQTPplotObject(parent)
{
    this->x=x;
    this->y=y;
    this->symbol=symbol;
    this->symbolSize=symbolSize;
    this->color=color;
    this->fillColor=fillColor;
    this->symbolWidth=1;
}

JKQTPgeoSymbol::JKQTPgeoSymbol(JKQtPlotter *parent, double x, double y, JKQTPgraphSymbols symbol, double symbolSize, QColor color, QColor fillColor):
    JKQTPplotObject(parent)
{
    this->x=x;
    this->y=y;
    this->symbol=symbol;
    this->symbolSize=symbolSize;
    this->color=color;
    this->fillColor=fillColor;
    this->symbolWidth=1;
}

bool JKQTPgeoSymbol::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    minx=x;
    maxx=x;
    double xvsgz;
    xvsgz=minx; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxx; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPgeoSymbol::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    miny=y;
    maxy=y;
    double xvsgz;
    xvsgz=miny; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxy; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

void JKQTPgeoSymbol::draw(JKQTPEnhancedPainter &painter)
{
    painter.save();
    JKQTPplotSymbol(painter, transformX(x), transformY(y), symbol, symbolSize, symbolWidth, color, fillColor);
    painter.restore();
}

void JKQTPgeoSymbol::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    const double minSize=qMin(rect.width(), rect.height());
    double symbolSize=parent->pt2px(painter, this->symbolSize);
    if (symbolSize>minSize*0.9) symbolSize=minSize*0.9;
    double symbolWidth=parent->pt2px(painter, this->symbolWidth*parent->get_lineWidthMultiplier());
    if (symbolWidth>0.3*symbolSize) symbolWidth=0.3*symbolSize;

    painter.save();
    JKQTPplotSymbol(painter, rect.left()+rect.width()/2.0, rect.top()+rect.height()/2.0, symbol, symbolSize, symbolWidth, color, fillColor);
    painter.restore();
}

QColor JKQTPgeoSymbol::getKeyLabelColor()
{
    return color;
}