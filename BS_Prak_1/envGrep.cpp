#include <iostream>
#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// this function work but is not completed
void readParameter(char* arg, char* envList[], bool ignoreCase){

    int len = strlen(arg);
    char *upperCase = static_cast<char *>(malloc(sizeof(char) * len));

    for (int i = 0; i < len; ++i)
    {
        upperCase[i] = toupper((unsigned char)arg[i]);
    }
/*
    char* result = std::getenv(upperCase);
    if(result != NULL){
        std::cout << "Variable= " << upperCase << std::endl;
        std::cout << "Value= " << result << std::endl;
        return;
    }
*/
    int i = 0;

    for(char* env = envList[0];env != nullptr && ignoreCase;env = envList[++i] ){
        char *substring = strstr(env, upperCase);
        if (substring != nullptr) {
            char *value = strchr(env, '=');
            std::cout << "value " << value << std::endl;
            *value = 0;//make it to the end of string
            char* variable = env;//string will terminate at the delimiter '=';
            std::cout << "variable = " << variable << std::endl;
        }
        //test the validity of variables
//        std::cout << arg;
//        std::cout << env;
    }


}

// this function works
int main(int argc, char *argv[], char* env[] )
{
    bool ignoreCase = false;
    std::string argument = argv[1];

    if(argc < 2){
        std::cout << "not enough arguments: ./envGrep [-i] [text]";
        return -1;
    }

    if (argument == "-i") ignoreCase = true;

    for (int i = (1 + ignoreCase); i < argc; i++){
        readParameter(argv[i],env, ignoreCase);
    }
    return 0;
}

