#include "opencv2/opencv.hpp"
#include "dicedetection.hpp"
#include "KNearestOcr.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    VideoCapture cap(0);
    if(!cap.isOpened()) 
        return -1;

    KNearestOcr model;

    model.loadTrainingData();

    namedWindow("debug",1);
    namedWindow("source",1);
    Mat image ;
    cap >> image;
    Mat number;
    Mat lastgood;
    float res;
    string text = "?";
    for(;;) {
        cap >> image;
        Mat number = getNumberEdgeMat(cap);
        if(number.size().height != 0 
                && number.size().width != 0 
                && number.size().width < 50 
                && number.size().height < 50) { 

            lastgood = number.clone();
            text = to_string((int) model.recognize(lastgood));
            imshow("debug",lastgood);
        }

        if(waitKey(30) >= 0) {
        }

        putText(image , text, Point(0,80), FONT_HERSHEY_SIMPLEX,3.0, Scalar(0,0,255),
        3);



        imshow("source",image);
    }

return 0;
}

