/************************************************************************
 **
 **  @file   dialogellipticalarc.cpp
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   15 9, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "dialogellipticalarc.h"

#include <limits.h>
#include <QDialog>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include <QTimer>
#include <QToolButton>
#include <Qt>

#include "../../tools/vabstracttool.h"
#include "../ifc/xml/vdomdocument.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../../visualization/path/vistoolellipticalarc.h"
#include "../../visualization/visualization.h"
#include "../support/dialogeditwrongformula.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "ui_dialogellipticalarc.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogEllipticalArc create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogEllipticalArc::DialogEllipticalArc(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogEllipticalArc),
      flagRadius1(false),
      flagRadius2(false),
      flagF1(false),
      flagF2(false),
      flagRotationAngle(false),
      timerRadius1(new QTimer(this)),
      timerRadius2(new QTimer(this)),
      timerF1(new QTimer(this)),
      timerF2(new QTimer(this)),
      timerRotationAngle(new QTimer(this)),
      radius1(),
      radius2(),
      f1(),
      f2(),
      rotationAngle(),
      formulaBaseHeightRadius1(0),
      formulaBaseHeightRadius2(0),
      formulaBaseHeightF1(0),
      formulaBaseHeightF2(0),
      formulaBaseHeightRotationAngle(0),
      angleF1(INT_MIN),
      angleF2(INT_MIN),
      angleRotation(INT_MIN)
{
    ui->setupUi(this);

    this->formulaBaseHeightRadius1 = ui->plainTextEditRadius1->height();
    this->formulaBaseHeightRadius2 = ui->plainTextEditRadius2->height();
    this->formulaBaseHeightF1 = ui->plainTextEditF1->height();
    this->formulaBaseHeightF2 = ui->plainTextEditF2->height();
    this->formulaBaseHeightRotationAngle = ui->plainTextEditRotationAngle->height();

    ui->plainTextEditRadius1->installEventFilter(this);
    ui->plainTextEditRadius2->installEventFilter(this);
    ui->plainTextEditF1->installEventFilter(this);
    ui->plainTextEditF2->installEventFilter(this);
    ui->plainTextEditRotationAngle->installEventFilter(this);

    timerRadius1->setSingleShot(true);
    connect(timerRadius1, &QTimer::timeout, this, &DialogEllipticalArc::EvalRadiuses);

    timerRadius2->setSingleShot(true);
    connect(timerRadius2, &QTimer::timeout, this, &DialogEllipticalArc::EvalRadiuses);

    timerF1->setSingleShot(true);
    connect(timerF1, &QTimer::timeout, this, &DialogEllipticalArc::EvalAngles);

    timerF2->setSingleShot(true);
    connect(timerF2, &QTimer::timeout, this, &DialogEllipticalArc::EvalAngles);

    timerRotationAngle->setSingleShot(true);
    connect(timerRotationAngle, &QTimer::timeout, this, &DialogEllipticalArc::EvalAngles);

    InitOkCancelApply(ui);

    FillComboBoxPoints(ui->comboBoxBasePoint);
    FillComboBoxLineColors(ui->comboBoxColor);
    FillComboBoxTypeLine(ui->comboBoxPenStyle, CurvePenStylesPics());

    connect(ui->toolButtonExprRadius1, &QPushButton::clicked, this, &DialogEllipticalArc::FXRadius1);
    connect(ui->toolButtonExprRadius2, &QPushButton::clicked, this, &DialogEllipticalArc::FXRadius2);
    connect(ui->toolButtonExprF1, &QPushButton::clicked, this, &DialogEllipticalArc::FXF1);
    connect(ui->toolButtonExprF2, &QPushButton::clicked, this, &DialogEllipticalArc::FXF2);
    connect(ui->toolButtonExprRotationAngle, &QPushButton::clicked, this, &DialogEllipticalArc::FXRotationAngle);

    connect(ui->plainTextEditRadius1, &QPlainTextEdit::textChanged, this, [this]()
    {
        timerRadius1->start(formulaTimerTimeout);
    });

    connect(ui->plainTextEditRadius2, &QPlainTextEdit::textChanged, this, [this]()
    {
        timerRadius2->start(formulaTimerTimeout);
    });

    connect(ui->plainTextEditF1, &QPlainTextEdit::textChanged, this, [this]()
    {
        timerF1->start(formulaTimerTimeout);
    });

    connect(ui->plainTextEditF2, &QPlainTextEdit::textChanged, this, [this]()
    {
        timerF2->start(formulaTimerTimeout);
    });

    connect(ui->plainTextEditRotationAngle, &QPlainTextEdit::textChanged, this, [this]()
    {
        timerRotationAngle->start(formulaTimerTimeout);
    });

    connect(ui->pushButtonGrowLengthRadius1, &QPushButton::clicked, this, &DialogEllipticalArc::DeployRadius1TextEdit);
    connect(ui->pushButtonGrowLengthRadius2, &QPushButton::clicked, this, &DialogEllipticalArc::DeployRadius2TextEdit);
    connect(ui->pushButtonGrowLengthF1, &QPushButton::clicked, this, &DialogEllipticalArc::DeployF1TextEdit);
    connect(ui->pushButtonGrowLengthF2, &QPushButton::clicked, this, &DialogEllipticalArc::DeployF2TextEdit);
    connect(ui->pushButtonGrowLengthRotationAngle, &QPushButton::clicked,
            this, &DialogEllipticalArc::DeployRotationAngleTextEdit);

    vis = new VisToolEllipticalArc(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogEllipticalArc::~DialogEllipticalArc()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetCenter return id of center point
 * @return id id
 */
