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

#include "deckcardmodel.h"

#include "dbmanager.h"

DeckCardModel::DeckCardModel(QObject *parent) :
    CountCardModel(parent)
{
}

uint DeckCardModel::deckId() const
{
    return _deckId;
}

void DeckCardModel::setDeckId(const uint deckId)
{
    if (deckId == _deckId)
        return;

    _deckId = deckId;
    emit deckIdChanged();
}

void DeckCardModel::dbUpdateCount(const QString &octgnId)
{
    QSqlQuery query = DBManager::instance()->prepare(
                "UPDATE deckCards SET count = ? WHERE deckId = ? AND octgnId = ?;", DBManager::userDB);
    query.bindValue(0, _count.value(octgnId, 1));
    query.bindValue(1, _deckId);
    query.bindValue(2, octgnId);
    DBManager::instance()->execute(query);
}

void DeckCardModel::dbInsertCard(const QString &octgnId)
{
    QSqlQuery query = DBManager::instance()->prepare(
                "INSERT INTO deckCards (deckId, octgnId, count) VALUES (?, ?, ?)", DBManager::userDB);
    query.bindValue(0, _deckId);
    query.bindValue(1, octgnId);
    query.bindValue(2, _count.value(octgnId, 1));
    DBManager::instance()->execute(query);
}

void DeckCardModel::dbDeleteCard(const QString &octgnId)
{
    QSqlQuery query = DBManager::instance()->prepare(
                "DELETE FROM deckCards WHERE deckId = ? AND octgnId = ?;", DBManager::userDB);
    query.bindValue(0, _deckId);
    query.bindValue(1, octgnId);
    DBManager::instance()->execute(query);
}

void DeckCardModel::initCards()
{
    QSqlQuery query = DBManager::instance()->prepare(
                "SELECT octgnId, count FROM deckCards WHERE deckId = ?;", DBManager::userDB);
    query.bindValue(0, _deckId);
    DBManager::instance()->execute(query);

    QStringList ids;
    while (query.next())
    {
        QString id = query.value(0).toString();
        _count.insert(id, query.value(1).toInt());
        ids.append(id);
    }

    insertCards(0, loadCards(ids));
}
