#include <Wire.h>

//unsigned long time1;
unsigned long time2;
unsigned long timenow;
unsigned long i = 0;

struct Watch {
  int Sec;
  int Minute;
  int Hour;
};
struct Watch CurrentTime;


void setup() {
  Wire.begin(9);
  Wire.onRequest(requestEvent);
  
  Serial.begin(9600);
  //time1 = 0;

  CurrentTime.Sec = 00;
  CurrentTime.Minute = 58;
  CurrentTime.Hour = 13;
}

void loop() {
  time2 = millis();
  timenow = time2 - i;
  if (timenow > 990 and timenow < 1010) {
    i = i + 1000;
    CurrentTime.Sec = CurrentTime.Sec + 1;
    if (CurrentTime.Sec == 60) {
      CurrentTime.Minute = CurrentTime.Minute + 1;
      CurrentTime.Sec = 00;
    }

    if ( CurrentTime.Minute == 60) {
      CurrentTime.Hour = CurrentTime.Hour + 1;
      CurrentTime.Minute = 00;
    }
    if ( CurrentTime.Hour == 24) {
      CurrentTime.Sec = 00;
      CurrentTime.Minute = 00;
      CurrentTime.Hour = 00;
    }
  }
  
  /*Serial.print ("Time: ");
  Serial.print ( CurrentTime.hour);
  Serial.print ( ":");
  Serial.print ( CurrentTime.minute);
  Serial.print ( ":");
  Serial.println ( CurrentTime.sec);
  //delay(200);*/
}

void requestEvent(){
  char svar[3] = { CurrentTime.Hour, CurrentTime.Minute, CurrentTime.Sec };
  //char svar[] = "13";
  //Serial.println(svar);
  Wire.write(svar,3);
}
