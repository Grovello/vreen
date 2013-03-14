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
#include "buddymanager_p.h"
#include "longpoll.h"
#include "utils_p.h"

namespace Vreen {

BuddyManager::BuddyManager(Vreen::Client *client) :
    QObject(client),
    d_ptr(new  BuddyManagerPrivate(this, client))
{
    Q_D(BuddyManager);
    connect(d->client->longPoll(), SIGNAL(contactStatusChanged(int, Vreen::Contact::Status)),
            this, SLOT(_q_status_changed(int, Vreen::Contact::Status)));
    connect(d->client, SIGNAL(onlineStateChanged(bool)), SLOT(_q_online_changed(bool)));
}

BuddyManager::~BuddyManager()
{

}

Buddy *BuddyManager::buddy(int id)
{
    Q_D(BuddyManager);
    if (!id) {
        qWarning("Contact's id cannot be null!");
        return 0;
    }
    if (id < 0) {
        id = qAbs(id);
        qWarning("For groups use class GroupManager");
    }


    auto buddy = d->buddyHash.value(id);
    if (!buddy) {
        if (d->owner && d->owner->id() == id)
            return d->owner;
        buddy = new Buddy(id, d->client);
        d->addBuddy(buddy);
    }
    return buddy;
}

Buddy *BuddyManager::buddy(int id) const
{
    return d_func()->buddyHash.value(id);
}

BuddyList BuddyManager::buddies() const
{
    return d_func()->buddyHash.values();
}

QStringList BuddyManager::tags() const
{
    return d_func()->tags;
}

void BuddyManager::setTags(const QStringList &tags)
{
    d_func()->tags = tags;
    //TODO rebuild tags
    emit tagsChanged(tags);
}

/*!
 * \brief Roster::update
 * \param ids
 * \param fields from \link http://vk.com/developers.php?oid=-1&p=Описание_полей_параметра_fields
 */
Reply *BuddyManager::update(const IdList &ids, const QStringList &fields)
{
    Q_D(BuddyManager);
    QVariantMap args;
    args.insert("uids", join(ids));
    args.insert("fields", fields.join(","));
    auto reply = d->client->request("users.get", args);
    reply->connect(reply, SIGNAL(resultReady(const QVariant&)),
                   this, SLOT(_q_update_finished(const QVariant&)));
    return reply;
}

Reply *BuddyManager::update(const BuddyList &buddies, const QStringList &fields)
{
    IdList ids;
    foreach (auto buddy, buddies)
        ids.append(buddy->id());
    return update(ids, fields);
}

ReplyBase<FriendRequestList> *BuddyManager::getFriendRequests(int count, int offset, FriendRequestFlags flags)
{
    Q_D(BuddyManager);
    QVariantMap args;
    args.insert("count", count);
    args.insert("offset", offset);
    if (flags & NeedMutualFriends)
        args.insert("need_mutual", true);
    if (flags & NeedMessages)
        args.insert("need_messages", true);
    if (flags & GetOutRequests)
        args.insert("out", 1);
    auto reply = d->client->request<ReplyBase<FriendRequestList>>("friends.getRequests",
                                                                  args,
                                                                  BuddyManagerPrivate::handleGetRequests);
    return reply;
}

void BuddyManagerPrivate::getTags()
{
    Q_Q(BuddyManager);
    auto reply = client->request("friends.getLists");
    reply->connect(reply, SIGNAL(resultReady(const QVariant&)),
                   q, SLOT(_q_tags_received(const QVariant&)));
}

void BuddyManagerPrivate::getOnline()
{
}

void BuddyManagerPrivate::addBuddy(Buddy *buddy)
{
    Q_Q(BuddyManager);
    buddyHash.insert(buddy->id(), buddy);
    emit q->buddyCreated(buddy);
}

void BuddyManagerPrivate::appendToUpdaterQueue(Buddy *contact)
{
    if (!updaterQueue.contains(contact->id()))
        updaterQueue.append(contact->id());
    if (!updaterTimer.isActive())
        updaterTimer.start();
}

QVariant BuddyManagerPrivate::handleGetRequests(const QVariant &response)
{
    FriendRequestList list;
    foreach (auto item, response.toList()) {
        QVariantMap map = item.toMap();
        FriendRequest request(map.value("uid").toInt());

        request.setMessage(map.value("message").toString());
        IdList ids;
        QVariantMap mutuals = map.value("mutual").toMap();
        foreach (auto user, mutuals.value("users").toList())
            ids.append(user.toInt());
        request.setMutualFriends(ids);
        list.append(request);
    }
    return QVariant::fromValue(list);
}

void BuddyManagerPrivate::_q_tags_received(const QVariant &response)
{
    Q_Q(BuddyManager);
    auto list = response.toList();
    QStringList tags;
    foreach (auto item, list) {
        tags.append(item.toMap().value("name").toString());
    }
    q->setTags(tags);
}

void BuddyManagerPrivate::_q_status_changed(int userId, Buddy::Status status)
{
    Q_Q(BuddyManager);
    auto buddy = q->buddy(userId);
    buddy->setStatus(status);
}

void BuddyManagerPrivate::_q_online_changed(bool set)
{
    if (set)
        getTags();
    else {
        foreach(auto buddy, buddyHash)
            buddy->setOnline(false);
    }
}

void BuddyManagerPrivate::_q_updater_handle()
{
    Q_Q(BuddyManager);
    q->update(updaterQueue);
    updaterQueue.clear();
}

void BuddyManagerPrivate::_q_update_finished(const QVariant &response)
{
    Q_Q(BuddyManager);
    auto list = response.toList();
    foreach (auto data, list) {
        auto map = data.toMap();
        int id = map.value("uid").toInt();
        auto buddy = q->buddy(id);
        Contact::fill(buddy, map);
    }
}

} // namespace Vreen

#include "moc_buddymanager.cpp"
