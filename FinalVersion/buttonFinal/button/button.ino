/*return state:
1: red button, start
2: capture
3: clear
*/
//Buttons
int button0 = 9;//red
int button1 = 10;//green
int button2 = 11;//yellow
//LEDs
int LED0 = 3;//red
int LED1 = 5;//green
int LED2 = 6;//yellow
//Flags for the function to run once when a button is pressed
int zeroFlag, oneFlag, twoFlag;
void setup()
{
  Serial.begin(9600);
  Serial.println("Program started");
  //Buttons
  pinMode(button0, INPUT); //Red
  pinMode(button1, INPUT); //Green
  pinMode(button2, INPUT); //Yellow
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}
//data stored in file = fopen("/dev/tty.wchusbserial1420", "w");
void loop(){
  
  digitalWrite(LED0, 0);
  digitalWrite(LED1, 0);
  digitalWrite(LED2, 0);
  digitalWrite(LED0, digitalRead(button0));
  digitalWrite(LED1, digitalRead(button1));
  digitalWrite(LED2, digitalRead(button2));
  //Button 'R' (red) pressed
  if (digitalRead(button0) == HIGH)
  {
    if(!zeroFlag){
      digitalWrite(LED0, 1);//turn on red light
      Serial.write("new\n");//return command for new board
      zeroFlag = 1;
    }
  }
  else{
    zeroFlag = 0;
    }
  //Button 'G' (green) pressed
  if (digitalRead(button1) == HIGH)//button released
  {
    if(!oneFlag){
      digitalWrite(LED1, 1);//turn on green light
      Serial.write("space\n");//return command for new move capture
      oneFlag = 1;
    }
  }
  else{
    oneFlag = 0;
    }
    
  //Button 'Y' (yellow) pressed
  if (digitalRead(button2) == HIGH)//button released
    {
      if(!twoFlag){
        digitalWrite(LED2, 1);//turn on yellow light
        Serial.write("esc\n");//return command to close program
        twoFlag = 1;
      }
    }
  else{
    twoFlag = 0;
    }
}
//Buttons: input 
//LEDs: output
//each LED will turn on when one hits the corresponding button
