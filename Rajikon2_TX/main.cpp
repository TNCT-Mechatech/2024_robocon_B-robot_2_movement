#include "Control.hpp"
#include "mbed.h"
#include <MbedHardwareSerial.hpp>
#include <SerialBridge.hpp>

AnalogIn x_axis(A2), y_axis(A3);

SerialDev *dev = new MbedHardwareSerial(new BufferedSerial(D10, D3, 115200));
SerialBridge serial(dev, 1024);

// Message
Control msg;

int main() {
  serial.add_frame(0, &msg);

  while (true) {

    msg.data.x = static_cast<int8_t>(-1*(x_axis.read() - 0.5) * 200);
    msg.data.y = static_cast<int8_t>((y_axis.read() - 0.5) * 200);

    serial.write(0);

    wait_us(30000);
    // wait_us(100000);
    
  }
}
