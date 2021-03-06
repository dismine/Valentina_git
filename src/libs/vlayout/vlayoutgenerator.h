/************************************************************************
 **
 **  @file   vlayoutgenerator.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   2 1, 2015
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

#ifndef VLAYOUTGENERATOR_H
#define VLAYOUTGENERATOR_H

#include <qcompilerdetection.h>
#include <QList>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QVector>
#include <QtGlobal>
#include <memory>
#include <atomic>
#include <QMargins>

#include "vbank.h"
#include "vlayoutdef.h"

class QGraphicsItem;
class VLayoutPaper;
class QElapsedTimer;

class VLayoutGenerator :public QObject
{
    Q_OBJECT
public:
    explicit VLayoutGenerator(QObject *parent = nullptr);
    virtual ~VLayoutGenerator() override;

    void SetDetails(const QVector<VLayoutPiece> &details);
    void SetLayoutWidth(qreal width);
    void SetCaseType(Cases caseType);
    int DetailsCount();

    qreal GetPaperHeight() const;
    void SetPaperHeight(qreal value);

    qreal GetPaperWidth() const;
    void SetPaperWidth(qreal value);

    int  GetNestingTime() const;
    int  GetNestingTimeMSecs() const;
    void SetNestingTime(int value);

    qreal GetEfficiencyCoefficient() const;
    void  SetEfficiencyCoefficient(qreal coefficient);

    bool IsUsePrinterFields() const;
    QMarginsF GetPrinterFields() const;
    void SetPrinterFields(bool usePrinterFields, const QMarginsF &value);

    qreal GetShift() const;
    void  SetShift(qreal shift);

    void Generate(const QElapsedTimer &timer, qint64 timeout, LayoutErrors previousState = LayoutErrors::NoError);

    qreal LayoutEfficiency() const;

    LayoutErrors State() const;

    int PapersCount() const {return papers.size();}

    Q_REQUIRED_RESULT QList<QGraphicsItem *> GetPapersItems() const;
    Q_REQUIRED_RESULT QList<QGraphicsItem *> GetGlobalContours() const;
    Q_REQUIRED_RESULT QList<QList<QGraphicsItem *>> GetAllDetailsItems() const;

    QVector<QVector<VLayoutPiece>> GetAllDetails() const;

    bool GetRotate() const;
    void SetRotate(bool value);

    bool GetFollowGrainline() const;
    void SetFollowGrainline(bool value);

    bool GetManualPriority() const;
    void SetManualPriority(bool value);

    bool IsNestQuantity() const;
    void SetNestQuantity(bool value);

    int GetRotationNumber() const;
    void SetRotationNumber(int value);

    bool GetAutoCropLength() const;
    void SetAutoCropLength(bool value);

    bool GetAutoCropWidth() const;
    void SetAutoCropWidth(bool value);

    bool IsSaveLength() const;
    void SetSaveLength(bool value);

    bool IsUnitePages() const;
    void SetUnitePages(bool value);

    quint8 GetMultiplier() const;
    void   SetMultiplier(quint8 value);

    bool IsStripOptimization() const;
    void SetStripOptimization(bool value);

    bool IsTestAsPaths() const;
    void SetTextAsPaths(bool value);

    bool IsRotationNeeded() const;

    bool IsPortrait() const;

public slots:
    void Abort();
    void Timeout();

private:
    Q_DISABLE_COPY(VLayoutGenerator)
    QVector<VLayoutPaper> papers;
    VBank *bank;
    qreal paperHeight;
    qreal paperWidth;
    QMarginsF margins;
    bool usePrinterFields;
    std::atomic_bool stopGeneration;
    LayoutErrors state;
    qreal shift;
    bool rotate;
    bool followGrainline;
    int rotationNumber;
    bool autoCropLength;
    bool autoCropWidth;
    bool saveLength;
    bool unitePages;
    bool stripOptimizationEnabled;
    quint8 multiplier;
    bool stripOptimization;
    bool textAsPaths;
    int nestingTime{1};
    qreal efficiencyCoefficient{0.0};

    int PageHeight() const;
    int PageWidth() const;

    void GatherPages();
    void UnitePages();
    void UniteDetails(int j, QList<QList<VLayoutPiece> > &nDetails, qreal length, int i);
    void UnitePapers(int j, QList<qreal> &papersLength, qreal length);
    QList<VLayoutPiece> MoveDetails(qreal length, const QVector<VLayoutPiece> &details);
};

#endif // VLAYOUTGENERATOR_H
