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

    Mat Ix, Iy;

    // desired depth = desired data type, anchor point: Point(-1, -1) is "use the center automatically", 
    // BORDER_DEFAULT reflects the border without repeating edge pixels in case there are not enough parameters to do our calculation (like in the left, right corner)
    filter2D(image, Ix, CV_32F, sobelX, Point(-1, -1), 0, BORDER_DEFAULT);
    filter2D(image, Iy, CV_32F, sobelY, Point(-1, -1), 0, BORDER_DEFAULT);

    // Now we have to build second moments (for later in harris corner)
    // And we also build our general gradient matrix Ixy (mul performs dot product)
    Mat Ix2 = Ix.mul(Ix);
    Mat Iy2 = Iy.mul(Iy);
    Mat Ixy = Ix.mul(Iy);

    // We will also apply the a gaussian blur to reduce the noise and make it more stable
    Mat Sx2, Sy2, Sxy;
    GaussianBlur(Ix2, Sx2, Size(5, 5), 1.0, 1.0, BORDER_DEFAULT);
    GaussianBlur(Iy2, Sy2, Size(5, 5), 1.0, 1.0, BORDER_DEFAULT);
    GaussianBlur(Ixy, Sxy, Size(5, 5), 1.0, 1.0, BORDER_DEFAULT);


    // Now we calculate the Harris response
    float k = 0.05;
    
    /* This matrix is the determinant of the Harris matrix,
        Harris matrix is composed by: 
        |S^2_x  S^2_xy|
        |S^2_xy  S^2_y|
        And we already have those components calculated (S is from second moment)

        it is a second-moment matrix (not the same as the hessian) 

        After applying Sobel filters, you get two matrices which are the gradients in components, those are the
        first moment matrices, you ^2 them and that describes how strong are horizontal

        this matrix describes hwo intensity changs around a pixel, not just a pixel but in a local window
        this hessian matrix is always 2x2 
    */
    Mat detM = Sx2.mul(Sy2) - Sxy.mul(Sxy);

    Mat traceM = Sx2 + Sy2;
    //trace is the sum of the squares of the main diagonal
    Mat R = detM - k * traceM.mul(traceM);

    // After computing R, the resulting values in R can vary a lot because these are not bounded, hence we should normalize it
    double rmin, rmax;
    minMaxLoc(R, &rmin, &rmax);
    if (rmax > rmin) {
        /* So what we do here is subtract rmin to everything so that the minimum value is 0, then we
        divide it by rmax - rmin to normalize it */
        R = (R - float(rmin)) / float(rmax - rmin);
    }
    else {
        R.setTo(0);
    }

    return R;
}




#endif