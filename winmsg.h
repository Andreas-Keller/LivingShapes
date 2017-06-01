/*  static class that takes in and stores messages, which later can be retrieved
    for printing */

#ifndef WINMSG_H
#define WINMSG_H

#include <vector>
#include <string>

class WinMsg {
public:
    static void add(const std::string& msg) { _msgPool.push_back(msg); }

    //get method deletes all stored messaged:
    static std::vector<std::string> get()     {
        auto ret = _msgPool;
        _msgPool.clear();
        return ret;
    }

private:
    static std::vector<std::string> _msgPool;
};

#endif // WINMSG_H
