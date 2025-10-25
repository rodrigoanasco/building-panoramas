# 🧭 Feature-Based Panorama Stitching in C++ (OpenCV)

This project implements a complete **image stitching pipeline** using **OpenCV** in C++.  
It reproduces and extends a MATLAB implementation by re-creating the entire process from scratch in C++, from **corner detection** to **panorama blending**.

---

## 🧩 What This Project Does

The program takes two overlapping images and merges them into a panorama using the following custom components:

1. **FAST Detector (custom implementation):**  
   Detects corners efficiently by comparing pixel intensities around a circular neighborhood.

2. **FASTR Detector (FAST + Harris refinement):**  
   Improves the FAST detector by filtering out weak corners using the **Harris corner response**, leading to more stable keypoints.

3. **Harris Corner Detector (from scratch):**  
   - Computes gradients using Sobel filters.  
   - Builds the second-moment matrix.  
   - Applies Gaussian smoothing to stabilize local variations.  
   - Calculates and normalizes the Harris response.

4. **Feature Description (SIFT):**  
   Generates 128-dimensional descriptors for each detected keypoint, capturing local texture and orientation.

5. **Feature Matching (KNN + Lowe’s Ratio Test):**  
   Compares descriptors between two images and keeps only reliable matches to reduce false positives.

6. **RANSAC Homography Estimation:**  
   Uses robust geometric fitting to estimate the best transformation matrix that aligns one image with the other.

7. **Panorama Stitching:**  
   Warps both images into a common reference frame using the estimated homography and blends them seamlessly.

---

## 🧠 Purpose

The main goal of this project was to **learn and apply feature detection and matching techniques in C++**,  
and to understand how each stage of the image stitching pipeline works — from raw intensity gradients to geometric alignment.  

> ✅ The same project was previously implemented in MATLAB, and the C++ version proved more robust and efficient.

---

## 🏗️ Build & Run Instructions (CMake)

### 1️⃣ Configure the project
```bash
cmake -B build
```

### 2️⃣ Build the project
```bash
cmake --build build
```

### 3️⃣ Run the program
```bash
./build/OpenCVExample
```

### 4️⃣ (Optional) Clean build files
```bash
rm -rf build
```

---

## 🗂️ Example Project Structure
```
building_panoramas/
├── CMakeLists.txt
├── src/
│   └── main.cpp
├── include/
│   ├── fast_detector.h
│   ├── fastR_detector.h
│   ├── harris_corner_detector.h
│   └── ransac.h
├── images/
│   ├── S1-im1.png
│   ├── S1-im2.png
│   ├── S2-im1.png
│   └── S2-im2.png
└── build/ ← generated automatically
```

---

## ⚙️ Dependencies
- OpenCV ≥ 4.5  
- CMake ≥ 3.10  
- g++ (C++17 or newer)

---

## 💡 Notes
- FAST/FASTR detectors are implemented manually (no OpenCV feature detectors used).  
- SIFT and RANSAC are used for descriptor extraction and robust homography estimation.  
- The resulting panorama is produced by warping and blending the aligned images.

---

**Author:** Rodrigo Armando Añasco Santillan  
Simon Fraser University  
