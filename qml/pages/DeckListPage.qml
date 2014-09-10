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
    id: deckListPage

    property int lastDeckEdit: -1
    onStatusChanged: {
        if (status === PageStatus.Activating && lastDeckEdit >= 0)
        {
            deckView.model.updateCounts(lastDeckEdit)
            lastDeckEdit = -1
        }
    }

    SilicaListView {
        id: deckView
        anchors.fill: parent

        header: PageHeader {
            title: "Decks"
        }

        ViewPlaceholder {
            enabled: deckView.count === 0
            text: "No decks available"
        }

        function tradeIconMap(trade) {
            switch (trade) {
            case 'Arcanist':
                return '../icons/obsession.png'
            case 'Banker':
                return '../icons/greed.png'
            case 'Gearsmith':
                return '../icons/elitism.png'
            case 'Rogue':
                return '../icons/deception.png'
            case 'Universal':
                return '../icons/volition.png'
            case 'Warlord':
                return '../icons/rage.png'
            }
        }

        PullDownMenu {
            MenuItem {
                text: "Add deck"
                onClicked: {
                    var dialog = pageStack.push(deckEdit, {
                                                    title: "New deck"
                                                })

                    dialog.accepted.connect(function () {
                        deckView.model.addDeck(dialog.name)
                        deckView.positionViewAtEnd()
                    })
                }
            }
        }


        model: DeckModel {}

        delegate: ListItem {
            id: listDelegate
            contentHeight: Math.max(Theme.itemSizeMedium, contentColumn.height)

            showMenuOnPressAndHold: true
            menu: ContextMenu {
                MenuItem {
                    text: "Statistics"
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("DeckStatsPage.qml"), {
                                                          deck: deckView.model.getDeck(index)
                                                      })
                    }
                }
                MenuItem {
                    text: "Export as OCTGN deck"
                    onClicked: {
                        deckView.model.exportDeckOCTGN(index)
                    }
                }

                MenuItem {
                    text: "Edit deck"
                    onClicked: {
                        var dialog = pageStack.push(deckEdit, {
                                                        title: "Edit deck",
                                                        name: model.name
                                                    })

                        dialog.accepted.connect(function () {
                            model.name = dialog.name
                        })
                    }
                }
                MenuItem {
                    text: "Remove deck"
                    onClicked: {
                        listDelegate.remorseAction("Removing deck", function () {
                            deckView.model.removeDeck(index)
                        })
                    }
                }
            }

            Column {
                id: contentColumn
                anchors {
                    left: parent.left
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    leftMargin: Theme.paddingLarge
                    rightMargin: anchors.leftMargin
                }

                Label {
                    width: parent.width - 2*parent.anchors.leftMargin
                    truncationMode: TruncationMode.Fade
                    color: listDelegate.highlighted ? Theme.highlightColor : Theme.primaryColor

                    text: model.name
                }
                Row {
                    Label {
                        font.pixelSize: Theme.fontSizeSmall
                        color: listDelegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                        text: totalCount + " cards:"
                    }

                    spacing: Theme.paddingMedium
                    Repeater {
                        model: tradeCounts
                        delegate: Row {
                            spacing: Theme.paddingSmall/2
                            Image {
                                anchors.verticalCenter: parent.verticalCenter
                                source: deckView.tradeIconMap(modelData.name)
                            }

                            Label {
                                textFormat: Text.StyledText
                                font.pixelSize: Theme.fontSizeSmall
                                color: listDelegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                                text: modelData.count
                            }
                        }
                    }
                }
            }

            onClicked: {
                deckListPage.lastDeckEdit = index
                pageStack.push(Qt.resolvedUrl("DeckPage.qml"), {
                                   deckId: model.id,
                                   deckName: model.name
                               })
            }

        }
    }

    Component {
        id: deckEdit
        Dialog {
            property alias title: header.title
            property alias name: nameInput.text

            Column {
                id: column
                width: parent.width

                DialogHeader {
                    id: header
                }

                TextField {
                    id: nameInput
                    anchors {
                        margins: Theme.paddingLarge
                        left: parent.left
                        right: parent.right
                    }

                    label: "Deck name"
                    placeholderText: "Insert deck name"
                    validator: RegExpValidator { regExp: /^.+$/ }
                    inputMethodHints: Qt.ImhNoPredictiveText

                    EnterKey.iconSource: "image://theme/icon-m-enter-close"
                    EnterKey.onClicked: focus = false

                    Component.onCompleted: focus = true
                }
            }
            canAccept: nameInput.acceptableInput
        }
    }
}
