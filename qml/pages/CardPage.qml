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
    id: cardPage

    property alias currentIndex: cardListView.currentIndex
    property alias cardmodel: cardListView.model

    backNavigation: cardListView.interactive

    SilicaListView {
        id: cardListView
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange

        pressDelay: 0

        anchors.fill: parent

        delegate: CardImage {
            id: delegateCard
            width: cardPage.width
            height: cardPage.height
            source: model.image

            onScaledChanged: {
                if (scaled) {
                    cardListView.interactive = false
                }
                else {
                    cardListView.interactive = true
                }
            }

            Loader {
                active: model.image === ""
                source: "../components/GetImageText.qml"
                anchors.centerIn: parent
                width: parent.width - 2*Theme.paddingLarge
            }
        }

        VerticalScrollDecorator {}
    }
}





