#include <cv.h>         // openCV head file. 

IplImage* bgr2hsv( IplImage* bgr )
{
   IplImage* bgr32f, * hsv;
   bgr32f = cvCreateImage( cvGetSize(bgr), IPL_DEPTH_32F, 3 );
   hsv = cvCreateImage( cvGetSize(bgr), IPL_DEPTH_32F, 3 );
   cvConvertScale( bgr, bgr32f, 1.0 / 255.0, 0 );
   cvCvtColor( bgr32f, hsv, CV_BGR2HSV );
   cvReleaseImage( &bgr32f );
   return hsv;
 }


IplImage* hsv2bgr( IplImage* hsv )
{
   IplImage * bgr, *bgr32f;
   bgr32f = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_32F, 3 );
   bgr = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_8U, 3 );
   cvCvtColor(hsv, bgr32f, CV_HSV2BGR );
   cvConvertScale( bgr32f, bgr, 255.0, 0 );
   cvReleaseImage( &bgr32f );
   return bgr;
}

void GetImageRect(IplImage* orgImage, CvRect rectInImage, IplImage* imgRect)
{
// extract a sub-image from orgImage
 IplImage *result=imgRect;
 CvSize size;
 size.width=rectInImage.width;
 size.height=rectInImage.height;
 cvSetImageROI(orgImage,rectInImage);
 cvCopy(orgImage,result);
 cvResetImageROI(orgImage);
}


