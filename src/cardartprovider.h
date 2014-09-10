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

#ifndef CARDARTPROVIDER_H
#define CARDARTPROVIDER_H

#include <QQuickImageProvider>
#include <QImageReader>

class CardArtProvider : public QQuickImageProvider
{
public:
    CardArtProvider() : QQuickImageProvider(QQuickImageProvider::Image,
                                            QQuickImageProvider::ForceAsynchronousImageLoading)
    {
    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize)
    {
        QImage art;
        QSize originalSize;

        if (id == "")
            return art;

        QImageReader imageReader(id);
        if (!imageReader.canRead())
            return art;

        originalSize = imageReader.size();

        if (originalSize.isValid())
        {
            QPoint artCenter((originalSize.width() -1) / 2, (originalSize.height() -1) * 0.368);
            QRect cr(0, 0, originalSize.width() * 0.842, originalSize.height() * 0.368);
            cr.moveCenter(artCenter);
            imageReader.setClipRect(cr);
        }
        art = imageReader.read();

        if (size)
            *size = art.size();

        if (requestedSize.isValid() && requestedSize.width() > 0 && requestedSize.height() > 0)
        {
            return art.scaled(requestedSize, Qt::KeepAspectRatio);
        }
        else
        {
            return art;
        }
    }
};

#endif // CARDARTPROVIDER_H
