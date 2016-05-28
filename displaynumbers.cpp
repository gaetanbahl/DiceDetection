#include "opencv2/opencv.hpp"
#include "dicedetection.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    VideoCapture cap(0);
    if(!cap.isOpened()) 
        return -1;

    namedWindow("debug",1);
    for(;;)
    {
        Mat number = getNumberEdgeMat(cap);
        if(number.size().height != 0 && number.size().width != 0)  
            imshow("debug",number);

        if(waitKey(30) >= 0) break;
    }

    return 0;
}
