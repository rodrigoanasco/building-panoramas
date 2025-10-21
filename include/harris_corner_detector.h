#ifndef HARRIS_CORNER_DETECTOR_H
#define HARRIS_CORNER_DETECTOR_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

/* The code idea of this implementation is:
- Compute the gradients x and y of the matrix
- Smooth the second moments with a Gaussian
- Compute the harris score
- Keep the local maxima of R above the threshold
*/
Mat my_harris_corner_detector(Mat input){
    /* To compute the gradient, I will use the sobel kernel */
    //Start creating the sobel Kernels
    
    Mat image = input;
    image.convertTo(image, CV_32F, 1.0/255.0);

    // open cv implemented the oveload operator that is why it works here
    Mat sobelX = (Mat_<float>(3, 3) <<
            -1, 0, 1,
            -2, 0, 2,
            -1, 0, 1);

    Mat sobelY = sobelX.t();

    

}




#endif