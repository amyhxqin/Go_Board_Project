#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
  VideoCapture cap(0); //capture the video from web cam

  if ( !cap.isOpened() )  // if not success, exit program
    {
      cout << "Cannot open the web cam" << endl;
      return -1;
    }

  namedWindow("Control",CV_WINDOW_AUTOSIZE); //create a window called "Control"
  //use B for Black and W for White
  int iLowHB = 0;
  int iHighHB = 128;

  int iLowSB = 0; 
  int iHighSB = 128;

  int iLowVB = 0;
  int iHighVB = 128;

  int iLowHW = 250;
  int iHighHW = 255;

  int iLowSW = 250; 
  int iHighSW = 255;

  int iLowVW = 250;
  int iHighVW = 255;

  //Create trackbars in "Control" window for Black
  cvCreateTrackbar("LowHB", "Control", &iLowHB, 128); //Hue (0 - 128)
  cvCreateTrackbar("HighHB", "Control", &iHighHB, 128);

  cvCreateTrackbar("LowSB", "Control", &iLowSB, 255); //Saturation (0 - 255)
  cvCreateTrackbar("HighSB", "Control", &iHighSB, 255);

  cvCreateTrackbar("LowVB", "Control", &iLowVB, 255);//Value (0 - 255)
  cvCreateTrackbar("HighVB", "Control", &iHighVB, 255);

  //Create trackbars in "Control" window for White
  cvCreateTrackbar("LowSW", "Control", &iLowSW, 255); //Saturation (0 - 255)
  cvCreateTrackbar("HighSW", "Control", &iHighSW, 255);

  cvCreateTrackbar("LowSB", "Control", &iLowSW, 255); //Saturation (0 - 255)
  cvCreateTrackbar("HighSB", "Control", &iHighSW, 255);

  cvCreateTrackbar("LowVB", "Control", &iLowVW, 255);//Value (0 - 255)
  cvCreateTrackbar("HighVB", "Control", &iHighVW, 255);

  while (true)
    {
      Mat imgOriginal;

      bool bSuccess = cap.read(imgOriginal); // read a new frame from video

      if (!bSuccess) //if not success, break loop
        {
	  cout << "Cannot read a frame from video stream" << endl;
	  break;
        }

      Mat imgHSV;

      cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
      
      /*Mat imgThresholdedB, imgThresholdedW;
	//Black
	inRange(imgHSVB, Scalar(iLowHB, iLowSB, iLowVB), Scalar(iHighHB, iHighSB, iHighVB), imgThresholdedB); //Threshold the image
	//White
	inRange(imgHSV, Scalar(iLowHW, iLowSW, iLowVW), Scalar(iHighHW, iHighSW, iHighVW), imgThresholdedW);
	/morphological opening (removes small objects from the foreground)
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

	//morphological closing (removes small holes from the foreground)
	dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

	imshow("Thresholded Image", imgThresholded); //show the thresholded image*/
      imshow("Original", imgOriginal); //show the original image

      if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
	{
	  cout << "esc key is pressed by user" << endl;
	  break; 
	}
    }

  return 0;
}
