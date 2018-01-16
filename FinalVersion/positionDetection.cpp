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
int colorDetect(Mat src, Point2f target){
  Vec3b colour = src.at<Vec3b>(target); //Color at a specific pixel

  if(colour.val[0] >= 160 && colour.val[0] <= 255 
      && colour.val[1]>= 160 && colour.val[1] <= 255
      && colour.val[2]>= 160 && colour.val[2] <= 255){
    return 2;

  }

  else if(colour.val[0] >= 0 && colour.val[0] <= 80 
        && colour.val[1] >= 0 && colour.val[1] <= 80
        && colour.val[2] >= 0 && colour.val[2] <= 80) {
    return 1;
  }

  else{
    return 0;
  }

}

//Find the distance in pixels between two holes
void findProportion(Mat src){

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
        if(colorDetect(src, mc_prop[i]) == 2 && mc_prop[i].x >= 200){
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

void findMidpoints(Mat src){

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

  //Boolean array for the position of the piece
  int position[9][9] = {0};

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
        if(colorDetect(src, mc[i]) != 0 && mc[i].x >= firstHole[0].x - casesDistPixels
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
              if(colorDetect(src, mc[i]) == 1){
                circle(src_mid, mc[i], 5, Scalar(255,255,0), -1, 8); //Indicate with blue point
                position[k][l] = 1;
              }
              else if(colorDetect(src, mc[i]) == 2){
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
            position[j][k] = 0;
        }

        else if(position[j][k] == 2){
            printf("White piece at (%d, %d)\n", j+1, k+1);
            position[j][k] = 0;
        }
    }
  }


  //Display score (TBA)
  //printf("Score:\n");
  //printf("White: %d\n", whiteScore);
  //printf("Black: %d\n", blackScore);

  imshow("GO board", src_mid);

  //printf("findMidpoints finished running\n");

}

int main( int argc, char** argv )
{

  VideoCapture cap(1);

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

        findProportion(originalImg);

    }

    if(casesDistPixels != 0 && waitKey(0) == 32){ //Press 'spacebar'
          cap >> originalImg; 
          if( originalImg.empty() ){
      printf("Empty frame");
      break;} 

      findMidpoints(originalImg);

    }

    else if (waitKey(0) == 27) { //Press 'esc'
      casesDistPixels = 0;
      cout << "esc key is pressed by user" << endl;
      break; 
   }
  }

  return 0;
}

