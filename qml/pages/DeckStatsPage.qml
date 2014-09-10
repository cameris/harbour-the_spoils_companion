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

Page {
    id: deckStatsPage

    property Deck deck

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height

        Column {
            id: column
            anchors {
                left: parent.left
                right: parent.right
            }

            PageHeader {
                title: deck.name
            }

            Column {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }

                Label {
                    color: deck.buildable ? Theme.primaryColor : "#ff4d4d"
                    font.pixelSize: Theme.fontSizeSmall
                    text: "Can " + (deck.buildable ? "" : "not ") + " be build from collection"
                }
                Item {
                    width: parent.width - 2 * parent.anchors.margins
                    height: Math.max(countLabel.height, nameLabel.height)

                    Label {
                        id: countLabel
                        width: Math.max(parent.width / 10, implicitWidth)
                        horizontalAlignment: Text.AlignRight
                        text: deck.totalCount
                    }
                    Label {
                        id: nameLabel
                        anchors {
                            leftMargin: Theme.paddingMedium
                            left: countLabel.right
                            right: parent.right
                        }

                        wrapMode: Text.WordWrap
                        text: "cards"
                    }
                }
            }

            SectionHeader { text: "Trades" }
            Column {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                Repeater {
                    model: deck.tradeCounts
                    delegate: countComponent
                }
            }

            SectionHeader { text: "Types" }
            Column {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                Repeater {
                    model: deck.typeCounts
                    delegate: countComponent
                }
            }

            SectionHeader { text: "Sets" }
            Column {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                Repeater {
                    model: deck.setCounts
                    delegate: countComponent
                }
            }
        }
    }

    Component {
        id: countComponent
        Item {
            width: parent.width - 2 * parent.anchors.margins
            height: Math.max(countLabel.height, nameLabel.height)

            Label {
                id: countLabel
                width: Math.max(parent.width / 10, implicitWidth)
                horizontalAlignment: Text.AlignRight
                text: modelData.count
            }
            Label {
                id: nameLabel
                anchors {
                    leftMargin: Theme.paddingMedium
                    left: countLabel.right
                    right: parent.right
                }

                wrapMode: Text.WordWrap
                text: modelData.name
            }
        }
    }
}
