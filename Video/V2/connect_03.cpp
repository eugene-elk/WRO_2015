#include <cstdio>
#include <iostream>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <cstdlib>
#include <cmath>

#include "bluetooth.h"

#define PointVal(img, x, y, c) *(img -> imageData + img -> widthStep*y + img -> nChannels*x + c)
#define Pi 3.14159265
#define centerX 320
#define centerY 240

using namespace std;

CvPoint queue1[640*480];
int comp [640][480];
int nsize[640*480];

int x_calibrate = 0;
int y_calibrate = 0;

void MyMouse(int event, int x, int y, int flag, void* param)
{
    IplImage* frame = (IplImage*)param;
    if (event == CV_EVENT_LBUTTONDOWN){
        x_calibrate = x;
        y_calibrate = y;
    }
}

int main()
{
    char c;

    int com1, com2;
    HANDLE hCom1, hCom2;

    int bytesToSend[100];
    int flag1 = 0, flag2 = 0;
    int u_motors, u_camera;
    int p1 = 0, p2 = 0, n = 0;
    int maximum;
    int object_left = 639;
    int object_right = 0;
    int object_top = 479;
    int object_bottom = 0;
    CvPoint3D32f object_center;

    CvPoint3D32f A, B, C;
    A.x = 0; A.y = 0; A.z = 0;
    B.z = 0; B.y = 0; B.x = 0;

    double angle_beta = 8, angle_alpha;
    double AB, AC, BC;

    CvCapture* capture = cvCreateFileCapture("http://admin:admin@192.168.10.239/video.mjpg");
    IplImage* frame;
    IplImage* image = cvCreateImage(cvSize(640/4, 480/4), IPL_DEPTH_8U, 3);
    IplImage* frameSmall = cvCreateImage(cvSize(640/4, 480/4), IPL_DEPTH_8U, 3);

    cvNamedWindow("calibrate");
    cvSetMouseCallback("calibrate", MyMouse, (void*)frame);

    while(c != 13){
        frame = cvQueryFrame(capture);
        cvSmooth(frame, frame, CV_GAUSSIAN, 7);
        cvShowImage("calibrate", frame);
        c = cvWaitKey(1);
    }
    for(int x = x_calibrate-5; x < x_calibrate+5; x++){
        for(int y = y_calibrate-5; y < y_calibrate+5; y++){
            B.z = B.z + (unsigned char)PointVal(frame, x, y, 0)/100.0;
            B.y = B.y + (unsigned char)PointVal(frame, x, y, 1)/100.0;
            B.x = B.x + (unsigned char)PointVal(frame, x, y, 2)/100.0;
        }
    }

    //подключение автономной тележки ---------------------------------------------------------------------------------------
    cout << "Input Autonomous Truck COM:" << endl;
    cin >> com1;
    if (com1 != 0)
        hCom1 = bt.connect(com1);

    while(c != 27)
    {
        frame = cvQueryFrame(capture);
        cvSetZero(image);
        cvSmooth(frame, frame, CV_GAUSSIAN, 13);//9
        cvResize(frame, frameSmall);

        //выбор пикселей по цвету конусом
        for(int x=0; x < frameSmall->width; x++)
        {
            for(int y=0; y < frameSmall->height; y++)
                {
                    C.z = (int)(unsigned char)PointVal(frameSmall, x, y, 0);
                    C.y = (int)(unsigned char)PointVal(frameSmall, x, y, 1);
                    C.x = (int)(unsigned char)PointVal(frameSmall, x, y, 2);

                    BC = sqrt((B.x-C.x)*(B.x-C.x)+(B.y-C.y)*(B.y-C.y)+(B.z-C.z)*(B.z-C.z));
                    AB = sqrt((B.x-A.x)*(B.x-A.x)+(B.y-A.y)*(B.y-A.y)+(B.z-A.z)*(B.z-A.z));
                    AC = sqrt((A.x-C.x)*(A.x-C.x)+(A.y-C.y)*(A.y-C.y)+(A.z-C.z)*(A.z-C.z));

                    angle_alpha = acos((AB*AB+AC*AC-BC*BC)/(2*AB*AC));
                    angle_alpha = angle_alpha/Pi*180.0;

                    if(abs(angle_alpha) < angle_beta){
                        PointVal(image, x, y, 0) = 0;//PointVal(frame, x, y, 0);
                        PointVal(image, x, y, 1) = 0;//PointVal(frame, x, y, 1);
                        PointVal(image, x, y, 2) = 255;//PointVal(frame, x, y, 2);
                    }
                }
        }

        //выделение наибольшей компоненты связности ------------------------------------------------------------------
        memset(nsize, 0, sizeof(nsize));
        memset(comp, -1, sizeof(comp));
        n = 0;
        for(int i = 0; i < image->width; i++){
            for(int j = 0; j < image->height; j++){
                if((unsigned char)PointVal(image, i, j, 2) != 255) continue;
                if(comp[i][j] != -1) continue;
                p1 = 0;
                p2 = 0;
                queue1[p2++] = cvPoint(i, j);
                comp[i][j] = n;
                nsize[n]++;
                while (p1 < p2){
                    CvPoint cur = queue1[p1++];
                    for(int dx=-1; dx<=1; dx++){
                        for(int dy=-1; dy<=1;dy++){
                            CvPoint t = cur;
                            t.x+=dx;
                            t.y+=dy;
                            if(dx == 0 && dy == 0) continue;
                            if(t.x < 0 || t.x >= image->width || t.y < 0 || t.y >=image->height) continue;
                            if(255 != (unsigned char)PointVal(image, t.x, t.y, 2)) continue;
                            if(comp[t.x][t.y] != -1) continue;
                            comp[t.x][t.y] = n;
                            queue1[p2++] = t;
                            nsize[n]++;
                        }
                    }
                }
                n++;
            }
        }

        maximum = 0;
        for(int tn = 1; tn < n; tn++){
            if(nsize[maximum] < nsize[tn]){
                maximum = tn;
            }
        }

        for(int x = 0; x < image->width; x++){
            for(int y = 0; y < image->height; y++){
                if(comp[x][y] == maximum){
                    PointVal(image, x, y, 2) = 0;
                    PointVal(image, x, y, 0) = 255;
                }
            }
        }

        //определение краев объекта
        int sredX = 0, sredY = 0, n = 0;
        for(int x = 0; x < image->width; x++)
        {
            for(int y = 0; y < image->height; y++)
            {
                if(PointVal(image, x, y, 0) != 0)
                {
                    sredX += x;
                    sredY += y;
                    n++;
                }
            }
        }
        if(n != 0)
        {
            sredX /= n;
            sredY /= n;
        }
        else
        {
            sredX = image->width/2;
            sredY = image->height/2;
        }
        object_center.x = sredX*4;
        object_center.y = sredY*4;

        cvShowImage("result", image);
        cvShowImage("camera", frame);
        cvShowImage("frameSmall", frameSmall);
        cvShowImage("image", image);

        u_motors = -0.21 * (320 - object_center.x);
        if ((object_center.y - 240) > 50) u_camera = -30;
        else u_camera = 0;
        if ((object_center.y - 240) < -50) u_camera = 30;

        cout << u_motors << " " << u_camera << endl;

        u_motors = u_motors + 100;
        if(u_motors < 0) u_motors = 0;
        if(u_motors > 200) u_motors = 200;

        u_camera = u_camera + 100;
        if(u_camera < 0) u_camera = 0;
        if(u_camera > 200) u_camera = 200;

        if (c == 49)
        {
            flag1 = 1;
            cout << "1 pressed" << endl;
        }

        if (com1 != 0)
            bt.dispatch(hCom1, u_motors, u_camera, flag1);

        c = cvWaitKey(1);
    }
}
