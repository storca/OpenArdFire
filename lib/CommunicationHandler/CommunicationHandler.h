#ifndef COMMUNICATIONHANDLER_H
#define COMMUNICATIONHANDLER_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#define LOCAL 0
#define RF 1

class CommunicationHandler
{
public:
  CommunicationHandler(HardwareSerial *s, SoftwareSerial *rf);

  void handler();
  ~CommunicationHandler();
private:
  HardwareSerial *_localSerial;
  SoftwareSerial *_rfSerial;

  String _localSerialMessage = "";
  String _rfSerialMessage = "";
};

#endif
