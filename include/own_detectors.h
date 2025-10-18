#ifndef OWN_DETECTORS_H
#define OWN_DETECTORS_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

vector<KeyPoint> my_fast_detector(const Mat image){
    Mat input;
    cvtColor(image, input, COLOR_BGR2GRAY);

    float threshold = 0.1;

    return;
}

#endif