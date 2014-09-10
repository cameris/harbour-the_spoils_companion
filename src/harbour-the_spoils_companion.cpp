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

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QStandardPaths>

#include "settings.h"
#include "cardmodel.h"
#include "cardproxymodel.h"
#include "categoryfilter.h"
#include "numericfilter.h"
#include "collectionmodel.h"
#include "deckmodel.h"
#include "deck.h"
#include "deckcardmodel.h"
#include "deckfactionmodel.h"

#include "cardartprovider.h"

int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/template.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    QCoreApplication::setOrganizationName("the.spoils");
    QCoreApplication::setApplicationName("harbour.the_spoils_companion");

    QGuiApplication *app = SailfishApp::application(argc, argv);
    QQuickView *view = SailfishApp::createView();

    view->engine()->addImageProvider(QLatin1String("cardArt"), new CardArtProvider);

    qmlRegisterType<CardProxyModel>("harbour.the_spoils_companion", 1, 0, "CardProxyModel");

    qmlRegisterType<CardModel>("harbour.the_spoils_companion", 1, 0, "CardModel");
    qmlRegisterType<CategoryFilter>("harbour.the_spoils_companion", 1, 0, "CategoryFilter");
    qmlRegisterType<NumericFilter>("harbour.the_spoils_companion", 1, 0, "NumericFilter");

    qmlRegisterType<CollectionModel>("harbour.the_spoils_companion", 1, 0, "CollectionModel");

    qmlRegisterType<DeckModel>("harbour.the_spoils_companion", 1, 0, "DeckModel");
    qmlRegisterType<Deck>("harbour.the_spoils_companion", 1, 0, "Deck");
    qmlRegisterType<DeckCardModel>("harbour.the_spoils_companion", 1, 0, "DeckCardModel");
    qmlRegisterType<DeckFactionModel>("harbour.the_spoils_companion", 1, 0, "DeckFactionModel");

    Settings settings;
    if (settings.value("cards/images").toString() == "")
    {
        settings.setValue("cards/images",
                          QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/spoils-images");
    }
    view->rootContext()->setContextProperty("settings", &settings);

    view->setSource(SailfishApp::pathTo("qml/harbour-the_spoils_companion.qml"));
    view->show();

    return app->exec();
}

