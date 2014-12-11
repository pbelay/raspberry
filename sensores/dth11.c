//Incluimos librerias necesarias
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//Definimos constantes
#define MAX_TIME 85
#define DHT11PIN  4
#define ATTEMPTS 10

//Definimos un vector global
int dht11_val[5]={0,0,0,0,0};

/////////////////////////////////////////////////////////////
//Funcion principal para leer los valores del sensor.
/////////////////////////////////////////////////////////////
int dht11_read_val(){
 uint8_t lststate=HIGH;
 uint8_t counter=0;
 uint8_t j=0,i;
 for(i=0;i<5;i++){
  dht11_val[i]=0;
 }
 pinMode(DHT11PIN,OUTPUT);
 digitalWrite(DHT11PIN,LOW);
 delay(18);
 digitalWrite(DHT11PIN,HIGH);
 delayMicroseconds(40);
 pinMode(DHT11PIN,INPUT);

 for(i=0;i<MAX_TIME;i++){
  counter=0;
  while(digitalRead(DHT11PIN)==lststate){
   counter++;
   delayMicroseconds(1);
   if(counter==255){
    break;
   }
  }

  lststate=digitalRead(DHT11PIN);
  if(counter==255){
   break;
  }
  //As 3 primeras transiciones son ignoradas
  if((i>=4)&&(i%2==0)){
   dht11_val[j/8]<<=1;
   if(counter>16){
    dht11_val[j/8]|=1;
   }
   j++;
  }
 }
  
//Comprobamos que o dato sexa correcto
if((j>=40)&&(dht11_val[4]==((dht11_val[0]+dht11_val[1]+dht11_val[2]+dht11_val[3])& 0xFF))){
  printf("%d.%d,%d.%d\n",dht11_val[0],dht11_val[1],dht11_val[2],dht11_val[3]);
  return 1;
 }else{
  return 0;
 }
}

////////////////////////////////////////////////////////////////
//                      PROGRAMA PRINCIPAL
////////////////////////////////////////////////////////////////
int main(void){
 //Establecemos o número de intentos que imos a realizar
 int attempts=ATTEMPTS;
 
 //Se a libreria wiringPi detecta que o GPIO non esta preparado 
 if(wiringPiSetup()==-1){
  printf("wiringPiSetup no esta listo");
  exit(1);
 }
 
 while(attempts){
  //Leemos o valor do gpio chamando a función
  int success = dht11_read_val();
  
  if (success){
      break; //Se a lectura é OK, saimos.
  }

 //Se a lectura NOK, intentamos unha vez mais ata esgotar 
 //O numero de intentos
 attempts--;
  
 //Esperamos medio segundo antes do seguinte intento.
 delay(500);
 }
 return 0;
}


