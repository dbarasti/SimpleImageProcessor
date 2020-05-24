#!/bin/sh
clear
echo "Running benchmark "

OMP_ExecutablePath="builds/img_processing_OMP" # omp implementation
ST_ExecutablePath="builds/img_processing_stdThreads" # std threads implementation
filenameRoot="plotting-and-data/data/benchmark"
imgDir="/home/marcod/brina"
maxThreads=250

for iter in 10; do #number of elements
  echo iters: ${iter}
  for exec in ${OMP_ExecutablePath} ${ST_ExecutablePath}; do # for each version (MasterWorker and ParallelFor)
    if [ "${exec}" = "${OMP_ExecutablePath}"  ]; then echo \"OMP\">> ${filenameRoot}-${iter}.txt; fi #print the heading
    if [ "${exec}" = "${ST_ExecutablePath}" ]; then echo \"stdThreads\">> ${filenameRoot}-${iter}.txt; fi #print the heading
    : $((w = 1))
    while [ $((w <= maxThreads)) -ne 0 ]; do #for each thread configuration
      echo $w
      ./${exec} ${imgDir} ${iter} ${w} >>${filenameRoot}-${iter}.txt
      : $((w = w + 1))
      sleep 0.5s
    done
    echo >>${filenameRoot}-${iter}.txt # separate the two datasets in the file
    echo >>${filenameRoot}-${iter}.txt
  done
  echo
done

echo "Benchmark ended"
