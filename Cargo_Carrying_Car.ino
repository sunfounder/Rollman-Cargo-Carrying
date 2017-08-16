/* -----------------------------------------------------------------------------
  Author             : Jackie
  Check              : Amy
  Version            : V1.0
  Date               : 24/04/2017
  Description        : LineFollowing
  Company website    : http://www.sunfounder.com
   ---------------------------------------------------------------------------*/
#include <Wire.h>                   //IIC header file
#include <Rollman.h>                //Motor driving header file
#include <Servo.h>
/************************** Instantiate ****************************/
RollmanMotors motors;

Servo Servo_0;
/************************* Define parameters ***********************/
//#define DataPrint
//#define FlagPrint

#define RIGHT_MAX_SPEED 190       // Max speed of the robot
#define LEFT_MAX_SPEED 190        // Max speed of the robot
#define RIGHT_BASE_SPEED 100      // This is the speed at which the motors should spin when the robot is perfectly on the line
#define LEFT_BASE_SPEED 100       // This is the speed at which the motors should spin when the robot is perfectly on the line

int speed1, speed2 = 0;
float Kp = 14;                     // 5  12     Define Kp variable
int Kd = 190;                     // 100  180  90  120  Define Kd variable
int lastError;
unsigned char data[16];
unsigned char Flag[8];
int i, t = 0;
int threshold;
int frequency = 0;
void Threshold_Check(int ms);
void Read_IICData();
void PID();
void turn_left();
void turn_right();
/********************* Initialized definition ********************/
void setup()
{
  Wire.begin();                   //Initialize IIC
  Serial.begin(9600);            //Initialize the Serial port
  //set the pin connected to the servo
  Servo_0.attach(A0);
  //set the servo to the initial position
  Servo_0.write(10);
  delay(2000);
  Threshold_Check(150);         //Initialize the threshold check
}


void Read_IICData()
{
  Wire.requestFrom(9, 16);    // request 16 bytes from slave device #9
  while (Wire.available())    // slave may send less than requested
  {
    data[t] = Wire.read();    // receive a byte as character
    if (t < 15) t++;
    else t = 0;
  }
  for (i = 0; i < 8; i++)
  {
    if (data[2 * i] < threshold)
      Flag[i] = 1;
    else
      Flag[i] = 0;
  }
}

