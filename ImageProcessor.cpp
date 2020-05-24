//
// Created by dbara on 23/05/20.
//

#include <thread>

class ImageProcessor{
private:
    std::vector<std::string> filePaths;
    int iterations;
    int nWorkers;

    static void executeJob(std::vector<std::string>* paths, int start, int end){
        std::vector<cv::Mat> images(paths->size());
        for (int i = start; i < end; ++i) {
            images[i] = cv::imread((*paths)[i]);
            cv::GaussianBlur(images[i], images[i], cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
            cv::Sobel(images[i], images[i], -1, 1, 1);
        }
    }
public:
    ImageProcessor(std::string inputDir, int iter, int workers): iterations(iter), nWorkers(workers){
        for (int i = 0; i < iterations; i++) {
            for (const auto &entry : std::filesystem::directory_iterator(inputDir)) {
                filePaths.emplace_back(entry.path());
            }
        }
    }

    // to account for the separate timings for the two filters, in the sequential implementation I split the processing in two cycles
    void sequentialProcessing(){
        std::vector<cv::Mat> images(filePaths.size());
        std::vector<cv::Mat> processed(filePaths.size());

        auto imagesIterator = images.begin();
        // load images from disk in images vector
        for (std::string &img: filePaths) {
            *imagesIterator = cv::imread(img);
            imagesIterator++;
        }

        auto destinationIterator = processed.begin();
        auto startGaussian = std::chrono::high_resolution_clock::now();

        // apply GaussianBlur
        for (cv::Mat &currentImage: images) {
            cv::GaussianBlur(currentImage, *destinationIterator, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
            destinationIterator++;
        }
        auto endGaussian = std::chrono::system_clock::now();
        std::cout << "Sequential time Gaussian blur: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(endGaussian - startGaussian).count() << "ms"
                  << std::endl;

        auto startSobel = std::chrono::high_resolution_clock::now();

        // apply Sobel filter
        for (cv::Mat &currentImage: processed) {
            cv::Sobel(currentImage, currentImage, -1, 1, 1);
        }
        auto endSobel = std::chrono::system_clock::now();

        std::cout << "Sequential time Sobel filter: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(endSobel - startSobel).count() << "ms"
                  << std::endl;

        // saving to disk just one image, to check the correct application of the filters
        cv::Mat firstImage = *processed.begin();
        cv::imwrite("sequential_test.jpg", firstImage);
    }

    void parallelThreads(){
        // find the correct number of images for each thread to process
        int jobsSize = filePaths.size();
        int jobsForEach = jobsSize/nWorkers;
        int remainingJobs = jobsSize%nWorkers;

        int givenAway = 0;

        std::vector<std::thread> workerThreads(nWorkers);

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < nWorkers; ++i) {
            workerThreads.emplace_back(std::thread(&ImageProcessor::executeJob, &filePaths, givenAway, givenAway + jobsForEach + (remainingJobs>0?1:0)));
            givenAway += jobsForEach + (remainingJobs>0?1:0);
            if (remainingJobs != 0) remainingJobs--;
        }

        for (auto &th : workerThreads) {
            if (th.joinable())
                th.join();
        }

        auto end = std::chrono::system_clock::now();
        std::cout << nWorkers << " " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
    }

    void parallelOmp(){

        std::vector<cv::Mat> processed(filePaths.size());
        //since in the stdThreads implementation I have to account for the loading from disk of the images, here I also account for it, starting the timer before the disk loading
        auto start = std::chrono::high_resolution_clock::now();

        auto processedIterator = processed.begin();

        // apply filters
#pragma omp parallel for num_threads(nWorkers) schedule(dynamic, 1)
        for (std::string &currentFile: filePaths) {
            auto currentImage = cv::imread(currentFile);
            cv::GaussianBlur(currentImage, currentImage, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
            cv::Sobel(currentImage, *processedIterator, -1, 1, 1);
            processedIterator++;
        }

        auto end = std::chrono::system_clock::now();

        std::cout << nWorkers << " " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

        // saving to disk just one image, to check the correct application of the filters
        cv::Mat firstImage = *processed.begin();
        cv::imwrite("omp_test.jpg", firstImage);

    }
};
