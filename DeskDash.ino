#define ECHO_PIN 6
#define TRIGGER_PIN 7
#define LED_PIN 8

void setup() 
{
  Serial.begin(9600);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

float filtered = 300; // 3m

void loop()
{
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  long duration = pulseIn(ECHO_PIN, HIGH);

  // convert the time into a distance
  long cm = microsecondsToCentimeters(duration);
  if (cm <= 500)
  {
    filtered = smooth(cm, 0.9, filtered);
    Serial.print(cm);
    Serial.print("cm, filtered: ");
    Serial.print(filtered);
    Serial.println("cm");
  }
  delay(100);
  
  if (filtered <= 95)
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }
}

long smooth(long data, float filterVal, float smoothedVal)
{
  if (filterVal > 1)
  { 
    // check to make sure param's are within range
    filterVal = .99;
  }
  else if (filterVal <= 0)
  {
    filterVal = 0;
  }
  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);
  return smoothedVal;
}



long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
