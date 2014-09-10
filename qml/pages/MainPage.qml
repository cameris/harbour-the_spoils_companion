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

import "../components"

Page {

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                text: "About"
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
                }
            }
        }

        Image {
            id: pageHeader
            anchors {
                top: parent.top
                right: parent.right
            }
            source: "../icons/page-logo.png"
            sourceSize.width: parent.width * 2/5

            OpenBrowser {
                anchors.fill: parent
                url: "http://www.thespoils.com"
            }
        }

        Column {
            id: column
            width: parent.width

            Rectangle {
                opacity: 0
                width: 1
                height: pageHeader.height * 2/3
            }

            BackgroundItem {
                id: game
                width: parent.width
                height: Theme.itemSizeLarge

                Row {
                    x: Theme.paddingLarge
                    height: parent.height
                    spacing: Theme.paddingMedium

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        source: "image://theme/icon-m-play" +
                                (game.highlighted ? "?" + Theme.highlightColor : "")
                    }
                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        color: game.highlighted ? Theme.highlightColor : Theme.primaryColor
                        text: "Influence Counter"
                    }
                }
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("InfluenceCounter.qml"))
                }
            }

            BackgroundItem {
                id: cards
                width: parent.width
                height: Theme.itemSizeLarge

                Row {
                    x: Theme.paddingLarge
                    height: parent.height
                    spacing: Theme.paddingMedium

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        source: "image://theme/icon-m-search" +
                                (cards.highlighted ? "?" + Theme.highlightColor : "")
                    }
                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        color: cards.highlighted ? Theme.highlightColor : Theme.primaryColor
                        text: "Card Search"
                    }
                }
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("SearchPage.qml"))
                }
            }

            BackgroundItem {
                id: decks
                width: parent.width
                height: Theme.itemSizeLarge

                Row {
                    x: Theme.paddingLarge
                    height: parent.height
                    spacing: Theme.paddingMedium

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        source: "image://theme/icon-m-levels" +
                                (decks.highlighted ? "?" + Theme.highlightColor : "")
                    }
                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        color: decks.highlighted ? Theme.highlightColor : Theme.primaryColor
                        text: "Decks"
                    }
                }
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("DeckListPage.qml"))
                }
            }

            BackgroundItem {
                id: collection
                width: parent.width
                height: Theme.itemSizeLarge

                Row {
                    x: Theme.paddingLarge
                    height: parent.height
                    spacing: Theme.paddingMedium

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        source: "image://theme/icon-m-document" +
                                (collection.highlighted ? "?" + Theme.highlightColor : "")
                    }
                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        color: collection.highlighted ? Theme.highlightColor : Theme.primaryColor
                        text: "Collection"
                    }
                }
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("CollectionPage.qml"))
                }
            }
        }
    }
}
