#include <Arduino.h>
#define LED 16
#define INTER 0 

void IRAM_ATTR isr(){
  /*codi a exetuar en l'interrupció,
  apagar el led*/
  digitalWrite(LED, LOW);
  Serial.println("OFF");
  delay(1000);
};

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(INTER, INPUT_PULLUP);
  attachInterrupt(INTER, isr, FALLING);
}

void loop() {
  digitalWrite(LED, HIGH);
  Serial.println("ON");
  /*codi per aturar la interrupció despres de 5segons,
  és a dir, tornar a encendre el led*/
  static uint32_t lastMillis=0;
  //s'utilitza una variable "static" per tal que només s'inicialitzi una unica vegada amb la primera crida
  //a més la variable no es destrueix si no s'esta executant la funcio loop (amb valor inicial 0).
  if(millis()-lastMillis>60000){
    lastMillis=millis();
    detachInterrupt(INTER);
    Serial.println("Interrupcio finalitzada");
  }
}