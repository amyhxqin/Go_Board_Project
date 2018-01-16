#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/features2d.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//#include "gofunc.h"

using namespace cv;
using namespace std;

int main(int argc,char** agrv)
{
    Mat originalImg;
    Mat modifImg;
    
    VideoCapture cap;
    int camOpen = cap.open(CV_CAP_ANY);
    
    if ( !cap.isOpened() )  // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }
    
    namedWindow("image",CV_WINDOW_FREERATIO);
    while(true){
        if(waitKey(0)==32){
            cap>>originalImg;
            //originalImg = imread("image.png",1);
            namedWindow("image",CV_WINDOW_FREERATIO);
            imshow("image",originalImg);
            waitKey(32);
            
            //Get the White
            Mat OutputImageW;
            inRange(originalImg,Scalar(200,200,200),Scalar(255,255,255),OutputImageW);
            
            //Get the Black
            Mat OutputImageB;
            inRange(originalImg,Scalar(0,0,0),Scalar(50,50,50),OutputImageB);
            
            int w,b;
            w = countNonZero(OutputImageW);
            b = countNonZero(OutputImageB);
            
            if(w>b) cout<<"White"<<"\n";
            else cout<<"Black"<<"\n";
            
            namedWindow("Output",CV_WINDOW_FREERATIO);
            imshow("Output",OutputImageW);
            waitKey(32);
        }
            
            
            
            if(originalImg.empty()){
                printf("Empty frame");
                break;} // end of video stream
                //imwrite("image.png",originalImg);
        
        else if (waitKey(0) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }
    /*originalImg = imread("image.png",1);
    namedWindow("image",CV_WINDOW_FREERATIO);
    imshow("image",originalImg);
    waitKey(32);
    
    //Get the White
    Mat OutputImageW;
    inRange(originalImg,Scalar(220,220,220),Scalar(255,255,255),OutputImageW);
    
    //Get the Black
    Mat OutputImageB;
    inRange(originalImg,Scalar(0,0,0),Scalar(30,30,30),OutputImageB);
    
    int w,b;
    w = countNonZero(OutputImageW);
    b = countNonZero(OutputImageB);
    
    if(w>b) cout<<"Black"<<"\n";
    else cout<<"White"<<"\n";
    
    namedWindow("Output",CV_WINDOW_FREERATIO);
    imshow("Output",OutputImageW);
    waitKey(32);*/
    
    return 0;
}
