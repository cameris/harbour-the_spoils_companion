/*
 * Copyright (C) 2014 cameris
 *
 * This file is part of The Spoils Companion.
 *
 * The Spoils Companion is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Spoils Companion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with The Spoils Companion.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COUNTCARDMODEL_H
#define COUNTCARDMODEL_H

#include "cardmodel.h"

class CountCardModel : public CardModel
{
    Q_OBJECT

    Q_PROPERTY(int totalCount READ totalCount NOTIFY cardCountChanged)
public:
    explicit CountCardModel(QObject *parent = 0);

    enum CardRoles {
        countRole = LastCardRole +1
    };

    // QAbstractItemModel interface
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void addCard(const QString &octgnId, const int count = 1);
    Q_INVOKABLE void removeCard(const QString &octgnId, const int count = 1);

    int totalCount();

signals:
    void cardCountChanged();

protected:
    QHash<QString, int> _count;

    virtual void dbUpdateCount(const QString &octgnId) = 0;
    virtual void dbInsertCard(const QString &octgnId) = 0;
    virtual void dbDeleteCard(const QString &octgnId) = 0;
    virtual void initCards() = 0;

};

#endif // COUNTCARDMODEL_H
