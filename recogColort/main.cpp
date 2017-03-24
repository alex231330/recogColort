//
//  main.cpp
//  recogColort
//
//  Created by Алексадр Тюльпанов on 04.03.17.
//  Copyright © 2017 Алексадр Тюльпанов. All rights reserved.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>

using namespace cv;
using namespace std;

#define PointVal(img,x,y,c) (*((unsigned char*)(img->imageData+y*img->widthStep+x*img->nChannels+c)))

vector<int> compSize;
vector<CvPoint> st;
int trpleRgb[3][3][3];

int main(int argc, const char * argv[]) {
   // FILE* nxt;
   // nxt = fopen("/dev/tty.Bluetooth-Incoming-Port", "w");
    namedWindow("Controls", CV_WINDOW_AUTOSIZE);
    int hue = 0, s = 0, v = 0;
    cvCreateTrackbar("H", "Controls", &hue, 360);
    cvCreateTrackbar("S", "Controls", &s, 100);
    cvCreateTrackbar("V", "Controls", &v, 100);
    CvCapture* capture = cvCreateCameraCapture(0);
    IplImage* frame = 0;
    IplImage* gray = cvCreateImage(cvGetSize(cvQueryFrame(capture)), IPL_DEPTH_8U, 3);
    IplImage* binary = cvCreateImage( cvGetSize( cvQueryFrame(capture) ), 8, 1 );
    IplImage* preCopy = cvCreateImage(cvGetSize(cvQueryFrame(capture)), IPL_DEPTH_8U, 3);
    int w = cvGetSize(cvQueryFrame(capture)).width, h = cvGetSize(cvQueryFrame(capture)).height;
    int comp[h][w];
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours = 0;
    while (true) {
        frame = cvQueryFrame(capture);
        cvCvtColor(frame, gray, CV_BGR2HSV);
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                if (PointVal(gray, x, y, 0) < hue  &&  PointVal(gray, x, y, 1) > s && PointVal(gray, x, y, 2) > v) {
                    PointVal(gray, x, y, 0) = 0;
                    PointVal(gray, x, y, 1) = 0;
                    PointVal(gray, x, y, 2) = 0;
                }else {
                    PointVal(gray, x, y, 0) = 255;
                    PointVal(gray, x, y, 1) = 255;
                    PointVal(gray, x, y, 2) = 255;
                }
            }
        }
        compSize.clear();
        for(int y = 0; y < w; y++){
            for(int x = 0; x < h; x++){
                comp[x][y] = -1;
            }
            
        }
        
        for (int y = 0;y < h; y++) {
            for (int x = 0; x < w; x++) {
                if (comp[y][x] == -1 && PointVal(gray , x, y, 0) > 0) {
                    comp[y][x] = compSize.size();
                    compSize.push_back(1);
                    st.push_back(cvPoint(x, y));
                    int cnt = 0;
                    while (st.size() > 0) {
                        CvPoint p = st.back();
                        st.pop_back();
                        int dx[] = {0,0,1,-1};
                        int dy[] = {1,-1,0,0};
                        for (int i = 0; i < 4; i++) {
                            CvPoint np = cvPoint(p.x+dx[i], p.y + dy[i]);
                            if (np.x < 0 || np.y < 0 || np.x >= w || np.y >= h) continue;
                            if (PointVal(gray, np.x, np.y, 0) == 0 || comp[np.y][np.x] != -1) continue;
                            comp[np.y][np.x] = compSize.size() - 1;
                            compSize[compSize.size() - 1]++;
                            st.push_back(np);
                        }
                    }
                }
                
            }
            
        }
        int cnt = 0;
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                if (PointVal(gray, x, y, 0) == 0) {
                    //cout << comp[y][x] << endl;
                    if(compSize[comp[y][x]] < 50)
                    {
                        cnt++;
                        PointVal(gray, x, y, 0) = 0;
                        PointVal(gray, x, y, 1) = 0;
                        PointVal(gray, x, y, 2) = 0;
                    }
                }
            }
        }
        cvCvtColor(gray, preCopy, CV_HSV2BGR);
        cvCvtColor(preCopy, binary, CV_BGR2GRAY);
        cvThreshold(binary, binary, 128, 255, CV_THRESH_BINARY_INV);
        cvFindContours( binary, storage, &contours, sizeof(CvContour), CV_RETR_LIST );
        for( CvSeq* c = contours; c != NULL; c = c -> h_next)
        {
            CvRect Rect = cvBoundingRect( c );
            if ( Rect.width * Rect.height < 120 ) continue;
            cvRectangle( gray, cvPoint( Rect.x, Rect.y ), cvPoint( Rect.x + Rect.width, Rect.y + Rect.height ), CV_RGB(255,0,0), 2 );
        }
        
        //cvShowImage("Contours Recognition", binary);
        cvShowImage("Colors Reconition", gray);
        //cvShowImage("Contour ", drawContrs);
        
        if (waitKey(100) == 27)
        {
            cout << "esc key is pressed" << endl;
          //  fclose(nxt);
            break;
        }
    }
    return 0;
}
