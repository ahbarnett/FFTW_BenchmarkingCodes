g++  -o fftwBenchmark -fopenmp fftwBenchmark.cpp ~/work/finufft/src/utils.cpp -I /usr/include -I ~/work/finufft/src -I /usr/lib/gcc/x86_64-linux-gnu/8/include -L /usr/lib/x86_64-linux-gnu -L ~/work/finufft/lib/ -lfftw3_omp -lfftw3 -lfinufft -lgomp -lm
