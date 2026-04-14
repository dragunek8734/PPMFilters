# PPMFilters

Basic image operations and filters for improving PPM image quality (with an interactive OpenCV preview).

This project loads **PPM images (P3 and P6)** into a simple custom `PPM`/`Pixel` structure, applies common spatial filters and per-pixel operations, and displays results using **OpenCV**.

---

## Features

### Filters
- **Average (box) filter** (e.g. 3×3)
- **Median filter** (e.g. 3×3)
- **Gaussian blur** (sigma-based, separable 1D kernel)
- **Sobel edge detection** (grayscale + gradient magnitude)
- **Unsharp mask** (sharpening with configurable amount)
- **User-defined custom convolution filter** (enter kernel values interactively)

### Pixel operations
- Add constant to all pixels
- Subtract constant from all pixels
- Multiply pixels by a factor
- Divide pixels by a factor
- Grayscale conversion:
  - **ITU-R** weighted grayscale (`0.299R + 0.587G + 0.114B`)
  - Mean grayscale

### Image I/O
- Loads:
  - `P3` (ASCII) PPM with comment skipping (`# ...`)
  - `P6` (binary) PPM with comment skipping (`# ...`)
- Note: the app expects a **PPM file path** from the user at runtime.

---

## Project structure

- `main.cpp`  
  Interactive CLI menu + OpenCV windows (`Original Image` + filtered output).

- `utils.h` / `utils.cpp`  
  `Pixel` and `PPM` classes + PPM parsing (`PPM::LoadFromFile`).

- `filters.cpp`  
  Spatial filters (avg/median/gaussian/sobel/unsharp/user-defined kernel).

- `operations.cpp`  
  Per-pixel arithmetic operations and grayscale helpers.

- `CMakeLists.txt`  
  CMake build config (C++17) + OpenCV discovery/linking.

---

## Requirements

- CMake **>= 3.10**
- A C++17 compiler (MSVC / clang / gcc)
- OpenCV installed (the project uses `find_package(OpenCV REQUIRED)`)

---

## Build (CMake)

### Windows note about OpenCV_DIR
`CMakeLists.txt` currently sets:

```cmake
set(OpenCV_DIR "C:/opencv/build")
```

If your OpenCV is installed elsewhere, update that path or pass `OpenCV_DIR` to CMake.

### Example build steps

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

This produces an executable named:

- `projekt2_opencv` (as defined in `CMakeLists.txt`)

---

## Run

Run the executable and enter a PPM filename when prompted:

```text
Enter PPM image filename (e.g., image.ppm):
```

The app opens:
- `Original Image` window
- A new window for each applied operation/filter

You’ll see an interactive menu like:

- Average Filter (3x3)
- Median Filter (3x3)
- Gaussian Blur
- Sobel Edge Detection
- Unsharp Mask
- Custom Filter
- Grayscale (ITU-R)
- Add/Subtract/Multiply/Divide
- Grayscale (Mean)

---

## Usage tips / notes

- **Kernel sizes** for average/median/custom filters must be **odd and >= 3**.
- **Unsharp amount** is expected in the range **0.5–2.0**.
- For **divide**, entering `0` is rejected (division-by-zero is prevented).
- Border handling in many filters uses **clamping** (samples outside the image clamp to nearest valid pixel).
- The OpenCV preview converts your internal RGB pixels into OpenCV’s BGR format before display.

---

## License

No license file is currently included in the repository. If you want this to be reusable by others, add a `LICENSE` (e.g., MIT, Apache-2.0, GPL-3.0, etc.).
