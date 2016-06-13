#include "opencv2/opencv.hpp"
#include "dicedetection.hpp"
#include "SVMDice.h"

using namespace cv;
using namespace std;

int n = 4;

int main(int argc, char** argv)
{
    VideoCapture cap(0);
    if(!cap.isOpened()) 
        return -1;

    SVMDice modele;


    namedWindow("debug",1);
    for(int i =1;i <= 12;i++)
    {
        for (int j = 0;j < n;) { 
            cout << "learning " << i << " sample " << j << endl;
            Mat lastgood;
            for(;;) {
                Mat number = getNumberEdgeMat(cap);
                if(number.size().height != 0 
                        && number.size().width != 0 
                        && number.size().width < 300 
                        && number.size().height < 300) { 

                    lastgood = number.clone();
                    imshow("debug",lastgood);
                }
				int k = waitKey(30);
                if(k >= 0) {
                	if (k == 1048586)
                	{
                		cout << "learned " << i << " sample " << j << endl;
                		j++;
                    	modele.learn(lastgood, i);
                    } else if (k == 1048686)
                    {
                    	j = n;
                    	break;
                    }
                }
            }
        }
    }
    modele.saveTrainingData();

    return 0;
}

