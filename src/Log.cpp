
#include "Log.hh"

void Log::openLogFile(const char *filePath)
{
	closeLogFile();
	if (filePath != nullptr)
		_logFile.open(filePath, std::fstream::out | std::fstream::app);
}

void Log::closeLogFile()
{
	if (_logFile.is_open())
		_logFile.close();
}

/**
 * Static function, can be called everywhere.
 * Log something in a log file (LOGFILE_PATH macro by default)
 * Please call openLogFile() before calling this method
 * For performance purpose file stay open until closeLogFile() is called
 * @param complementary_msg Additional message to display
 * @param file_path Special filepath, if empty the default file path is the macro LOGFILE_PATH
 */
 void Log::logSomething(const std::string &msg)
{
     if (!_logActive)
         return;
	if (!_logFile.is_open())
		openLogFile(LOGFILE_NAME);
	_logFile << msg.c_str() << std::endl;
	_logFile.flush();
}

 void Log::logSuccessMsg(const std::string &msg)
 {
     if (_logActive)
     {
         if (_tryStatus)
         {
             logSomething("\t\t[OK] " + msg);
             _tryStatus = false;
         }
         else
             logSomething("[OK] " + msg);
     }
 }

 void Log::logFailureMsg(const std::string &msg)
 {
     if (_logActive)
     {
         if (_tryStatus)
         {
             logSomething("\t\t[KO] " + msg);
             _tryStatus = false;
         }
         else
             logSomething("[KO] " + msg);
     }
 }

 void Log::logInfoMsg(const std::string &msg)
 {
     if (_logActive)
	    logSomething("[info] " + msg + '\n');
 }

 void Log::logTryMsg(const std::string &msg)
 {
     if (_logActive)
     {
         logSomething("\t[try] " + msg);
         _tryStatus = true;
     }
 }

 void Log::setLogActive(bool log)
 {
     _logActive = log;
 }