quint32 DialogEllipticalArc::GetCenter() const
{
    return getCurrentObjectId(ui->comboBoxBasePoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetCenter set id of center point
 * @param value id
 */
void DialogEllipticalArc::SetCenter(const quint32 &value)
{
    ChangeCurrentData(ui->comboBoxBasePoint, value);
    vis->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius1 return formula of radius1
 * @return formula
 */
QString DialogEllipticalArc::GetRadius1() const
{
    return qApp->TrVars()->TryFormulaFromUser(radius1, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetRadius1 set formula of radius1
 * @param value formula
 */
void DialogEllipticalArc::SetRadius1(const QString &value)
{
    radius1 = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (radius1.length() > 80)
    {
        this->DeployRadius1TextEdit();
    }
    ui->plainTextEditRadius1->setPlainText(radius1);

    VisToolEllipticalArc *path = qobject_cast<VisToolEllipticalArc *>(vis);
    SCASSERT(path != nullptr)
    path->setRadius1(radius1);

    MoveCursorToEnd(ui->plainTextEditRadius1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius2 return formula of radius2
 * @return formula
 */
QString DialogEllipticalArc::GetRadius2() const
{
    return qApp->TrVars()->TryFormulaFromUser(radius2, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetRadius2 set formula of radius2
 * @param value formula
 */
void DialogEllipticalArc::SetRadius2(const QString &value)
{
    radius2 = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (radius2.length() > 80)
    {
        this->DeployRadius2TextEdit();
    }
    ui->plainTextEditRadius2->setPlainText(radius2);

    VisToolEllipticalArc *path = qobject_cast<VisToolEllipticalArc *>(vis);
    SCASSERT(path != nullptr)
    path->setRadius2(radius2);

    MoveCursorToEnd(ui->plainTextEditRadius2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF1 return formula first angle of elliptical arc
 * @return formula
 */
QString DialogEllipticalArc::GetF1() const
{
    return qApp->TrVars()->TryFormulaFromUser(f1, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetF1 set formula first angle of elliptical arc
 * @param value formula
 */
void DialogEllipticalArc::SetF1(const QString &value)
{
    f1 = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (f1.length() > 80)
    {
        this->DeployF1TextEdit();
    }
    ui->plainTextEditF1->setPlainText(f1);

    VisToolEllipticalArc *path = qobject_cast<VisToolEllipticalArc *>(vis);
    SCASSERT(path != nullptr)
    path->setF1(f1);

    MoveCursorToEnd(ui->plainTextEditF1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF2 return formula second angle of elliptical arc
 * @return formula
 */
QString DialogEllipticalArc::GetF2() const
{
    return qApp->TrVars()->TryFormulaFromUser(f2, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetF2 set formula second angle of elliptical arc
 * @param value formula
 */
void DialogEllipticalArc::SetF2(const QString &value)
{
    f2 = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (f2.length() > 80)
    {
        this->DeployF2TextEdit();
    }
    ui->plainTextEditF2->setPlainText(f2);

    VisToolEllipticalArc *path = qobject_cast<VisToolEllipticalArc *>(vis);
    SCASSERT(path != nullptr)
    path->setF2(f2);

    MoveCursorToEnd(ui->plainTextEditF2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRotationAngle return formula rotation angle of elliptical arc
 * @return formula
 */
QString DialogEllipticalArc::GetRotationAngle() const
{
    return qApp->TrVars()->TryFormulaFromUser(rotationAngle, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetRotationAngle set formula rotation angle of elliptical arc
 * @param value formula
 */
void DialogEllipticalArc::SetRotationAngle(const QString &value)
{
    rotationAngle = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (rotationAngle.length() > 80)
    {
        this->DeployRotationAngleTextEdit();
    }
    ui->plainTextEditRotationAngle->setPlainText(rotationAngle);

    VisToolEllipticalArc *path = qobject_cast<VisToolEllipticalArc *>(vis);
    SCASSERT(path != nullptr)
    path->setRotationAngle(rotationAngle);

    MoveCursorToEnd(ui->plainTextEditRotationAngle);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogEllipticalArc::GetPenStyle() const
{
    return GetComboBoxCurrentData(ui->comboBoxPenStyle, TypeLineLine);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::SetPenStyle(const QString &value)
{
    ChangeCurrentData(ui->comboBoxPenStyle, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetColor return color of elliptical arc
 * @return formula
 */
QString DialogEllipticalArc::GetColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxColor, ColorBlack);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetColor set color of elliptical arc
 * @param value formula
 */
void DialogEllipticalArc::SetColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EvalRadiuses calculate value of radiuses
 */
void DialogEllipticalArc::EvalRadiuses()
{
    FormulaData formulaData;
    formulaData.formula = ui->plainTextEditRadius1->toPlainText();
    formulaData.variables = data->DataVariables();
    formulaData.labelEditFormula = ui->labelEditRadius1;
    formulaData.labelResult = ui->labelResultRadius1;
    formulaData.postfix = UnitsToStr(qApp->patternUnit(), true);
    formulaData.checkLessThanZero = true;

    Eval(formulaData, flagRadius1);

    formulaData.formula = ui->plainTextEditRadius2->toPlainText();
    formulaData.labelEditFormula = ui->labelEditRadius2;
    formulaData.labelResult = ui->labelResultRadius2;

    Eval(formulaData, flagRadius2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EvalAngles calculate value of angles
 */
void DialogEllipticalArc::EvalAngles()
{
    FormulaData formulaData;
    formulaData.formula = ui->plainTextEditF1->toPlainText();
    formulaData.variables = data->DataVariables();
    formulaData.labelEditFormula = ui->labelEditF1;
    formulaData.labelResult = ui->labelResultF1;
    formulaData.postfix = degreeSymbol;
    formulaData.checkZero = false;

    angleF1 = Eval(formulaData, flagF1);

    formulaData.formula = ui->plainTextEditF2->toPlainText();
    formulaData.labelEditFormula = ui->labelEditF2;
    formulaData.labelResult = ui->labelResultF2;

    angleF2 = Eval(formulaData, flagF2);

    formulaData.formula = ui->plainTextEditRotationAngle->toPlainText();
    formulaData.labelEditFormula = ui->labelEditRotationAngle;
    formulaData.labelResult = ui->labelResultRotationAngle;

    angleRotation = Eval(formulaData, flagRotationAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::FXRadius1()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit radius1"));
    dialog->SetFormula(GetRadius1());
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetRadius1(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::FXRadius2()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit radius2"));
    dialog->SetFormula(GetRadius2());
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetRadius2(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::FXF1()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit first angle"));
    dialog->SetFormula(GetF1());
    dialog->setPostfix(degreeSymbol);
    if (dialog->exec() == QDialog::Accepted)
    {
        SetF1(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::FXF2()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit second angle"));
    dialog->SetFormula(GetF2());
    dialog->setPostfix(degreeSymbol);
    if (dialog->exec() == QDialog::Accepted)
    {
        SetF2(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::FXRotationAngle()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit rotation angle"));
    dialog->SetFormula(GetRotationAngle());
    dialog->setPostfix(degreeSymbol);
    if (dialog->exec() == QDialog::Accepted)
    {
        SetRotationAngle(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::DeployRadius1TextEdit()
{
    DeployFormula(this, ui->plainTextEditRadius1, ui->pushButtonGrowLengthRadius1, formulaBaseHeightRadius1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::DeployRadius2TextEdit()
{
    DeployFormula(this, ui->plainTextEditRadius2, ui->pushButtonGrowLengthRadius2, formulaBaseHeightRadius2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::DeployF1TextEdit()
{
    DeployFormula(this, ui->plainTextEditF1, ui->pushButtonGrowLengthF1, formulaBaseHeightF1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::DeployF2TextEdit()
{
    DeployFormula(this, ui->plainTextEditF2, ui->pushButtonGrowLengthF2, formulaBaseHeightF2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::DeployRotationAngleTextEdit()
{
    DeployFormula(this, ui->plainTextEditRotationAngle, ui->pushButtonGrowLengthRotationAngle,formulaBaseHeightRotationAngle);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogEllipticalArc::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            if (SetObject(id, ui->comboBoxBasePoint, QString()))
            {
                vis->VisualMode(id);
                prepare = true;
                this->setModal(true);
                this->show();
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::ShowVisualization()
{
    AddVisualization<VisToolEllipticalArc>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::SaveData()
{
    radius1 = ui->plainTextEditRadius1->toPlainText();
    radius2 = ui->plainTextEditRadius2->toPlainText();
    f1 = ui->plainTextEditF1->toPlainText();
    f2 = ui->plainTextEditF2->toPlainText();
    rotationAngle = ui->plainTextEditRotationAngle->toPlainText();

    VisToolEllipticalArc *path = qobject_cast<VisToolEllipticalArc *>(vis);
    SCASSERT(path != nullptr)

    path->setObject1Id(GetCenter());
    path->setRadius1(radius1);
    path->setRadius2(radius2);
    path->setF1(f1);
    path->setF2(f2);
    path->setRotationAngle(rotationAngle);
    path->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditRadius1->blockSignals(true);
    ui->plainTextEditRadius2->blockSignals(true);
    ui->plainTextEditF1->blockSignals(true);
    ui->plainTextEditF2->blockSignals(true);
    ui->plainTextEditRotationAngle->blockSignals(true);
    DialogTool::closeEvent(event);
}
