#include <EEPROM.h>

#define S1 2
#define S2 3
#define S3 4
#define SENSOR_PIN A0

#define PB_PUTIH 8
#define PB_HITAM 12
#define PB_SIMPAN 11

boolean A[8] = {0,1,0,1,0,1,0,1};
boolean B[8] = {0,0,1,1,0,0,1,1};
boolean C[8] = {0,0,0,0,1,1,1,1};

int nilaiPutih = 0;
int nilaiHitam = 0;
int threshold = 0;

void setup(){
  Serial.begin(9600);

  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  pinMode(S3,OUTPUT);

  pinMode(PB_PUTIH, INPUT_PULLUP);
  pinMode(PB_HITAM, INPUT_PULLUP);
  pinMode(PB_SIMPAN, INPUT_PULLUP);

  EEPROM.get(0, threshold);

  Serial.println("=== MODE KALIBRASI MUX SENSOR ===");
  Serial.print("Threshold terakhir: "); Serial.println(threshold);
}

void loop(){
  int total = 0;

  // Baca rata-rata dari 8 sensor
  for(int i = 0; i < 8; i++){
    digitalWrite(S1, A[i]);
    digitalWrite(S2, B[i]);
    digitalWrite(S3, C[i]);
    delayMicroseconds(200);
    total += analogRead(SENSOR_PIN);
  }
  int rata = total / 8;

  if(digitalRead(PB_PUTIH) == LOW){
    nilaiPutih = rata;
    Serial.print("PUTIH = "); Serial.println(nilaiPutih);
    delay(300);
  }

  if(digitalRead(PB_HITAM) == LOW){
    nilaiHitam = rata;
    Serial.print("HITAM = "); Serial.println(nilaiHitam);
    delay(300);
  }

  if(digitalRead(PB_SIMPAN) == LOW){
    threshold = (nilaiPutih + nilaiHitam) / 2;
    EEPROM.put(0, threshold);

    Serial.println("=== DATA DISIMPAN ===");
    Serial.print("PUTIH = "); Serial.println(nilaiPutih);
    Serial.print("HITAM = "); Serial.println(nilaiHitam); //
    Serial.print("THRESHOLD = "); Serial.println(threshold);
    delay(500);
  }
}