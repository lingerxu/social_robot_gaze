#include <cv.h>         // openCV head file. 
#include <highgui.h>	// openCV head file.

#include <stdio.h>

#include "ExperimentDefine.h"
#include "cv_util.h"

extern bool  OUTPUT_INTERMEDIATE_RESULTS;

IplImage ** detect_color_object(IplImage * img)
{
     // convert image to HSV
     IplImage * hsv = bgr2hsv(img);
   	 IplImage ** objects = (IplImage **) operator new (sizeof(IplImage*) * OBJ_NUM); // OBJ_NUM, shw modi, 2011.05.03
     
     // check each pixel of the image    
     IplImage * H, * S, * V;
     H = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_32F, 1);
     S = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_32F, 1);
     V = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_32F, 1);
     
     cvSplit(hsv, H, S, V, 0);
     
//   cvConvertScale( S, S, 255.0, 0 );
//   cvConvertScale( V, V, 255.0, 0 );
//   cvConvertScale( H, H, 255.0, 0 );

	/* if (OUTPUT_INTERMEDIATE_RESULTS) {
		cvSaveImage("h.jpg", H);
		cvSaveImage("s.jpg", S);
		cvSaveImage("v.jpg", V);
     }     
	 */
 
     IplImage * H_B, * S_B, *V_B;
     H_B = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_8U, 1 );
     S_B = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_8U, 1 );
     V_B = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_8U, 1 );
     
   	 cvThreshold( V, V_B, 0.1, 127, CV_THRESH_BINARY );
   	 cvThreshold( S, S_B, 0.12, 127, CV_THRESH_BINARY );
    
	if (OUTPUT_INTERMEDIATE_RESULTS) {
		cvSaveImage("v-B.jpg", V_B);
		cvSaveImage("s-B.jpg", S_B);
	}
     
     IplImage * NotBackground = cvCreateImage(cvGetSize(hsv), IPL_DEPTH_8U, 1);
     cvAdd(V_B, S_B,  NotBackground);
     cvThreshold(NotBackground, NotBackground, 150, 127, CV_THRESH_BINARY);
 	if (OUTPUT_INTERMEDIATE_RESULTS) {
 		cvSaveImage("NotBackground.jpg", NotBackground);
	}

     // objects[0] = (h > 0.56) & (h < 0.72) & (s > 0.55) & ~bk;  Blue object
     objects[0] = cvCreateImage(cvGetSize(hsv), IPL_DEPTH_8U, 1); 
   	 cvThreshold( H, objects[0], 0.56*360, 127, CV_THRESH_BINARY );
   	 cvThreshold( H, H_B,  0.72*360, 127, CV_THRESH_BINARY_INV );  	 
     cvAnd(objects[0], H_B, objects[0]);
   	 cvThreshold( S, S_B,   0.55, 127, CV_THRESH_BINARY );  	   	 
     cvAnd(objects[0], S_B, objects[0]);
	 cvAnd(objects[0], NotBackground, objects[0]);


     // objects[1] = (h > 0.28 ) & ( h<0.48) & (s>0.3) & ~bk;  // Green object
     objects[1] = cvCreateImage(cvGetSize(hsv), IPL_DEPTH_8U, 1); 
   	 cvThreshold( H, objects[1], 0.28*360, 127, CV_THRESH_BINARY );
   	 cvThreshold( H, H_B,  0.48*360, 127, CV_THRESH_BINARY_INV );  	 
     cvAnd(objects[1], H_B, objects[1]);
   	 cvThreshold( S, S_B,   0.3, 127, CV_THRESH_BINARY );  	   	 
     cvAnd(objects[1], S_B, objects[1]);
     cvAnd(objects[1], NotBackground, objects[1]);

      // objects[2] = (h > 0.8) & & (h < 0.94) & (s>0.4) ~bk;   // Pink object
     objects[2] = cvCreateImage(cvGetSize(hsv), IPL_DEPTH_8U, 1); 
   	 cvThreshold( H, objects[2], 0.8*360, 127, CV_THRESH_BINARY );
   	 cvThreshold( H, H_B,  0.94*360, 127, CV_THRESH_BINARY_INV );  	 
     cvAnd(objects[2], H_B, objects[2]);
     cvThreshold( S, S_B,   0.4, 127, CV_THRESH_BINARY );  	   	 
     cvAnd(objects[2], S_B, objects[2]);
     cvAnd(objects[2], NotBackground, objects[2]);     

	 if (OBJ_NUM > 3) {
		  // objects[3] = ((h < 0.04 ) | ( h>0.96)) & (s>0.65) & ~bk;  // red object
		 objects[3] = cvCreateImage(cvGetSize(hsv), IPL_DEPTH_8U, 1); 
   		 cvThreshold( H, objects[3], 0.96*360, 127, CV_THRESH_BINARY );
   		 cvThreshold( H, H_B,  0.04*360, 127, CV_THRESH_BINARY_INV );  	 
		 cvOr(objects[3], H_B, objects[3]);
   		 cvThreshold( S, S_B,  0.65, 127, CV_THRESH_BINARY );  	   	 
		 cvAnd(objects[3], S_B, objects[3]);
		 cvAnd(objects[3], NotBackground, objects[3]);
	 }

 	 if (OBJ_NUM > 4) {  // objects[4] = (h > 0.12) & (h < 0.21) & (s>0.65) ~bk;  Yellow object
		 objects[4] = cvCreateImage(cvGetSize(hsv), IPL_DEPTH_8U, 1); 
   		 cvThreshold( H, objects[4], 0.12*360, 127, CV_THRESH_BINARY );
   		 cvThreshold( H, H_B,  0.21*360, 127, CV_THRESH_BINARY_INV );  	 
		 cvAnd(objects[4], H_B, objects[4]);
   		 cvThreshold( S, S_B,  0.65, 127, CV_THRESH_BINARY );  	   	 
		 cvAnd(objects[4], S_B, objects[4]);
		 cvAnd(objects[4], NotBackground, objects[4]);
	 }

	 for (int i=0; i<OBJ_NUM; i++) {
		 cvSmooth(objects[i],objects[i],CV_MEDIAN,9,0,0,0);
	 }

 	 if (OUTPUT_INTERMEDIATE_RESULTS) {
		 for (int i=0; i<OBJ_NUM; i++) {
			 char img_name_tmp[128];
			 sprintf(img_name_tmp, "obj%d.jpg", i);
			 cvSaveImage(img_name_tmp, objects[i]);
		 }
	 }

	 cvReleaseImage(&hsv);
	 cvReleaseImage(&H);
	 cvReleaseImage(&S);
	 cvReleaseImage(&V);
	 cvReleaseImage(&H_B);
	 cvReleaseImage(&S_B);
	 cvReleaseImage(&V_B);
	 cvReleaseImage(&NotBackground);

	 return objects;
}
