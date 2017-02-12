    #include <opencv2/highgui/highgui.hpp>  
    #include <opencv2/imgproc/imgproc.hpp>  
    #include <opencv2/core/core.hpp>  
    #include <stdio.h>
    #include <iostream>
    #include <string>
    #include <ml.h>
extern CvMat *trainData;
extern CvMat *trainClasses;
extern int train_samples;
extern int classes;
extern int size_w;
extern int size_h;
extern int classify(IplImage* img);
extern void train();
extern void getData();

