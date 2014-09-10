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

#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlResult>

class DBManager
{
public:
    static DBManager *instance();

    enum databases {
        cardDB,
        userDB
    };

    QSqlQuery prepare(const QString &statement, int db = cardDB);
    bool execute(QSqlQuery &query);

private:
    explicit DBManager();

    QSqlDatabase _cardDB;
    QSqlDatabase _userDB;
};

#endif // DBMANAGER_H
