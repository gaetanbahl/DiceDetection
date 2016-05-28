#include "opencv2/opencv.hpp"
#include <string>

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
    VideoCapture cap(0);
    if(!cap.isOpened()) 
        return -1;

    namedWindow("source",1);
    namedWindow("debug",1);
    for(;;)
    {
        UMat frame,edges;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        bitwise_not(edges,edges);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        threshold( edges, edges, 220, 255,3);

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        findContours( edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

        string text = to_string(contours.size());

        putText(frame, text, Point(0,80), FONT_HERSHEY_SIMPLEX,3.0, Scalar(0,0,255),
        3);

        Scalar color = Scalar(0,255,0);
        for (int i = 0; i < contours.size(); i++) {
            drawContours( frame, contours, i, color, 2, 8, hierarchy, 0, Point() );
        }

        imshow("source", frame);
        imshow("debug", edges);
        if(waitKey(30) >= 0) break;
    }

    return 0;
}
