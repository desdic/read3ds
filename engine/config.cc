#include <iostream>
#include <fstream>
#include <sstream>
#include "exception.h"
#include "config.h"

using namespace std;

string eatwhitespaces(const string &str, const string &delimiters = " \t\n\r") {

    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos     = str.find_first_of(delimiters, lastPos);
    string tmp="";

    while (string::npos != pos || string::npos != lastPos)
    {
        tmp+=str.substr(lastPos, pos - lastPos);

        lastPos = str.find_first_not_of(delimiters, pos);
        pos     = str.find_first_of(delimiters, lastPos);
    }
    return tmp;
}

Config::Config(const string &configfile) {

    ifstream file(configfile.c_str());
    string line;
    int linenr = 1;
    string section, name, value;
    string::size_type pos;
    ostringstream tmp;

    if (!file.is_open()) {
        tmp << "Unable to open " << configfile;
        throw EngineException( tmp.str() );
    }

    while (getline(file, line)) {

        string tmpline = eatwhitespaces( line );

        // Skip empty lines
        if (!tmpline.length()) continue;
        if (tmpline[0]=='#') continue;

        if (tmpline[0]=='[') {
            pos = tmpline.find(']');
            if (pos == string::npos) {
                tmp << "[" << configfile << "] line " << linenr << " is missing ]";
                throw EngineException( tmp.str() );
            }

            section=tmpline.substr(1,pos-1);
            continue;
        }

        pos=tmpline.find('=');

        if (pos == string::npos) {
            tmp << "[" << configfile << "] Line " << linenr << " is missing = ";
            throw EngineException( tmp.str() );
        }

        name  = tmpline.substr(0,pos);
        value = tmpline.substr(pos+1);

        pair< string, string> tmp;
        tmp.first = section+'/'+name;
        tmp.second = value;

        content_.push_back( tmp );
        linenr++;
    }
}

const bool Config::Value(const std::string &section, const std::string &entry, const bool defval=false) const {

    for (std::vector< pair < string, string > >::const_iterator ci = content_.begin(); ci!=content_.end(); ++ci) {
        if (ci->first == section+'/'+entry) {
            if (ci->second == "true") 
                return true;
            if (ci->second == "false")
                return false;

            return defval;
        }
    }
    return defval;
}

const string &Config::Value(const std::string &section, const std::string &entry, const std::string &defvalue) const {
    for (std::vector< pair < string, string > >::const_iterator ci = content_.begin(); ci!=content_.end(); ++ci) {
        if (ci->first == section+'/'+entry) {
            return ci->second;
        }
    }
    return defvalue;
}

void Config::Value(const std::string &section, const std::string &entry, vector<string> &l) const {

    for (std::vector< pair < string, string > >::const_iterator ci = content_.begin(); ci!=content_.end(); ++ci) {
        if (ci->first == section+'/'+entry) {
            l.push_back( ci->second);
        }
    }
}

void Config::Section(const std::string &section, std::vector<std::string> &l) const {
    for (std::vector< pair < string, string > >::const_iterator ci = content_.begin(); ci!=content_.end(); ++ci) {

        if (ci->first.substr(0, section.length())+'/' == section+'/') {
            //cout << ci->first+'='+ci->second << endl;
            l.push_back( ci->first+'='+ci->second );
        }

    }
}

