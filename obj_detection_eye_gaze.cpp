// try.cpp : Defines the entry point for the console application.
//

#include <afxwin.h>
// #include <winsock.h>

#include <stdlib.h>
#include <iostream>

#include "windows.h"

using namespace std;

// #include "stdafx.h"
//#include "cstring.h"

// ASL COM Server
/*#include "ASLSerialOutLib2.h"
#include "EventHandler.h"
#include "TCPEchoClientWS.h"

extern int DoWinsock(const char* , int);
*/
#define _AFXDLL

// #define GET_TIMING
// #define DEBUG_OUTPUT

#include <cv.h>         // openCV head file. 
#include <highgui.h>	// openCV head file.
#include <math.h>
#include <conio.h>		// for keyboard input

#include "ExperimentDefine.h"
#include "BlobResult.h" // for blob detection on binary image
#include "ConfigFile.h" // for accessing configuraton file

#include "cv_util.h"
#include "blob_util.h"

// Extern functions declaration
extern int  InitEyeTrackerClient(char *, int);
extern void updateETData();
extern void getEyeGaze(int * eye_gaze, int * raw_eye_gaze, int mode);
extern void setGazeBufTime(double seconds);

extern IplImage ** detect_color_object(IplImage * img);
extern IplImage * detect_nao_head(IplImage * img, int edge_widths[], int * eye_gaze, int * ROI_size, int marker_color);
extern IplImage * hBitmap2Ipl(HBITMAP hBmp);

// local functions declaration
void ReadConfigFile();
int process_eye_tracking_image(IplImage * src, char * output_name, int * eye_gaze, int * raw_eye_gaze, int * ROI_size);
IplImage * detection(IplImage * whole_img, IplImage* ROI_img, char * output_name, int * eye_gaze, int * raw_eye_gaze, int * ROI_size);
void DrawRect(IplImage * img, CvRect * rect, int rectNum, CvScalar color); 
int  get_obj_move_distance(int old_x, int old_y, int new_x, int new_y);
void GenOutput(char * output_name, IplImage ** objs, IplImage ** nao_heads, int * eye_gaze, int * raw_eye_gaze, int * ROI_size, int blob_size_threshold, double img_scale);
void show_image_with_eye_gaze(char * img_window_name, IplImage * img, int * eye_gaze);

// define meanning of output
#define OBJ_NOT_THERE 0
#define OBJ_APPEAR 999
#define OBJ_DISAPPEAR -999

// other define
#define HISOTRY_BUFFER_SIZE 512

// define global variables
char  output_log[1024]; 
char  ROI_raw_file[1024];
char  current_eye_file[1024];
FILE  * fp_ROI_raw;
FILE  * fp_current_eye_data;
int   show_eyegaze_on_whole_image =0;
int   show_ROI_image = 1;
bool  OUTPUT_INTERMEDIATE_RESULTS = false;
int   raw_img_height;
int   raw_img_width;
int   raw_ROI_size[2];
float SCALE = 1; // if SCALE is other value, some codes such as output part need to be modified. 	  
bool  print_on_screen = false;
int   use_frame_number;  // if 1, use output_frame_number; else, use output_gaze_duration. 
int   output_frame_number;  
int   output_gaze_duration;  // in ms
int   show_timing;
int   print_current_attention;
static double history_output_s[HISOTRY_BUFFER_SIZE*(OBJ_NUM+HEAD_NUM)];
static double history_output_p[HISOTRY_BUFFER_SIZE*(OBJ_NUM+HEAD_NUM)];
static int    history_target[HISOTRY_BUFFER_SIZE];
static clock_t time_stamps[HISOTRY_BUFFER_SIZE];
static bool flagEyeTrackerInited = false; 

CWnd* m_pMain;


