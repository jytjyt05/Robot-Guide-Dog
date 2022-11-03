#include <ros.h>
#include <std_msgs/UInt16.h>
#include <geometry_msgs/Twist.h>

//C = Forward, D = Right, E = Back, A = Left
#define NOTE_C5 523
#define NOTE_D6 1175
#define NOTE_A6 1760
#define NOTE_G5 784
const int pinOut=3;

ros::NodeHandle node_handle;

//geometry_msgs::Twist move_msg;
std_msgs::UInt16 int_msg;

void subscribeCallback(const std_msgs::UInt16 &int_msg){
  //0 = object in front blocking, 
  if (int_msg.data == 0){
    tone(pinOut, NOTE_C5, 700/4);
    delay(700/4);
    tone(pinOut, NOTE_C5, 700/4);
    delay(700/4);
  }
  //1 = object on left blocking
  else if(int_msg.data == 1){
    tone(pinOut, NOTE_A6, 700/4);
    delay(700/4);
    tone(pinOut, NOTE_A6, 700/4);
    delay(700/4);
  }
  else if(int_msg.data == 2){
    tone(pinOut, NOTE_D6, 700/4);
    delay(700/4);
    tone(pinOut, NOTE_D6, 700/4);
    delay(700/4);
  }
//  else if(int_msg.data == 3){
//    tone(pinOut, NOTE_G5, 700/4);
//    delay(700/4);
//    noTone(pinOut);
//  }
  noTone(pinOut);
  
}

ros::Subscriber<std_msgs::UInt16> my_subscriber("code", &subscribeCallback);


void setup() {
  node_handle.initNode();
  node_handle.subscribe(my_subscriber);
}
void loop() {
// put your main code here, to run repeatedly:
  node_handle.spinOnce();
}
