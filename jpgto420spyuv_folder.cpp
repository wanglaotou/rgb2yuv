// If you use visual studio, define _CRT_SECURE_NO_WARNINGS to use unsafe function fopen.
// how to make: g++ -g jpgto420spyuv_folder.cpp `pkg-config --libs opencv`

#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstring>        // for strcat()
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
using namespace std;
using namespace cv;
#define max 100
template<class T>

int length(T& arr)
{
    //cout << sizeof(arr[0]) << endl;
    //cout << sizeof(arr) << endl;
    return sizeof(arr) / sizeof(arr[0]);
}

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



/* Show all files under dir_name , do not show directories ! */
vector<string> listFiles( const char * dir_name )
{
	vector<string> allfiles;
	// check the parameter !
	if( NULL == dir_name )
	{
		cout<<" dir_name is null ! "<<endl;
	}

	// check if dir_name is a valid dir
	struct stat s;
	lstat( dir_name , &s );
	if( ! S_ISDIR( s.st_mode ) )
	{
		cout<<"dir_name is not a valid directory !"<<endl;
	}

	struct dirent * filename;    // return value for readdir()
 	DIR * dir;                   // return value for opendir()
	dir = opendir( dir_name );
	if( NULL == dir )
	{
		cout<<"Can not open dir "<<dir_name<<endl;
	}
	cout<<"Successfully opened the dir !"<<endl;

	/* read all the files in the dir ~ */
	while( ( filename = readdir(dir) ) != NULL )
	{
		// get rid of "." and ".."
		if( strcmp( filename->d_name , "." ) == 0 ||
			strcmp( filename->d_name , "..") == 0    )
			continue;
		cout<<filename->d_name <<endl;
		allfiles.push_back(filename->d_name);
	}
	return allfiles;
}

string to_String(int n)
{
    int m = n;
    char s[max];
    char ss[max];
    int i=0,j=0;
    if (n < 0)// ��������
    {
        m = 0 - m;
        j = 1;
        ss[0] = '-';
    }
    while (m>0)
    {
        s[i++] = m % 10 + '0';
        m /= 10;
    }
    s[i] = '\0';
    i = i - 1;
    while (i >= 0)
    {
        ss[j++] = s[i--];
    }
    ss[j] = '\0';
    return ss;
}

int main()
{
	// int folder[] = {1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,51,52,53,54,55,
	// 56,57,59,61,62,63,64,65,66,67,69,70,71,72,73,74,75,76,77,78,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127};
	// int folder[] = {1,2,3,4,5,6,7,8,10,11,12,13,14,15,16,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,51,52,53,54,55,58,59,61,62,64,65,66,67,68,69,70,71,72,
	// 73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,104,105,106,107,108,109,110,111,112,113,115,116,117,118,119,120,121};

	// int folder[] = {1732,1733,1734,1735,1736,1737,1738,1739,1740,1741,1742,1743,1744,1745,1746,1747,1748,1749,1750,1751,1752,1753,1754,1755,1756,1757,1758,1759,1760,1761,1762,
	// 1763,1764,1765,1766,1767,1768,1769,1770,1771,1772,1773,1774,1775,1776,1777,1778,1779,1781,1782,1783,1784,1785,1786,1787,1788,1789,1790,1791,1792,1793,1794,1795,1796,1797,1799
	// ,1800,1801,1802,1803,1804,1805,1806,1807,1808,1809,1810,1811,1812,1813,1814,1815,1816,1817,1818,1819,1820,1821,1822,1823,1824,1825,1826,1827,1828,1829,1830,1831,1832,1833};
	// int folder[] = {871,872,873,874,876,877,878,879,881,882,883,884,885,886,887,888,889,891,892,894,895,896,897,898,899,900,901,902,903,904,905,907,908,909,910,
	// 911,912,913,914,915,916,917,918,919,920,921,922,923,924,925,926,927,928,929,930,931,932,933,934,935,936,937,938,939,940,941,942,943,944,945,946,947,948,949,950,951,952,953,954,
	// 955,956,957,958,959,960,961,962,963,964,965,966,967,968,969,970,971,972,973,974,975};
	int folder[] = {1814};
	cout<<length(folder)<<endl;
	for(int f=0;f<length(folder);f++)
	{
		char dir[200];
		string sdir;
		sdir = "/home/workspace/RMAI/tmpRw/mrwang/norm/reid_gn_0420/back/"+to_String(folder[f])+"/query/";
		// sdir = "/home/workspace/RMAI/tmpRw/mrwang/norm/reid_0412/back/"+to_String(folder[f])+"/query/";
		strcpy(dir,sdir.c_str());
		//strcat(dir,"*.*");
		vector<string> xmlname;
		xmlname = listFiles(dir);
		for(int i=0;i<xmlname.size();i++)
		{
			string pattern = ".jpg";
			int pos = xmlname[i].find(pattern);
			string name = xmlname[i].substr(0,pos);
			cout << "name:" << name << endl;
			string imgpath = sdir+xmlname[i];
			//sdir.append(xmlname[i]);
			Mat img_eye = imread(imgpath.c_str());
			size_t height = img_eye.rows;
			size_t width = img_eye.cols;
			unsigned char* img_eye_bgr_data = (unsigned char*)malloc(height * width * 3 * sizeof(unsigned char));
			//if (img_eye_bgr_data == nullptr)
			//{
			//   printf("malloc error");
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
			string yuvname = name+".yuv";
			string resultImgPath = sdir+yuvname;
			FILE* fp = fopen(resultImgPath.c_str(), "wb");
			fwrite(img_eye_nv12_data, sizeof(unsigned char), height * width * 3 / 2, fp);
			free(img_eye_nv12_data);
			free(img_eye_bgr_data);
		}
	}

    return 0;
}
