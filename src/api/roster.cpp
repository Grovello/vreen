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
#include "roster_p.h"
#include "longpoll.h"

#include <QDebug>
#include <QTimer>

namespace Vreen {

/*!
 * \brief The Roster class
 * Api reference: \link http://vk.com/developers.php?oid=-1&p=friends.get
 */

/*!
 * \brief Roster::Roster
 * \param client
 */
Roster::Roster(Buddy *owner) :
    QAbstractListModel(owner),
    d_ptr(new RosterPrivate(this, owner))
{
    auto roles = roleNames();
    roles[ContactRole] = "contact";
    setRoleNames(roles);
}

Roster::~Roster()
{

}

Buddy *Roster::owner() const
{
    return d_func()->owner;
}

QVariant Roster::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    switch (role) {
    case ContactRole: {
        auto contact = d_func()->buddies.at(row);
        return QVariant::fromValue(contact);
        break;
    }
    return QVariant();
    }
}

int Roster::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return d_func()->buddies.count();
}

BuddyList Roster::buddies() const
{
    return d_func()->buddies;
}

Reply *Roster::sync(const QStringList &fields)
{
    Q_D(Roster);
    //TODO rewrite with method chains with lambdas in Qt5
    QVariantMap args;
    args.insert("fields", fields.join(","));
    args.insert("order", "hints");
    auto reply = d->owner->client()->request("friends.get", args);
    reply->connect(reply, SIGNAL(resultReady(const QVariant&)),
                   this, SLOT(_q_friends_received(const QVariant&)));
    return reply;
}

void RosterPrivate::_q_friends_received(const QVariant &response)
{
    Q_Q(Roster);
    clear();
    auto list = response.toList();
    q->beginInsertRows(QModelIndex(), 0, list.count());
    foreach (auto data, list) {
        auto map = data.toMap();
        int id = map.value("uid").toInt();
        auto buddy = owner->client()->buddyManager()->buddy(id);
        Contact::fill(buddy, map);
        buddies.append(buddy);
    }
    q->endInsertRows();
}

void RosterPrivate::clear()
{
    Q_Q(Roster);
    q->beginResetModel();
    buddies.clear();
    q->endResetModel();
}

} // namespace Vreen

#include "moc_roster.cpp"

