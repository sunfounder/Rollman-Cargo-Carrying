#include <Servo.h>
Servo Servo_0;
//int M2 = 90;  //initialize the position
#define SIG A1
#define WAV A0
int distance, rxTime = 0;
int dis[4];
void avoidobstacle()
{
  digitalWrite(SIG, HIGH);
  delayMicroseconds(11);
  digitalWrite(SIG, LOW);                   //给超声波模块触发信号
  rxTime = pulseIn(WAV, HIGH);              //读取超声波模块检测时间
  distance = (float)rxTime * 34 / 500.0;   //计算超声波模块检测距离，将数据转化成cm
  distance = distance / 2;
  Serial.print("distance:");
  Serial.println(distance);
}
void setup()
{
  Serial.begin(9600);
  Servo_0.attach(2);
  Servo_0.write(0);//close the door
}
void loop()
{
  avoidobstacle();
  if (distance < 15)
  {
    delay(500);
    avoidobstacle();
    if (distance < 12)
    {
      delay(500);
      avoidobstacle();
      if (distance < 12)
      {
        Servo_0.attach(2);
        Servo_0.write(100);//close the door
        delay(2000);
      }
      else
      {
        Servo_0.attach(2);
        Servo_0.write(0);//close the door
      }

    }
    else
    { Servo_0.attach(2);
      Servo_0.write(0);//close the door
    }
  }
  else
  {
    Servo_0.attach(2);
    Servo_0.write(0);//close the door
  }
}
