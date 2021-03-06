/************************************************************************
 **
 **  @file   vcontainer.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef VCONTAINER_H
#define VCONTAINER_H

#include <qcompilerdetection.h>
#include <QCoreApplication>
#include <QHash>
#include <QMap>
#include <QMessageLogger>
#include <QSet>
#include <QSharedPointer>
#include <QSharedData>
#include <QSharedDataPointer>
#include <QString>
#include <QStringList>
#include <QTypeInfo>
#include <QtGlobal>
#include <new>

#include "../vmisc/def.h"
#include "../ifc/exception/vexceptionbadid.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcubicbezierpath.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vgobject.h"
#include "../vmisc/diagnostic.h"
#include "variables.h"
#include "variables/vinternalvariable.h"
#include "vpiece.h"
#include "vpiecepath.h"
#include "vtranslatevars.h"

class VEllipticalArc;

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_INTEL(2021)
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VContainerData : public QSharedData //-V690
{
public:

    VContainerData(const VTranslateVars *trVars, const Unit *patternUnit, const QString &nspace)
        : calculationObjects(QHash<quint32, QSharedPointer<VGObject> >()),
          modelingObjects(QSharedPointer<QHash<quint32, QSharedPointer<VGObject>>>::create()),
          variables(QHash<QString, QSharedPointer<VInternalVariable> > ()),
          pieces(QSharedPointer<QHash<quint32, VPiece>>::create()),
          piecePaths(QSharedPointer<QHash<quint32, VPiecePath>>::create()),
          trVars(trVars),
          patternUnit(patternUnit),
          nspace(nspace)
    {}

    VContainerData(const VContainerData &data)
        : QSharedData(data),
          calculationObjects(data.calculationObjects),
          modelingObjects(data.modelingObjects),
          variables(data.variables),
          pieces(data.pieces),
          piecePaths(data.piecePaths),
          trVars(data.trVars),
          patternUnit(data.patternUnit),
          nspace(data.nspace)
    {}

    virtual ~VContainerData();

    QHash<quint32, QSharedPointer<VGObject> > calculationObjects;
    QSharedPointer<QHash<quint32, QSharedPointer<VGObject>>> modelingObjects;

    /**
     * @brief variables container for measurements, increments, lines lengths, lines angles, arcs lengths, curve lengths
     */
    QHash<QString, QSharedPointer<VInternalVariable>> variables;

    QSharedPointer<QHash<quint32, VPiece>> pieces;
    QSharedPointer<QHash<quint32, VPiecePath>> piecePaths;

    const VTranslateVars *trVars;
    const Unit *patternUnit;

    /** @brief nspace namespace for static variables */
    QString nspace;

private:
    Q_DISABLE_ASSIGN(VContainerData)
};

QT_WARNING_POP

/**
 * @brief The VContainer class container of all variables.
 */
class VContainer
{
    Q_DECLARE_TR_FUNCTIONS(VContainer)
public:
    VContainer(const VTranslateVars *trVars, const Unit *patternUnit, const QString &nspace);
    VContainer(const VContainer &data);
    ~VContainer();

    friend class VContainerData;

    VContainer &operator=(const VContainer &data);
#ifdef Q_COMPILER_RVALUE_REFS
    VContainer(const VContainer &&data) Q_DECL_NOTHROW;
    VContainer &operator=(VContainer &&data) Q_DECL_NOTHROW;
#endif

    static QString UniqueNamespace();

    template <typename T>
    const QSharedPointer<T> GeometricObject(const quint32 &id) const;
    const QSharedPointer<VGObject> GetGObject(quint32 id) const;
    static const QSharedPointer<VGObject> GetFakeGObject(quint32 id);
    VPiece             GetPiece(quint32 id) const;
    VPiecePath         GetPiecePath(quint32 id) const;
    quint32            GetPieceForPiecePath(quint32 id) const;
    template <typename T>
    QSharedPointer<T>  GetVariable(const QString &name) const;
    quint32            getId() const;
    quint32            getNextId() const;
    void               UpdateId(quint32 newId) const;
    static void        UpdateId(quint32 newId, const QString &nspace);

