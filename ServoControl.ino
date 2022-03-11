//Arduino embedded software for the LP_Servo_Proto to receive data from a c++ Thread in UART or from the serial port directly
//Added commands to move the servomotor

#define BAUDRATE 256000 //High baudrate for fast transmission

//#define TIME

//Variables communication
String save = "";
int message;
#ifdef TIME
unsigned long start;
#endif


//Biblios for servomotor
#include <Servo.h>

Servo servo;
//min max parameters to control the servo in PWM range pulse = [900 2100]
//min max for servo with long rod
int min = 900; //0°
int max = 1050;




int maxMotor = 2100; //120°

void setup() {
  Serial.begin(BAUDRATE); // opens serial port, sets data rate to baudrate
  //Parameters for servo wing FT3325M
  min = min + 610; //0°
  max = min + 760;

  servo.attach(9, min, max);
  servo.writeMicroseconds(min + 25);
  delay(3000);
}

void loop() {
  if (Serial.available() > 0) {
    message = Serial.read();
    //if the message is an integer aka ASCII code between 48 & 57
    if (message > 47 && message < 58) {
      //we save the data in a string
      //      Serial.print("Message : ");
      //      Serial.println(message);
      //      Serial.flush();
#ifdef TIME
      start = micros();
#endif
      save = save + char(message);
    }
    //if we started to memorize the data sent AND we read the character "e" we have read the number completely
    else if ((save != "") && (message == 101)) {
      //      Serial.print("complete message : ");
      //      Serial.println(save);
      //      Serial.println(save.toInt());
      servo.writeMicroseconds(save.toInt() + min);
      save = "";

      //      Serial.flush();
#ifdef TIME
      //      Serial.print("Duration :  ");
      Serial.println(micros() - start);
#endif
    }
  }
}

void writeMicroSmooth(int a, int b, int smoothness) {
  if (a < b) {
    for (int i = a; i <= b; i++) {
      servo.writeMicroseconds(i);
      delay(smoothness);
    }
  }
  if (a > b) {
    for (int i = a; i >= b; i = i - 1) {
      servo.writeMicroseconds(i);
      delay(smoothness);
    }
  }
}

void roundTrip(int a, int b, int smoothness) {
  writeMicroSmooth(a, b, smoothness);
  writeMicroSmooth(a, b, smoothness);
}
