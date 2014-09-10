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

#include "countcardmodel.h"

CountCardModel::CountCardModel(QObject *parent) :
    CardModel(parent)
{
    connect(this, &CountCardModel::countChanged, this, &CountCardModel::cardCountChanged);
}

QVariant CountCardModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    if (role > LastCardRole)
    {
        switch (role)
        {
        case countRole:
            return _count.value(_cards.at(index.row())->id());
        }
    }

    return CardModel::data(index, role);
}

bool CountCardModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() >= rowCount())
        return false;

    if (role > LastCardRole)
    {
        Card *card = _cards.at(index.row());
        if (role == countRole)
        {
            int newCount = value.toInt();
            int currentCount = _count.value(card->id());

            if (newCount == currentCount)
                return true;
            if (newCount < 1)
                return false;

            _count.insert(card->id(), newCount);
            dbUpdateCount(card->id());

            QVector<int> roles;
            roles << role;
            emit dataChanged(index, index, roles);
            emit cardCountChanged();

            return true;
        }
    }

    return false;
}

QHash<int, QByteArray> CountCardModel::roleNames() const
{
    QHash<int, QByteArray> roles = CardModel::roleNames();
    roles[countRole] = "count";
    return roles;
}

void CountCardModel::addCard(const QString &octgnId, const int count)
{
    if (_count.contains(octgnId))
    {
        int newCount = _count.value(octgnId) + count;
        _count.insert(octgnId, newCount);
        dbUpdateCount(octgnId);
        for (int i = 0; i < _cards.size(); ++i)
        {
            if (_cards.at(i)->id() == octgnId)
            {
                QVector<int> roles;
                QModelIndex tmp = QAbstractListModel::index(i);
                roles << countRole;

                emit dataChanged(tmp, tmp, roles);
                emit cardCountChanged();
                break;
            }
        }
    }
    else
    {
        _count.insert(octgnId, count);
        insertCards(_cards.size(), loadCards(QStringList(octgnId)));
        dbInsertCard(octgnId);
    }
}

void CountCardModel::removeCard(const QString &octgnId, const int count)
{
    if (_count.contains(octgnId))
    {
        int newCount = _count.value(octgnId, 0) - count;
        if (newCount >= 1)
        {
            _count.insert(octgnId, newCount);
            dbUpdateCount(octgnId);

            int i;
            for (i = 0; i < _cards.size(); ++i)
            {
                if (_cards.at(i)->id() == octgnId)
                {
                    break;
                }
            }

            QVector<int> roles;
            QModelIndex tmp = QAbstractListModel::index(i);
            roles << countRole;

            emit dataChanged(tmp, tmp, roles);
            emit cardCountChanged();
        }
        else
        {
            int i;
            for (i = 0; i < _cards.size(); ++i)
            {
                if (_cards.at(i)->id() == octgnId)
                {
                    break;
                }
            }

            beginRemoveRows(QModelIndex(), i, i);
            _cards.removeAt(i);
            _count.remove(octgnId);
            endRemoveRows();
            emit countChanged();

            dbDeleteCard(octgnId);
        }
    }
}

int CountCardModel::totalCount()
{
    int total = 0;
    for (int i = 0; i < _cards.size(); ++i) {
        total += _count.value(_cards.at(i)->id(), 0);
    }

    return total;
}
