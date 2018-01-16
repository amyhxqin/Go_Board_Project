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
void loop(){
  
  digitalWrite(LED0, 0);
  digitalWrite(LED1, 0);
  digitalWrite(LED2, 0);


  digitalWrite(LED0, digitalRead(button0));
  digitalWrite(LED1, digitalRead(button1));
  digitalWrite(LED2, digitalRead(button2));

if (digitalRead(button0) == HIGH)//button released
{
  digitalWrite(LED0, 1);//turn on redlight
  Serial.write("LED0 on \n");//return state;
  }
if (digitalRead(button1) == HIGH)//button released
{
  digitalWrite(LED1, 1);//turn on green light
  Serial.write("LED1 on \n");
  }
if (digitalRead(button2) == HIGH)
  {
  digitalWrite(LED2, 1);//turn on yellowlight
  Serial.write("LED2 on \n");//return state;
  }

}



//Buttons: input 
//LEDs: output
//each LED will turn on when one hits the corresponding buttom
