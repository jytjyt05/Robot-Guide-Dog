#include <ros.h>
#include <std_msgs/UInt16.h>
//#include <geometry_msg/Twist.h>

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define SPEEDLIMIT 255
#define PWM 3
#define echo A4
#define trig A5
#define dtime 2000

#include <Servo.h>
Servo servo;

ros::NodeHandle node_handle;

//geometry_msgs::Twist move_msg;
std_msgs::UInt16 int_msg;

ros::Publisher my_publisher("code",&int_msg);
//ros::Subscriber<std_msgs::UInt16> my_subscriber("dist",&subscribeCallback);

void setup()
{
  //Initialize Serial Monitor
  // set all the motor control pins to outputs
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  servo.attach(PWM);
  node_handle.initNode();
  node_handle.advertise(my_publisher);
  servo.write(90);
}

void servoLeft(){
  servo.write(179);
  delay(5);
}
void servoRight(){
  servo.write(1);
  delay(5);
}
void servoreset(){
  servo.write(90);
  delay(5);
}

double getDistance(){
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);

  double distance = pulseIn(echo,HIGH);
  return (distance*0.0343)/2;
}

void leftsideM(int Speed){
  int sspeed=constrain(Speed,-SPEEDLIMIT,SPEEDLIMIT);
  if(sspeed>0){
    analogWrite(ENA,sspeed);
    analogWrite(ENB,sspeed);
  
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
  }
  if(sspeed==0){
    analogWrite(ENA,0);
    analogWrite(ENB,0);
  }
  if(sspeed<0){
    analogWrite(ENA,abs(sspeed));
    analogWrite(ENB,abs(sspeed));
  
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
  }
}
void rightsideM(int Speed){
  int sspeed=constrain(Speed,-SPEEDLIMIT,SPEEDLIMIT);
  if(sspeed>0){
    analogWrite(ENA,sspeed);
    analogWrite(ENB,sspeed);
  
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
  }
  if(sspeed==0){
    analogWrite(ENA,0);
    analogWrite(ENB,0);
  }
  if(sspeed<0){
    analogWrite(ENA,abs(sspeed));
    analogWrite(ENB,abs(sspeed));
  
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
  }
}

void chassisMove(int vf, int vr){
  leftsideM(vf+vr);
  rightsideM(vf-vr);
}

//right chassisMove(0,150);
//left chassisMove(0,-155);
//stop chassisMove(0,0);
void loop()
{ 
  chassisMove(150,0);
  int_msg.data=3;
  my_publisher.publish(&int_msg);
  
  if(getDistance()<15){
    chassisMove(0,0);
    //object in front, code 0
    int_msg.data=0;
    my_publisher.publish(&int_msg);
    delay(1000);
    
    servoLeft();
    delay(dtime);
    if(getDistance()<15){
      //object to the left, code 1
      int_msg.data=1;
      my_publisher.publish(&int_msg);
      
      servoRight();
      delay(dtime);
      
      if(getDistance()<15){
        //object to the right, code 2
        int_msg.data=2;
        my_publisher.publish(&int_msg);
        
        chassisMove(0,-155);
        chassisMove(0,-155);
        delay(dtime);
      }
      else{
        chassisMove(0,150);
        delay(dtime);
      }
    }
    else{
      chassisMove(0,-155);
      delay(dtime);
    }
  servoreset();
  }
 
  node_handle.spinOnce();
}
