#ifndef Logger_H
#define Logger_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

class Logger
{
public:
    static void log(const std::string& message)
    {
        std::ofstream ofs("log.txt", std::ofstream::app);
        ofs << "[" << "INFO" << "] : " << message << std::endl;
        ofs.close();
    }

    static void logTimer(const double value, int precision = 13)
    {
        std::ofstream ofs("log.txt", std::ofstream::app);
        ofs << "[" << "TIME" << "] : " << std::fixed << std::setprecision(precision) << value << std::endl;
        ofs.close();
    }

    static void logFull(const std::string& message, const double value, int precision = 13)
    {
        log(message);
        logTimer(value, precision);

        std::ofstream ofs("log.txt", std::ofstream::app);
        ofs << "________________________________________" << std::endl;
        ofs.close();
    }
};
#endif // !Logger_H