int main(int argc, char* argv[])
{
 /*   if (argc<3) {
        printf("%s input_image_name  ouput_file_name [gaze_mode] [ROI_width] [ROI_height] [max_repeat_number] [print_to_screen]\n", argv[0]);
		printf("Example:\n");
		printf("%s current.bmp eye_gaze.txt\n", argv[0]);
		printf("Or: %s current.bmp eye_gaze.txt 2 300 200 10000 print", argv[0]);
        return 1;
    }
	*/

/*	system("cd");
	printf("pesss a key to start");
	system("pause");
*/
	ConfigFile cf("obj_detect_eye_gaze_config.txt");

	char img_name[1024], output_name[1024]; 
	string tmp1 = cf.Value("input", "input_file"); 
	strcpy(img_name, tmp1.c_str());
	string tmp2 = cf.Value("output", "output_file");
	strcpy(output_name, tmp2.c_str());
	tmp2 = cf.Value("output", "output_log");
	strcpy(output_log, tmp2.c_str());
	
	tmp2 = cf.Value("output", "ROI_raw_file");
	strcpy(ROI_raw_file,  tmp2.c_str());
	tmp2 = cf.Value("gaze", "current_eye_file");
	strcpy(current_eye_file,  tmp2.c_str());

	fp_ROI_raw = fopen(ROI_raw_file, "a+");
	if (!fp_ROI_raw) {
		printf("Cannot open %s\n", ROI_raw_file);
		return(-1);	
	}

	// char eye_tracker_cfg  = cf.Value("eye_tracker", "cfgfile");
	long eye_tracker_port = cf.Value("eye_tracker", "port");
	char eye_tracker_cfg[1024];
	string tmp3 = cf.Value("eye_tracker", "config_file");  // "./E6000.cfg";
	strcpy(eye_tracker_cfg, tmp3.c_str());

	SCALE = cf.Value("input", "scaling_input");
	show_ROI_image = cf.Value("output", "show_ROI_image");
	
	int gaze_mode = 0; 
	gaze_mode = cf.Value("gaze", "mode");
	double gaze_buffer_time = cf.Value("gaze", "eye_gaze_buffer_time");
	setGazeBufTime(gaze_buffer_time);

	raw_ROI_size[0] = cf.Value("ROI", "width");
	raw_ROI_size[1] = cf.Value("ROI", "height");
	
	bool run_forever= true;
//	run_forever = cf.Value("control", "run_forever");
//	int max_repeat_number = 1;
//	max_repeat_number = cf.Value("control", "max_repeat_number");

	show_timing = cf.Value("output", "show_timing");
	print_current_attention = cf.Value("output", "print_current_attention");
	print_on_screen = cf.Value("output", "print_on_screen");
	
	OUTPUT_INTERMEDIATE_RESULTS = cf.Value("output", "output_intermediate_results");
	int sleep_time = cf.Value("control", "sleep_time");
	show_eyegaze_on_whole_image = cf.Value("output", "show_eyegaze_on_whole_image");
	output_frame_number = cf.Value("output", "output_frame_number");
	use_frame_number = cf.Value("output", "use_frame_number");
	output_gaze_duration = cf.Value("output", "output_gaze_duration");

/*	history_target = new int[output_frame_number];  
	history_output_s = new double[output_frame_number * (OBJ_NUM+HEAD_NUM)]; 
	history_output_p = new double[output_frame_number * (OBJ_NUM+HEAD_NUM)];
	for (int i=0; i<output_frame_number*(OBJ_NUM+HEAD_NUM); i++) {  // set default values
		history_output_s[i] = OBJ_NOT_THERE;
		history_output_p[i] = 0;
	}
*/
 

 //   char img_name[] = "test.png";
 //   char output_name[] = "test_out.jpg";
    
	clock_t begin, end;
	double  cost;

	if (flagEyeTrackerInited == false && gaze_mode!=0 && gaze_mode != 1 ) {
		if (InitEyeTrackerClient(eye_tracker_cfg, eye_tracker_port) != -1) {
			flagEyeTrackerInited = true;
		}
	}

//	printf("Start detecting which object the subject is looking at.\n");
//	printf("Press q when you want to finish the program. \n");

	if (show_eyegaze_on_whole_image){
		cvNamedWindow("img_with_eye_gaze", CV_WINDOW_AUTOSIZE); 
	}

	int img_count = 0;
	if (show_ROI_image) {
		// create a window
		cvNamedWindow("EyeGaze", CV_WINDOW_AUTOSIZE); 	
	}

	int ROI_size[2];
	ROI_size[0] = (int)((float)raw_ROI_size[0] * SCALE);
	ROI_size[1] = (int)((float)raw_ROI_size[1] * SCALE);

	IplImage * src;
    for (int i=1; run_forever; i++) {
		if (kbhit()) {
			char ch = getch();
			if (ch == 27) {
				goto when_exit;
			}
		}

		if (show_timing) {
			begin = clock();
		}

		src = cvLoadImage(img_name, CV_LOAD_IMAGE_COLOR );   
	    if(!src){
			//printf("Could not load image file: %s\n",img_name);
			Sleep(3);
			continue;
		} else {
			//printf(" loaded!!\n");
			img_count++;
	//		printf("%d\n", img_count);
		}

		raw_img_height = src->height;
		raw_img_width  = src->width;

		int eye_gaze[2]; // eye_gaze[0]: x;  eye_gaze[1]: y; 
		int raw_eye_gaze[2]; // raw_eye_gaze[0]: x;  raw_eye_gaze[1]: y; 

		long t_start_reading_gaze, t_end_reading_gaze;
		if (show_timing) {
			t_start_reading_gaze = clock();
		}
		getEyeGaze(eye_gaze, raw_eye_gaze, gaze_mode);		
		if (show_timing) {
			t_end_reading_gaze = clock();
			printf("Time used in reading eye data: %d ms\n", t_end_reading_gaze - t_start_reading_gaze);
		}

	 	//  Resize the image
		CvSize scaled_cvsize; 
		scaled_cvsize.width = (int) ((float)src->width * SCALE);		
		scaled_cvsize.height = (int) ((float)(src->height) * SCALE);
		IplImage * scaled_src = cvCreateImage( scaled_cvsize, src->depth, src->nChannels);	
		cvResize(src, scaled_src, CV_INTER_LINEAR);	

		int return_value = process_eye_tracking_image(scaled_src, output_name, eye_gaze, raw_eye_gaze, ROI_size);
    
		cvReleaseImage(&scaled_src);
		cvReleaseImage(&src);

		if (return_value == 100)
			break;

		if (show_timing) {
			end = clock();
			cost = (double)(end - begin) / CLOCKS_PER_SEC;
			printf("%lf seconds per image\n", cost);
		}

/*		if (kbhit()) {
			if (getch()=='q') {
				printf("\nYou pressed q. Do you want to quit the program? (y/n) \n");
				if (getch() == 'y')
					break;
				else
					printf("Program resumed\n");
			}
		}
*/
		Sleep(sleep_time);
    }
 

when_exit:
	fclose(fp_ROI_raw);

	if (show_ROI_image) {
		cvDestroyWindow("EyeGaze");
	}

	if (show_eyegaze_on_whole_image) {
		cvDestroyWindow("img_with_eye_gaze"); 
	}

/*	free(history_target);
	free(history_output_s);
	free(history_output_p);
*/


    return 0;
}



