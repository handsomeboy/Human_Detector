/*
 *  @file test.cpp
 *  Copyright [2018]
 *  @author Ghost1995 [Ashwin Goyal]
 *  @author anirudhtopiwala [Anirudh Topiwala]
 *  @date Oct 04, 2018
 *  @brief This is the declaration of all units tests to be conducted.
 */

#include <gtest/gtest.h>
#include <Data.hpp>
#include <Train.hpp>
#include <Detect.hpp>

TEST(DataTest, DataLoadImagesTest) {
  Data test("Data for DataTest 1");
  test.loadImages("../data/test");
  ASSERT_TRUE(test.imgList.size() > 0);
}

TEST(DataTest, DataSampleImagesTest) {
  Data test("Data for DataTest 2");
  test.loadImages("../data/test");
  std::vector<cv::Mat> orgDataList = test.imgList;
  test.sampleImages(cv::Size(100, 100));
  std::vector<cv::Mat> newDataList = test.imgList;
  bool match = false, checkSize = true;
  for (auto i = 0; i < orgDataList.size(); i++) {
    if (orgDataList[i].size() == newDataList[i].size()) {
      match = true;
      break;
    }
    if (newDataList[i].size() != cv::Size(100,100)) {
      checkSize = false;
      break;
    }
  }
  ASSERT_TRUE(!match & checkSize);
}

TEST(TrainTest, TrainGetHOGTest) {
  Data testData("Data for TrainTest 1");
  Train test;
  testData.loadImages("../data/test");
  test.getHOGfeatures(cv::Size(96, 160), testData.imgList);
  ASSERT_TRUE(test.gradientList.size() > 0);
}

TEST(TrainTest, TrainSVMTest) {
  Data testData("Data for TrainTest 2");
  Train test;
  testData.loadImages("../data/test");
  test.getHOGfeatures(cv::Size(96, 160), testData.imgList);
  test.labels.assign(test.gradientList.size()/2, 1);
  test.labels.insert(test.labels.end(), test.gradientList.size() - test.labels.size(), -1);
  bool cond = test.classifier->getSupportVectors().empty();
  test.trainSVM();
  cond = cond & !test.classifier->getSupportVectors().empty();
  ASSERT_TRUE(cond);
}

TEST(TrainTest, TrainGetClassifierTest) {
  Data testData("Data for TrainTest 3");
  Train test;
  testData.loadImages("../data/test");
  test.getHOGfeatures(cv::Size(96, 160), testData.imgList);
  test.labels.assign(test.gradientList.size()/2, 1);
  test.labels.insert(test.labels.end(), test.gradientList.size() - test.labels.size(), -1);
  test.trainSVM();
  ASSERT_TRUE(test.getClassifier().size() != 0);
}

TEST(DetectTest, DetectModeNameTest) {
  cv::HOGDescriptor hog;
  hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
  Detect test(hog);
  ASSERT_EQ("Default", test.modeName());
}

TEST(DetectTest, DetectToggleTest) {
  cv::HOGDescriptor hog;
  hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
  Detect test(hog);
  test.toggleMode();
  ASSERT_EQ("User", test.modeName());
}

TEST(DetectTest, DetectHumansTest) {
  cv::HOGDescriptor hog;
  hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
  Detect test(hog);
  cv::Rect orgBox(115,6,238,475);  //[238 x 475 from (115, 6)]
  std::string imageName("../data/test/pedestrian_5.jpg");
  cv::Mat img = cv::imread(imageName, CV_LOAD_IMAGE_COLOR);
  std::vector<cv::Rect> found = test.findHumans(img);
  std::vector<cv::Rect>::iterator i = found.begin();
  cv::Rect &r = *i;
  ASSERT_TRUE((orgBox.area() - (orgBox & r).area()) < 500);
}

TEST(DetectTest, DetectAdjustBoxTest) {
  cv::HOGDescriptor hog;
  hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
  Detect test(hog);
  cv::Rect newBox(139,39,190,380);  //[190 x 380 from (139, 39)]
  std::string imageName("../data/test/pedestrian_5.jpg");
  cv::Mat img = cv::imread(imageName, CV_LOAD_IMAGE_COLOR);
  std::vector<cv::Rect> found = test.findHumans(img);
  std::vector<cv::Rect>::iterator i = found.begin();
  cv::Rect &r = *i;
  test.adjustBoundingBox(r);
  ASSERT_TRUE((newBox.area() - (newBox & r).area()) < 500);
}

TEST(DetectTest, DetectTestClassifierTest) {
  cv::HOGDescriptor hog;
  hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
  Detect test1(hog), test2(hog);
  std::string imageName("../data/test/pedestrian_5.jpg");
  cv::Mat img = cv::imread(imageName, CV_LOAD_IMAGE_COLOR);
  std::vector<cv::Rect> found = test1.findHumans(img);
  std::vector<cv::Rect>::iterator i = found.begin();
  cv::Rect &orgBox = *i;
  cv::Rect r = test2.testClassifier("Default","../data/test", "", false);
  ASSERT_TRUE((orgBox.area() - (orgBox & r).area()) < 500);
}