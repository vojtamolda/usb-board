/*
QAttenuatorWidget - Controller widget that manages USB Signal Attenuator
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

#include "attenuatorwidget.h"

QAttenuatorWidget::QAttenuatorWidget(QWidget* parent) :
    QWidget(parent), attenuator(NULL)
{
    setupUi(this);

    deviceTableModel = new QFtdiDeviceTableModel(deviceTableView);
    QObject::connect(deviceTableView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
                     this, SLOT(syncAttenuator(QModelIndex,QModelIndex)));

    QObject::connect(connectButton, SIGNAL(clicked()),
                     this, SLOT(connect()));
    QObject::connect(disconnectButton, SIGNAL(clicked()),
                     this, SLOT(disconnect()));
    QObject::connect(refreshButton, SIGNAL(clicked()),
                     this, SLOT(refreshAttenuators()));
    QObject::connect(settingsButton, SIGNAL(toggled(bool)),
                     this, SLOT(toggleSettings(bool)));
    QObject::connect(attenuationSlider, SIGNAL(valueChanged(const int)),
                     this, SLOT(transmit(const int)));

    toggleSettings(false);
}

void QAttenuatorWidget::connect() {
    if (attenuator != NULL) {
        QObject::connect(this->attenuator, SIGNAL(deviceConnected(bool)),
                        this, SLOT(deviceConnected(bool)));
        QObject::connect(this->attenuator, SIGNAL(deviceFailed(int)),
                        this, SLOT(deviceFailed(int)));
        attenuator->connect((char)0xFF);
        transmit(attenuationSlider->value());
    } else {
        emit attenuatorConnected(false);
    }
}

void QAttenuatorWidget::transmit(const int attenuation) {
    if (attenuator != NULL) {
        attenuator->transmit((uchar*)&attenuation, 1);
    }
}

void QAttenuatorWidget::disconnect() {
    if (attenuator != NULL) {
        attenuator->disconnect();
    } else {
        emit attenuatorConnected(false);
    }
}

void QAttenuatorWidget::deviceConnected(bool state) {
    attenuationSlider->setEnabled(state);
    attenuationSpinBox->setEnabled(state);
    connectButton->setEnabled(!state);
    disconnectButton->setEnabled(state);
    refreshButton->setEnabled(!state);
    deviceTableView->setEnabled(!state);
    emit attenuatorConnected(state);
}

void QAttenuatorWidget::deviceFailed(int error) {
    QMessageBox messageBox;
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.setInformativeText(tr("Please verify that the device is properly connected."));
    if (attenuator != NULL) messageBox.setDetailedText(attenuator->getErrorMessage());
    if (error == QFtdiDevice::Error.InitFailed) {
        messageBox.setText(tr("Initialization failed."));
    } else if (error == QFtdiDevice::Error.ConnectFailed) {
        messageBox.setText(tr("Connection failed."));
    } else if (error == QFtdiDevice::Error.TransmitFailed) {
        messageBox.setText(tr("Data transmission failed."));
    } else if (error == QFtdiDevice::Error.DisconnectFailed) {
        messageBox.setText(tr("Disconnect failed."));
    } else {
        return;
    }
    messageBox.exec();
    emit attenuatorFailed(error);
    disconnect();
}

void QAttenuatorWidget::refreshAttenuators() {
    deviceTableModel->refreshDeviceList(Default.Vid, Default.Pid);
    QModelIndex indexToSelect = deviceTableModel->getIndex(attenuator);
    if (indexToSelect.isValid()) {
        deviceTableView->selectionModel()->clearSelection();
        deviceTableView->selectionModel()->select(indexToSelect, QItemSelectionModel::Rows);
    }
}

void QAttenuatorWidget::syncAttenuator(QModelIndex currentSelection, QModelIndex /* pastSelection */) {
    setAttenuator(deviceTableModel->getDevice(currentSelection));
}


void QAttenuatorWidget::toggleSettings(bool state) {
    settingsButton->blockSignals(true);
    settingsButton->setChecked(state);
    settingsButton->blockSignals(false);
    settingsGroupBox->setVisible(state);
    if (state == true) {
        deviceTableView->setFocus();
        refreshAttenuators();
    }
}

void QAttenuatorWidget::storeSettings(QSettings &settings) const {
    settings.setValue(SettingKey.Attenuation(), attenuationSlider->value());
    if ((attenuator != NULL) && (attenuator->isConnected())) {
        settings.setValue(SettingKey.Attenuator(), true);
        settings.beginGroup(SettingKey.Attenuator());
        attenuator->storeSettings(settings);
        settings.endGroup();
    } else {
        settings.remove(SettingKey.Attenuator());
    }
}

void QAttenuatorWidget::restoreSettings(QSettings &settings) {
    attenuationSlider->setValue(settings.value(SettingKey.Attenuation(), Default.Attenuation).toInt());
    if (settings.childGroups().contains(SettingKey.Attenuator())) {
        settings.beginGroup(SettingKey.Attenuator());
        setAttenuator(new QFtdiDevice(settings, this));
        settings.endGroup();
        connect();
    } else {
        setAttenuator(NULL);
    }
}

void QAttenuatorWidget::setAttenuator(QFtdiDevice *attenuator) {
    if (this->attenuator != NULL) {
        disconnect();
        delete this->attenuator;
    }
    this->attenuator = attenuator;
    if (this->attenuator != NULL) {
        this->attenuator->setParent(this);
    }
}

QFtdiDevice* QAttenuatorWidget::getAttenuator() const {
    return attenuator;
}

void QAttenuatorWidget::changeEvent(QEvent *event) {
    QWidget::changeEvent(event);
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi(this);
    }
}
