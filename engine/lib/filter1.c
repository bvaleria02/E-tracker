#include "filestructs.h"
#include "subFunctions.h"
  #define filter filterData[0]

extern struct __FILTER_STRUCT__ filterData[4];

// *** FILTER 1 ***

short filter1(){
  // multiplies input sample by input volume
  short sample = (filter.__in * filter.inSample) / 256;

  // Your filter algorythm starts here
      __audioBuffer__[0][filter.counter] = (short)(0.9 * (float)(sample +   __audioBuffer__[0][(filter.counter - 7200) & 0x7FFF] +  __audioBuffer__[0][filter.counter]));
      sample = __audioBuffer__[0][filter.counter];

      filter.counter++;
      if (filter.counter > 7199) filter.counter -= 7199;

  // output stage: Don't change unless you know what you're doing
  // This copies the sample into the previous sample, and into other filters.
  filterData[1].inSample += (sample * filter.filterOut[1]) / 256;
  filterData[2].inSample += (sample * filter.filterOut[2]) / 256;
  filterData[3].inSample += (sample * filter.filterOut[3]) / 256;
  filter.previousSample = sample;
  filter.inSample = 0;
  return (sample * filter.__out) / 256;
}
