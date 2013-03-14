/****************************************************************************
**
** Vreen - vk.com API Qt bindings
**
** Copyright © 2012 Aleksey Sidorov <gorthauer87@ya.ru>
**
*****************************************************************************
**
** $VREEN_BEGIN_LICENSE$
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see http://www.gnu.org/licenses/.
** $VREEN_END_LICENSE$
**
****************************************************************************/
#include "buddymodel.h"
#include <contact.h>
#include <utils.h>
#include <roster.h>
#include <QApplication>

#include <QDebug>

BuddyModel::BuddyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

void BuddyModel::setFilterByName(const QString &filter)
{
    m_filterByName = filter;
    emit filterByNameChanged(filter);
    setFilterFixedString(m_filterByName);
}

QString BuddyModel::filterByName()
{
    return m_filterByName;
}

Vreen::Buddy *BuddyModel::owner() const
{
    return m_owner.data();
}

void BuddyModel::setOwner(Vreen::Buddy *owner)
{
    if (m_owner != owner) {
        if (sourceModel())
            sourceModel()->deleteLater();
        if (owner) {
            auto roster = new Vreen::Roster(owner);
            setSourceModel(roster);
        }
        emit ownerChanged(owner);
    }
}

void BuddyModel::sync()
{
    auto roster = qobject_cast<Vreen::Roster*>(sourceModel());
    if (roster)
        roster->sync();
}
