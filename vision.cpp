#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <algorithm>
#include <cmath>

#include <ros/ros.h>
#include <std_msgs/String.h>

using namespace std;

int line_detection(cv::Mat frame, string filename = "");


ros::Publisher pub;

void publishDirection(const std::string& direction);

void capture_video()
{
    // Récupération du flux vidéo de la webcam externe
    cv::VideoCapture vid_capture(2);
    int frame_width = static_cast<int>(vid_capture.get(3));
    int frame_height = static_cast<int>(vid_capture.get(4));
    cv::Size frame_size(frame_width, frame_height);
    int i = 0;

    if (!vid_capture.isOpened())
    {
        cout << "Error during opening video stream or file " << endl;
    }
    else
    {
        double fps = vid_capture.get(5);
        cout << "Frames per second :" << fps << endl;

        double frame_count = vid_capture.get(7);
        vid_capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        vid_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
        cout << "Frame count : " << frame_count << endl;
        cv::VideoWriter ouput("../Video/output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frame_size);
        while (true)
        {
            cv::Mat frame;
            bool isSuccess = vid_capture.read(frame);
            if(i==0){
                cout<<"Frame size :"<<frame_size<<endl;
                i =1;
            }
            if (isSuccess)
            {
                int valeur = line_detection(frame);
                if (valeur > 0)
                {
           	    publishDirection("Gauche");
                    cout << "Gauche" << endl;
                }
                else if (valeur < 0)
                {
                    publishDirection("Droite");
                    cout << "Droite" << endl;
                }
                else
                {
                    publishDirection("Avancer");
                    cout << "Avancer" << endl;
                }
                cv::namedWindow("Frame", cv::WINDOW_GUI_EXPANDED);
                cv::imshow("Frame", frame);
                ouput.write(frame);
            }
            else
            {
                cout << "Video camera is disconnected " << endl;
            }
            if (cv::waitKey(1) == 'q')
            {
                cout << "q key is pressed by the user. Stopping the video" << endl;
                break;
            }
        }
    }
    vid_capture.release();
    cv::destroyAllWindows();
}

int line_detection(cv::Mat frame, string filename /*= ""*/)
{
    vector<cv::Vec4i> output_img;
    cv::Mat img_hsv, dectInRange, resultMat;
    int high_h = 112, low_h = 93;
    int high_s = 255, low_s = 50;
    int high_v = 255, low_v = 122;
    int left = 0, right = 0, seuil = 200;
    int minLineLenght = 80;
    int maxLineGap = 20;

    cv::Scalar minHSV = cv::Scalar(low_h, low_s, low_v);
    cv::Scalar maxHSV = cv::Scalar(high_h, high_s, high_v);

    cv::Mat image = frame.empty() ? cv::imread(filename, 1) : frame.clone();
    if (image.empty())
    {
        cout << "Erreur lors du chargement de l'image!!!!" << endl;
        exit(-1);
    }
    // Convertion de l'image en HSV
    cv::cvtColor(image, img_hsv, cv::COLOR_BGR2HSV);
    // Détection de la couleur bleu
    cv::inRange(img_hsv, minHSV, maxHSV, dectInRange);
    cv::bitwise_and(image, image, resultMat, dectInRange);

    // Détection de ligne par la version probabiliste de l'algo
    cv::HoughLinesP(dectInRange, output_img, 1, 3.141592653589793 / 180, 50, minLineLenght, maxLineGap);

    // Opération de morphologie pour améliorer la détection de line
    cv::Mat element = getStructuringElement(0, cv::Size(2 * 2 + 1, 2 * 2 + 1), cv::Point(2, 2));
    morphologyEx(dectInRange, dectInRange, cv::MORPH_CLOSE, element);
    morphologyEx(dectInRange, dectInRange, cv::MORPH_DILATE, element);

    int y_moy = dectInRange.rows / 2;
    for (int i = dectInRange.cols / 2; i > 0; i--)
    {
        if ((int)dectInRange.at<uchar>(y_moy, i) > seuil)
        {
            break;
        }
        else
        {
            left += 1;
        }
    }
    for (int i = dectInRange.cols / 2; i < dectInRange.cols; i++)
    {
        if ((int)dectInRange.at<uchar>(y_moy, i) > seuil)
        {
            break;
        }
        else
        {
            right += 1;
        }
    }
    // Affichage des traitements sur une image spécifique sans la vidéo
    if(!filename.empty()){
        cv::imshow("Detection de ligne",dectInRange);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    return left - right;
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        cv::Mat frame;
        string filename = argv[1];
        line_detection(frame, filename);
    }else{
    	 // Initialisation du nœud ROS
        ros::init(argc, argv, "vision_node");

       // Création de l'objet NodeHandle
       ros::NodeHandle nh;

       // Création du publisher
        pub = nh.advertise<std_msgs::String>("direction", 10);
        capture_video();
    }   
    return 0;
}

void publishDirection(const std::string& direction)
{
    std_msgs::String msg;
    msg.data = direction;
    pub.publish(msg);
}
