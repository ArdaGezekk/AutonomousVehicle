#include <opencv2/opencv.hpp>
#include <raspicam_cv.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <wiringPi.h>
using namespace std;
using namespace cv;
using namespace raspicam;
//ImageProcessingVariables
RaspiCam_Cv Camera;
Mat frame, Matrix, framePers, frameGray, frameTresh,frameEdge, frameFinal,ROILane,frameFinalDuplicate,frameFinalDuplicate1,ROILaneEnd;
vector<int> histogramLane;
vector<int> histrogramLaneEnd;
int LeftLanePos, RightLanePos, frameCenter, laneCenter, Result, laneEnd;
stringstream ss;
//                         L-T(x,y)         R-T(x,y)       L-B(x,y)         R-B(x,y)
Point2f Source[] = {Point2f(35,160),Point2f(360,160),Point2f(5,210),Point2f(390,210)};
Point2f Destination[] = {Point2f(80,0),Point2f(285,0),Point2f(45,240),Point2f(305,240)};
//MachineLeraning
CascadeClassifier Stop_Cascade, Barrier_Cascade, TrafficLight_Cascade;
Mat frame_Stop, RoI_Stop, gray_Stop, frame_Barrier, RoI_Barrier, gray_Barrier, frame_TrafficLight, RoI_TrafficLight, gray_TrafficLight;
vector<Rect> Stop, Barrier, TrafficLight;
int dist_Stop, dist_Barrier, dist_TrafficLight;
void Setup ( int argc,char **argv, RaspiCam_Cv &Camera )
{
  Camera.set ( CAP_PROP_FRAME_WIDTH,  ( "-w",argc,argv,400 ) );
  Camera.set ( CAP_PROP_FRAME_HEIGHT,  ( "-h",argc,argv,240 ) );
  Camera.set ( CAP_PROP_BRIGHTNESS, ( "-br",argc,argv,50 ) );
  Camera.set ( CAP_PROP_CONTRAST ,( "-co",argc,argv,50 ) );
  Camera.set ( CAP_PROP_SATURATION,  ( "-sa",argc,argv,50 ) );
  Camera.set ( CAP_PROP_GAIN,  ( "-g",argc,argv ,50 ) );
  Camera.set ( CAP_PROP_FPS,  ( "-fps",argc,argv,10));
}
void Capture(){
  Camera.grab();
  Camera.retrieve( frame);
  cvtColor(frame, frame_Stop, COLOR_BGR2RGB);
  cvtColor(frame, frame_Barrier, COLOR_BGR2RGB);
  cvtColor(frame, frame_TrafficLight, COLOR_BGR2RGB);
  cvtColor(frame, frame, COLOR_BGR2RGB);
}
void Perspective()
{
  line(frame,Source[0],Source[1],Scalar(0,0,255),2);
  line(frame,Source[1],Source[3],Scalar(0,0,255),2);
  line(frame,Source[3],Source[2],Scalar(0,0,255),2);
  line(frame,Source[2],Source[0],Scalar(0,0,255),2);
  Matrix = getPerspectiveTransform(Source, Destination);
  warpPerspective(frame, framePers, Matrix, Size(400,240));
}
void Treshold(){
  cvtColor(framePers, frameGray, COLOR_RGB2GRAY);
  inRange(frameGray, 180, 255, frameTresh);
  Canny(frameGray, frameEdge, 100, 700, 3, false);
  add(frameTresh, frameEdge, frameFinal);
  cvtColor(frameFinal, frameFinal, COLOR_GRAY2RGB);
  cvtColor(frameFinal, frameFinalDuplicate, COLOR_RGB2BGR);   //used in histrogram function only
  cvtColor(frameFinal, frameFinalDuplicate1, COLOR_RGB2BGR);
}
void Histogram() {
  histogramLane.resize(400); //width of array
  histogramLane.clear();
  for (int i = 0; i < frame.size().width; i++) { //frame.Size().width=400
    ROILane = frameFinalDuplicate(Rect(i,140,1,100)); //create reason of interest's parts
    divide(255,ROILane, ROILane);
    histogramLane.push_back((int)(sum(ROILane)[0]));
  }
  histrogramLaneEnd.resize(400);
  histrogramLaneEnd.clear();
  for (int i = 0; i < frame.size().width; i++)
  {
    ROILaneEnd = frameFinalDuplicate1(Rect(i, 0, 1, 240));
    divide(255, ROILaneEnd, ROILaneEnd);
    histrogramLaneEnd.push_back((int)(sum(ROILaneEnd)[0]));
  }
  laneEnd = sum(histrogramLaneEnd)[0];
  //cout<<"Lane END Value is :  "<<laneEnd<<endl;
}
void LaneFinder(){
  vector<int>:: iterator LeftPtr;
  LeftPtr = max_element(histogramLane.begin(), histogramLane.begin() + 150);
  LeftLanePos = distance(histogramLane.begin(), LeftPtr);
  vector<int>:: iterator RightPtr;
  RightPtr = max_element(histogramLane.begin() +250, histogramLane.end());
  RightLanePos = distance(histogramLane.begin(), RightPtr);
  line(frameFinal, Point2f(LeftLanePos, 0), Point2f(LeftLanePos, 240), Scalar(0, 255,0), 2);
  line(frameFinal, Point2f(RightLanePos, 0), Point2f(RightLanePos, 240), Scalar(0,255,0), 2);
}
void LaneCenter(){
  laneCenter = (RightLanePos-LeftLanePos)/2 +LeftLanePos;
  frameCenter = 176;
  line(frameFinal, Point2f(laneCenter,0), Point2f(laneCenter,240), Scalar(0,255,0), 3);
  line(frameFinal, Point2f(frameCenter,0), Point2f(frameCenter,240), Scalar(255,0,0), 3);
  Result = laneCenter - frameCenter;
  //Result = 0;
}
void Stop_detection()
{
  if(!Stop_Cascade.load("//home//pi//Desktop//MACHINELEARNING//stop_cascade.xml"))
  {
    printf("Unable to open stop cascade file");
  }
  RoI_Stop = frame_TrafficLight(Rect(0,0,400,240));
  cvtColor(RoI_Stop, gray_Stop, COLOR_RGB2GRAY);
  equalizeHist(gray_Stop, gray_Stop);
  Stop_Cascade.detectMultiScale(gray_Stop, Stop);
  for(int i=0; i<Stop.size(); i++)
  {
    Point P1(Stop[i].x, Stop[i].y);
    Point P2(Stop[i].x + Stop[i].width, Stop[i].y + Stop[i].height);
    rectangle(RoI_Stop, P1, P2, Scalar(0, 120, 255), 2);
    dist_Stop = (-0.7894736842105263)*(P2.x-P1.x) + 65.5263157895;
    char txt[200];
    sprintf(txt,"StopSign %dcm",dist_Stop);
    putText(RoI_Stop, txt, P1, FONT_HERSHEY_PLAIN, 1,  Scalar(0, 120, 255), 2);
  }
}
void Barrier_detection()
{
  if(!Barrier_Cascade.load("//home//pi//Desktop//MACHINELEARNING//Barrier.xml"))
  {
    printf("Unable to open Barrier cascade file");
  }
  RoI_Barrier = frame_TrafficLight(Rect(0,0,400,240));
  cvtColor(RoI_Barrier, gray_Barrier, COLOR_RGB2GRAY);
  equalizeHist(gray_Barrier, gray_Barrier);
  Barrier_Cascade.detectMultiScale(gray_Barrier, Barrier);
  for(int i=0; i<Barrier.size(); i++)
  {
    Point P1(Barrier[i].x, Barrier[i].y);
    Point P2(Barrier[i].x + Barrier[i].width, Barrier[i].y + Barrier[i].height);
    rectangle(RoI_Barrier, P1, P2, Scalar(0, 120, 255), 2);
    dist_Barrier = (-0.42857142857)*(P2.x-P1.x) + 77.1428571427;
    char txt[200];
    sprintf(txt,"Barrier %dcm",dist_Barrier);
    putText(RoI_TrafficLight, txt, P1, FONT_HERSHEY_PLAIN, 1.15,  Scalar(0, 120, 255), 2);
  }
}
void TrafficLight_detection()
{
  if(!TrafficLight_Cascade.load("//home//pi//Desktop//MACHINELEARNING//TrafficLight.xml"))
  {
    printf("Unable to open TrafficLight cascade file");
  }
  RoI_TrafficLight = frame_TrafficLight(Rect(0,0,400,240));
  cvtColor(RoI_TrafficLight, gray_TrafficLight, COLOR_RGB2GRAY);
  equalizeHist(gray_TrafficLight, gray_TrafficLight);
  TrafficLight_Cascade.detectMultiScale(gray_TrafficLight, TrafficLight);
  for(int i=0; i<TrafficLight.size(); i++)
  {
    Point P1(TrafficLight[i].x, TrafficLight[i].y);
    Point P2(TrafficLight[i].x + TrafficLight[i].width, TrafficLight[i].y + TrafficLight[i].height);
    rectangle(RoI_TrafficLight, P1, P2, Scalar(0, 120, 255), 2);
    dist_TrafficLight = abs((-2.5)*(P2.x-P1.x) + 95);
    char txt[200];
    sprintf(txt,"TrafficLight %dcm",dist_TrafficLight);
    putText(RoI_TrafficLight, txt, P1, FONT_HERSHEY_PLAIN, 1.15,  Scalar(0, 120, 255), 2);
  }
}
int main(int argc,char **argv)
{
  wiringPiSetup();
  pinMode(25, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(21, OUTPUT);
  Setup(argc, argv, Camera);
  cout<<"Connecting to camera"<<endl;
  if (!Camera.open())
  {
    cout<<"Failed to Connect"<<endl;
  }
  cout<<"Camera Id = "<<Camera.getId()<<endl;
  while(1)
  {
    auto start = std::chrono::system_clock::now();
    Capture();
    Perspective();
    Treshold();
    Histogram();
    LaneFinder();
    LaneCenter();
    Stop_detection();
    Barrier_detection();
    TrafficLight_detection();
    //cout<<"Lane Value= "<<laneEnd<<endl;
    if (laneEnd > 10000) {
      digitalWrite(25, 0);
      digitalWrite(24, 1);
      digitalWrite(23, 1);          // 15 ===> 01111
      digitalWrite(22, 1);
      digitalWrite(21, 1);
      cout<<"Lane End Detected = "<<laneEnd<<endl;
    }
    if (dist_Stop > 1 && dist_Stop < 15){
      digitalWrite(25, 1);
      digitalWrite(24, 0);
      digitalWrite(23, 0);          // 17 ===> 10001
      digitalWrite(22, 0);
      digitalWrite(21, 1);
      cout<<"Stop Sign Detected  "<<dist_Stop<<endl;
      dist_Stop = 0;
      goto jump;
    }
    if (dist_Barrier > 1 && dist_Barrier < 35){
      digitalWrite(25, 1);
      digitalWrite(24, 0);
      digitalWrite(23, 0);          // 19 ===> 10011
      digitalWrite(22, 1);
      digitalWrite(21, 1);
      cout<<"BARRIER Detected"<<endl;
      dist_Barrier = 0;
      goto jump;
    }
    if (dist_TrafficLight > 0 && dist_TrafficLight < 35 || dist_TrafficLight < 0){
      digitalWrite(25, 1);
      digitalWrite(24, 0);
      digitalWrite(23, 1);          // 21 ===> 10101
      digitalWrite(22, 0);
      digitalWrite(21, 1);
      cout<<"TRAFFIC LIGHT Detected"<<endl;
      dist_TrafficLight = 0;
      goto jump;
    }
    if (Result == 0)
    {
      digitalWrite(25, 0);
      digitalWrite(24, 0);              // 1 ===> 0001
      digitalWrite(23, 0);    // Set to Arudino Values 0
      digitalWrite(22, 0);
      digitalWrite(21, 1);
      cout<<"Forward"<<endl;
    }
    else if (Result >0 && Result <10)
    {
      digitalWrite(25, 0);
      digitalWrite(24, 0);              // 3 ===> 0011
      digitalWrite(23, 0);    // Set to Arudino Values 3
      digitalWrite(22, 1);
      digitalWrite(21, 1);
      cout<<"littleRight"<<endl;
    }
    else if (Result >=10 && Result <20)
    {
      digitalWrite(25, 0);
      digitalWrite(24, 0);              // 5 ===>  00101
      digitalWrite(23, 1);    // Set to Arudino Values 5
      digitalWrite(22, 0);
      digitalWrite(21, 1);
      cout<<"mediumRight"<<endl;
    }
    else if (Result >20)
    {
      digitalWrite(25, 0);
      digitalWrite(24, 0);              // 7 ===> 00111
      digitalWrite(23, 1);    // Set to Arudino Values 7
      digitalWrite(22, 1);
      digitalWrite(21, 1);
      cout<<"veryRight"<<endl;
    }
    else if (Result <0 && Result >-10)
    {
      digitalWrite(25, 0);
      digitalWrite(24, 1);              // 9 ===> 01001
      digitalWrite(23, 0);    // Set to Arudino Values 9
      digitalWrite(22, 0);
      digitalWrite(21, 1);
      cout<<"littleLeft"<<endl;
    }
    else if (Result <=-10 && Result >-20)
    {
      digitalWrite(25, 0);
      digitalWrite(24, 1);              // 11 ===> 01011
      digitalWrite(23, 0);    // Set to Arudino Values 11
      digitalWrite(22, 1);
      digitalWrite(21, 1);
      cout<<"mediumLeft"<<endl;
    }
    else if (Result <-20)
    {
      digitalWrite(25, 0);
      digitalWrite(24, 1);          // 13 ===> 01101
      digitalWrite(23, 1);    // Set to Arudino Values 13
      digitalWrite(22, 0);
      digitalWrite(21, 1);
      cout<<"veryLeft"<<endl;
    }
    jump:
    if (laneEnd > 10000) {
      ss.str("");
      ss.clear();
      ss<<"Lane END ";
      putText(frame, ss.str(), Point2f(1,50),0,1, Scalar(250,0,250),2);
    }
    else if (Result == 0) {
      ss.str("");
      ss.clear();
      ss<<"Result = "<<Result<<" Move Forward";
      putText(frame, ss.str(), Point2f(1,50),0,1, Scalar(0,0,250),2);
    }
    else if (Result > 0) {
      ss.str("");
      ss.clear();
      ss<<"Result = "<<Result<<"Move Right";
      putText(frame, ss.str(), Point2f(1,50),0,1, Scalar(0,0,250),2);
    }
    else if (Result < 0) {
      ss.str("");
      ss.clear();
      ss<<"Result = "<<Result<<"Move Left";
      putText(frame, ss.str(), Point2f(1,50),0,1, Scalar(0,0,250),2);
    }
    namedWindow("Main",WINDOW_KEEPRATIO);
    moveWindow("Main", 505,36);
    resizeWindow("Main",640,480);
    imshow("Main",frame);
    namedWindow("Perspective",WINDOW_KEEPRATIO);
    moveWindow("Perspective", 1145,36);
    resizeWindow("Perspective",640,480);
    imshow("Perspective",framePers);
    namedWindow("Canny+Trasholding",WINDOW_KEEPRATIO);
    moveWindow("Canny+Trasholding", 505,550);
    resizeWindow("Canny+Trasholding",640,480);
    imshow("Canny+Trasholding",frameFinal);
    namedWindow("TrafficLight",WINDOW_KEEPRATIO);
    moveWindow("TrafficLight", 1145,550);
    resizeWindow("TrafficLight",640,480);
    imshow("TrafficLight",RoI_TrafficLight);
    waitKey(1);
    // auto end = std::chrono::system_clock::now();
    // std::chrono::duration<double> elapsed_seconds = end-start;
    // float t = elapsed_seconds.count();
    // int FPS = 1/t;
    // cout<<"FPS = "<<FPS<<endl;
  }
  return 0;
}
