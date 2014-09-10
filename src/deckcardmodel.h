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

#ifndef DECKCARDMODEL_H
#define DECKCARDMODEL_H

#include "countcardmodel.h"

class DeckCardModel : public CountCardModel
{
    Q_OBJECT

    Q_PROPERTY(uint deckId READ deckId WRITE setDeckId NOTIFY deckIdChanged)
public:
    explicit DeckCardModel(QObject *parent = 0);

    uint deckId() const;
    void setDeckId(const uint deckId);

signals:
    void deckIdChanged();

private:
    uint _deckId;

    void dbUpdateCount(const QString &octgnId);
    void dbInsertCard(const QString &octgnId);
    void dbDeleteCard(const QString &octgnId);
    void initCards();
};

#endif // DECKCARDMODEL_H
