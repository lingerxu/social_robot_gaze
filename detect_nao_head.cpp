#include "ExperimentDefine.h"
#include "cv_util.h"
#include "blob_util.h"

CvPoint * filter_pairs(CBlobResult blobs, CvSize img_size, int edge_widths[]);
IplImage * generate_head_image(CvPoint * eyes, CvSize img_size, int *human_eye_gaze, int *ROI_size );
IplImage * cut_roi(IplImage * src, int * eye_gaze, int * ROI_size);
CBlobResult * search_marker(IplImage * V, CvSize img_size, int edge_widths[]);
IplImage * generate_head_image_by_marker(CBlobResult blobs, CvSize img_size, int * human_eye_gaze, int * ROI_size);

extern bool  OUTPUT_INTERMEDIATE_RESULTS;

#define SQUARE(x) (x)*(x)

IplImage * detect_nao_head(IplImage * img, int edge_widths[], int * human_eye_gaze, int * ROI_size, int marker_color)
{
	 IplImage * head_image;

     // convert image to HSV
     IplImage * hsv = bgr2hsv(img);        
     IplImage * H, * S, * V;
     H = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_32F, 1);
     S = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_32F, 1);
     V = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_32F, 1);  
     cvSplit(hsv, H, S, V, 0);
    
     IplImage * H_B, * S_B, *V_B, *head_marker;
	 H_B = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_8U, 1 );
	 S_B = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_8U, 1 );
	 V_B = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_8U, 1 );
	 head_marker = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_8U, 1 );

	 // detect the marker on Nao's head. 
	 if (marker_color == 0) { // black marker)
		 cvThreshold( V, head_marker, 0.14, 127, CV_THRESH_BINARY_INV );
	 } else if(marker_color == 1) { // yellow marker 
		 // yellow_marker = (h > 0.13) & (h < 0.22) & (s>0.8) & (v>0.2); 
   		 cvThreshold( H, head_marker, 0.13*360, 127, CV_THRESH_BINARY );
   		 cvThreshold( H, H_B,  0.22*360, 127, CV_THRESH_BINARY_INV );  	 
		 cvAnd(head_marker, H_B, head_marker);
   		 cvThreshold( S, S_B,  0.8, 127, CV_THRESH_BINARY );  	   	 
		 cvAnd(head_marker, S_B, head_marker);
   		 cvThreshold( V, V_B,  0.2,  127, CV_THRESH_BINARY );  	   	 
		 cvAnd(head_marker, V_B, head_marker);
	 }

	 if (OUTPUT_INTERMEDIATE_RESULTS) {
		 char tmp_str[256];
		 sprintf(tmp_str, "head_marker_raw_%d.jpg", marker_color);
		 cvSaveImage(tmp_str, head_marker);
	 }
 	 cvSmooth(head_marker, head_marker,  CV_MEDIAN , 5, 5); 
	 if (OUTPUT_INTERMEDIATE_RESULTS) {
		 char tmp_str[256];
		 sprintf(tmp_str, "head_marker_%d.jpg", marker_color);
	     cvSaveImage(tmp_str, head_marker);
	 }

	 CBlobResult * head_marker_blob = search_marker(head_marker, cvGetSize(hsv), edge_widths);
    
	 head_image = generate_head_image_by_marker(*head_marker_blob, cvGetSize(hsv), human_eye_gaze, ROI_size);
	 if (OUTPUT_INTERMEDIATE_RESULTS) {
 		 char tmp_str[256];
		 sprintf(tmp_str, "filtered_head_marker_%d.jpg", marker_color);
		 cvSaveImage(tmp_str, head_image);
	 }

	 cvReleaseImage(&hsv);
	 cvReleaseImage(&H);
	 cvReleaseImage(&S);
	 cvReleaseImage(&V);
	 cvReleaseImage(&H_B);
	 cvReleaseImage(&S_B);
	 cvReleaseImage(&V_B);
	 cvReleaseImage(&head_marker);
	 delete head_marker_blob;

	 return head_image;
}

