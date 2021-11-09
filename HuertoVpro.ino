#include <BH1750FVI.h>
#include <SoftwareSerial.h> //se incluye libreria para uso de bluetooth
#include <Adafruit_Sensor.h> //librerias para sensorTH DHT - Adafruit unified sensor
#include <DHT.h> //librerias para sensorTH -DHT
#include <DHT_U.h>
#include <Wire.h>
//#include <BH1750.h> //Libreria para BH1750

/*
0analogo 
1analogo 
2analogo HUMEDADTIERRA 2
3analogo LUZ
4analogo LUZ
5analogo HUMEDADTIERRA

0digital rx BT
1digital tx BT
2digital TMHD
*3digital red
4digital VENTILADOR
*5digital RELEAGUA
*6digital green
7digital LED
8digital RELEHUMEDAD
*9digital blue
*10digital RELEFRIO
*11digital RELECALIENTE
12digital  TMHD 2
13digital  
*/

int z,r; //variables para control de temporizador
int TMHD = 2;//digitalRead(2);; //DIGITAL sensor temperatura humedad
int TMHD2 = 12; //digitalRead(12); //DIGITAL
int TEMPERATURA; 
int HUMEDAD;
int RELEHUMEDAD = 8;//digitalRead(8); //DIGITAL
int VENTILADOR = 4;//digitalRead(4); //DIGITAL
int RELEFRIO = 10;//digitalRead(10); //DIGITAL 
int RELECALIENTE = 11;//digitalRead(11); //DIGITAL

char dato = 0;
char buffer[30];

int LED = 7;//digitalRead(7); //DIGITAL LED indicador de luminosidad y prueba
int VALORLUZ=0;//analogRead(0); //ANALOGO

int HUMEDADTIERRA = 5;//analogRead(5); //ANALOGO
int HUMEDADTIERRA2 = 3;//analogRead(3); //ANALOGO
int RELEAGUA = 5;//digitalRead(5); //DIGITAL

float red = 3;//digitalRead(3);
float green = 6;//digitalRead(6);
float blue = 9;//digitalRead(9);

//BH1750FVI Luxometro;
DHT dht(TMHD, DHT11);
SoftwareSerial HUERTRONIC(0,1); //creacion de Bluetooth RX11 TX10 - SoftwareSerial HUERTO(10,11)

void setup(){

  Serial.begin(9600); //inicializa TH
    dht.begin();
    pinMode(LED, OUTPUT); 
//    pinMode(RELERGB, OUTPUT); 
    pinMode(RELEAGUA, OUTPUT); 
    pinMode(RELEFRIO,OUTPUT); 
    pinMode(RELECALIENTE, OUTPUT); 
    pinMode(RELEHUMEDAD, OUTPUT); 
    pinMode(red, OUTPUT);   
    pinMode(green, OUTPUT); 
    pinMode(blue, OUTPUT);  
    pinMode(VENTILADOR, OUTPUT); 
    HUERTRONIC.begin(9600);      //velocidad de comunicacion
//   Luxometro.begin(BH1750_CONTINUOUS_HIGH_RES_MODE); //inicializamos el sensor

  
    //Se da un valor 0 para inicializar en off el PWM de los led RGB

  analogWrite(red,0);
  analogWrite(green,0);
  analogWrite(blue,0);
}

