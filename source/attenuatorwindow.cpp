/*
QAttenuatorWindow - Main window of application that controls USB Attenuator
PCB based on FT245 IC.

Copyright (C) 2009  Vojta Molda <vojta.molda@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include "attenuatorwindow.h"

QAttenuatorWindow::QAttenuatorWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    actionCut->setShortcut(QKeySequence::Cut);
    actionCopy->setShortcut(QKeySequence::Copy);
    actionPaste->setShortcut(QKeySequence::Paste);
    QObject::connect(actionAbout, SIGNAL(triggered()),
                     this, SLOT(about()));
    QObject::connect(actionConnect, SIGNAL(triggered()),
                     this, SLOT(connect()));
    QObject::connect(actionDisconnect, SIGNAL(triggered()),
                     this, SLOT(disconnect()));

    languageSignalMapper = new QSignalMapper(this);
    languageSignalMapper->setMapping(actionDefault, "default");
    languageSignalMapper->setMapping(actionEnglish, "en");
    languageSignalMapper->setMapping(actionGerman, "de");
    languageSignalMapper->setMapping(actionFrench, "fr");
    languageSignalMapper->setMapping(actionCzech, "cs");
    QObject::connect(actionDefault, SIGNAL(triggered()),
                     languageSignalMapper, SLOT(map()));
    QObject::connect(actionEnglish, SIGNAL(triggered()),
                     languageSignalMapper, SLOT(map()));
    QObject::connect(actionGerman, SIGNAL(triggered()),
                     languageSignalMapper, SLOT(map()));
    QObject::connect(actionFrench, SIGNAL(triggered()),
                     languageSignalMapper, SLOT(map()));
    QObject::connect(actionCzech, SIGNAL(triggered()),
                     languageSignalMapper, SLOT(map()));
    QObject::connect(languageSignalMapper, SIGNAL(mapped(QString)),
                     this, SLOT(setLanguage(QString)));

    QObject::connect(attenuatorWidget, SIGNAL(attenuatorConnected(bool)),
                     this, SLOT(attenuatorConnected(bool)));

    settings = new QSettings(qApp->organizationName(), qApp->applicationName(), this);
    restoreSettings(*settings);
}

void QAttenuatorWindow::connect() {
    attenuatorWidget->connect();
}

void QAttenuatorWindow::attenuatorConnected(bool state) {
    actionDisconnect->setEnabled(state);
    actionConnect->setEnabled(!state);
}

void QAttenuatorWindow::disconnect() {
    attenuatorWidget->disconnect();
}

void QAttenuatorWindow::storeSettings(QSettings& settings) const {
    settings.setValue(SettingKey.Language(), getLanguage());
    settings.setValue(SettingKey.WindowState(), saveState());
    settings.setValue(SettingKey.AttenuatorWidget(), true);
    settings.beginGroup(SettingKey.AttenuatorWidget());
    attenuatorWidget->storeSettings(settings);
    settings.endGroup();
}

void QAttenuatorWindow::restoreSettings(QSettings& settings) {
    setLanguage(settings.value(SettingKey.Language(), "default").toString());
    restoreState(settings.value(SettingKey.WindowState()).toByteArray());
    settings.beginGroup(SettingKey.AttenuatorWidget());
    attenuatorWidget->restoreSettings(settings);
    settings.endGroup();
}

QString QAttenuatorWindow::getLanguage() const {
    return *language;
}

void QAttenuatorWindow::setLanguage(const QString &language) {
    static QTranslator translator;
    this->language = new QString(language);
    qApp->removeTranslator(&translator);
    if (language == "default") {
        translator.load(QLocale::system(), "", "", ":/language/", ".qm");
    } else {
        translator.load(":/language/" + language);
    }
    qApp->installTranslator(&translator);
}

void QAttenuatorWindow::about() {
    QString title = tr("About");
    QString message = tr("<h2>%1 %2</h2>"
                         "<p>Distribute under terms of the GNU GPL license.</p>"
                         "<p>Vojta Molda <a href='mailto:vojta.molda@gmail.com'>"
                         "&lt;vojta.molda@gmail.com&gt;</a></p>")
                        .arg(qApp->applicationName(), qApp->applicationVersion());
    QMessageBox::about(this, title, message);
}

void QAttenuatorWindow::changeEvent(QEvent *event) {
    QMainWindow::changeEvent(event);
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi(this);
    }
}

QAttenuatorWindow::~QAttenuatorWindow() {
    storeSettings(*settings);
    delete language;
}
