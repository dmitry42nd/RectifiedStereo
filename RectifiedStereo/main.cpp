// TestTrackingOCV.cpp : Defines the entry point for the console application.
//
#include <time.h>
#include <iostream> 
#include <fstream>
#include <memory>
#include <vector>

#include <cstdio>

#include "boost/filesystem.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>


int main()
{
  clock_t tStart = clock();
  /*
  std::string imgsDirLPath = "..\\image_00_0\\";
  std::string imgsDirRPath = "..\\image_01_0\\";
  */
  std::string imgsDirLPath = "..\\image_00\\data\\";
  std::string imgsDirRPath = "..\\image_01\\data\\";

  boost::filesystem::path imgsDirL(imgsDirLPath);
  boost::filesystem::path imgsDirR(imgsDirRPath);
  typedef std::vector<boost::filesystem::path> vec; // store paths, so we can sort them later
  vec imgsL, imgsR;
  copy(boost::filesystem::directory_iterator(imgsDirL), boost::filesystem::directory_iterator(), std::back_inserter(imgsL));
  copy(boost::filesystem::directory_iterator(imgsDirR), boost::filesystem::directory_iterator(), std::back_inserter(imgsR));
  sort(imgsL.begin(), imgsL.end());
  sort(imgsR.begin(), imgsR.end());

  cv::Mat g1, g2, disp, disp8;

  cv::StereoSGBM sgbm;

  sgbm.SADWindowSize = 5;
  sgbm.numberOfDisparities = 192;
  sgbm.preFilterCap = 4;
  sgbm.minDisparity = -64;
  sgbm.uniquenessRatio = 1;
  sgbm.speckleWindowSize = 150;
  sgbm.speckleRange = 2;
  sgbm.disp12MaxDiff = 10;
  sgbm.fullDP = false;
  sgbm.P1 = 600;
  sgbm.P2 = 2400;

  int cnt = 0;
  char outImgPath[100];
  while (cnt < imgsL.size()) {

    std::string imgLPath = imgsL[cnt].string();
    std::string imgRPath = imgsR[cnt].string();

    cv::Mat img1 = cv::imread(imgLPath);
    cv::Mat img2 = cv::imread(imgRPath);
    cvtColor(img1, g1, CV_BGR2GRAY);
    cvtColor(img2, g2, CV_BGR2GRAY);

    sgbm(g1, g2, disp);
    normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);

    sprintf_s(outImgPath, "../out/%d.png", cnt);
    //cv::equalizeHist(disp8, disp8);
    cv::imwrite(outImgPath, disp8);

    cnt++;
  }

  double totalTime = (double)(clock() - tStart) / CLOCKS_PER_SEC;
  printf("Done.\nTotal time taken: %.2fs\n", totalTime);

  return 0;
}