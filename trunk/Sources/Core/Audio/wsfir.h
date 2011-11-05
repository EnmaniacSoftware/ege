/*
 Windowed Sinc FIR Generator
 Bob Maling (BobM.DSP@gmail.com)
 Contributed to musicdsp.org Source Code Archive
 Last Updated: April 12, 2005
 
 Usage:
 Lowpass:	wsfirLP(h, N, WINDOW, fc)
 Highpass:	wsfirHP(h, N, WINDOW, fc)
 Bandpass:	wsfirBP(h, N, WINDOW, fc1, fc2)
 Bandstop:	wsfirBS(h, N, WINDOW, fc1, fc2)
 
 where:
 h (double[N]):	filter coefficients will be written to this array
 N (int):		number of taps
 WINDOW (int):	Window (W_BLACKMAN, W_HANNING, or W_HAMMING)
 fc (double):	cutoff (0 < fc < 0.5, fc = f/fs)
 --> for fs=48kHz and cutoff f=12kHz, fc = 12k/48k => 0.25
 
 fc1 (double):	low cutoff (0 < fc < 0.5, fc = f/fs)
 fc2 (double):	high cutoff (0 < fc < 0.5, fc = f/fs)
 
 
 Windows included here are Blackman, Hanning, and Hamming. Other windows	can be
 added by doing the following:
 1. "Window type constants" section: Define a global constant for the new window.
 2. wsfirLP() function: Add the new window as a case in the switch() statement.
 3. Create the function for the window
 
 For windows with a design parameter, such as Kaiser, some modification
 will be needed for each function in order to pass the parameter.
 */
#ifndef WSFIR_H
#define WSFIR_H

#include <EGEMath.h>
#include <EGEDynamicArray.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

// Function prototypes
void wsfirLP(DynamicArray<float32>& h, const s32 N, const int &WINDOW, const float32 fc);
void wsfirHP(DynamicArray<float32>& h, const s32 N, const int &WINDOW, const float32 fc);
void wsfirBS(DynamicArray<float32>& h, const s32 N, const int &WINDOW, const float32 fc1, const float32 fc2);
void wsfirBP(DynamicArray<float32>& h, const s32 N, const int &WINDOW, const float32 fc1, const float32 fc2);
void genSinc(DynamicArray<float32>& sinc, const s32 N, const float32 fc);
void wBlackman(DynamicArray<float32>& w, const s32 N);
void wHanning(DynamicArray<float32>& w, const s32 N);
void wHamming(DynamicArray<float32>& w, const s32 N);

// Window type contstants
const int W_BLACKMAN = 1;
const int W_HANNING = 2;
const int W_HAMMING = 3;

// Generate lowpass filter
// 
// This is done by generating a sinc function and then windowing it
void wsfirLP(DynamicArray<float32>& h,		// h[] will be written with the filter coefficients
			 const s32 N,		// size of the filter (number of taps)
			 const int &WINDOW,	// window function (W_BLACKMAN, W_HANNING, etc.)
			 const float32 fc)	// cutoff frequency
{
	int i;

  h.clear();

  DynamicArray<float32> w;
  DynamicArray<float32> sinc;

	//double *w = new double[N];		// window function
 //   double *sinc = new double[N];	// sinc function
    
	// 1. Generate Sinc function
	genSinc(sinc, N, fc);
    
	// 2. Generate Window function
	switch (WINDOW) {
		case W_BLACKMAN:	// W_BLACKMAN
			wBlackman(w, N);
			break;
		case W_HANNING:		// W_HANNING
			wHanning(w, N);
			break;
		case W_HAMMING:		// W_HAMMING
			wHamming(w, N);
			break;
		default:
			break;
	}
	
	// 3. Make lowpass filter
	for (i = 0; i < N; i++) {
		h.push_back(sinc[i] * w[i]);
	
    EGE_PRINT("%f", h[i]);
  }

  int size1 = h.size();
  int size2 = sinc.size();
  int size3 = w.size();

		
	return;
}

