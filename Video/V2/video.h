class object
{
    public:
        int left, right, top, bottom;
        CvPoint3D32f center;
};

object object;

class video
{
    public:
        void color_calibrate()
        {
            cvNamedWindow("calibrate");
            cvSetMouseCallback("calibrate", MyMouse, (void*)frame);
            while(c != 13)
            {
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
        }
};

video video;
