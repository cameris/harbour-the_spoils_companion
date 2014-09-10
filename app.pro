# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-the_spoils_companion

CONFIG += sailfishapp c++11
QT += sql

SOURCES += \
    src/harbour-the_spoils_companion.cpp \
    src/cardmodel.cpp \
    src/card.cpp \
    src/cardproxymodel.cpp \
    src/dbmanager.cpp \
    src/categoryfilter.cpp \
    src/settings.cpp \
    src/numericfilter.cpp \
    src/collectionmodel.cpp \
    src/countcardmodel.cpp \
    src/deckcardmodel.cpp \
    src/deckfactionmodel.cpp \
    src/deck.cpp \
    src/deckmodel.cpp

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    translations/*.ts \
    harbour-the_spoils_companion.desktop \
    qml/pages/CardPage.qml \
    qml/components/CounterLabel.qml \
    qml/pages/MainPage.qml \
    qml/components/CardImage.qml \
    qml/pages/SearchPage.qml \
    qml/components/SearchHeader.qml \
    qml/components/GridContextMenu.qml \
    qml/pages/InfluenceCounter.qml \
    qml/pages/CardPicker.qml \
    qml/pages/CardDialog.qml \
    qml/pages/CollectionPage.qml \
    qml/pages/DeckListPage.qml \
    qml/pages/DeckPage.qml \
    qml/pages/DeckStatsPage.qml \
    qml/harbour-the_spoils_companion.qml \
    rpm/harbour-the_spoils_companion.changes.in \
    rpm/harbour-the_spoils_companion.yaml \
    rpm/harbour-the_spoils_companion.spec \
    qml/components/GetImageText.qml \
    qml/components/OpenBrowser.qml \
    qml/pages/AboutPage.qml

# additional files to install
cardDB.files = cards.sqlite
cardDB.path = /usr/share/$${TARGET}

INSTALLS += cardDB

DEFINES += SPOILS_OCTGNID=\\\"844d5fe3-bdb5-4ad2-ba83-88c2c2db6d88\\\"

# to disable building translations every time, comment out the
# following CONFIG line
#CONFIG += sailfishapp_i18n
#TRANSLATIONS += translations/harbour-the_spoils_companion-de.ts

HEADERS += \
    src/cardmodel.h \
    src/card.h \
    src/cardproxymodel.h \
    src/dbmanager.h \
    src/categoryfilter.h \
    src/settings.h \
    src/numericfilter.h \
    src/collectionmodel.h \
    src/cardartprovider.h \
    src/countcardmodel.h \
    src/deckcardmodel.h \
    src/deckfactionmodel.h \
    src/deck.h \
    src/deckmodel.h

