//Should work on any arduino (no hardware specific funtionality)

const long pulseLengths_us[] = {1000,1250,1500,1750};

const int ledPin =  LED_BUILTIN;
const int irRecPin =  12;

unsigned long pulseTimer_us = 0;
boolean activePulse = false;

unsigned long debugPrintTimer_ms = 0;

int currentId = -1;
int lastPulseLength_us = 0;

void setup() {
  pinMode(irRecPin,INPUT);
  Serial.begin(9600);
}

void loop() {
  int currentVal = digitalRead(irRecPin); //ir receiver sensor is active low
  if(currentVal == LOW && !activePulse)
  {
    activePulse = true;
    pulseTimer_us = micros();
  }

  if(currentVal == HIGH && activePulse)
  {
    activePulse = false;
    unsigned long pulseLength = micros() - pulseTimer_us;
    int id = findClosestId(pulseLength);
    if(id != -1)
    {
      currentId = id;
      lastPulseLength_us = pulseLength;
    }
  }

  if(millis() - debugPrintTimer_ms > 1000)
  {
    debugPrintTimer_ms = millis();
    Serial.print(lastPulseLength_us);
    Serial.print(" uS, ");
    Serial.println(currentId);
  }
}

int findClosestId(int pulseLength)
{
  long smallestError = 1000000;
  int bestId = -1;
  for(int i = 0;i< sizeof(pulseLengths_us);i++)
  {
    long error = abs(pulseLength - pulseLengths_us[i]);
    if(error < 125 && error<smallestError)
    {
      smallestError = error;
      bestId = i;
    }
  }
  return bestId;
}
