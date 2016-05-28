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

    KNearestOcr modele;

    modele.loadTrainingData();

    namedWindow("debug",1);
    Mat lastgood;
    for(;;) {
        Mat number = getNumberEdgeMat(cap);
        if(number.size().height != 0 
                && number.size().width != 0 
                && number.size().width < 50 
                && number.size().height < 50) { 

            lastgood = number.clone();
            imshow("debug",lastgood);
        }

        if(waitKey(30) >= 0) {
            cout << modele.recognize(lastgood) << endl;
        }
    }

return 0;
}

