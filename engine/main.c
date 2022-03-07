#include <stdio.h>
#include <stdlib.h>
#include "lib/filestructs.h"
#include "lib/subFunctions.h"
    #define True 1
    #define False 0
    #define __log__(n) {for(int i  = 0; i < n; i++) printf("channel %1x:\t%2X\t%2X\t%2X\t%2X\t%2X\n", i, channel[i].key, channel[i].volume, channel[i].instrument, channel[i].tunning, channel[i].waitFrames);}

FILE *dataFile;
FILE *audioOut;

int main(int argc, char *argv[]){
  if (argc == 1) {
                    fprintf(stderr, "Error 1: No hay archivo de entrada especificado.\n");
                    return 1;
                  };
  dataFile = fopen(argv[1], "rb");
  if (dataFile == NULL) {
                    fprintf(stderr, "Error 2: El archivo ingresado no existe.\n");
                    return 2;
                  };
  audioOut = fopen("out.wav", "wb+");
  fileMaster.fileInit = True;
  fileMaster.dataCursor = 0;

  #include "lib/fileInit.c"
  int a = 0;

  while (fileMaster.fileInit){
    updateChannel();
    audiorender();
    if (a == 0) instrument[0].morph += 0x1;
    else if (a == 1) instrument[0].morph -= 0x1;

    if (instrument[0].morph == 0xF0) a = 1;
    else if (instrument[0].morph == 0x10) a = 0;
    // instrument[0].morph++;
    // __log__(1)
  };

  fclose(audioOut);
  fclose(dataFile);
  return 0;
}
