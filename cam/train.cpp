#include "train.h"
char file_path[] = "./Result/";
CvMat *trainData;
CvMat *trainClasses;
int train_samples = 300;
int classes= 10;
int size_w = 15;
int size_h = 25;
static const int K = 10;//KNN
CvKNearest *knn;

void getData()
{
	IplImage* src_image;
	CvMat row,data;
	char file[255];
	int i,j;
	for(i=0;i<classes;i++)
	{
		for(j=0;j<train_samples;j++)
		{
			if(j<10)
			{
				sprintf(file,"%s%d/%d00%d.pbm",file_path,i,i,j);
			}
			else if(j<100)
			{
				sprintf(file,"%s%d/%d0%d.pbm",file_path,i,i,j);
			}
			else
			{
				sprintf(file,"%s%d/%d%d.pbm",file_path,i,i,j);
			}
			//src_image = imread(file,0);
			src_image = cvLoadImage(file, 0);
		   	if(!src_image)
			{
				printf("Error: Cant load image %s\n", file);
				//exit(-1);
			}

			//Set class label
			cvGetRow(trainClasses, &row, i*train_samples + j);
			cvSet(&row, cvRealScalar(i));

			//Set data 
			cvGetRow(trainData, &row, i*train_samples + j);    
			IplImage* img = cvCreateImage(cvSize(size_w, size_h), IPL_DEPTH_32F, 1);
			//convert 8 bits image to 32 float image
			cvConvertScale(src_image, img, 0.0039215, 0);
			cvGetSubRect(img, &data, cvRect(0, 0, size_w, size_h));
			CvMat row_header, *row1;
			row1 = cvReshape(&data, &row_header, 0, 1);
			cvCopy(row1, &row, NULL);
		}
	}
}

void train()
{
	knn = new CvKNearest(trainData, trainClasses, 0, false, K);
	printf(" -----Train success---------------\n");
}

int classify(IplImage* img)
{
	CvMat data;
	CvMat* nearest = cvCreateMat(1, K, CV_32FC1);
	float result;

	//Set data 
	IplImage* img32 = cvCreateImage(cvSize(size_w, size_h), IPL_DEPTH_32F, 1);
	cvConvertScale(img, img32, 0.0039215, 0);
	cvGetSubRect(img32, &data, cvRect(0, 0, size_w, size_h));
	CvMat row_header, *row1;
	row1 = cvReshape(&data, &row_header, 0, 1);

	result = knn->find_nearest(row1, K, 0, 0, nearest, 0);

	int accuracy = 0;
	for (int i = 0; i<K; i++)
        {
		if (nearest->data.fl[i] == result)
			accuracy++;
	}
	float pre = 100 * ((float)accuracy / (float)K);
    printf("|\t%.0f\t| \t%.2f%%  \t| \t%d of %d \t| \n", result, pre, accuracy, K);
	//printf("|\t%.0f\t| \t%.2f%%  \t| \n", result, pre);
	//printf(" ---------------------------------------------------------------\n");
	return (int)result;
}