int process_eye_tracking_image(IplImage * src, char * output_name, int * eye_gaze, int * raw_eye_gaze, int * ROI_size)  // put rectangles on the objects and the head. 
{
    IplImage *dst = 0;		

	if (show_eyegaze_on_whole_image){
		show_image_with_eye_gaze("img_with_eye_gaze", src, eye_gaze);
		if (cvWaitKey(1) == 'q')
            return 100; 
	}

	if (eye_gaze[0] < 0 || eye_gaze[1] < 0 || eye_gaze[0] > (src->width-1) || eye_gaze[1] > (src->height -1) ) {
		GenOutput(output_name, (IplImage **)0, (IplImage **)0, eye_gaze, raw_eye_gaze, ROI_size, 0, 1.0);
	    if (print_on_screen) {
			GenOutput("stdout", (IplImage **)0, (IplImage **)0, eye_gaze, raw_eye_gaze, ROI_size, 0, 1.0);
		}
		return -2;
	}

    CvSize dst_cvsize;			

    // cut the ROI 
    CvRect rect;
    rect.x = floor(double(eye_gaze[0] - ROI_size[0]/2));
    rect.y = floor(double(eye_gaze[1] - ROI_size[1]/2));
	int x2 = rect.x + ROI_size[0] - 1;
	int y2 = rect.y + ROI_size[1] - 1;

	if (rect.x < 0) 
		rect.x = 0;
	if (rect.y < 0)
		rect.y = 0;
	if (x2 >= src->width)
		x2 = src->width - 1;;
	if (y2 >= src->height)
		y2 = src->height - 1;;

	rect.width  = x2 - rect.x + 1;
	rect.height = y2 - rect.y + 1;

	if (rect.width <= 0 || rect.height <= 0)  {
		GenOutput(output_name, (IplImage **)0, (IplImage **)0, eye_gaze, raw_eye_gaze, ROI_size, 0, 1.0);
	    if (print_on_screen) {
	 		GenOutput("stdout", (IplImage **)0, (IplImage **)0, eye_gaze, raw_eye_gaze, ROI_size, 0, 1.0);
		}
		return -2;
	}
		
    IplImage * imgRect;
    CvSize rect_cvsize;
    rect_cvsize.width = rect.width;
    rect_cvsize.height = rect.height;			
    imgRect = cvCreateImage(rect_cvsize, src->depth, src->nChannels);    
    GetImageRect(src, rect, imgRect);

 /*   //  Resize the image
    dst_cvsize.width = imgRect->width * SCALE;		
    dst_cvsize.height = (int) ((float)(imgRect->height) * SCALE);
    dst = cvCreateImage( dst_cvsize, imgRect->depth, imgRect->nChannels);	
    cvResize(imgRect, dst, CV_INTER_LINEAR);	

    dst_cvsize.width = src->width * SCALE;		
    dst_cvsize.height = (int) ((float)(src->height) * SCALE);
    IplImage * resized_whole = cvCreateImage( dst_cvsize, src->depth, src->nChannels);	
    cvResize(src, resized_whole, CV_INTER_LINEAR);	
	

	int sub_ROI_size[2];
	sub_ROI_size[0] = ROI_size[0] * SCALE;
	sub_ROI_size[1] = ROI_size[1] * SCALE;
	

    IplImage * result_img = detection(resized_whole, dst, output_name, eye_gaze, ROI_size);   */

    IplImage * result_img = detection(src, imgRect, output_name, eye_gaze, raw_eye_gaze, ROI_size);  
    
	if (show_ROI_image) {
		cvShowImage("EyeGaze", result_img);  	 // show the image
		cvWaitKey(1);
	}

  //  cvSaveImage("test_out.jpg", result_img); 

    cvReleaseImage(&dst);
    cvReleaseImage(&imgRect);
//    cvReleaseImage(&resized_whole);
    cvReleaseImage(&result_img);
    return 1; 
}

