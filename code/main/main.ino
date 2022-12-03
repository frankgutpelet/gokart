int maxValue;
int minValue;

int pwmPin = 16;
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;

void calibrate()
{
 do
  {
     maxValue = sensorValue;
     sensorValue = analogRead(sensorPin);
     delay(100);
  }while ((maxValue - 20) < sensorValue);

  Serial.println(String("Kalibriert: min: ") + minValue + ", max: " + maxValue);
}


void setup(void) 
{
  pinMode(pwmPin, OUTPUT);
  analogWrite(pwmPin, 0);
  Serial.begin(115200);
  calibrate();
  
}

void loop(void)
{ 
    long sensorVal = 0;
    int trottle = 0;
    int counter = 0;
    while(1)
    {
      counter++;
      sensorVal = analogRead(sensorPin);
      sensorVal = ((sensorVal - minValue)<0)?0:sensorVal; 
      
      trottle = sensorVal*255 / (maxValue - minValue);
      if (255 < trottle)
      {
        trottle = 255;
      }
      analogWrite(pwmPin, trottle);

      if (100 <= counter)
      {
        counter = 0;
        Serial.println(String("SensorVal: ") + sensorVal + ", trottle: " + trottle);
      }
    }
} 
