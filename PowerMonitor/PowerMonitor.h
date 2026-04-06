#ifndef POWER_MONITOR_H
#define POWER_MONITOR_H

#include <Adafruit_INA219.h>

class PowerMonitor {
  public:
    PowerMonitor();
    bool begin();
    void update(); // Call this in your main loop
    
    float getCurrent_mA();
    double get_mAh();
    void reset_mAh();

  private:
    Adafruit_INA219 _ina219;
    double _total_mAh;
    float _previous_mA;
    unsigned long _last_micros;
    const unsigned long _interval = 50000; // 50ms sampling
};

#endif