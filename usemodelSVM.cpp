#include "opencv2/opencv.hpp"
#include "dicedetection.hpp"
#include "SVMDice.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    VideoCapture cap(0);
    if(!cap.isOpened()) 
        return -1;

    SVMDice model;

    model.loadTrainingData();

    namedWindow("debug",1);
    namedWindow("source",1);
    Mat image ;
    cap >> image;
    Mat number;
    Mat lastgood;
    int res;
    string text = "?";
    for(;;) {
        cap >> image;
        Mat number = getNumberEdgeMat(cap);
        if(number.size().height != 0 
                && number.size().width != 0
               && number.size().width < 300 
                && number.size().height < 300) 
        { 

            lastgood = number.clone();
            int predictednumber = (int) model.recognize(lastgood);
            if(predictednumber > 6)
            	predictednumber -= 6;

            text = to_string((int) predictednumber);
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

