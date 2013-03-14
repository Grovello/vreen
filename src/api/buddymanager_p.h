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
#ifndef BUDDYMANAGER_P_H
#define BUDDYMANAGER_P_H

#include "buddymanager.h"
#include "client.h"
#include <QTimer>

namespace Vreen {

typedef QHash<int, Buddy*> BuddyHash;

class BuddyManager;
class BuddyManagerPrivate
{
    Q_DECLARE_PUBLIC(BuddyManager)
public:
    BuddyManagerPrivate(BuddyManager *q, Client *client) : q_ptr(q), client(client)
    {
        updaterTimer.setInterval(5000);
        updaterTimer.setSingleShot(true);
        updaterTimer.connect(&updaterTimer, SIGNAL(timeout()),
                             q, SLOT(_q_updater_handle()));
    }
    BuddyManager *q_ptr;
    Client *client;

    BuddyHash buddyHash;
    Buddy *owner;
    QStringList tags;

    //TODO i want to use Qt5 slots
    //class Updater {
    //public:
    //    typedef std::function<void (Client *client, const IdList &idList, const QVariant &query)> Handler;

    //    Updater(Client *client, const QVariantMap &query, const Handler &handler) :
    //        client(client),
    //        query(query),
    //        handler(handler)
    //    {
    //        timer.setInterval(5000);
    //        timer.setSingleShot(true);
    //        QObject::connect(&timer, &timeout, this, &handle);
    //    }
    //    inline void handle() {
    //        if (queue.count()) {
    //            handler(client.data(), queue, query);
    //            queue.clear();
    //        }
    //    }
    //    inline void append(const IdList &items) {
    //        queue.append(items);
    //        if (!timer.isActive()) {
    //            timer.start();
    //        }
    //    }
    //protected:
    //    QPointer<Client> client;
    //    QVariantMap query;
    //    IdList queue;
    //    QTimer timer;
    //    Handler handler;
    //} updater;

    //updater
    QTimer updaterTimer;
    IdList updaterQueue;

    void getTags();
    void getOnline();
    void getFriends(const QVariantMap &args = QVariantMap());
    void addBuddy(Buddy *contact);
    void appendToUpdaterQueue(Buddy *contact);

    static QVariant handleGetRequests(const QVariant &response);

    void _q_tags_received(const QVariant &response);
    void _q_friends_received(const QVariant &response);
    void _q_status_changed(int userId, Vreen::Contact::Status status);
    void _q_online_changed(bool);
    void _q_updater_handle();
    void _q_update_finished(const QVariant &response);
};

} //namespace Vreen

#endif // BUDDYMANAGER_P_H
