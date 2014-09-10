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

#include "cardmodel.h"

#include <QSettings>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QImageReader>

#include <sailfishapp.h>

#include "dbmanager.h"

#define THUMB_WIDTH 180
#define THUMB_HEIGHT 252

CardModel::CardModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QSettings settings;
    _cardsPath = settings.value("cards/images").toString();
    init();
}

void CardModel::classBegin()
{
}

void CardModel::componentComplete()
{
    initCards();
}

int CardModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _cards.size();
}

QString imageString(Card *card)
{
//    return QString("844d5fe3-bdb5-4ad2-ba83-88c2c2db6d88/Sets/%1/Cards/%2.jpg")
//            .arg(card->set()->octgnId, card->id());
    return QString("%1/Sets/%2/Cards/%3.jpg")
            .arg(SPOILS_OCTGNID, card->set()->octgnId, card->id());
}

QString getImageFile(Card *card, QString cardsPath)
{
    QDir imagesDir(cardsPath);
    QString image = imageString(card);
    if (imagesDir.exists(image))
    {
        return imagesDir.absoluteFilePath(image);
    }
    return "";
}

QString getThumbnailFile(Card *card, QString cardsPath)
{
    QDir thumbsDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)+"/thumbnails");
    QString image = imageString(card);

    QFileInfo fi(thumbsDir.absoluteFilePath(image));
    if (!thumbsDir.exists(fi.absolutePath()))
    {
        thumbsDir.mkpath(fi.absolutePath());
    }

    if (thumbsDir.exists(image))
    {
        return thumbsDir.absoluteFilePath(image);
    }

    QDir imagesDir(cardsPath);

    if (!imagesDir.exists(image))
    {
        return "";
    }

    // taken from https://github.com/nemomobile/nemo-qml-plugin-thumbnailer
    QImage thumbnail;
    QSize originalSize;
    QSize thumbSize(THUMB_WIDTH, THUMB_HEIGHT);

    QImageReader imageReader(imagesDir.absoluteFilePath(image));
    if (!imageReader.canRead())
    {
        return "";
    }

    originalSize = imageReader.size();

    if (originalSize != thumbSize && originalSize.isValid())
    {
        QSize scaledSize(originalSize);
        scaledSize.scale(thumbSize, Qt::KeepAspectRatioByExpanding);

        QPoint center((scaledSize.width()-1)/2, (scaledSize.height()-1)/2);
        QRect cr(0, 0, thumbSize.width(), thumbSize.height());
        cr.moveCenter(center);
        imageReader.setScaledClipRect(cr);
        imageReader.setScaledSize(scaledSize);
    }
    thumbnail = imageReader.read();
    thumbnail.save(thumbsDir.absoluteFilePath(image),"JPG");

    return thumbsDir.absoluteFilePath(image);
}

QVariant CardModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    Card *card = _cards.at(index.row());
    QStringList thresholdIcons;

    switch (role)
    {
    case idRole:
        return card->id();
    case nameRole:
        return card->name();
    case numberRole:
        return card->number();
    case rarityRole:
        return card->rarity()->name;
    case raritySortOrderRole:
        return card->rarity()->sortOrder;
    case tradeRole:
        return card->trade()->name;
    case typeRole:
        return card->type()->name;
    case subtypeRole:
        return card->subtype();
    case thresholdRole:
        foreach (QString threshold, card->threshold())
        {
            if (threshold == "D")
                thresholdIcons.append("../icons/deception.png");
            else if (threshold == "E")
                thresholdIcons.append("../icons/elitism.png");
            else if (threshold == "G")
                thresholdIcons.append("../icons/greed.png");
            else if (threshold == "O")
                thresholdIcons.append("../icons/obsession.png");
            else if (threshold == "R")
                thresholdIcons.append("../icons/rage.png");
            else if (threshold == "V")
                thresholdIcons.append("../icons/volition.png");
        }
        return thresholdIcons;
    case thresholdCountRole:
        return card->thresholdCount();
    case costRole:
        return card->cost();
    case strengthRole:
        return card->strength();
    case lifeRole:
        return card->life();
    case speedRole:
        return card->speed();
    case structureRole:
        return card->structure();
    case textRole:
        return card->text();
    case flavorRole:
        return card->flavor();
    case setRole:
        return card->set()->name;
    case setReleaseOrderRole:
        return card->set()->releaseOrder;
    case imageRole:
        return getImageFile(card, _cardsPath);
    case thumbnailRole:
        return getThumbnailFile(card, _cardsPath);
    }

    return QVariant();
}

