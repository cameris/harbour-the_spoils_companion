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

#include "cardproxymodel.h"

#include "cardmodel.h"

CardProxyModel::CardProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent), _settingsSection("search"), _autoInvalidateFilter(true),
    _stringFilter(""), _typeFilterOn(false), _tradeFilterOn(false), _setFilterOn(false),
    _rarityFilterOn(false)
{
    setDynamicSortFilter(true);

    _sortList << "" << "" << "";
    _sortDescList << false << false << false;

    _stringRegExp.setCaseSensitivity(Qt::CaseInsensitive);
    _stringRegExp.setPatternSyntax(QRegExp::Wildcard);
}

QString stringToLeetPattern(const QString &string)
{
    if (string.length() <= 0)
        return "";
    QString tmp = string;
    tmp.replace('a', "[a4]", Qt::CaseInsensitive);
    tmp.replace('b', "[b68]", Qt::CaseInsensitive);
    tmp.replace('e', "[e3]", Qt::CaseInsensitive);
    tmp.replace('g', "[g9]", Qt::CaseInsensitive);
    tmp.replace('i', "[i1]", Qt::CaseInsensitive);
    tmp.replace('l', "[l1]", Qt::CaseInsensitive);
    tmp.replace('o', "[o0]", Qt::CaseInsensitive);
    tmp.replace('s', "[s5]", Qt::CaseInsensitive);
    tmp.replace('t', "[t7]", Qt::CaseInsensitive);

    return tmp;
}

void CardProxyModel::classBegin()
{
}

void CardProxyModel::componentComplete()
{
    _autoInvalidateFilter = false;

    loadTextFilter();
    loadTypeFilter();
    loadTradeFilter();
    loadSetFilter();
    loadRarityFilter();
    loadStatsFilters();

    loadSortStrings();

    setStringFilter(_settings.value(_settingsSection + "/string", "").toString());

    _autoInvalidateFilter = true;

    invalidateFilter();

    loadSortList();
    loadSortDescList();
    fullSort();

    emit sortStringChanged();
    emit descendingSortChanged();
}

QObject *CardProxyModel::source() const
{
    return sourceModel();
}

void CardProxyModel::setSource(QObject *source)
{
    if (!source)
        return;

    _cardmodel = static_cast<CardModel *>(source);
    setSourceModel(qobject_cast<QAbstractItemModel *>(source));
}

QString CardProxyModel::settingsSection() const
{
    return _settingsSection;
}

void CardProxyModel::setSettingsSection(const QString &settingsSection)
{
    if (settingsSection == _settingsSection)
        return;
    _settingsSection = settingsSection;
    emit settingsSectionChanged();
}

QString CardProxyModel::stringFilter() const
{
    return _stringFilter;
}

void CardProxyModel::setStringFilter(const QString &stringFilter)
{
    if (stringFilter == _stringFilter)
        return;

    _stringFilter = stringFilter;
    _stringRegExp.setPattern(stringToLeetPattern(stringFilter));

    emit stringFilterChanged();
    emit stringRegExpChanged();

    if (_autoInvalidateFilter)
        invalidateFilter();
}

QRegExp CardProxyModel::stringRegExp() const
{
    return _stringRegExp;
}

QQmlListProperty<CategoryFilter> CardProxyModel::textFilter()
{
    return QQmlListProperty<CategoryFilter>(this, _textFilters);
}

QQmlListProperty<CategoryFilter> CardProxyModel::typeFilter()
{
    return QQmlListProperty<CategoryFilter>(this, _typeFilters);
}

QQmlListProperty<CategoryFilter> CardProxyModel::tradeFilter()
{
    return QQmlListProperty<CategoryFilter>(this, _tradeFilters);
}

QQmlListProperty<CategoryFilter> CardProxyModel::setFilter()
{
    return QQmlListProperty<CategoryFilter>(this, _setFilters);
}

QQmlListProperty<CategoryFilter> CardProxyModel::rarityFilter()
{
    return QQmlListProperty<CategoryFilter>(this, _rarityFilters);
}

QQmlListProperty<NumericFilter> CardProxyModel::statsFilter()
{
    return QQmlListProperty<NumericFilter>(this, _statsFilters);
}

