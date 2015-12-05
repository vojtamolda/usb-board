/*
QBoardSettings - Settings dialog that detects and sets up USB Measuring Boards.

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

#include "boardsettings.h"

QBoardSettings::QBoardSettings(QBoardWidget* boardWidget, QWidget *parent) :
    QDialog(parent, Qt::Dialog), boardWidget(boardWidget)
{
    setupUi(this);

    setVid(QFtdiDevice::Default.Vid);
    setPid(QFtdiDevice::Default.Pid);
    setBoard(boardWidget->getBoard());
    setReferenceVoltage(boardWidget->getReferenceVoltage());

    buttonBox->button(QDialogButtonBox::Ok)->setText(tr("&Connect"));
    buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(QString(":/icon/dialog-ok.png")));
    buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(QString(":/icon/dialog-cancel.png")));
    buttonBox->button(QDialogButtonBox::RestoreDefaults)->setIcon(QIcon(QString(":/icon/edit-undo.png")));
    connect(buttonBox->button(QDialogButtonBox::RestoreDefaults), SIGNAL(clicked()),
            this, SLOT(restoreDefaults()));

    connect(refreshButton, SIGNAL(clicked()),
            this, SLOT(refreshBoards()));
    connect(vidLineEdit, SIGNAL(editingFinished()),
            this, SLOT(syncVid()));
    connect(pidLineEdit, SIGNAL(editingFinished()),
            this, SLOT(syncPid()));
    connect(referenceVoltageSpinBox, SIGNAL(editingFinished()),
            this, SLOT(syncReferenceVoltage()));

    deviceTableModel = new QFtdiDeviceTableModel(deviceTableView);
    connect(deviceTableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(syncBoard(QModelIndex,QModelIndex)));

    refreshBoards();
}

void QBoardSettings::refreshBoards() {
    deviceTableModel->refreshDeviceList(vid, pid);
    QModelIndex indexToSelect = deviceTableModel->getIndex(board);
    if (indexToSelect.isValid()) {
        deviceTableView->selectionModel()->clearSelection();
        deviceTableView->selectionModel()->select(indexToSelect, QItemSelectionModel::Rows);
    }
}

void QBoardSettings::restoreDefaults() {
    setBoard(boardWidget->getBoard());
    setVid(QFtdiDevice::Default.Vid);
    setPid(QFtdiDevice::Default.Pid);
    setReferenceVoltage(QBoardWidget::Default.ReferenceVoltage());
    refreshBoards();
}

void QBoardSettings::accept() {
    QDialog::accept();
    boardWidget->setBoard(board);
    boardWidget->setReferenceVoltage(referenceVoltage);
    boardWidget->connect();
}

void QBoardSettings::syncVid() {
    vid = QByteArray(vidLineEdit->text().mid(2).toLatin1(), 4).toInt(NULL, 16);
}

void QBoardSettings::syncPid() {
    pid = QByteArray(pidLineEdit->text().mid(2).toLatin1(), 4).toInt(NULL, 16);
}

void QBoardSettings::syncBoard(QModelIndex currentSelection, QModelIndex /* pastSelection */) {
    setBoard(deviceTableModel->getDevice(currentSelection));
}

void QBoardSettings::syncReferenceVoltage() {
    referenceVoltage = referenceVoltageSpinBox->value();
}

void QBoardSettings::setVid(const int vid) {
    this->vid = vid;
    vidLineEdit->setText(QString("0x%1").arg(vid, 4, 16, QChar('0')));
}

void QBoardSettings::setPid(const int pid) {
    this->pid = pid;
    pidLineEdit->setText(QString("0x%1").arg(pid, 4, 16, QChar('0')));
}

void QBoardSettings::setBoard(QFtdiDevice* board) {
    this->board = board;
    if (this->board != NULL) {
        setVid(this->board->getVid());
        setPid(this->board->getPid());
    }
}

void QBoardSettings::setReferenceVoltage(const double referenceVoltage) {
    this->referenceVoltage = referenceVoltage;
    referenceVoltageSpinBox->setValue(referenceVoltage);
}

void QBoardSettings::changeEvent(QEvent *event) {
    QDialog::changeEvent(event);
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi(this);
    }
}