CBlobResult * search_marker(IplImage * V, CvSize img_size, int edge_widths[])
{

	// parameters
	int test_img_width  = 320;
	int test_img_height = 240;
	int test_img_area = test_img_width * test_img_height;
	int img_area = img_size.height * img_size.width;

	int region_size_upper_threshold = floor(float(750) * img_area / test_img_area);
	int region_size_lower_threshold = floor(float(50) * img_area / test_img_area);

	CBlobResult blobs = CBlobResult(V, (IplImage*)0, 0);
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_LESS, region_size_upper_threshold);
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_GREATER, region_size_lower_threshold);

	int n = blobs.GetNumBlobs();

	CBlobResult * markers = new CBlobResult();
	int k = 0;
	for (int i =0; i<n; i++) {
		CBlob b = blobs.GetBlob(i);
		double avg_x =   ( b.MaxX() + b.MinX() ) /2; 
		double avg_y =   ( b.MaxY() + b.MinY() ) /2; 
		double box_width =   b.MaxX() - b.MinX(); 
		double box_height =  b.MaxY() - b.MinY(); 
		double blob_area = b.Area();
	//	if (b.Area() <= 10 * region_size_threshold && avg_x > edge_widths[0] && avg_x <= (img_size.width - edge_widths[1])) {
		if (avg_x > img_size.width * 0.06 && avg_x <= img_size.width * 0.94  && avg_y > img_size.height * 0.01 && avg_y < img_size.height * 0.65) {
			if (box_width/box_height > 0.35 && box_width/box_height < 3) {
				markers->AddBlob(&b);   ////   NOTE ??????? 
			}
		}
	}

	return markers;
}

CvPoint * filter_pairs(CBlobResult blobs, CvSize img_size, int edge_widths[])
{
CvPoint * eyes = new CvPoint[2];

// parameters
int test_img_width  = 360;
int test_img_height = 240;
int test_img_area = test_img_width * test_img_height;
int img_area = img_size.height * img_size.width;

int region_size_threshold = floor(float(6) * img_area / test_img_area);
int eye_distance_mean = floor(float(16) * img_area / test_img_area);
int eye_distance_std  = floor(float(4) * img_area / test_img_area);
int eye_y_mean = floor(float(75) * img_size.height / test_img_height);
int eye_y_std  = floor(float(25) * img_size.height / test_img_height);
int eye_x_mean = floor(float(180) * img_size.width / test_img_width);
int eye_x_std  = floor(float(120) * img_size.width / test_img_width);

/*
mask = [regions.Area] < region_size_threshold;
regions = regions(mask);

locs = vertcat(regions.Centroid);
*/

// blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 1);
blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_LESS, 1.5 * region_size_threshold);


int n = blobs.GetNumBlobs();
double * locs[2];
locs[0] = new double[n];
locs[1] = new double[n];

int k = 0;
for (int i =0; i<n; i++) {
	CBlob b = blobs.GetBlob(i);
	double avg_x =   ( b.MaxX() + b.MinX() ) /2; 
	double blob_area = b.Area();
//	if (b.Area() <= 10 * region_size_threshold && avg_x > edge_widths[0] && avg_x <= (img_size.width - edge_widths[1])) {
	if (avg_x > edge_widths[0] && avg_x <= (img_size.width - edge_widths[1])) {
		locs[0][k] = avg_x; 
		locs[1][k] = ( b.MaxY() + b.MinY() ) /2;
		k++;
	}
}

double min_y_dist_to_mean = img_size.height;
// find the pairs of eye candidates that have certain distances.
for (int i=0; i<k ; i++ ){
	for (int j=i+1; j<k ; j++) {
		double dist = sqrt(SQUARE(locs[0][i] - locs[0][j]) + SQUARE(locs[1][i]-locs[1][j]));
		double x_diff = abs(locs[0][i] - locs[0][j]);
		double y_diff = abs(locs[1][i] - locs[1][j]);
		if ( (dist < eye_distance_mean+2*eye_distance_std && dist > eye_distance_mean-2*eye_distance_std)  && x_diff > y_diff *1.5 ) {
			double eye_center_y = ( locs[1][i] + locs[1][j] ) / 2; 
			if (abs(eye_center_y-eye_y_mean) < min_y_dist_to_mean ) {
				min_y_dist_to_mean = abs(eye_center_y - eye_y_mean);
				eyes[0].x = locs[0][i];
				eyes[0].y = locs[1][i];
				eyes[1].x = locs[0][j];
				eyes[1].y = locs[1][j];
			}
		}
	}
}

if (min_y_dist_to_mean > 2.5 * eye_y_std) {
	free(eyes);
	eyes = (CvPoint *)0;
}

delete [] locs[0];
delete [] locs[1];

/*
distances = vector_distances_matrix(locs);
mask = distances < eye_distance_mean + 2*eye_distance_std & ...
    distances > eye_distance_mean - 2*eye_distance_std;
[a b] = find(mask);

x_diff = abs(locs(a,1) - locs(b,1));
y_diff = abs(locs(a,2) - locs(b,2));
mask = x_diff > y_diff * 1.5;
a = a(mask);
b = b(mask);

if isempty(a)
    eyes = [];
else
    eyes = [locs(a,:) locs(b,:)];
    eyes_y = (eyes(:,2) + eyes(:,4))/2;
    p = pdf('normal', eyes_y, eye_y_mean, eye_y_std);
    
    [max_p idx] = max(p);
    
    if max_p < 0.005
        eyes = [];
    else
        eyes = eyes(idx, :);
    end
end

*/
return eyes;
}

