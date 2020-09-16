#ifndef _LOGGER_H_
#define _LOGGER_H_

extern int infof(const char *format, int count, ...);

extern int warnf(const char *format, int count, ...);

extern int errorf(const char *format, int count, ...);

extern int panicf(const char *format, int count, ...);

#endif