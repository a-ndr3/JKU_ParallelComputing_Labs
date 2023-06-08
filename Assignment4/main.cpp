#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <future>
#include <vector>
#include <algorithm>
#include <execution>

using namespace cv;

//This function implements the business logic code. The recommendation is to avoid changes here
void transformation(const cv::Mat& inputImage, cv::Mat& outputImage)
{
    cv::Mat grayImage;
    cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);

    cv::Mat edges;
    cv::Canny(grayImage, edges, 50, 200);

    Mat image_blurred;
    GaussianBlur(edges, image_blurred, Size(5, 5), 0);

    std::vector<cv::Vec2f> lines;
    cv::HoughLines(image_blurred, lines, 1, CV_PI / 180, 150);

    for (size_t i = 0; i < lines.size(); ++i) {
        float rho = lines[i][0];
        float theta = lines[i][1];
        double a = std::cos(theta);
        double b = std::sin(theta);
        double x0 = a * rho;
        double y0 = b * rho;
        cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
        cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
        cv::line(outputImage, pt1, pt2, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }
}

void processImage(const std::filesystem::path& inputPath, const std::filesystem::path& outputDirectoryPath)
{	
    cv::Mat inputImage = cv::imread(inputPath.string());
    cv::Mat outputImage = inputImage.clone();
    transformation(inputImage, outputImage);
    std::string outputFilePath = outputDirectoryPath.string() + "/output_" + inputPath.filename().string();
    cv::imwrite(outputFilePath, outputImage);
}


int main()
{
    std::string directoryPath = "input";
    std::filesystem::path outputDirectoryPath = "output";

    std::vector<std::filesystem::path> paths;
    for (const auto & entry : std::filesystem::directory_iterator(directoryPath))
        paths.push_back(entry.path());

    auto t_start = std::chrono::high_resolution_clock::now();
    std::for_each(std::execution::par, paths.begin(), paths.end(),
                  [&outputDirectoryPath](const std::filesystem::path& p){
                      processImage(p, outputDirectoryPath);
                  }
    );
    auto t_end = std::chrono::high_resolution_clock::now();

    std::cout << "OVERALL Time(seconds): "  << std::chrono::duration<double>(t_end-t_start).count() << std::endl;

    return 0;
}