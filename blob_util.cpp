#include "blob_util.h"

extern bool  OUTPUT_INTERMEDIATE_RESULTS;

double get_blobs_size(CBlobResult blobs)
{
	double blobs_size = 0;
	for (int i = 0; i < blobs.GetNumBlobs(); i++) { 
		CBlob  b = blobs.GetBlob(i);
		blobs_size += b.Area();
	} 

	return blobs_size;
}

double get_min_distace_to_blob(int * point, IplImage * bin_img, CBlobResult blobs)
{	
	double distance;
	int n = blobs.GetNumBlobs();

	// when the object is not around eye gaze point.
	if (blobs.GetNumBlobs() < 1) {
		// distance = 2 * (bin_img->height + bin_img->width);
		return -1;
	}		

    CvScalar s;
    s=cvGet2D(bin_img,point[1],point[0]); // get the pixel value.  Please note order of parameters.
    if (s.val[0] >= 128) {
	  distance = 0;
	  return distance;
    }

	IplImage * tmp = cvCreateImage( cvGetSize(bin_img), IPL_DEPTH_32F, bin_img->nChannels);  // the deapth couldn't be 8U if it is the destination of cvLaplace.
	cvLaplace(bin_img,  tmp, 1);
//	cvSobel(bin_img, tmp, 1, 0, 3);

    IplImage * edge_img = cvCreateImage( cvGetSize(tmp), IPL_DEPTH_8U, 1);
 	cvThreshold(tmp, edge_img, 100, 255, CV_THRESH_BINARY);

    if (OUTPUT_INTERMEDIATE_RESULTS) {
		cvSaveImage("laplace.jpg", edge_img);
	}

	int step       = edge_img->widthStep / sizeof(unsigned char );
	unsigned char * data = (unsigned char *) edge_img->imageData;

	distance = 2 * (bin_img->height + bin_img->width); // initial distance is a large number. 
	for (int i = 0; i < blobs.GetNumBlobs(); i++ ) {
	  CBlob b = blobs.GetBlob(i);
	  CvRect box = b.GetBoundingBox();

	  for (int j=box.y;  j<box.y+box.height; j+=2) {
		  for (int k=box.x; k<box.x+box.width; k+=2) {
			  int d = data[j*step +k]; 
			  if (d) {	
				  double this_dist = sqrt(double((j-point[1]) * (j-point[1]) + (k-point[0])*(k-point[0])));
				  if (this_dist < distance)
					  distance = this_dist;
			  }
		  }
	  }

	}

	cvReleaseImage(&tmp);
	cvReleaseImage(&edge_img);

	return distance;
}


CvRect * detect_blob(IplImage * binImg, int blob_size_threshold, int * numRect)
{             
//////////////////////////////////////////////////////////////
// get blobs and filter them using its area
/////////////////////////////////////////////////////////////
CBlobResult blobs;

// find blobs in thresholded image
blobs = CBlobResult(binImg, (IplImage*)0, 0);
// exclude the ones smaller than blob_size_threshold
blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, blob_size_threshold );

// printf("blob number: %d\n", blobs.GetNumBlobs() );

int n = blobs.GetNumBlobs();
*numRect = n;
//CvRect * boxes = new CvRect[n]; 
int maxArea = 0;
int maxIdx  = -1;
for (int j=0; j<n; j++) {  // find the size of largest blob. 
	CBlob cb = blobs.GetBlob(j);
	int thisArea = cb.Area();
	if (thisArea > maxArea) {
		maxArea = thisArea;
		maxIdx = j;
	}
//	boxes[j] = cb.GetBoundingBox();
//	printf("blob %d: %d %d %d %d, size: %lf\n", j, boxes[j].x, boxes[j].y, boxes[j].width, boxes[j].height, cb.Area()); 
}

// only leave the largest blob. 
blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, maxArea);
n = blobs.GetNumBlobs();
*numRect = n;
CvRect * boxes = new CvRect[n]; 
for (int j=0; j<n; j++) {  // find the size of largest blob. 
	CBlob cb = blobs.GetBlob(j);
	boxes[j] = cb.GetBoundingBox();
//	printf("blob %d: %d %d %d %d, size: %lf\n", j, boxes[j].x, boxes[j].y, boxes[j].width, boxes[j].height, cb.Area()); 
}
return boxes;
}
