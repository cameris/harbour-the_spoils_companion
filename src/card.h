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

#ifndef CARD_H
#define CARD_H

#include <QObject>
#include <QStringList>

class Set : public QObject
{
    Q_OBJECT
public:
    explicit Set(QObject *parent = 0) : QObject(parent) {}

    QString octgnId;
    QString name;
    int releaseOrder;
};

class Trade : public QObject
{
    Q_OBJECT
public:
    explicit Trade(QObject *parent = 0) : QObject(parent) {}

    QString name;
};

class Type : public QObject
{
    Q_OBJECT
public:
    explicit Type(QObject *parent = 0) : QObject(parent) {}

    QString name;
};

class Rarity : public QObject
{
    Q_OBJECT
public:
    explicit Rarity(QObject *parent = 0) : QObject(parent) {}

    QString name;
    int sortOrder;
};

class Card : public QObject
{
    Q_OBJECT
public:
    explicit Card(QObject *parent = 0);

    QString id() const;
    void setId(const QString &id);

    QString name() const;
    void setName(const QString &name);

    int number() const;
    void setNumber(int number);

    Rarity* rarity() const;
    void setRarity(Rarity *rarity);

    Trade* trade() const;
    void setTrade(Trade *trade);

    Type* type() const;
    void setType(Type *type);

    QString subtype() const;
    void setSubtype(const QString &subtype);

    QStringList threshold() const;
    void setThreshold(const QStringList &threshold);
    int thresholdCount() const;

    int cost() const;
    void setCost(int cost);

    int strength() const;
    void setStrength(int strength);

    int life() const;
    void setLife(int life);

    int speed() const;
    void setSpeed(int speed);

    int structure() const;
    void setStructure(int structure);

    QString text() const;
    void setText(const QString &text);

    QString flavor() const;
    void setFlavor(const QString &flavor);

    Set* set() const;
    void setSet(Set *set);

private:
    QString _id;
    QString _name;
    int _number;
    Rarity* _rarity;
    Trade* _trade;
    Type* _type;
    QString _subtype;
    QStringList _threshold;
    int _cost;
    int _strength;
    int _life;
    int _speed;
    int _structure;
    QString _text;
    QString _flavor;
    Set* _set;
};

#endif // CARD_H
