
/**
 * @copyright  Copyright (C) 2025 Kaden Slater. All Rights Reserved.
 * @license    GNU General Public License version 2 or later; see LICENSE.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#ifndef INCLUDE_FILE_UTILS_H_
#define INCLUDE_FILE_UTILS_H_


// Make this C library callable from C++
#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus


void read_file(const char *, char **, unsigned int *);


#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // INCLUDE_FILE_UTILS_H_
