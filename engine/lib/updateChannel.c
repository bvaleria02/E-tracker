#include "filestructs.h"
#include "subFunctions.h"
#include <stdio.h>
typedef unsigned char BYTE;

extern const float note[];
extern struct __CHANNEL_STRUCT__ channel[16];
extern struct __INSTRUMENT_STRUCT__ instrument[128];
extern struct __MASTER_FILE_STRUCT__ fileMaster;
extern FILE *dataFile;
extern FILE *audioOut;

BYTE __endFlag = 1;
BYTE __loop_flag__ = 1;

void specialOpcodes(BYTE opcode);
void filterOpcodes(BYTE opcode);

void updateChannel(){
  BYTE opcode = 0;
  BYTE operator = 0;
  BYTE special = 0;
  short readyFlag = 0x0;

  __endFlag = 1;
  __loop_flag__ = 1;

  for(int i = 0; i < 1; i++){
      if (channel[i].waitFrames > 0) {
                                        channel[i].waitFrames--;
            } else if (channel[i].waitFrames == 0){
                                        readyFlag = (readyFlag | (1 << i)) | !channel[i].active;
            };
  };

  if (readyFlag == 0) return;

  fseek(dataFile, fileMaster.dataCursor, SEEK_SET);
  do{
    fread(&opcode, sizeof(BYTE), 1, dataFile);
    switch(opcode >> 4){
          case  0x0   : fread(&operator, sizeof(BYTE), 1, dataFile);
                        channel[opcode & 0x0F].key        = operator & 0x7F;
                        if (!(operator >> 7)) {
                                          channel[opcode & 0x0F].seed = 0;
                                          channel[opcode & 0x0F].counter = 0;
                                        };
                        fileMaster.dataCursor += 2;
                        break;

          case  0x1   : fread(&operator, sizeof(BYTE), 1, dataFile);
                        channel[opcode & 0x0F].volume     = operator & 0x7F;
                        channel[opcode & 0x0F].active     = 1 - (operator >> 7);
                        fileMaster.dataCursor += 2;
                        break;

          case  0x2   : fread(&operator, sizeof(BYTE), 1, dataFile);
                        channel[opcode & 0x0F].instrument = operator;
                        fileMaster.dataCursor += 2;
                        break;

          case  0x3   : fread(&operator, sizeof(BYTE), 1, dataFile);
                        channel[opcode & 0x0F].tunning    = operator;
                        fileMaster.dataCursor += 2;
                        break;

          case  0x4   : fread(&operator, sizeof(BYTE), 1, dataFile);
                        channel[opcode & 0x0F].waitFrames = operator;
                        fileMaster.dataCursor += 2;
                        break;

          case  0x5   : fread(&operator, sizeof(BYTE), 1, dataFile);
                        channel[opcode & 0x0F].transpose = operator;
                        fileMaster.dataCursor += 2;
                        break;

          case  0x6   : fread(&operator, sizeof(BYTE), 1, dataFile);
                        channel[opcode & 0x0F].octave = operator;
                        fileMaster.dataCursor += 2;
                        break;

          case  0x7   : filterOpcodes(opcode);
                        break;

          case  0x8   : // Not used //
                        break;

          case  0x9   : // Not used //
                        break;

          case  0xA   : // Not used //
                        break;

          case  0xB   : // Not used //
                        break;

          case  0xC   : // Not used //
                        break;

          case  0xD   : // Not used //
                        break;

          case  0xE   : // Not used //
                        break;

          case  0xF   : specialOpcodes(opcode);
                        break;

    };

  } while (__endFlag == 1);

  printf("--------------------------------------------\n");

  return;
}

void specialOpcodes(BYTE opcode){
  opcode = opcode & 0xF;


  switch(opcode){
        case  0x0   :
                        break;

        case  0x1   :
                        break;

        case  0x2   :
                        break;

        case  0x3   :
                        break;

        case  0x4   :
                        break;

        case  0x5   :
                        break;

        case  0x6   :
                        break;

        case  0x7   :
                        break;

        case  0x8   :
                        break;

        case  0x9   :
                        break;

        case  0xA   :
                        break;

        case  0xB   :
                        break;

        case  0xC   :
                        break;

        case  0xD   :   for(int i = 0; i < 16; i++) channel[i].active = 0;
                        fileMaster.dataCursor += 1;
                        break;

        case  0xE   :   __endFlag = 0;
                        fileMaster.dataCursor += 1;
                        printf("End of frame\n");
                        break;

        case  0xF   :   __endFlag = 0;
                        fileMaster.fileInit = 0;
                        fileMaster.dataCursor += 1;
                        printf("End of file\n");
                        break;

  };
  return;
}

void filterOpcodes(BYTE opcode){
  fileMaster.dataCursor += 1;
  int i = opcode & 0xF;
  while (__loop_flag__){
          fread(&opcode, sizeof(BYTE), 1, dataFile);
          switch(opcode){
              case  0x0   :   fread(&opcode, sizeof(BYTE), 1, dataFile);
                              channel[i].master = opcode;
                              break;
              case  0x1   :   fread(&opcode, sizeof(BYTE), 1, dataFile);
                              channel[i].filter[0] = opcode;
                              break;
              case  0x2   :   fread(&opcode, sizeof(BYTE), 1, dataFile);
                              channel[i].filter[1] = opcode;
                              break;
              case  0x3   :   fread(&opcode, sizeof(BYTE), 1, dataFile);
                              channel[i].filter[2] = opcode;
                              break;
              case  0x4   :   fread(&opcode, sizeof(BYTE), 1, dataFile);
                              channel[i].filter[3] = opcode;
                              break;
              case  0xFF  :   __loop_flag__ = 0;
                              break;
          };
  };
  return;
}
