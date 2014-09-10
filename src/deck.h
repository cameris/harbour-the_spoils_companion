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

#ifndef DECK_H
#define DECK_H

#include <QObject>
#include <QVariant>

class Deck : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    Q_PROPERTY(int totalCount READ totalCount NOTIFY cardCountChanged)
    Q_PROPERTY(QVariantList tradeCounts READ tradeCounts NOTIFY cardCountChanged)
    Q_PROPERTY(QVariantList typeCounts READ typeCounts NOTIFY cardCountChanged)
    Q_PROPERTY(QVariantList setCounts READ setCounts NOTIFY cardCountChanged)
    Q_PROPERTY(bool buildable READ buildable NOTIFY cardCountChanged)
public:
    explicit Deck(QObject *parent = 0);

    uint id() const;
    void setId(const uint &id);

    QString name() const;
    void setName(const QString &name);

    int totalCount();
    QVariantList tradeCounts();
    QVariantList typeCounts();
    QVariantList setCounts();
    bool buildable();

    void exportOCTGN();

signals:
    void idChanged();
    void nameChanged();
    void cardCountChanged();

private:
    uint _id;
    QString _name;
};

#endif // DECK_H
