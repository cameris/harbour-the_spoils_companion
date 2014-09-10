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
    id: searchPage
    objectName: "cardSearchPage"

    property string activeLayout: settings.value(cardModel.settingsSection + "/layout", "list")

    function scrollToTop() {
        viewLoader.item.scrollToTop()
    }

    onStatusChanged: {
        if (status === PageStatus.Deactivating)
        {
            cardModel.saveSettings()
        }
    }

    CardProxyModel {
        id: cardModel
        source: CardModel {}
        settingsSection: "search"
    }

    Loader {
        id: viewLoader
        anchors.fill: parent
        sourceComponent: activeLayout === "list" ? cardListComponent : cardGridComponent
    }

    SearchHeader {
        id: headerContainer
        width: searchPage.width
        cardModel: cardModel

        header: PageHeader { title: "Card Search" }
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

            delegate: BackgroundItem {
                id: gridDelegate
                width: cardGridView.cellWidth
                height: cardGridView.cellHeight

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

            delegate: BackgroundItem {
                id: listDelegate

                property int delegateMargin: Theme.paddingLarge

                height: Math.max(thumbLoader.height, textColumn.height) + delegateMargin

                Loader {
                    id: thumbLoader
                    x: delegateMargin
                    width: active ? parent.width/4 : 0
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
                Column {
                    id: textColumn
                    anchors {
                        left: thumbLoader.right
                        right: parent.right
                        verticalCenter: parent.verticalCenter

                        leftMargin: thumbLoader.active ? Theme.paddingMedium : 0
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