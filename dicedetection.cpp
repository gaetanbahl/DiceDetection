#include "opencv2/opencv.hpp"
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    VideoCapture cap(0);
    if(!cap.isOpened()) 
        return -1;

    UMat edges;
    namedWindow("edges",1);
    for(;;)
    {
        UMat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        bitwise_not(edges,edges);
        threshold( edges, edges, 220, 255,3);

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        findContours( edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

        string text = to_string(contours.size());

        putText(frame, text, Point(0,80), FONT_HERSHEY_SIMPLEX,3.0, Scalar(0,0,255),
        3);
  

        imshow("edges", frame);
        if(waitKey(30) >= 0) break;
    }

    return 0;
}