IplImage * generate_head_image(CvPoint * eyes, CvSize img_size, int * human_eye_gaze, int * ROI_size)
{
	IplImage * color_image = cvCreateImage(img_size, IPL_DEPTH_32F, 3);

	memset(color_image->imageData,0, color_image->imageSize);
	
	if (eyes != NULL) {
		CvPoint center;
		int radius = (int) sqrt((double)SQUARE(eyes[0].x-eyes[1].x) + SQUARE(eyes[0].y - eyes[1].y));
		CvScalar color = CV_RGB(255, 255, 255);
		center.x = (eyes[0].x + eyes[1].x)/2;
		center.y = (eyes[0].y + eyes[1].y)/2;
		cvCircle(color_image, center, radius, color, -1, 8, 0);
	}

	IplImage * whole_head_image = cvCreateImage(img_size, IPL_DEPTH_8U, 1);
	IplImage * hsv = bgr2hsv(color_image);

	 IplImage * H, * S, * V;
     H = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_32F, 1);
     S = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_32F, 1);
     V = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_32F, 1);  
     cvSplit(color_image, H, S, V, 0);

   	cvThreshold( V, whole_head_image, 3, 254, CV_THRESH_BINARY);

	if (OUTPUT_INTERMEDIATE_RESULTS) {
	    cvSaveImage("whole_head_image.jpg", whole_head_image); 
	}

    // cut the ROI 
	IplImage * head_image = cut_roi(whole_head_image, human_eye_gaze, ROI_size); 

	 cvReleaseImage(&color_image);
	 cvReleaseImage(&hsv);
	 cvReleaseImage(&H);
	 cvReleaseImage(&S);
	 cvReleaseImage(&V);
	 cvReleaseImage(&whole_head_image);

	return head_image;
}

IplImage * generate_head_image_by_marker(CBlobResult blobs, CvSize img_size, int * human_eye_gaze, int * ROI_size)
{
	IplImage * color_image = cvCreateImage(img_size, IPL_DEPTH_32F, 3);
	
	if (blobs.GetNumBlobs() > 0) {
		CvPoint center;
		CBlob b = blobs.GetBlob(0);

		int radius = (int)(0.9 * sqrt((double)SQUARE(b.MaxY()-b.MinY()) + SQUARE(b.MaxX() - b.MinX())));
		CvScalar color = CV_RGB(255, 255, 255);
		center.x = (b.MaxX() + b.MinX())/2;
		center.y = b.MaxY() + (b.MaxY()-b.MinY())/2.5;
		cvCircle(color_image, center, radius, color, -1, 8, 0);
	}

	IplImage * whole_head_image = cvCreateImage(img_size, IPL_DEPTH_8U, 1);
	IplImage * hsv = bgr2hsv(color_image);

	 IplImage * H, * S, * V;
     H = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_32F, 1);
     S = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_32F, 1);
     V = cvCreateImage( cvGetSize(hsv), IPL_DEPTH_32F, 1);  
     cvSplit(color_image, H, S, V, 0);

   	cvThreshold( V, whole_head_image, 3, 127, CV_THRESH_BINARY);

	if (OUTPUT_INTERMEDIATE_RESULTS) {
	    cvSaveImage("whole_head_image.jpg", whole_head_image); 
	}

    // cut the ROI 
	IplImage * head_image = cut_roi(whole_head_image, human_eye_gaze, ROI_size); 

	 cvReleaseImage(&color_image);
	 cvReleaseImage(&hsv);
	 cvReleaseImage(&H);
	 cvReleaseImage(&S);
	 cvReleaseImage(&V);
	 cvReleaseImage(&whole_head_image);

	return head_image;
}


IplImage * cut_roi(IplImage * src, int * eye_gaze, int * ROI_size) 
{
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
	
    IplImage * imgRect;
	if (rect.width > 0 && rect.height > 0) {
		CvSize rect_cvsize;
		rect_cvsize.width = rect.width;
		rect_cvsize.height = rect.height;			
		imgRect = cvCreateImage(rect_cvsize, src->depth, src->nChannels);  
		GetImageRect(src, rect, imgRect);
	} else {
		imgRect = (IplImage *)NULL;
	}

	return imgRect;
}


