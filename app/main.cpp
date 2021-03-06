/*
 * MIT License
 * 
 * Copyright (c) 2018 Anirudh Topiwala
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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
    // Project starts
    std::cout << "This is a Human Detection Project." << std::endl;
    std::cout << "It uses a SVM classifier trained using " <<
                 "HOG features to detect Humans." << std:: endl;
    std::cout << std::endl;

    // Define a window/image size
    cv::Size windowSize = cv::Size(96, 160);

    // Train or Test a classifier
    std::cout << "Do you want to Train a Classifier? (y/n): ";
    std::string trainDetector;
    getline(std::cin, trainDetector);
    std::transform(trainDetector.begin(), trainDetector.end(),
                   trainDetector.begin(), ::tolower);
    if (trainDetector == "yes" || trainDetector == "y") {
        // Set directory for Positive Images
        std::cout << "Give the Path for Positive Training Images " <<
        "(Default is set as '../data/INRIAPerson/Train/pos/'): ";
        std::string posDir;
        getline(std::cin, posDir);
        if (posDir.empty())
            posDir = "../data/INRIAPerson/Train/pos/";

        // Set directory for Annotations
        std::cout << "Give the Path for Annotations for Positive " <<
                     "Training Images (Default is set as " <<
                     "'../data/INRIAPerson/Train/annotations/'): ";
        std::string annotationPath;
        getline(std::cin, annotationPath);
        if (annotationPath.empty())
            annotationPath = "../data/INRIAPerson/Train/annotations/";

        // Set directory for Negative Images
        std::cout << "Give the Path for Negative Training Images " <<
        "(Default is set as '../data/INRIAPerson/Train/neg/'): ";
        std::string negDir;
        getline(std::cin, negDir);
        if (negDir.empty())
            negDir = "../data/INRIAPerson/Train/neg/";

        // Load positive images
        Data allData;
        std::cout << "Loading Positive Images" << std::endl;
        allData.loadPosImages(static_cast<cv::String>(annotationPath),
                            static_cast<cv::String>(posDir), windowSize, true);
        // Check if images were successfully loaded
        if (allData.posImgList.size() > 0) {
            std::cout << "Loading Positive Training Data Complete" << std::endl;
        } else {
            std::cout << "No images found. " <<
                    "Please check the Path Directory: " << posDir << std::endl;
            return 0;
        }

        // Now load negative images
        std::cout << "Loading Negative Images" << std::endl;
        allData.loadNegImages(static_cast<cv::String>(negDir), windowSize);
        if (allData.negImgList.size() > 0) {
            std::cout << "Loading Negative Training Data Complete" << std::endl;
        } else {
            std::cout << "No images found. " <<
                    "Please check the Path Directory: " << negDir << std::endl;
            return 0;
        }

        // Start with the training
        Train trainClass;
        // For Positive Images
        std::cout << "Extracting HOG features and storing in a " <<
                     "vector for Positive Images" << std::endl;
        trainClass.getHOGfeatures(windowSize, allData.posImgList);
        // Assign Positive labels
        size_t positiveCount = trainClass.gradientList.size();
        trainClass.labels.assign(positiveCount, 0);
        std::cout << "Done getting HOG Features for Positive Images" <<
                                                                std::endl;

        // For Negative Images
        std::cout << "Extracting HOG features and storing in a " <<
                     "vector for Negative Images" << std::endl;
        trainClass.getHOGfeatures(windowSize, allData.negImgList);
        // Assign Negative labels
        size_t negativeCount = trainClass.gradientList.size() - positiveCount;
        trainClass.labels.insert(trainClass.labels.end(), negativeCount, 1);
        std::cout << "Done getting HOG Features for Negative Images" <<
                                                                std::endl;

        // Ask the user if the classifier is to be saved
        std::cout << "Do you want to Save the Trained Classifier? (y/n): ";
        std::string saveClassifier;
        getline(std::cin, saveClassifier);
        std::transform(saveClassifier.begin(), saveClassifier.end(),
                       saveClassifier.begin(), ::tolower);
        if (saveClassifier == "yes" || saveClassifier == "y") {
            // Get path for the classifier
            std::cout << "Give the Path for the Location where the " <<
            "Classifier is to be Saved (Default is set as " <<
            "'../data/classifier/svmClassifier'): ";
            std::string svmName;
            getline(std::cin, svmName);
            if (svmName.empty())
                svmName = "../data/classifier/svmClassifier";

            // Start training the SVM classifier
            trainClass.trainSVM(true, svmName);
        } else if (saveClassifier == "no" || saveClassifier == "n") {
            // Start training the SVM classifier
            trainClass.trainSVM(false, "");
        } else {
            std::cout << "Invalid Input" << std::endl;
            return 0;
        }

        // Set directory for Testing Images
        std::cout << "Give the Path for Testing Images " <<
        "(Default is set as '../data/INRIAPerson/Test/pos/'): ";
        std::string testDir;
        getline(std::cin, testDir);
        if (testDir.empty())
            testDir = "../data/INRIAPerson/Test/pos/";

        // Test Classifier
        Detect detector;
        detector.hog_user.winSize = windowSize;
        detector.hog_user.cellSize = cv::Size(4, 4);
        detector.hog_user.setSVMDetector(trainClass.getClassifier());
        cv::Rect r = detector.testClassifier(static_cast<cv::String>(testDir),
                                             windowSize, true, "User");
        std::cout << "Program Finshed" << std::endl;
    } else if (trainDetector == "no" || trainDetector == "n") {
        // Ask the user if Default classifier is to be used
        std::cout << "Do yo want to use OpenCV's Default " <<
                     "People Detector Classifier? (y/n): ";
        std::string defaultClassifier;
        getline(std::cin, defaultClassifier);
        std::transform(defaultClassifier.begin(), defaultClassifier.end(),
                       defaultClassifier.begin(), ::tolower);
        if (defaultClassifier == "yes" || defaultClassifier == "y") {
            // Set directory for Testing Images
            std::cout << "Give the Path for Testing Images " <<
            "(Default is set as '../data/test/pos/'): ";
            std::string testDir;
            getline(std::cin, testDir);
            if (testDir.empty())
                testDir = "../data/test/pos/";

            // Test Classifier
            Detect detector;
            cv::Rect r = detector.testClassifier(testDir, cv::Size(),
                                                 true, "Default");
            std::cout << "Finshed" << std::endl;
        } else if (defaultClassifier == "no" || defaultClassifier == "n") {
            // Ask the user for the path to the classifier
            std::cout << "Give the Path for the Classifier " <<
            "(Default is set as '../data/classifier/svmClassifier'): ";
            std::string svmName;
            getline(std::cin, svmName);
            if (svmName.empty())
                svmName = "../data/classifier/svmClassifier";

            // Initialize the classifier
            Train trainClass;
            trainClass.classifier = cv::ml::SVM::load(svmName);

            // Set directory for Testing Images
            std::cout << "Give the Path for Testing Images " <<
            "(Default is set as '../data/test/pos/'): ";
            std::string testDir;
            getline(std::cin, testDir);
            if (testDir.empty())
                testDir = "../data/test/pos/";

            // Test Classifier
            Detect detector;
            detector.hog_user.winSize = windowSize;
            detector.hog_user.cellSize = cv::Size(4, 4);
            detector.hog_user.setSVMDetector(trainClass.getClassifier());
            cv::Rect r = detector.testClassifier(testDir, windowSize,
                                                 true, "User");
            std::cout << "Finshed" << std::endl;
        } else {
            std::cout << "Invalid Input" << std::endl;
            return 0;
        }
    } else {
        std::cout << "Invalid Input" << std::endl;
        return 0;
    }
    return 0;
}
