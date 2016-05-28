#ifndef ddetectionh
#define ddetectionh

#include "opencv2/opencv.hpp"
#include <string>
#include <cmath>

using namespace cv;
using namespace std;

RotatedRect getBiggestRect(vector<RotatedRect> rects) ;

void dispRotatedRectangle(RotatedRect rect, UMat & frame);

vector<vector<Point> > pretraitement(UMat & src, UMat & canny, UMat & contourmat);

UMat getNumberEdgeMat(VideoCapture & cap);

#endif
