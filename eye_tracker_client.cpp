// obj_detection_eye_gaze.cpp : Defines the entry point for the console application.
//

#include <afxwin.h>
// #include <winsock.h>

#include <stdlib.h>
#include <iostream>

using namespace std;

#include "stdafx.h"
//#include "cstring.h"

// ASL COM Server
#include "ASLSerialOutLib2.h"
#include "EventHandler.h"

#include <stdio.h>
#include <time.h>

IASLSerialOutPort3* gpISerialOutPort = NULL;
CEventHandler* m_pHandler = NULL;
// CWnd* m_pMain;
FILE* fp;
int   isRecalled = 0;
extern bool print_on_screen;
extern int   raw_img_width;
extern int   raw_img_height;
extern float SCALE; 
extern char current_eye_file[1024];


int gazeBufSize = 100; // 100 should be large enough
double gazeBuffTime = 0.1;  // default value
int * eyeGazeXBuf = NULL;
int * eyeGazeYBuf = NULL;
int current_raw_eye_x; 
int current_raw_eye_y; 

clock_t * timeStamps = NULL;


void read_current_eye_data(int * eye_gaze, clock_t * time_stamp);
int InitEyeTrackerClient(char *, int);
void getETData(char * strToSend, int * returnSize);
void updateETData();
void getEyeGaze(int * eye_gaze, int mode);
void setGazeBufSize(int bufSize);
void setGazeBufTime(double seconds);
void get_eye_data_in_range(clock_t start_time, clock_t end_time, int * eye_gaze);
void push_eye_data_array(int eye_gaze[], clock_t t);
int median(int * data, int array_size);
void convert_eye_data(int raw_eye_x, int raw_eye_y, double * eye_x, double * eye_y);


void setGazeBufTime(double seconds)
{
	gazeBuffTime = seconds; 
}

void setGazeBufSize(int bufSize)
{
	gazeBufSize = bufSize; 
	
	if (eyeGazeXBuf)
		free(eyeGazeXBuf);

	if (eyeGazeYBuf)
		free(eyeGazeYBuf);

	if (timeStamps)
		free(timeStamps);

	eyeGazeXBuf = new int[gazeBufSize];
	eyeGazeYBuf = new int[gazeBufSize];
	timeStamps  = new clock_t[gazeBufSize];

	for (int i=0; i<gazeBufSize; i++) {
		eyeGazeXBuf[i] = -1;
		eyeGazeYBuf[i] = -1;
		timeStamps[i] = -100000;
	}
}

void getETData(char * strToSend, int * returnSize)
{
	// get a data item from the eye tracker
    long count;
	VARIANT_BOOL bAvailable;	
	int retSize = 0;
	LPSAFEARRAY items;

	HRESULT hr = gpISerialOutPort->GetScaledData(&items, &count, &bAvailable);
	if (FAILED(hr))
	{
		CComBSTR bsError;
		gpISerialOutPort->GetLastError(&bsError);
		if (print_on_screen)
			printf("-%s- \n", bsError);
	}
		
	// Sleep(1000);
	// Delay that prevents memory leaks(???)
	if (print_on_screen)
		printf("printing data:%d--\n", bAvailable);
	//for(int i = 0; i < 200; i++)			
		//printf("_\n");
	
	CComVariant value;	
	if (bAvailable != VARIANT_FALSE) {
		//printf("count: %d ", count);
		for (long i = 0; i < count; i++)
		{			
			SafeArrayGetElement(items, &i, &value);
			value.ChangeType(VT_BSTR);
			CString str = value.bstrVal;
			VariantClear(&value);	
			char* s = str.GetBuffer(0);	
			if (print_on_screen)
				printf("added segment: %s ", s);

			if(i == 3) {
				strcpy(strToSend, s);
				strcat(strToSend, " ");
				retSize += str.GetLength() + 1;
			}
			if (i == 4) {
/*				SYSTEMTIME st;	
				char timeString[14];
				GetLocalTime(&st);
				sprintf(timeString, " %02d-%02d-%02d-%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
				retSize += 14;
*/
				strcat(strToSend, s);
//				strcat(strToSend, timeString);
				strcat(strToSend, "\n");
				retSize += str.GetLength() + 1;
				
			}
		}
		if (print_on_screen)
			printf("\n");
	} else {
		*returnSize = retSize;
		if (print_on_screen)
			printf("return size: %d \n", *returnSize);
		return;
	}
	*returnSize = retSize;
	if (print_on_screen) {
		printf("return size: %d \n", *returnSize);
		printf("result: %s \n", strToSend);
	}
	SafeArrayDestroy(items);
}

void updateETData()
{
	int eye_gaze[2];
	clock_t t;

	read_current_eye_data(eye_gaze, &t);

	push_eye_data_array(eye_gaze, t);
}

