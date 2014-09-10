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

#include "dbmanager.h"

#include <QSqlError>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSqlDriver>

#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <sailfishapp.h>

#include <QDebug>

DBManager *DBManager::instance()
{
    static DBManager *dbManager;
    if (!dbManager)
    {
        dbManager = new DBManager();
    }
    return dbManager;
}

DBManager::DBManager()
{
    const QString cardDBFile = SailfishApp::pathTo("/cards.sqlite").toString(QUrl::RemoveScheme);
    QDir userDBDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    const QString userDBFile = "data.sqlite";

    _cardDB = QSqlDatabase::addDatabase("QSQLITE", "cardDB");
    _cardDB.setDatabaseName(cardDBFile);

    if (!_cardDB.open())
    {
        qWarning() << "Failed to open card database " << cardDBFile;
        return;
    }

    if (!userDBDir.mkpath(userDBDir.absolutePath()))
    {
        qWarning() << "Can't create directory " + userDBDir.absolutePath();
    }

    _userDB = QSqlDatabase::addDatabase("QSQLITE", "userDB");
    _userDB.setDatabaseName(userDBDir.absoluteFilePath(userDBFile));

    bool userDBCreated = QFileInfo(userDBDir.absoluteFilePath(userDBFile)).exists();

    if (!_userDB.open())
    {
        qWarning() << "Failed to open user database " << userDBDir.absoluteFilePath(userDBFile);
        return;
    }

    QSqlQuery query;
    query = prepare("PRAGMA foreign_keys = ON;", userDB);
    execute(query);

    query = prepare(QString("ATTACH DATABASE '%1' AS cardDB").arg(cardDBFile), userDB);
    execute(query);


    if (!userDBCreated)
    {
        query = prepare(
                    "CREATE TABLE collection ("
                    "   id INTEGER PRIMARY KEY,"
                    "   octgnId TEXT NOT NULL UNIQUE,"
                    "   count INTEGER"
                    "   );", userDB);
        execute(query);

        query = prepare(
                    "CREATE TABLE decks ("
                    "   id INTEGER PRIMARY KEY,"
                    "   name TEXT"
                    "   );", userDB);
        execute(query);

        query = prepare(
                    "CREATE TABLE deckCards ("
                    "   id INTEGER PRIMARY KEY,"
                    "   deckId INTEGER NOT NULL REFERENCES decks(id) ON DELETE CASCADE,"
                    "   octgnId TEXT NOT NULL,"
                    "   count INTEGER,"
                    "   UNIQUE(deckId, octgnId)"
                    "   );", userDB);
        execute(query);

        query = prepare(
                    "CREATE TABLE deckFactions ("
                    "   id INTEGER PRIMARY KEY,"
                    "   deckId INTEGER NOT NULL REFERENCES decks(id) ON DELETE CASCADE,"
                    "   octgnId TEXT NOT NULL,"
                    "   count INTEGER,"
                    "   UNIQUE(deckId, octgnId)"
                    "   );", userDB);
        execute(query);

        query = prepare(
                    "CREATE VIEW deckCardcounts AS"
                    " SELECT deckId, octgnId, SUM(count) AS count FROM ("
                    "   SELECT deckId, octgnId, count FROM deckCards"
                    "   UNION ALL"
                    "   SELECT deckId, octgnId, count FROM deckFactions"
                    "  )"
                    " GROUP BY deckId, octgnId;", userDB);
        execute(query);
    }
}

QSqlQuery DBManager::prepare(const QString &statement, int db)
{
    QSqlDatabase *dbp;
    if (db == userDB)
    {
        dbp = &_userDB;
    }
    else
    {
        dbp = &_cardDB;
    }

    QSqlQuery query(*dbp);
    query.setForwardOnly(true);
    if (!query.prepare(statement))
    {
        qWarning() << Q_FUNC_INFO << "failed to prepare query";
        qWarning() << query.lastQuery();
        qWarning() << query.lastError();
        return QSqlQuery();
    }
    return query;
}

bool DBManager::execute(QSqlQuery &query)
{
    if (!query.exec())
    {
        qWarning() << Q_FUNC_INFO << "failed to execute query";
        qWarning() << query.lastQuery();
        qWarning() << query.lastError();
        return false;
    }
    return true;
}
