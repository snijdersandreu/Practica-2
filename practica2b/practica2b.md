# Pràctica 2B: Interrupció per timer
L'objectiu d'aquesta pràctica és continuar treballant amb les interrupcions però aquesta vegada seràn invocades per un "timer".
___
## CODI DE LA PRÀCTICA
```
#include <Arduino.h>

volatile int interruptCounter;
int totalInterruptCounter;
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}
 
void setup() {
 
  Serial.begin(115200);
 
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
 
}
 
void loop() {
 
  if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;
 
    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
 
  }
}
```
___
## FUNCIONAMENT
En aquest cas la interrupció serà periodica ja que es cridarà a través d'un temporitzador propi del programa.
### Declaració de variables
```
volatile int interruptCounter;
int totalInterruptCounter;
```
La primera variable s'utilitzarà per comunicar el codi de la interrupció amb el codi del loop i servirà per tenir un registre de si s'ha produït una interrupció. Per aquest motiu és de tipus "volatile" ja que s'emmagatzemarà a la RAM i no hi haurà perill de que es borri quan no s'estigui executant algun dels dos processos i tots dos podràn accedir-hi.

La segona variable s'utilitzarà com un contador total d'interrupcions.
___
Tot seguit es declaren dues variables més, que s'utilitzaràn per poder sincronitzar la interrupció i el procés loop:
```
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
```
___
### Codi de la interrupció
```
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}
```
Aquest codi simplement sumarà 1 al contador de interrupcions quan es produeixi una.
___
### Setup
```
void setup() {
 
  Serial.begin(115200);
 
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
 
}
```
S'inicialitza el temporitzador "timer" i s'afegeix la interrupció amb aquest mateix timer.
____
### Loop
```
void loop() {
 
  if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;
 
    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
 
  }
}
```
El codi que trobem al loop senzillament revisa si s'ha produit una interrupció per poder afegir-la al contador global. Això ho fa mitjantçant el següent condicional:
```
if (interruptCounter > 0) {
    ---
  }
```
És a dir, si el contador d'interrupcions presenta un valor diferent de 0 (1 en qualsevol cas), executarà el codi que sumarà 1 al contador global i li restarà 1 al contador "volatile" (el que comparteixen la interrupció i el loop) fent-lo retornar a 0. 

Ens mostrarà pel terminal cada vegada que això succeixi i ens dirà el nombre total d'interrupcions. Així repetidament:

![terminalp2b](terminalp2b.PNG)