// Generate highpass filter
//
// This is done by generating a lowpass filter and then spectrally inverting it
void wsfirHP(DynamicArray<float32>& h,		// h[] will be written with the filter coefficients
			 const s32 N,		// size of the filter
			 const int &WINDOW,	// window function (W_BLACKMAN, W_HANNING, etc.)
			 const float32 fc)	// cutoff frequency
{
	int i;
	
	// 1. Generate lowpass filter
	wsfirLP(h, N, WINDOW, fc);
	
	// 2. Spectrally invert (negate all samples and add 1 to center sample) lowpass filter
	// = delta[n-((N-1)/2)] - h[n], in the time domain
	for (i = 0; i < N; i++) {
		h[i] *= -1.0;	// = 0 - h[i]
	}
	h[(N-1)/2] += 1.0;	// = 1 - h[(N-1)/2]
	
	return;
}

// Generate bandstop filter
//
// This is done by generating a lowpass and highpass filter and adding them
void wsfirBS(DynamicArray<float32>& h,		// h[] will be written with the filter taps
			 const s32 N,		// size of the filter
			 const int &WINDOW,	// window function (W_BLACKMAN, W_HANNING, etc.)
			 const float32 fc1,	// low cutoff frequency
			 const float32 fc2)	// high cutoff frequency
{
	int i;
  DynamicArray<float32> h1;
  DynamicArray<float32> h2;
	
	// 1. Generate lowpass filter at first (low) cutoff frequency
	wsfirLP(h1, N, WINDOW, fc1);
	
	// 2. Generate highpass filter at second (high) cutoff frequency
	wsfirHP(h2, N, WINDOW, fc2);
	
	// 3. Add the 2 filters together
	for (i = 0; i < N; i++) {
		h[i] = h1[i] + h2[i];
	}
		
	return;
}

// Generate bandpass filter
//
// This is done by generating a bandstop filter and spectrally inverting it
void wsfirBP(DynamicArray<float32>& h,		// h[] will be written with the filter taps
			 const s32 N,		// size of the filter
			 const int &WINDOW,	// window function (W_BLACKMAN, W_HANNING, etc.)
			 const float32 fc1,	// low cutoff frequency
			 const float32 fc2)	// high cutoff frequency
{
	int i;
	
	// 1. Generate bandstop filter
	wsfirBS(h, N, WINDOW, fc1, fc2);
	
	// 2. Spectrally invert bandstop (negate all, and add 1 to center sample)
	for (i = 0; i < N; i++) {
		h[i] *= -1.0;	// = 0 - h[i]
	}
	h[(N-1)/2] += 1.0;	// = 1 - h[(N-1)/2]
	
	return;
}

// Generate sinc function - used for making lowpass filter
void genSinc(DynamicArray<float32>& sinc,		// sinc[] will be written with the sinc function
			 const s32 N,		// size (number of taps)
			 const float32 fc)	// cutoff frequency
{
	int i;
	const double M = N-1;
	double n;
	
	// Constants
	const double PI = 3.14159265358979323846;
	
	// Generate sinc delayed by (N-1)/2
	for (i = 0; i < N; i++)
  {
		if (i == M/2.0) {
			sinc.push_back(2.0 * fc);
		}
		else
    {
			n = (double)i - M/2.0;
			sinc.push_back(Math::Sin(2.0*PI*fc*n) / (PI*n));
		}
	}        
	
	return;
}

// Generate window function (Blackman)
void wBlackman(DynamicArray<float32>& w,		// w[] will be written with the Blackman window
			   const s32 N)	// size of the window
{
	int i;
	const double M = N-1;
	const double PI = 3.14159265358979323846;
	
	for (i = 0; i < N; i++) {
		w.push_back(0.42 - (0.5 * cos(2.0*PI*(double)i/M)) + (0.08*cos(4.0*PI*(double)i/M)));
	}
	
	return;
}

// Generate window function (Hanning)
void wHanning(DynamicArray<float32>& w,		// w[] will be written with the Hanning window
			  const s32 N)		// size of the window
{
	int i;
	const double M = N-1;
	const double PI = 3.14159265358979323846;
	
	for (i = 0; i < N; i++) {
		w.push_back(0.5 * (1.0 - cos(2.0*PI*(double)i/M)));
	}
	
	return;
}

// Generate window function (Hamming)
void wHamming(DynamicArray<float32>& w,		// w[] will be written with the Hamming window
			  const s32 N)		// size of the window
{
	int i;
	const double M = N-1;
	const double PI = 3.14159265358979323846;
	
	for (i = 0; i < N; i++) {
		w.push_back(0.54 - (0.46*cos(2.0*PI*(double)i/M)));
	}
	
	return;
}

EGE_NAMESPACE_END

#endif
