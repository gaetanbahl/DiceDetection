#include "opencv2/opencv.hpp"
#include <cmath>

using namespace cv;
using namespace std;

RotatedRect getBiggestRect(vector<RotatedRect> rects) 
{
    if(rects.size() < 1)
        return RotatedRect();

    double area = 0.0;
    int maxindex = 0;
    double maxarea;
    for(int i =0; i < rects.size(); i++) {
        Point2f pts[4];
        rects[i].points(pts);

        area = (sqrt(pow(pts[0].x - pts[1].x,2) + pow(pts[0].y - pts[1].y,2) ) *
                sqrt(pow(pts[1].x - pts[2].x,2) + pow(pts[1].y - pts[2].y,2) ) );

        if (area > maxarea) {
            maxindex = i;
            maxarea = area;
        }

    }

    return rects[maxindex];
}

void dispRotatedRectangle(RotatedRect rect, UMat & frame)
{
    Point2f vertices[4];
    rect.points(vertices);
    for (int i = 0; i < 4; i++)
        line(frame, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));
       
}

vector<vector<Point> > pretraitement(UMat & src, UMat & canny, UMat & contourmat)
{
    cvtColor(src, canny, COLOR_BGR2GRAY);
    bitwise_not(canny,canny);
    threshold( canny, canny, 220, 255,3);
    //GaussianBlur(canny, canny, Size(7,7), 1.5, 1.5);
    Canny(canny,canny, 130,150);

    contourmat = canny.clone();

    vector<vector<Point> > contours;

    findContours( contourmat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    return contours;
}

UMat getNumberEdgeMat(VideoCapture & cap) {
    UMat frame,edges, contourmat;
    UMat rotated, cropped;
    cap >> frame;

    vector<vector<Point> > contours = pretraitement(frame, edges, contourmat);

    vector<vector<Point> > contours_poly( contours.size() );
    vector<RotatedRect> boundRect( contours.size() );

    for( int i = 0; i < contours.size(); i++ )
    { 
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = minAreaRect( Mat(contours_poly[i]) );
    }

    RotatedRect bigrect = getBiggestRect(boundRect);
    if(bigrect.size.height != 0 && bigrect.size.width != 0) {


        Mat rot_mat = cv::getRotationMatrix2D(bigrect.center, bigrect.angle, 1);

        warpAffine(edges, rotated, rot_mat, frame.size(), cv::INTER_CUBIC);
        getRectSubPix(rotated, bigrect.size, bigrect.center, cropped);

        dispRotatedRectangle(bigrect, frame);

        if(cropped.size().width > cropped.size().height) {
            double angle = 90.0;
            Point2f src_center(cropped.cols/2.0F, cropped.rows/2.0F);
            rot_mat =getRotationMatrix2D(src_center, angle, 1.0); 
            warpAffine(cropped, cropped, rot_mat, frame.size(), cv::INTER_CUBIC);
        }

        return cropped;
    } else {
        return cropped;
    }
}

