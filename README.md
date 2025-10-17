The idea of this project is to test a bit the OpenCV Library and also try to program it in a new programming language like C++, not MATLAB/Python

First steps:
Learn how to install, run and build OpenCV: https://www.youtube.com/watch?v=CnXUTG9XYGI

How to run it:


# 🧠 Build & Run Instructions (CMake)

## 1️⃣ Configure the project
Generate build files inside a separate folder (`build/`):
```bash
cmake -B build
```
> This scans `CMakeLists.txt`, finds dependencies (like OpenCV), and prepares the build system.

---

## 2️⃣ Build (compile) the project
Compile the source code using the generated configuration:
```bash
cmake --build build
```
> This step runs the compiler (e.g., `g++`) and produces your executable inside `build/`.

---

## 3️⃣ Run the program
Execute your compiled binary:
```bash
./build/OpenCVExample
```
> You can also move the executable elsewhere or rename it if needed.

---

## 4️⃣ (Optional) Clean build files
If you want to start fresh:
```bash
rm -rf build
```
> This removes all generated files — your source code stays untouched.

---

## 🧩 Example of Project Structure
```
building_panoramas/
├── CMakeLists.txt
├── src/
│   └── main.cpp
├── images/
│   └── bb.JPG
└── build/             ← created automatically by CMake
    ├── CMakeCache.txt
    ├── Makefile
    └── OpenCVExample  ← compiled executable
```

---

## 💡 Tip for VS Code Users
To make IntelliSense recognize your includes automatically, enable compile commands:
```bash
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```
Then add this to `.vscode/c_cpp_properties.json` (once):
```json
{
  "configurations": [
    {
      "name": "WSL",
      "includePath": [
        "${workspaceFolder}/**",
        "/usr/local/include/opencv4"
      ],
      "compilerPath": "/usr/bin/g++",
      "cppStandard": "c++17"
    }
  ],
  "version": 4
}
```
