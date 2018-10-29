/************************************************************************
 **
 **  @file   vexceptioninvalidnotch.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 9, 2018
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2018 Valentina project
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
#ifndef VEXCEPTIONINVALIDNOTCH_H
#define VEXCEPTIONINVALIDNOTCH_H

#include "vexception.h"

class VExceptionInvalidNotch : public VException
{
public:
    explicit VExceptionInvalidNotch(const QString &error);
    VExceptionInvalidNotch(const VExceptionInvalidNotch &e);
    VExceptionInvalidNotch &operator=(const VExceptionInvalidNotch &e);
    virtual ~VExceptionInvalidNotch() V_NOEXCEPT_EXPR (true) Q_DECL_EQ_DEFAULT;
};

#endif // VEXCEPTIONINVALIDNOTCH_H