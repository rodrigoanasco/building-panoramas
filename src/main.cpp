#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <fast_detector.h>
#include <fastR_detector.h>

using namespace std;
using namespace cv;

int main(){
    /* Test with Messi image*/
    string imagePath = "../images/aura(messi).jpeg";
    // Mat is a matrix data type
    //IMREAD_COLOR imports in BGR (RBG in different order)
    Mat image = imread(imagePath, IMREAD_COLOR);
    if (image.empty()){
        cerr << "Could not read: " << imagePath << "\n";
    } else {
        imshow("Display window", image);
        waitKey(0);
        destroyAllWindows();
    }
    

    /* Retrieve paths */
    vector<String> paths;

    //"false" recursively means to not keep looking in the same directory over and over again
    //if it doesnt finds it
    glob("../images/S*-im*.png", paths, false);
    

    /* Step 1: Showing all the images */
    // Image imports
    vector<Mat> images;
    images.reserve(paths.size()); //Creates empty spaces
    for(int i = 0; i < paths.size(); i++){
        Mat img = imread(paths[i], IMREAD_COLOR);
        if (img.empty()){
            cerr << "Failed to read path number: " << i << endl; 
            continue;
        }
        images.push_back(img);

        
    }
    for(int i = 0; i < images.size(); i++){
        imshow(paths[i], images[i]);
        
        /* Step 2: FAST feature detector */
        /* since the function is already created in "own_functions.h" just call it*/
        vector<KeyPoint> fast_kps = my_fast_detector(images[i]);
        vector<KeyPoint> fastR_kps = my_fastR_detector(images[i]);
        

        // Now to overlay it, we can simply use the function:
        Mat fast_img, fastR_img;
        // parameters: image, keypoints, output, points_color, if you want to draw the points over the image
        // NOTE: 'DRAW_OVER_OUTIM' was giving me an error because it overwrites the image that was contained already in "fast_img"
        // However, since fast_img is empty rn, it gives an error. Be careful about this (changed it to default that actually creates a new image with the points)
        drawKeypoints(images[i], fast_kps, fast_img, Scalar(0, 255, 0), DrawMatchesFlags::DEFAULT);
        drawKeypoints(images[i], fastR_kps, fastR_img, Scalar(255, 0, 0), DrawMatchesFlags::DEFAULT);
        
        
        //Visualize both side to side:
        Mat left = fast_img, right = fastR_img;

        int height = max(left.rows, right.rows) - 20;
        int wl = left.cols, wr = right.cols;

        if (left.rows != height)  
            resize(left,  left,  Size(wl * height / left.rows,  height));
        if (right.rows != height) 
            resize(right, right, Size(wr * height / right.rows, height));

        putText(left,  "FAST",  {15, 30}, FONT_HERSHEY_SIMPLEX, 1.0, {0,255,0}, 2, LINE_AA);
        putText(right, "FASTR", {15, 30}, FONT_HERSHEY_SIMPLEX, 1.0, {0,255,0}, 2, LINE_AA);
        

        Mat sep(height, 3, left.type(), Scalar(30,30,30));

        Mat sideBySide;
        hconcat(vector<Mat>{left, sep, right}, sideBySide);
        imshow("FAST vs FASTR", sideBySide);
        waitKey(0);

        waitKey(0);
        destroyAllWindows();
    }

    
    
    
    return 0;
}