void show_image_with_eye_gaze(char * img_window_name, IplImage * img, int * input_eye_gaze)
{
	int line_length = 5;

	int eye_gaze[2];
	eye_gaze[0] = input_eye_gaze[0]; // * SCALE;
	eye_gaze[1] = input_eye_gaze[1]; //  * SCALE;

	IplImage * img_with_eye_gaze = cvCreateImage(cvSize(img->width, img->height), img->depth, img->nChannels);
	cvCopy(img, img_with_eye_gaze, NULL);

	if (eye_gaze[0] >=0 && eye_gaze[1] >=0 && eye_gaze[0] < img->width && eye_gaze[1] < img->height) {
		int x0, x1, y0, y1, x, y;
		x = eye_gaze[0];
		y = eye_gaze[1];
		x0 = max(0, x-line_length);
		x1 = min(img->width-1, x+line_length);
		y0 = max(0, y-line_length);
		y1 = min(img->height-1, y+line_length);

		cvLine(img_with_eye_gaze, cvPoint(x0, y),   cvPoint(x1, y),  cvScalar(0,0,255), 1);
		cvLine(img_with_eye_gaze, cvPoint(x0, y0),  cvPoint(x1, y1), cvScalar(0,0,255), 1);
		cvLine(img_with_eye_gaze, cvPoint(x,  y0),  cvPoint(x,  y1), cvScalar(0,0,255), 1);
		cvLine(img_with_eye_gaze, cvPoint(x1, y0),  cvPoint(x0, y1), cvScalar(0,0,255), 1);
	}

	cvShowImage(img_window_name, img_with_eye_gaze);  	 // show the image
	cvReleaseImage(&img_with_eye_gaze);
}

