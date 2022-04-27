#include <NewPing.h>

NewPing Sonar1(9, 10, 100);


void setup() 
{
  Serial.begin(9600);
}


int Sonar1Lenght;

void loop() 
{
  delay(10);
  Sonar1Lenght = Sonar1.ping_cm();
  
  Serial.println(Sonar1Lenght);
}
