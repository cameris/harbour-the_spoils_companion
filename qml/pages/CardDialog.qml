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

Dialog {
    id: cardDialog

    property alias currentIndex: dialogListView.currentIndex
    property alias cardmodel: dialogListView.model
    property string selectedId

    backNavigation: dialogListView.interactive

    DialogHeader {
        id: cardDialogHeader
        title: "Add Card"
        visible: dialogListView.interactive
    }

    SilicaListView {
        id: dialogListView
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange

        pressDelay: 0

        anchors.fill: parent

        delegate: CardImage {
            id: delegateCard
            width: cardDialog.width
            height: cardDialog.height
            source: model.image

            property string octgnId: model.id

            onScaledChanged: {
                if (scaled) {
                    dialogListView.interactive = false
                }
                else {
                    dialogListView.interactive = true
                }
            }

            Loader {
                active: model.image === ""
                sourceComponent: placeholder
                anchors.centerIn: parent
                width: parent.width - 2*Theme.paddingLarge
            }
        }

        VerticalScrollDecorator {}
    }

    Component {
        id: placeholder
        Label {
            wrapMode: Text.WordWrap

            textFormat: Text.StyledText
            font.pixelSize: Theme.fontSizeSmall
            text:  "<p>To see the card images, go to " +
                   "<a href=\"http://www.theworldsnotes.com\"/><font color=\"" +
                   Theme.highlightColor + "\">www.theworldsnotes.com</font></a> " +
                   "and download all the official OCTGN imagepacks (*.o8c).</p>" +
                   "<p>After downloading, unzip all files to some directory. The directory should " +
                   "be hidden or all images clutter up the gallery app.</p>" +
                   "<p>Example:<br>" +
                   "<font color=\"" + Theme.secondaryHighlightColor + "\">" +
                   "unzip /path/to/TheSpoils-1.3.0.0.o8c -d /path/to/sd/.spoils-images" +
                   "</font></p>" +
                   "<p>Since there is currently no FilePicker, create a symbolic link" +
                   " to the directory:<br>" +
                   "<font color=\"" + Theme.secondaryHighlightColor + "\">" +
                   "ln -s /path/to/sd/.spoils-images/ ~/spoils-images" +
                   "</font></p>"
        }
    }

    onAccepted: {
        selectedId = dialogListView.currentItem.octgnId
    }
}
