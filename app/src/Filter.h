#ifndef CFXRBJFILTER 
#define CFXRBJFILTER
#include <math.h>
#define LOWPASS		0 
#define HIPASS		1
#define BANDPASS	2


#ifndef TWOPI
#define TWOPI 6.283185307179586f
#endif
// 2pi / SAMPLERATE (44100)
#define TWOPIBYFS 1.424758573057e-04
#define HI		2
#define MID		1
#define LO		0

#define LUT_SIZE 4096
#define PIBY2 1.570796327f


#ifndef ABS
#define ABS(A) (A>0?A:-A)
#endif
class Filter
	{
	public:
		static float LUT[LUT_SIZE];
		static bool LUTGenerated;
		float LUTBYTWOPI;
		float amp;
		Filter()
		{
			amp = 0;
			
			// reset filter coeffs
			b0a0=b1a0=b2a0=a1a0=a2a0=0.0;
			
			// reset in/out history
			ou1=ou2=in1=in2=0.0f;
			q = 3;
			type = LO;
			if(!LUTGenerated) {
				LUTGenerated = true;
				for(int i = 0; i < LUT_SIZE; i++) {
					LUT[i] = sin(TWOPI*(float)i/LUT_SIZE);
				}
			}
			LUTBYTWOPI = (float)LUT_SIZE/TWOPI;
		}
		
		float fastSin(float phase) {
			return LUT[(int)floor(phase*LUTBYTWOPI)];
		}

		
		float yn;
		float filter(float in0)
		{
			// filter
			//float const yn = b0a0*in0 + b1a0*in1 + b2a0*in2 - a1a0*ou1 - a2a0*ou2;
			yn = (b0*in0 + b1*in1 + b2*in2 - a1*ou1 - a2*ou2)/a0;
			// push in/out buffers
			in2=in1;
			in1=in0;
			ou2=ou1;
			ou1=yn;
			
			// return output
			return yn;
		};
		
		/**
		 * Filters a whole buffer of signal.
		 */
		float filter(float *ins, int length) {
			for(int i = 0; i < length; i++) {
				// filter
				//float const yn = b0a0*in0 + b1a0*in1 + b2a0*in2 - a1a0*ou1 - a2a0*ou2;
				yn = (b0*ins[i] + b1*in1 + b2*in2 - a1*ou1 - a2*ou2)/a0;
				// push in/out buffers
				in2=in1;
				in1=ins[i];
				ou2=ou1;
				ou1=yn;
				
				// return output
				ins[i] = yn;
				if(amp<ABS(ins[i])) amp = ins[i];
				else amp *= 0.999;
			}
			return amp;
		};

		/**
		 * Just calculates the amplitude, doesn't actually 
		 * process the signal.
		 */
		float calculateAmplitude(float *ins, int length) {
			for(int i = 0; i < length; i++) {
				// filter
				//float const yn = b0a0*in0 + b1a0*in1 + b2a0*in2 - a1a0*ou1 - a2a0*ou2;
				yn = (b0*ins[i] + b1*in1 + b2*in2 - a1*ou1 - a2*ou2)/a0;
				// push in/out buffers
				in2=in1;
				in1=ins[i];
				ou2=ou1;
				ou1=yn;
				
				if(amp<ABS(yn)) amp = yn;
				else amp *= 0.999;
			}
			return amp;
		};

		
		
		
		int type;
		float q;
		
		float tsin, tcos, omega;
		
		void calc_filter_coeffs(float const frequency)
		{
			
			
			omega	=	TWOPIBYFS*frequency;
			tsin	=	fastSin(omega);
			
			// fast cos
			omega += PIBY2;
			if(omega>TWOPI) omega -= TWOPI;
			tcos	=	fastSin(omega);

			alpha=tsin/q;

			a0=1.0+alpha;
			
			if(type==LO) {
				b0 = b2 = (1.0-tcos)/2.0;
				b1=1.0-tcos;
				a1=-2.0*tcos;
				a2=1.0-alpha;
			} else if(type==HI) {
				b0=b2=(1.0+tcos)/2.0;
				b1=-(1.0+tcos);

				a1=-2.0*tcos;
				a2=1.0-alpha;
			}else if(type==MID) {
				b0=tsin/2.0;
				b1=0.0;
				b2=-tsin/2;

				a1=-2.0*tcos;
				a2=1.0-alpha;
			}
		};
		

		
		// filter coeffs
		float b0a0,b1a0,b2a0,a1a0,a2a0;
		
		// in/out history
		float ou1,ou2,in1,in2, b0, b1, b2, a0, a1, a2, alpha;
	};
#endif