#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{

    Mat input_img = imread("../../street_scene.png");

    if (!input_img.data)
    {
        printf(" No image found in the path! \n");
        return -1;
    }

    
    Mat gray_img;
    cvtColor(input_img, gray_img, COLOR_BGR2GRAY);
    
    // Reshape the image to a 2D array of pixels and 3 color values (RGB)
    Mat pixel_values_img = input_img.reshape(1, input_img.rows * input_img.cols);
    
    // Convert to float
    pixel_values_img.convertTo(pixel_values_img, CV_32F);
    
    // Define stopping criteria
    TermCriteria criteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 100, 0.2);
    
    // Number of clusters (K)
    int k = 3;
    
    // Perform k-means clustering
    Mat labels, centers;
    kmeans(pixel_values_img, k, labels, criteria, 10, KMEANS_PP_CENTERS, centers);  
    
    // Convert back to 8-bit values
    centers.convertTo(centers, CV_8U);
    
    // Flatten the labels array
    labels = labels.reshape(1, input_img.rows);
    
    // Convert all pixels to the color of the centroids
    Mat segmented_image = Mat::zeros(input_img.size(), input_img.type());
    
    for(int i = 0; i < input_img.rows; i++)
    {
        for(int j = 0; j < input_img.cols; j++)
        {
            int label = labels.at<int>(i,j);
            segmented_image.at<Vec3b>(i,j) = centers.at<Vec3b>(label,0);
        }
    }
    
    // Show the image
    namedWindow("Segmented Image", WINDOW_KEEPRATIO);
    imshow("Segmented Image", segmented_image);

    waitKey(0);
    return 0;
}