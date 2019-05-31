#include <utils.h>
#include <fftw3.h>
#include <math.h>
#include <iostream>


/*A Preliminary FFTW benchmark driver that compares
  performance running n fftw executes with either a
  single plan or n plans.  

*/
int main(int argc, char ** argv){

  //Baked in parameters:
  //DOUBLE PRECISION
  //SINGLE THREADED - to do 
  //SINGLE DIMENSION - to do

  if(argc < 4){

    std::cout << "Usage: \n ./fftwBenchMark [n_iterations] [FFTW_PLAN_TYPE] [SIZE] \n for plan types {0:FFTW_MEASURE, 1:FFTW_ESTIMATE}";
    std::cout << std::endl ;
    exit(1);

  }

  unsigned int fftwPlanType;

  int n_iters = std::stoi(argv[1]); 
  
  switch(std::stoi(argv[2])){
  case 0:
    fftwPlanType = FFTW_MEASURE;
    break;
  case 1:
    fftwPlanType = FFTW_ESTIMATE;
    break;
  default: //incorrect usage- default to estimate
    fftwPlanType = FFTW_ESTIMATE;
  }
  
  CNTime timer; 

  size_t size(std::stoi(argv[3]));

  fftw_complex *in, *out;
  fftw_plan p;

  in = fftw_alloc_complex(size); 
  out = fftw_alloc_complex(size);

  std::cout << "*---------------------------------------------------------------------*" << std::endl;
  std::string planType;
  std::string measureString("FFTW_MEASURE");
  std::string estimateString("FFTW_ESTIMATE");
  fftwPlanType == 0 ? planType = measureString : planType = estimateString;
  std::cout << "Executing " << n_iters << " iterations with a shared plan of type " << planType << std::endl; 

  timer.start();
  p = fftw_plan_dft_1d(size, in, out, FFTW_FORWARD, fftwPlanType);
  
  double planConstruction1 = timer.elapsedsec();

  std::cout << "Time for plan construction: " << planConstruction1 << std::endl;

  unsigned int dummy = 1;
  //initialize data after initializing the plan
  for(int i = 0; i < size; i++){
    in[i][0] = M_PI*randm11r(&dummy);
    in[i][1] = M_PI*randm11r(&dummy);
  }
  

  double executeTime;
  double totalExecTime1{0};
  for(int i = 0; i < n_iters; i++){
    timer.restart();
    fftw_execute(p);
    executeTime = timer.elapsedsec();
    totalExecTime1 += executeTime;

  }
  std::cout << "Total Execution Time: " << totalExecTime1 << std::endl; 
  std::cout << "Final Total for this trial: " << planConstruction1 + totalExecTime1 << std::endl;
  fftw_destroy_plan(p);
  fftw_free(in); fftw_free(out); // no error checking yet


  //Remove the advantage of collected wisdom
  FFTW_FORGET_WISDOM();

  std::cout << "*---------------------------------------------------------------------*" << std::endl;
  std::cout << "Executing " << n_iters << " iterations, initializing a new " << planType << " plan each time " << std::endl;

  fftw_complex *in2, *out2;
  fftw_plan p2;
  in2 = fftw_alloc_complex(size); 
  out2 = fftw_alloc_complex(size);

  double planConstruction2;
  double totalPlanConstruction2{0};
  double totalExecTime2{0};

  for(int i = 0; i < n_iters; i++){
    timer.restart();
    p2 = fftw_plan_dft_1d(size, in2, out2, FFTW_FORWARD, FFTW_MEASURE);
    planConstruction2 = timer.elapsedsec();
    if(i == 0){
      std::cout << "Time in first call to construct plan: " << planConstruction2 << std::endl;
     }
    totalPlanConstruction2 += planConstruction2; 

    //initialize data for this round
    for(int k = 0; k < size; k++){
      in2[k][0] = M_PI*randm11r(&dummy);
      in2[k][1] = M_PI*randm11r(&dummy);
    }

    timer.restart();
    fftw_execute(p2);
    executeTime = timer.elapsedsec();
    totalExecTime2 += executeTime; 

    
  }
  std::cout << "Total Time for Construction of all plans: " << totalPlanConstruction2 << std::endl;
  std::cout << "Total Time for Execution " << totalExecTime2 << std::endl;
  std::cout << "Final Total for this Trial: " << totalPlanConstruction2 + totalExecTime2 << std::endl;
  
  fftw_destroy_plan(p2);
  fftw_free(in2); fftw_free(out2);

  FFTW_FORGET_WISDOM();

}