QHash<int, QByteArray> CardModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[idRole] = "id";
    roles[nameRole] = "name";
    roles[numberRole] = "number";
    roles[rarityRole] = "rarity";
    roles[raritySortOrderRole] = "raritySortOrder";
    roles[tradeRole] = "trade";
    roles[typeRole] = "type";
    roles[subtypeRole] = "subtype";
    roles[thresholdRole] = "threshold";
    roles[thresholdCountRole] = "thresholdCount";
    roles[costRole] = "cost";
    roles[strengthRole] = "strength";
    roles[lifeRole] = "life";
    roles[speedRole] = "speed";
    roles[structureRole] = "structure";
    roles[textRole] = "text";
    roles[flavorRole] = "flavor";
    roles[setRole] = "set";
    roles[setReleaseOrderRole] = "setReleaseOrder";
    roles[imageRole] = "image";
    roles[thumbnailRole] = "thumbnail";

    return roles;
}

bool sortSetAscReleaseOrder(const Set *set1, const Set *set2)
{
    return set1->releaseOrder < set2->releaseOrder;
}

QStringList CardModel::sets() const
{
    QList<Set *> setList = _sets.values();
    qStableSort(setList.begin(), setList.end(), sortSetAscReleaseOrder);

    QStringList setNames;
    for (int i = 0; i < setList.size(); ++i)
    {
        setNames.append(setList.at(i)->name);
    }

    return setNames;
}

QStringList CardModel::trades() const
{
    QList<Trade *> tradeList = _trades.values();

    QStringList tradeNames;
    for (int i = 0; i < tradeList.size(); ++i)
    {
        tradeNames.append(tradeList.at(i)->name);
    }
    tradeNames.sort(Qt::CaseInsensitive);
    return tradeNames;
}

QStringList CardModel::types() const
{
    QList<Type *> typeList = _types.values();

    QStringList typeNames;
    for (int i = 0; i < typeList.size(); ++i)
    {
        typeNames.append(typeList.at(i)->name);
    }
    typeNames.sort(Qt::CaseInsensitive);
    return typeNames;
}

bool sortRarityAscSortOrder(const Rarity *rar1, const Rarity *rar2)
{
    return rar1->sortOrder < rar2->sortOrder;
}

QStringList CardModel::rarities() const
{
    QList<Rarity *> rarityList = _rarities.values();
    for (int i = 0; i < rarityList.size(); ++i) {
    }
    qStableSort(rarityList.begin(), rarityList.end(), sortRarityAscSortOrder);
    for (int i = 0; i < rarityList.size(); ++i) {
    }

    QStringList rarityNames;
    for (int i = 0; i < rarityList.size(); ++i)
    {
        rarityNames.append(rarityList.at(i)->name);
    }
    return rarityNames;
}

void CardModel::init()
{
    QSqlQuery query;
    DBManager *db = DBManager::instance();

    query = db->prepare("SELECT id, octgnId, name, releaseOrder FROM sets ORDER BY releaseOrder;");
    db->execute(query);
    while (query.next())
    {
        Set* tmp = new Set(this);
        tmp->octgnId = query.value(1).toString();
        tmp->name = query.value(2).toString();
        tmp->releaseOrder = query.value(3).toInt();

        _sets.insert(query.value(0).toUInt(), tmp);
    }

    query = db->prepare("SELECT id, name FROM trades ORDER BY name;");
    db->execute(query);
    while (query.next())
    {
        Trade* tmp = new Trade(this);
        tmp->name = query.value(1).toString();
        _trades.insert(query.value(0).toUInt(), tmp);
    }

    query = db->prepare("SELECT id, name FROM types ORDER BY name;");
    db->execute(query);
    while (query.next())
    {
        Type* tmp = new Type(this);
        tmp->name = query.value(1).toString();
        _types.insert(query.value(0).toUInt(), tmp);
    }

    query = db->prepare("SELECT id, name, sortOrder FROM rarities ORDER BY sortOrder;");
    db->execute(query);
    while (query.next())
    {
        Rarity* tmp = new Rarity(this);
        tmp->name = query.value(1).toString();
        tmp->sortOrder = query.value(2).toInt();
        _rarities.insert(query.value(0).toUInt(), tmp);
    }
}

