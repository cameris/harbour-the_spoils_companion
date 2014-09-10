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

#ifndef CARDMODEL_H
#define CARDMODEL_H

#include <QAbstractListModel>
#include <QQmlParserStatus>

#include "card.h"

class CardModel : public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT

    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
public:
    explicit CardModel(QObject *parent = 0);

    enum CardRoles {
        idRole = Qt::UserRole +1,
        nameRole,
        numberRole,
        rarityRole,
        raritySortOrderRole,
        tradeRole,
        typeRole,
        subtypeRole,
        thresholdRole,
        thresholdCountRole,
        costRole,
        strengthRole,
        lifeRole,
        speedRole,
        structureRole,
        textRole,
        flavorRole,
        setRole,
        setReleaseOrderRole,
        imageRole,
        thumbnailRole,
        LastCardRole = thumbnailRole
    };

    // QQmlParserStatus interface
    void classBegin();
    void componentComplete();

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    QStringList sets() const;
    QStringList trades() const;
    QStringList types() const;
    QStringList rarities() const;

signals:
    void countChanged();

protected:
    QHash<uint, Set *> _sets;
    QHash<uint, Trade *> _trades;
    QHash<uint, Type *> _types;
    QHash<uint, Rarity *> _rarities;
    QList<Card *> _cards;
    QString _cardsPath;

    void init();
    virtual void initCards();
    QList<Card *> loadCards();
    QList<Card *> loadCards(QStringList octgnIds);
    void insertCards(const int row, const QList<Card *> cards);
};

#endif // CARDMODEL_H
