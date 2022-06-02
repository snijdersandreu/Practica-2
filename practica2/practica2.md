# Pràctica 2A: Interrupció per GPIO 
L'objectiu d'aquesta pràctica és conéixer que són les interrupcions, com implementar-les i com funcionen. Això ens ajudarà també a saber quan convé utilitzar-les i quan no.
___
## CODI DE LA PRÀCTICA
```
#include <Arduino.h>
#define LED 16
#define INTER 0 

void IRAM_ATTR isr(){
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
  static uint32_t lastMillis=0;
  if(millis()-lastMillis>60000){
    lastMillis=millis();
    detachInterrupt(INTER);
    Serial.println("Interrupcio finalitzada");
  }
}
```
___
## FUNCIONAMENT
En aquest apartat de la pràctica declararem una interrupció per hardware.
### Declaració dels pins
Primerament declarem els pins que utilitzarem. El pin 16 pel led i 0 per l'interruptor (el pin GPIO_0 es correspont amb l'interruptor BOOT propi de la ESP32):
```
#define LED 16
#define INTER 0 
```
___
### Declaració d'interrupció
Tot seguit declarem l'acció d'interrupció. És a dir el codi que s'executarà en el moment de la interrupció en qüestió:
```
void IRAM_ATTR isr(){
  digitalWrite(LED, LOW);
  Serial.println("OFF");
  delay(1000);
};
```
Aquesta acció apaga el led, mostra "OFF" per pantalla i aplica un delay de 1000ms abans d'acabar la interrupció. Aixó resultarà en que quan s'executi la interrupció el led s'apagarà durant 1 segon.
___
### Setup
```
void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(INTER, INPUT_PULLUP);
  attachInterrupt(INTER, isr, FALLING);
}
```
Declarem els modes dels pins: pin de sortida pel led i d'entrada per l'interruptor. 
El mode "INPUT_PULLUP" permet detectar la polsació de l'interruptor com a entrada. 

Seguidament, hem afegit la interrupció amb la acció que hem declarat anteriorment:
```
attachInterrupt(INTER, isr, FALLING);
```
* "INTER" indica l'input que farà que s'executi la interrupció.

* "isr" és el nom de la acció declarada anteriorment

* "FALLING" indica que s'activarà o s'executarà la interrupció en el moment en que l'interruptor passi de nivell alt (HIGH) a nivell baix (LOW). Això és degut a que el GPIO_0 per defecte es troba en nivell alt, i al prémer el botó BOOT (propi de la ESP32) pasa a nivell baix durant aquell instant. 
___
### Loop
```
void loop() {
  digitalWrite(LED, HIGH);
  Serial.println("ON");
  static uint32_t lastMillis=0;
  if(millis()-lastMillis>60000){
    lastMillis=millis();
    detachInterrupt(INTER);
    Serial.println("Interrupcio finalitzada");
  }
}
```
Les primeres dues linies de codi simplement encén el led i mostra "ON" per pantalla.

 Tot seguit, s'inicialitza una variable de tipus "static uint_32". El seu valor inicial serà 0 i s'utilitzarà per tenir un registre de l'ultim instant (en milisegons) que hi ha hagut una interrupció. 

* "static" implica que la variable es crea una unica vegada, amb la primera crida, és a dir, la primera execució del loop.

*  "uint_32" és un tipus de variable "int" on el prefix "u"(unsigned) indica que nomes pot prendre valors positius (per tant pot emagatzemar nombres el doble de grans ja que el bit de mes importància, normalment reservat pel signe, s'utilitza per codificar el nombre) i la terminació "_32" indica que el tamany de memoria de la variable (32bits, el valor màxim serà 2^32)

La part final del codi serveix per, quan l'interruptor porti  60 segons sense ser utilitzat, el programa desactivi aquest i mostri per pantalla "Interrupció finalitzada":
```
  if(millis()-lastMillis>60000){
    lastMillis=millis();
    detachInterrupt(INTER);
    Serial.println("Interrupcio finalitzada");
  }
```
Per poder tornar a utilitzar el programa caldrà prèmer el botó RESET de la ESP32.
