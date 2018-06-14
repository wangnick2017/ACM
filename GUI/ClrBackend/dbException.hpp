#ifndef _DBEXCEPTION_HPP_
#define _DBEXCEPTION_HPP_
#include <iostream>
#include <cstring>

const size_t MAX_MSG_LEN = 200;
const int FILE_NOT_MATCH = 100;
const int FILE_NOT_EXIST = 101;
class dbException{
protected:
    char msg[MAX_MSG_LEN];
    int errCode = 0;
public:
    dbException(){}
    virtual ~dbException(){}
    virtual char *what() const
    {
        return NULL;
    }
    virtual int code() const
    {
        return 0;
    }
};

class fileNotMatch : public dbException{
public:
    fileNotMatch(){
        strcpy_s(msg, "Error : FILE NOT MATCH!");
        errCode = FILE_NOT_MATCH;
    }

    char *what(){
        return msg;
    }

    int code(){
        return errCode;
    }
};

class ImportFileNotExist : public dbException{
public:
    ImportFileNotExist(){
        strcpy_s(msg, "Error : Import file not exist! If you want to create new file, please specify the dataLen!");
        errCode = FILE_NOT_EXIST;
    }

    char *what(){
        return msg;
    }

    int code(){
        return errCode;
    }
};
#endif
