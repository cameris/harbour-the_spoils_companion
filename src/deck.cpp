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

#include "deck.h"

#include <QXmlStreamWriter>
#include <QStandardPaths>
#include <QFile>
#include "dbmanager.h"

Deck::Deck(QObject *parent) :
    QObject(parent)
{
}

uint Deck::id() const
{
    return _id;
}

void Deck::setId(const uint &id)
{
    if (id == _id)
        return;

    _id = id;
}

QString Deck::name() const
{
    return _name;
}

void Deck::setName(const QString &name)
{
    if (name == _name)
        return;

    _name = name;
    emit nameChanged();
}

int Deck::totalCount()
{
    QSqlQuery query = DBManager::instance()->prepare(
                "SELECT SUM(count) FROM deckCardcounts WHERE deckId = ?;", DBManager::userDB);
    query.bindValue(0, _id);
    DBManager::instance()->execute(query);

    query.next();
    return query.value(0).toInt();
}

QVariantList Deck::tradeCounts()
{
    QSqlQuery query;
    DBManager *db = DBManager::instance();

    query = db->prepare(
                "SELECT card.trade, SUM(deck.count) AS tradecount FROM ("
                "  SELECT octgnId, count FROM deckCardcounts WHERE deckId = ?"
                " ) AS deck"
                " LEFT JOIN ("
                "  SELECT cards.octgnId, trades.name AS trade FROM cardDB.cards"
                "   LEFT JOIN cardDB.trades ON cards.tradeId = trades.id"
                "   ) AS card"
                " ON deck.octgnId = card.octgnId"
                " GROUP BY card.trade"
                " ORDER BY tradecount DESC, card.trade ASC;", DBManager::userDB);
    query.bindValue(0, _id);
    db->execute(query);

    QVariantList tradeCounts;
    while (query.next())
    {
        QVariantMap tmp;
        int count = query.value(1).toInt();

        if (count <= 0)
            continue;

        tmp.insert("name", query.value(0));
        tmp.insert("count", count);
        tradeCounts.append(tmp);
    }
    return tradeCounts;
}

QVariantList Deck::typeCounts()
{
    QSqlQuery query;
    DBManager *db = DBManager::instance();

    query = db->prepare(
                "SELECT card.type, SUM(deck.count) AS typecount FROM ("
                "  SELECT octgnId, count FROM deckCardcounts WHERE deckId = ?"
                " ) AS deck"
                " LEFT JOIN ("
                "  SELECT cards.octgnId, types.name AS type FROM cardDB.cards"
                "   LEFT JOIN cardDB.types ON cards.typeId = types.id"
                "   ) AS card"
                " ON deck.octgnId = card.octgnId"
                " GROUP BY card.type"
                " ORDER BY typecount DESC, card.type ASC;", DBManager::userDB);
    query.bindValue(0, _id);
    db->execute(query);

    QVariantList typeCounts;
    while (query.next())
    {
        QVariantMap tmp;
        int count = query.value(1).toInt();

        if (count <= 0)
            continue;

        tmp.insert("name", query.value(0));
        tmp.insert("count", count);
        typeCounts.append(tmp);
    }
    return typeCounts;
}

QVariantList Deck::setCounts()
{
    QSqlQuery query;
    DBManager *db = DBManager::instance();

    query = db->prepare(
                "SELECT card.setname, SUM(deck.count) AS setcount FROM ("
                "  SELECT octgnId, count FROM deckCardcounts WHERE deckId = ?"
                " ) AS deck"
                " LEFT JOIN ("
                "  SELECT cards.octgnId, sets.name AS setname, sets.releaseOrder"
                "   FROM cardDB.cards"
                "   LEFT JOIN cardDB.sets ON cards.setId = sets.id"
                "   ) AS card"
                " ON deck.octgnId = card.octgnId"
                " GROUP BY card.setname"
                " ORDER BY setcount DESC, card.releaseOrder ASC;", DBManager::userDB);
    query.bindValue(0, _id);
    db->execute(query);

    QVariantList setCounts;
    while (query.next())
    {
        QVariantMap tmp;
        int count = query.value(1).toInt();

        if (count <= 0)
            continue;

        tmp.insert("name", query.value(0));
        tmp.insert("count", count);
        setCounts.append(tmp);
    }
    return setCounts;
}

#include <QDebug>
bool Deck::buildable()
{
    QSqlQuery query;
    DBManager *db = DBManager::instance();

    query = db->prepare(
                "SELECT deck.octgnId, (collection.count - deck.count) AS diff FROM ("
                "  SELECT octgnId, count FROM deckCardcounts WHERE deckId = ?"
                " ) AS deck"
                " LEFT OUTER JOIN collection ON deck.octgnId = collection.octgnId"
                " WHERE diff < 0 OR diff IS NULL;", DBManager::userDB);
    query.bindValue(0, _id);
    db->execute(query);

    return !query.next();
}

void Deck::exportOCTGN()
{
    QSqlQuery query;
    DBManager *db = DBManager::instance();

    QString filename = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
            "/" + _name + ".o8d";

    QFile file(filename);

    file.open(QFile::WriteOnly);

    QXmlStreamWriter wr(&file);
    wr.setAutoFormatting(true);
    wr.writeStartDocument();

    wr.writeStartElement("deck");
    wr.writeAttribute("game", SPOILS_OCTGNID);

    wr.writeStartElement("section");
    wr.writeAttribute("name", "Game Deck");
    wr.writeAttribute("shared", "False");

    query = db->prepare(
                "SELECT deckCards.octgnId, count, cards.name FROM deckCards"
                " LEFT JOIN cardDB.cards ON deckCards.octgnId = cards.octgnId"
                " WHERE deckCards.deckId = ?;", DBManager::userDB);
    query.bindValue(0, _id);
    db->execute(query);

    while (query.next())
    {
        QString octgnId = query.value(0).toString();
        QString count = query.value(1).toString();
        QString name = query.value(2).toString();

        wr.writeStartElement("card");
        wr.writeAttribute("qty", count);
        wr.writeAttribute("id", octgnId);
        wr.writeCharacters(name);
        wr.writeEndElement();
    }

    wr.writeEndElement(); // section

    wr.writeStartElement("section");
    wr.writeAttribute("name", "Faction");
    wr.writeAttribute("shared", "False");

    query = db->prepare(
                "SELECT deckFactions.octgnId, count, cards.name FROM deckFactions"
                " LEFT JOIN cardDB.cards ON deckFactions.octgnId = cards.octgnId"
                " WHERE deckFactions.deckId = ?;", DBManager::userDB);
    query.bindValue(0, _id);
    db->execute(query);

    while (query.next())
    {
        QString octgnId = query.value(0).toString();
        QString count = query.value(1).toString();
        QString name = query.value(2).toString();

        wr.writeStartElement("card");
        wr.writeAttribute("qty", count);
        wr.writeAttribute("id", octgnId);
        wr.writeCharacters(name);
        wr.writeEndElement();
    }

    wr.writeEndElement(); // section
    wr.writeEndElement(); // deck

    wr.writeEndDocument();

    file.close();
}
