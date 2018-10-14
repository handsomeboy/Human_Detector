/*
 *  @file main.cpp
 *  Copyright [2018]
 *  @author1 Ghost1995 [Ashwin Goyal]
 *  @author2 anirudhtopiwala [Anirudh Topiwala]
 *  @date Oct 04, 2018
 *  @brief This is the main file which runs the detection algorithm
 */

#include <Data.hpp>
#include <Train.hpp>
#include <Detect.hpp>

/*
 * @brief This is the main function to call the classes, execute the functions, and print the results.
 *
 * @return It returns a 0 just to avoid a warning.
 */
int main() {
    // Setting Data store file path

    const cv::String posDir = "../data/INRIAPerson/Train/pos/";
    const cv::String negDir = "../data/INRIAPerson/Train/neg/";
    const cv::String testDir = "../data/INRIAPerson/Test/pos/";
    const cv::String annotationpath = "../data/INRIAPerson/Train/annotations/";
    // Check if directories exist
    if (posDir.empty() || negDir.empty() || testDir.empty()) {
        std::cout << "Input data directory empty or incorrect." <<
                     " Please enter a valid data path." << std::endl;
        return 0;
    }

    // // Load positive images
    // Data posData;
    // // Loading Annotations
    // posData.loadAnnotations(annotationpath, posDir);
    // std::cout << "Loading Positive Images" << std::endl;
    // posData.loadImages();
    // // Check if images were successfully loaded
    // if (posData.imgList.size() > 0) {
    //     std::cout << "Loading Training Data Complete" << std::endl;
    // } else {
    //     std::cout << "No images found. " <<
    //                 "Please check the Path Directory: " << posDir << std::endl;
    //     return 0;
    // }

    // // Now load negative images and sample them randomly
    // Data negData;
    // negData.loadImages(negDir);
    // // Define a window size
    cv::Size windowSize = cv::Size(96, 160);
    // negData.sampleImages(windowSize);
    // std::cout << "Negative Image Sampling Completed" << std::endl;

    // Train trainClass;
    // // For Positive Images
    // std::cout << "Extracting HOG features and storing in a " <<
    //              "vector for Positive Images" << std::endl;
    // trainClass.getHOGfeatures(windowSize, posData.imgList);
    // size_t positiveCount = trainClass.gradientList.size();
    // // Assigning Positive labels
    // trainClass.labels.assign(positiveCount, 1);
    // std::cout << "Done getting HOG Features for Positive Images" << std::endl;

    // // For Negative Images
    // std::cout << "Extracting HOG features and storing in a " <<
    //              "vector for Negative Images" << std::endl;
    // trainClass.getHOGfeatures(windowSize, negData.imgList);
    // size_t negativeCount = trainClass.gradientList.size() - positiveCount;
    // // Assigning Negative labels
    // trainClass.labels.insert(trainClass.labels.end(), negativeCount, -1);
    // std::cout << "Done getting HOG Features for Negative Images" << std::endl;

    // Training Starts
    // trainClass.trainSVM();

    Train test;
    test.classifier = cv::ml::SVM::load("../data/svmclassifier"); 


    // Testing Trained Classifier
    Detect detector;
    detector.hog_user.winSize =  windowSize;
    detector.hog_user.setSVMDetector(test.getClassifier());
    cv::Rect r = detector.testClassifier(testDir, true);
    std::cout << "Finshed" << std::endl;

    // std::string imageName("../data/pedestrian_5.jpg");
    // cv::Mat img = cv::imread(imageName, CV_LOAD_IMAGE_COLOR);
    // std::vector<cv::Rect> found = test.findHumans(img);
    // for (std::vector<cv::Rect>::iterator i = found.begin();
    //                                      i != found.end(); ++i) {
    //     cv::Rect &r = *i;
    //     std::cout << r << std::endl;
    //     cv::rectangle(img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
    //     test.adjustBoundingBox(r);
    //     std::cout << r << std::endl;
    //     cv::rectangle(img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
    // }
    // cv::imshow("People detector", img);
    // cv::waitKey(1000);

    // cv::VideoCapture cap;
    // cap.open(0);
    // if (!cap.isOpened()) {
    //     std::cout << "Can not open video stream: " << std::endl;
    //     return 2;
    // }

    // std::cout << "Press <space> to toggle between " <<
    //               "Default and Daimler detector" << std::endl;
    // Detect detector;
    // cv::Mat frame;
    // for (;;) {
    //     cap >> frame;
    //     if (frame.empty()) {
    //         std::cout << "Finished reading: empty frame" << std::endl;
    //         break;
    //     }
    //     int64 t = cv::getTickCount();
    //     std::vector<cv::Rect> found = detector.findHumans(frame);
    //     t = cv::getTickCount() - t;

    //     for (std::vector<cv::Rect>::iterator i = found.begin();
    //                                          i != found.end(); ++i) {
    //         cv::Rect &r = *i;
    //         detector.adjustBoundingBox(r);
    //         cv::rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
    //     }
    //     cv::imshow("People detector", frame);

    //     // interact with user
    //     const char key = static_cast<char>(cv::waitKey(30));
    //     if (key == 27 || key == 'q') {  // ESC
    //         std::cout << "Exit requested" << std::endl;
    //         break;
    //     } else if (key == ' ') {
    //         detector.toggleMode();
    //     }
    // }

    return 0;
}