void loop()
{
#ifdef DataPrint
  while (1)
  {
    Read_IICData();
    Serial.print("data[0]:");
    Serial.println(data[0]);
    Serial.print("data[2]:");
    Serial.println(data[2]);
    Serial.print("data[4]:");
    Serial.println( data[4]);
    Serial.print("data[6]:");
    Serial.println(data[6]);
    Serial.print("data[8]:");
    Serial.println(data[8]);
    Serial.print("data[10]:");
    Serial.println(data[10]);
    Serial.print("data[12]:");
    Serial.println( data[12]);
    Serial.print("data[14]:");
    Serial.println(data[14]);
    delay(200);
  }
#endif
#ifdef FlagPrint
  while (1)
  {
    Read_IICData();;
    Serial.print("Flag[0]:");
    Serial.println(Flag[0]);
    Serial.print("Flag[1]:");
    Serial.println(Flag[1]);
    Serial.print("Flag[2]:");
    Serial.println( Flag[2]);
    Serial.print("Flag[3]:");
    Serial.println(Flag[3]);
    Serial.print("Flag[4]:");
    Serial.println(Flag[4]);
    Serial.print("Flag[5]:");
    Serial.println(Flag[5]);
    Serial.print("Flag[6]:");
    Serial.println( Flag[6]);
    Serial.print("Flag[7]:");
    Serial.println(Flag[7]);
    delay(200);
  }
#endif
 /**************************************************************************/     
  Read_IICData();
    if  (Flag[1] == 1 || Flag[2] == 1 || Flag[3] == 1 || Flag[4] == 1 || Flag[5] == 1 || Flag[6] == 1)
    {
          if ( Flag[1] == 0 && Flag[2] == 0 && Flag[5] == 0 && Flag[6] == 0 && (Flag[3] == 1 || Flag[4] == 1))
          {
            motors.drive(100, 100);
           }
          
          else if ((Flag[1] == 1 && Flag[2] == 1 )&&( Flag[3] == 1 && Flag[4] == 1 )&&( Flag[5] == 1 && Flag[6] == 1))
          {
              delay(40);
              for (int i = 100; i > 0; i--)
              {
                motors.drive(i, i); delay(4);
              }
               Read_IICData();
              if ((Flag[1] == 0 && Flag[2] == 0 )&&( Flag[3] == 0 &&Flag[4] == 0 )&&( Flag[5] == 0 && Flag[6] == 0))
                {
                     turn_right();
                }
                else 
                {
                     motors.drive(100, 100);
                }
           }
           
             else
          {
                PID();
          } 
    }
    
    /**************************************************************************/       
   else   if ((Flag[0] == 0) && (Flag[1] == 0 )&& (Flag[2] == 0) && (Flag[3] == 0) && (Flag[4] == 0) && (Flag[5] == 0) && (Flag[6] == 0) && (Flag[7] == 0)) //多次判断是否是是全白
    {
      for (int i = 80; i > 0; i--)
      {
        motors.drive(i, i); delay(2);
      }
      Read_IICData();
      if  ((Flag[0] == 0) && (Flag[1] == 0 )&& (Flag[2] == 0) && (Flag[3] == 0) && (Flag[4] == 0) && (Flag[5] == 0) && (Flag[6] == 0) && (Flag[7] == 0)) 
      {
                      for (int i = 80; i > 0; i--)
                      {
                        motors.drive(i, i); delay(2);
                      }
                      Read_IICData();
           if  ((Flag[0] == 0) && (Flag[1] == 0 )&& (Flag[2] == 0) && (Flag[3] == 0) && (Flag[4] == 0) && (Flag[5] == 0) && (Flag[6] == 0) && (Flag[7] == 0)) 
                {
                  turn_back();
                }
           else   if ((Flag[1] == 1 || Flag[2] == 1 )&&( Flag[3] == 1 || Flag[4] == 1 )&&( Flag[5] == 1 || Flag[6] == 1))
                {
                     motors.drive(0, 0);
                     frequency++;
                     if(frequency == 2)
                     {
                        Servo_0.attach(A0);
                        Servo_0.write(100);
                        delay(1000);
                        Servo_0.attach(A0);
                        Servo_0.write(0);
                        frequency = 0;
                     }                    
                     delay(3000);
                     motors.drive(-100, -100);
                     delay(200);
                     turn_back();     
                 }
//           else 
//                {
//                  motors.drive(100, 100); 
//                  Read_IICData();
//                 }
      }
      else   if ((Flag[1] == 1 || Flag[2] == 1 )&&( Flag[3] == 1 || Flag[4] == 1 )&&( Flag[5] == 1 || Flag[6] == 1))
       {
          motors.drive(0, 0);
             frequency++;
         if(frequency == 2)
         {
            Servo_0.attach(A0);
            Servo_0.write(100);
            delay(1000);
            Servo_0.attach(A0);
            Servo_0.write(0);
            frequency = 0;
          }
         delay(3000);
         motors.drive(-100, -100);
         delay(200);
         turn_back();     
       }
//      else 
//      {
//             motors.drive(100, 100); 
//             Read_IICData();
//       }
    }
 /**************************************************************************/        
    else if (Flag[1] == 1 && Flag[2] == 1 && Flag[3] == 1 && Flag[4] == 1 && Flag[5] == 1 && Flag[6] == 1)
    {
         motors.drive(100, 100); 
     }
   /**************************************************************************/        
}

