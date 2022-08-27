#include "FirebaseESP8266.h"  // Firebase ESP8266 library
#include <ESP8266WiFi.h>      // ESP8266 library

//Peklarasi pin dan variabel yang digunakan
#define trig2     D0          // Pin untuk trigger
#define echo2     D1          // Pin untuk echo
#define buzzer    D8          // Pin untuk buzzer
#define relay     D7          // Pin untuk relay
int jml_pengunjung = 0;       // Variabel untuk jumlah pengunjung
long duration2, jarak2;       // Variabel untuk durasi trigger dan jarak

// Pendefinisian untuk menghubungkan data dari nodemcu dengan jaringan internet dan firebase
#define FIREBASE_HOST "gosanitize-69034-default-rtdb.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "qd4ewnbS49h0SPziaVIpYIBkg0oa4ShY5w0bwsev"
#define WIFI_SSID "KELUARGA AA BB"
#define WIFI_PASSWORD "alifbariqayah"

// Pendefinisian FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseJson json;


void setup() {
  Serial.begin (115200);
  pinMode(buzzer, OUTPUT);  
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);

  // Penghubungan antara nodemcu dengan wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  // Penghubungan antara nodemcu dengan wifi dan firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
}

void loop() {
  digitalWrite(trig2, LOW);
  delayMicroseconds(2); 
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trig2, LOW);
  duration2 = pulseIn(echo2, HIGH);
  jarak2 = duration2*0.034/2;
  if(jarak2<=10){
    digitalWrite(buzzer, HIGH);
    digitalWrite(relay, LOW);// menonaktifkan RELAY dalam artian melakukan aktivasi pada Motor DC 5V
    delay(1000);
    digitalWrite(relay,HIGH);
    digitalWrite(buzzer,LOW);
    jml_pengunjung =1+jml_pengunjung;
  }
  else{
    digitalWrite(buzzer, LOW);
    digitalWrite(relay, HIGH);// mengaktifkan RELAY dalam artian melakukan deaktivasi pada Motor DC 5V
  }

  // Menampilkan jarak dan jumlah pengunjung pada serial monitor  
  Serial.print("jarak :");
  Serial.print(jarak2);
  Serial.println(" cm");
  Serial.println("    ");
  Serial.print("Jumlah Pengunjung : ");
  Serial.print(jml_pengunjung);
  Serial.println( " orang");
  delay(500);

  // Pengiriman data dari nodemcu ke firebase
  if (Firebase.setFloat(firebaseData, "/Gosanitize/jumlah_pengunjung", jml_pengunjung))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}
