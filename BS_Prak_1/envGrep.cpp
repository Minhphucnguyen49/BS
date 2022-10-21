#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// this function work but is not completed
void readParameter(char* arg, char* envList[], bool ignoreCase, bool nameOnly){
    int len = strlen(arg);
    char *upperCase = static_cast<char *>(malloc(sizeof(char) * len));
    if(ignoreCase) {
        for (int i = 0; i < len; ++i)
        {
            upperCase[i] = toupper((unsigned char)arg[i]);
        }
    }

    int i = 0;
    //hier
    for(char* env = envList[0];env != nullptr && ignoreCase;env = envList[++i] ){
        char *substring = strstr(env, upperCase);
            if (substring != nullptr) {
                if (!nameOnly){
                    std::cout << env<<std::endl;
                } else {
                    char *value = strchr(env, '=');
                    //std::cout << "value " << value << std::endl;
                    *value = 0;//make it to the end of string
                    char *variable = env;//string will terminate at the delimiter '=';
                    std::cout << "variable = " << variable << std::endl;
                }
            }
    }

}

// this function works
int main(int argc, char *argv[], char* env[] )
{
    bool ignoreCase = false;
    bool nameOnly = false;

    char* argument = argv[1];

    if(argc < 2){
        std::cout << "not enough arguments: ./envGrep [-i] [text]";
        return -1;
    }
    //if (argument == "-i") ignoreCase = true;
    if (!strcmp(argument,"-i")) ignoreCase = true;
    argument = argv[1+ignoreCase];
    //if (argument == "-n") nameOnly = true;
    if (!strcmp(argument,"-n")) nameOnly = true;

    for (int i = (1 + ignoreCase + nameOnly); i < argc; i++){
        readParameter(argv[i],env, ignoreCase,nameOnly);
    }
    return 0;
}

