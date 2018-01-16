#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/features2d.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace cv;
using namespace std;

//Instructions

//Arduino version
//1. PLace two white pieces at (2,2) and at (3,2). 
//2. Press button 'R' to start a new game (The red lightbulb should light up.). 
//The webcam will capture an image of board and, 
//through findProportion, find the distance in pixels between two holes (casesDistPixels).
//If casesDistPixels is 0, please restart this step. 
//3. Press button 'G' after each new move (The green lightbulb should light up.). 
//The program will output the coordinates and the colors of all pieces on the board. 
//4. Press button 'Y' to close the program (The yellow lightbulb should light up.).

//Computer version (this version)
//Same as in Arduino version, but with key 'n' as 'R', spacebar as 'G', and 'esc' as 'Y'.


//Matrices to store photos
Mat src; Mat src_gray; Mat src_mid; 
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

//Distance in pixels between two holes
double casesDistPixels = 0;

//Position in pixels of the first hole of the board
vector<Point2f> firstHole(1);

//Find the color of the piece (return 0 if not black nor white, return 1 if black, return 2 if white)
int colorDetect(Mat src, Point2f target,int blackX[],int blackY[]){
  Vec3b colour = src.at<Vec3b>(target); //Color at a specific pixel

  if(colour.val[0] >= 160 && colour.val[0] <= 255 
      && colour.val[1]>= 160 && colour.val[1] <= 255
      && colour.val[2]>= 160 && colour.val[2] <= 255){
    return 2;

  }

  else if(colour.val[0] >= 0 && colour.val[0] <= 50 
        && colour.val[1] >= 0 && colour.val[1] <= 60
        && colour.val[2] >= 0 && colour.val[2] <= 50) {
    return 1;
  }

  else{
    return 0;
  }

}

//############################################ SCORING ALGORITHM PART1 BEGIN #####################################
  //Boolean array for the position of the piece
  int position[9][9] = {0};//black is 1

//node(x,y) is [starting node] nodes of the board
//oldcolor is 0/1/2 of the current node
//newcolor is color of the area contour
//may need a function to find the node before scoring

  int node[9][9];//can be created by Point2f

int getOldColor(int x0,int y0,int blackX[],int blackY[]){
  int x=x0;
  int y=y0;
  int color[4];
  for(int i;i<4;i++){
    color[i]=0;
  }

  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      position[blackX[i]][blackY[j]]=1;

    }
  }
  while(x<=9){
    if(position[x][y]!=0){
      color[0]=position[x][y];
      break;
    }
    else x++;
  }
  x=x0;
  while(x>=0){
    if(position[x][y]!=0){
      color[1]=position[x][y];
      break;
    }
    else x--;
  }
  x=x0;
  while(y<=9){
    if(position[x][y]!=0){
      color[2]=position[x][y];
      break;
    }
    else y++;
  }
  y=y0;
  while(y>=0){
    if(position[x][y]!=0){
      color[3]=position[x][y];
      break;
    }
    else y--;
  }
  //if there's a match ie. 1 black 2 white 3 border(then detect more)
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
    if(color[i]==color[j] && color[i]!=3) return color[i];
    else if(color[i]==3 && color[i]==color[j]) return 1;
    }
  }
  return 0;
}

void score(int x,int y,int oldcolor,int newcolor){
  //find the node(empty spots)indirectly record the position of
  //empty spots to be used as starting nodes
    if(node[x][y]==0 || node[x][y]==oldcolor)
    {
      score(x+1,y,oldcolor,newcolor);
      score(x,y+1,oldcolor,newcolor);
      score(x-1,y,oldcolor,newcolor);
      score(x,y-1,oldcolor,newcolor);
    }
}

//############################################ SCORING ALGORITHM PART1 END #####################################

//Find the distance in pixels between two holes
void findProportion(Mat src,int blackX[],int blackY[]){

  //Convert image to gray
  cvtColor( src, src_gray, CV_BGR2GRAY );

  src_mid = src;

  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  //Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );

  //Find contours of shapes
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  vector<Moments> mu_prop(contours.size() );
  for( int i = 0; i < contours.size(); i++ ) { 
  mu_prop[i] = moments( contours[i], false );} 

  vector<Point2f> mc_prop( contours.size() );

  //Markers to detect positions
  vector<Point2f> marker(2);

  for(int i = 0; i<2; ++i){
      marker[i].x = 0;
      marker[i].y = 0;

  }

  int j = 0;
  for (int i = 0; i < contours.size(); ++i)
  {
    if(j == 2){ break;}
    if(mu_prop[i].m00 != 0){ 
      mc_prop[i] = Point2f( round(mu_prop[i].m10/mu_prop[i].m00) , round(mu_prop[i].m01/mu_prop[i].m00) );



        //Detect 2 white pieces in two adjacent holes to find the case length
        //We use white pieces because there is a greater accuracy than using black pieces
        if(colorDetect(src, mc_prop[i],blackX,blackY) == 2 && mc_prop[i].x >= 200){
              marker[j] = mc_prop[i];
              printf("White proportion marker at (%0.f, %0.f)\n", marker[j].x, marker[j].y);
              ++j;
        }
      
    }
  }

  //Find the length in pixels of each case
  casesDistPixels = sqrt(pow(marker[0].x - marker[1].x, 2) + pow(marker[0].y - marker[1].y, 2));


  // Find position of the first hole: we will not place a marker at the edge (where the actual first hole is
  // because it is harder to detect)
  if(marker[0].x < marker[1].x){
      firstHole[0].x = marker[0].x - casesDistPixels;
      firstHole[0].y = marker[0].y - casesDistPixels;
  }
  else{
      firstHole[0].x = marker[1].x - casesDistPixels;
      firstHole[0].y = marker[1].y - casesDistPixels;
  }

  printf("First hole at (%0.f, %0.f)\n", firstHole[0].x, firstHole[0].y);

  printf("casesDistPixels = %0.f; \n", casesDistPixels);

  if (casesDistPixels == 0)
  {
    printf("Failed proportion calculation. Please restart.\n");
  }

//Indicate markers with red points
for(int k = 0; k < 2; k++){
  circle(src_mid, marker[k], 5, Scalar(4,4,255), -1, 8);
}

     imshow("GO board", src_mid);

     //printf("findProportion finished running\n");

}

