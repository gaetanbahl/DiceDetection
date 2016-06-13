/*
 * SVMDice.cpp
 *
 * OCR to train and recognize digits with the SVM model.
 *
 */

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/ml/ml.hpp>

#include <exception>

#include "SVMDice.h"

using namespace ml;

SVMDice::SVMDice() :
        _pModel() {
}

SVMDice::~SVMDice() {
    if (_pModel) {
        delete _pModel;
    }
}

/**
 * Learn a single digit.
 */
int SVMDice::learn(const cv::Mat & img, int n) {
    _responses.push_back(cv::Mat(1, 1, CV_32S, n));
    _samples.push_back(prepareSample(img));

    return (int) n;
}
/**
 * Save training data to file.
 */
void SVMDice::saveTrainingData() {
    cv::FileStorage fs("../modeldataSVM", cv::FileStorage::WRITE);
    fs << "samples" << _samples;
    fs << "responses" << _responses;
    fs.release();
}

/**
 * Load training data from file and init model.
 */
bool SVMDice::loadTrainingData() {
    cv::FileStorage fs("../modeldataSVM", cv::FileStorage::READ);
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
int SVMDice::recognize(const cv::Mat& img) {
    char cres = '?';
    if (!_pModel) {
        throw std::runtime_error("Model is not initialized");
    }
    cv::Mat results, neighborResponses, dists;
    cres = _pModel->predict(prepareSample(img));
    return cres;
}

/**
 * Prepare an image of a digit to work as a sample for the model.
 */
cv::Mat SVMDice::prepareSample(const cv::Mat& img) {
    cv::Mat roi, sample;
    cv::resize(img, roi, cv::Size(10, 10));
    roi.reshape(1, 1).convertTo(sample, CV_32F);
    return sample;
}

/**
 * Initialize the model.
 */
void SVMDice::initModel() {
    _pModel = ml::SVM::create();

	_pModel->setType(ml::SVM::C_SVC);
	_pModel->setKernel(ml::SVM::LINEAR);
	_pModel->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));

    _pModel->train(_samples,ROW_SAMPLE, _responses);
}

