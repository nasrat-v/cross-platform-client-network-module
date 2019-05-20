
#ifndef __LOGNETWORK_HH__
#define __LOGNETWORK_HH__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

# define SUCCESS			(ERR)1
# define FILE_ERROR			(ERR)~0
# define LOGFILE_NAME		"log.txt"

typedef short			ERR;
static bool				__attribute__((unused))_tryStatus = false;
static bool             __attribute__((unused))_logActive = true;
static std::fstream		_logFile;

class Log
{
public:
	static void			openLogFile(const char *filePath);
	static void			closeLogFile();
	static void			logSuccessMsg(const std::string &msg);
	static void			logFailureMsg(const std::string &msg);
	static void			logInfoMsg(const std::string &msg);
	static void			logTryMsg(const std::string &msg);
	static void			logSomething(const std::string &msg);
    static void         setLogActive(bool log);

private:
    Log() = default;
	~Log() = default;
};

#endif /* !__LOGNETWORK_HH__ */