bool CardProxyModel::descendingSort() const
{
    return _sortDescList.at(2);
}

void CardProxyModel::setDescendingSort(bool descendingSort)
{
    if (descendingSort == _sortDescList.at(2))
        return;

    _sortDescList.replace(2, descendingSort);
    sort(0, _sortDescList.at(2) ? Qt::DescendingOrder : Qt::AscendingOrder);
    saveSortDescList();

    emit descendingSortChanged();
}

QString CardProxyModel::sortString() const
{
    return _sortList.last();
    return _sortList.at(2);
}

void CardProxyModel::setSortString(const QString &sortString)
{
    int sortRole = _sortNameRoleMap.value(sortString, CardModel::setReleaseOrderRole);

    if (sortRole == this->sortRole())
        return;

    _sortList.removeFirst();
    _sortList.append(sortString);
    _sortDescList.removeFirst();
    _sortDescList.append(_sortDescList.at(1));

    setSortRole(sortRole);

    emit sortStringChanged();
    //DISABLED BUG 2
//    emit sectionPropertyChanged();
//    emit sectionExtraLabelChanged();
    //END DISABLED
}

QStringList CardProxyModel::sortStrings() const
{
    QStringList sortStrings = _sortNameRoleMap.keys();
    return sortStrings;
}

QSettings *CardProxyModel::settings()
{
    return &_settings;
}

void CardProxyModel::saveSettings()
{
    saveCategoryFilters(_textFilters);
    saveCategoryFilters(_typeFilters);
    saveCategoryFilters(_tradeFilters);
    saveCategoryFilters(_setFilters);
    saveCategoryFilters(_rarityFilters);
    saveNumericFilters(_statsFilters);

    _settings.setValue(_settingsSection + "/string", _stringFilter);

    saveSortList();
    saveSortDescList();
}

void CardProxyModel::resetFiltersAndSorting()
{
    _autoInvalidateFilter = false;

    resetCategoryFilters(_textFilters);
    resetCategoryFilters(_typeFilters);
    resetCategoryFilters(_tradeFilters);
    resetCategoryFilters(_setFilters);
    resetCategoryFilters(_rarityFilters);
    resetNumericFilters(_statsFilters);

    setStringFilter("");

    _autoInvalidateFilter = true;
    invalidateFilter();

    _sortList.clear();
    _sortList.append("Name");
    _sortList.append("Trade");
    _sortList.append("Set");

    _sortDescList.clear();
    _sortDescList.append(false);
    _sortDescList.append(false);
    _sortDescList.append(true);

    fullSort();

    emit sortStringChanged();
    emit descendingSortChanged();
}

//DISABLED BUG 2
//QString CardProxyModel::sectionProperty() const
//{
//    int role = sortRole();

//    // special handling
//    if (role == CardModel::setReleaseOrderRole)
//    {
//        // sorting set by releaseOrder but want name as section
//        role = CardModel::setRole;
//    }
//    else if (role == CardModel::raritySortOrderRole)
//    {
//        role = CardModel::rarityRole;
//    }
//    else if (role == CardModel::nameRole)
//    {
//        // no use for section, just clutters screen
//        return "";
//    }

//    return roleNames().value(role);
//}

//QString CardProxyModel::sectionExtraLabel() const
//{
//    // extra label for numeric values
//    switch (sortRole())
//    {
//    case CardModel::costRole:
//        return "Cost ";
//    case CardModel::thresholdCountRole:
//        return "Threshold ";
//    case CardModel::strengthRole:
//        return "Strength ";
//    case CardModel::lifeRole:
//        return "Life ";
//    case CardModel::speedRole:
//        return "Speed ";
//    case CardModel::structureRole:
//        return "Structure ";
//    }
//    return "";
//}
//END DISABLED

void CardProxyModel::updateTextFilter()
{
    if (_autoInvalidateFilter)
        invalidateFilter();
}

void CardProxyModel::updateTypeFilter()
{
    _typeFilterOn = false;
    for (int i = 0; i < _typeFilters.size(); ++i) {
        if (!_typeFilters.at(i)->include())
        {
            _typeFilterOn = true;
            break;
        }
    }
    if (_autoInvalidateFilter)
        invalidateFilter();
}

