#include "FPS_GT511C3.h"
//#include "SoftwareSerial.h"

// Hardware setup - FPS connected to:
//	  digital pin 4(arduino rx, fps tx)
//	  digital pin 5(arduino tx - 560ohm resistor - fps tx - 1000ohm resistor - ground)
//		this voltage divider brings the 5v tx line down to about 3.2v so we dont fry our fps

int RX = 4;
int TX = 3;
int magnet_mode = 9;
int magnet_state = 7;
int buzzer_pin = 8;
int buzDelay = 25;

// Function defenitions
void magnet_on();
void magnet_off();
void enableLogic();
void disableLogic();
bool IsPressButton();
void LongBeep();
void ShortBeep();

void setup()
{
  pinMode(magnet_mode, INPUT);
  pinMode(magnet_state, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  
	Serial.begin(9600);
  delay(200);
  
  disableLogic();
}

void loop()
{ 
    delay(50);
    if( IsPressButton() )
      enableLogic();
}

 void enableLogic()
 {
	FPS_GT511C3 fps(RX, TX);
 
    fps.Open();
    fps.SetLED(true);
    magnet_on();
    
    Serial.println("Please press finger");

    while(true)
    {
        delay(50);
    	  
    	  // Identify fingerprint test
        if (fps.IsPressFinger())
      	{
      	    fps.CaptureFinger(false);
      		  int id = fps.Identify1_N();
      		  if (id <200)
      		  {
      		      Serial.print("Verified ID:");
      			    Serial.println(id);
                LongBeep();
                disableLogic();
                break;
      		  }
           else
           {
              ShortBeep();
           }
      	}

        // check if button press
        if( IsPressButton() == true )
        {
            disableLogic();
            break;
        }
        
   	}
    fps.SetLED(false);
    fps.Close();
}

void disableLogic()
{
  magnet_off();
  Serial.println("Logic disabled.");
}

void magnet_on()
{
  digitalWrite(magnet_state,HIGH);
}

void magnet_off()
{
  digitalWrite(magnet_state,LOW);
}

bool IsPressButton()
{
  int val = digitalRead(magnet_mode);
  //Serial.println("VAL1");
  //Serial.println(val);
  if(val > 0)
  {
    //while(val > 0)
    val = digitalRead(magnet_mode);
    //Serial.println("VAL2");
    //Serial.println(val);
    return true;
    }
       
  return false;
}

void LongBeep()
{
  digitalWrite(buzzer_pin,HIGH);
  delay(1000);
  digitalWrite(buzzer_pin,LOW);
}


  void ShortBeep()
{
  digitalWrite(buzzer_pin,HIGH);
  delay(buzDelay);
  digitalWrite(buzzer_pin,LOW);
  delay(buzDelay);
  digitalWrite(buzzer_pin,HIGH);
  delay(buzDelay);
  digitalWrite(buzzer_pin,LOW);
}
