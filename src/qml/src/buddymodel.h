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
#ifndef CONTACTSMODEL_H
#define CONTACTSMODEL_H

#include <QSortFilterProxyModel>
#include <roster.h>
#include <utils.h>
#include <QPointer>

class BuddyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(Vreen::Buddy* owner READ owner WRITE setOwner NOTIFY ownerChanged)
    Q_PROPERTY(QString filterByName READ filterByName WRITE setFilterByName NOTIFY filterByNameChanged)
public:
    explicit BuddyModel(QObject *parent = 0);
    
    int count() const;
    void setFilterByName(const QString &filter);
    QString filterByName();
    Vreen::Buddy *owner() const;
    void setOwner(Vreen::Buddy* owner);
public slots:
    void sync();
signals:
    void filterByNameChanged(const QString &filter);
    void ownerChanged(Vreen::Buddy* arg);

private:
    QPointer<Vreen::Buddy> m_owner;
    QString m_filterByName;
};

#endif // CONTACTSMODEL_H

