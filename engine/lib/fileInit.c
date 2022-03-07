channel[0].instrument = 0x0;
channel[0].master = 0x7F;
channel[0].filter[0] = 0x0;

filterData[0].__in = 0xFF;
filterData[0].__out = 0xFF;

instrument[0].type = 0x0;
instrument[0].morph = 0x10;

instrument[0].volumeEnvelope[0] = 0;
instrument[0].volumeEnvelope[1] = 30000;
instrument[0].volumeEnvelope[2] = 300000;
instrument[0].volumeEnvelope[3] = 140000;
instrument[0].volumeEnvelope[4] = 1200000;

instrument[0].targetEG[0] = 0x5FFF;
instrument[0].targetEG[1] = 0x1FFF;