void findMidpoints(Mat src,int blackX[],int blackY[]){

  //Convert image to gray
  cvtColor( src, src_gray, CV_BGR2GRAY );

  src_mid = src;

  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  // Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );

  // Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Get the moments
  vector<Moments> mu(contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }

  //Get the mass centers:
  vector<Point2f> mc( contours.size() );

  //Find the pixel position of each hole
  Point2f board[9][9];

  for(int i = 0; i < 9; ++i){
    for (int j = 0; j < 9; ++j){
      board[i][j].x = i * casesDistPixels;
      board[i][j].y = j * casesDistPixels; 
    }

  }

  for( int i = 0; i < contours.size(); ++i){
      if(mu[i].m00 != 0){ 
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); //new midpoint

        //If the mass center is either black or white and the piece is on the board
        if(colorDetect(src, mc[i],blackX,blackY) != 0 && mc[i].x >= firstHole[0].x - casesDistPixels
          && mc[i].y >= firstHole[0].y - casesDistPixels){

          printf("piece at pixel coordinates (%0.f, %0.f)\n", mc[i].x, mc[i].y);

        //If the piece is at the right position (on the hole):

        //Distance from first hole for each component of the vector
        double distFromFirstX = fabs(mc[i].x - firstHole[0].x);
        double distFromFirstY = fabs(mc[i].y - firstHole[0].y);

          //Position and color of the piece
          //position = 1 if black and = 2 if white

        for (int k = 0; k < 9; ++k){
          for(int l = 0; l < 9; ++l){
            if (abs(distFromFirstX - board[k][l].x) <= (casesDistPixels/2) 
              && abs(distFromFirstY - board[k][l].y) <= (casesDistPixels/2))
            {
              if(colorDetect(src, mc[i],blackX,blackY) == 1){
                circle(src_mid, mc[i], 5, Scalar(255,255,0), -1, 8); //Indicate with blue point
                position[k][l] = 1;
              }
              else if(colorDetect(src, mc[i],blackX,blackY) == 2){
                circle(src_mid, mc[i], 5, Scalar(255,51,255), -1, 8); //Indicate with pink point
                position[k][l] = 2;
              }
            }
          }
        }
      }

    }
  }

  //Find the score of each player and show the coordinates

  for(int j = 0; j < 9; ++j){

    for(int k = 0; k < 9; ++k){

        if(position[j][k] == 1){
            printf("Black piece at (%d, %d)\n", j+1, k+1);
            blackX[j]=j+1;
            blackY[k]=k+1;
            position[j][k] = 0;
        }

        else if(position[j][k] == 2){
            printf("White piece at (%d, %d)\n", j+1, k+1);
            position[j][k] = 0;
        }
    }
  }

//############################################ SCORING ALGORITHM PART2 BEGIN #####################################s
  int blackScore=0;
  int whiteScore=0;
  int oldcolor;

  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      if(i==0||i==9||j==0||j==9) position[i][j]=3;
      //set border value to 3 
    }
  }

  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
    node[i][j]=-1;
    }
  }

  //find the node(empty spots)indirectly record the position of
  //empty spots to be used as starting nodes
  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
    if(position[i][j]==0) node[i][j]=0;//"board" is the stones on
    //the current board
    }
  }
  //get oldcolor
  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      if(node[i][j]==0) oldcolor=getOldColor(i,j,blackX,blackY);
    }
  }

  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      if(node[i][j]==1) score(i,j,oldcolor,1);
    }
  }

  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      if(node[i][j]==1) blackScore++;
    }
  }


  whiteScore = 81-blackScore;


  //Display score
  printf("Score:\n");
  printf("White: %d\n", whiteScore);
  printf("Black: %d\n", blackScore);
//############################################ SCORING ALGORITHM PART2 END #####################################

  imshow("GO board", src_mid);

  //printf("findMidpoints finished running\n");

}

int main( int argc, char** argv )
{
    int blackX[9],blackY[9];
    for(int i=0;i<9;i++){
      blackX[i]=0;
      blackY[i]=0;
      }
    

  VideoCapture cap(1); //Open secondary camera (1). Default is 0.

  namedWindow("GO board", CV_WINDOW_AUTOSIZE );
  
  //If the camera is empty, stop program.
  if ( !cap.isOpened() )
    {
      cout << "Cannot open the web cam" << endl;
      return -1;
    }

  while(true){

    Mat originalImg;
    Mat modifImg;

    if(waitKey(0) == 110){ //Press 'n'
        cap >> originalImg; 
        if( originalImg.empty() ){
        printf("Empty frame");
        break;} 

        findProportion(originalImg,blackX,blackY);

    }

    if(casesDistPixels != 0 && waitKey(0) == 32){ //Press 'spacebar'
          cap >> originalImg; 
          if( originalImg.empty() ){
      printf("Empty frame");
      break;} 

      findMidpoints(originalImg,blackX,blackY);

    }

    else if (waitKey(0) == 27) { //Press 'esc'
      casesDistPixels = 0;
      cout << "esc key is pressed by user" << endl;
      break; 
   }
  }

  return 0;
}
