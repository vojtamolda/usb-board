/*
QBoardWindow - Main window of application that controls USB Measuring Board
based on FT232 and ATmega1280 ICs.

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

#ifndef QBOARDWINDOW_H
#define QBOARDWINDOW_H

#include <QApplication>
#include <QTranslator>
#include <QMainWindow>
#include <QSignalMapper>
#include <QMessageBox>
#include <QToolBar>
#include <QSettings>
#include <QLocale>

#include "ui_boardwindow.h"
#include "boardwidget.h"
#include "boardsettings.h"

class QBoardWindow: public QMainWindow, private Ui::QBoardWindow  {
    Q_OBJECT
public slots:
    void connect();
    void disconnect();
    void changeSettings();
    void about();

    void setLanguage(const QString &language);

public:
    QBoardWindow(QWidget* parent = NULL);
    ~QBoardWindow();
    void changeEvent(QEvent *event);

    void storeSettings(QSettings &settings) const;
    void restoreSettings(QSettings &settings);

    QString getLanguage() const;

private slots:
    void boardConnected(bool state);

 private:
    QString* language;
    QSettings* settings;
    QSignalMapper* languageSignalMapper;

    static const struct QBoardWindowSettingKey {
        static QString Language() { return QString("Language"); }
        static QString WindowState() { return QString("WindowState"); }
        static QString BoardWidget() { return QString("BoardWidget"); }
    } SettingKey;
};

#endif // QBOARDWINDOW_H
