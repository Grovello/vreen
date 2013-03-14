/****************************************************************************
**
** Vreen - vk.com API Qt bindings
**
** Copyright © 2013 Aleksey Sidorov <gorthauer87@ya.ru>
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
#ifndef VREEN_BUDDYMANAGER_H
#define VREEN_BUDDYMANAGER_H

#include "contact.h"
#include "message.h"
#include "reply.h"
#include "friendrequest.h"

namespace Vreen {

class BuddyManagerPrivate;

class VK_SHARED_EXPORT BuddyManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(BuddyManager)
    Q_FLAGS(FriendRequestFlags)
public:
    enum FriendRequestFlag {
        NeedMutualFriends,
        NeedMessages,
        GetOutRequests
    };
    Q_DECLARE_FLAGS(FriendRequestFlags, FriendRequestFlag)

    explicit BuddyManager(Vreen::Client *client);
    ~BuddyManager();

    Buddy *buddy(int id);
    Buddy *buddy(int id) const;
    BuddyList buddies() const;
    QStringList tags() const;
    void setTags(const QStringList &list);
public slots:
    Reply *update(const IdList &ids, const QStringList &fields = QStringList()
            << VK_ALL_FIELDS
            );
    Reply *update(const BuddyList &buddies, const QStringList &fields = QStringList()
            << VK_ALL_FIELDS
            );
    ReplyBase<FriendRequestList> *getFriendRequests(int count = 100, int offset = 0, FriendRequestFlags flags = NeedMessages);
signals:
    void tagsChanged(const QStringList &);
    void buddyCreated(Vreen::Buddy*);
protected:
    QScopedPointer<BuddyManagerPrivate> d_ptr;

    friend class Buddy;

    Q_PRIVATE_SLOT(d_func(), void _q_tags_received(QVariant))
    Q_PRIVATE_SLOT(d_func(), void _q_status_changed(int userId, Vreen::Contact::Status status))
    Q_PRIVATE_SLOT(d_func(), void _q_online_changed(bool))
    Q_PRIVATE_SLOT(d_func(), void _q_updater_handle())
    Q_PRIVATE_SLOT(d_func(), void _q_update_finished(QVariant))
};

} // namespace Vreen

Q_DECLARE_METATYPE(Vreen::BuddyManager*)
Q_DECLARE_OPERATORS_FOR_FLAGS(Vreen::BuddyManager::FriendRequestFlags)

#endif // VREEN_BUDDYMANAGER_H
