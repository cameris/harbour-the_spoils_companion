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

#include "deckmodel.h"

#include "dbmanager.h"

DeckModel::DeckModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QSqlQuery query = DBManager::instance()->prepare(
                "SELECT id, name FROM decks ORDER BY name;", DBManager::userDB);
    DBManager::instance()->execute(query);

    QList<Deck *> decks;
    while (query.next())
    {
        Deck *tmp = new Deck(this);
        tmp->setId(query.value(0).toUInt());
        tmp->setName(query.value(1).toString());
        decks.append(tmp);
    }

    insertDecks(0, decks);
}

int DeckModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _decks.size();
}

QVariant DeckModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    Deck *deck = _decks.at(index.row());

    switch (role)
    {
    case idRole:
        return deck->id();
    case nameRole:
        return deck->name();
    case totalCountRole:
        return deck->totalCount();
    case tradeCountsRole:
        return deck->tradeCounts();
    }

    return QVariant();
}

bool DeckModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() >= rowCount())
        return false;

    Deck *deck = _decks.at(index.row());

    if (role == nameRole)
    {
        QString name = value.toString();
        deck->setName(name);

        QSqlQuery query = DBManager::instance()->prepare(
                    "UPDATE decks SET name = ? WHERE id = ?;", DBManager::userDB);
        query.bindValue(0, value.toString());
        query.bindValue(1, deck->id());
        DBManager::instance()->execute(query);

        QVector<int> roles;
        roles << role;
        emit dataChanged(index, index, roles);

        return true;
    }

    return false;
}

QHash<int, QByteArray> DeckModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[idRole] = "id";
    roles[nameRole] = "name";
    roles[totalCountRole] = "totalCount";
    roles[tradeCountsRole] = "tradeCounts";

    return roles;
}

void DeckModel::addDeck(const QString &name)
{
    QSqlQuery query = DBManager::instance()->prepare(
                "INSERT INTO decks (name) VALUES (?);", DBManager::userDB);
    query.bindValue(0, name);
    DBManager::instance()->execute(query);

    QVariant lastId = query.lastInsertId();
    if (!lastId.isValid())
    {
        // TODO rollback?
    }

    Deck *tmp = new Deck();
    tmp->setId(lastId.toUInt());
    tmp->setName(name);

    QList<Deck *> decks;
    decks.append(tmp);

    insertDecks(rowCount(), decks);
}

void DeckModel::removeDeck(const int row)
{
    if (row < 0 || row >= rowCount())
        return;

    QSqlQuery query = DBManager::instance()->prepare(
                "DELETE FROM decks WHERE id = ?;", DBManager::userDB);
    query.bindValue(0, _decks.at(row)->id());
    DBManager::instance()->execute(query);

    beginRemoveRows(QModelIndex(), row, row);
    delete _decks.at(row);
    _decks.removeAt(row);
    endRemoveRows();
}

Deck *DeckModel::getDeck(const int row)
{
    if (row < 0 || row >= rowCount())
        return nullptr;

    return _decks.at(row);
}

void DeckModel::exportDeckOCTGN(const int row)
{
    if (row < 0 || row >= rowCount())
        return;

    _decks.at(row)->exportOCTGN();
}

void DeckModel::updateCounts(const int row)
{
    if (row < 0 || row >= rowCount())
        return;

    QModelIndex updateIndex = index(row, 0);

    QVector<int> roles;

    roles << totalCountRole;
    roles << tradeCountsRole;

    dataChanged(updateIndex, updateIndex, roles);
}

void DeckModel::insertDecks(const int row, const QList<Deck *> decks)
{
    if (row < 0 || row > rowCount())
        return;
    if (decks.size() == 0)
        return;

    beginInsertRows(QModelIndex(), row, row + decks.size()-1);
    for (int i = 0; i < decks.size(); ++i)
    {
        _decks.insert(i + row, decks.at(i));
    }
    endInsertRows();
    emit countChanged();
}
