// If you use visual studio, define _CRT_SECURE_NO_WARNINGS to use unsafe function fopen.
// how to make: g++ -g jpgto420spyuv_folder.cpp `pkg-config --libs opencv`
#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


// ��U��VΪNV12����V��UΪNV21
void encodeYUV420sp(unsigned char* yuv420sp, unsigned char* rgb, int width, int height, int type = 0)
{
    //if (yuv420sp == nullptr || rgb == nullptr)
    //    return;
    int frameSize = width * height;
    int yIndex = 0;
    int uvIndex = frameSize;

    int R, G, B, Y, U, V;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            B = rgb[(i * width + j) * 3 + 0];
            G = rgb[(i * width + j) * 3 + 1];
            R = rgb[(i * width + j) * 3 + 2];

            // RGB to YUV
            Y = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
            U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
            V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;

            yuv420sp[yIndex++] = (unsigned char)((Y < 0) ? 0 : ((Y > 255) ? 255 : Y));
            if (i % 2 == 0 && j % 2 == 0)
            {
                if (type == 0)
                {
                    yuv420sp[uvIndex++] = (unsigned char)((U < 0) ? 0 : ((U > 255) ? 255 : U));
                    yuv420sp[uvIndex++] = (unsigned char)((V < 0) ? 0 : ((V > 255) ? 255 : V));
                }
                else if (type == 1)
                {
                    yuv420sp[uvIndex++] = (unsigned char)((V < 0) ? 0 : ((V > 255) ? 255 : V));
                    yuv420sp[uvIndex++] = (unsigned char)((U < 0) ? 0 : ((U > 255) ? 255 : U));
                }

            }
        }
    }
}


int main()
{
    string file = "/home/workspace/RMAI/mrwang/avitoyuv/rgb2yuv/test361.jpg";
    Mat img_eye = imread(file.c_str());
    size_t height = img_eye.rows;
    size_t width = img_eye.cols;
    cout << "width, height:" << width << "," << height << endl;
    unsigned char* img_eye_bgr_data = (unsigned char*)malloc(height * width * 3 * sizeof(unsigned char));
    //if (img_eye_bgr_data == nullptr)
    //{
    //    printf("malloc error");
    //    return -1;
    //}
    for (int i = 0; i < height; i++)
    {
        unsigned char* current_row = img_eye.ptr<uchar>(i);
        for (int j = 0; j < width; j++)
        {
            img_eye_bgr_data[(i * width + j) * 3 + 0] = current_row[j * 3 + 0]; // B
            img_eye_bgr_data[(i * width + j) * 3 + 1] = current_row[j * 3 + 1]; // G
            img_eye_bgr_data[(i * width + j) * 3 + 2] = current_row[j * 3 + 2]; // R
        }
    }
    unsigned char* img_eye_nv12_data = (unsigned char*)malloc(height * width * 3 / 2 * sizeof(unsigned char));
    //if (img_eye_nv12_data == nullptr)
    //{
    //    printf("malloc error");
    //    return -1;
    //}
    encodeYUV420sp(img_eye_nv12_data, img_eye_bgr_data, width, height);
    string resultImgPath = "/home/workspace/RMAI/mrwang/avitoyuv/rgb2yuv/test361.yuv";
    FILE* fp = fopen(resultImgPath.c_str(), "wb");
    fwrite(img_eye_nv12_data, sizeof(unsigned char), height * width * 3 / 2, fp);
    free(img_eye_nv12_data);
    free(img_eye_bgr_data);
    return 0;
}
