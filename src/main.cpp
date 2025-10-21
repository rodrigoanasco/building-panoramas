#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <own_detectors.h>

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
        vector<KeyPoint> kps = my_fast_detector(images[i]);
        // Now to overlay it, we can simply use the function:
        Mat fast_img;
        // parameters: image, keypoints, output, points_color, if you want to draw the points over the image
        drawKeypoints(images[i], kps, fast_img, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_OVER_OUTIMG);
        
        imshow(paths[i], fast_img);
        
        waitKey(0);
        destroyAllWindows();
    }

    
    
    
    return 0;
}