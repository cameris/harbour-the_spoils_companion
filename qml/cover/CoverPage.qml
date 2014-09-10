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

CoverBackground {
    Rectangle {
        anchors.fill: parent
        color: Qt.rgba(1,1,1,0.2)
    }

    Image {
        anchors.centerIn: parent
        width: parent.width * 0.9
        sourceSize.width: parent.width
        smooth: true
        fillMode: Image.PreserveAspectFit
        source: "../icons/logo.png"
    }

    CoverActionList {
        CoverAction {
            iconSource: "image://theme/icon-cover-play"
            onTriggered: {
                while(pageStack.depth>1) {
                    pageStack.pop(undefined, PageStackAction.Immediate)
                }
                pageStack.push(Qt.resolvedUrl("../pages/InfluenceCounter.qml"))
                mainWindow.activate()
            }
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-search"
            onTriggered: {
                var searchPage = pageStack.find(function (page) {
                    if (page.objectName == "cardSearchPage")
                        return true
                })

                if (searchPage)
                {
                    pageStack.pop(searchPage, PageStackAction.Immediate)
                    searchPage.scrollToTop()
                }
                else
                {
                    while(pageStack.depth>1) {
                        pageStack.pop(undefined, PageStackAction.Immediate)
                    }
                    searchPage = pageStack.push(Qt.resolvedUrl("../pages/SearchPage.qml"))
                }

                mainWindow.activate()
            }
        }
    }
}


