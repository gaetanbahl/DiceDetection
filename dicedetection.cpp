#include "opencv2/opencv.hpp"
#include <string>
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

int main(int argc, char** argv)
{
    VideoCapture cap(0);
    if(!cap.isOpened()) 
        return -1;

    namedWindow("debug",1);
    for(;;)
    {
        UMat frame,edges;
        cap >> frame;
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        bitwise_not(edges,edges);
        threshold( edges, edges, 220, 255,3);
        //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        //Canny(edges,edges, 130,150);

        vector<vector<Point> > contours;

        findContours( edges, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

        vector<vector<Point> > contours_poly( contours.size() );
        vector<RotatedRect> boundRect( contours.size() );

        for( int i = 0; i < contours.size(); i++ )
        { 
            approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
            boundRect[i] = minAreaRect( Mat(contours_poly[i]) );
        }

        RotatedRect bigrect = getBiggestRect(boundRect);
        cv::Mat rot_mat = cv::getRotationMatrix2D(bigrect.center, bigrect.angle, 1);
/*
        for( int i = 0; i< contours.size(); i++ )
        {
            Scalar color = Scalar(255,0,0); 
            drawContours( frame, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
            Point2f vertices[4];
            boundRect[i].points(vertices);
            for (int i = 0; i < 4; i++)
                line(frame, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));
        }
        */
        Mat rotated, cropped;
        warpAffine(frame, rotated, rot_mat, frame.size(), cv::INTER_CUBIC);
        getRectSubPix(rotated, bigrect.size, bigrect.center, cropped);

        dispRotatedRectangle(bigrect, frame);

        if(cropped.size().width > cropped.size().height) {
            double angle = 90.0;
            Point2f src_center(cropped.cols/2.0F, cropped.rows/2.0F);
            rot_mat =getRotationMatrix2D(src_center, angle, 1.0); 
            warpAffine(cropped, cropped, rot_mat, frame.size(), cv::INTER_CUBIC);
        }


       imshow("debug",cropped);


        if(waitKey(30) >= 0) break;
    }

    return 0;
}
