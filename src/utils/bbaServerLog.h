/*
 * bbadebug.h
 *
 *  Created on: Jan 16, 2022
 *      Author: aress
 */

#ifndef BBASERVERLOG_H_
#define BBASERVERLOG_H_

class bbaServerLog {
private:
	void send_log(const char *format, ...);

public:
	static void dbg_log(const char *format, ...);
	static void info_log(const char *format, ...);
	static void error_log(const char *format, ...);
	static void warning_log(const char *format, ...);
};

#endif /* BBASERVERLOG_H_ */
