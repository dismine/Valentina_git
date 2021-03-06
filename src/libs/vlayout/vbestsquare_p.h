/************************************************************************
 **
 **  @file   vbestsquare_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   26 3, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
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
#ifndef VBESTSQUARE_P_H
#define VBESTSQUARE_P_H

#include <QSharedData>
#include <QTransform>

#include "vlayoutdef.h"
#include "../vmisc/diagnostic.h"
#include "../vmisc/defglobal.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VBestSquareData : public QSharedData
{
public:
    VBestSquareData()
    {}

    VBestSquareData(const QSizeF &sheetSize, bool saveLength, bool isPortrait)
        : isValid(true),
          sheetSize(sheetSize),
          saveLength(saveLength),
          isPortrait(isPortrait)
    {
        data.bestSize = QSizeF(sheetSize.width()+10, sheetSize.height()+10);
    }

    VBestSquareData(const VBestSquareData &res)
        : QSharedData(res),
          isValid(res.isValid),
          sheetSize(res.sheetSize),
          valideResult(res.valideResult),
          saveLength(res.saveLength),
          data(res.data),
          isPortrait(res.isPortrait),
          terminatedByException(res.terminatedByException),
          exceptionReason(res.exceptionReason)
    {}

    ~VBestSquareData() {}

    bool isValid{false};
    QSizeF sheetSize{};
    bool valideResult{false};
    bool saveLength{false};
    VBestSquareResData data{};
    bool isPortrait{true};
    bool terminatedByException{false};
    QString exceptionReason{};

private:
    Q_DISABLE_ASSIGN(VBestSquareData)
};

#endif // VBESTSQUARE_P_H
