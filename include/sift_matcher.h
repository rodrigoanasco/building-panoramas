#ifndef SIFT_MATCHER_H
#define SIFT_MATCHER_H

#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
using namespace cv;

void SIFT_matcher(const Mat im1, const Mat im2, vector<KeyPoint> kps1, vector<KeyPoint> kps2, const string title){
    Mat gray1, gray2;
    cvtColor(im1, gray1, COLOR_BGR2GRAY);
    cvtColor(im2, gray2, COLOR_BGR2GRAY);

    Ptr<SIFT> sift = SIFT::create();

    Mat d1, d2;
    sift->compute(gray1, kps1, d1);
    sift->compute(gray2, kps2, d2);

    if (d1.empty() || d2.empty()){
        cerr << "[match_with_SIFT_at_keypoints] Empty descriptors; kps1=" << kps1.size()
             << " kps2=" << kps2.size() << endl;
        return;
    }

    Mat vis;
    if (true){
        
    }






}


#endif