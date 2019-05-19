//Add the necessary libraries
#include "I2Cdev.h"     //For communicate with MPU6050
#include "MPU6050.h"    //The main library of the MPU6050
#include <Servo.h>

Servo servo1;  // create servo object to control a servo
int pos1=90;   // variable to store the servo position
Servo servo2;
int pos2=90;
Servo servo3;
int pos3=135;  //claw servo (Threshold: close=135; open=65)

int temp;
int mode = 0;
int off=0;
int button = 4;

//Define the object to access and cotrol the Gyro and Accelerometer (We don't use the Gyro data)
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

//Store direction (X axis and Y axis)
int data[2];


void setup(){
  pinMode(button, INPUT_PULLUP);
  servo1.attach(9);           // attaches the servo1 on pin 9 to the servo object
  servo2.attach(10);
  servo3.attach(11);
  servo3.write(135);
  mpu.initialize();           //Initialize the MPU object
  Serial.begin(9600);
}


void loop(void){

  if(digitalRead(button)==LOW){
    delay(300);
  
  if(digitalRead(button)==LOW){  //for press+hold : claw open/close
    if(mode==0){
      mode=1;
    } else{
      mode=0;
    }
    delay(400);
    if(mode==1){
    servo3.write(65);
    //delay(20);
  }else{
    servo3.write(135);
    //delay(20);
  }
  }
  else{      //for single press : sensor on/off
    if(off==0){
      off=1;
    } else{
      off=0;
    }
  }
  }
  
Serial.println(off);

  //With this function, the acceleration and gyro values of the axes are taken. 
  
  if(off==0){
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  //In two-way control, the X axis (data [0]) of the MPU6050 allows the robot arm to move up/down.
  //Y axis (data [1]) allows the robot to move wrist.
  data[0] = map(ax, -17000, 17000, 100, 400 ); //Send X axis data
  data[1] = map(ay, -17000, 17000, 100, 400);  //Send Y axis data
  Serial.print("X: ");
  Serial.print(data[0]);
  Serial.print("    Y: ");
  Serial.println(data[1]);
  
   if(data[1] > 320){
    temp=pos1-5;
    if(temp>=0 && temp<=180){
    pos1=temp;
    servo1.write(pos1);
    }
   }
   if(data[1] < 180){
    temp=pos1+5;
    if(temp>=0 && temp<=180){
    pos1=temp;
    servo1.write(pos1);
    }
   }

   if(data[0] > 290){
    temp=pos2+5;
    if(temp>=0 && temp<=180){
    pos2=temp;
    servo2.write(pos2);
    }
   }
   if(data[0] < 210){
    temp=pos2-5;
    if(temp>=0 && temp<=180){
    pos2=temp;
    servo2.write(pos2);
    }
   }
  }
  
  delay(50);
  
} //end loop