void push_eye_data_array(int eye_gaze[], clock_t t)
{
	for (int i = gazeBufSize-2; i>=0; i--) {
		eyeGazeXBuf[i+1] = eyeGazeXBuf[i];
		eyeGazeYBuf[i+1] = eyeGazeYBuf[i];
		timeStamps[i+1] = timeStamps[i];
	}

	eyeGazeXBuf[0] = eye_gaze[0];
	eyeGazeYBuf[0] = eye_gaze[1];
	timeStamps[0]  = t;
}

void getEyeGaze(int * eye_gaze, int * raw_eye_gaze, int mode)
// 	mode:   0: return the center of image. 1: random point on the imag;  2: get value from eye-tracker.
{
	if (mode == 0) { // testing
		eye_gaze[0] = (int)(raw_img_width*SCALE/2);
		eye_gaze[1] = (int)(raw_img_height*SCALE/2);
		raw_eye_gaze[0] = eye_gaze[0];
		raw_eye_gaze[1] = eye_gaze[1];
	} else if(mode == 1) { // testing
		int r1 = rand();
		int r2 = rand();

  		eye_gaze[0] = int(double(r1)/double(32768) * (raw_img_width*SCALE+10)) - 5; 
		eye_gaze[1] = int(double(r2)/double(32768) * (raw_img_height*SCALE+10)) - 5;
		raw_eye_gaze[0] = eye_gaze[0];
		raw_eye_gaze[1] = eye_gaze[1];
	} else if (mode == 2) {
		clock_t current_time = clock();
		clock_t start_time = current_time -  gazeBuffTime * CLOCKS_PER_SEC ;

		updateETData();

		get_eye_data_in_range(start_time, current_time, eye_gaze);

		eye_gaze[0] = eye_gaze[0] * SCALE;
		eye_gaze[1] = eye_gaze[1] * SCALE;

		raw_eye_gaze[0] = current_raw_eye_x;
		raw_eye_gaze[1] = current_raw_eye_y;
	} else if (mode == 3) {
		clock_t t; 
		read_current_eye_data(eye_gaze, &t);

		eye_gaze[0] = eye_gaze[0] * SCALE;
		eye_gaze[1] = eye_gaze[1] * SCALE;

		raw_eye_gaze[0] = current_raw_eye_x;
		raw_eye_gaze[1] = current_raw_eye_y;
	} else if (mode == 4) {
		FILE * fp_eye_data = fopen(current_eye_file, "r");
		if (!fp_eye_data) {
			printf("Cannot open %s\n", current_eye_file);
			eye_gaze[0] = -1;
			eye_gaze[1] = -1;
			return;
		}

		double time_stamp, eye_x, eye_y;

	    fscanf(fp_eye_data, "%lf %d %d",&time_stamp, raw_eye_gaze+0, raw_eye_gaze+1); 
		current_raw_eye_x = raw_eye_gaze[0];
		current_raw_eye_y = raw_eye_gaze[1];
		convert_eye_data(raw_eye_gaze[0], raw_eye_gaze[1], &eye_x, &eye_y);

		eye_x = eye_x * (raw_img_width/720.0);  // convert to [1 raw_img_width]
		eye_y = eye_y * (raw_img_height/480.0);  // convert to [1 raw_img_height]

		eye_gaze[0] = int(eye_x);
		eye_gaze[1] = int(eye_y);

		fclose(fp_eye_data);
	}

	if (print_on_screen) {
//		printf("eye_gaze: x=%d, y=%d\n", eye_gaze[0], eye_gaze[1]);
		printf("raw eye_gaze: x=%d, y=%d\n", raw_eye_gaze[0], raw_eye_gaze[1]);
	}
}

void get_eye_data_in_range(clock_t start_time, clock_t end_time, int * eye_gaze)
{ // use median eye data for now. 
int eye_x[1024];
int eye_y[1024];

int k = 0;
for (int i = 0; i < gazeBufSize; i++) {
	if (timeStamps[i] >= start_time && timeStamps[i] <= end_time) {
		eye_x[k] = eyeGazeXBuf[i];
		eye_y[k] = eyeGazeYBuf[i];
		k++;
	}
}

eye_gaze[0] = median(eye_x, k);
eye_gaze[1] = median(eye_y, k);
}




void sort(int * data, int n)
{ // bubble sort
	for (int i=0; i < n; i++)
		for (int j = n-1; j>i; j--)
			if (data[j-1] > data[j]) 
				swap(data[j-1], data[j]);
}
 
void swap(int& v1, int& v2)
{
	int temp;
	temp = v1;
	v1 = v2;
	v2 = temp;
}
 
int median(int * data, int array_size)
{
sort(data, array_size);	
int m = data[ array_size / 2 ];
return m;
}

