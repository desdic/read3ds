#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <map>
#include <vector>
//#include <pair>

class Config {
public:
    Config(const std::string &configfile);

    const bool Value(const std::string &section, const std::string &entry, const bool defvalue) const;
    const std::string &Value(const std::string &section, const std::string &entry, const std::string &defvalue) const;
    void Value(const std::string &section, const std::string &entry, std::vector<std::string> &l) const;

    void Section(const std::string &section, std::vector<std::string> &l) const;

    std::vector< std::pair< std::string, std::string > > content_;

    //std::map<std::string, std::string> content_;
};


#endif