IplImage * detection(IplImage * whole_img, IplImage* ROI_img, char * output_name, int * eye_gaze, int * raw_eye_gaze, int * ROI_size)
{
	static int call_count; 
	static int old_x[OBJ_NUM],  old_y[OBJ_NUM];

	if (call_count == 0) {
		for (int i=0; i<OBJ_NUM; i++) {
			old_x[i] = OBJ_NOT_THERE;
			old_y[i] = OBJ_NOT_THERE;
		}
		call_count = 1;
	}

	IplImage ** objects = detect_color_object(ROI_img);

	int edges[2] = {15,15};
	IplImage ** nao_heads = (IplImage **) operator new (sizeof(IplImage*) * HEAD_NUM); 
	for (int i=0; i<HEAD_NUM; i++) {
		nao_heads[i] = detect_nao_head(whole_img, edges, eye_gaze, ROI_size, i);
	}

    int blob_size_threshold = (int)(50 * SCALE * SCALE); // hard coding, asuming the raw image is 320*240.

	GenOutput(output_name, objects, nao_heads, eye_gaze, raw_eye_gaze, ROI_size, blob_size_threshold,SCALE);
    if (print_on_screen) {
		GenOutput("stdout", objects, nao_heads, eye_gaze, raw_eye_gaze, ROI_size, blob_size_threshold,SCALE);
	}

    // draw rectangle on image
    IplImage * outputImg  = cvCloneImage(ROI_img);

	// HARD CODING !!
	CvScalar cv_color_obj[5];
	cv_color_obj[0] = CV_RGB(0,0,255);
	cv_color_obj[1] = CV_RGB(0,255,0);
	cv_color_obj[2] = CV_RGB(255,0,255);
	cv_color_obj[3] = CV_RGB(255,0,0);
	cv_color_obj[4] = CV_RGB(255,255,0);

	CvScalar cv_color_head[2];
	cv_color_head[0] = CV_RGB(255,255,255);
	cv_color_head[1] = CV_RGB(150,150,150);

	for (int i=0; i < OBJ_NUM; i++) {
		int rect_num;
		CvRect * boxes = detect_blob(objects[i], 2.5*blob_size_threshold, &rect_num); // 2.5, hard coding
		DrawRect(outputImg, boxes, rect_num, cv_color_obj[i]); 
		delete [] boxes;
	}

	for (int i=0;i < HEAD_NUM;i++) {
		int rect_num;
		CvRect * boxes = detect_blob(nao_heads[i], blob_size_threshold, &rect_num);           
		DrawRect(outputImg, boxes, rect_num, cv_color_head[i]); 
		delete [] boxes;
	}

	for (int i=0; i < OBJ_NUM ; i++) 
		cvReleaseImage(&(objects[i]));

	for (int i=0; i < HEAD_NUM; i++)
		cvReleaseImage(&(nao_heads[i]));
	
	delete [] objects;
	delete [] nao_heads;

    return outputImg;
}


// tmp modi
int ObjIDMapping(int id)
{
	int mapping[7] = {0,1,2,3,4,5,6};
	return mapping[id];
}

