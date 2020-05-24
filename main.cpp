#include <iostream>

// keep in mind that all or some of these includes
// may change depending on the library installation and version
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <filesystem>
#include <vector>
#include <functional>
#include <chrono>
#include "ImageProcessor.cpp"



int main(int argc, char * argv[]) {

    if(argc < 4){
        std::cerr << "use: " << argv[0]  << " source iterations workers \n";
        return -1;
    }

    std::string path = argv[1];
    int iterations = atoi(argv[2]);
    int nWorkers = atoi(argv[3]);

    auto processor = ImageProcessor(path, iterations, nWorkers);
    // processor.sequentialProcessing();
    // processor.parallelOmp();
    processor.parallelThreads();

    return 0;
}