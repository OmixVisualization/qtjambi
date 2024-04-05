/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qml_abstractobject.h"
#include "../reporthandler.h"

AbstractObject::AbstractObject(QObject *parent)
    : QObject{parent}
{

}

QVariant AbstractObject::getUntil() const
{
    return QVariant::fromValue<QVersionNumber>(m_until);
}

QVersionNumber toVersion(const QVariant &v, int fill){
    QVersionNumber version;
    if(v.userType()==QMetaType::Double){
            int d = qRound(v.value<double>()*10.);
            int major = d/10;
            int minor = (d-major*10);
            version = QVersionNumber(major, minor, fill);
    }else if(v.userType()==QMetaType::Int || v.userType()==QMetaType::UInt
        || v.userType()==QMetaType::Short || v.userType()==QMetaType::UShort
        || v.userType()==QMetaType::Char || v.userType()==QMetaType::UChar){
        version = QVersionNumber(v.value<int>(), fill, fill);
    }else if(v.userType()==QMetaType::fromType<QJSValue>().id()){
        QJSValue jsval = v.value<QJSValue>();
        if(jsval.isNumber()){
            version = QVersionNumber(jsval.toInt(), fill, fill);
        }else if(jsval.isArray()){
            QVector<int> ints;
            for(quint32 i=0, length = jsval.property("length").toUInt(); i<length; ++i){
                ints << jsval.property(i).toInt();
            }
            for(auto i=ints.size(); i<3; ++i){
                ints << fill;
            }
            version = QVersionNumber(std::move(ints));
        }
    }else if(v.userType()==QMetaType::fromType<QString>().id()){
        version = QVersionNumber::fromString(v.toString());
    }else if(v.userType()==QMetaType::fromType<QVersionNumber>().id()){
        version = v.value<QVersionNumber>();
    }else{
        ReportHandler::warning(QString("Cannot interpret version. Type is %1").arg(QLatin1String(QMetaType(v.userType()).name())));
    }
    return version;
}

void AbstractObject::setUntil(const QVariant &v)
{
    QVersionNumber version = toVersion(v, std::numeric_limits<int>::max());
//    qWarning("until %s = %s", qPrintable(v.toString()), qPrintable(version.toString()));
    if (m_until == version)
        return;
    m_until = version;
    emit untilChanged();
}

QVariant AbstractObject::getSince() const
{
    return QVariant::fromValue<QVersionNumber>(m_since);
}

void AbstractObject::setSince(const QVariant &v)
{
    QVersionNumber version = toVersion(v, 0);
//    qWarning("since %s = %s", qPrintable(v.toString()), qPrintable(version.toString()));
    if (m_since == version)
        return;
    m_since = version;
    emit sinceChanged();
}

const QString &AbstractObject::getUnless() const
{
    return unless;
}

void AbstractObject::setUnless(const QString &newUnless)
{
    if (unless == newUnless)
        return;
    unless = newUnless;
    emit unlessChanged();
}

QQmlListProperty<AbstractObject> AbstractObject::getChildren()
{
    return QQmlListProperty<AbstractObject>(this, &children);
}