void CardProxyModel::updateTradeFilter()
{
    _tradeFilterOn = false;
    for (int i = 0; i < _tradeFilters.size(); ++i) {
        if (!_tradeFilters.at(i)->include())
        {
            _tradeFilterOn = true;
            break;
        }
    }
    if (_autoInvalidateFilter)
        invalidateFilter();
}

void CardProxyModel::updateSetFilter()
{
    _setFilterOn = false;
    for (int i = 0; i < _setFilters.size(); ++i) {
        if (!_setFilters.at(i)->include())
        {
            _setFilterOn = true;
            break;
        }
    }
    if (_autoInvalidateFilter)
        invalidateFilter();
}

void CardProxyModel::updateRarityFilter()
{
    _rarityFilterOn = false;
    for (int i = 0; i < _rarityFilters.size(); ++i) {
        if (!_rarityFilters.at(i)->include())
        {
            _rarityFilterOn = true;
            break;
        }
    }
    if (_autoInvalidateFilter)
        invalidateFilter();
}

void CardProxyModel::updateStatsFilter()
{
    if (_autoInvalidateFilter)
        invalidateFilter();
}

bool CardProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    // Filter Type
    if (_typeFilterOn)
    {
        for (int i = 0; i < _typeFilters.size(); ++i)
        {
            if (index.data(CardModel::typeRole).toString() == _typeFilters.at(i)->name())
            {
                if (!_typeFilters.at(i)->include())
                {
                    return false;
                }
                break;
            }
        }
    }

    // Filter Trade
    if (_tradeFilterOn)
    {
        for (int i = 0; i < _tradeFilters.size(); ++i)
        {
            if (index.data(CardModel::tradeRole).toString() == _tradeFilters.at(i)->name())
            {
                if (!_tradeFilters.at(i)->include())
                {
                    return false;
                }
                break;
            }
        }
    }

    // Filter Set
    if (_setFilterOn)
    {
        for (int i = 0; i < _setFilters.size(); ++i)
        {
            if (index.data(CardModel::setRole).toString() == _setFilters.at(i)->name())
            {
                if (!_setFilters.at(i)->include())
                {
                    return false;
                }
                break;
            }
        }
    }

    // Filter Rarity
    if (_rarityFilterOn)
    {
        for (int i = 0; i < _rarityFilters.size(); ++i)
        {
            if (index.data(CardModel::rarityRole).toString() == _rarityFilters.at(i)->name())
            {
                if (!_rarityFilters.at(i)->include())
                {
                    return false;
                }
                break;
            }
        }
    }

    // Filter Stats
    foreach (NumericFilter *filter, _statsFilters)
    {
        int value;
        if (filter->name() == "Cost")
            value = index.data(CardModel::costRole).toInt();
        else if (filter->name() == "Threshold")
            value = index.data(CardModel::thresholdRole).toStringList().count();
        else if (filter->name() == "Strength")
            value = index.data(CardModel::strengthRole).toInt();
        else if (filter->name() == "Life")
            value = index.data(CardModel::lifeRole).toInt();
        else if (filter->name() == "Speed")
            value = index.data(CardModel::speedRole).toInt();
        else if (filter->name() == "Structure")
            value = index.data(CardModel::structureRole).toInt();
        else
            continue;

        if (!filter->evaluate(value))
        {
            return false;
        }
    }

    // Filter String (OR)
    if (_stringFilter != "")
    {
        bool containsString = false;
        for (int i = 0; i < _textFilters.size(); ++i)
        {
            if (_textFilters.at(i)->include())
            {
                bool value;
                if (_textFilters.at(i)->name() == "Name")
                {
                    value = index.data(CardModel::nameRole).toString().contains(_stringRegExp);
                }
                else if (_textFilters.at(i)->name() == "Text")
                {
                    value = index.data(CardModel::textRole).toString().contains(_stringRegExp);
                }
                else if (_textFilters.at(i)->name() == "Flavor")
                {
                    value = index.data(CardModel::flavorRole).toString().contains(_stringRegExp);
                }
                else if (_textFilters.at(i)->name() == "Subtype")
                {
                    value = index.data(CardModel::subtypeRole).toString().contains(_stringRegExp);
                }

                if (value)
                {
                    containsString = true;
                    break;
                }
            }
        }
        if (!containsString)
        {
            return false;
        }
    }

    return true;
}

