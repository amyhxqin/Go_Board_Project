  int buttonStart = 7;//red
  int button1 = 4;
  int button2 = 2;
//  int buttonShow = ;

  int LEDStart = 13;//white
  int LED1 = 12;
  int LED2 = 8;
//  int LEDShow = ;

  int buttonStateStart=0; //variable 
  int buttonState1=0; //variable 
  int buttonState2; //variable 

void setup()
{
	pinMode(buttonStart, INPUT);
	pinMode(button1, INPUT);
	pinMode(button2, INPUT);
	pinMode(buttonShow, INPUT);

	pinMode(LEDStart, OUTPUT);
	pinMode(LED1, INPUT);
	pinMode(LED2, INPUT);
	pinMode(LEDShow, INPUT);

	digitalWrite(buttonStart, HIGH);
	digitalWrite(button1, HIGH);
	digitalWrite(button2, HIGH);  
	digitalWrite(buttonShow, HIGH);
}

void loop()
{
buttonStateStart = digitalRead(buttonStart);//
buttonState1 = digitalRead(button1);//
buttonState2 = digitalRead(button2);//

file = fopen("/dev/tty.wchusbserial1420", "w");//opening device file
  
if (ButtonStateStart = HIGH){//button released
  digitalWrite(LEDStart, HIGH)//turn on redlight
  delay(1500);//turn on for 1.5 second
  digitalWrite(LEDStart, LOW)//turn off redlight
fprintf(file,"%s", "start_progarm");  
}

if (ButtonState1 = HIGH){//button released
  digitalWrite(LED1, HIGH)//turn on white light, player 1
  delay(1500);//turn on for 1.5 second
  digitalWrite(LED1, LOW)//turn off white light, player 1
fprintf(file,"%s", "player_1_capture");  
}

if (ButtonState2 = HIGH){//button released
  digitalWrite(LED2, HIGH)//turn on yellow light, player 2
  delay(1500);//turn on for 1.5 second
  digitalWrite(LED2, LOW)//turn on yellow light, player 2
fprintf(file,"%s", "player_2_capture");  
}
fclose(file);
}

//Buttons: input 
  /*bottonStart: start/end the game
    botton1: player1(black)'s turn (take a picture for player 1)
    botton2: player2(whilte)'s turn (take a picture for player 2)
    bottonShow: show the scores for each player
    */
//LEDs: output
  //each LED will turn on when one hits the corresponding buttom

