#include <cv.h>         // openCV head file. 
#include <highgui.h>	// openCV head file.

IplImage* bgr2hsv( IplImage* bgr );
IplImage* hsv2bgr( IplImage* hsv );
void GetImageRect(IplImage* orgImage, CvRect rectInImage, IplImage* imgRect);

