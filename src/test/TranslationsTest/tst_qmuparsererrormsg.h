/************************************************************************
 **
 **  @file   tst_qmuparsererrormsg.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 10, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef TST_QMUPARSERERRORMSG_H
#define TST_QMUPARSERERRORMSG_H

#include "../vtest/abstracttest.h"
#include "../qmuparser/qmuparsererror.h"

#include <QTranslator>
#include <QPointer>

class TST_QmuParserErrorMsg : public AbstractTest
{
    Q_OBJECT
public:
    explicit TST_QmuParserErrorMsg(const QString &locale, QObject *parent = nullptr);
    virtual ~TST_QmuParserErrorMsg() override;

private slots:
    void initTestCase();
    void TestEErrorCodes_data();
    void TestEErrorCodes();
    void cleanupTestCase();

private:
    Q_DISABLE_COPY(TST_QmuParserErrorMsg)

    QString m_locale;
    QPointer<QTranslator> appTranslator;
    qmu::QmuParserErrorMsg *msg;

    void AddCase(int code, bool tok, bool pos);
    int  LoadTranslation(const QString &checkedLocale);
    void RemoveTranslation();
};

#endif // TST_QMUPARSERERRORMSG_H
