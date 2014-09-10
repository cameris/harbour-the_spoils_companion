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

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.the_spoils_companion 1.0

Column {
    property alias header: headColumn.children
    property var cardModel
    property alias searchField: searchField

    Column {
        id: headColumn
        width: parent.width
    }

    Row {
        spacing: Theme.paddingLarge
        anchors.horizontalCenter: parent.horizontalCenter

        Button {
            text: "Set"
            onClicked: {
                searchField.focus = false
                pageStack.push(categoryFilter, {
                                   filterTitle: text,
                                   filterModel: cardModel.setFilter
                               })
            }
        }
        Button {
            text: "Rarity"
            onClicked: {
                searchField.focus = false
                pageStack.push(categoryFilter, {
                                   filterTitle: text,
                                   filterModel: cardModel.rarityFilter
                               })
            }
        }
    }

    Row {
        spacing: Theme.paddingLarge
        anchors.horizontalCenter: parent.horizontalCenter

        Button {
            text: "Trade"
            onClicked: {
                searchField.focus = false
                pageStack.push(categoryFilter, {
                                   filterTitle: text,
                                   filterModel: cardModel.tradeFilter
                               })
            }
        }
        Button {
            text: "Type"
            onClicked: {
                searchField.focus = false
                pageStack.push(categoryFilter, {
                                   filterTitle: text,
                                   filterModel: cardModel.typeFilter
                               })
            }
        }
    }

    Row {
        spacing: Theme.paddingLarge
        anchors.horizontalCenter: parent.horizontalCenter

        Button {
            text: "Stats"
            onClicked: {
                searchField.focus = false
                pageStack.push(numericFilter, {
                                   filterTitle: text,
                                   filterModel: cardModel.statsFilter
                               })
            }
        }
        Button {
            text: "Search"
            onClicked: {
                searchField.focus = false
                pageStack.push(categoryFilter, {
                                   filterTitle: text,
                                   filterModel: cardModel.textFilter
                               })
            }
        }
    }

    BackgroundItem {
        width: parent.width
        Label {
            x: Theme.paddingLarge
            anchors {
                verticalCenter: parent.verticalCenter
            }

            color: Theme.highlightColor
            text: Theme.highlightText("Sort by ", "Sort by", Theme.primaryColor) + cardModel.sortString
        }
        IconButton {
            anchors {
                right: parent.right
                rightMargin: Theme.paddingLarge
                verticalCenter: parent.verticalCenter
            }
            icon.source: "image://theme/icon-m-play"
            rotation: cardModel.descendingSort ? 90 : -90

            onClicked: {
                cardModel.descendingSort = !cardModel.descendingSort
            }
        }
        onClicked: {
            searchField.focus = false
            var dialog = pageStack.push(sortPicker, {
                                            model: cardModel.sortStrings,
                                            currentString: cardModel.sortString
                                        })
            dialog.accepted.connect(function () {
                cardModel.sortString = dialog.currentString
            })
        }
    }

    SearchField {
        id: searchField
        width: parent.width

        font.capitalization: Font.AllLowercase
        inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
        validator: RegExpValidator { regExp: /^[0-9a-zA-Z\.#+-=_ ]*$/ }

        text: cardModel.stringFilter
        Binding {
            target: cardModel
            property: "stringFilter"
            value: searchField.text.trim()
        }

        EnterKey.iconSource: "image://theme/icon-m-enter-close"
        EnterKey.onClicked: searchField.focus = false
    }

    Component {
        id: categoryFilter
        Page {
            property alias filterTitle: filterHeader.title
            property alias filterModel: filterRepeater.model
            SilicaFlickable {
                anchors.fill: parent
                contentHeight: column.height

                PullDownMenu {
                    MenuItem {
                        text: filterRepeater.selectCount == filterRepeater.count ?
                                  "Deselect all" : "Select all"
                        onClicked: {
                            if (filterRepeater.selectCount == filterRepeater.count)
                                filterRepeater.deselectAll()
                            else
                                filterRepeater.selectAll()
                        }
                    }
                }

                Column {
                    id: column
                    width: parent.width

                    PageHeader {
                        id: filterHeader
                    }

                    Repeater {
                        id: filterRepeater
                        property int selectCount
                        signal selectAll
                        signal deselectAll

                        delegate: TextSwitch {
                            id: filterDelegate
                            width: parent.width
                            text: model.name
                            checked: model.include
                            automaticCheck: false

                            property bool initComplete: false

                            onClicked: {
                                model.include = !model.include
                            }

                            onCheckedChanged: {
                                if (initComplete)
                                {
                                    filterRepeater.selectCount += checked ? 1 : -1
                                }
                            }

                            function deselectInclude() {
                                if (model.include)
                                    model.include = false
                            }

                            function selectInclude() {
                                if (!model.include)
                                    model.include = true
                            }

                            Component.onCompleted: {
                                filterRepeater.selectAll.connect(selectInclude)
                                filterRepeater.deselectAll.connect(deselectInclude)
                                filterRepeater.selectCount += model.include ? 1 : 0
                                initComplete = true
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: numericFilter
        Page {
            property alias filterTitle: filterHeader.title
            property alias filterModel: filterRepeater.model
            SilicaFlickable {
                anchors.fill: parent
                contentHeight: column.height

                PullDownMenu {
                    MenuItem {
                        text: "Clear values"
                        onClicked: {
                            filterRepeater.clearValues()
                        }
                    }
                }

                Column {
                    id: column
                    width: parent.width

                    PageHeader {
                        id: filterHeader
                    }

                    Repeater {
                        id: filterRepeater
                        signal clearValues

                        delegate: ListItem {
                            id: filterDelegate
                            Row {
                                x: Theme.paddingLarge
                                spacing: Theme.paddingMedium
                                anchors.verticalCenter: parent.verticalCenter
                                Label {
                                    anchors {
                                        verticalCenter: parent.verticalCenter
                                    }
                                    color: operandField.text.length > 0 ? Theme.primaryColor : Theme.secondaryColor
                                    text: model.name
                                }
                                Label {
                                    anchors {
                                        verticalCenter: parent.verticalCenter
                                    }
                                    color: Theme.highlightColor
                                    text: model.operation
                                }

                                TextField {
                                    id: operandField
                                    width: 150
                                    height: Math.max(Theme.itemSizeMedium, _editor.height)
                                    textLeftMargin: 0
                                    textTopMargin: height/2 - _editor.implicitHeight/2
                                    labelVisible: false

                                    placeholderText: "value"
                                    inputMethodHints: Qt.ImhDigitsOnly
                                    validator: RegExpValidator { regExp: /^[0-9]*$/ }

                                    color: errorHighlight ? "red" : Theme.primaryColor

                                    background: null

                                    onTextChanged: {
                                        model.operand = text
                                    }
                                    Component.onCompleted: {
                                        text = model.operand
                                        filterRepeater.clearValues.connect(function () {
                                            operandField.text = ""
                                        })
                                    }

                                    EnterKey.iconSource: "image://theme/icon-m-enter-close"
                                    EnterKey.onClicked: operandField.focus = false
                                }


                            }
                            IconButton {
                                anchors {
                                    right: parent.right
                                    rightMargin: Theme.paddingLarge
                                    verticalCenter: parent.verticalCenter
                                }

                                icon.source: "image://theme/icon-m-clear"
                                opacity: operandField.text.length > 0 ? 1 : 0
                                Behavior on opacity {
                                    FadeAnimation {}
                                }

                                onClicked: {
                                    if (opacity != 0)
                                        operandField.text = ""
                                }
                            }

                            function updateOperation(op) {
                                model.operation = op
                            }

                            showMenuOnPressAndHold: true
                            onClicked: {
                                operandField.focus = true
                            }

                            menu: ContextMenu {
                                id: operationMenu
                                Row {
                                    width: parent.width
                                    height: Theme.itemSizeSmall
                                    Repeater {
                                        id: operationRepeater
                                        model: modelData.operations()
                                        delegate: BackgroundItem {
                                            id: operationDelegate
                                            width: parent.width / operationRepeater.count
                                            Label {
                                                anchors.centerIn: parent
                                                horizontalAlignment: Text.AlignHCenter
                                                color: highlighted ? Theme.highlightColor : Theme.primaryColor
                                                text: modelData
                                            }

                                            onClicked: {
                                                filterDelegate.updateOperation(modelData)
                                                operationMenu.hide()
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: sortPicker
        Dialog {
            id: sortPickerDialog
            property alias model: sortView.model
            property string currentString

            SilicaListView {
                id: sortView
                anchors.fill: parent

                header: DialogHeader {
                    title: "Sort by"
                }

                delegate: BackgroundItem {
                    width: parent.width
                    height: Theme.itemSizeSmall
                    highlighted: down || currentString === modelData

                    Label {
                        x: Theme.paddingLarge
                        anchors.verticalCenter: parent.verticalCenter
                        color: highlighted ? Theme.highlightColor : Theme.primaryColor
                        text: modelData
                    }

                    onClicked: {
                        currentString = modelData
                        sortPickerDialog.accept()
                    }
                }
            }
        }
    }
}
