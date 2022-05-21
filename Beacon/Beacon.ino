//currently designed for arduino pro mini 328 - 3.3v/8Mhz board

const int ledPin =  LED_BUILTIN;

//based on excitation profile of TSOP34838 (https://www.mouser.com/datasheet/2/427/tsop322-1767012.pdf)
//These parameters change with carrier frequency and are currently calculated for 38 khz
//min pulse length 260 us
//max pulse length (before excessive cooldown): 1.8 ms
//cooldown per pulse 260 us

const long beaconID_us = 1500;
const long pulseRecovery_us = 250;
unsigned long repeatTimer = 0;


//registers determined from atmega 328p doc: (https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)

#define Timer2_On()   TCCR2A = _BV(COM2A0) | _BV(WGM21)
#define Timer2_Off()   TCCR2A = _BV(COM2A1) | _BV(WGM21)

void setup() {
  // put your setup code here, to run once:
  pinMode(11, OUTPUT);

  //cs20: no prescaling (n = 1)
  //com2a0: toggle oc2a (pin 3) on compare match
  //wgm21: ctc waveform generation mode
  TCCR2A = _BV(COM2A0) | _BV(WGM21);
  TCCR2B = _BV(CS20);

  //fclk : 8mhz or 16mhz (processor clock speed)
  OCR2A = 104; //sets frequency based on: f =  fclk / (2 x (n) x (1+ocr2a))
}

void loop() {
  transmit_ID();
}

void transmit_ID()
{
  unsigned long currentMicros = micros();
  if(currentMicros - repeatTimer >= beaconID_us + pulseRecovery_us)
  {
    repeatTimer = currentMicros;
  }
  
  if(currentMicros - repeatTimer < beaconID_us)
  {
    Timer2_On();
  }
  else
  {
    Timer2_Off();
  }
}

void debugMacro1()
{
  Timer2_On();
  digitalWrite(ledPin, HIGH);
  delay(2000);
  Timer2_Off();
  digitalWrite(ledPin, LOW);
  delay(2000);
}
