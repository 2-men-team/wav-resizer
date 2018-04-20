#ifndef _WAV_H_
#define _WAV_H_

#include <stdint.h>

typedef int16_t sample_t;

typedef struct {
  int32_t chunkId;
  int32_t chunkSize;
  int32_t format;
} RIFFHEADER;

typedef struct {
  int32_t subchunk1Id;
  int32_t subchunk1Size;
  int16_t audioFormat;
  int16_t numChannels;
  int32_t sampleRate;
  int32_t byteRate;
  int16_t blockAlign;
  int16_t bitsPerSample;
} SUBCHUNK1;

typedef struct {
  int32_t   subchunk2Id;
  int32_t   subchunk2Size;
  sample_t* data;
} SUBCHUNK2;

typedef struct {
  RIFFHEADER riff;
  SUBCHUNK1 chunk1;
  SUBCHUNK2 chunk2;
} WAVFile;

WAVFile* wav_fread(const char*);
int wav_fwrite(const WAVFile*, const char*);
int wav_resize(WAVFile*, double);
void wav_free(WAVFile*);

#endif
