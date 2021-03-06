/************************************************************************
 **
 **  @file   vpointf_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 8, 2014
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

#ifndef VPOINTF_P_H
#define VPOINTF_P_H

#include <QSharedData>
#include "vgeometrydef.h"
#include "../vmisc/diagnostic.h"
#include <QPointF>

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VPointFData : public QSharedData
{
public:

    VPointFData()
        : _mx(0), _my(0), _x(0), _y(0), m_showLabel(true)
    {}

    VPointFData(const VPointFData &point)
        :QSharedData(point), _mx(point._mx), _my(point._my), _x(point._x), _y(point._y), m_showLabel(point.m_showLabel)
    {}

    explicit VPointFData(const QPointF &point)
        :_mx(0), _my(0), _x(point.x()), _y(point.y()), m_showLabel(true)
    {}

    VPointFData(qreal x, qreal y, qreal mx, qreal my)
        :_mx(mx), _my(my), _x(x), _y(y), m_showLabel(true)
    {}

    VPointFData(const QPointF &point, qreal mx, qreal my)
        :_mx(mx), _my(my), _x(point.x()), _y(point.y()), m_showLabel(true)
    {}

    virtual ~VPointFData();

    /** @brief _mx offset name respect to x */
    qreal   _mx;

    /** @brief _my offset name respect to y */
    qreal   _my;

    /** @brief _x x coordinate */
    qreal   _x;

    /** @brief _y y coordinate */
    qreal   _y;

    /** @brief m_hideLabel show or not label for this point */
    bool m_showLabel;

private:
    Q_DISABLE_ASSIGN(VPointFData)
};

VPointFData::~VPointFData()
{}

QT_WARNING_POP

#endif // VPOINTF_P_H
