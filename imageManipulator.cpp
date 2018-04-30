//
//  imageManipulator.cpp
//  OpenCV
//
//  Created by Savita Medlang on 4/29/18.
//  Copyright © 2018 Savita Medlang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
//using namespace std;

#define NUM_THREADS 16

//#define imageName "sample.png"

Mat image = imread("sample.png", IMREAD_COLOR);
int choice =0;



void *AlterPixels(void *threadID){
    
    //make sure the choice is set first
    long tid;
    tid = (long)threadID;
    
    int numRows = image.rows;
    int numCols = image.cols;
    
    int rowStart = image.rows/NUM_THREADS * tid;
    
    
    //index 0 is blue
    //index 1 is green
    //index 2 is red
    
    
    
    for (int i=rowStart; i<rowStart+(numRows/NUM_THREADS); i++){
        for (int j=0; j<numCols; j++){
            
            //inversion
            if (choice == 1){
                image.at<cv::Vec3b>(i,j)[0] = 255.0-image.at<cv::Vec3b>(i,j)[0];
                image.at<cv::Vec3b>(i,j)[1] = 255.0-image.at<cv::Vec3b>(i,j)[1];
                image.at<cv::Vec3b>(i,j)[2] = 255.0-image.at<cv::Vec3b>(i,j)[2];
            }
            
            // switch values
            // blue gets green
            // green gets red
            // red gets blue
            else if (choice == 2){
                double a = image.at<cv::Vec3b>(i,j)[0];
                double b = image.at<cv::Vec3b>(i,j)[1];
                double c = image.at<cv::Vec3b>(i,j)[2];
                image.at<cv::Vec3b>(i,j)[0] = b;
                image.at<cv::Vec3b>(i,j)[1] = c;
                image.at<cv::Vec3b>(i,j)[2] = a;
            }
            
            // grayscale
            else if (choice == 3){
                double avg = (image.at<cv::Vec3b>(i,j)[0] + image.at<cv::Vec3b>(i,j)[1] + image.at<cv::Vec3b>(i,j)[2]) / 3;
                image.at<cv::Vec3b>(i,j)[0] = avg;
                image.at<cv::Vec3b>(i,j)[1] = avg;
                image.at<cv::Vec3b>(i,j)[2] = avg;
            }
            
            //remove the green
            else if (choice == 4){
                image.at<cv::Vec3b>(i,j)[0] = image.at<cv::Vec3b>(i,j)[0];
                image.at<cv::Vec3b>(i,j)[1] = 0;
                image.at<cv::Vec3b>(i,j)[2] = image.at<cv::Vec3b>(i,j)[2];
            }
            
            //remove the blue
            else if(choice == 5){
                image.at<cv::Vec3b>(i,j)[0] = 0;
            }
            
            //remove the red
            else if (choice == 6){
                image.at<cv::Vec3b>(i,j)[2] = 0;
            }
            
            //change based on the threadID
            else if(choice == 7){
                image.at<cv::Vec3b>(i,j)[0] = (tid* image.at<cv::Vec3b>(i,j)[0])%255;
                image.at<cv::Vec3b>(i,j)[1] = (tid* image.at<cv::Vec3b>(i,j)[1])%255;
                image.at<cv::Vec3b>(i,j)[2] = (tid* image.at<cv::Vec3b>(i,j)[2])%255;
            }
             
        }
    }
    sleep(1);
    
    pthread_exit(NULL);
}

/**
 
 1:inversion
 2:switch RBG values
 3:grayscale
 4:remove green
 5:remove blue
 6:remove red
 7:changes based on threadID
 */

int main(int argc, char* argv[]){

    //user enters choice with number 1-7
    imshow("Original", image);
    waitKey(0);
    pthread_t threads[NUM_THREADS];
    int rc, i;
    
    choice = atoi(argv[1]);
    if (choice == 1){
        printf("Inversion!");
    }
    else if (choice == 2){
        printf("Switching RBG Values!");
    }
    else if (choice == 3){
        printf("Grayscale photo!");

    }
    else if (choice == 4){
        printf("Remove all green from the photo!");

    }
    else if (choice == 5){
        printf("Remove all blue from the photo!");

    }
    else if (choice == 6){
        printf("Remove all red from the photo!");

    }
    else {
        printf("Change the color based on thread number!");

    }
    fflush(stdout);
    
    for( i = 0; i < NUM_THREADS; i++ ) {
        rc = pthread_create(&threads[i], NULL, AlterPixels, (void *)i);
        
        if (rc) {
            std::cout << "Error:unable to create thread," << rc << std::endl;
            exit(-1);
        }
    }
    
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    imshow("After Manipulation!",image);
    waitKey(0);
    pthread_exit(NULL);
    return 0;

}
