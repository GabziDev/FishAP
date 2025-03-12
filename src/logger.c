//
// Created by gabzdev on 12.03.2025.
//

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "include/logger.h"

#define LOG_FILE_DIR "log"
#define LOG_FILE_PREFIX "log_"

static char logFileName[100] = {0}; // Nom du fichier de log

/*
 * Creer un dossier (LOG_FILE_DIR) si il n'existe pas et
 * un fichier avec la date actuelle
 */
void loggerInit() {
  time_t t = time(NULL);
  char timestamp[64];
  FILE* pLogFile;

// Creer un dossier si n'existe pas
#ifdef _WIN32
  if (CreateDirectory(LOG_FILE_DIR, NULL) == 0) {
    if (GetLastError() != ERROR_ALREADY_EXISTS) {
      printf("Error creating log directory\n");
      return;
    }
  }
#else
  struct stat st = {0};
  if (stat(LOG_FILE_DIR, &st) == -1) {
    if (mkdir(LOG_FILE_DIR, 0700) != 0) {
      printf("Error creating log directory\n");
      return;
    }
  }
#endif

  // format date
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", localtime(&t));

  // format path + file name
  snprintf(logFileName, sizeof(logFileName), "%s/%s%s.txt", LOG_FILE_DIR, LOG_FILE_PREFIX, timestamp);

  pLogFile = fopen(logFileName, "a+");

  if (pLogFile == NULL) {
    printf("Error opening log file\n");
    return;
  }

  if (fputs("<===> log file FishAP ", pLogFile) == EOF || fputs(timestamp, pLogFile) == EOF || fputs(" <===>\n", pLogFile) == EOF) {
    printf("Error writing to log file\n");
  }

  fclose(pLogFile);
}

/*
 * Ecrire dans le fichier stocker dans (logFileName) les messages avec le type de LogLevel
 *
 * Param:
 * - level : pointeur du message
 *
 * Return:
 *  Rien
 */
void logMessage(LogLevel level, const char* cpMessage) {
  time_t t = time(NULL);
  char *levelStr;
  char formattedMessage[1024] = {0};
  FILE* pLogFile;

  // format message
  strftime(formattedMessage, sizeof(formattedMessage), "[%Y-%m-%d %H:%M:%S]", localtime(&t));

  // Type log to string
  switch (level) {
    case LOG_INFO:
      levelStr = "INFO";
    break;
    case LOG_DEBUG:
      levelStr = "DEBUG";
    break;
    case LOG_WARNING:
      levelStr = "WARNING";
    break;
    case LOG_ERROR:
      levelStr = "ERROR";
    break;
    default:
      levelStr = "UNKNOWN";
  }

  // Format message with loglevel
  snprintf(formattedMessage + strlen(formattedMessage), sizeof(formattedMessage) - strlen(formattedMessage), " [%s] %s\n", levelStr, cpMessage);

  pLogFile = fopen(logFileName, "a+");

  if (pLogFile == NULL) {
    return;
  }

  fputs(formattedMessage, pLogFile);
  fclose(pLogFile);
}