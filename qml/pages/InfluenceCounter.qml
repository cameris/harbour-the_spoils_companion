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
import harbour.the_spoils_companion.keepalive 1.0

import "../components"

Page {
    id: gamePage
    showNavigationIndicator: false

    property bool mirrorOpponent: (settings.value("game/mirrorOpponent", "true") === "true")
    property alias opponentCounter: opponentLabel.counter
    property alias myCounter: myLabel.counter
    property var oldCover

    Component.onCompleted: {
        oldCover = mainWindow.cover
        mainWindow.cover = gameCover
        opponentCounter = settings.value("game/opponentLastInfluence", 25)
        myCounter = settings.value("game/myLastInfluence", 25)
    }

    onStatusChanged: {
        if (status === PageStatus.Deactivating)
        {
            saveCounters()
            mainWindow.cover = oldCover

            // allow blanking when leaving the page
            DisplayBlanking.preventBlanking = false
            mainWindow.applicationActiveChanged.disconnect(blankOnApplicationActive)
        }
        else if (status === PageStatus.Active)
        {
            DisplayBlanking.preventBlanking = (settings.value("game/preventBlanking", "false") === "true")
            mainWindow.applicationActiveChanged.connect(blankOnApplicationActive)
        }
    }

    function saveCounters () {
        settings.setValue("game/opponentLastInfluence", opponentCounter)
        settings.setValue("game/myLastInfluence", myCounter)
    }

    function resetGame() {
        var startInfluence = settings.value("game/startInfluence", 25)
        opponentCounter = startInfluence
        myCounter = startInfluence
    }

    function blankOnApplicationActive() {
        if (mainWindow.applicationActive)
        {
            DisplayBlanking.preventBlanking = (settings.value("game/preventBlanking", "false") === "true")
        }
        else
        {
            // allow blanking when app is not active
            DisplayBlanking.preventBlanking = false
        }
    }

    function toggleBlanking() {
        DisplayBlanking.preventBlanking = !DisplayBlanking.preventBlanking
        settings.setValue("game/preventBlanking", DisplayBlanking.preventBlanking)
    }

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: parent.height

        PullDownMenu {
            id: opponentMenu
            enabled: visible
            MenuItem {
                text: "Reset game"
                rotation: 180
                onClicked: {
                    resetGame()
                }
            }
        }
        PushUpMenu {
            id: myMenu
            MenuItem {
                text: "Reset game"
                onClicked: {
                    resetGame()
                }
            }
            MenuItem {
                text: "Toggle mirror"
                onClicked: {
                    gamePage.mirrorOpponent = !gamePage.mirrorOpponent
                    settings.setValue("game/mirrorOpponent", gamePage.mirrorOpponent)
                    opponentMenu.visible = !opponentMenu.visible
                }
            }
            MenuItem {
                text: (drawer.opened ? "Save" : "Edit") + " starting influence"
                onClicked: {
                    if (drawer.opened)
                    {
                        settings.setValue("game/startInfluence", startInfluenceLabel.counter)
                        drawer.hide()
                    }
                    else
                    {
                        startInfluenceLabel.counter = settings.value("game/startInfluence", 25)
                        drawer.show()
                    }
                }
            }
            MenuItem {
                text: (DisplayBlanking.preventBlanking ? "Allow" : "Prevent") + " screen blanking"
                onClicked: {
                    toggleBlanking()
                }
            }
        }

        Drawer {
            id: drawer

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            height: parent.height/2

            dock: Dock.Top

            background: Item {
                anchors.fill: parent
                Rectangle {
                    anchors.fill: parent
                    color: Theme.highlightBackgroundColor
                    opacity: Theme.highlightBackgroundOpacity
                }
                CounterLabel {
                    id: startInfluenceLabel
                    anchors.fill: parent
                }
            }
            backgroundSize: height

            CounterLabel {
                id: opponentLabel
                anchors.fill: parent
                rotation: mirrorOpponent ? 180 : 0
                opponentCounter: gamePage.myCounter
                opponentCounterVisible: gamePage.mirrorOpponent
            }
        }

        Separator {
            anchors {
                left: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
        }

        CounterLabel {
            id: myLabel
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            opponentCounter: gamePage.opponentCounter
            opponentCounterVisible: gamePage.mirrorOpponent
        }
    }

    // cover stuff
    CoverBackground {
        id: gameCover
        Rectangle {
            anchors.fill: parent
            color: Qt.rgba(1,1,1,0.2)
        }
        Column {
            anchors.centerIn: parent
            width: parent.width
            spacing: Theme.paddingMedium

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.8
                sourceSize.width: parent.width
                smooth: true
                fillMode: Image.PreserveAspectFit
                source: "../icons/logo.png"
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: Theme.fontSizeHuge
                font.bold: true
                text: gamePage.opponentCounter
            }

            Separator {
                width: parent.width
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: Theme.fontSizeHuge
                font.bold: true
                text: gamePage.myCounter
            }
        }

        onStatusChanged: {
            if (status === Cover.Activating)
            {
                saveCounters()
            }
        }
    }
}
