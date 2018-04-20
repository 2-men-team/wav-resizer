#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

#define POLINOMIAL_ORDER 3

#define LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))

WAVFile* wav_fread(const char* file) {
  if (!file) return NULL;

  FILE* wav = fopen(file, "rb");
  if (!wav) return NULL;

  WAVFile* result = (WAVFile*)malloc(sizeof(WAVFile));
  if (!result) goto error;

  if (fread(&result->riff, sizeof(RIFFHEADER), 1, wav) != 1) goto error;
  if (fread(&result->chunk1, sizeof(SUBCHUNK1), 1, wav) != 1) goto error;
  if (fread(&result->chunk2.subchunk2Id, sizeof(int32_t), 1, wav) != 1) goto error;
  if (fread(&result->chunk2.subchunk2Size, sizeof(int32_t), 1, wav) != 1) goto error;

  result->chunk2.data = (sample_t*)malloc(result->chunk2.subchunk2Size);
  if (!result->chunk2.data) goto error;
  size_t dataLength = result->chunk2.subchunk2Size / result->chunk1.blockAlign;

  if (fread(result->chunk2.data, sizeof(sample_t), dataLength, wav) != dataLength) {
    free(result->chunk2.data);
    goto error;
  }

  fclose(wav);
  return result;

error:
  free(result);
  fclose(wav);
  return NULL;
}

int wav_fwrite(const WAVFile* wav, const char* file) {
  if (!wav) return 1;

  FILE* out = fopen(file, "wb");
  if (!out) return 2;

  if (fwrite(&wav->riff, sizeof(RIFFHEADER), 1, out) != 1) goto error;
  if (fwrite(&wav->chunk1, sizeof(SUBCHUNK1), 1, out) != 1) goto error;
  if (fwrite(&wav->chunk2.subchunk2Id, sizeof(int32_t), 1, out) != 1) goto error;
  if (fwrite(&wav->chunk2.subchunk2Size, sizeof(int32_t), 1, out) != 1) goto error;

  size_t dataLength = wav->chunk2.subchunk2Size / wav->chunk1.blockAlign;
  if (fwrite(wav->chunk2.data, sizeof(sample_t), dataLength, out) != dataLength)
    goto error;

  fclose(out);
  return 0;

error:
  fclose(out);
  return 3;
}

static sample_t wav_polinom(size_t order, const sample_t* data, double k, size_t x) {
  float out = 0;

  // Lagrange polinom
  for (size_t i = 0; i <= order; i++) {
    float temp = 1;

    for (size_t j = 0; j <= order; j++) {
      if (i == j) continue;
      temp *= (x / k - j) / (i - j);
    }

    out += temp * data[i]; // BUG
  }

  return out;
}

static int wav_interpolate(sample_t** input, size_t* insize, double k, size_t order) {
  if (!input || !insize || k <= 0.0 || order < 1 || order >= *insize) return 3;

  size_t outsize = *insize * k;
  sample_t* output = (sample_t*)malloc(outsize * sizeof(sample_t));
  if (!output) return 4;

  // FIXME
  for (size_t i = 0, j = order + 1; i < outsize; i++) {
    sample_t* data = *input;

    if (j == 0) data += i - order;
    else j--;

    output[i] = wav_polinom(order, data, k, j == 0 ? order : i);
  }

  free(*input);
  *input = output;
  *insize = outsize;
  return 0;
}

int wav_resize(WAVFile* wav, double increaseBy) {
  if (!wav || increaseBy <= 0.0) return 1;

  size_t size = wav->chunk2.subchunk2Size / wav->chunk1.blockAlign;

  int status;
  if ((status = wav_interpolate(&wav->chunk2.data, &size, increaseBy, POLINOMIAL_ORDER)))
    return status;

  wav->chunk2.subchunk2Size = size * sizeof(sample_t);
  wav->riff.chunkSize = 36 + wav->chunk2.subchunk2Size;

  return 0;
}

void wav_free(WAVFile* wav) {
  if (!wav) return;
  free(wav->chunk2.data);
  free(wav);
}
