#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{

    Mat input_img = imread("../../robocup.jpg");

    if (!input_img.data)
    {
        printf(" No image found in the path! \n");
        return -1;
    }

    Mat gray_img;
    cvtColor(input_img, gray_img, COLOR_BGR2GRAY);

    Mat blur_img;
    GaussianBlur(gray_img, blur_img, Size(3, 3), 0);

    // Reshape the image to a 2D array of pixels and 3 color values (RGB)
    Mat pixel_values_img = input_img.reshape(1, input_img.rows * input_img.cols);
    
    // Convert to float
    pixel_values_img.convertTo(pixel_values_img, CV_32F);
    
    // Define stopping criteria
    TermCriteria criteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 100, 0.2);
    
    // Number of clusters (K)
    int k = 5;
    
    // Perform k-means clustering
    Mat labels, centers;
    kmeans(pixel_values_img, k, labels, criteria, 10, KMEANS_PP_CENTERS, centers);  
    
    // Convert back to 8-bit values
    centers.convertTo(centers, CV_8U);
    
    // Flatten the labels array
    labels = labels.reshape(1, input_img.rows);
    
    // Convert all pixels to the color of the centroids
    Mat segmented_img = Mat::zeros(input_img.size(), input_img.type());
    
    for(int i = 0; i < input_img.rows; i++)
    {
        for(int j = 0; j < input_img.cols; j++)
        {
            int label = labels.at<int>(i,j);
            segmented_img.at<Vec3b>(i,j) = centers.at<Vec3b>(label,0);
        }
    }
    
    // Show the image
    namedWindow("Segmented Image", WINDOW_KEEPRATIO);
    imshow("Segmented Image", segmented_img);

    Scalar low = Scalar(5, 80, 100); // BGR
    Scalar high = Scalar(48, 153, 202);   //[48, 153, 202]  ---> BGR mean value of the t-shirts  
    inRange(segmented_img, low, high, segmented_img);
    namedWindow("T-shirts", WINDOW_KEEPRATIO);
    imshow("T-shirts", segmented_img);

    waitKey(0);
    return 0;

}