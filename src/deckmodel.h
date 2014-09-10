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

#ifndef DECKMODEL_H
#define DECKMODEL_H

#include <QAbstractListModel>

#include "deck.h"

class DeckModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
public:
    explicit DeckModel(QObject *parent = 0);

    enum DeckRoles {
        idRole = Qt::UserRole +1,
        nameRole,
        totalCountRole,
        tradeCountsRole,
    };

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void addDeck(const QString &name);
    Q_INVOKABLE void removeDeck(const int row);
    Q_INVOKABLE Deck* getDeck(const int row);
    Q_INVOKABLE void exportDeckOCTGN(const int row);

    Q_INVOKABLE void updateCounts(const int row);

signals:
    void countChanged();

private:
    QList<Deck *> _decks;

    void insertDecks(const int row, const QList<Deck *> ids);
};

#endif // DECKMODEL_H
