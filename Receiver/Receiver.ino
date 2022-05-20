

const long pulseLengths_us[] = {750,1000,1250,1500};
const long idRepeat_us = 3000;

const int ledPin =  LED_BUILTIN;
const int irRecPin =  12;

unsigned long pulseTimer_us = 0;
boolean activePulse = false;

unsigned long debugPrintTimer_ms = 0;

int currentId = 0;

void setup() {
  pinMode(irRecPin,INPUT);
  Serial.begin(9600);
}

void loop() {
  int currentVal = digitalRead(irRecPin);
  if(currentVal == HIGH && !activePulse)
  {
    activePulse = true;
    pulseTimer_us = micros();
  }

  if(currentVal == LOW && activePulse)
  {
    activePulse = false;
    unsigned long pulseLength = micros() - pulseTimer_us;
    int id = findClosestId(pulseLength);
    if(id != -1)
    {
      currentId = id;
    }
  }

  if(millis() - debugPrintTimer_ms > 1000)
  {
    debugPrintTimer_ms = millis();
    Serial.println(currentId);
  }
}

int findClosestId(int pulseLength)
{
  long smallestError = idRepeat_us;
  int bestId = -1;
  for(int i = 0;i< sizeof(pulseLengths_us);i++)
  {
    long error = abs(pulseLength - pulseLengths_us[i]);
    if(error<smallestError)
    {
      smallestError = error;
      bestId = i;
    }
  }
  return bestId;
}
