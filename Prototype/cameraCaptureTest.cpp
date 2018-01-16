#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

int main(int, char**)
{
    VideoCapture cap(0);
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    if(!cap.isOpened()){
        return -1;}
    for(;;)
    {
          Mat frame;
          cap >> frame; // get a new frame from the camera
          if( frame.empty() ){
      printf("Empty frame");
      break;} // end of video stream
    
    imshow("GO board", frame);
    imwrite("image.png", frame);
    Mat src = imread("image.png", IMREAD_GRAYSCALE);
    imshow("Grayscale img", src);

    //horDist(src);
    //vertDist(src);

          if(waitKey(0) == 27 ){ break;} // press ESC to end
    }
    // the camera will be closed automatically upon exit
    // cap.close();
    return 0;
}
