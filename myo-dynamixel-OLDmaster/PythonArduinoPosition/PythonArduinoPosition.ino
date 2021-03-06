#include <Dynamixel2Arduino.h> //Used for controlling the dynamixel servos
#include <Adafruit_NeoPixel.h> //Used for the indicator LEDs

  #define LEDPIN 3    //Pin that the indicator LEDs are connected to
  #define LEDNUM 24   //Number of NeoPixel LEDs

  #define DXL_SERIAL Serial1 //Serial that RS485 is connected to
  #define REC_SERIAL Serial //Serial from computer, receives data from myoband
  const uint8_t DXL_DIR_PIN = 2; // RS485 direction (LED) pin

  uint8_t DXL_ID = 4; //Starting id of a servo
  const float DXL_PROTOCOL_VERSION = 2.0;

  int debLED = 13;
  int Opt = 0;
  int pos = 150;
  int musclestr = 1;
  Adafruit_NeoPixel ledindicator(LEDNUM, LEDPIN, NEO_GRB + NEO_KHZ800); //Creating object for LEDs
  Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN); //Creating object for dynamixel communication

void setup() {
  REC_SERIAL.begin(57600); //Computer serial baudrate, has to be the same as in our python code

  pinMode(debLED, OUTPUT);

  dxl.begin(115200); //Dynamixel baudrate, has to be the same as on the servos

  
  
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);//Sets dynamixel protocol
  dxl.ping(DXL_ID);


for(int i=1; i<=5; i++){ //We have 
  dxl.torqueOff(i);
  dxl.setOperatingMode(i, OP_POSITION); //Sets operating mode of dynamixels
  dxl.torqueOn(i);
}

  pos = dxl.getPresentPosition(DXL_ID);
  ledindicator.begin(); // Initializes NeoPixel leds
  ledindicator.setBrightness(5); //Sets the brightness (can be changed)
  ledindicator.clear(); //Clears the currently displayed
  indicatorset();
}
void colorset(int a, int r, int g, int b){
  for(int i=a; i<(a+6); i++) { //For leds from a until a+6 (1/4 of the ring) a is 0 , 6, 12 , 18

    ledindicator.setPixelColor(i, ledindicator.Color(r,g,b));
  }
}
void indicatorset(){
    ledindicator.clear();
    colorset(0,255,0,0);
    if (DXL_ID > 1){
      colorset(6,0,255,0);
    }
    if(DXL_ID > 2){
    colorset(12,0,0,255);
    }
    if(DXL_ID > 3){
      colorset(18,255,255,255);
    }
    ledindicator.show();
}
void loop() {

  switch(Opt)
    {
  
  case 1:
    digitalWrite(debLED, HIGH);
    if(DXL_ID==4){
      DXL_ID=1;
    }
    else{
      DXL_ID++;
    }
    pos = dxl.getPresentPosition(DXL_ID);
    Opt = 0;
    indicatorset();
    break;
  case 2:
    digitalWrite(debLED, HIGH);
    if(dxl.getPresentPosition(DXL_ID)-pos < 24){
      if (DXL_ID==4)
      pos=pos+2*musclestr+2;
      if (DXL_ID==3)
      pos=pos+2*musclestr;
      if (DXL_ID==2)
      pos=pos+musclestr;
      if (DXL_ID==1){
        if(musclestr==1)
        pos=pos+1;
        if(musclestr==2)
        pos=pos+3;
      }
      

    }
    else{
      pos = dxl.getPresentPosition(DXL_ID);
    }
    dxl.setGoalPosition(DXL_ID, pos);
    break;
  case 3:
    digitalWrite(debLED, HIGH);
    if(dxl.getPresentPosition(DXL_ID)-pos > -24){
      if (DXL_ID==4)
      pos=pos-2*musclestr-2;
      if (DXL_ID==3)
      pos=pos-2*musclestr;
      if (DXL_ID==2)
      pos=pos-musclestr;
      if (DXL_ID==1){
        if(musclestr==1)
        pos=pos-1;
        if(musclestr==2)
        pos=pos-3;
      }
    }
    else{
      pos = dxl.getPresentPosition(DXL_ID);
    }
    dxl.setGoalPosition(DXL_ID, pos);
    break;
  case 4:
    digitalWrite(debLED, HIGH);
    if(DXL_ID==1){
      DXL_ID=4;
    }
    else{
      DXL_ID--;
    }
    pos = dxl.getPresentPosition(DXL_ID);
    Opt = 0;
    indicatorset();
    break;
  default:
    digitalWrite(debLED, LOW);
    dxl.setGoalPosition(DXL_ID, pos);
 
  
}

if(REC_SERIAL.available()>0){
 delay(10);
 String Str = REC_SERIAL.readStringUntil('x');
  
 if(Str=="fist"){
      digitalWrite(debLED, HIGH);
      Opt = 1;
    }
    else if(Str=="in"){
      digitalWrite(debLED, HIGH);
      Opt = 2;
      musclestr = 1;
    }
    else if(Str=="Sin"){
      digitalWrite(debLED, HIGH);
      Opt = 2;
      musclestr = 2;
    }
    else if(Str=="out"){
      digitalWrite(debLED, HIGH);
      Opt = 3;
      musclestr = 1;
    }
    else if(Str=="Sout"){
      digitalWrite(debLED, HIGH);
      Opt = 3;
      musclestr = 2;
    }
    else if(Str=="fingers"){
      digitalWrite(debLED, HIGH);
      Opt = 4;
    }
    else if(Str=="rest"){
      digitalWrite(debLED, LOW);
      Opt = 0;
      if(DXL_ID!=4){
      pos = dxl.getPresentPosition(DXL_ID);
      dxl.setGoalPosition(DXL_ID, pos);
      }
    }
  }


}