void Threshold_Check(int ms)
{
  int i;
  int Min0, Min1, Min2, Max0, Max1, Max2;
  for (int i = 0; i > 0; i--)
  {
    motors.drive(-i, i); delay(5);
  }
  Read_IICData();
  Min0 = data[0];
  Max0 = data[0];
  for (i = 0; i < 16; i = i + 2)
  {
    if (data[i] < Min0)
      Min0 = data[i];
    if (data[i] > Max0)
      Max0 = data[i];
  }
  delay(ms);
  for (int i = 0; i > 0; i--)
  {
    motors.drive(i, -i); delay(5);
  }
  Read_IICData();
  Min1 = data[0];
  Max1 = data[0];
  for (i = 0; i < 16; i = i + 2)
  {
    if (data[i] < Min1)
      Min1 = data[i];
    if (data[i] > Max1)
      Max1 = data[i];
  }
  delay(ms);
  for (int i = 0; i > 0; i--)
  {
    motors.drive(i, -i); delay(5);
  }
  Read_IICData();
  Min2 = data[0];
  Max2 = data[0];
  for (i = 0; i < 16; i = i + 2)
  {
    if (data[i] < Min2)
      Min2 = data[i];
    if (data[i] > Max2)
      Max2 = data[i];
  }
  delay(ms);
  for (int i = 0; i > 0; i--)
  {
    motors.drive(-i, i); delay(5);
  }
  threshold = (int)(((Min0 + Min1 + Min2) / 3 + (Max0 + Max1 + Max2) / 3) / 2);
}
void turn_right()
{
  motors.drive(80, -80);
  Read_IICData();
  while (!(Flag[7] == 1)) // the right sensor;
  {
    Read_IICData();
  }
  motors.drive(60, -60);
  Read_IICData();
  while (!(Flag[3] == 1)) // turn right - wait for line position to find near center
  {
    Read_IICData();
  }
  for (int i = 60; i > 0; i--)
  {
    motors.drive(i, -i); delay(2);
  } 
//  motors.drive(0, 0);
   Read_IICData();
}
void turn_back()
{
  motors.drive(80, -80);
  Read_IICData();
  while (!(Flag[7] == 1)) // tune - wait for line position to find near cente
  {
    Read_IICData();
  }
  motors.drive(60, -60);
  Read_IICData();
  while (!(Flag[4] == 1)) // tune - wait for line position to find near center
  {
    Read_IICData();
  }
  for (int i = 60; i > 0; i--)
  {
    motors.drive(i, -i); delay(2);
  }
  // motors.drive(0, 0);
   Read_IICData();
}
void PID()
{
       lastError = 0;
      float error = 0.3 * Flag[1] + 0.2 * Flag[2] + 0.1 * Flag[3] - 0.1 * Flag[4] - 0.2 * Flag[5] - 0.3 * Flag[6];
      float motorSpeed = Kp * error + Kd * (error - lastError);      //Rollman's PID control
      lastError = error;
      speed1 = LEFT_BASE_SPEED - motorSpeed;
      speed2 = RIGHT_BASE_SPEED + motorSpeed;
      if (speed2 > RIGHT_MAX_SPEED ) speed2 = RIGHT_MAX_SPEED;  // prevent the motor from going beyond max speed
      if (speed1 > LEFT_MAX_SPEED ) speed1 = LEFT_MAX_SPEED;    // prevent the motor from going beyond max speed
      if (speed2 < 0) speed2 = 0;                               // keep the motor speed positive
      if (speed1 < 0) speed1 = 0;                               // keep the motor speed positive
      motors.drive(speed1, speed2);
}
void turn_left()
{
  motors.drive(-60, 60);
  Read_IICData();
  while (!(Flag[0] == 1)) // tune - wait for line position to find near cente
  {
    Read_IICData();
  }
  motors.drive(-60, 60);
  Read_IICData();
  while (!(Flag[4] == 1)) // tune - wait for line position to find near center
  {
    Read_IICData();
  }

  for (int i = 60; i > 0; i--)
  {
    motors.drive(-i, i); delay(2);
  }
   Read_IICData();
}


