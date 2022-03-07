#include "filestructs.h"
#include "subFunctions.h"
#include <stdio.h>
typedef unsigned char BYTE;
  #define __minSampleLength 1600
  #define __channelCount 1
  #define currentChannel channel[i]
  #define channelInstrument instrument[channel[i].instrument]
  #define elif else if
  #define key_transpose (channel[i].transpose + (channel[i].octave * 12) + instrument[channel[i].instrument].transpose + (instrument[channel[i].instrument].transpose * 12))

extern const float note[];
extern struct __CHANNEL_STRUCT__ channel[16];
extern struct __INSTRUMENT_STRUCT__ instrument[128];
extern struct __MASTER_FILE_STRUCT__ fileMaster;
extern FILE *audioOut;
extern struct __FILTER_STRUCT__ filterData[4];

const float note[] =   {0.0218, 0.0231, 0.0245, 0.0259, 0.0275, 0.0291, 0.0308, 0.0327, 0.0346, 0.0367, 0.0388, 0.0412,
                        0.0436, 0.0462, 0.0489, 0.0519, 0.0549, 0.0582, 0.0617, 0.0653, 0.0692, 0.0733, 0.0777, 0.0823,
                        0.0872, 0.0924, 0.0979, 0.1037, 0.1099, 0.1164, 0.1233, 0.1307, 0.1384, 0.1467, 0.1554, 0.1646,
                        0.1744, 0.1848, 0.1958, 0.2074, 0.2198, 0.2328, 0.2467, 0.2613, 0.2769, 0.2933, 0.3108, 0.3293,
                        0.3488, 0.3696, 0.3916, 0.4148, 0.4395, 0.4656, 0.4933, 0.5227, 0.5537, 0.5867, 0.6216, 0.6585,
                        0.6977, 0.7392, 0.7831, 0.8297, 0.8790, 0.9313, 0.9867, 1.0453, 1.1075, 1.1733, 1.2431, 1.3170,
                        1.3953, 1.4783, 1.5662, 1.6593, 1.7580, 1.8626, 1.9733, 2.0906, 2.2150, 2.3467, 2.4862, 2.6340,
                        2.7907, 2.9566, 3.1324, 3.3187, 3.5160, 3.7251, 3.9466, 4.1813, 4.4299, 4.6933, 4.9724, 5.2681,
                        5.5813, 5.9132, 6.2648, 6.6374, 7.0321, 7.4502, 7.8932, 8.3626, 8.8598, 9.3867, 9.9448, 10.5362,
                        11.1627, 11.8265, 12.5297, 13.2748, 14.0641, 14.9004, 15.7864, 16.7251, 17.7197, 18.7733, 19.8897, 21.0724,
                        22.3254, 23.6529, 25.0594, 26.5495, 28.1282, 29.8008, 31.5729, 33.45039};

short renderSquare(int i);
short renderNoise(int i);
short renderWave(int i);
short renderSample(int i);

short sample_ADSDR(int i, short sample);


void audiorender(){
    short ind_sample = 0;
    short total_sample = 0;

    for(int s = 0; s < __minSampleLength; s++){

            total_sample = 0;
            for(int i = 0; i < __channelCount; i++){

                    switch(channelInstrument.type){
                          case  0 :   ind_sample = renderSquare(i);
                                      break;
                          case  1 :   ind_sample = renderNoise(i);
                                      break;
                          case  2 :   ind_sample = renderWave(i);
                                      break;
                          case  3 :   ind_sample = renderSample(i);
                                      break;
                                    };

                    ind_sample = sample_ADSDR(i, ind_sample);
                    currentChannel.counter++;
                    filterData[0].inSample += (ind_sample * currentChannel.filter[0]) / 256;
                    filterData[1].inSample += (ind_sample * currentChannel.filter[1]) / 256;
                    filterData[2].inSample += (ind_sample * currentChannel.filter[2]) / 256;
                    filterData[3].inSample += (ind_sample * currentChannel.filter[3]) / 256;
                    total_sample += (ind_sample * currentChannel.master) / 256;

            };

            total_sample = total_sample + filter1();
            total_sample = total_sample + filter2();
            total_sample = total_sample + filter3();
            total_sample = total_sample + filter4();
            fwrite(&total_sample, sizeof(short), 1, audioOut);
    };
    return;
}

short renderSquare(int i){
    short sample = 0;
    short inSample = 0;
    for(int j = 0; j < 4; j++){
        currentChannel.seed += note[currentChannel.key + key_transpose] / (float)4;
        inSample = ((((channelInstrument.morph > (unsigned char)currentChannel.seed)) << 8) - 0x40) << 4;
        sample += inSample;
    };
    return sample;
}
short renderNoise(int i){
    short sample = 0;
    int seed;
    currentChannel.seed += note[currentChannel.key + key_transpose] / (float)32;
    seed = (int)currentChannel.seed;
    seed = seed & ((1 << channelInstrument.morph)-1);
    for(int j = 0; j < 8; j++){
      seed = (seed << 4) - ((seed >> 24) ^ (seed >> 19) ^ (seed >> 13) ^ (seed >> 7) ^ (seed >> 11) ^ (seed >> 2) ^ (seed >> 0) ^ (seed >> 17));
    };
    return seed;
}
short renderWave(int i){
    short sample = 0;

    return sample;
}
short renderSample(int i){
    short sample = 0;

    return sample;
}

short sample_ADSDR(int i, short sample){
  int memoryCounter = 0;

  if (currentChannel.counter < channelInstrument.volumeEnvelope[0])       sample = (short)(sample * (float)(currentChannel.counter / (float)(channelInstrument.volumeEnvelope[0] + 1)));
    elif (currentChannel.counter < channelInstrument.volumeEnvelope[1])   sample = (short)sample * (float)(1 - (float)((currentChannel.counter - channelInstrument.volumeEnvelope[0]) / (float)(channelInstrument.volumeEnvelope[1] - channelInstrument.volumeEnvelope[0])) * ((float)(0xFFFF - channelInstrument.targetEG[0]) / 0xFFFF));
    elif (currentChannel.counter < channelInstrument.volumeEnvelope[2])   sample = (short)sample * ((float)(channelInstrument.targetEG[0]) / 0xFFFF);
    elif (currentChannel.counter < channelInstrument.volumeEnvelope[3])   sample = (short)sample * (float)(((float)(0xFFFF - channelInstrument.targetEG[0]) / 0xFFFF) - (float)((currentChannel.counter - channelInstrument.volumeEnvelope[2]) / (float)(channelInstrument.volumeEnvelope[3] - channelInstrument.volumeEnvelope[2])) * ((float)(channelInstrument.targetEG[0] - channelInstrument.targetEG[1]) / 0xFFFF));
    else                                                                  sample = (short)sample * ((float)(channelInstrument.targetEG[1]) / 0xFFFF);
  return sample;
}
