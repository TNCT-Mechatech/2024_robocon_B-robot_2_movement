#include "mbed.h"
#include "math.h"

#include <MbedHardwareSerial.hpp>
#include <SerialBridge.hpp>
#include <cstdio>

#include "Control.hpp"

PwmOut pwm1(D5);
PwmOut pwm2(D6);
AnalogIn ud(A1);
AnalogIn lr(A2);
DigitalOut dir1(A3);
DigitalOut dir2(A4);

DigitalOut LED(PA_5);

#define PI 3.1415

SerialDev *dev = new MbedHardwareSerial(new BufferedSerial(D10, D3, 115200));
SerialBridge serial(dev, 1024);

// Message
Control msg;

class chem {
private:
  float x, y, r;
  float d, da;
  float a, b;

public:
  // コンストラクタ
  chem(){};

  //メンバ関数
  void angle(float _x, float _y) {
    x = _x;
    y = _y;

    if (atan2(y, x) < 0)
      d = PI + fabs((fabs(atan2(y, x)) - PI));
    else
      d = atan2(y, x);

    d = d * (180 / PI);
  }

  void lol() {
    if (90 > d && d >= 0.0) {

      dir1 = 0;
      dir2 = 1;
      da = d / 90;

      r = fabs(sqrt(x * x + y * y));
      if (r > 1)
        r = 0.98;
      pwm1.write(r * da);
      pwm2.write(r);
    }

    if (180 > d && d >= 90.0) {

      dir1 = 0;
      dir2 = 1;

      da = fabs(d - 180.0) / 90;

      r = fabs(sqrt(x * x + y * y));
      if (r > 1)
        r = 0.98;
      pwm1.write(r);
      pwm2.write(r * da);
    }

    if (270 > d && d >= 180.0) {

      dir1 = 1;
      dir2 = 0;

      da = (d - 180.0) / 90;

      r = fabs(sqrt(x * x + y * y));
      if (r > 1)
        r = 0.98;
      pwm1.write(r);
      pwm2.write(r * da);
    }

    if (360 > d && d >= 270.0) {

      dir1 = 1;
      dir2 = 0;

      da = fabs(d - 360.0) / 90;

      r = fabs(sqrt(x * x + y * y));
      if (r > 1)
        r = 0.98;
      pwm1.write(r * da);
      pwm2.write(r);
    }

    r = r * 0.5;
    // wait_us(30000);
    // printf("%f\n\r",r);
    // printf("%f\n\r",d);
    // printf("%f\n\r",da);
    // wait(0.5);
  }
};

chem chem1;



int main() {

  serial.add_frame(0, &msg);

  pwm1.period(1.0 / 20000);
  pwm2.period(1.0 / 20000);

  while (1) {

    //  update and read
    serial.update();

    

    if (msg.was_updated())
    {
        // printf("x:%2.3f | y:%2.3f\n\r", static_cast<float>(msg.data.x) / 100.0, static_cast<float>(msg.data.y) / 100.0);

        chem1.angle(static_cast<float>(msg.data.x) / 100.0, static_cast<float>(msg.data.y) / 100.0);
        chem1.lol();

        LED = !LED;

        wait_us(30000);
        // wait_us(100000);
    }

  }
}