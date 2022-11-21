/// @file

#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>

#include "wavfile_mono.h"
#include "pitch_analyzer.h"

#include "docopt.h"

#define FRAME_LEN   0.030 /* 30 ms. */
#define FRAME_SHIFT 0.015 /* 15 ms. */

using namespace std;
using namespace upc;

static const char USAGE[] = R"(
get_pitch - Pitch Estimator 

Usage:
    get_pitch [options] <input-wav> <output-txt>
    get_pitch (-h | --help)
    get_pitch --version

Options:
    -r REAL, --r2maxth=REAL  Normalized Autocorrelation 2nd Max Threshold.[default: 0.43]
    -c REAL, --clipmult=REAL  Clipping max multiplier [default: 0.01]

    -h, --help  Show this screen
    --version   Show the version of the project

Arguments:
    input-wav   Wave file with the audio signal
    output-txt  Output file: ASCII file with the result of the estimation:
                    - One line per frame with the estimated f0
                    - If considered unvoiced, f0 must be set to f0 = 0
)";
float abs_f(float value){
  if (value < 0.0)
    return -1.0*value;
  return value;
}
int main(int argc, const char *argv[]) {
	/// \TODO 
  /// \DONE
	///  Modify the program syntax and the call to **docopt()** in order to
	///  add options and arguments to the program.
  /*
      -1 REAL, --r1r0th=REAL  Autocorrelation r[1]/r[0] relation Threshold. [default: 0.9]
    -z REAL, --zcrth=REAL  ZCR Threshold. [default: 1.0]
  */
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
        {argv + 1, argv + argc},	// array of arguments, without the program name
        true,    // show help if requested
        "2.0");  // version string

	std::string input_wav = args["<input-wav>"].asString();
	std::string output_txt = args["<output-txt>"].asString();
  float r2maxth = stof(args["--r2maxth"].asString());
  float clipmult = stof(args["--clipmult"].asString());
  float r1r0th = 0;//stof(args["--r1r0th"].asString());
  float zcrth = 0;//stof(args["--zcrth"].asString());

  // Read input sound file
  unsigned int rate;
  vector<float> x;
  if (readwav_mono(input_wav, rate, x) != 0) {
    cerr << "Error reading input file " << input_wav << " (" << strerror(errno) << ")\n";
    return -2;
  }

  int n_len = rate * FRAME_LEN;
  int n_shift = rate * FRAME_SHIFT;

  // Define analyzer
  PitchAnalyzer analyzer(n_len, rate, PitchAnalyzer::RECT, 50, 500, r2maxth,r1r0th, zcrth);

  /// \TODO
  /// \DONE
  /// Preprocess the input signal in order to ease pitch estimation. For instance, central-clipping or low pass filtering may be used.

  std::vector<float>::iterator iX;

  // CLIPPING
  // Iterate for each frame and save values in f0 vector
  // Get Max value in magnitude, either positive or negative 
  float Cl = -1.0;
  for(iX = x.begin(); iX < x.end(); ++iX){    
    Cl = std::max(Cl, abs_f(*iX));
  }
  Cl = clipmult * Cl;

  for(iX = x.begin(); iX < x.end(); ++iX){
    if(abs_f(*iX)<Cl)      
      *iX = 0;    
    else{
      if(*iX > 0)        
        *iX = *iX - Cl;
      else        
        *iX = *iX + Cl;    
    } 
  }

  vector<float> f0;
  float f, aux, zcr=0; 
  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift) {
    //Implement code to pass its original ZCR value.
    f = analyzer(iX, iX + n_len, zcr);
    f0.push_back(f);
  }

  // JUST ODD NUMBERS  
  int F_size = 1;  
  vector<float> filter; 
  
  /// \TODO
  /// \DONE
  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.
  for(iX = f0.begin(); iX < f0.end() - (F_size - 1); ++iX){
    // fill filter    
    for(int i = 0; i<F_size; i++)      
      filter.push_back(*(iX+i));
    // Order filter  ​
    int k, l;

    for(k = 0; k < F_size-1; k++){
      for(l = 0; l < F_size-k-1; l++){
        if (filter[l] > filter[l+1]){        
          aux = filter[l];        
          filter[l] = filter[l+1]; 
          filter[l+1] = aux;      
        }    
      }   
    }
    // Get median    
    f0[iX - f0.begin()] = filter[F_size/2];
    filter.clear();  
  } 
  // Write f0 contour into the output file
  ofstream os(output_txt);
  if (!os.good()) {
    cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
    return -3;
  }

  os << 0 << '\n'; //pitch at t=0
  for (iX = f0.begin(); iX != f0.end(); ++iX) 
    os << *iX << '\n';
  os << 0 << '\n';//pitch at t=Dur

  return 0;
}
