#include "filestructs.h"
#include "subFunctions.h"
  #define filter filterData[2]

extern struct __FILTER_STRUCT__ filterData[4];

// *** FILTER 3 ***

short filter3(){
  // multiplies input sample by input volume
  short sample = (filter.__in * filter.inSample) / 256;


  // output stage: Don't change unless you know what you're doing
  // This copies the sample into the previous sample, and into other filters.
  filterData[0].inSample += (sample * filter.filterOut[0]) / 256;
  filterData[1].inSample += (sample * filter.filterOut[1]) / 256;
  filterData[3].inSample += (sample * filter.filterOut[3]) / 256;
  filter.previousSample = sample;
  filter.inSample = 0;
  return (filter.inSample * filter.__out) / 256;
}
