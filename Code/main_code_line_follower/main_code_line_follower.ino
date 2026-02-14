#include <EEPROM.h>

// ================== MULTIPLEXER SENSOR ==================
#define S1 2
#define S2 3
#define S3 4
#define SENSOR_PIN A0

boolean A[8] = {0,1,0,1,0,1,0,1};
boolean B[8] = {0,0,1,1,0,0,1,1};
boolean C[8] = {0,0,0,0,1,1,1,1};

int threshold = 0;

// ================== MOTOR ==================
#define pin_pwm_motor_L 6
#define pin_dir_motor_L 9
#define pin_pwm_motor_R 5
#define pin_dir_motor_R 10

void setMotor(int L, int R){
  digitalWrite(pin_dir_motor_L, (L >= 0) ? HIGH : LOW );
  analogWrite(pin_pwm_motor_L, abs(L));

  digitalWrite(pin_dir_motor_R, (R >= 0) ? HIGH : LOW );
  analogWrite(pin_pwm_motor_R, abs(R));
}

// ================== PID ==================
float Kp = 22;
float Ki = 0;
float Kd = 12;

float lastError = 0;
float sumError = 0;

void setup(){
  Serial.begin(9600);

  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  pinMode(S3,OUTPUT);

  pinMode(pin_pwm_motor_L,OUTPUT);
  pinMode(pin_dir_motor_L,OUTPUT);
  pinMode(pin_pwm_motor_R,OUTPUT);
  pinMode(pin_dir_motor_R,OUTPUT);

  EEPROM.get(0, threshold);
  Serial.print("Threshold Loaded: "); Serial.println(threshold);
}

void loop(){
  byte s = read_sensor();
  int error = getError(s);

  float P = error * Kp;
  sumError += error;
  float I = sumError * Ki;
  float D = (error - lastError) * Kd;
  lastError = error;

  float PID = P + I + D;

  int base = 120;
  setMotor(base - PID, base + PID);
}

// ============ BACA SENSOR =============
byte read_sensor(){
  byte out = 0;
  for(int i = 0; i < 8; i++){
    digitalWrite(S1, A[i]);
    digitalWrite(S2, B[i]);
    digitalWrite(S3, C[i]);
    delayMicroseconds(200);
    if(analogRead(SENSOR_PIN) > threshold) out |= (1 << (7 - i));
  }
  return out;
}

// ============ ERROR SENSOR =============
int getError(byte s){
  switch(s){
    case 0b00011000: return 0;
    case 0b00011100: return -1;
    case 0b00111000: return 1;
    case 0b00001100: return -2;
    case 0b00110000: return 2;
    case 0b00001110: return -3;
    case 0b01110000: return 3;
    case 0b00000110: return -4;
    case 0b01100000: return 4;
    case 0b00000010: return -5;
    case 0b01000000: return 5;
    case 0b00000001: return -6;
    case 0b10000000: return 6;
    default: return 0;
  }
}