void CardProxyModel::loadTextFilter()
{
    if (_textFilters.size())
    {
        qDeleteAll(_textFilters);
        _textFilters.clear();
    }

    QStringList filters;
    filters << "Name" << "Text" << "Flavor" << "Subtype";
    foreach(QString name, filters)
    {
        CategoryFilter *filter = new CategoryFilter(this);
        filter->setName(name);
        filter->setInclude(_settings.value(QString("%1/%2").arg(_settingsSection, name),
                                          true).toBool());

        connect(filter, &CategoryFilter::includeChanged, this, &CardProxyModel::updateTextFilter);
        _textFilters.append(filter);
    }
    emit textFilterChanged();
}

void CardProxyModel::loadTypeFilter()
{
    if (_typeFilters.size())
    {
        qDeleteAll(_typeFilters);
        _typeFilters.clear();
    }

    _typeFilterOn = false;

    foreach (QString name, _cardmodel->types())
    {
        CategoryFilter *filter = new CategoryFilter(this);
        filter->setName(name);
        filter->setInclude(_settings.value(QString("%1/%2").arg(_settingsSection,name),
                                          true).toBool());

        if (!filter->include())
        {
            _typeFilterOn = true;
        }

        connect(filter, &CategoryFilter::includeChanged, this, &CardProxyModel::updateTypeFilter);
        _typeFilters.append(filter);
    }
    emit typeFilterChanged();
}

void CardProxyModel::loadTradeFilter()
{
    if (_tradeFilters.size())
    {
        qDeleteAll(_tradeFilters);
        _tradeFilters.clear();
    }

    _tradeFilterOn = false;

    foreach (QString name, _cardmodel->trades())
    {
        CategoryFilter *filter = new CategoryFilter(this);
        filter->setName(name);
        filter->setInclude(_settings.value(QString("%1/%2").arg(_settingsSection,name),
                                          true).toBool());

        if (!filter->include())
        {
            _tradeFilterOn = true;
        }

        connect(filter, &CategoryFilter::includeChanged, this, &CardProxyModel::updateTradeFilter);
        _tradeFilters.append(filter);
    }
    emit tradeFilterChanged();
}

void CardProxyModel::loadSetFilter()
{
    if (_setFilters.size())
    {
        qDeleteAll(_setFilters);
        _setFilters.clear();
    }

    _setFilterOn = false;

    foreach (QString name, _cardmodel->sets())
    {
        CategoryFilter *filter = new CategoryFilter(this);
        filter->setName(name);
        filter->setInclude(_settings.value(QString("%1/%2").arg(_settingsSection,name),
                                          true).toBool());

        if (!filter->include())
        {
            _setFilterOn = true;
        }

        connect(filter, &CategoryFilter::includeChanged, this, &CardProxyModel::updateSetFilter);
        _setFilters.append(filter);
    }
    emit setFilterChanged();
}

void CardProxyModel::loadRarityFilter()
{
    if (_rarityFilters.size())
    {
        qDeleteAll(_rarityFilters);
        _rarityFilters.clear();
    }

    _rarityFilterOn = false;

    foreach (QString name, _cardmodel->rarities())
    {
        CategoryFilter *filter = new CategoryFilter(this);
        filter->setName(name);
        filter->setInclude(_settings.value(QString("%1/%2").arg(_settingsSection,name),
                                          true).toBool());

        if (!filter->include())
        {
            _rarityFilterOn = true;
        }

        connect(filter, &CategoryFilter::includeChanged, this, &CardProxyModel::updateRarityFilter);
        _rarityFilters.append(filter);
    }
    emit rarityFilterChanged();

}

