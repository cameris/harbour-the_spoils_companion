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

Column {
    property var fontSize: Theme.fontSizeSmall

    spacing: Theme.paddingSmall

    Label {
        width: parent.width
        wrapMode: Text.WordWrap
        font.pixelSize: fontSize
        text: "To see the card images, go to"
    }

    OpenBrowser {
        id: openBrowser
        width: parent.width
        height: Theme.itemSizeSmall
        url: "http://www.theworldsnotes.com"

        Label {
            anchors.centerIn: parent
            wrapMode: Text.WordWrap
            font.pixelSize: fontSize
            color: Theme.highlightColor
            text: openBrowser.url.replace(/^http:\/\//, '')
        }
    }

    Label {
        width: parent.width
        wrapMode: Text.WordWrap
        font.pixelSize: fontSize
        text: "and download all the official OCTGN image packs (*.o8c)."
    }
    Label {
        width: parent.width
        wrapMode: Text.WordWrap
        font.pixelSize: fontSize
        text: "After downloading, unzip all files to some directory. The" +
              " directory should be hidden or all images clutter up the gallery" +
              " app."
    }
    Label {
        width: parent.width
        wrapMode: Text.WordWrap
        font.pixelSize: fontSize
        text: "Example:"
    }
    Label {
        width: parent.width
        wrapMode: Text.WordWrap
        font.pixelSize: fontSize
        color: Theme.highlightColor
        text: "unzip /path/to/TheSpoils-1.3.0.0.o8c -d /path/to/sd/.spoils-images"
    }
    Label {
        width: parent.width
        wrapMode: Text.WordWrap
        font.pixelSize: fontSize
        text: "Since there is currently no FilePicker, create a symbolic link" +
              " to the directory:"
    }
    Label {
        width: parent.width
        wrapMode: Text.WordWrap
        font.pixelSize: fontSize
        color: Theme.highlightColor
        text: "ln -s /path/to/sd/.spoils-images/ ~/spoils-images"
    }
}
