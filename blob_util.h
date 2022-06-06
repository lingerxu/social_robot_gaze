#include <BlobResult.h> // for blob detection on binary image
#include <cv.h>         // openCV head file. 
#include <highgui.h>	// openCV head file.

double get_blobs_size(CBlobResult blobs);
double get_min_distace_to_blob(int * point, IplImage * bin_img, CBlobResult blobs);
CvRect * detect_blob(IplImage * binImg, int blob_size_threshold, int * numRect);

