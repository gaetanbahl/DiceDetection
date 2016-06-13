/*
 * SVMDice.h
 *
 */

#ifndef SVMDICE_H_
#define SVMDICE_H_

#include <vector>
#include <list>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;

class SVMDice {
public:
    SVMDice();
    virtual ~SVMDice();

    int learn(const cv::Mat & img, int n);
    int learn(const std::vector<cv::Mat> & images);
    void saveTrainingData();
    bool loadTrainingData();

    int recognize(const cv::Mat & img);
    std::string recognize(const std::vector<cv::Mat> & images);

private:
    cv::Mat prepareSample(const cv::Mat & img);
    void initModel();

cv::Mat _samples;
    cv::Mat _responses;
    Ptr<ml::SVM> _pModel;
};

#endif /* SVMDICE_H_ */
