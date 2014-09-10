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

#include "card.h"

Card::Card(QObject *parent) :
    QObject(parent)
{
}
QString Card::id() const
{
    return _id;
}

void Card::setId(const QString &id)
{
    _id = id;
}
QString Card::name() const
{
    return _name;
}

void Card::setName(const QString &name)
{
    _name = name;
}
int Card::number() const
{
    return _number;
}

void Card::setNumber(int number)
{
    _number = number;
}
Rarity *Card::rarity() const
{
    return _rarity;
}

void Card::setRarity(Rarity* rarity)
{
    _rarity = rarity;
}
Trade* Card::trade() const
{
    return _trade;
}

void Card::setTrade(Trade* trade)
{
    _trade = trade;
}
Type* Card::type() const
{
    return _type;
}

void Card::setType(Type* type)
{
    _type = type;
}
QString Card::subtype() const
{
    return _subtype;
}

void Card::setSubtype(const QString &subtype)
{
    _subtype = subtype;
}
QStringList Card::threshold() const
{
    return _threshold;
}

void Card::setThreshold(const QStringList &threshold)
{
    _threshold = threshold;
}

int Card::thresholdCount() const
{
    return _threshold.count();
}
int Card::cost() const
{
    return _cost;
}

void Card::setCost(int cost)
{
    _cost = cost;
}
int Card::strength() const
{
    return _strength;
}

void Card::setStrength(int strength)
{
    _strength = strength;
}
int Card::life() const
{
    return _life;
}

void Card::setLife(int life)
{
    _life = life;
}
int Card::speed() const
{
    return _speed;
}

void Card::setSpeed(int speed)
{
    _speed = speed;
}
int Card::structure() const
{
    return _structure;
}

void Card::setStructure(int structure)
{
    _structure = structure;
}
QString Card::text() const
{
    return _text;
}

void Card::setText(const QString &text)
{
    _text = text;
}
QString Card::flavor() const
{
    return _flavor;
}

void Card::setFlavor(const QString &flavor)
{
    _flavor = flavor;
}
Set *Card::set() const
{
    return _set;
}

void Card::setSet(Set* set)
{
    _set = set;
}
