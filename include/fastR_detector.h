#ifndef FASTR_DETECTOR_H
#define FASTR_DETECTOR_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "harris_corner_detector.h"
#include "fast_detector.h"


using namespace cv;
using namespace std;


// Essentially here I have to: calculate the fast first, then calculate the harris corner and compare if the
// keypoints of the fast detector have also a high harris corner to have more strong corners!


vector<KeyPoint> my_fastR_detector(const Mat input){

    // image holds the actual image, H holds the harris matrix
    Mat gray;
    if (input.channels() == 3) 
        cvtColor(input, gray, COLOR_BGR2GRAY);
    else gray = input;
    
    Mat image, H;
    if (gray.type() != CV_32F) gray.convertTo(image, CV_32F, 1.0/255.0);
    else image = gray;

    vector<KeyPoint> temp = my_fast_detector(input);

    H = my_harris_corner_detector(image);

    vector<KeyPoint> result;
    float threshold = 0.35;
    for (int i = 0; i < temp.size(); i++){
        int x = cvRound(temp[i].pt.x);
        int y = cvRound(temp[i].pt.y);
        if (H.at<float>(y, x) > threshold){
            result.push_back(temp[i]);
        }
    }
    
    return result;
}






#endif