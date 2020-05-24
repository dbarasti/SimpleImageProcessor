# Simple Image Processor

This is the fourth assignment for the class of [Parallel and Distributed Systems](http://didawiki.di.unipi.it/doku.php/magistraleinformaticanetworking/spm/sdpm09support) @Unipi. I developed two implementations for the problem of applying two openCV filters to a series of images, using different patterns.  

The two parallel implementations are:
* **Standard C++ Threads** (see file [ImageProcessor.hpp](https://github.com/dbarasti/SimpleImageProcessor/blob/master/ImageProcessor.cpp)) and
* **OMP Parallel For** (see file [ImageProcessor.cpp](https://github.com/dbarasti/SimpleImageProcessor/blob/master/ImageProcessor.cpp))
  

## Getting Started

To build the project simply clone the repo and build it using the following commands.


### Building
To generate the executable:  
```
g++ -std=c++17 -O3 -I <openCV includes> -L <openCV library> main.cpp -o main -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -fopenmp -pthread
```
in the root of the cloned project.  

e.g.  (if you are running it from the xeon-phi machine @Unipi)
```
g++ -std=c++17 -O3 -I /home/marcod/opencv4/include/opencv4 -L /home/marcod/opencv4/lib main.cpp -o main -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -fopenmp -pthread
```

No makefile this time, due to some problems with linking the openCV library.


### Running
The executable can be found in the root directory of the project. Just run it with:

```./main <input_folder> <n_iterations> <nWorkers>``` 


## Results
The experience results are summarized in some plots that express the scalability and the speedup obtained running the code on the Xeon-phi machine.  
The benchmark depicts the scenario of **applying the two filters** to 300 images. 

For the scenario I computed the execution time considering 1 up to 250 threads. Following are illustrated the plots with the metrics described. 

![](plotting-and-data/img/speed10iter.png)

![](plotting-and-data/img/scal10iter.png)


## Plotting

To obtain the data used in the results shown you have to build the project in two different ways: the first by leaving in ```main.cpp```  just the execution of the parallelOMP version and the second by executing only the parallelThreads version. Refer to [main.cpp](https://github.com/dbarasti/SimpleImageProcessor/blob/master/main.cpp) to understand what this means in practice.  
The two executables obtained must be put in a folder called ```builds/``` in order for the ```benchmark.sh``` script to work properly. These two executable should be called respectively ```img_processing_OMP``` and ```img_processing_stdThreads```.  

*To change the namings just edit the variables in *```benchmark.sh```.

Running the script with ```./benchmark.sh``` will generate the data for plotting in the folder ```plotting-and-data/data/```. To obtain the plots just open [Gnuplot](http://www.gnuplot.info/) in the folder ```plotting-and-data/``` and load the scripts with

```load "<script-name.gp>"```
 
    
## Acknowledgments
This project was developed for an assignment of the course [Parallel and Distributed Systems](http://didawiki.di.unipi.it/doku.php/magistraleinformaticanetworking/spm/sdpm09support) at University of Pisa under the guide of [Prof. Marco Danelutto](http://calvados.di.unipi.it/paragroup/danelutto/) and [Prof. Massimo Torquati](http://calvados.di.unipi.it/paragroup/torquati/).

