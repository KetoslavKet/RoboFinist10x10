#include <NewPing.h>

NewPing Sonar1(9, 10, 100);


void setup() 
{
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
}


int Sonar1Lenght;

void loop() 
{
  delay(10);
  Sonar1Lenght = Sonar1.ping();
  Serial.println(Sonar1Lenght);

  
}
