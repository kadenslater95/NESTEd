
/**
 * @copyright  Copyright (C) 2026 Kaden Slater. All Rights Reserved.
 * @license    GNU General Public License version 2 or later; see LICENSE.txt
 */

#include "file_utils.h"

/**
 * Read the file from the given file path and malloc an array for its content which the user of the function
 * will then be responsible for freeing
 * 
 * @param filePath char*, the path to the file to read
 * @param content char*, a pointer placeholder that will be pointed to a malloced array of file content
 * @param length int*, a pointer placehold that will be set to have the size of the file in bytes
*/
void read_file(const char *filePath, char **content, unsigned int *length) {
  FILE *filePointer = fopen(filePath, "r");
  if (!filePointer) {
    perror("Failed to open file for reading! Error: ");

    *content = NULL;
    *length = 0;

    return;
  }

  if (fseek(filePointer, 0, SEEK_END) != 0) {
    perror("Failed to seek file for reading! Error: ");

    *content = NULL;
    *length = 0;

    fclose(filePointer);

    return;
  }

  long fileLength = ftell(filePointer);
  if (fileLength < 0) {
    perror("Failed to get file length! Error: ");

    *content = NULL;
    *length = 0;

    fclose(filePointer);

    return;
  }

  if (fileLength == 0) {
    printf("Filepath provided to read_file was empty!");

    *content = NULL;
    *length = 0;

    fclose(filePointer);

    return;
  }

  rewind(filePointer);

  *length = (unsigned int) fileLength;
  *content = (char*) malloc(sizeof(char) * (*length + 1));
  if (!*content) {
    perror("Failed to allocate file content! Error: ");

    *length = 0;

    fclose(filePointer);

    return;
  }

  size_t readLength = fread(*content, 1, *length, filePointer);
  if (readLength != *length) {
    perror("Failed to read file content! Error: ");

    free(*content);
    *content = NULL;
    *length = 0;

    fclose(filePointer);

    return;
  }

  content[0][*length] = '\0';

  fclose(filePointer);
}
