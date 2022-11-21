/// @file

#include <iostream>
#include <math.h>
#include "pitch_analyzer.h"

using namespace std;

/// Name space of UPC
namespace upc {
  void PitchAnalyzer::autocorrelation(const vector<float> &x, vector<float> &r) const {

    for (unsigned int l = 0; l < r.size(); ++l) {
  		/// \TODO Compute the autocorrelation r[l]
      /// \DONE Autocorrelation computed
      /// - Inicialitzem l'autocorrelació a 0
      /// - Afegim el producte
      /// - Dividim per la durada
      /// *** TAXANNNN ***
      r[l]=0;
      for(unsigned int n=l; n< x.size(); n++){
        r[l] += x[n]*x[n-l];
      }
      r[l] = r[l]/x.size();
    }

    if (r[0] == 0.0F) //to avoid log() and divide zero 
      r[0] = 1e-10; 
  }

  void PitchAnalyzer::set_window(Window win_type) {
    if (frameLen == 0)
      return;

    window.resize(frameLen);

    switch (win_type) {
    case HAMMING:
      /// \TODO Implement the Hamming window
      /// \DONE Formula aplicada
      for(unsigned int i=0; i<frameLen; i++){
        window[i] = 0.54 - 0.46*cos((2*M_PI*i)/(frameLen-1));
      }
      break;
    case RECT:
    default:
      window.assign(frameLen, 1);
    }
  }

  void PitchAnalyzer::set_f0_range(float min_F0, float max_F0) {
    npitch_min = (unsigned int) samplingFreq/max_F0;
    if (npitch_min < 2)
      npitch_min = 2;  // samplingFreq/2

    npitch_max = 1 + (unsigned int) samplingFreq/min_F0;

    //frameLen should include at least 2*T0
    if (npitch_max > frameLen/2)
      npitch_max = frameLen/2;
  }

  bool PitchAnalyzer::unvoiced(float zcr, float r1norm, float rmaxnorm) const {
    /// \TODO Implement a rule to decide whether the sound is voiced or not.
    /// * You can use the standard features (pot, r1norm, rmaxnorm),
    ///   or compute and use other ones.


    int voiced = 0;                 //Sum of conditions
    if(rmaxnorm > r2maxth) voiced++;//2nd auto.r. max > threshold, voiced
    if(r1norm > r1r0th) voiced++;   //2nd sample ~= than 1st, voiced
    if(zcr < zcrth) voiced++;       //if low zcr, voiced.
  
    if(voiced >= 2) return false;   //Return false if voiced.
    return true;                    // Return true if unvoiced.
  }

  float PitchAnalyzer::compute_pitch(vector<float>& x, float zcr) const {
    if (x.size() != frameLen)
      return -1.0F;

    //Window input frame
    for (unsigned int i=0; i<x.size(); ++i)
      x[i] *= window[i];

    vector<float> r(npitch_max);

    //Compute correlation
    autocorrelation(x, r);

    std::vector<float>::iterator iR = r.begin(), iRMax = iR;

    /// \TODO 
	/// Find the lag of the maximum value of the autocorrelation away from the origin.<br>
	/// Choices to set the minimum value of the lag are:
	///    - The first negative value of the autocorrelation.
	///    - The lag corresponding to the maximum value of the pitch.
    ///	   .
	/// In either case, the lag should not exceed that of the minimum value of the pitch.
    for(iRMax = iR = r.begin() + npitch_min; iR< r.begin()+ npitch_max; iR++){ //Com que el màxim ha d'estar entre en màx i el mín pitch fem el for que recorri aquest espai
      if(*iR>*iRMax){
        iRMax= iR;   //Actualitzem la direcció del màxim
      }
    }
    unsigned int lag = iRMax - r.begin();

    //float pot = 10 * log10(r[0]);

    //You can print these (and other) features, look at them using wavesurfer
    //Based on that, implement a rule for unvoiced
    //change to #if 1 and compile
#if 0
    if (r[0] > 0.0F)
      cout << pot << '\t' << r[1]/r[0] << '\t' << r[lag]/r[0] << endl;
#endif
    
    if (unvoiced(zcr, r[1]/r[0], r[lag]/r[0]))
      return 0;
    else
      return (float) samplingFreq/(float) lag;
  }
}