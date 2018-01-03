/*
 * logger.h
 *
 *  Created on: 14 nov. 2017
 *      Author: Usuario
 */

#ifndef LOGGER_H_
#define LOGGER_H_

//logger.h
void log_print(char* filename, int line, char *fmt,...);
#define LOG_PRINT(...) log_print(__FILE__, __LINE__, __VA_ARGS__ )

#endif /* LOGGER_H_ */
