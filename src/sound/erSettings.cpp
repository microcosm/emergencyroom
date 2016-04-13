#include "erSettings.h"

void erSettings::load(){
    if(getComputerName() == "andy-tw-1"){
        cout << "YES";
    }else{
        cout << "NO - '" << getComputerName() << "'";
    }
}

string erSettings::getComputerName(){
    fp = popen("scutil --get ComputerName", "r");
    if(fp != NULL){
        fgets(path, 20, fp);
        string str(path);
        str.erase(remove(str.begin(), str.end(), '\n'), str.end());
        pclose(fp);
        return str;
    }
    return "";
}