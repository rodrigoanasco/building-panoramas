#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main(){
    string imagePath = "../images/aura(messi).jpeg";
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    cv::imshow("Display window", image);
    cv::waitKey(0);

    return 0;
}