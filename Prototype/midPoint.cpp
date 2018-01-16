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

Mat src; Mat src_gray; Mat src_mid;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);


/*void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Get the moments
  vector<Moments> mu(contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }

  ///  Get the mass centers:
  vector<Point2f> mc( contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }


  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
       circle( drawing, mc[i], 4, color, -1, 8, 0 );
     }

  /// Show in a window
  imshow( "GO board", drawing );

  /// Calculate the area with the moments 00 and compare with the result of the OpenCV function
  printf("\t Info: Area and Contour Length \n");
  for( int i = 0; i< contours.size(); i++ )
     {
       printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength( contours[i], true ) );
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
       circle( drawing, mc[i], 4, color, -1, 8, 0 );
     }
}*/

void findMidpoints(Mat src){

/// Convert image to gray
  cvtColor( src, src_gray, CV_BGR2GRAY );

  src_mid = src;
  //createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
  //thresh_callback( 0, 0 );

  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Get the moments
  vector<Moments> mu(contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }

  ///  Get the mass centers:
  vector<Point2f> mc( contours.size() );
  //vector<Point2f> rounded_mc( contours.size() );
  vector<Point2f> mc_new( contours.size() );
	for( int i = 0; i < contours.size(); i++ ){
  		if(mu[i].m00 != 0){ 
    	mc[i] = Point2f( round(mu[i].m10/mu[i].m00) , round(mu[i].m01/mu[i].m00) );
    	/*for(int j = 0; j < i; j++){
    		if(fabs(mc[j].x - mc_new[i].x) >= 0.01 && fabs(mc[j].y - mc_new[i].y) >= 0.01 ){
    			i--;
    			break;

    		}

    	mc[i].x = mc_new[i].x;
    	mc[i].y = mc_new[i].y; //too much time

    	}*/
    	
    //rounded_mc[i] = mc[i]*100/100.0;
		}
    //cout << mc[i] << "\n";
	}

for(int k = 0; k < contours.size(); k++){
	//printf("piece at (%.2f, %.2f)\n", mc[k].x, mc[k].y);
	printf("piece at (%.0f, %.0f)\n", mc[k].x, mc[k].y);
	circle(src_mid, mc[k], 5, Scalar(4,4,255), -1, 8);
}

     imshow("GO board", src_mid);

     printf("findMidpoints finished running\n");

}

/*double horDist(Mat src){

}

double vertDist(Mat src){
	
}

Mat findMoments(Mat src){


}*/

int main( int argc, char** argv )
{
  VideoCapture cap(0); //open the default camera

  //Creation of a window
  namedWindow("GO board", CV_WINDOW_AUTOSIZE );
  
  if ( !cap.isOpened() )  // if not success, exit program
    {
      cout << "Cannot open the web cam" << endl;
      return -1;
    }

	while(true){

    Mat originalImg;
    Mat modifImg;

    if(waitKey(0) == 32){ //press spacebar to take a new frame
        	cap >> originalImg; // get a new frame from the camera
        	if( originalImg.empty() ){
			printf("Empty frame");
			break;} // end of video stream
		
		//imwrite("image.png", originalImg);
		//modifImg = imread("image.png", IMREAD_GRAYSCALE);
    
    //imshow(source_window, originalImg);
		//imshow("GO board", modifImg);

    //Function to calculate midpoints
    findMidpoints(originalImg);


    //Functions to calculate coordinates
		//horDist(modifImg); //horizontal coordinate
		//vertDist(modifImg); //vertical coordinate

  }

      else if (waitKey(0) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
	{
	  cout << "esc key is pressed by user" << endl;
	  break; 
	}
    }

  return 0;
}

