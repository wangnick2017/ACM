#pragma once

namespace CppBackend
{
    using namespace std;
    
    class Backend
    {
    public:
        static char *errorMessage;
        
        static int Register(char *msg);
        static int Login(char *msg);
        static char *QueryProfile(char *msg);
        static int ModifyProfile(char *msg);
        static int ModifyPrivilege(char *msg);

        static char *QueryTicket(char *msg);
        static char *QueryTransfer(char *msg);
        static int BuyTicket(char *msg);
        static char *QueryOrder(char *msg);
        static int RefundTicket(char *msg);

        static int AddTrain(char *msg);
        static int SaleTrain(char *msg);
        static char *QueryTrain(char *msg);
        static int DeleteTrain(char *msg);
        static int ModifyTrain(char *msg);
    };
}