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

    /* SIFT function is in charge of making descriptions
    what are the descriptions?
    it is just a vector of numbers that sumamrizes how the image looks around that point,
    for example, the directions, the brightness change, and by how much*/
    Mat d1, d2;
    sift->compute(gray1, kps1, d1);
    sift->compute(gray2, kps2, d2);

    if (d1.empty() || d2.empty()){
        cerr << "[match_with_SIFT_at_keypoints] Empty descriptors; kps1=" << kps1.size()
             << " kps2=" << kps2.size() << endl;
        return;
    }

    Mat vis; // Prepare an empty image to draw on it
    
    // We are going to use the KNN + Lowe ratio test to keep only the strongest matches
    
    
    /* This is a brute force matcher, it literally compares each descriptor in image 1 with
    each descriptor of image 2
    
    It measures distances between descriptions (Using Euclidean or L1 distance) to find which
    are the most similar

    So the parameters are NORM_L2 because we are using Euclidean
    and false because we are only matching one way
    */
    BFMatcher matcher(NORM_L2, false);
    
    
    
    /* A DMatch is a small OpenCV object that stores info about one match:
    struct DMatch {
        int queryIdx;   // index of descriptor in image 1
        int trainIdx;   // index of descriptor in image 2
        float distance; // how far (different) they are — smaller = better
    };

    So each DMatch means "descriptor X in image1 matches descriptor Y in image 2
    with distance Z"

    why vector of vectors?
    Because for each descriptor in image 1, we'll get K nearest neighbors in image 2
    Example: if we ask for 2 matches per descriptor, we should get something like:
    knn[0] = {DMatch(best match), DMatch(second best match)}
    knn[1] = {DMatch(best match), DMatch(second best match)}

    */
    vector<vector<DMatch>> knn;
    
    
    /* This line does the actual work:
    d1 = descriptors from image 1 (rows = number of keypoints, columns = feature length).
    d2 = descriptors from image 2.
    knn = where results are stored (that table of vectors).
    2 = K, meaning we want the 2 nearest matches for each descriptor in image 1.
    
    So this fills knn with results like:
    knn = [
        [match1_best, match1_second],
        [match2_best, match2_second],
        ...
    ];


    Now, we make a simple 1D vector to store only the good matches after we filter them, so 
    we will take only one match per descriptor 

    
    */
    matcher.knnMatch(d1, d2, knn, 2);

    vector<DMatch> good;
    for(int i = 0; i < knn.size(); i++){
        vector<DMatch> v = knn[i];

        if (v.size() < 2) continue;
        if (v[0].distance < 0.75 * v[1].distance){
            good.push_back(v[0]);
        }
    }

    




}


#endif