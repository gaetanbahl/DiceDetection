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


bool compareRects(RotatedRect a, RotatedRect b) 
{
    double areaa = 0.0;
    double areab = 0.0;
    Point2f ptsa[4];
    Point2f ptsb[4];
    a.points(ptsa);
    b.points(ptsb);

    areaa = (sqrt(pow(ptsa[0].x - ptsa[1].x,2) + pow(ptsa[0].y - ptsa[1].y,2) ) *
            sqrt(pow(ptsa[1].x - ptsa[2].x,2) + pow(ptsa[1].y - ptsa[2].y,2) ) );

    areab = (sqrt(pow(ptsb[0].x - ptsb[1].x,2) + pow(ptsb[0].y - ptsb[1].y,2) ) *
            sqrt(pow(ptsb[1].x - ptsb[2].x,2) + pow(ptsb[1].y - ptsb[2].y,2) ) );

	return (areaa > areab) ? true : false;
}



void dispRotatedRectangle(RotatedRect rect, Mat & frame)
{
    Point2f vertices[4];
    rect.points(vertices);
    for (int i = 0; i < 4; i++)
        line(frame, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));

}

vector<vector<Point> > pretraitement(Mat & src, Mat & canny, Mat & contourmat)
{
	//imshow("source",src);
    cvtColor(src, canny, COLOR_BGR2GRAY);
    //bitwise_not(canny,canny);
    threshold( canny, canny, 185, 255,THRESH_BINARY);
    GaussianBlur(canny, canny, Size(7,7), 1.5, 1.5);
    Canny(canny,canny, 130,150);

    contourmat = canny.clone();

    vector<vector<Point> > contours;

    findContours( contourmat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    return contours;
}

Mat getNumberEdgeMat(VideoCapture & cap) {
    Mat frame,edges, contourmat;
    Mat rotated, cropped;
    cap >> frame;

    vector<vector<Point> > contours = pretraitement(frame, edges, contourmat);
    imshow("fuu",edges);

    vector<vector<Point> > contours_poly( contours.size() );
    vector<RotatedRect> boundRect( contours.size() );
    //cout << contours.size() << endl;
	if (contours.size() > 0) 
	{
    	for( int i = 0; i < contours.size(); i++ )
    	{ 
        	approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        	boundRect[i] = minAreaRect( Mat(contours_poly[i]) );
        	dispRotatedRectangle(boundRect[i], frame);
    	}

		std::sort(boundRect.begin(), boundRect.end(), compareRects);

		RotatedRect bigrect = boundRect[0];
		dispRotatedRectangle(bigrect, frame);
		imshow("frame", frame);

    	if(bigrect.size.height != 0 && bigrect.size.width != 0) {

        	Mat rot_mat = cv::getRotationMatrix2D(bigrect.center, bigrect.angle, 1);

        	warpAffine(edges, rotated, rot_mat, frame.size(), cv::INTER_CUBIC);
        	getRectSubPix(rotated, bigrect.size, bigrect.center, cropped);

        	if(cropped.size().width > cropped.size().height) {
        		transpose(cropped,cropped);
        		flip(cropped,cropped, 0);
        	}

        	return cropped;
    	} else {
        	return cropped;
    	}
    }
    return cropped;
}

vector<Mat> getNumbers(VideoCapture & cap) {
    Mat frame,edges, contourmat;
    Mat rotated, cropped;
    cap >> frame;
    vector<Mat> output;

    vector<vector<Point> > contours = pretraitement(frame, edges, contourmat);

    vector<vector<Point> > contours_poly( contours.size() );
    vector<RotatedRect> boundRect( contours.size() );
    //cout << contours.size() << endl;
	if (contours.size() > 0) 
	{
    	for( int i = 0; i < contours.size(); i++ )
    	{ 
        	approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        	boundRect[i] = minAreaRect( Mat(contours_poly[i]) );
        	dispRotatedRectangle(boundRect[i], frame);
    	}

		std::sort(boundRect.begin(), boundRect.end(), compareRects);

		for(int i = 0; i < (2 > contours.size() ? contours.size() : 2); i++) 
		{
			RotatedRect bigrect = boundRect[i];
			dispRotatedRectangle(bigrect, frame);
			imshow("frame", frame);

    		if(bigrect.size.height != 0 && bigrect.size.width != 0) {

        		Mat rot_mat = cv::getRotationMatrix2D(bigrect.center, bigrect.angle, 1);

        		warpAffine(edges, rotated, rot_mat, frame.size(), cv::INTER_CUBIC);
        		getRectSubPix(rotated, bigrect.size, bigrect.center, cropped);

        		if(cropped.size().width > cropped.size().height) {
        			transpose(cropped,cropped);
        			flip(cropped,cropped, 0);
        		}

				output.push_back(cropped);	
    		}
    	}
    }
    return output;
}

