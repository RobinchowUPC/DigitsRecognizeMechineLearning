#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "ocr_decoder.h"
#include "histogram1d.h"
#include <string>
#include <ml.h>
#include <stdio.h>

using namespace cv;
using namespace std;

struct RectInfo{
	int value;
	int cen_x;
	int cen_y;
}  rectinfo[255];
//RectInfo tempsort;
const int totaldigits = 11;
const int totalpassword = 5;
void SortRect(struct RectInfo t[],int n);

int main(int argc, char* argv[])
{
    VideoCapture cap(0); 
    if(!cap.isOpened())  
    {  
	return -1;  
    } 
    Mat frame; 
    Mat gray_frame;
    Mat temp_frame;
    int i =0;
    char str[3]="ok";
    bool stop = false;  
    while(!stop)
    {
		i = 0;
		if(i>5)
		{
			i= 0 ;
		}
		sprintf(str,"%d",i);

		cap>>frame;//capture the frame 
		if(!frame.empty())
		{
			putText(frame,str,cvPoint(10,20),CV_FONT_HERSHEY_DUPLEX,1.0f,CV_RGB(255,255,255));
			imshow("Oral Video", frame);
			cvtColor(frame,gray_frame,CV_BGR2GRAY);

			//使用tesseract识别
			char buffer[255];
			OCRDecoder decoder;
			//temp_frame = imread("test2.jpg",0);
			decoder.decodeGrayMat(gray_frame, buffer);
			cout<<"result:"<<buffer<<endl;
			if(waitKey(30) >=0)  
				  stop = true; 
		}
	}
}


//Tell the password and the hit area from those rects.
void SortRect(struct RectInfo t[],int n)
{
    RectInfo tempsort;
    for(int i=0;i<n;i++)
    {
	for(int j=0;j<n-i-1;j++)
	{
		if(t[j].cen_y > t[j+1].cen_y)
		{
			tempsort = t[j+1];
			t[j+1] = t[j];
			t[j] = tempsort;		    
		}
	}
    }
    for(int i=0;i < totalpassword;i++)
    {
	for(int j=0;j< totalpassword-1-i;j++)
	{
	if(t[j].cen_x > t[j+1].cen_x)
	{
		tempsort = t[j+1];
		t[j+1] = t[j];
		t[j] = tempsort;		    
	}
	}
    }
}
