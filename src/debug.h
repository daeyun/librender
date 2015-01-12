/**
 * @file debug.h
 * @author Daeyun Shin <daeyun@dshin.org>
 * @version 0.1
 * @date 2015-01-02
 * @copyright Scry is free software released under the BSD 2-Clause license.
 */
#pragma once
#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef DEBUG
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <unistd.h>

#define __COLOR_RESET "\033[0m"
#define _COLOR_NORMAL(str) "\x1B[0m" str __COLOR_RESET
#define _COLOR_RED(str) "\x1B[31m" str __COLOR_RESET
#define _COLOR_GREEN(str) "\x1B[32m" str __COLOR_RESET
#define _COLOR_YELLOW(str) "\x1B[33m" str __COLOR_RESET
#define _COLOR_BLUE(str) "\x1B[34m" str __COLOR_RESET
#define _COLOR_MAGENTA(str) "\x1B[35m" str __COLOR_RESET
#define _COLOR_CYAN(str) "\x1B[36m" str __COLOR_RESET
#define _COLOR_WHITE(str) "\x1B[37m" str __COLOR_RESET

#define _UNUSED(x) (void)(sizeof((x), 0))

#define _var(name)                                                          \
  {                                                                         \
    std::ostringstream val_str;                                             \
    val_str << name;                                                        \
    DisplayVariable(#name, val_str.str(), sizeof(name), __FILE__, __LINE__, \
                    __func__);                                              \
  }

#define _assert(expr)                                                       \
  if (!(expr)) {                                                            \
    fprintf(stderr, _COLOR_RED("[ERROR] (%s:%d %s) assertion %s failed\n"), \
            __FILE__, __LINE__, __func__, #expr);                           \
    __check_errno() _b                                                      \
  }

#define _b \
  { asm("int $3"); }

#define _print_env                       \
  for (int i = 0; *(environ + i); i++) { \
    printf("%s\n", *(environ + i));      \
  }

#define __check_errno()                                                  \
  if (errno != 0) {                                                      \
    fprintf(stderr, _COLOR_RED("[ERROR] errno: %s\n"), strerror(errno)); \
  }

static inline void DisplayVariable(std::string name, std::string value,
                                   size_t size, std::string file, int line,
                                   std::string func) {
  _UNUSED(DisplayVariable);
  fprintf(stderr, "[INFO]  (%s:%d %s) " _COLOR_MAGENTA("%s=%s") "   %d\n",
          file.c_str(), line, func.c_str(), name.c_str(), value.c_str(),
          (int)size);
}

extern char** environ;

#else  // End of #if defined(DEBUG)

#define _var(name)
#define _assert(expr)
#define __check_errno()
#define _b
#define _print_env
#endif  // End of #if !defined(DEBUG)

#endif  // DEBUG_H_
