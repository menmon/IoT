#define BLYNK_TEMPLATE_ID "TMPL69WEkNLhF"
#define BLYNK_TEMPLATE_NAME "suhu"
#define BLYNK_AUTH_TOKEN "HrGZw3vlB3jAHyQYBhkz_mHNTQrVhFWh"
// Memuat pustaka yang diperlukan

#include "TRIGGER_WIFI.h"
#include "TRIGGER_GOOGLESHEETS.h"    /*Library file for Google Sheets, has to be used after Wi-Fi Client Secure declaration, here everything is in Trigger_WIFI.h, so using it after Trigger_WIFI.h*/ 
char column_name_in_sheets[ ][20] = {"value1","value2"};
String Sheets_GAS_ID = "AKfycbyckpCQ1XRGR4p_KY8NQeXI3i5Z6omN4SYhpnkxWn9qTG7-1w1eoXUgQUMc_vEUWZBU";
int No_of_Parameters = 2;


#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "DHT.h"
// Mendefinisikan pin DHT
#define DHTPIN D3
// Mendefinisikan jenis DHT
#define DHTTYPE DHT22

// Membuat objek DHT
DHT dht(DHTPIN, DHTTYPE);

// Menyimpan token auth Blynk, ganti dengan token auth Anda
char auth[] = "HrGZw3vlB3jAHyQYBhkz_mHNTQrVhFWh"; 
// Menyimpan SSID dan password WiFi
char ssid[] = "Mutualan_plus"; //ganti dengan SSID Anda
char pass[] = "Mamaxedo220701"; //ganti dengan password Anda

int suhu;

// Membuat timer Blynk
//BlynkTimer timer;

// Fungsi untuk membaca data dari sensor DHT dan mengirimkannya ke aplikasi Blynk
//void sendSensor()
//{
  
  // Membaca kelembaban dalam persen
  //float h = dht.readHumidity();
  
  // Memeriksa apakah pembacaan valid
  //if ( isnan(t)) {
    //Serial.println("Gagal membaca dari sensor DHT!");
    //return;
  //}

  // Mengirim data ke aplikasi Blynk
 
  //Blynk.virtualWrite(V6, h); //mengirim kelembaban ke pin virtual V6
//}

const int dcMotor = D4;
int slider;
const int inPut = D5;
int mappedValue=0;

// int trigger = 0;

// float treshold = 38;
// int a = 3;
// float downhold = (treshold - a);


void setup()
{
  // Menginisialisasi komunikasi serial
  Serial.begin(9600);

  // Menginisialisasi sensor DHT
  dht.begin();

  // Menghubungkan ke WiFi
  Blynk.begin(auth, ssid, pass);
  pinMode(dcMotor,OUTPUT);
  pinMode(inPut, OUTPUT);
  digitalWrite(inPut, HIGH);
  

  // Menetapkan fungsi timer untuk mengirim data setiap detik
  //timer.setInterval(1000L, sendSensor);
}

// BLYNK_WRITE(V2) {
//   trigger = param.asInt();
// }

// BLYNK_WRITE(V3){
//   slider = param.asInt(); 
//   mappedValue = map(slider,0,1023,0,255);
//   //generate PWM signal
//   // analogWrite(dcMotor, mappedValue);
  
// }

// BLYNK_WRITE(V4){
//   treshold = param.asInt();
//    downhold = (treshold - a);
// }

BLYNK_WRITE(V0){
  suhu = dht.readTemperature();
  Blynk.virtualWrite(V0, suhu);
 }

void loop(){

  float suhu = dht.readTemperature();
  Blynk.virtualWrite(V0, suhu);

  Blynk.run();
  // Membaca suhu dalam derajat Celcius
  
  

  

  // if ( trigger == 1) {
  //   Serial.print("\n");
  //   Serial.print("Sistem Otomatis Berjalan");
  //   Serial.print("\n");

  //   if( downhold >  suhu ){
  //       digitalWrite(dcMotor, 0);
  //       Blynk.virtualWrite(V3, 0);
  //   }
  //   else if( downhold <= suhu && suhu <= treshold ){
  //       digitalWrite(dcMotor, 180);
  //       Blynk.virtualWrite(V3, 512);
  //   }
  //   else{
        digitalWrite(dcMotor, 255);
        Blynk.virtualWrite(V3, 1023);
  //   }


  // } else {
  //   Serial.print("\n");
  //   Serial.print("Sistem Otomatis Berhenti");
  //   Serial.print("\n");

  //   //Menjalankan Kipas Manual
  //   analogWrite(dcMotor, mappedValue);

    
  // }
  
  // //mengirim suhu ke pin virtual V5
  //   Blynk.virtualWrite(V0, suhu);
  Serial.print(" Suhu : ");
  Serial.print(suhu);
  Serial.print("\n");

  // Menjalankan fungsi Blynk
  
  delay(1000);

    //Google Sheet
  Data_to_Sheets(No_of_Parameters, suhu, density);         /*1. This function accepts multiple float parameter, here No_of_Parameters decides how many parameters you want to send to Google Sheets; 2. The values sent should be in order as per the column in Google Sheets*/
  Serial.println();
  delay(10000);      
  // Menjalankan fungsi timer
  //timer.run();
}
