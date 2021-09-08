
//#include<SoftwareSerial.h> //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
//SoftwareSerial s(D7, D8);
#include <Ticker.h>


#define DELAY_CYCLES(n) __builtin_avr_delay_cycles(n)
byte dataRx [32000]={};
byte dataCommand [3]={0x00,0x00,0x00};
int count = 0, i = 0;
long countRx = 0;
bool flagCompleteRx = false;
int clocks = 0;
int ledState = LOW;
Ticker blinker;

void changeState()
{
  digitalWrite(D6, !(digitalRead(D6)));  //Invert Current State of LED  
}

void setup() {
  // start serial port at 9600 bp
  ESP.wdtDisable();
  ESP.wdtEnable(WDTO_8S);
  Serial.begin(115200);
  Serial.swap(); //Dung GPIO13 va GPIO15 lam RX,TX
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  blinker.attach(1, changeState);
}

// the loop function runs over and over again forever
void loop() {
//  digitalWrite(D6 , LOW);
//  blinker.detach();
  while (Serial.available()) {
    //Serial.write("Hello");
    //s.write("Hello");
    //Serial.write((byte)s.read());
    dataRx[countRx] = (byte)Serial.read();
    if(dataRx[countRx] == 0xAA){
      countRx = 0;
    }
    if(dataRx[countRx] == 0xEE){
      flagCompleteRx = true;
    }
    countRx++;

//    if(count < 3){
//      dataCommand[count] = (byte)Serial.read();
//      count++;
//    }
//    if(count == 3){
//      if(dataCommand[0] == 0xAA){
//        for (i=0;i<3;i++){
//          dataRx[i]= dataCommand[i];
//        }
//        countRx = count;          
//      }
//      else if(dataCommand[0] == 0xAE){
//        if(dataCommand[2] == 0xEE){
//          flagCompleteRx = true;
//          if (dataCommand[1] == 0x01){
//            digitalWrite(D6 , HIGH);
//            blinker.detach();
//          }
//          else 
//            blinker.attach(1, changeState);
//        }
//      }
//    }
//    if(countRx > 3){
//      dataRx[countRx] = (byte)Serial.read();
//      if(dataRx[countRx]==0xEE)
//        flagCompleteRx = true;
//      countRx++;
//    }
    //delay(1); 
    
    if(flagCompleteRx){  
      digitalWrite(D6 , HIGH);
      blinker.detach();
      flagCompleteRx = false;
      for(long i=0; i<=countRx-1;i++){
        Serial.write(dataRx[i]);
        ESP.wdtFeed();
      }
      while(!Serial.available()){
        //ESP.wdtFeed();
        for(long i=1; i<=countRx-2;i++){
          if(dataRx[i] == 0xFF){
            digitalWrite(D5 , HIGH);        
          }
          else{
            digitalWrite(D5, LOW);
          }
          clocks = 12;
          __asm( "nop" );
          __asm( "nop" );
          __asm( "nop" );
          do{
            __asm( "nop" );
//            __asm( "nop" );
//            __asm( "nop" );
          }while ( --clocks );
          //yield();
        }     
      }
    }
  }
}


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
//void serialEvent() {
//  while (Serial.available()) {
//    //Serial.write("Hello");
//    //s.write("Hello");
//    //Serial.write((byte)s.read());
//    dataRx[countRx] = (byte)Serial.read();
//    if (dataRx[countRx] == 0xAA) {
//      countRx = 0;
//    }
//    if (dataRx[countRx] == 0xEE) {
//      flagCompleteRx = true;
//    }
//    // get the new byte:
//    countRx++; 
//    //delay(1); 
//    
//    if(flagCompleteRx){
//      flagCompleteRx = false;
//      for(long i=0; i<=countRx-1;i++){
//        Serial.write(dataRx[i]);
//        //yield();
//      }
//    }
//  }
//}
