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

import "../components"

Page {
    id: collectionPage

    property string activeLayout: settings.value(cardModel.settingsSection + "/layout", "list")

    onStatusChanged: {
        if (status === PageStatus.Deactivating)
        {
            cardModel.saveSettings()
        }
    }

    CardProxyModel {
        id: cardModel
        source: CollectionModel { id: collectionModel }
        settingsSection: "collection"
    }

    Loader {
        id: viewLoader
        anchors.fill: parent
        sourceComponent: activeLayout === "list" ? cardListComponent : cardGridComponent
    }

    SearchHeader {
        id: headerContainer
        width: collectionPage.width
        cardModel: cardModel

        header: PageHeader { title: "Collection" }
    }

    Component {
        id: sharedPullDown
        PullDownMenu {
            MenuItem {
                text: "Reset filters and sorting"
                onClicked: {
                    cardModel.resetFiltersAndSorting()
                }
            }
            MenuItem {
                text: "Switch to " + (activeLayout === "list" ? "grid" : "list")
                onClicked: {
                    activeLayout = activeLayout === "list" ? "grid" : "list"
                    settings.setValue(cardModel.settingsSection + "/layout", activeLayout)
                }
            }
            MenuItem {
                text: "Add card"
                onClicked: {
                    headerContainer.searchField.focus = false
                    var picker = pageStack.push("CardPicker.qml")

                    picker.pick.connect(function (octgnId) {
                        collectionModel.addCard(octgnId)
                    })
                }
            }
        }
    }

    Component {
        id: cardGridComponent
        SilicaGridView {
            id: cardGridView
            anchors.fill:parent

            currentIndex: -1
            header: Item {
                id: header
                width: headerContainer.width
                height: headerContainer.height
                Component.onCompleted: headerContainer.parent = header
            }

            ViewPlaceholder {
                enabled: cardGridView.count === 0
                verticalOffset: headerContainer.height/2
                text: "No cards in collection"
            }

            cellWidth: Math.floor(width / 3)
            cellHeight: Math.floor(cellWidth * 7/5)

            Loader {
                sourceComponent: sharedPullDown
            }

            //WORKAROUND BUG 1, first item not visible after
            // filtering until the gridview is moved
            // moving helps, but seems to not work always
            function microMove() {
                    cardGridView.contentY += 1
                    cardGridView.contentY -= 1
            }
            Component.onCompleted: headerContainer.searchField.textChanged.connect(microMove)
            Component.onDestruction: headerContainer.searchField.textChanged.disconnect(microMove)
            //END WORKAROUND

            onMovingVerticallyChanged: {
                headerContainer.searchField.focus = false
            }

            model: cardModel

            delegate: ListItem {
                id: gridDelegate
                width: cardGridView.cellWidth
                contentHeight: cardGridView.cellHeight

                menu: GridContextMenu {
                    gridDelegate: gridDelegate
                    modelCount: cardGridView.count

                    MenuItem {
                        text: "Remove card"
                        onClicked: {
                            gridDelegate.remorseAction("Removing card", function () {
                                collectionModel.removeCard(id, count)
                            })
                        }
                    }
                }

                Loader {
                    id: thumbLoader
                    anchors.fill: parent

                    active: thumbnail

                    sourceComponent: Image {
                        asynchronous: true
                        fillMode: Image.PreserveAspectCrop
                        source: thumbnail
                    }
                }

                Loader {
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width - Theme.paddingMedium

                    active: !thumbLoader.active
                    sourceComponent: Component {
                        Label {
                            wrapMode: Text.Wrap
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: Theme.fontSizeSmall
                            color: gridDelegate.highlighted ? Theme.highlightColor : Theme.primaryColor

                            textFormat: Text.StyledText
                            text: Theme.highlightText(name, cardModel.stringRegExp, Theme.highlightColor)
                        }
                    }
                }

                Rectangle {
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }
                    height: parent.height/2
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: Theme.rgba("black", 0.0) }
                        GradientStop { position: 0.3; color: Theme.rgba("black", 0.5) }
                        GradientStop { position: 1.0; color: Theme.rgba("black", 0.9) }
                    }
                }

                TextField {
                    id: countField
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }

                    horizontalAlignment: TextInput.AlignHCenter

                    textMargin: 0
                    labelVisible: false

                    placeholderText: "?"
                    placeholderColor: "#ff4d4d"

                    inputMethodHints: Qt.ImhDigitsOnly
                    validator: RegExpValidator { regExp: /^[1-9][0-9]*/ }

                    font.bold: true
                    font.pixelSize: Theme.fontSizeHuge

                    background: null

                    onTextChanged: {
                        if (text != model.count && text > 0)
                        {
                            cardModel.dynamicSortFilter = false
                            model.count = text
                            cardModel.dynamicSortFilter = true
                        }
                    }
                    Binding {
                        target: countField
                        property: "text"
                        value: model.count
                    }

                    EnterKey.iconSource: "image://theme/icon-m-enter-close"
                    EnterKey.onClicked: countField.focus = false
                }

                onClicked: {
                    headerContainer.searchField.focus = false
                    var page = pageStack.push(Qt.resolvedUrl("CardPage.qml"),
                                              {currentIndex: index, cardmodel: cardModel})

                    function repositionView() {
                        if (page.status === PageStatus.Deactivating)
                        {
                            page.statusChanged.disconnect(repositionView)
                            cardGridView.positionViewAtIndex(page.currentIndex, ListView.Contain)
                        }
                    }

                    page.statusChanged.connect(repositionView)
                }
            }

            VerticalScrollDecorator {}

        }
    }

    Component {
        id: cardListComponent
        SilicaListView {
            id: cardListView
            anchors.fill: parent

            currentIndex: -1
            header: Item {
                id: header
                width: headerContainer.width
                height: headerContainer.height
                Component.onCompleted: headerContainer.parent = header
            }

            ViewPlaceholder {
                enabled: cardListView.count === 0
                verticalOffset: headerContainer.height/2
                text: "No cards in collection"
            }

            Loader {
                sourceComponent: sharedPullDown
            }

            onMovingVerticallyChanged: {
                headerContainer.searchField.focus = false
            }

            model: cardModel

            //DISABLED BUG 2
//            section.property: cardModel.sectionProperty
//            section.criteria: ViewSection.FullString
//            section.delegate: SectionHeader {
//                text: cardModel.sectionExtraLabel + section
//            }
            //END DISABLED

            delegate: ListItem {
                id: listDelegate

                property int delegateMargin: Theme.paddingLarge

                contentHeight: Math.max(delegateRow.height, textColumn.height) + delegateMargin

                menu: ContextMenu {
                    MenuItem {
                        text: "Remove card"
                        onClicked: {
                            listDelegate.remorseAction("Removing card", function () {
                                collectionModel.removeCard(id, count)
                            })
                        }
                    }
                }

                MouseArea {
                    id: countArea
                    anchors {
                        left: parent.left
                        top: parent.top
                        bottom: parent.bottom
                    }
                    width: delegateRow.x + delegateRow.width

                    Row {
                        id: delegateRow
                        x: delegateMargin

                        anchors.verticalCenter: parent.verticalCenter

                        spacing: Theme.paddingMedium

                        TextField {
                            id: countField
                            anchors {
                                verticalCenter: parent.verticalCenter
                            }

                            horizontalAlignment: TextInput.AlignHCenter

                            textMargin: 0
                            labelVisible: false

                            placeholderText: "?"
                            placeholderColor: "#ff4d4d"

                            inputMethodHints: Qt.ImhDigitsOnly
                            validator: RegExpValidator { regExp: /^[1-9][0-9]*/ }

                            font.bold: true
                            font.pixelSize: Theme.fontSizeLarge

                            background: null

                            onTextChanged: {
                                if (text != model.count && text > 0)
                                {
                                    cardModel.dynamicSortFilter = false
                                    model.count = text
                                    cardModel.dynamicSortFilter = true
                                }
                            }
                            Binding {
                                target: countField
                                property: "text"
                                value: model.count
                            }

                            EnterKey.iconSource: "image://theme/icon-m-enter-close"
                            EnterKey.onClicked: countField.focus = false
                        }

                        Loader {
                            id: thumbLoader
                            width: active ? listDelegate.width/4 : 0
                            anchors {
                                verticalCenter: parent.verticalCenter
                            }
                            active: thumbnail

                            sourceComponent: Component {
                                Image {
                                    asynchronous: true
                                    fillMode: Image.PreserveAspectFit
                                    source: "image://cardArt/" + thumbnail
                                }
                            }
                        }

                    }
                    onClicked: countField.forceActiveFocus()
                }

                Column {
                    id: textColumn
                    anchors {
                        left: countArea.right
                        right: parent.right
                        verticalCenter: parent.verticalCenter

                        leftMargin: Theme.paddingMedium
                        rightMargin: delegateMargin
                    }

                    Label {
                        id: nameLabel
                        width: parent.width
                        wrapMode: Text.WordWrap
                        font.pixelSize: Theme.fontSizeMedium
                        color: listDelegate.highlighted ? Theme.highlightColor : Theme.primaryColor

                        textFormat: Text.StyledText
                        text: Theme.highlightText(name, cardModel.stringRegExp, Theme.highlightColor)
                    }
                    Label {
                        id: tradeTypeLabel
                        width: parent.width
                        wrapMode: Text.WordWrap
                        font.pixelSize: Theme.fontSizeSmall
                        color: listDelegate.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor

                        text: trade + ' ' + type
                    }
                }

                onClicked: {
                    headerContainer.searchField.focus = false
                    var page = pageStack.push(Qt.resolvedUrl("CardPage.qml"),
                                              {currentIndex: index, cardmodel: cardModel})

                    function repositionView() {
                        if (page.status === PageStatus.Deactivating)
                        {
                            page.statusChanged.disconnect(repositionView)
                            cardListView.positionViewAtIndex(page.currentIndex, ListView.Contain)
                        }
                    }

                    page.statusChanged.connect(repositionView)
                }
            }

            VerticalScrollDecorator {}
        }
    }
}
