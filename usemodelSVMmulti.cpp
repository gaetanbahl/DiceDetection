#include "opencv2/opencv.hpp"
#include "dicedetection.hpp"
#include "SVMDice.h"

using namespace cv;
using namespace std;

int recognizeNum(Mat& number, SVMDice & model)
{
	int predictednumber = 0;
    if(number.size().height != 0 
            && number.size().width != 0
            && number.size().width < 300 
            && number.size().height < 300) 
    { 
        predictednumber = (int) model.recognize(number);
        if(predictednumber > 6)
            predictednumber -= 6;

    }

	return predictednumber;
}


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
        vector<Mat> numbers = getNumbers(cap);
        if(numbers.size() > 1) 
        {
            imshow("debug",numbers[0]);
            imshow("debug2",numbers[1]);
        	int number1 = recognizeNum(numbers[0], model);
        	int number2 = recognizeNum(numbers[1], model);
        	if (number1 < number2)
        	{
        		int c = number1;
        		number1 = number2;
        		number2 = c;
        	}

        	text = to_string(number1) 
        	+ '+' + to_string(number2) 
        	+ "=" + to_string(number1+number2);
        	putText(image , text, Point(0,80), FONT_HERSHEY_SIMPLEX,3.0, Scalar(0,0,255),
        			3);

        }

        if(waitKey(30) >= 0) {
        }

        imshow("source",image);
    }

return 0;
}

