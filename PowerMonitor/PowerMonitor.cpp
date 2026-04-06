#include "PowerMonitor.h"

PowerMonitor::PowerMonitor() : _total_mAh(0.0), _previous_mA(0.0), _last_micros(0) {}

bool PowerMonitor::begin() {
  if (!_ina219.begin()) return false;
  
  // Set to 32V/2A but the chip is smart enough for your 7.4V
  _ina219.setCalibration_32V_2A();
  _previous_mA = _ina219.getCurrent_mA();
  _last_micros = micros();
  return true;
}

void PowerMonitor::update() {
  unsigned long current_micros = micros();
  unsigned long elapsed = current_micros - _last_micros;

  if (elapsed >= _interval) {
    float current_mA = _ina219.getCurrent_mA();
    
    // Trapezoidal integration math
    double delta_hours = (double)elapsed / 3600000000.0;
    double avg_current = (current_mA + _previous_mA) / 2.0;
    _total_mAh += (avg_current * delta_hours);

    _previous_mA = current_mA;
    _last_micros = current_micros;
  }
}

float PowerMonitor::getCurrent_mA() { return _ina219.getCurrent_mA(); }
double get_mAh() { return _total_mAh; }
void reset_mAh() { _total_mAh = 0.0; }