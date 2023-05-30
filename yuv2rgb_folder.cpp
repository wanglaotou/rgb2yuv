
// g++ -g yuv2rgb_folder.cpp `pkg-config --libs opencv`


#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <opencv2/opencv.hpp>

// YUV420 to RGB conversion function
void YUV420toRGB(const unsigned char* yuvData, unsigned char* rgbData, int width, int height, int type)
{
    cv::Mat yuvImage(height + height / 2, width, CV_8UC1, const_cast<unsigned char*>(yuvData));
    cv::Mat rgbImage(height, width, CV_8UC3, rgbData);

    if(0 == type)
    {
        cv::cvtColor(yuvImage, rgbImage, cv::COLOR_YUV2RGB_NV21);
    }
    else if(1 == type) 
    {
        cv::cvtColor(yuvImage, rgbImage, cv::COLOR_YUV2RGB_NV21);
    }
    else
    {
        std::cout << "error type mode!" << std::endl;
    }
}

bool isFolderExists(const std::string& folderPath)
{
    struct stat buffer;
    return (stat(folderPath.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
}

bool createFolder(const std::string& folderPath)
{
    int result = mkdir(folderPath.c_str(), 0777);
    return (result == 0);
}

int main()
{

    int width = 1920;  // YUV image width
    int height = 1280; // YUV image height

    // Allocate memory for YUV and RGB data
    int yuvSize = width * height * 3 / 2; // YUV420 format
    int rgbSize = width * height * 3;     // RGB format
    unsigned char* yuvData = new unsigned char[yuvSize];
    unsigned char* rgbData = new unsigned char[rgbSize];

    
    std::string folderPath = "/home/dell/datasets/data_calib/pipeline0";
    std::string newFolderPath = "/home/dell/datasets/data_calib/pipeline0_rgb";
    if(isFolderExists(newFolderPath))
    {
        std::cout << "Folder already exists: " << folderPath << std::endl;
    }
    else
    {
        if(createFolder(newFolderPath))
        {
            std::cout << "Folder created successed: " << newFolderPath << std::endl;
        }
        else
        {
            std::cout << "Failed to create folder: " << newFolderPath << std::endl;
        }
    }

    std::string fileName;
    DIR* dir;
    struct dirent* entry;
    if((dir = opendir(folderPath.c_str())) == nullptr)
    {
        std::cerr << "Failed to open folder:" << folderPath << std::endl;
        return -1;
    }
    while((entry = readdir(dir)) != nullptr)
    {
        if(entry->d_type == DT_REG)
        {
            fileName = entry->d_name;
            std::string imgPath = folderPath + "/" + fileName;
            std::cout << "imgPath: " << imgPath << std::endl;
            // Read YUV data from file
            std::ifstream file(imgPath, std::ios::binary);
            if (!file)
            {
                std::cerr << "Failed to open input file!" << std::endl;
                return 1;
            }

            // Read YUV data from file
            file.read(reinterpret_cast<char*>(yuvData), yuvSize);
            file.close();

            // Convert YUV to RGB
            YUV420toRGB(yuvData, rgbData, width, height, 1);

            // Save RGB data as JPEG image
            cv::Mat rgbImage(height, width, CV_8UC3, rgbData);
            std::vector<int> params;
            params.push_back(cv::IMWRITE_JPEG_QUALITY);
            params.push_back(90);
            std::string outputFilename = newFolderPath + "/" + fileName + ".jpg";
            cv::imwrite(outputFilename, rgbImage, params);
        }
        
    }

    // Clean up memory
    delete[] yuvData;
    delete[] rgbData;

    return 0;
}
