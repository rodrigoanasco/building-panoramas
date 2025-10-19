#ifndef OWN_DETECTORS_H
#define OWN_DETECTORS_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;


enum POLARITY {
        POS,
        NONE,
        NEG,
};

POLARITY high_speed_test(Mat image, Point p, float threshold){
    
    int bright = 0;
    int dark = 0;

    /* 
    important note: image.at<float> uses rows, cols = y, x
    because cols grow from left to right like the 'x' parameter
    and the same with rows
    */
    float cenP = image.at<float>(p.y, p.x);
    float up = image.at<float>(p.y - 3, p.x); // up is -3 because rows grow down, so if we want to go up we (-)
    float down = image.at<float>(p.y + 3, p.x); //similar logic here
    float right = image.at<float>(p.y, p.x + 3);
    float left = image.at<float>(p.y, p.x - 3);
    
    if (cenP > up + threshold && 
        cenP > down + threshold && 
        cenP > left + threshold &&
        cenP > right + threshold)
        temp = POS;
    

    
    if (cenP < up - threshold &&
        cenP < down - threshold &&
        cenP < left - threshold && 
        cenP < right - threshold)
        temp = NEG;
    
    if (temp == NONE)
        return false;    
   
    return true; 
};

vector<KeyPoint> my_fast_detector(const Mat image){
    

    /* first get the image and since it's normalized from input, we wont have to do it again*/
    Mat input;
    cvtColor(image, input, COLOR_BGR2GRAY);

    /* now start with the padding */
    copyMakeBorder(input, input, 3, 3, 3, 3, BORDER_REFLECT_101);

    // This will be the threshold to check the points in the image
    float threshold = 0.1;

    // making the offsets for the padding
    // Point comes from cv library, it is just a regular Point structure
    // There is also Point2f (float), Point2d (double)
    // The usage, Point r(1, 2) -> r.x, r.y
    // ok, but why if I populate the points inside the array I have to use {1, 2} and I can't use {(1, 2)}?
    /*
    In C++ (since C++11), curly braces {} are used for something called list initialization (or uniform initialization).
    It’s a general way of constructing objects or assigning values from a list of data.

    int a{5};          // initializes 'a' with 5
    int b = {5};       // same thing

    So: () calls constructors or functions
        {} initializes values or aggregates (list of members)
    */
    Point offsets[] = {{-3, 0}, {-3, 1}, {-2, 2}, {-1, 3},
    { 0, 3}, { 1, 3}, { 2, 2}, { 3, 1},
    { 3, 0}, { 3,-1}, { 2,-2}, { 1,-3},
    { 0,-3}, {-1,-3}, {-2,-2}, {-3,-1}};

    /* 
        Some theory before continuing:
        the pixel p is a corner if there exists a set of n contiguous pixels in the circle (of 16 pixels) 
        which are all brighter than Ip+t, or all darker than Ip−t. n was chosen to be 12.    
    */

    /* 
        High speed test, this test examines the four pixels: 1, 9, 5 and 13.
        First check 1 and 9 if they are both lighter or darker, check 5 and 13
        If p is a corner, then at least three of these must all be brighter than Ip+t or darker than Ip−t. 
        If neither of these is the case, then p cannot be a corner.
    */



    return;
}

#endif