    quint32            AddGObject(VGObject *obj);
    quint32            AddGObject(const QSharedPointer<VGObject> &obj);
    quint32            AddPiece(const VPiece &detail);
    quint32            AddPiecePath(const VPiecePath &path);
    void               AddLine(const quint32 &firstPointId, const quint32 &secondPointId);
    void               AddArc(const QSharedPointer<VAbstractCurve> &arc, const quint32 &id,
                              const quint32 &parentId = NULL_ID);
    void               AddSpline(const QSharedPointer<VAbstractBezier> &curve, quint32 id, quint32 parentId = NULL_ID);
    void               AddCurveWithSegments(const QSharedPointer<VAbstractCubicBezierPath> &curve, const quint32 &id,
                                            quint32 parentId = NULL_ID);

    template <typename T>
    void               AddUniqueVariable(T *var);
    template <typename T>
    void               AddUniqueVariable(const QSharedPointer<T> &var);
    template <typename T>
    void               AddVariable(T *var);
    template <typename T>
    void               AddVariable(const QSharedPointer<T> &var);
    void               RemoveVariable(const QString& name);
    void               RemovePiece(quint32 id);

    template <class T>
    void               UpdateGObject(quint32 id, T* obj);
    template <class T>
    void               UpdateGObject(quint32 id, const QSharedPointer<T> &obj);
    void               UpdatePiece(quint32 id, const VPiece &detail);
    void               UpdatePiecePath(quint32 id, const VPiecePath &path);

    void               Clear();
    void               ClearForFullParse();
    void               ClearGObjects();
    void               ClearCalculationGObjects();
    void               ClearVariables(const VarType &type = VarType::Unknown);
    void               ClearVariables(const QVector<VarType> &types);
    void               ClearUniqueNames() const;
    void               ClearUniqueIncrementNames() const;
    void               ClearExceptUniqueIncrementNames() const;

    void               SetSize(qreal size) const;
    void               SetHeight(qreal height) const;
    qreal              size() const;
    static qreal       size(const QString &nspace);
    qreal              height() const;
    static qreal       height(const QString &nspace);

    void               RemoveIncrement(const QString& name);

    const QHash<quint32, QSharedPointer<VGObject> >         *CalculationGObjects() const;
    const QHash<quint32, VPiece>                            *DataPieces() const;
    const QHash<QString, QSharedPointer<VInternalVariable>> *DataVariables() const;

    const QMap<QString, QSharedPointer<VMeasurement> >  DataMeasurements() const;
    const QMap<QString, QSharedPointer<VIncrement> >    DataIncrements() const;
    const QMap<QString, QSharedPointer<VIncrement> >    DataIncrementsWithSeparators() const;
    const QMap<QString, QSharedPointer<VLengthLine> >   DataLengthLines() const;
    const QMap<QString, QSharedPointer<VCurveLength> >  DataLengthCurves() const;
    const QMap<QString, QSharedPointer<VCurveCLength> > DataCurvesCLength() const;
    const QMap<QString, QSharedPointer<VLineAngle> >    DataAngleLines() const;
    const QMap<QString, QSharedPointer<VArcRadius> >    DataRadiusesArcs() const;
    const QMap<QString, QSharedPointer<VCurveAngle> >   DataAnglesCurves() const;

    bool        IsUnique(const QString &name) const;
    static bool IsUnique(const QString &name, const QString &nspace);

    QStringList        AllUniqueNames() const;
    static QStringList AllUniqueNames(const QString &nspace);

    const Unit *GetPatternUnit() const;
    const VTranslateVars *GetTrVars() const;

private:
    /**
     * @brief _id current id. New object will have value +1. For empty class equal 0.
     */
    static QMap<QString, quint32> _id;
    static QMap<QString, qreal>   _size;
    static QMap<QString, qreal>   _height;
    static QMap<QString, QSet<QString>> uniqueNames;
    static QMap<QString, quint32> copyCounter;

    QSharedDataPointer<VContainerData> d;

    void AddCurve(const QSharedPointer<VAbstractCurve> &curve, const quint32 &id, quint32 parentId = NULL_ID);

    template <class T>
    uint qHash( const QSharedPointer<T> &p );

    template <typename T>
    void UpdateObject(const quint32 &id, const QSharedPointer<T> &point);

    template <typename T>
    const QMap<QString, QSharedPointer<T> > DataVar(const VarType &type) const;

    static void ClearNamespace(const QString &nspace);
};

Q_DECLARE_TYPEINFO(VContainer, Q_MOVABLE_TYPE);

