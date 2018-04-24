#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

// prototypes
int validate(int, char*[]);

// implementation
int main(int argc, char* argv[]) {
  if (validate(argc, argv)) {
    printf("Invalid command line arguments\n");
    return 1;
  }

  const char* infile = argv[1];
  const char* outfile = argv[2];
  const double scale = atof(argv[3]);

  WAVFile* wav = wav_fread(infile);
  if (!wav) {
    printf("Cannot read '%s' file\n", infile);
    return 2;
  }

  int status;
  if ((status = wav_resize(wav, scale))) {
    printf("Got an error while scaling data: %d\n", status);
    return 3;
  }

  if ((status = wav_fwrite(wav, outfile))) {
    printf("Got an error while writing output: %d\n", status);
    return 4;
  }

  wav_free(wav);
  return 0;
}

int validate(int argc, char* argv[]) {
  if (argc != 4) return 1;
  if (atof(argv[3]) == 0.0) return 2;
  return 0;
}
