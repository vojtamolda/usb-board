/*
QBoardWidget - Controller widget that manages USB Measuring Board
PCB based on FT232 and ATmega1280 ICs.

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

#include "boardwidget.h"

QBoardWidget::QBoardWidget(QWidget *parent) :
    QWidget(parent), board(NULL)
{
    setupUi(this);
    aoSetup();
    doSetup();
    pwmSetup();

    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()),
                     this, SLOT(transmit()));
}

void QBoardWidget::connect() {
    if (board != NULL) {
        QObject::connect(this->board, SIGNAL(deviceConnected(bool)),
                         this, SLOT(deviceConnected(bool)));
        QObject::connect(this->board, SIGNAL(deviceFailed(int)),
                         this, SLOT(deviceFailed(int)));
        memset(&(txdata.raw), 0, sizeof(txdata_t));
        memset(&(rxdata.raw), 0, sizeof(rxdata_t));
        board->connect(USART_BAUD, 1, sizeof(rxdata_t), sizeof(txdata_t));
    } else {
        emit boardConnected(false);
    }
    timer->start();
}

void QBoardWidget::transmit() {
    aoSync();
    doSync();
    pwmSync();
    if (board != NULL) {
        board->transmit((uchar*)&(rxdata.raw), sizeof(rxdata_t));
        board->receive((uchar*)&(txdata.raw), sizeof(txdata_t));
    }
    aiSync();
    diSync();
}

void QBoardWidget::disconnect() {
    timer->stop();
    if (board != NULL) {
        board->disconnect();
    } else {
        emit boardConnected(false);
    }
}

void QBoardWidget::deviceConnected(bool flag) {
    setEnabled(flag);
    emit boardConnected(flag);
}

void QBoardWidget::deviceFailed(int error) {
    QMessageBox messageBox;
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.setInformativeText(tr("Please verify that the device is properly connected."));
    if (board != NULL) messageBox.setDetailedText(board->getErrorMessage());
    if (error == QFtdiDevice::Error.InitFailed) {
        messageBox.setText(tr("Initialization failed."));
    } else if (error == QFtdiDevice::Error.ConnectFailed) {
        messageBox.setText(tr("Connection failed."));
    } else if (error == QFtdiDevice::Error.TransmitFailed) {
        messageBox.setText(tr("Data transmission failed."));
    } else if (error == QFtdiDevice::Error.ReceiveFailed) {
        messageBox.setText(tr("Data receiving failed."));
    } else if (error == QFtdiDevice::Error.DisconnectFailed) {
        messageBox.setText(tr("Disconnect failed."));
    } else {
        return;
    }
    messageBox.exec();
    emit boardFailed(error);
    disconnect();
}

void QBoardWidget::storeSettings(QSettings &settings) const {
    settings.setValue(SettingKey.ReferenceVoltage(), getReferenceVoltage());
    if ((board != NULL) && (board->isConnected())) {
        settings.setValue(SettingKey.Board(), true);
        settings.beginGroup(SettingKey.Board());
        board->storeSettings(settings);
        settings.endGroup();
    } else {
        settings.remove(SettingKey.Board());
    }
}

void QBoardWidget::restoreSettings(QSettings &settings) {
    double referenceVoltage = settings.value(SettingKey.ReferenceVoltage(), Default.ReferenceVoltage()).toDouble();
    setReferenceVoltage(referenceVoltage);
    if (settings.childGroups().contains(SettingKey.Board())) {
        settings.beginGroup(SettingKey.Board());
        setBoard(new QFtdiDevice(settings, this));
        settings.endGroup();
        connect();
    } else {
        setBoard(NULL);
    }
}

void QBoardWidget::setBoard(QFtdiDevice *board) {
    if (this->board != NULL) {
        disconnect();
        delete this->board;
    }
    this->board = board;
    if (this->board != NULL) {
        this->board->setParent(this);
    }
}

QFtdiDevice* QBoardWidget::getBoard() const {
    return board;
}

void QBoardWidget::setReferenceVoltage(const double referenceVoltage) {
    this->referenceVoltage = referenceVoltage;
    aoSpinBox_0->setMaximum(referenceVoltage);
    aoSpinBox_1->setMaximum(referenceVoltage);
}

double QBoardWidget::getReferenceVoltage() const {
    return referenceVoltage;
}

void QBoardWidget::changeEvent(QEvent *event) {
    QWidget::changeEvent(event);
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi(this);
    }
}

void QBoardWidget::diSync(void) {
    diLineEdit_0->setText(QString("%L1").arg(txdata.data.din[0]));
    diLineEdit_1->setText(QString("%L1").arg(txdata.data.din[1]));
    diRadioButton_0->setChecked((bool)(txdata.data.din[0] & (1 << 0)));
    diRadioButton_1->setChecked((bool)(txdata.data.din[0] & (1 << 1)));
    diRadioButton_2->setChecked((bool)(txdata.data.din[0] & (1 << 2)));
    diRadioButton_3->setChecked((bool)(txdata.data.din[0] & (1 << 3)));
    diRadioButton_4->setChecked((bool)(txdata.data.din[0] & (1 << 4)));
    diRadioButton_5->setChecked((bool)(txdata.data.din[0] & (1 << 5)));
    diRadioButton_6->setChecked((bool)(txdata.data.din[0] & (1 << 6)));
    diRadioButton_7->setChecked((bool)(txdata.data.din[0] & (1 << 7)));
    diRadioButton_8->setChecked((bool)(txdata.data.din[1] & (1 << 0)));
    diRadioButton_9->setChecked((bool)(txdata.data.din[1] & (1 << 1)));
    diRadioButton_10->setChecked((bool)(txdata.data.din[1] & (1 << 2)));
    diRadioButton_11->setChecked((bool)(txdata.data.din[1] & (1 << 3)));
    diRadioButton_12->setChecked((bool)(txdata.data.din[1] & (1 << 4)));
    diRadioButton_13->setChecked((bool)(txdata.data.din[1] & (1 << 5)));
    diRadioButton_14->setChecked((bool)(txdata.data.din[1] & (1 << 6)));
    diRadioButton_15->setChecked((bool)(txdata.data.din[1] & (1 << 7)));
}

void QBoardWidget::aiSync(void) {
    aiProgressBar_0->setValue(txdata.data.adc[0]);
    aiProgressBar_1->setValue(txdata.data.adc[1]);
    aiProgressBar_2->setValue(txdata.data.adc[2]);
    aiProgressBar_3->setValue(txdata.data.adc[3]);
    aiProgressBar_4->setValue(txdata.data.adc[4]);
    aiProgressBar_5->setValue(txdata.data.adc[5]);
    aiProgressBar_6->setValue(txdata.data.adc[6]);
    aiProgressBar_7->setValue(txdata.data.adc[7]);
    aiProgressBar_8->setValue(txdata.data.adc[8]);
    aiProgressBar_9->setValue(txdata.data.adc[9]);
    aiProgressBar_10->setValue(txdata.data.adc[10]);
    aiProgressBar_11->setValue(txdata.data.adc[11]);
    aiProgressBar_12->setValue(txdata.data.adc[12]);
    aiProgressBar_13->setValue(txdata.data.adc[13]);
    aiProgressBar_14->setValue(txdata.data.adc[14]);
    aiProgressBar_15->setValue(txdata.data.adc[15]);
    aiProgressBar_0->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[0])
                               /Range.AI*referenceVoltage, 0, 'f', 3));
    aiProgressBar_1->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[1])
                               /Range.AI*referenceVoltage, 0, 'f', 3));
    aiProgressBar_2->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[2])
                               /Range.AI*referenceVoltage, 0, 'f', 3));
    aiProgressBar_3->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[3])
                               /Range.AI*referenceVoltage, 0, 'f', 3));
    aiProgressBar_4->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[4])
                               /Range.AI*referenceVoltage, 0, 'f', 3));
    aiProgressBar_5->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[5])
                               /Range.AI*referenceVoltage, 0, 'f', 3));
    aiProgressBar_6->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[6])
                               /Range.AI*referenceVoltage, 0, 'f', 3));
    aiProgressBar_7->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[7])
                               /Range.AI*referenceVoltage, 0, 'f', 3));
    aiProgressBar_8->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[8])
                               /Range.AI*referenceVoltage, 0, 'f', 3));
    aiProgressBar_9->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[9])
                               /Range.AI*referenceVoltage, 0, 'f', 3));
    aiProgressBar_10->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[10])
                                /Range.AI*referenceVoltage, 0, 'f', 3));
    aiProgressBar_11->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[11])
                                /Range.AI*referenceVoltage, 0, 'f', 3));
    aiProgressBar_12->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[12])
                                /Range.AI*referenceVoltage, 0, 'f', 3));
    aiProgressBar_13->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[13])
                                /Range.AI*referenceVoltage, 0, 'f', 3));
    aiProgressBar_14->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[14])
                                /Range.AI*referenceVoltage, 0, 'f', 3));
    aiProgressBar_15->setFormat(QString("%L1 V").arg(((double)txdata.data.adc[15])
                                /Range.AI*referenceVoltage, 0, 'f', 3));
}

void QBoardWidget::doSetup() {
    doSignalMapper = new QSignalMapper(this);
    doSignalMapper->setMapping(doCheckBox_0, SignalMap.CheckBox);
    doSignalMapper->setMapping(doCheckBox_1, SignalMap.CheckBox);
    doSignalMapper->setMapping(doCheckBox_2, SignalMap.CheckBox);
    doSignalMapper->setMapping(doCheckBox_3, SignalMap.CheckBox);
    doSignalMapper->setMapping(doCheckBox_4, SignalMap.CheckBox);
    doSignalMapper->setMapping(doCheckBox_5, SignalMap.CheckBox);
    doSignalMapper->setMapping(doCheckBox_6, SignalMap.CheckBox);
    doSignalMapper->setMapping(doCheckBox_7, SignalMap.CheckBox);
    doSignalMapper->setMapping(doCheckBox_8, SignalMap.CheckBox);
    doSignalMapper->setMapping(doCheckBox_9, SignalMap.CheckBox);
    doSignalMapper->setMapping(doCheckBox_10, SignalMap.CheckBox);
    doSignalMapper->setMapping(doCheckBox_11, SignalMap.CheckBox);
    doSignalMapper->setMapping(doCheckBox_12, SignalMap.CheckBox);
    doSignalMapper->setMapping(doCheckBox_13, SignalMap.CheckBox);
    doSignalMapper->setMapping(doCheckBox_14, SignalMap.CheckBox);
    doSignalMapper->setMapping(doCheckBox_15, SignalMap.CheckBox);
    doSignalMapper->setMapping(doSpinBox_0, SignalMap.SpinBox);
    doSignalMapper->setMapping(doSpinBox_1, SignalMap.SpinBox);
    QObject::connect(doCheckBox_0, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doCheckBox_1, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doCheckBox_2, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doCheckBox_3, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doCheckBox_4, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doCheckBox_5, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doCheckBox_6, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doCheckBox_7, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doCheckBox_8, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doCheckBox_9, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doCheckBox_10, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doCheckBox_11, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doCheckBox_12, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doCheckBox_13, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doCheckBox_14, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doCheckBox_15, SIGNAL(clicked(bool)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doSpinBox_0, SIGNAL(valueChanged(int)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doSpinBox_1, SIGNAL(valueChanged(int)),
                     doSignalMapper, SLOT(map()));
    QObject::connect(doSignalMapper, SIGNAL(mapped(const int)),
                     this, SLOT(doSync(const int)));
}

void QBoardWidget::doSync(int sender) {
    if (sender == SignalMap.CheckBox) {
        rxdata.data.dout[0] = ((uchar)(doCheckBox_0->isChecked() << 0)) |
                              ((uchar)(doCheckBox_1->isChecked() << 1)) |
                              ((uchar)(doCheckBox_2->isChecked() << 2)) |
                              ((uchar)(doCheckBox_3->isChecked() << 3)) |
                              ((uchar)(doCheckBox_4->isChecked() << 4)) |
                              ((uchar)(doCheckBox_5->isChecked() << 5)) |
                              ((uchar)(doCheckBox_6->isChecked() << 6)) |
                              ((uchar)(doCheckBox_7->isChecked() << 7));
        rxdata.data.dout[1] = ((uchar)(doCheckBox_8->isChecked() << 0)) |
                              ((uchar)(doCheckBox_9->isChecked() << 1)) |
                              ((uchar)(doCheckBox_10->isChecked() << 2)) |
                              ((uchar)(doCheckBox_11->isChecked() << 3)) |
                              ((uchar)(doCheckBox_12->isChecked() << 4)) |
                              ((uchar)(doCheckBox_13->isChecked() << 5)) |
                              ((uchar)(doCheckBox_14->isChecked() << 6)) |
                              ((uchar)(doCheckBox_15->isChecked() << 7));
        doSpinBox_0->setValue(rxdata.data.dout[0]);
        doSpinBox_1->setValue(rxdata.data.dout[1]);
    }
    if (sender == SignalMap.SpinBox) {
        rxdata.data.dout[0] = doSpinBox_0->value();
        rxdata.data.dout[1] = doSpinBox_1->value();
        doCheckBox_0->setChecked((bool)(rxdata.data.dout[0] & (1 << 0)));
        doCheckBox_1->setChecked((bool)(rxdata.data.dout[0] & (1 << 1)));
        doCheckBox_2->setChecked((bool)(rxdata.data.dout[0] & (1 << 2)));
        doCheckBox_3->setChecked((bool)(rxdata.data.dout[0] & (1 << 3)));
        doCheckBox_4->setChecked((bool)(rxdata.data.dout[0] & (1 << 4)));
        doCheckBox_5->setChecked((bool)(rxdata.data.dout[0] & (1 << 5)));
        doCheckBox_6->setChecked((bool)(rxdata.data.dout[0] & (1 << 6)));
        doCheckBox_7->setChecked((bool)(rxdata.data.dout[0] & (1 << 7)));
        doCheckBox_8->setChecked((bool)(rxdata.data.dout[1] & (1 << 0)));
        doCheckBox_9->setChecked((bool)(rxdata.data.dout[1] & (1 << 1)));
        doCheckBox_10->setChecked((bool)(rxdata.data.dout[1] & (1 << 2)));
        doCheckBox_11->setChecked((bool)(rxdata.data.dout[1] & (1 << 3)));
        doCheckBox_12->setChecked((bool)(rxdata.data.dout[1] & (1 << 4)));
        doCheckBox_13->setChecked((bool)(rxdata.data.dout[1] & (1 << 5)));
        doCheckBox_14->setChecked((bool)(rxdata.data.dout[1] & (1 << 6)));
        doCheckBox_15->setChecked((bool)(rxdata.data.dout[1] & (1 << 7)));
   }
}

void QBoardWidget::aoSetup() {
    aoSignalMapper = new QSignalMapper(this);
    aoSignalMapper->setMapping(aoSlider_0, SignalMap.Slider);
    aoSignalMapper->setMapping(aoSlider_1, SignalMap.Slider);
    aoSignalMapper->setMapping(aoSpinBox_0, SignalMap.SpinBox);
    aoSignalMapper->setMapping(aoSpinBox_1, SignalMap.SpinBox);
    QObject::connect(aoSlider_0, SIGNAL(valueChanged(int)),
                     aoSignalMapper, SLOT(map()));
    QObject::connect(aoSlider_1, SIGNAL(valueChanged(int)),
                     aoSignalMapper, SLOT(map()));
    QObject::connect(aoSpinBox_0, SIGNAL(valueChanged(double)),
                     aoSignalMapper, SLOT(map()));
    QObject::connect(aoSpinBox_1, SIGNAL(valueChanged(double)),
                     aoSignalMapper, SLOT(map()));
    QObject::connect(aoSignalMapper, SIGNAL(mapped(const int)),
                     this, SLOT(aoSync(const int)));
}

void QBoardWidget::aoSync(int sender) {
    if (sender == SignalMap.Slider) {
        rxdata.data.dac[0] = (uint16_t)aoSlider_0->value();
        rxdata.data.dac[1] = (uint16_t)aoSlider_1->value();
        double value0 = (double)rxdata.data.dac[0]/(double)Range.AO*referenceVoltage;
        double value1 = (double)rxdata.data.dac[1]/(double)Range.AO*referenceVoltage;
        aoSpinBox_0->setValue(value0);
        aoSpinBox_1->setValue(value1);
    }
    if (sender == SignalMap.SpinBox) {
        rxdata.data.dac[0] = (uint16_t)((aoSpinBox_0->value()/referenceVoltage)*(double)Range.AO);
        rxdata.data.dac[1] = (uint16_t)((aoSpinBox_1->value()/referenceVoltage)*(double)Range.AO);
        aoSlider_0->setValue(rxdata.data.dac[0]);
        aoSlider_1->setValue(rxdata.data.dac[1]);
    }
}

void QBoardWidget::pwmSetup() {
    pwmSignalMapper = new QSignalMapper(this);
    pwmSignalMapper->setMapping(pwmSlider_0, SignalMap.Slider);
    pwmSignalMapper->setMapping(pwmSlider_1, SignalMap.Slider);
    pwmSignalMapper->setMapping(pwmSlider_2, SignalMap.Slider);
    pwmSignalMapper->setMapping(pwmSlider_3, SignalMap.Slider);
    pwmSignalMapper->setMapping(pwmSlider_4, SignalMap.Slider);
    pwmSignalMapper->setMapping(pwmSlider_5, SignalMap.Slider);
    pwmSignalMapper->setMapping(pwmSlider_6, SignalMap.Slider);
    pwmSignalMapper->setMapping(pwmSlider_7, SignalMap.Slider);
    pwmSignalMapper->setMapping(pwmSpinBox_0, SignalMap.SpinBox);
    pwmSignalMapper->setMapping(pwmSpinBox_1, SignalMap.SpinBox);
    pwmSignalMapper->setMapping(pwmSpinBox_2, SignalMap.SpinBox);
    pwmSignalMapper->setMapping(pwmSpinBox_3, SignalMap.SpinBox);
    pwmSignalMapper->setMapping(pwmSpinBox_4, SignalMap.SpinBox);
    pwmSignalMapper->setMapping(pwmSpinBox_5, SignalMap.SpinBox);
    pwmSignalMapper->setMapping(pwmSpinBox_6, SignalMap.SpinBox);
    pwmSignalMapper->setMapping(pwmSpinBox_7, SignalMap.SpinBox);
    QObject::connect(pwmSlider_0, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSlider_1, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSlider_2, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSlider_3, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSlider_4, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSlider_5, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSlider_6, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSlider_7, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSpinBox_0, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSpinBox_1, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSpinBox_2, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSpinBox_3, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSpinBox_4, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSpinBox_5, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSpinBox_6, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSpinBox_7, SIGNAL(valueChanged(int)),
                     pwmSignalMapper, SLOT(map()));
    QObject::connect(pwmSignalMapper, SIGNAL(mapped(const int)),
                     this, SLOT(pwmSync(const int)));
}

void QBoardWidget::pwmSync(int sender) {
    if (sender == SignalMap.Slider) {
        rxdata.data.pwm[0] = pwmSlider_0->value();
        rxdata.data.pwm[1] = pwmSlider_1->value();
        rxdata.data.pwm[2] = pwmSlider_2->value();
        rxdata.data.pwm[3] = pwmSlider_3->value();
        rxdata.data.pwm[4] = pwmSlider_4->value();
        rxdata.data.pwm[5] = pwmSlider_5->value();
        rxdata.data.pwm[6] = pwmSlider_6->value();
        rxdata.data.pwm[7] = pwmSlider_7->value();
        pwmSpinBox_0->setValue(rxdata.data.pwm[0]);
        pwmSpinBox_1->setValue(rxdata.data.pwm[1]);
        pwmSpinBox_2->setValue(rxdata.data.pwm[2]);
        pwmSpinBox_3->setValue(rxdata.data.pwm[3]);
        pwmSpinBox_4->setValue(rxdata.data.pwm[4]);
        pwmSpinBox_5->setValue(rxdata.data.pwm[5]);
        pwmSpinBox_6->setValue(rxdata.data.pwm[6]);
        pwmSpinBox_7->setValue(rxdata.data.pwm[7]);
    }
    if (sender == SignalMap.SpinBox) {
        rxdata.data.pwm[0] = pwmSpinBox_0->value();
        rxdata.data.pwm[1] = pwmSpinBox_1->value();
        rxdata.data.pwm[2] = pwmSpinBox_2->value();
        rxdata.data.pwm[3] = pwmSpinBox_3->value();
        rxdata.data.pwm[4] = pwmSpinBox_4->value();
        rxdata.data.pwm[5] = pwmSpinBox_5->value();
        rxdata.data.pwm[6] = pwmSpinBox_6->value();
        rxdata.data.pwm[7] = pwmSpinBox_7->value();
        pwmSlider_0->setValue(rxdata.data.pwm[0]);
        pwmSlider_1->setValue(rxdata.data.pwm[1]);
        pwmSlider_2->setValue(rxdata.data.pwm[2]);
        pwmSlider_3->setValue(rxdata.data.pwm[3]);
        pwmSlider_4->setValue(rxdata.data.pwm[4]);
        pwmSlider_5->setValue(rxdata.data.pwm[5]);
        pwmSlider_6->setValue(rxdata.data.pwm[6]);
        pwmSlider_7->setValue(rxdata.data.pwm[7]);
    }
}
