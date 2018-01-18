# Go_Board_Project
##General
Goal: Create a machine vision tool that detects and classifies Go board game pieces through visual input
Language: C++ and Sketch
Tools: OpenCV and Arduino

##Open CV component
This component detects the positions and the colors of the pieces through visual input, classifies the colours into black or white and the pixel positions into the nearest point (intersection), then outputs the results.

1. Position detection: the program analyzes the input (obtained through a real time photo stream in my test cases), finds the contours of the board and of each piece, calculates the distance between adjacent points on the board to build a matrix, measures the pixel positions of the center of each piece, classifies each piece to the nearest point, then ouputs the board coordinates.

2. Color detection: the program detects the colors of the pieces, classifies them in the closest BGR color range (black or white), then outputs the results.

##Arduino Component
This component controls the user interface and is connected to the Open CV component. It allows the user to control the program through buttons (see User Instructions).

##User Instructions
The program has two versions of controls that users can choose based on their resource availability. 

Arduino version
1. Place two white pieces at (2,2) and at (3,2). 
2. Press button 'R' to start a new game (The red lightbulb should light up.). The webcam will capture an image of board and, through findProportion, find the distance in pixels between two points (casesDistPixels). If casesDistPixels is 0, please restart this step. 
3. Press button 'G' after each new move (The green lightbulb should light up.). The program will output the coordinates and the colors of all pieces on the board. 
4. Press button 'Y' to close the program (The yellow lightbulb should light up.).

Computer version
Same as in Arduino version, but with key 'n' as 'R', spacebar as 'G', and 'esc' as 'Y'