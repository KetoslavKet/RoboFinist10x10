#include "IRremote.h"
#include <NewPing.h>

// Varibles
int ControlSet = 0;
String RemoteCMD = "";


// Pins
NewPing Sonar1(9, 10, 100);

int Pin_IRPort = 8;
IRrecv IRPort(Pin_IRPort);
decode_results IRPortResults;

int Pin_Saw = 6;

int Pin_Motors1 = A0;
int Pin_Motors2 = A1;
int Pin_Motors3 = A2;
int Pin_Motors4 = A3;




void setup() 
{
  Serial.begin(9600);
  
  //pinMode(Pin_SonarTrig, OUTPUT);
  //digitalWrite(Pin_SonarTrig, LOW);
  //pinMode(Pin_SonarEcho, INPUT);

  pinMode(Pin_IRPort, INPUT);
  IRPort.enableIRIn();
  
  pinMode(Pin_Saw, OUTPUT);
  digitalWrite(Pin_Saw, LOW);
  
  pinMode(Pin_Motors1, OUTPUT);
  pinMode(Pin_Motors2, OUTPUT);
  pinMode(Pin_Motors3, OUTPUT);
  pinMode(Pin_Motors4, OUTPUT);
  digitalWrite(Pin_Motors1, LOW);
  digitalWrite(Pin_Motors2, LOW);
  digitalWrite(Pin_Motors3, LOW);
  digitalWrite(Pin_Motors4, LOW);

  Serial.println("Started");
}

void loop() 
{
  // Update IR port
  UpdateRemoteCMD();
  
  //RemoteCMD
  // 0 - None
  // 1 - Switch to Fight
  // Orher -> use after
  // Logica
  // Swich controlsets

  if(RemoteCMD == "16746615")
  {
    ControlSet = 0;
  }
  if(RemoteCMD == "16750695")
  {
    ControlSet = 1;
  }
  if(RemoteCMD == "16734885")
  {
    ControlSet = 2;
  }

  // CONTROL
  if(ControlSet == 0)
  {
    StopAll();
  }
  if(ControlSet == 1)
  {
    RemoteControl();
  }
  if(ControlSet == 2)
  {
    Fight();
  }
}




void UpdateRemoteCMD()
{
  if( IRPort.decode( &IRPortResults ))
  {
    Serial.println( IRPortResults.value );
    RemoteCMD = IRPortResults.value;
    IRPort.resume();
  }
}

void StopAll()
{
  digitalWrite(Pin_Motors1, LOW);
  digitalWrite(Pin_Motors2, LOW);
  digitalWrite(Pin_Motors3, LOW);
  digitalWrite(Pin_Motors4, LOW);
  digitalWrite(Pin_Saw, LOW);
}




void RemoteControl()
{
  
  if(RemoteCMD == "16736415")
  {
    digitalWrite(Pin_Motors1, LOW);
    digitalWrite(Pin_Motors2, HIGH);
    digitalWrite(Pin_Motors3, LOW);
    digitalWrite(Pin_Motors4, HIGH);
  }
  if(RemoteCMD == "16738455")
  {
    digitalWrite(Pin_Motors1, HIGH);
    digitalWrite(Pin_Motors2, LOW);
    digitalWrite(Pin_Motors3, HIGH);
    digitalWrite(Pin_Motors4, LOW);
  }
  if(RemoteCMD == "16767015")
  {
    digitalWrite(Pin_Motors1, HIGH);
    digitalWrite(Pin_Motors2, LOW);
    digitalWrite(Pin_Motors3, LOW);
    digitalWrite(Pin_Motors4, HIGH);
  }
  if(RemoteCMD == "16767525")
  {
    digitalWrite(Pin_Motors1, LOW);
    digitalWrite(Pin_Motors2, HIGH);
    digitalWrite(Pin_Motors3, HIGH);
    digitalWrite(Pin_Motors4, LOW);
  }
  if(RemoteCMD == "16734375")
  {
    StopAll();
  }

  if(RemoteCMD == "16741005")
  {
    digitalWrite(Pin_Saw, HIGH);
  }
  if(RemoteCMD == "16730805")
  {
    digitalWrite(Pin_Saw, LOW);
  }

}


// Settings
int TriggerLenght = 75;
int Sonar1Lenght = 0;
int FoundingFrames1ST = 20;  // 100 Frame = 1 Second
int FoundingFrames2ST = 50;  // 100 Frame = 1 Second

// Varibles
int State = 0;
// 0 - Enemy Not Found
// 1 - Enemy Founded
// 2 - Enemy Lost
// 3 - Enemy Refounding

int RefoundLoop = 0;
bool ExitLoop = false;

void UpdateSonar()
{
  Sonar1Lenght = Sonar1.ping_cm();
  if(Sonar1Lenght == 0)
  {
    Sonar1Lenght = 100;
  }
  Serial.println("St:" + String(State) + "  SL:" + String(Sonar1Lenght) + "  RL:" + String(RefoundLoop));
}

void Fight()
{
  delay(10);
  UpdateSonar();
  
  if(Sonar1Lenght < TriggerLenght)
  {
    State = 1;
    digitalWrite(Pin_Motors1, LOW);
    digitalWrite(Pin_Motors2, HIGH);
    digitalWrite(Pin_Motors3, LOW);
    digitalWrite(Pin_Motors4, HIGH);
  }
  else
  {
    if(State == 0)
    {
      digitalWrite(Pin_Motors1, LOW);
      digitalWrite(Pin_Motors2, HIGH);
      digitalWrite(Pin_Motors3, HIGH);
      digitalWrite(Pin_Motors4, LOW);
    }
    if(State == 1)
    {
      State = 2;
    }
    if(State == 2)
    {
      State = 3;
      RefoundLoop = 0;
    }
    if(State == 3)
    {
      ExitLoop = false;
      while (RefoundLoop < (FoundingFrames1ST + FoundingFrames2ST) && ExitLoop == false)
      {
        if(RefoundLoop < FoundingFrames1ST)
        {
          digitalWrite(Pin_Motors1, LOW);
          digitalWrite(Pin_Motors2, HIGH);
          digitalWrite(Pin_Motors3, HIGH);
          digitalWrite(Pin_Motors4, LOW);
        }
        else
        {
          digitalWrite(Pin_Motors1, HIGH);
          digitalWrite(Pin_Motors2, LOW);
          digitalWrite(Pin_Motors3, LOW);
          digitalWrite(Pin_Motors4, HIGH);
        }

        UpdateSonar();

        if(Sonar1Lenght < TriggerLenght)
        {
           State = 1;
           ExitLoop = true;
        }
        
        delay(10);
        RefoundLoop++;
      }

      
      if(State != 1)
      {
        State = 0;
      }
    }
  }

  if(State != 0)
  {
    digitalWrite(Pin_Saw, HIGH);
  }
  else
  {
    digitalWrite(Pin_Saw, LOW);
  }
}
