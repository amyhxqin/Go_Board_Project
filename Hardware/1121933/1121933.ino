/*return state:
1: red button, start
2: capture
3: clear
*/
  int button0 = 9;//red
  int button1 = 10;//green
  int button2 = 11;//yellow

  int LED0 = 3;//red
  int LED1 = 5;//green
  int LED2 = 6;//yellow

  int buttonState0; //variable 
  int buttonState1; //variable 
  int buttonState2; //variable 

void setup()
{
	Serial.begin(9600);
  Serial.println("Program started");
	pinMode(button0, INPUT);
	pinMode(button1, INPUT);
	pinMode(button2, INPUT);

	pinMode(LED0, OUTPUT);
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);

}
// data stored in file = fopen("/dev/tty.wchusbserial1420", "w");
void loop()
{
buttonState0 = digitalRead(button0);//
buttonState1 = digitalRead(button1);//
buttonState2 = digitalRead(button2);//

digitalWrite(LED0, digitalRead(button0));
digitalWrite(LED1, digitalRead(button1));
digitalWrite(LED2, digitalRead(button2));  

if (buttonState0 = HIGH)//button released
{
   digitalWrite(LED0, 1);//turn on redlight
  }
else{
  digitalWrite(LED0, 0);//turn off redlight
  }

if (buttonState1 = HIGH)//button released
{
  digitalWrite(LED1, 1);//turn on green light
  }
else{
  digitalWrite(LED1, 0);//turn off greenlight
  }

if (buttonState2 = LOW)
  {
  digitalWrite(LED2, 1);//turn on yellowlight
  }
  else{
  digitalWrite(LED2, 0);//turn off yellowlight
  }
}
 
void port(){
  if (buttonState0 = HIGH)//button released
{
  Serial.write("s");//return state;
  }
if (buttonState1 = HIGH)//button released
{
  Serial.write("c");
  }
  
if (buttonState2 = HIGH)//button released
{
  Serial.write("e");//return state;
  }
}



//Buttons: input 
//LEDs: output
//each LED will turn on when one hits the corresponding buttom

