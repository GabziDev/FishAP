//
// Created by gabzdev on 12.03.2025.
//

#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
  LOG_INFO = 0,
  LOG_DEBUG = 1,
  LOG_WARNING = 2,
  LOG_ERROR = 3,
} LogLevel;

void loggerInit();
void logMessage(LogLevel level, const char *message);

#endif //LOGGER_H
