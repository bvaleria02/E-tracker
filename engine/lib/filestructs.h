#include <stdio.h>
typedef unsigned char BYTE;

struct __CHANNEL_STRUCT__ {
                              BYTE  key;
                              BYTE  volume;
                              BYTE  instrument;
                              BYTE  waitFrames;
                              BYTE  tunning;
                              char  transpose;
                              char  octave;

                              BYTE  filter[4];
                              BYTE  master;

                              int  counter;
                              double seed;
                              BYTE  active;
                          } channel[16];

struct __INSTRUMENT_STRUCT__ {
                              BYTE type;      // square, noise, wave, sample;
                              char octave;
                              char transpose;

                              BYTE  morph;    // square: pulse width; noise: lsfr; wave: multiwave offset; sample: sample counter offset
                              BYTE  morphSelfIncrement;
                              BYTE  sample;           // noise: seed ofset; wave: wave; sample: sample;
                              int   sampleCounter[3]; // [0] = start, [1] = loop, [2] = end

                              int   volumeEnvelope[5]; // [0] = attack, [1] = decay1, [2] = sustain, [3] = decay2, [4] = release
                              int   targetEG[2];       // decay1 target, decay2 target.
                          } instrument[128];

struct __MASTER_FILE_STRUCT__ {
                              BYTE  volume;
                              BYTE  transpose;
                              BYTE  tempo;
                              short active;

                              BYTE  fileInit;
                              long  dataLength;
                              long  dataCursor;

                              long sampleWritten;
                          } fileMaster;

struct __FILTER_STRUCT__     {
                              BYTE prop[4];
                              short previousSample;
                              short inSample;
                              unsigned int counter;

                              BYTE filterOut[4];
                              BYTE __in;
                              BYTE __out;
                          } filterData[4];

short __audioBuffer__[2][524288];
