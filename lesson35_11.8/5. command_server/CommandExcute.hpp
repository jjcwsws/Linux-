#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <set>
#include "Log.hpp"

const std::string sep = " ";

class Command
{
private:
    void LoadConf(const std::string &conf)
    {
        std::ifstream in(conf);
        if (!in.is_open())
        {
            LOG(FATAL, "open %s error\n", conf.c_str());
            return;
        }
        std::string line;
        while (std::getline(in, line))
        {
            LOG(DEBUG, "load command [%s] success\n", line.c_str());
            _safe_cmd.insert(line);
        }
        in.close();
    }

public:
    Command(const std::string &cond_path) : _cond_path(cond_path)
    {
        LoadConf(_cond_path);
    }
    std::string PrefixCommand(const std::string &cmd)
    {
        if(cmd.empty()) return std::string();
        auto pos = cmd.find(sep);
        if(pos == std::string::npos) return cmd;
        else return cmd.substr(0, pos);
    }
    bool SafeCheck(const std::string &cmd)
    {
        std::string prefix = PrefixCommand(cmd); // ls -a -l , touch a.txt
        auto iter = _safe_cmd.find(prefix);
        if (iter == _safe_cmd.end())
            return false;
        return true;
    }
    std::string Excute(const std::string &cmd) // ls -a -l
    {
        // 1. fork && pipe
        // 2. exec
        // 2.1 dup
        std::string result;
        if (SafeCheck(cmd))
        {
            FILE *fp = popen(cmd.c_str(), "r");
            if (fp == nullptr)
            {
                return "failed";
            }
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), fp) != NULL)
            {
                result += buffer;
            }
            pclose(fp);
        }
        else
        {
            result = "坏人\n";
        }
        return result;
    }
    ~Command()
    {
    }

private:
    std::set<std::string> _safe_cmd;
    std::string _cond_path;
};