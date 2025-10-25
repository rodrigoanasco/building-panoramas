#ifndef RANSAC_H
#define RANSAC_H

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <vector>
#include <algorithm>
#include <cmath>

#include <fast_detector.h>
#include <fastR_detector.h>

using namespace cv;
using namespace std;

/* 
RANSAC: random sample consensus
it's a robust algorithm that finds the best mathematical model (like a line, plane, etc.) that fits your data,
even when some points are wrong

In simpler words: RANSAC separate the good data (inliers) from the bad data (outliers) and fits a model only
to the good ones

Why do we need it in panoramas?
When you match keypoints between two images (like with SIFT/SURF/FastR), some matches are wrong.
For example, two similar textures might match even though they’re not the same real-world point.
If you try to compute a homography (the transformation that maps one image onto the other) using all matches,
the bad matches will distort the result badly. This will affect your panorama will bend or twist.

So RANSAC is used to find the transformation (homography) that is most consistent with the majority of the good matches.
*/

// Struct for the RANSAC
struct RansacParameters{
    double confidence = 0.995; // the condifence
    int maxIters = 2000; // maximum number of trials
    double maxDistance = 4.0; // reprojection threshold in px
    float ratio = 0.85;
};

void ensureGray(const Mat& src, Mat& gray) {
    if (src.channels() == 3) cvtColor(src, gray, COLOR_BGR2GRAY);
    else                     gray = src.clone();
}

void siftDescriptorsAt(const Mat& gray, vector<KeyPoint>& kps, Mat& desc) {
    Ptr<SIFT> sift = SIFT::create();
    sift->compute(gray, kps, desc);
}



vector<DMatch> knnRatioMatch(const Mat &d1, const Mat &d2, float ratio = 0.8){
    BFMatcher matcher(NORM_L2, false);
    vector<vector<DMatch>> knn;
    matcher.knnMatch(d1, d2, knn, 2);

    vector<DMatch> good;
    good.reserve(knn.size());
    for (int i = 0; i < knn.size(); i++){
        vector<DMatch> v = knn[i];
        if (v.size() < 2)
            continue;
        if (v[0].distance < ratio * v[1].distance)
            good.push_back(v[0]);
    }

    return good;
}


/* Estimate homography with RANSAC (keep inliers) */
Mat estimateHomographyRANSAC(const vector<KeyPoint>& kA,
                                    const vector<KeyPoint>& kB,
                                    const vector<DMatch>& matches,
                                    const RansacParameters& P,
                                    vector<char>& inlierMask)
{
    vector<Point2f> pA, pB;
    pA.reserve(matches.size());
    pB.reserve(matches.size());

    for (const auto& m : matches) {
        pA.push_back(kA[m.queryIdx].pt); // image A points
        pB.push_back(kB[m.trainIdx].pt); // image B points
    }

    // findHomography(src, dst, ...) maps src->dst;
    // we want H that maps B -> A (so src = pB, dst = pA)
    Mat H = findHomography(pB, pA, RANSAC, P.maxDistance, inlierMask, P.maxIters, P.confidence);
    return H;
}

/* both images into a common canvas and max-blend */
Mat warpAndBlendPanorama(const Mat& imgA, const Mat& imgB, const Mat& H_BtoA)
{
    // B corners -> A space
    vector<Point2f> cornersB = {
        {0,0}, {float(imgB.cols),0},
        {float(imgB.cols),float(imgB.rows)}, {0,float(imgB.rows)}
    };
    vector<Point2f> xB;
    perspectiveTransform(cornersB, xB, H_BtoA);

    // A corners in A space
    vector<Point2f> cornersA = {
        {0,0}, {float(imgA.cols),0},
        {float(imgA.cols),float(imgA.rows)}, {0,float(imgA.rows)}
    };

    // bounds
    float minX = 0, minY = 0, maxX = float(imgA.cols), maxY = float(imgA.rows);
    for (auto& p : xB) {
        minX = min(minX, p.x); minY = min(minY, p.y);
        maxX = max(maxX, p.x); maxY = max(maxY, p.y);
    }
    for (auto& p : cornersA) {
        minX = min(minX, p.x); minY = min(minY, p.y);
        maxX = max(maxX, p.x); maxY = max(maxY, p.y);
    }

    // translate so everything is positive
    Mat T = (Mat_<double>(3,3) << 1,0,-minX,  0,1,-minY,  0,0,1);

    // canvas size
    int W = int(ceil(maxX - minX));
    int H = int(ceil(maxY - minY));
    Size panoSize(W, H);

    // warp B with H and translation
    Mat warpedB;
    warpPerspective(imgB, warpedB, T * H_BtoA, panoSize);

    // warp A with identity + translation
    Mat warpedA;
    warpPerspective(imgA, warpedA, T, panoSize);

    // simple max blend (per-pixel)
    Mat panorama;
    max(warpedA, warpedB, panorama);
    return panorama;
}

/* Build a panorama using FAST keypoints */
Mat panorama_FAST(const Mat& imgA, const Mat& imgB, const RansacParameters& P = {})
{
    // 1) detect (your FAST)
    vector<KeyPoint> kpA = my_fast_detector(imgA);
    vector<KeyPoint> kpB = my_fast_detector(imgB);

    // 2) describe at those KPs (SIFT)
    Mat gA, gB; 
    ensureGray(imgA, gA); 
    ensureGray(imgB, gB);
    Mat dA, dB;

    siftDescriptorsAt(gA, kpA, dA);
    siftDescriptorsAt(gB, kpB, dB);
    
    if (dA.empty() || dB.empty()) return Mat();

    // 3) match
    vector<DMatch> good = knnRatioMatch(dA, dB, P.ratio);
    if (good.size() < 8) return Mat(); // need enough for a homography

    // 4) RANSAC homography
    vector<char> inliers;
    Mat H_BtoA = estimateHomographyRANSAC(kpA, kpB, good, P, inliers);
    if (H_BtoA.empty()) return Mat();

    // 5) warp & blend
    return warpAndBlendPanorama(imgA, imgB, H_BtoA);
}


Mat panorama_FASTR(const Mat& imgA, const Mat& imgB, const RansacParameters& P = {})
{
    // 1) detect (your FASTR)
    vector<KeyPoint> kpA = my_fastR_detector(imgA);
    vector<KeyPoint> kpB = my_fastR_detector(imgB);

    // 2) describe (SIFT)
    Mat gA, gB; ensureGray(imgA, gA); ensureGray(imgB, gB);
    Mat dA, dB;
    siftDescriptorsAt(gA, kpA, dA);
    siftDescriptorsAt(gB, kpB, dB);
    if (dA.empty() || dB.empty()) return Mat();

    // 3) match
    vector<DMatch> good = knnRatioMatch(dA, dB, P.ratio);
    if (good.size() < 7) return Mat();

    // 4) RANSAC homography
    vector<char> inliers;
    Mat H_BtoA = estimateHomographyRANSAC(kpA, kpB, good, P, inliers);
    if (H_BtoA.empty()) return Mat();

    // 5) warp & blend
    return warpAndBlendPanorama(imgA, imgB, H_BtoA);
}

#endif