void CardProxyModel::loadStatsFilters()
{
    QStringList filters;
    filters << "Cost" << "Threshold" << "Strength" << "Life" << "Speed" << "Structure";
    foreach(QString name, filters)
    {
        NumericFilter *filter = new NumericFilter(this);
        filter->setName(name);
        filter->setOperation(_settings.value(QString("%1/%2/operation").arg(_settingsSection,name),
                                          "=").toString());
        filter->setOperand(_settings.value(QString("%1/%2/operand").arg(_settingsSection,name),
                                          "").toString());

        connect(filter, &NumericFilter::operationChanged, this, &CardProxyModel::updateStatsFilter);
        connect(filter, &NumericFilter::operandChanged, this, &CardProxyModel::updateStatsFilter);
        _statsFilters.append(filter);
    }
    emit statsFilterChanged();
}

void CardProxyModel::loadSortStrings()
{
    if (_sortNameRoleMap.size())
    {
        _sortNameRoleMap.clear();
    }

    _sortNameRoleMap.insert("Set", CardModel::setReleaseOrderRole);
    _sortNameRoleMap.insert("Name", CardModel::nameRole);
    _sortNameRoleMap.insert("Trade", CardModel::tradeRole);
    _sortNameRoleMap.insert("Type", CardModel::typeRole);
    _sortNameRoleMap.insert("Cost", CardModel::costRole);
    _sortNameRoleMap.insert("Rarity", CardModel::raritySortOrderRole);
    _sortNameRoleMap.insert("Threshold", CardModel::thresholdCountRole);
    _sortNameRoleMap.insert("Strength", CardModel::strengthRole);
    _sortNameRoleMap.insert("Life", CardModel::lifeRole);
    _sortNameRoleMap.insert("Speed", CardModel::speedRole);
    _sortNameRoleMap.insert("Structure", CardModel::structureRole);

    emit sortStringsChanged();
}

void CardProxyModel::loadSortList()
{
    _sortList.clear();
    QVariantList sortList =
            _settings.value(_settingsSection + "/sort",
                            (QVariantList() << "Name" << "Trade" << "Set")).toList();

    for (int i = 0; i < sortList.size(); ++i)
    {
        _sortList.append(sortList.at(i).toString());
    }
}

void CardProxyModel::loadSortDescList()
{
    _sortDescList.clear();
    QVariantList sortDescList =
            _settings.value(_settingsSection + "/sortDescending",
                            (QVariantList() << false << false << true)).toList();
    for (int i = 0; i < sortDescList.size(); ++i)
    {
        _sortDescList.append(sortDescList.at(i).toBool());
    }
}

void CardProxyModel::saveSortList()
{
    QVariantList tmp;
    for (int i = 0; i < _sortList.size(); ++i)
    {
        tmp.append(_sortList.at(i));
    }
    _settings.setValue(_settingsSection + "/sort", tmp);
}

void CardProxyModel::saveSortDescList()
{
    QVariantList tmp;
    for (int i = 0; i < _sortDescList.size(); ++i)
    {
        tmp.append(_sortDescList.at(i));
    }
    _settings.setValue(_settingsSection + "/sortDescending", tmp);
}

void CardProxyModel::fullSort()
{
    for (int i = 0; i < _sortList.size(); ++i)
    {
        int sortRole = _sortNameRoleMap.value(_sortList.at(i), CardModel::setReleaseOrderRole);
        bool sortDesc = _sortDescList.at(i);

        setSortRole(sortRole);
        sort(0, sortDesc ? Qt::DescendingOrder : Qt::AscendingOrder);
    }
}

void CardProxyModel::saveCategoryFilters(QList<CategoryFilter *> filters)
{
    for (int i = 0; i < filters.size(); ++i)
    {
        filters.at(i)->saveSettings();
    }
}

void CardProxyModel::resetCategoryFilters(QList<CategoryFilter *> filters)
{
    for (int i = 0; i < filters.size(); ++i)
    {
        filters.at(i)->setInclude();
    }
}

void CardProxyModel::saveNumericFilters(QList<NumericFilter *> filters)
{
    for (int i = 0; i < filters.size(); ++i)
    {
        filters.at(i)->saveSettings();
    }
}

void CardProxyModel::resetNumericFilters(QList<NumericFilter *> filters)
{
    for (int i = 0; i < filters.size(); ++i)
    {
        filters.at(i)->setOperation();
        filters.at(i)->setOperand();
    }
}
