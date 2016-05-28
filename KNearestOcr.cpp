/*
 * KNearestOcr.cpp
 *
 * OCR to train and recognize digits with the KNearest model.
 *
 */

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/ml/ml.hpp>

#include <exception>

#include "KNearestOcr.h"

using namespace ml;

KNearestOcr::KNearestOcr() :
        _pModel() {
}

KNearestOcr::~KNearestOcr() {
    if (_pModel) {
        delete _pModel;
    }
}

/**
 * Learn a single digit.
 */
int KNearestOcr::learn(const cv::Mat & img, float n) {
    _responses.push_back(cv::Mat(1, 1, CV_32F, n));
    _samples.push_back(prepareSample(img));

    return (int) n;
}
/**
 * Save training data to file.
 */
void KNearestOcr::saveTrainingData() {
    cv::FileStorage fs("modeldata", cv::FileStorage::WRITE);
    fs << "samples" << _samples;
    fs << "responses" << _responses;
    fs.release();
}

/**
 * Load training data from file and init model.
 */
bool KNearestOcr::loadTrainingData() {
    cv::FileStorage fs("modeldata", cv::FileStorage::READ);
    if (fs.isOpened()) {
        fs["samples"] >> _samples;
        fs["responses"] >> _responses;
        fs.release();

        initModel();
    } else {
        return false;
    }
    return true;
}

/**
 * Recognize a single digit.
 */
char KNearestOcr::recognize(const cv::Mat& img) {
    char cres = '?';
        if (!_pModel) {
            throw std::runtime_error("Model is not initialized");
        }
        cv::Mat results, neighborResponses, dists;
        float result = _pModel->predict(prepareSample(img), results);
    return cres;
}

/**
 * Prepare an image of a digit to work as a sample for the model.
 */
cv::Mat KNearestOcr::prepareSample(const cv::Mat& img) {
    cv::Mat roi, sample;
    cv::resize(img, roi, cv::Size(10, 10));
    roi.reshape(1, 1).convertTo(sample, CV_32F);
    return sample;
}

/**
 * Initialize the model.
 */
void KNearestOcr::initModel() {
    _pModel = ml::KNearest::create();
    _pModel->train(_samples,ROW_SAMPLE, _responses);
}