void GenOutput(char * output_name, IplImage ** objs, IplImage ** heads, 
			   int * eye_gaze, int *raw_eye_gaze, int * ROI_size, 
			   int blob_size_threshold, double img_scale)
{
	double p[OBJ_NUM+HEAD_NUM], s[OBJ_NUM+HEAD_NUM];
	int target = OBJ_NOT_THERE;

	bool flag = !(!objs || !heads);
	for (int i=0; flag && i < OBJ_NUM; i++ ) { 
		if (!objs[i]) 
			flag = false;
	}
	for (int i=0; flag && i < HEAD_NUM; i++ ) { 
		if (!heads[i]) 
			flag = false;
	}		
	 
	if (!flag) {
		for (int i=0; i<OBJ_NUM+HEAD_NUM; i++ ) {
			s[i] = OBJ_NOT_THERE; 
			p[i] = 0;
		}
	} else{
		CBlobResult * blobs = new CBlobResult[OBJ_NUM+HEAD_NUM];
		for (int i=0; i < OBJ_NUM; i++) {
			blobs[i] = CBlobResult(objs[i],  (IplImage*)0, 0);  	// find blobs in thresholded image
		}

		for (int i=OBJ_NUM; i<(OBJ_NUM+HEAD_NUM); i++)
			blobs[i] = CBlobResult(heads[i-OBJ_NUM],  (IplImage*)0, 0);  	// find blobs in thresholded image

		for (int i=0; i<(OBJ_NUM+HEAD_NUM); i++) { 
			blobs[i].Filter( blobs[i], B_EXCLUDE, CBlobGetArea(), B_LESS, blob_size_threshold ); 	// exclude the ones smaller than blob_size_threshold
		}

		double total_area = ROI_size[0] * ROI_size[1];

		for (int i=0; i< (OBJ_NUM+HEAD_NUM); i++) {
			p[i] = get_blobs_size(blobs[i]) / total_area;
		}
		
		int ROI_center[2];
		if (eye_gaze[0] > ROI_size[0]/2.0) {
			ROI_center[0] = (int)(floor((double)ROI_size[0]/2.0));   
		} else {
			ROI_center[0] = eye_gaze[0];
		}

		if (eye_gaze[1] > ROI_size[1]/2.0) {
			ROI_center[1] = (int)(floor((double)ROI_size[1]/2.0));
		} else {
			ROI_center[1] = eye_gaze[1];
		}		

		for (int i=0; i<OBJ_NUM; i++) { 		
			s[i] = get_min_distace_to_blob(ROI_center, objs[i],  blobs[i]); 
			if (s[i] > 0)
				s[i] = s[i] / img_scale;
		}

		for (int i=0; i<HEAD_NUM; i++) { 		
			s[i+OBJ_NUM] = get_min_distace_to_blob(ROI_center, heads[i],  blobs[i+OBJ_NUM]); 
			if (s[i+OBJ_NUM] > 0)
				s[i+OBJ_NUM] = s[i+OBJ_NUM] / img_scale;
		}

		// decide which object/head the subject is looking at
		double max_p = p[0];
		int max_idx = 0;
		for (int g=1; g<(OBJ_NUM+HEAD_NUM); g++) {
			if (p[g] > max_p) {
				max_p = p[g];
				max_idx =g;
			}
		}

		if (max_p > 0.01) {  // HARD CODING!!
			target = max_idx+1;
		} else {
			target = OBJ_NOT_THERE;
		}

		delete [] blobs;
	} // end of 'else'

	if (print_current_attention) {
		printf("Looking at %d\n", target);
	}

	// save ouput to history.  
	for (int i=HISOTRY_BUFFER_SIZE-1; i>0; i--) {
		time_stamps[i] = time_stamps[i-1];
		history_target[i] = history_target[i-1];
		for (int j=0; j<(OBJ_NUM+HEAD_NUM); j++) {
			history_output_s[i*(OBJ_NUM+HEAD_NUM)+j] = history_output_s[(i-1)*(OBJ_NUM+HEAD_NUM)+j]; 
			history_output_p[i*(OBJ_NUM+HEAD_NUM)+j] = history_output_p[(i-1)*(OBJ_NUM+HEAD_NUM)+j]; 
		}
	}

	time_stamps[0] = clock(); 
 
	SYSTEMTIME now;
	GetLocalTime(&now);
	double current_time = now.wHour * 3600.0 + now.wMinute * 60.0 + now.wSecond + now.wMilliseconds / 1000.0;

	history_target[0] = target;
	for (int i=0;i<(OBJ_NUM+HEAD_NUM); i++ ) {
		history_output_s[i] = s[i];
		history_output_p[i] = p[i];
	}

	int output_number = output_frame_number;
	if (!use_frame_number) {
		double begin_time = time_stamps[0] - ((double)output_gaze_duration * CLOCKS_PER_SEC / 1000); 
		for (int j=1; j<HISOTRY_BUFFER_SIZE; j++) { // hard coding
			if (time_stamps[j] < begin_time) {
				output_number = j; 
				break;
			}
		}
	} else {
		output_number = output_frame_number;
	}

	
	// output to file
	FILE * fp;

	if (strcmp(output_name, "stdout") == 0) {
		fp = stdout;
	} else {
		fp = fopen(output_name, "wt+");
		if (fp == 0) {
			printf("Error! Can't open file %s.\n", output_name);
			return;
		}
	}

	fprintf(fp, "[ROI_Size]\nWidth = %d\nHeight = %d\n\n", raw_ROI_size[0], raw_ROI_size[1]);
	fprintf(fp, "[Eye_Gaze]\nX = %d\nY = %d\n\n", eye_gaze[0], eye_gaze[1]); 
	fprintf(fp, "[Raw_Eye_Gaze]\nX = %d\nY = %d\n\n", raw_eye_gaze[0], raw_eye_gaze[1]); 

	fprintf(fp, "[TimeStamp]\n");
	fprintf(fp, "time = ");
	for (int i=0; i<output_number; i++) {
		fprintf(fp, "%lf ", (double)time_stamps[i] * 1000 / CLOCKS_PER_SEC);
	}
	fprintf(fp, "\n\n");

	fprintf(fp, "[Target]\n");
	fprintf(fp, "attention = ");
      
	for (int i=0; i < output_number; i++) {
		fprintf(fp, "%d", ObjIDMapping(history_target[i]));   // NOTE: tmp modi !!!
	}
	fprintf(fp, "\n\n");

	fprintf(fp, "[Distance]\n");
	for (int ob=0; ob<OBJ_NUM; ob++) {
		fprintf(fp, "object%d =", ob+1);
		for (int i=0; i < output_number; i++) {
			fprintf(fp, " %lf",   history_output_s[i*(OBJ_NUM+HEAD_NUM)+ob]);
		}
		fprintf(fp, "\n");
	}

	for (int h=0; h<HEAD_NUM; h++) {
		fprintf(fp, "nao_head%d =", h+1);
		for (int i=0; i < output_number; i++) {
			fprintf(fp, " %lf",   history_output_s[i*(OBJ_NUM+HEAD_NUM)+ OBJ_NUM + h]);
		}
		fprintf(fp, "\n");
	}

	fprintf(fp, "\n[Proportion]\n");
	
	for (int ob = 0; ob < OBJ_NUM; ob++) {
		fprintf(fp, "object%d =", ob+1);
		for (int i=0; i < output_number; i++) {
			fprintf(fp, " %lf",   history_output_p[i*(OBJ_NUM+HEAD_NUM)+ob]);
		}
		fprintf(fp, "\n");
	}

	for (int h=0; h < HEAD_NUM; h++) {
		fprintf(fp, "nao_head%d =", h+1);
		for (int i=0; i < output_number; i++) {
			fprintf(fp, " %lf",   history_output_p[i*(OBJ_NUM+HEAD_NUM)+ OBJ_NUM + h]);
		}
		fprintf(fp, "\n");
	}

	if (strcmp(output_name, "stdout") != 0) {
	    fclose(fp);
	}

	// record data
	fprintf(fp_ROI_raw, "%lf %d\n", (double)current_time, target);

	// output to log file
	FILE * fp_log;
	fp_log = fopen(output_log, "at+");
	if (fp_log == 0) {
	//	printf("Error! Can't open log file %s.\n", output_log);
		return;
	}

	fprintf(fp_log, "%lf %d ", (double)current_time, target);

	for (int i=0; i < OBJ_NUM + HEAD_NUM; i++) {
		fprintf(fp_log, "%lf\n", p[i]);
	}
	for (int i=0; i < OBJ_NUM + HEAD_NUM; i++) {
		fprintf(fp_log, "%lf\n", s[i]);
	}
	fprintf(fp_log, "\n");

	fclose(fp_log);
}



void DrawRect(IplImage * img, CvRect * rect, int rectNum, CvScalar color) 
{
	for (int k=0; k<rectNum; k++) {
		CvPoint pt1, pt2;
		CvRect r = rect[k];
	    pt1.x = r.x;
		pt2.x = (r.x+r.width);
		pt1.y = r.y;
		pt2.y = (r.y+r.height);
		cvRectangle(img, pt1, pt2, color, 3, 8, 0);
	}
}

