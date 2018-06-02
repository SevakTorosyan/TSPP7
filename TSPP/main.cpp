//
//  main.cpp
//  TSPP
//
//  Created by Sevak on 30.05.2018.
//  Copyright © 2018 Sevak. All rights reserved.
//
// коммент
#include <iostream>
#include <ctime>
#include <stdlib.h>

using namespace std;
class bank {
    string name;
public:
    bank(string _name) {
        name=_name;
    }
};
class card;
class Transaction;
class ATM {
    bank *owner;
    int balance;
    Transaction *CurrentTransaction;
public:
    ATM(bank *_owner,    int _balance) {
        owner=_owner;
        balance=_balance;
    }
    
    card *InsertedCard;
    bool CheckMoney(int sum);
    void MakeTransaction(int mode, int sum);
    void MakeTransaction(int mode, int sum,card *OtherCard);
    void PrintCheck(int mode);
    bool CheckPin(int Pin);
};
class user {
    string name;
    ATM *UsedATM;
public:
    user(string _name) {
        name=_name;
    };
    void InsertCard(card *UsedCard) {
        UsedATM->InsertedCard=UsedCard;
    };
    int InsertPin() {
        int pin;
        cin>>pin;
        return pin;
    };
    void GetMoney(int sum) {
        if(UsedATM->CheckMoney(sum))
            UsedATM->MakeTransaction(1,sum);
        else
            cout<<"Не достаточно средств на счету или в банкомате\n";
    };
    void Deposit(int sum, card *OtherCard);
    void ChooseATM(ATM *UsedMachine) {
        UsedATM=UsedMachine;
    };
};
class card {
    user *Owner;
    bank *SelfBank;
public:
    string code;
    int pin;
    double balance;
    card(int _pin,double _balance, string _code, user *_owner, bank *_selfbank) {
        pin=_pin;
        balance=_balance;
        code=_code;
        Owner=_owner;
        SelfBank=_selfbank;
    }
};
bool ATM::CheckPin(int Pin) {
    if (Pin==InsertedCard->pin)
        return true;
    else
        return false;
}

class Transaction {
    int code;
    time_t date;
    int sum;
    string UsedCardcode;
    string OtherCardcode;
public:
    int GetCode() {
        return code;
    };
    time_t GetDate() {
        return date;
    };
    int Getsum() {
        return sum;
    }
    string GetOtherCardcode() {
        return OtherCardcode;
    }
    Transaction(int _sum,string _UsedCardcode) {
        code=rand() % 10000;
        sum=_sum;
        UsedCardcode=_UsedCardcode;
        date= time(0);
        
    };
    Transaction(int _sum,string _UsedCardcode, string _OtherCardcode) {
        code=rand() % 10000;
        sum=_sum;
        UsedCardcode=_UsedCardcode;
        OtherCardcode=_OtherCardcode;
        date= time(0);
        
    };
};
void user::Deposit(int sum, card *OtherCard) {
    if(UsedATM->InsertedCard->balance<sum)
        cout<<"Не достаточно средств на счету\n";
    else
        UsedATM->MakeTransaction(2,sum,OtherCard);
};
void ATM::MakeTransaction(int mode, int sum) {
    balance=balance-sum;
    InsertedCard->balance=InsertedCard->balance-sum;
    CurrentTransaction=new Transaction(sum,InsertedCard->code);
};
void ATM::MakeTransaction(int mode, int sum,card *OtherCard) {
    InsertedCard->balance=InsertedCard->balance-sum;
    OtherCard->balance=OtherCard->balance+sum;
    CurrentTransaction=new Transaction(sum,InsertedCard->code,OtherCard->code);
};
void ATM::PrintCheck(int mode) {
    
    cout<<"Номер транзакции "<<CurrentTransaction->GetCode()<<"\n";
    time_t now=CurrentTransaction->GetDate();
    cout<<"Время транзакции "<< ctime(&now)<<"\n";
    cout<<"Выведено средств "<<CurrentTransaction->Getsum()<<"\n";
    cout<<"Использована карта "<<InsertedCard->code<<"\n";
    if(mode==2) {cout<<"Card deposited"<<CurrentTransaction->GetOtherCardcode()<<"\n";
    }
    CurrentTransaction=NULL;
}
bool ATM::CheckMoney(int sum){if(sum<=balance && sum<=InsertedCard->balance)return true;
else return false;
}
int main() {
    user *TestUser=new user("Торосян Севак Артушович");
    bank *TestBank=new bank("RNCB");
    card *TestCard1=new card(1111,1000, "Debit",TestUser,TestBank);
    card *TestCard2=new card(1111,1000, "Credit",TestUser,TestBank);
    ATM *TestATM=new ATM(TestBank,2000) ;
    TestUser->ChooseATM(TestATM);
    int Choose;
    int sum;
    while(1) {
        cout<<"Введите номер карты \n"; // ввод номера карты
        cin>>Choose;
        switch(Choose) {
            case 1 : {
                TestUser->InsertCard(TestCard1);
                break;
            }
            case 2 : {
                TestUser->InsertCard(TestCard2);
                break;
            }
            default: {
                cout << "Вы ввели номер которого нет в базе \n" << "До свидания!\n"; //ghbdtn
                return 1;
            }
        }
        cout<<"Введите PIN\n";
        int TestPin=TestUser->InsertPin();
        while(!TestATM->CheckPin(TestPin)){cout<<"Неверный PIN! Повторите попытку\n";
            TestPin=TestUser->InsertPin();
        }
        cout<<"Выберите операцию: 1 снять наличные или 2 пополнить карту\n";
        cin>>Choose;
        switch(Choose) {
            case 1 : cout<<"Введите сумму\n";
                cin>>sum;
                TestUser->GetMoney(sum);
                cout<<"Печатать чек: 1 – ДА, 2 – НЕТ\n";
                cin>>Choose;
                if(Choose==1)
                    TestATM->PrintCheck(1);
                break;
            case 2 : cout<<"Выберите сумму\n";
                cin>>sum;
                cout<<"Выберите карту для пополнения: 1 or 2\n";
                cin>>Choose;
                switch(Choose) {
                    case 1 :     TestUser->Deposit(sum,TestCard1);
                        break;
                    case 2 :     TestUser->Deposit(sum,TestCard2);
                        break;
                }
                cout<<"Печатать чек: 1 – ДА, 2 – НЕТ\n";
                cin>>Choose;
                if(Choose==1)
                    TestATM->PrintCheck(2);
                break;
        }
        cout<<"Нажмите 2 чтобы выйти, или другую цифру для продолжения\n";
        cin>>Choose;
        if(Choose==2)
            return 0;
    }
    return 1;
}

