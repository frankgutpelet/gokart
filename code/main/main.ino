int maxValue = 400;
int minValue = 300;

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
}

void loop(void)
{ 
    long sensorVal = 0;
    int trottle = 0;

    sensorVal = analogRead(sensorPin);
    if (sensorVal < minValue)
    {
      sensorVal = minValue;
    }   
    if (sensorVal > maxValue)
    {
      sensorVal = maxValue;
    }
    
    trottle = 255 - ((sensorVal-minValue)*255 / (maxValue - minValue));
    analogWrite(pwmPin, trottle);

} 

