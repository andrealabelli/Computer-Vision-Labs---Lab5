#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    Mat images[] = {imread("../../Asphalt-1.png"), imread("../../Asphalt-2.png"), imread("../../Asphalt-3.png")};

    Mat gray_img1, gray_img2, gray_img3;
    Mat blur_img1, blur_img2, blur_img3;
    Mat thresh_img1, thresh_img2, thresh_img3;
    Mat morph_img1, morph_img2, morph_img3;
    Mat bit_img1, bit_img2, bit_img3;

    for(int i = 0; i < 3; i++)
    {
        if(!images[0].data && !images[1].data && !images[2].data)
        {
            printf(" No images found in the path! \n");
            return -1;
        }

        if(i==0)
        {
            //Converting to grayscale
            cvtColor(images[0], gray_img1, COLOR_BGR2GRAY);
            
            //Smoothing the image applying a Gaussian filter
            GaussianBlur(gray_img1, blur_img1, Size(3, 3), 0);

            //Thresholding the image in order to make visible the crack in the asphalt
            threshold(blur_img1, thresh_img1, 50, 200, THRESH_BINARY);
           
            //Using "MORPH_RECT" to take the small rectangles from which is composed the crack
            Mat kernel1 = getStructuringElement(MORPH_RECT, Size(3, 3));   
            morphologyEx(thresh_img1, morph_img1, MORPH_CLOSE, kernel1);

            //Inverting every bit of the image in order to make the cracks more visible 
            bitwise_not(morph_img1, bit_img1);


        }else if(i==1){

            cvtColor(images[1], gray_img2, COLOR_BGR2GRAY);

            GaussianBlur(gray_img2, blur_img2, Size(3, 3), 0);

            threshold(blur_img2, thresh_img2, 70, 120, THRESH_BINARY);   //Best value to reduce the rumor around the crack and to make it visible

            Mat kernel2 = getStructuringElement(MORPH_RECT, Size(3, 3));  
            morphologyEx(thresh_img2, morph_img2, MORPH_CLOSE, kernel2);

            bitwise_not(morph_img2, bit_img2);

        }else{

            cvtColor(images[2], gray_img3, COLOR_BGR2GRAY);

            GaussianBlur(gray_img3, blur_img3, Size(3, 3), 0);

            threshold(blur_img3, thresh_img3, 50, 150, THRESH_BINARY); 

            Mat kernel3 = getStructuringElement(MORPH_RECT, Size(3, 3));  
            morphologyEx(thresh_img3, morph_img3, MORPH_CLOSE, kernel3);

            bitwise_not(morph_img3, bit_img3);

        }
    }

    namedWindow("Cracks1", WINDOW_KEEPRATIO);
    imshow("Cracks1", bit_img1);

    namedWindow("Cracks2", WINDOW_KEEPRATIO);
    imshow("Cracks2", bit_img2);

    namedWindow("Cracks3", WINDOW_KEEPRATIO);
    imshow("Cracks3", bit_img3);

    waitKey(0);
    return 0;

}