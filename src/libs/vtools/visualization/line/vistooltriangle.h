/************************************************************************
 **
 **  @file   vistooltriangle.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#ifndef VISTOOLTRIANGLE_H
#define VISTOOLTRIANGLE_H

#include <qcompilerdetection.h>
#include <QColor>
#include <QGraphicsItem>
#include <QLineF>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <Qt>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "visline.h"

class VisToolTriangle : public VisLine
{
    Q_OBJECT
public:
    explicit VisToolTriangle(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolTriangle() = default;

    virtual void RefreshGeometry() override;

    void         setObject2Id(const quint32 &value);
    void         setHypotenuseP1Id(const quint32 &value);
    void         setHypotenuseP2Id(const quint32 &value);

    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolTriangle)};
private:
    Q_DISABLE_COPY(VisToolTriangle)
    quint32            object2Id;//axis second point
    quint32            hypotenuseP1Id;
    quint32            hypotenuseP2Id;
    VScaledEllipse    *point;
    VScaledEllipse    *axisP1;
    VScaledEllipse    *axisP2;
    VCurvePathItem    *axis;
    VScaledEllipse    *hypotenuseP1;
    VScaledEllipse    *hypotenuseP2;
    VScaledLine       *foot1;
    VScaledLine       *foot2;

    void         DrawAimedAxis(VCurvePathItem *item, const QLineF &line, const QColor &color,
                               Qt::PenStyle style = Qt::SolidLine);
    void         DrawArrow(const QLineF &axis, QPainterPath &path, const qreal &arrow_size);
};

#endif // VISTOOLTRIANGLE_H
