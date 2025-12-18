#include "seriallink.h"
#include <QSerialPortInfo>
#include <QDebug>
seriallink::seriallink(QObject *parent)
    : QObject{parent}
{
  //_serial.setPortName("COM7");
  QSerialPortInfo serialPortInfo;
  _serial.setPortName(serialPortInfo.availablePorts().at(0).portName());
  _serial.setBaudRate(QSerialPort::Baud9600);
  _serial.setDataBits(QSerialPort::Data8);
  _serial.setParity(QSerialPort::NoParity);
  _serial.setStopBits(QSerialPort::OneStop);
  _serial.setFlowControl(QSerialPort::NoFlowControl);
  connect(&_serial, &QSerialPort::readyRead, this, &seriallink::newData);
}

seriallink::~seriallink(){
    closeConnection();
}
void seriallink::openConnection(){
    if(!_serial.open(QIODevice::ReadWrite))
        qDebug() <<"connection impossible";
    else qDebug() <<"connection ok";

}

void seriallink::write(const char* messageTowrite){
    _serial.write(messageTowrite);

}
void seriallink::closeConnection(){
    _serial.close();

}
bool seriallink::isOpen(){
    return _serial.isOpen();
}

bool seriallink::isWritable(){
    return _serial.isWritable();

}
void seriallink::newData(){
    emit gotNewData(_serial.readAll()) ;

}
