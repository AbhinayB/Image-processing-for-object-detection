#include <opencv/cv.h>

#include <opencv/highgui.h>

#include <opencv2\opencv.hpp>

#include <iostream>

#include <stdio.h>

#include <stdlib.h>

#include <string>

using namespace cv;

int lowerH=50;

int lowerS=192;

int lowerV=91;

int upperH=180;

int upperS=256;

int upperV=250;

int erodecount=1;

int maxerode=20;

int dilatecount=1;

int maxdilation=20;

IplImage* GetThresholdedImage(IplImage* imgHSV){
 
	IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
 
	cvInRangeS(imgHSV, cvScalar(lowerH,lowerS,lowerV), cvScalar(upperH,upperS,upperV), imgThresh); 
 
	return imgThresh;
}


void setwindowSettings(){
 
	cvNamedWindow("Video");
 
	cvNamedWindow("Ball");
 
	cvCreateTrackbar("LowerH", "Ball", &lowerH, 180, NULL);
    
	cvCreateTrackbar("UpperH", "Ball", &upperH, 180, NULL);
  
	cvCreateTrackbar("LowerS", "Ball", &lowerS, 256, NULL);
    
	cvCreateTrackbar("UpperS", "Ball", &upperS, 256, NULL);
  
	cvCreateTrackbar("LowerV", "Ball", &lowerV, 256, NULL);
    
	cvCreateTrackbar("UpperV", "Ball", &upperV, 256, NULL); 
  	
	createTrackbar("erode","Ball", &erodecount, maxerode, NULL);
	
	createTrackbar("dilate","Ball", &dilatecount, maxdilation, NULL);
}

int main(){
 
	CvCapture* capture =0; 
 
 capture = cvCaptureFromCAM(0);
 
 if(!capture){
 
	 printf("Capture failure\n");
     return -1;
 }

     
 IplImage* frame=0;
 
 vector<vector<Point> > contours;

 vector<Vec4i> hierarchy;
 
 int j,framenum=0;

 String abhi;
  
 setwindowSettings();

 while(true){
 
  frame = cvQueryFrame(capture);
 
  if(!frame)  break;
  
  frame=cvCloneImage(frame); 

   IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
  
   cvCvtColor(frame, imgHSV, CV_BGR2HSV); 
  
   IplImage* imgThresh = GetThresholdedImage(imgHSV);
  
   cvErode(imgThresh, imgThresh, 0, erodecount);
  
   cvDilate(imgThresh, imgThresh, 0, dilatecount);
  
   cv::Mat mrix = cv::cvarrToMat(imgThresh);
  
   findContours( mrix,contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  
   std::cout << contours.size();
  
   j=contours.size();
  
   if(j==1){abhi="GES 1";framenum++;   }
  
   else if(j==2){abhi="GES 2";framenum++; }
  
   else if(j==3){abhi="GES 3";framenum++;}else if(j==4){abhi="GES 4";framenum++;}
  
   else if(j==5){abhi="GES 5";framenum++;}else{ abhi="TOO MANY";framenum=0;}
  
   putText(mrix,abhi,cvPoint(30,90),FONT_ITALIC,2,cvScalar(255,255,255),4, CV_AA);
  
   if(framenum>10&&j==1)
   {
	
	   system("START notepad");
      
	   framenum=0;  
  }
   if(framenum>10&&j==2)
   {
	
	   system("STOP` notepad");
      
	   framenum=0;  
  }
  if(framenum>10&&j==3)
   {
	
	   system("START iexplore");
      
	   framenum=0;  
  }
  if(framenum>10&&j==4)
   {
	
	   system("START mspaint");
      
	   framenum=0;  
  }
  if(framenum>10&&j==5)
   {
	
	   system("START calc.exe");
      
	   framenum=0;  
  }

   imshow("Ball",mrix);
  
   cvShowImage("Video", frame);
  
   cvReleaseImage(&imgHSV);
  
   cvReleaseImage(&imgThresh);
  
   cvReleaseImage(&frame);

   
  int c = cvWaitKey(80);
  
  if((char)c==27 ) break;

 }

  
 cvDestroyAllWindows();

 cvReleaseCapture(&capture);

       return 0;
}