/*
QFtdiDeviceTableModel - Model that provides available QFtdiDevices as
a data source for QTableView.

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

#include "ftditablemodel.h"

QFtdiDeviceTableModel::QFtdiDeviceTableModel(QTableView *parentView) :
    QAbstractTableModel(parentView)
{
    deviceList = new QList<QFtdiDevice*>();
    parentView->setModel(this);
}

int QFtdiDeviceTableModel::rowCount(const QModelIndex& /* parent */) const {
   return deviceList->isEmpty() ? 1 : deviceList->count();
}

int QFtdiDeviceTableModel::columnCount(const QModelIndex& /* parent */) const {
    return Column.Count;
}

QVariant QFtdiDeviceTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();
    if (orientation == Qt::Horizontal) {
        if (section == Column.Vendor) {
            return tr("Vendor");
        } else if (section == Column.Product) {
            return tr("Product");
        } else if (section == Column.SerialNumber) {
            return tr("Serial Number");
        }
    }
    return QVariant();
}

QVariant QFtdiDeviceTableModel::data(const QModelIndex &index, int role) const {
    if (role != Qt::DisplayRole) return QVariant();
    if (deviceList->isEmpty()) {
        if (index.row() == 0 && index.column() == 0) {
            return tr("No Devices Found.");
        }
    } else {
        if (index.column() == Column.Vendor) {
            return deviceList->at(index.row())->getVendor();
        } else if (index.column() == Column.Product) {
            return deviceList->at(index.row())->getProduct();
        } else if (index.column() == Column.SerialNumber) {
            return deviceList->at(index.row())->getSerial();
        }
    }
    return QVariant();
}

void QFtdiDeviceTableModel::refreshDeviceList(const int vid, const int pid) {
    delete deviceList;
    deviceList = QFtdiDevice::DeviceList(vid, pid, this);
    QModelIndex topLeft = index(0, 0, QModelIndex());
    QModelIndex bottomRight = index(rowCount()-1, columnCount()-1, QModelIndex());
    dataChanged(topLeft, bottomRight);
}

QFtdiDevice* QFtdiDeviceTableModel::getDevice(const QModelIndex &index) const {
    if (deviceList->isEmpty() || !index.isValid()) return NULL;
    return deviceList->at(index.row());
}

QModelIndex QFtdiDeviceTableModel::getIndex(QFtdiDevice* const device) const {
    if (device == NULL) return QModelIndex();
    for (int i = 0; i < deviceList->size(); i++) {
        if (*deviceList->at(i) == *device) {
             return index(i, 0, QModelIndex());
        }
    }
    return QModelIndex();
}
