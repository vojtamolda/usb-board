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

#ifndef QATTENUATORWINDOW_H
#define QATTENUATORWINDOW_H

#include <QApplication>
#include <QTranslator>
#include <QMainWindow>
#include <QSignalMapper>

#include "ui_attenuatorwindow.h"

class QAttenuatorWindow : public QMainWindow, private Ui::QAttenuatorWindow {
    Q_OBJECT
public slots:
    void connect();
    void disconnect();
    void about();

    void setLanguage(const QString &language);

public:
    QAttenuatorWindow(QWidget *parent = NULL);
    ~QAttenuatorWindow();
    void changeEvent(QEvent *event);

    void storeSettings(QSettings &settings) const;
    void restoreSettings(QSettings &settings);

    QString getLanguage() const;

private slots:
    void attenuatorConnected(bool state);

private:
   QString* language;
   QSettings* settings;
   QSignalMapper* languageSignalMapper;

   static const struct AttenuatorWindowSettingKey {
       static QString Language() { return QString("Language"); }
       static QString WindowState() { return QString("WindowState"); }
       static QString AttenuatorWidget() { return QString("AttenuatorWidget"); }
   } SettingKey;

};

#endif // QATTENUATORWINDOW_H
