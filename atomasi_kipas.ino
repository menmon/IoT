#define BLYNK_TEMPLATE_ID "TMPL69WEkNLhF"
#define BLYNK_TEMPLATE_NAME "suhu"
#define BLYNK_AUTH_TOKEN "HrGZw3vlB3jAHyQYBhkz_mHNTQrVhFWh"
// Memuat pustaka yang diperlukan
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Menyimpan token auth Blynk, ganti dengan token auth Anda
char auth[] = "HrGZw3vlB3jAHyQYBhkz_mHNTQrVhFWh"; 
// Menyimpan SSID dan password WiFi
char ssid[] = "Mutualan_plus"; //ganti dengan SSID Anda
char pass[] = "Mamaxedo220701"; //ganti dengan password Anda

/**********Google Sheets Definations***********/
#include "TRIGGER_WIFI.h"
#include "TRIGGER_GOOGLESHEETS.h"    /*Library file for Google Sheets, has to be used after Wi-Fi Client Secure declaration, here everything is in Trigger_WIFI.h, so using it after Trigger_WIFI.h*/ 
char column_name_in_sheets[ ][20] = {"value1","value2"};
String Sheets_GAS_ID = "AKfycbwi3bxLxlR8HAly2po5ZUI2mK9IsJEYvvYPLbar5deRr8Sf4YPuwNY61JDMr1NFF_bY";
int No_of_Parameters = 2;  /*Here No_of_Parameters decides how many parameters you want to send it to Google Sheets at once, change it according to your needs*/
/*********************************************/


#include "PIR.h"

//mendefinisikan sensor suhu
#include "DHT.h"
// Mendefinisikan pin DHT
#define DHTPIN D5
// Mendefinisikan jenis DHT
#define DHTTYPE DHT22
// Membuat objek DHT
DHT dht(DHTPIN, DHTTYPE);

//mendefinisikan sensor cahaya
float sensorldr = A0;
float density;

//mendefinisikan sensor gerakan
#define pir1  D0
#define pir2  D1
#define pir3  D2
#define pir4  D3




//mendefinisikan PWM Kipas
const int dcMotor = D3;
int slider;
const int inPut = D4;
// const int inPut;
int mappedValue=0;

//mendefinisikan Lampu
const int Dimmer = D6;
int sliderlampu;
const int inputlampu = D7;
// const int inputlampu;
int ValueLampu=0;

int trigger = 1;

float treshold = 30;
float downhold = 27;

float downholdlampu = 300;




void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  
  // Menghubungkan ke WiFi
  Blynk.begin(auth, ssid, pass);

  // Menginisialisasi sensor DHT
  dht.begin();

  // Menginisialisasi sensor Cahaya
  pinMode (sensorldr, OUTPUT);

  //deklarasi PIR sebagai input
  pinMode(pir1, INPUT); 
  pinMode(pir2, INPUT); 
  pinMode(pir3, INPUT); 
  pinMode(pir4, INPUT); 

  // Wire.begin(D2, D1);
  // lightMeter.begin();


  pinMode(dcMotor,OUTPUT);
  pinMode(inPut, OUTPUT);
  digitalWrite(inPut, HIGH);

  pinMode(Dimmer,OUTPUT);
  pinMode(inputlampu, OUTPUT);
  digitalWrite(inputlampu, HIGH);

   //Google Sheet
  while (!Serial);
  // dht.begin();
  WIFI_Connect("Mutualan_plus","Mamaxedo220701");
  Google_Sheets_Init(column_name_in_sheets, Sheets_GAS_ID, No_of_Parameters );


}

BLYNK_WRITE(V2) {
  trigger = param.asInt();
}

BLYNK_WRITE(V3){
  slider = param.asInt(); 
  mappedValue = map(slider,0,1023,0,255);
  //generate PWM signal
  // analogWrite(dcMotor, mappedValue);
}

