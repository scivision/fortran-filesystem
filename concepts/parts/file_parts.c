#include <stdio.h>
#include <string.h>
#include <stdlib.h>


size_t file_parts(char* path, char** parts ){

  const char* sep = "/";
  int i=0;

  char* tok = strtok(path, sep);
  while( tok != NULL ) {
    parts[i++] = tok;
    tok = strtok(NULL, sep);
  }

  return i;
}

int main(void) {

  const int MAX_PARTS = 50;
  const int MAX_PART_LEN = 100;

  char** parts = (char**) malloc(MAX_PARTS * MAX_PART_LEN*sizeof(char*));

  const char path[] = "/path/to////file.txt";

  char* buf = (char*) malloc(strlen(path) + 1);  // +1 for null terminator
  strcpy(buf, path);

  size_t N = file_parts(buf, parts);

  printf("File name: %s %lu parts found\n", path, N);

  for (size_t i=0; i<N; i++) {
    printf("Part %lu: %s\n", i, parts[i]);
  }

  free(buf);
  free(parts);

  return 0;
}
