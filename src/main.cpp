#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>


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
        img.convertTo(img, CV_32F, 1.0/255.0);
        images.push_back(img);
    }

    for(int i = 0; i < images.size(); i++){
        imshow(paths[i], images[i]);
        waitKey(0);
        destroyAllWindows();
    }
    /* Step 1 ends here */

    
    
    return 0;
}