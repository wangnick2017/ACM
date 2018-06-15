#include "ClrBackend.h"

namespace Clr
{
    int ReadInt(char *&s)
    {
        while ((*s) < '0' || (*s) > '9')
            ++s;
        int ret = 0;
        for (; (*s) >= '0' && (*s) <= '9'; ++s)
            ret = ret * 10 + ((*s) - '0');
        return ret;
    }

    double ReadDouble(char *&s)
    {
        while ((*s) < '0' || (*s) > '9')
            ++s;
        double ret = 0.0, p = 0.1;
        bool point = false;
        for (; (*s) >= '0' && (*s) <= '9' || (*s) == '.'; ++s)
        {
            if (*s == '.')
                point = true;
            else if (point)
            {
                ret += p * (*s - '0');
                p /= 10.0;
            }
            else
            {
                ret = ret * 10.0 + (double)(*s - '0');
            }
        }
        return ret;
    }

    String ^ReadString(char *&s)
    {
        char str[100];
        int l = 0;
        for (; *s == ' ' || *s == '\n' || *s == '\t' || *s == '\0'; ++s);
        for (; *s != ' ' && *s != '\n' && *s != '\t' && *s != '\0'; ++s)
            str[l++] = *s;
        str[l] = '\0';
        return gcnew String(str);
    }

    Ticket ^ReadTicket(char *&s)
    {
        Ticket ^ticket = gcnew Ticket();
        ticket->trainID = ReadString(s);
        ticket->locFrom = ReadString(s);
        ticket->dateFrom = ReadString(s);
        ticket->timeFrom = ReadString(s);
        ticket->locTo = ReadString(s);
        ticket->dateTo = ReadString(s);
        ticket->timeTo = ReadString(s);
        ticket->cnt = 0;
        ticket->ticketKind = gcnew array<String ^>(5);
        ticket->num = gcnew array<Int32>(5);
        ticket->price = gcnew array<Double>(5);
        while (*s != '\n' && *s != '\0')
        {
            ticket->ticketKind[ticket->cnt] = ReadString(s);
            ticket->num[ticket->cnt] = ReadInt(s);
            ticket->price[ticket->cnt] = ReadDouble(s);
            ++ticket->cnt;
            while (*s == ' ' || *s == '\t')
                ++s;
        }
        return ticket;
    }

    Station ^ReadStation(char *&s, int numPrice)
    {
        Station ^station = gcnew Station();
        station->numPrice = numPrice;
        station->name = ReadString(s);
        station->timeArrive = ReadString(s);
        station->timeStart = ReadString(s);
        station->timeStopover = ReadString(s);
        station->price = gcnew array<Double>(numPrice);
        for (int i = 0; i < numPrice; ++i)
        {
            station->price[i] = ReadDouble(s);
        }
        return station;
    }

    char *GetChars(String ^s)
    {
        char *ret1 = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(s);
        char *ret2 = _strdup(ret1);
        System::Runtime::InteropServices::Marshal::FreeHGlobal((System::IntPtr)ret1);
        return ret2;
    }

    User::User()
    {
    }

    User::User(Int32 id, char *strUser)
    {
        char *s = strUser;
        ID = id;
        name = ReadString(s);
        email = ReadString(s);
        phone = ReadString(s);
        privilege = ReadInt(s);
        delete[] strUser;
    }

    Ticket::Ticket()
    {
    }

    String ^Ticket::ToString()
    {
        return gcnew String("");
    }

    Station::Station()
    {
    }

    Train::Train()
    {
    }

    Train::Train(char *strTrain)
    {
        char *s = strTrain;
        trainID = ReadString(s);
        if (trainID == "0")
        {
            delete[] strTrain;
            return;
        }
        name = ReadString(s);
        catalog = ReadString(s);
        numStation = ReadInt(s);
        numPrice = ReadInt(s);
        namePrice = gcnew array<String ^>(numPrice);
        for (int i = 0; i < numPrice; ++i)
        {
            namePrice[i] = ReadString(s);
        }
        stations = gcnew List<Station ^>(numStation);
        for (int i = 0; i < numStation; ++i)
        {
            stations->Add(ReadStation(s, numPrice));
        }
        delete[] strTrain;
    }