void CardModel::initCards()
{
    insertCards(0, loadCards());
}

QList<Card *> CardModel::loadCards()
{
    QSqlQuery query;
    DBManager *db = DBManager::instance();

    QString whereClause;

    query = db->prepare(
                "SELECT C.octgnId, C.name, ruletext, flavor, numberInSet, cost,"
                " threshold, strength, life, speed, structure, subtype,"
                " tradeId, typeId, rarityId, setId"
                " FROM cards AS C;"
                );
    db->execute(query);

    QList<Card *> cards;
    while (query.next())
    {
        Card *card = new Card(this);
        card->setId(query.value(0).toString());
        card->setName(query.value(1).toString());
        card->setText(query.value(2).toString());
        card->setFlavor(query.value(3).toString());
        card->setNumber(query.value(4).toInt());
        card->setCost(query.value(5).toInt());
        card->setThreshold(query.value(6).toString().split(QString(""),QString::SkipEmptyParts));
        card->setStrength(query.value(7).toInt());
        card->setLife(query.value(8).toInt());
        card->setSpeed(query.value(9).toInt());
        card->setStructure(query.value(10).toInt());
        card->setSubtype(query.value(11).toString());

        card->setTrade(_trades.value(query.value(12).toUInt()));
        card->setType(_types.value(query.value(13).toUInt()));
        card->setRarity(_rarities.value(query.value(14).toUInt()));
        card->setSet(_sets.value(query.value(15).toUInt()));

        cards.append(card);
    }

    return cards;
}

QList<Card *> CardModel::loadCards(QStringList octgnIds)
{
    if (!octgnIds.size())
    {
        return QList<Card *>();
    }

    QSqlQuery query;
    DBManager *db = DBManager::instance();

    QString whereClause = QString(" WHERE C.octgnId IN ('%1') ").arg(octgnIds.join("', '"));

    query = db->prepare(QString(
                            "SELECT C.octgnId, C.name, ruletext, flavor, numberInSet, cost,"
                            " threshold, strength, life, speed, structure, subtype,"
                            " tradeId, typeId, rarityId, setId"
                            " FROM cards AS C"
                            " %1;"
                            ).arg(whereClause));
    db->execute(query);

    QList<Card *> cards;
    while (query.next())
    {
        Card *card = new Card(this);
        card->setId(query.value(0).toString());
        card->setName(query.value(1).toString());
        card->setText(query.value(2).toString());
        card->setFlavor(query.value(3).toString());
        card->setNumber(query.value(4).toInt());
        card->setCost(query.value(5).toInt());
        card->setThreshold(query.value(6).toString().split(QString(""),QString::SkipEmptyParts));
        card->setStrength(query.value(7).toInt());
        card->setLife(query.value(8).toInt());
        card->setSpeed(query.value(9).toInt());
        card->setStructure(query.value(10).toInt());
        card->setSubtype(query.value(11).toString());

        card->setTrade(_trades.value(query.value(12).toUInt()));
        card->setType(_types.value(query.value(13).toUInt()));
        card->setRarity(_rarities.value(query.value(14).toUInt()));
        card->setSet(_sets.value(query.value(15).toUInt()));

        cards.append(card);
    }

    return cards;
}

void CardModel::insertCards(const int row, const QList<Card *> cards)
{
    if (row < 0 || row > rowCount())
        return;
    if (cards.size() == 0)
        return;

    beginInsertRows(QModelIndex(), row, row + cards.size()-1);
    for (int i = 0; i < cards.size(); ++i)
    {
        _cards.insert(i + row, cards.at(i));
    }
    endInsertRows();
    emit countChanged();
}