void loop(){



//uint16_t lux = Luxometro.readLightLevel();//Realizamos una lectura del sensor
  TEMPERATURA = dht.readTemperature(); //lectura T
  HUMEDAD = dht.readHumidity(); //lectura H
  int valorluz2= VALORLUZ;
  int val=analogRead(5); //lectura de Sensor Humedad tierra
  int val2=(((val*100)/1024)*-1)+100;
  
  int temp=TEMPERATURA;
  int hum= HUMEDAD;
  int luz=VALORLUZ;
  int tierra=val2; 
  

//----------------------ENVÍO DE DATOS A LA APP----------------
  sprintf(buffer,"%d.%d.%d.%d",temp,hum,luz,tierra); // Se almacenan datos en buffer y se configuran para el string
  Serial.println(buffer);
   
//----------------------BLUETOOTH Y LED DE PRUEBA----------------
  if (HUERTRONIC.available()){
    dato = HUERTRONIC.read();
  if (dato == '1')
    digitalWrite(LED, HIGH);
    
  if (dato == '2')
    digitalWrite(LED, LOW);
     }

//----------------------CONDICIONALES MODOS-----------------------------
//**Fresa**
if (dato=='3'){
  
  int HUMEDADTIERRA=analogRead(5);
  if (val2 <= 0 ){
    digitalWrite(RELEAGUA,HIGH); 
    }
  if (val2 > 35 ){
    digitalWrite(RELEAGUA,LOW);
    }
    
  int TMHD = digitalRead(2);
  if (HUMEDAD < 61){
    digitalWrite(RELEHUMEDAD,HIGH);
    }
  if (HUMEDAD > 68){
    digitalWrite(RELEHUMEDAD,LOW);
    }
  if (TEMPERATURA < 14){
    digitalWrite(RELECALIENTE,HIGH);
    digitalWrite(RELEFRIO,HIGH);
    }
  if (TEMPERATURA >= 13){
    digitalWrite(RELECALIENTE,LOW);
    digitalWrite(RELEFRIO,LOW);
    }
 
   }
  
//**Calido**
if (dato=='4'){
  
  int HUMEDADTIERRA=analogRead(5);
  if (val2 < 36 ){
    digitalWrite(RELEAGUA,HIGH); 
    }
  if (val2 > 70 ){
    digitalWrite(RELEAGUA,LOW);
    }
  int TMHD = digitalRead(2);
  if (HUMEDAD < 19){
    digitalWrite(RELEHUMEDAD,HIGH);
    }
  if (HUMEDAD > 27){
    digitalWrite(RELEHUMEDAD,LOW);
    }
  if (TEMPERATURA < 32){
    digitalWrite(RELECALIENTE,HIGH);
    digitalWrite(RELEFRIO,HIGH); //analogWrite(red,255) tambien en setup corregir
    }
  if (TEMPERATURA >= 35){
    digitalWrite(RELECALIENTE,LOW);
    digitalWrite(RELEFRIO,LOW);
    }
  if (TEMPERATURA >= 36){
    digitalWrite(RELEFRIO,HIGH);
    }
  
}

//**Templado**
if (dato=='5'){
  
  int HUMEDADTIERRA=analogRead(5);
  if (val2 < 11 ){
    digitalWrite(RELEAGUA,HIGH); 
    }
  if (val2 > 45 ){
    digitalWrite(RELEAGUA,LOW);
    }
    
  int TMHD = digitalRead(2);
  if (HUMEDAD < 61){
    digitalWrite(RELEHUMEDAD,HIGH);
    }
  if (HUMEDAD > 68){
    digitalWrite(RELEHUMEDAD,LOW);
    }
  if (TEMPERATURA < 26){
    digitalWrite(RELECALIENTE,HIGH);
    digitalWrite(RELEFRIO,HIGH);
    }
  if (TEMPERATURA >= 30){
    digitalWrite(RELECALIENTE,LOW);
    digitalWrite(RELEFRIO,LOW);
    }
  if (TEMPERATURA >= 31){
   digitalWrite(RELEFRIO,HIGH);
    }
}

//**Frio**
if (dato=='6'){
    
  int HUMEDADTIERRA=analogRead(5);
  if (HUMEDADTIERRA <= 0 ){
    digitalWrite(RELEAGUA,HIGH); 
    }
  if (HUMEDADTIERRA >= 20 ){
    digitalWrite(RELEAGUA,LOW);
    }
  
  int TMHD = digitalRead(2);
  if (HUMEDAD <= 45){
    digitalWrite(RELEHUMEDAD,HIGH);
    }
  if (HUMEDAD > 60){
    digitalWrite(RELEHUMEDAD,LOW);
    }
  if (TEMPERATURA <= 20){
    digitalWrite(RELECALIENTE,HIGH);
    digitalWrite(RELEFRIO,HIGH);
    }
  if (TEMPERATURA > 24){
    digitalWrite(RELECALIENTE,LOW);
    digitalWrite(RELEFRIO,LOW);
    }
  if (TEMPERATURA >= 25){
    digitalWrite(RELEFRIO,HIGH);
    }
}

//**OFFALL**
if (dato=='8'){
    
    digitalWrite(RELEAGUA,LOW);
//    analogWrite(RELERGB,LOW); 
    digitalWrite(RELEHUMEDAD,LOW);
    digitalWrite(RELECALIENTE,LOW);
    digitalWrite(RELEFRIO,LOW);
    analogWrite(red,LOW);
    analogWrite(green,LOW);
    analogWrite(blue,LOW);
}
 
//----------------------MANUAL----------------------------------------
//**TEMPERATURA**
if (dato=='t'){
  if (TEMPERATURA >12){
    digitalWrite(RELEFRIO,HIGH);
  }
  if (TEMPERATURA <= 11){
    digitalWrite(RELECALIENTE,LOW);
    digitalWrite(RELEFRIO,LOW);
  }
  if (TEMPERATURA < 0){
    digitalWrite(RELECALIENTE,HIGH);
    digitalWrite(RELEFRIO,HIGH);
  }
}
if (dato=='A'){
  if (TEMPERATURA >24){
    digitalWrite(RELEFRIO,HIGH);
  }
  if (TEMPERATURA <= 23){
    digitalWrite(RELECALIENTE,LOW);
    digitalWrite(RELEFRIO,LOW);
  }
  if (TEMPERATURA < 13){
    digitalWrite(RELECALIENTE,HIGH);
    digitalWrite(RELEFRIO,HIGH);
  }
}
if (dato=='T'){
  if (TEMPERATURA >36){
    digitalWrite(RELEFRIO,HIGH);
  }
  if (TEMPERATURA <= 35){
    digitalWrite(RELECALIENTE,LOW);
    digitalWrite(RELEFRIO,LOW);
  }
  if (TEMPERATURA < 25){
    digitalWrite(RELECALIENTE,HIGH);
    digitalWrite(RELEFRIO,HIGH);
  }
}
//**HUMEDAD AIRE**
if (dato=='h'){
  if (HUMEDAD < 0){
    digitalWrite(RELEHUMEDAD,HIGH);
  }
  if (HUMEDAD >30){
    digitalWrite(RELEHUMEDAD,LOW);
  digitalWrite(RELECALIENTE,HIGH);
    digitalWrite(RELEFRIO,HIGH);
  }
}
if (dato=='B'){
  if (HUMEDAD < 31){
    digitalWrite(RELEHUMEDAD,HIGH);
  }
  if (HUMEDAD >60){
    digitalWrite(RELEHUMEDAD,LOW);
  digitalWrite(RELECALIENTE,HIGH);
    digitalWrite(RELEFRIO,HIGH);
  }
}
if (dato=='H'){
  if (HUMEDAD < 61){
    digitalWrite(RELEHUMEDAD,HIGH);
  }
  if (HUMEDAD >90){
    digitalWrite(RELEHUMEDAD,LOW);
  digitalWrite(RELECALIENTE,HIGH);
    digitalWrite(RELEFRIO,HIGH);
  }
}
//**LUMINOSIDAD**
/*if (dato=='l'){
  if (HUMEDAD >12){
    digitalWrite(RELEFRIO,HIGH);
  }
  if (HUMEDAD <= 11){
    digitalWrite(RELECALIENTE,LOW);
    digitalWrite(RELEFRIO,LOW);
  }
  if (HUMEDAD < 0){
    digitalWrite(RELECALIENTE,HIGH);
    digitalWrite(RELEFRIO,HIGH);
  }
}
if (dato=='C'){
  
}
if (dato=='L'){
  
}
*/
//**HUMEDAD TIERRA**
if (dato=='s'){
  if (val2 < 0){
    digitalWrite(RELEAGUA,HIGH);
  }
  if (val2 > 30){
    digitalWrite(RELEAGUA,LOW);
    digitalWrite(RELECALIENTE,HIGH);
  digitalWrite(RELEFRIO,HIGH);
  }
}
if (dato=='D'){
  if (val2 < 31){
    digitalWrite(RELEAGUA,HIGH);
  }
  if (val2 > 60){
    digitalWrite(RELEAGUA,LOW);
    digitalWrite(RELECALIENTE,HIGH);
  digitalWrite(RELEFRIO,HIGH);
  }
}
if (dato=='S'){
  if (val2 < 61){
    digitalWrite(RELEAGUA,HIGH);
  }
  if (val2 > 90){
    digitalWrite(RELEAGUA,LOW);
    digitalWrite(RELECALIENTE,HIGH);
  digitalWrite(RELEFRIO,HIGH);
  }
}

//Activacion y tiempos de ventilador
if (dato=='V'){
  z=millis()/1000;
  while(z==0)
    {
      r=(millis()/1000)-z;
      Serial.println(r);
      delay(1000);
    }
    if (r>=10){
      digitalWrite(VENTILADOR,HIGH);
    } 
    if (r==15){
      digitalWrite(VENTILADOR,LOW);
      r=0; 
     }
   }

 //Apagar ventilacion
 if (dato=='F'){
   digitalWrite(VENTILADOR,LOW);
 }
 
//----------------------LEDS RGB-------------------------------------
//**LEDs OFF**
  if (dato == 'n')
  {
    analogWrite(red,0);
    analogWrite(green,0);
    analogWrite(blue,0);
  }
  //**White**
  if (dato == 'w')
  {
    analogWrite(red,255);
    analogWrite(green,255);
    analogWrite(blue,255);
  }
  //**Red**
  if (dato == 'r')
  {
    analogWrite(red,255);
    analogWrite(green,0);
    analogWrite(blue,0);
  }
  //**Green**
  if (dato == 'g')
  {
    analogWrite(red,0);
    analogWrite(green,255);
    analogWrite(blue,0);
  }
  //**Blue**
  if (dato == 'b')
  {
    analogWrite(red,0);
    analogWrite(green,0);
    analogWrite(blue,255);
  }
  //**Orange**
  if (dato == 'o')
  {
    analogWrite(red,255);
    analogWrite(green,153);
    analogWrite(blue,0);
  }
  //**Violet**
  if (dato == 'v')
  {
    analogWrite(red,102);
    analogWrite(green,0);
    analogWrite(blue,153);
  }
  //**Cyan**
  if (dato == 'c')
  {
    analogWrite(red,0);
    analogWrite(green,255);
    analogWrite(blue,255);
  }
  //**Yellow**
  if ( dato == 'y')
  {
    analogWrite(red,255);
    analogWrite(green,204);
    analogWrite(blue,0);
  }  
  
delay(2000); //Tiempo de 2 segundos entre lecturas
}