    char *Train::ToStr()
    {
        String ^s = trainID + " " + name + " " + catalog + " " + numStation + " " + numPrice;
        for (int i = 0; i < numPrice; ++i)
            s += " " + namePrice[i];
        for (int i = 0; i < numStation; ++i)
        {
            Station ^station = stations->default[i];
            s += "\n" + station->name + " " + station->timeArrive + " " + station->timeStart + " " + station->timeStopover;
            for (int j = 0; j < numPrice; ++j)
                s += " " + station->price[j];
        }
        s += "\n";
        return GetChars(s);
    }




    ClrBackend::ClrBackend()
    {
        Backend::init();
    }

    void ClrBackend::Register(User ^user)
    {
        user->ID = Backend::regist(GetChars(user->name + " " + user->password + " " + user->email + " " + user->phone));
    }

    Int32 ClrBackend::Login(Int32 ID, String ^password)
    {
        return Backend::login(GetChars(ID.ToString() + " " + password));
    }

    User ^ClrBackend::QueryProfile(Int32 ID)
    {
        return gcnew User(ID, Backend::query_profile(ID));
    }

    Int32 ClrBackend::ModifyProfile(User ^user)
    {
        return Backend::modify_profile(GetChars(user->ID.ToString() + " " + user->name + " " + user->password + " " + user->email + " " + user->phone));
    }

    Int32 ClrBackend::ModifyPrivilege(Int32 ID1, Int32 ID2, Int32 privilege)
    {
        return Backend::modify_privilege(ID1, ID2, privilege);
    }

    List<Ticket ^> ^ClrBackend::QueryTicket(String ^loc1, String ^loc2, String ^date, String ^catalog, Boolean transfer)
    {
        char *ostr = transfer ? Backend::query_transfer(GetChars(loc1 + " " + loc2 + " " + date + " " + catalog)) :
            Backend::query_ticket(GetChars(loc1 + " " + loc2 + " " + date + " " + catalog));
        char *str = ostr;
        if (!transfer)
        {
            int size = ReadInt(str);
            List<Ticket ^> ^list = gcnew List<Ticket ^>(size);
            for (int i = 0; i < size; ++i)
            {
                list->Add(ReadTicket(str));
            }
            delete[] ostr;
            return list;
        }
        else
        {
            List<Ticket ^> ^list = gcnew List<Ticket ^>(0);
            if (str[0] == '-' && str[1] == '1' && str[2] == '\n' && str[3] == '\0')
                ;
            else
            {
                list->Add(ReadTicket(str));
                list->Add(ReadTicket(str));
            }
            delete[] ostr;
            return list;
        }
    }

    Int32 ClrBackend::BuyTicket(Int32 ID, Int32 num, String ^trainID, String ^loc1, String ^loc2, String ^date, String ^ticketKind)
    {
        return Backend::buy_ticket(GetChars(ID.ToString() + " " + num + " " + trainID + " " + loc1 + " " + loc2 + " " + date + " " + ticketKind));
    }

    List<Ticket ^> ^ClrBackend::QueryOrder(Int32 ID, String ^date, String ^catalog)
    {
        char *ostr = Backend::query_order(GetChars(ID.ToString() + " " + date + " " + catalog));
        char *str = ostr;
        int size = ReadInt(str);
        if (size < 0)
            size = 0;
        List<Ticket ^> ^list = gcnew List<Ticket ^>(size);
        for (int i = 0; i < size; ++i)
        {
            list->Add(ReadTicket(str));
        }
        delete[] ostr;
        return list;
    }

    Int32 ClrBackend::RefundTicket(Int32 ID, Int32 num, String ^trainID, String ^loc1, String ^loc2, String ^date, String ^ticketKind)
    {
        return Backend::refund_ticket(GetChars(ID.ToString() + " " + num + " " + trainID + " " + loc1 + " " + loc2 + " " + date + " " + ticketKind));
    }

    Int32 ClrBackend::AddTrain(Train ^train)
    {
        return Backend::add_train(train->ToStr());
    }

    Int32 ClrBackend::SaleTrain(String ^trainID)
    {
        return Backend::sale_train(GetChars(trainID));
    }

    Train ^ClrBackend::QueryTrain(String ^trainID)
    {
        return gcnew Train(Backend::query_train(GetChars(trainID)));
    }

    Int32 ClrBackend::DeleteTrain(String ^trainID)
    {
        return Backend::delete_train(GetChars(trainID));
    }

    Int32 ClrBackend::ModifyTrain(Train ^train)
    {
        return Backend::modify_train(train->ToStr());
    }

}