/*
*  Defintion of templated member functions of VContainer
*/

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
const QSharedPointer<T> VContainer::GeometricObject(const quint32 &id) const
{
    if (id == NULL_ID)
    {
        throw VExceptionBadId(tr("Can't find object"), id);
    }

    QSharedPointer<VGObject> gObj;
    if (d->calculationObjects.contains(id))
    {
        gObj = d->calculationObjects.value(id);
    }
    else if (d->modelingObjects->contains(id))
    {
        gObj = d->modelingObjects->value(id);
    }
    else
    {
        throw VExceptionBadId(tr("Can't find object"), id);
    }

    QSharedPointer<T> obj = qSharedPointerDynamicCast<T>(gObj);
    SCASSERT(obj.isNull() == false)
    return obj;
}


//---------------------------------------------------------------------------------------------------------------------
/**
* @brief GetVariable return varible by name
* @param name variable's name
* @return variable
*/
template <typename T>
QSharedPointer<T> VContainer::GetVariable(const QString &name) const
{
    SCASSERT(name.isEmpty()==false)
    if (d->variables.contains(name))
    {
        try
        {
            QSharedPointer<T> value = qSharedPointerDynamicCast<T>(d->variables.value(name));
            SCASSERT(value.isNull() == false)
            return value;
        }
        catch (const std::bad_alloc &)
        {
            throw VExceptionBadId(tr("Can't cast object"), name);
        }
    }
    else
    {
        throw VExceptionBadId(tr("Can't find object"), name);
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VContainer::AddUniqueVariable(T *var)
{
    AddUniqueVariable(QSharedPointer<T>(var));
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VContainer::AddUniqueVariable(const QSharedPointer<T> &var)
{
    AddVariable(var);

    if (d->variables.contains(var->GetName()))
    {
        uniqueNames[d->nspace].insert(var->GetName());
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VContainer::AddVariable(T *var)
{
    AddVariable(QSharedPointer<T>(var));
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VContainer::AddVariable(const QSharedPointer<T> &var)
{
    SCASSERT(not var->GetName().isEmpty())
    if (d->variables.contains(var->GetName()))
    {
        if (d->variables.value(var->GetName())->GetType() == var->GetType())
        {
            QSharedPointer<T> v = qSharedPointerDynamicCast<T>(d->variables.value(var->GetName()));
            if (v.isNull())
            {
                throw VExceptionBadId(tr("Can't cast object."), var->GetName());
            }
            *v = *var;
        }
        else
        {
            throw VExceptionBadId(tr("Can't find object. Type mismatch."), var->GetName());
        }
    }
    else
    {
        d->variables.insert(var->GetName(), var);
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <class T>
uint VContainer::qHash( const QSharedPointer<T> &p )
{
    return qHash( p.data() );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateGObject update GObject by id
 * @param id id of existing GObject
 * @param obj object
 */
template <class T>
void VContainer::UpdateGObject(quint32 id, T* obj)
{
    SCASSERT(obj != nullptr)
    UpdateGObject(id, QSharedPointer<T>(obj));
}

//---------------------------------------------------------------------------------------------------------------------
template <class T>
void VContainer::UpdateGObject(quint32 id, const QSharedPointer<T> &obj)
{
    SCASSERT(not obj.isNull())
    UpdateObject(id, obj);
    uniqueNames[d->nspace].insert(obj->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateObject update object in container
 * @param id id of existing object
 * @param point object
 */
template <typename T>
void VContainer::UpdateObject(const quint32 &id, const QSharedPointer<T> &point)
{
    Q_ASSERT_X(id != NULL_ID, Q_FUNC_INFO, "id == 0"); //-V654 //-V712
    SCASSERT(point.isNull() == false)
    point->setId(id);

    if (d->calculationObjects.contains(id) && point->getMode() == Draw::Calculation)
    {
        QSharedPointer<T> obj = qSharedPointerDynamicCast<T>(d->calculationObjects.value(id));
        if (obj.isNull())
        {
            throw VExceptionBadId(tr("Can't cast object"), id);
        }
        *obj = *point;
    }
    else if (d->modelingObjects->contains(id) && point->getMode() == Draw::Modeling)
    {
        QSharedPointer<T> obj = qSharedPointerDynamicCast<T>(d->modelingObjects->value(id));
        if (obj.isNull())
        {
            throw VExceptionBadId(tr("Can't cast object"), id);
        }
        *obj = *point;
    }
    else if (point->getMode() == Draw::Calculation)
    {
        d->calculationObjects.insert(id, point);
    }
    else if (point->getMode() == Draw::Modeling)
    {
        d->modelingObjects->insert(id, point);
    }
    else
    {
        qWarning("Can't update an object with mode 'Layout'");
        return;
    }

    UpdateId(id);
}
#endif // VCONTAINER_H
