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
#ifndef VK_ROSTER_H
#define VK_ROSTER_H

#include "contact.h"
#include "message.h"
#include "reply.h"
#include "friendrequest.h"
#include <QVariant>
#include <QStringList>
#include <QAbstractListModel>

namespace Vreen {
class Client;

class RosterPrivate;
class VK_SHARED_EXPORT Roster : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Roster)
    Q_PROPERTY(Buddy* owner READ owner CONSTANT)
public:

    enum Roles {
        ContactRole = Qt::UserRole + 1
    };

    Roster(Buddy *owner);
    virtual ~Roster();

    Buddy *owner() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent) const;
    BuddyList buddies() const;
public slots:
    Reply *sync(const QStringList &fields = QStringList()
            << VK_COMMON_FIELDS
            );
signals:
    void ownerChanged(Vreen::Buddy *owner);
protected:
    QScopedPointer<RosterPrivate> d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_friends_received(const QVariant &response))
};

} // namespace Vreen

Q_DECLARE_METATYPE(Vreen::Roster*)

#endif // VK_ROSTER_H