BLYNK_WRITE(V4){
  sliderlampu = param.asInt(); 
  ValueLampu = map(sliderlampu,0,1023,0,255);
  //generate PWM signal
  // analogWrite(Dimmer, mappedValue);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();

  float suhu = dht.readTemperature();
  suhu = (suhu - 0.2);
  Blynk.virtualWrite(V0, suhu);
  // float h = dht.readHumidity();
  // Membaca suhu dalam derajat Celcius

  // deklarasi sensor cahaya
  float density = analogRead(sensorldr);
   density = 1000 - density;
  Blynk.virtualWrite(V1, density);

  
  long sensorpir1 = digitalRead(pir1);
  long sensorpir2 = digitalRead(pir2);
  long sensorpir3 = digitalRead(pir3);
  long sensorpir4 = digitalRead(pir4);

  float statuspir1;
  float statuspir2;
  float statuspir3;
  float statuspir4;
  float statuskipas;
  float statuslampu;
  

  if (trigger == 1) {
     Serial.print("\n");
     Serial.print("Sistem Otomatis Berjalan");
     Serial.print("\n");

      if((sensorpir1 == HIGH)||(sensorpir2 == HIGH)||(sensorpir3 == HIGH)||(sensorpir3 == HIGH){
          Blynk.virtualWrite(V5, 1);
        Serial.println("Motion detected!");

            // kontrol kipas

            
           if( treshold <  suhu ) {
           digitalWrite(dcMotor, 255);
           Blynk.virtualWrite(V3, 1023);
          //  statuskipas = 1.00;

           }else if(  treshold >=  suhu && downhold <= suhu){      
           digitalWrite(dcMotor, 180);
           Blynk.virtualWrite(V3, 512);
            // statuskipas = 0.00;

           }else{
            digitalWrite(dcMotor, 0);
           Blynk.virtualWrite(V3, 0);
           }


          // kontrol lampu
           if( density <= downholdlampu ){
           digitalWrite(Dimmer, 255);
           Blynk.virtualWrite(V4, 1023);
          // statuslampu = 1;

          }else{
           digitalWrite(Dimmer, 0);
           Blynk.virtualWrite(V4, 0);
          //  statuslampu = 0;
          }
      } 
}else{
    Serial.print("\n");
    Serial.print("Sistem Otomatis Berhenti");
    Serial.print("\n");

    //Menjalankan Kipas Manual
    analogWrite(dcMotor, mappedValue);
    //Menjalankan Lampu Manual
    analogWrite(Dimmer, ValueLampu);

  
}
    if(sensorpir1 == HIGH){
        statuspir1 = 1.00;
    }else{
        statuspir1 = 0.00;
    }

    if(sensorpir2 == HIGH){
        statuspir2 = 1.00;
    }else{
        statuspir2 = 0.00;
    }

    if(sensorpir3 == HIGH){
        statuspir3 = 1.00;
    }else{
        statuspir3 = 0.00;
    }

    if(sensorpir4 == HIGH){
        statuspir4 = 1.00;
    }else{
        statuspir4 = 0.00;
    }


  // //mengirim suhu ke pin virtual V5
    Blynk.virtualWrite(V0, suhu);
  Serial.print(" Suhu : ");
  Serial.print(suhu);
  Serial.print("\n");

  Serial.print("Light: ");
  Serial.print(density);
  Serial.print("\n");

  Serial.print("statuspir1: ");
  Serial.print(statuspir1);
  Serial.print("\n");

  Serial.print("statuspir2: ");
  Serial.print(statuspir2);
  Serial.print("\n");

  Serial.print("statuspir3: ");
  Serial.print(statuspir3);
  Serial.print("\n");

  Serial.print("statuspir4: ");
  Serial.print(sensorpir3);

  Serial.print("\n");

  Serial.print("\n");

  // Menjalankan fungsi Blynk
  
  delay(1000);  

  WIFI_Connect("Mutualan_plus","Mamaxedo220701");
  Data_to_Sheets(No_of_Parameters, suhu, density);         /*1. This function accepts multiple float parameter, here No_of_Parameters decides how many parameters you want to send to Google Sheets; 2. The values sent should be in order as per the column in Google Sheets*/
  Serial.println();
   delay(5000);     
}
