/*
Jaztin Tabunda
Lab2
COMP 522 - V. Bhatt
4/13/2023
*/

//only writing some comments for this assignment (not my code), but code for lab2

#include "MeasureTask.h"

MeasureTask::MeasureTask() : dht_sensor( DHT_SENSOR_PIN ) {}

bool MeasureTask::run() { //this looks like it does some kind of measuring
  unsigned long timeStamp;
  int returnCode;
  msleep(4000); // Wait a bit
  if (isAwake) {
    Log.traceln("Measure %d", DHT_SENSOR_PIN);

    dht_sensor.setWaitForReading(true);
    timeStamp = millis();
    returnCode = dht_sensor.read();

    if (returnCode == DHTLIB_OK) {
      float t, h;
      t = dht_sensor.getTemperature()*9/5+32;
      h = dht_sensor.getHumidity();
      Log.traceln("Temperature %F deg. F, Humidity %F percent", t, h);
      Log.traceln("Latency %d ms", millis() - timeStamp);
      LCDQueue.push({t, h});
    } else {
      Log.traceln("DHT read error %d ", returnCode);
      dht_sensor.reset();
    }
  }

  return true;
}
