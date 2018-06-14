#pragma once
#include "backend.hpp"
using namespace System;
using namespace System::Collections::Generic;

namespace Clr
{
    public ref class User
    {
    public:
        String ^ name, ^password, ^email, ^phone;
        Int32 ID, privilege;

        User();
        User(Int32 id, char *s);
    };

    public ref class Ticket
    {
    public:
        String ^ trainID, ^locFrom, ^locTo, ^dateFrom, ^dateTo, ^timeFrom, ^timeTo;
        array<String ^> ^ticketKind;
        array<Int32> ^num;
        array<Double> ^price;
        Int32 cnt;

        Ticket();
        virtual String^ ToString() override;
    };

    public ref class Station
    {
    public:
        String ^ name, ^timeArrive, ^timeStart, ^timeStopover;
        array<Double> ^price;
        Int32 numPrice;

        Station();
    };

    public ref class Train
    {
    public:
        String ^ trainID, ^name, ^catalog;
        array<String ^> ^namePrice;
        Int32 numStation, numPrice;
        Boolean IsSaled;
        List<Station ^> ^stations;

        Train();
        Train(char *strTrain);
        char *ToStr();
    };

    public ref class ClrBackend
    {
    public:
        String ^ ErrorMessage;

        ClrBackend();
        void Register(User ^user);
        Int32 Login(Int32 ID, String ^password);
        User ^QueryProfile(Int32 ID);
        Int32 ModifyProfile(User ^user);
        Int32 ModifyPrivilege(Int32 ID1, Int32 ID2, Int32 privilege);

        List<Ticket ^> ^QueryTicket(String ^loc1, String ^loc2, String ^date, String ^catalog, Boolean transfer);
        Int32 BuyTicket(Int32 ID, Int32 num, String ^trainID, String ^loc1, String ^loc2, String ^date, String ^ticketKind);
        List<Ticket ^> ^QueryOrder(Int32 ID, String ^date, String ^catalog);
        Int32 RefundTicket(Int32 ID, Int32 num, String ^trainID, String ^loc1, String ^loc2, String ^date, String ^ticketKind);

        Int32 AddTrain(Train ^train);
        Int32 SaleTrain(String ^trainID);
        Train ^QueryTrain(String ^trainID);
        Int32 DeleteTrain(String ^trainID);
        Int32 ModifyTrain(Train ^train);
    };
}