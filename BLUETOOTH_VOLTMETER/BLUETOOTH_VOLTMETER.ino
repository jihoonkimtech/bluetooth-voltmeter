#include <SoftwareSerial.h>
#define R1 30000.0
#define R2 7500.0
#define ANALOG_MAX 1024.0
#define MUITPLE 5.0
#define SENSORPIN A0

volatile int count = 0;
volatile float stack = 0.0;
volatile float send_volt = 0.0;

SoftwareSerial bt(6, 7);

void setup() {
  TCCR0A = 0; //TCCR0A initialize

  TCCR0B = 0; //TCCR0B initialize

  TCNT0 = 0;  //TCNT0 initialize

  OCR0A= 155; //Period: 10ms , Frequency: 100Hz

  TCCR0A |= (1<<WGM01);

  TCCR0B |= (1<<CS02) | (1<<CS00);

  TIMSK0 |= (1<<OCIE0A);

  sei();

  DDRD = B00001100; 
  
  bt.begin(9600);
}

float voltageRead(int pin){
  float analogIn = analogRead(pin);
  float volt1 = (analogIn * MUITPLE) / ANALOG_MAX;
  float volt2 = volt1 / (R2/(R1+R2));
  return volt2;
}

ISR(TIMER0_COMPA_vect)
{
  stack += voltageRead(SENSORPIN);
  if(++count == 500){
    String sendVolt = String(stack / count);
    bt.print(sendVolt);
    count = 0;
  }
}

void loop() {
}
