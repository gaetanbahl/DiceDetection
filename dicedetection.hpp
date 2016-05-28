#ifndef ddetectionh
#define ddetectionh

#include "opencv2/opencv.hpp"
#include <string>
#include <cmath>

using namespace cv;
using namespace std;

RotatedRect getBiggestRect(vector<RotatedRect> rects) ;

void dispRotatedRectangle(RotatedRect rect, Mat & frame);

vector<vector<Point> > pretraitement(Mat & src, Mat & canny, Mat & contourmat);

Mat getNumberEdgeMat(VideoCapture & cap);

#endif
