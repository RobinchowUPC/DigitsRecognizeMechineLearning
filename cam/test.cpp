#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  
#include <stdio.h>
#include <iostream>
#include <string>
#include <ml.h>
#include "train.h"

using namespace cv;  
using namespace std;

Point2f vertex[4];
Rect rect;
struct RectInfo{
int value;
int cen_x;
int cen_y;
}  rectinfo[255];
//RectInfo tempsort;
const int totaldigits = 11;
const int totalpassword = 5;

void SortRect(struct RectInfo t[],int n);

int main()  
{  
	VideoCapture cap(0); 
	if(!cap.isOpened())  
	{  
		return -1;  
	}  
	Mat frame;  
	Mat gray_frame;  
	Mat gray_contours;
	vector<vector<Point> > contours;
	//vector<Mat> contours(100);
	vector<vector<Point> >::iterator It;
	vector<Vec4i> hierarchy;
	int rect_num = 0;
	char str_rectnum[3]="ok";
    int rect_height;
    int rect_width;
	String s;
	bool stop = false;  
	// complete the parameter of imwrite
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PXM_BINARY);
	compression_params.push_back(0);
    //creat train matrix
	trainData = cvCreateMat(train_samples*classes, size_h * size_w, CV_32FC1);
	trainClasses = cvCreateMat(train_samples*classes, 1, CV_32FC1);
	getData();
	train();
	while(!stop)  
	{      
		//show the number           
		cap>>frame;  //capture the frame   
		rect_num = 0;          	  
		imshow("Oral Video", frame);  //show the Oral frame         
		cvtColor(frame,gray_frame,CV_BGR2GRAY);//convert to gray image           
		threshold(gray_frame,gray_frame,110,255,CV_THRESH_BINARY_INV);//show the threshold frame	    
		erode(gray_frame,gray_frame,Mat(1,1,CV_8U),Point(-1,-1),1);//erode image
                dilate(gray_frame,gray_frame,Mat(2,2,CV_8U),Point(-1,-1),1);//dilate image
		imshow("Threshold Video",gray_frame);  
		gray_frame.copyTo(gray_contours);
		//adaptiveThreshold(gray_frame,gray_frame,255,0,1,5,10);
		findContours(gray_contours, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		Mat result(gray_frame.size(),CV_8U,Scalar(255));   
		drawContours(result,contours,-1,Scalar(0),2);
		imshow("Contours Video",result);  
         
		for(It = contours.begin();It < contours.end(); ++It) //get the rects of numbers
		{	
			rect = boundingRect(*It);
			vertex[0] = rect.tl();//
			vertex[1].x = (float)rect.tl().x, vertex[1].y = (float)rect.br().y;
			vertex[2] = rect.br();
			vertex[3].x = (float)rect.br().x, vertex[3].y = (float)rect.tl().y;  
            rect_width =   rect.br().x - rect.tl().x;
            rect_height = rect.br().y - rect.tl().y;
			if( ((rect_height * rect_width) < 600) && ( 150 < (rect_height * rect_width))&& (rect_height < 35) &&(10 <rect_height) && (5 < rect_width)&& (rect_width < 30))//*******Threshold********
			{
				
				sprintf(str_rectnum,"%d",rect_num);
				Mat ROI_image = gray_frame(rect); //Save ROI image
				s=str_rectnum;
				resize(ROI_image, ROI_image, Size(size_w, size_h), 0, 0, INTER_NEAREST);
				imwrite("./ROI//"+s+".pbm",ROI_image,compression_params);
				IplImage recog_wait(ROI_image);
                //printf("Local:%d   %d   %d   %d \t ",rect.tl().x,rect.tl().y,rect.br().x,rect.br().y);
                // complete the info of rect
				rectinfo[rect_num].cen_x = (rect.tl().x+rect.br().x)/2;
				rectinfo[rect_num].cen_y = (rect.tl().y+rect.br().y)/2;
				rectinfo[rect_num].value = classify(&recog_wait);
                rect_num  ++;
				for(int j=0;j<4;j++) //Draw Rect
				{
					line(gray_frame,vertex[j],vertex[(j+1)%4],Scalar(255,255,255),1);
				}                       
			}
		}
		if(It == contours.end())
		{
             if(rect_num == totaldigits)
		     {
				SortRect(rectinfo,rect_num);
				for(int j = 0;j < rect_num;j++)
				{
					printf("%d local:%d %d \n",rectinfo[j].value,rectinfo[j].cen_x,rectinfo[j].cen_y);	
				}
		     }
                     printf("\n -----%d--------OVER------------------------\n",rect_num);
		}
		imshow("CRect Video",gray_frame);  
		//cout << "Contours: "  << endl;
		if(waitKey(30) >=0)  
		stop = true;  
		}  
	return 0;  
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

