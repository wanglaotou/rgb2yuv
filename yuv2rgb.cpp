#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

// YUV420 to RGB conversion function
void YUV420toRGB(const unsigned char* yuvData, unsigned char* rgbData, int width, int height)
{
    cv::Mat yuvImage(height + height / 2, width, CV_8UC1, const_cast<unsigned char*>(yuvData));
    cv::Mat rgbImage(height, width, CV_8UC3, rgbData);

    cv::cvtColor(yuvImage, rgbImage, cv::COLOR_YUV2RGB_NV12);	 //COLOR_YUV2RGB_NV21
}

int main()
{
    // Read YUV data from file
    std::ifstream file("input.yuv", std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to open input file!" << std::endl;
        return 1;
    }

    int width = 640;  // YUV image width
    int height = 480; // YUV image height

    // Allocate memory for YUV and RGB data
    int yuvSize = width * height * 3 / 2; // YUV420 format
    int rgbSize = width * height * 3;     // RGB format
    unsigned char* yuvData = new unsigned char[yuvSize];
    unsigned char* rgbData = new unsigned char[rgbSize];

    // Read YUV data from file
    file.read(reinterpret_cast<char*>(yuvData), yuvSize);
    file.close();

    // Convert YUV to RGB
    YUV420toRGB(yuvData, rgbData, width, height);

    // Save RGB data as JPEG image
    cv::Mat rgbImage(height, width, CV_8UC3, rgbData);
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(90);
    std::string outputFilename = "output.jpg";
    cv::imwrite(outputFilename, rgbImage, params);

    // Clean up memory
    delete[] yuvData;
    delete[] rgbData;

    return 0;
}
