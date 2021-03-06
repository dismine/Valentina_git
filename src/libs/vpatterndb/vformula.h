/************************************************************************
 **
 **  @file   vformula.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 8, 2014
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

#ifndef VFORMULA_H
#define VFORMULA_H

#include <QCoreApplication>
#include <QSharedDataPointer>
#include <QMetaType>
#include <QTypeInfo>
#include <QString>
#include <QtGlobal>

enum class FormulaType : qint8
{
    ToUser = 0,
    FromUser = 1,
    ToSystem = FromUser,
    FromSystem = ToUser
};

class VContainer;
class VFormulaData;

class VFormula
{
    Q_DECLARE_TR_FUNCTIONS(VFormula)
public:
    VFormula();
    VFormula(const QString &formula, const VContainer *container);
    VFormula &operator=(const VFormula &formula);
    VFormula(const VFormula &formula);
    ~VFormula();

    bool operator==(const VFormula &formula) const;
    bool operator!=(const VFormula &formula) const;

    QString GetFormula(FormulaType type = FormulaType::ToUser) const;
    void SetFormula(const QString &value, FormulaType type = FormulaType::FromSystem);

    QString getStringValue() const;
    qreal   getDoubleValue() const;

    bool getCheckZero() const;
    void setCheckZero(bool value);

    bool getCheckLessThanZero() const;
    void setCheckLessThanZero(bool value);

    const VContainer *getData() const;
    void setData(const VContainer *value);

    quint32 getToolId() const;
    void setToolId(quint32 value);

    QString getPostfix() const;
    void setPostfix(const QString &value);

    bool error() const;
    QString Reason() const;

    static int FormulaTypeId();

    void Eval();
private:
    QSharedDataPointer<VFormulaData> d;

    void ResetState();
};
Q_DECLARE_METATYPE(VFormula)
Q_DECLARE_TYPEINFO(VFormula, Q_MOVABLE_TYPE);

#endif // VFORMULA_H
