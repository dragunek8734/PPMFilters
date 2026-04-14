#include "utils.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

// Helper function to convert PPM to OpenCV Mat
cv::Mat PPMtoMat(const PPM& image) {
    int width = image.getWidth();
    int height = image.getHeight();
    
    // Create OpenCV Mat in BGR format (OpenCV uses BGR, not RGB)
    cv::Mat mat(height, width, CV_8UC3);
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const Pixel& pix = image.getPixel(x, y);
            // OpenCV uses BGR, so reverse the order
            mat.at<cv::Vec3b>(y, x) = cv::Vec3b(pix.GetBlue(), pix.GetGreen(), pix.GetRed());
        }
    }
    
    return mat;
}

// Function to display menu and get user choice
int displayMenu() {
    std::cout << "\n========== IMAGE FILTER MENU ==========" << std::endl;
    std::cout << "1. Average Filter (3x3)" << std::endl;
    std::cout << "2. Median Filter (3x3)" << std::endl;
    std::cout << "3. Gaussian Blur" << std::endl;
    std::cout << "4. Sobel Edge Detection" << std::endl;
    std::cout << "5. Unsharp Mask" << std::endl;
    std::cout << "6. User-defined Custom Filter" << std::endl;
    std::cout << "7. Grayscale (ITU-R)" << std::endl;
    std::cout << "--- PIXEL OPERATIONS ---" << std::endl;
    std::cout << "8. Add to Pixels" << std::endl;
    std::cout << "9. Subtract from Pixels" << std::endl;
    std::cout << "10. Multiply Pixels" << std::endl;
    std::cout << "11. Divide Pixels" << std::endl;
    std::cout << "12. Grayscale (Mean)" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << "Enter your choice: ";
    
    int choice;
    std::cin >> choice;
    return choice;
}

int main() {
    try {
        // Get filename from user
        std::string filename;
        std::cout << "Enter PPM image filename (e.g., image.ppm): ";
        std::cin >> filename;
        
        // Load image
        std::cout << "Loading image..." << std::endl;
        PPM originalImage;
        originalImage.LoadFromFile(filename);
        
        std::cout << "Image loaded successfully!" << std::endl;
        std::cout << "Resolution: " << originalImage.getWidth() << "x" 
                  << originalImage.getHeight() << std::endl;
        
        // Display original image
        cv::Mat originalMat = PPMtoMat(originalImage);
        cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE);
        cv::imshow("Original Image", originalMat);
        
        bool running = true;
        while (running) {
            int choice = displayMenu();
            PPM filteredImage = originalImage;
            std::string filterName = "";
            
            switch (choice) {
                case 1: {
                    std::cout << "Applying Average Filter..." << std::endl;
                    filteredImage = avgFilter(originalImage, 3);
                    filterName = "Average Filter (3x3)";
                    break;
                }
                case 2: {
                    std::cout << "Applying Median Filter..." << std::endl;
                    filteredImage = medianFilter(originalImage, 3);
                    filterName = "Median Filter (3x3)";
                    break;
                }
                case 3: {
                    float sigma;
                    std::cout << "Enter sigma value (e.g., 1.0, 1.5, 2.0): ";
                    std::cin >> sigma;
                    std::cout << "Applying Gaussian Blur (sigma=" << sigma << ")..." << std::endl;
                    filteredImage = gaussianFilter(originalImage, sigma);
                    filterName = "Gaussian Blur (sigma=" + std::to_string(sigma) + ")";
                    break;
                }
                case 4: {
                    std::cout << "Applying Sobel Edge Detection..." << std::endl;
                    filteredImage = sobelFilter(originalImage);
                    filterName = "Sobel Edge Detection";
                    break;
                }
                case 5: {
                    float amount;
                    std::cout << "Enter sharpening amount (0.5-2.0): ";
                    std::cin >> amount;
                    std::cout << "Applying Unsharp Mask (amount=" << amount << ")..." << std::endl;
                    filteredImage = unsharp(originalImage, amount);
                    filterName = "Unsharp Mask (amount=" + std::to_string(amount) + ")";
                    break;
                }
                case 6: {
                    int kernelSize;
                    std::cout << "Enter kernel size (odd number, e.g., 3, 5, 7): ";
                    std::cin >> kernelSize;
                    std::cout << "Applying Custom User Filter..." << std::endl;
                    filteredImage = userFilter(originalImage, kernelSize);
                    filterName = "Custom User Filter";
                    break;
                }
                case 7: {
                    std::cout << "Converting to Grayscale (ITU-R)..." << std::endl;
                    GrayScale(filteredImage);
                    filterName = "Grayscale (ITU-R)";
                    break;
                }
                case 8: {
                    int modifier;
                    std::cout << "Enter value to add (0-255): ";
                    std::cin >> modifier;
                    std::cout << "Adding " << modifier << " to pixels..." << std::endl;
                    addPixel(filteredImage, modifier);
                    filterName = "Add Pixels (" + std::to_string(modifier) + ")";
                    break;
                }
                case 9: {
                    int modifier;
                    std::cout << "Enter value to subtract (0-255): ";
                    std::cin >> modifier;
                    std::cout << "Subtracting " << modifier << " from pixels..." << std::endl;
                    subtractPixel(filteredImage, modifier);
                    filterName = "Subtract Pixels (" + std::to_string(modifier) + ")";
                    break;
                }
                case 10: {
                    float modifier;
                    std::cout << "Enter multiply factor (e.g., 0.5, 1.5, 2.0): ";
                    std::cin >> modifier;
                    std::cout << "Multiplying pixels by " << modifier << "..." << std::endl;
                    multiplyPixel(filteredImage, modifier);
                    filterName = "Multiply Pixels (" + std::to_string(modifier) + ")";
                    break;
                }
                case 11: {
                    float modifier;
                    std::cout << "Enter divide factor (e.g., 0.5, 1.5, 2.0): ";
                    std::cin >> modifier;
                    if(modifier == 0.0f) {
                        std::cout << "Cannot divide by zero!" << std::endl;
                        continue;
                    }
                    std::cout << "Dividing pixels by " << modifier << "..." << std::endl;
                    dividePixel(filteredImage, modifier);
                    filterName = "Divide Pixels (" + std::to_string(modifier) + ")";
                    break;
                }
                case 12: {
                    std::cout << "Converting to Grayscale (Mean)..." << std::endl;
                    GrayScaleMean(filteredImage);
                    filterName = "Grayscale (Mean)";
                    break;
                }
                case 0: {
                    std::cout << "Exiting application..." << std::endl;
                    running = false;
                    break;
                }
                default: {
                    std::cout << "Invalid choice! Please try again." << std::endl;
                    continue;
                }
            }
            
            if (choice != 0 && choice >= 1 && choice <= 12) {
                // Display filtered image
                cv::Mat filteredMat = PPMtoMat(filteredImage);
                cv::namedWindow(filterName, cv::WINDOW_AUTOSIZE);
                cv::imshow(filterName, filteredMat);
                
                std::cout << "Filter applied successfully! Displaying comparison..." << std::endl;
                std::cout << "Press any key in the filter window to continue." << std::endl;
                cv::waitKey(0);
            }
        }
        
        // Clean up
        cv::destroyAllWindows();
        std::cout << "Application closed." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