void convert_eye_data(int raw_eye_x, int raw_eye_y, double * eye_x, double * eye_y)
{
	// convert eye data to the coordinates of image. 
	*eye_x = (int) (67 + 2.4 * raw_eye_x);  // convert to [1 720]
	*eye_y = 2 * raw_eye_y;         // convert to [1 480]
}

void read_current_eye_data(int * eye_gaze, clock_t * time_stamp)
{
	char data[512];
	int size;
	
	getETData(data, &size);

	*time_stamp = clock();
	double eye_x, eye_y;
	sscanf(data, "%lf %lf", &eye_x, &eye_y);
	current_raw_eye_x = eye_x; 
	current_raw_eye_y = eye_y; 

	// convert eye data to the coordinates of image. 
    convert_eye_data(current_raw_eye_x, current_raw_eye_y, &eye_x, &eye_y);

	eye_x = eye_x * (raw_img_width/720.0);  // convert to [1 raw_img_width]
	eye_y = eye_y * (raw_img_height/480.0);  // convert to [1 raw_img_height]

	eye_gaze[0] = int(eye_x);
	eye_gaze[1] = int(eye_y);
}

int InitEyeTrackerClient(char * cfgfile, int port)
{
	printf("Client for ADE server that interfaces eye-tracker box\n");

	CoInitialize(NULL);
//	_Module.Init(NULL, NULL);
//	AfxEnableControlContainer();
	HRESULT hr = CoCreateInstance(CLSID_ASLSerialOutPort3, NULL, CLSCTX_INPROC_SERVER,
								IID_IASLSerialOutPort3, (void**)&gpISerialOutPort);
	if (FAILED(hr)) { 
		printf("failed:%d\n", hr);
		return -1;
	}
	
	int ret_value = -1;

	long baudRate, updateRate, itemCount;
	VARIANT_BOOL streamingMode;
	LPSAFEARRAY itemNames;
	//CString filename("C:/Eye Tracker 6000/EyeTracking/E6000.cfg");

	// NOTE!
//	CComBSTR bstrFile("./ETSerialPortViewer115200.cfg"); 
	CComBSTR bstrFile(cfgfile); 
//	CComBSTR bstrFile("C:/Program Files/ASL Eye Tracker 6000/EyeTracking/E6000.cfg"); 
//	CComBSTR bstrFile(filename);
//	CComBSTR bstrFile("C:/Program Files/ASL Eye Tracker 6000/SDK/VisualCPP/SerialOutClient/Release/ETSerialPortViewer115200.cfg"); 

	printf("Port: %d BstrFile: %s\n", port, bstrFile);

	hr = gpISerialOutPort->Connect(bstrFile, port, VARIANT_FALSE, &baudRate, &updateRate, &streamingMode, &itemCount, &itemNames);
	
	
	// update user interface
	if (SUCCEEDED(hr))
	{
		printf("get connected\n");
	    // Display values
		printf("baudrate:%d; updaterate:%d, streamingmode:%d; itemCount:%d\n", baudRate, updateRate, streamingMode, itemCount);
		//EnableDisable(true /*connected*/, false /*continuous reading*/);
		ret_value = 1;
	}
	else
	{
		printf("ETSerialPortViewer - FAILED TO CONNECT\n");
		CComBSTR bsError;
		gpISerialOutPort->GetLastError(&bsError);
		printf("%s \n", bsError);
		ret_value = -1;
	}

	Sleep(1000);
	SafeArrayDestroy(itemNames);

	setGazeBufSize(gazeBufSize);  // init gaze buffer
	return ret_value;
}		

/*
//Main Loop
		SYSTEMTIME st;	
		char fileName[50];
		GetLocalTime(&st);
		sprintf(fileName, "Eye-%02d-%02d-%02d-%02d.txt", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		//sprintf( fileName,  "C:/multisensory/raw_data_eye_temp/Test.txt");
		if ((fp = fopen(fileName, "at")) ==  NULL)
		{
			fprintf(stderr,"could not open the file \n");
			exit(-1);
		} else {
			fprintf(fp, "Timestamp\t\tX\tY\n");
		}
		while(1){

			char * data = new char [50];
			int size;
			getETData(data, &size);
		
			CComVariant value;	
			if (size != 0) {

				SYSTEMTIME st;
				GetLocalTime(&st);
		
				if (st.wMilliseconds < 100) {
					double time = (st.wHour*3600)+(st.wMinute*60)+st.wSecond;
					fprintf(fp, "%g.0%02d\t", time, st.wMilliseconds);
				} else {
					double time = (st.wHour*3600)+(st.wMinute*60)+st.wSecond;			
					fprintf(fp, "%g.%02d\t", time, st.wMilliseconds);
				}
		
				fprintf(fp, "%s", data);
			}
			delete [] data;
		}
		fclose(fp);
	return 0;
}

*/