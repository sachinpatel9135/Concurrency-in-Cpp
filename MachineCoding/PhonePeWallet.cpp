/*
PhonePe Mini Wallet
Design a prepaid mini wallet for PhonePe through which users can send and receive money among themselves and can win exciting offers.
The user can load money into the wallet via. various sources like CC, DC, UPI, etc, and perform easy transactions.


Requirements


Users need to register on PhonePe to use this wallet.
The minimum amount of transaction will be always greater the 0.
The user must have a sufficient balance in his wallet while doing the transaction.
The user can fetch their wallet balance.
The user can sort transactions history (eg. based on date, amount, etc)
The user can filter transactions history (eg. based on user etc)
The transactions will be eligible for exciting offers if they meet their respective criteria.
Capabilities
Below are various functions that should be supported with the necessary parameters passed.
These are just suggestions, the signatures can be altered as long as the functionalities are provided.
registerUser - register user to use wallet
topUpWallet() - add money to the wallet
fetchBalance() - fetch balance available in the wallet
sendMoney() - send money to the user, all the eligible offers will be applicabie for transaction
get Transactions(filter, sorter) - fetches transactions history based on filter and sorting criteria.


Offers
Following are example offers supported by the wallet


The first transaction amounting greater than 100 will be eligible for 10% cashback
If the wallet balance of both the sender and receiver becomes equal after the transaction
Guidelines


You should store the data in memory using a language-specific data structure.
Implement clear separation between your data layers and service layers.
Simple and basic functions are expected as entry point - no marks for providing fancy restful API or framework implementation
Because this is a machine coding round, heavy focus would be given on data modeling, code quality etc, candidate should not focus too much time on algo which compromises with implementation time
Expectations:


Your code should cover all the mandatory functionalities explained above.
The code should be extendable and scalable to incorporate future extensions.
Your code should be executable and clean.
Your code should be properly refactored, and exceptions should be gracefully handled.
Appropriate errors should be displayed on the console
Code should be extendaple to support multiple offers, fiters and sorters
How will you be evaluated?


Code Should be working
Code readability and testability
Separation Of Concerns
Abstraction
Object-Oriented concepts
Exception handling
Language proficiency.
Scalability
Test Coverage (Bonus Points)
[execution time limit) 4 seconds (sh)
[memory limit) 1 GB
Verdict : Shortlisted for next round as I have implemented design pattern in such a way that the code is scalable.Also, I have implemented the bonus question(offer) in my code too.

Actors - User, System
Entities - Wallet, Transaction, Offer

User - registerUser(), addPaymentToWallet(), makePayment()
registerUser - register user to use wallet
topUpWallet() - add money to the wallet
fetchBalance() - fetch balance available in the wallet
sendMoney() - send money to the user, all the eligible offers will be applicabie for transaction
get Transactions(filter, sorter) - fetches transactions history based on filter and sorting criteria.

Transaction - Sender, Reciever, Amount, Status, Time, Date;
Offer - CashbackPercentage
*/

#include<bits/stdc++.h>

using namespace std;

enum Status {
    NEW,
    IN_PROGRESS,
    FAILED,
    COMPLETED
};

class Transaction {
private:
    string transactionId;
    string senderId;
    string recieverId;
    double amount;
    Status status;
    string time;
    string date;
    int cnt = 0;
public:
    Transaction(string senderId, string recieverId, double amount) {
        this->senderId = senderId;
        this->recieverId = recieverId;
        this->amount = amount;
        this->transactionId = to_string(++cnt);
        this->status = Status::NEW;
    }

    Status getStatus() {
        return status;
    }

    void setStatus(Status status) {
        this->status = status;
    }

    string getDate() {
        return date;
    }
};



class Account {
private:
    string id;
    string name;
    string number;
public:
    Account(string id, string name, string number) {
        this->id = id;
        this->name = name;
        this->number = number;
    }

    string getName() {
        return name;
    }

    string getId() {
        return id;
    }
};

class User: public Account{
private:
    double balance;
    vector<Transaction> transactions;
    bool firstTransaction;
public:
    User(string id, string name, string number): Account(id, name, number) {
        this->firstTransaction = false;
    }

    void addBalance(double amount) {
        balance += amount;
    }

    double getBalance() {
        return balance;
    }

    void addTransaction(Transaction transaction) {
        transactions.push_back(transaction);
    }

    bool isFirstTransaction() {
        return firstTransaction;
    }
    
    void setFirstTransaction() {
        this->firstTransaction = true;
    }

    vector<Transaction> getTransactions() {
        return transactions;
    }
};

class TransactionSortStrategy {
private:
public:
    virtual vector<Transaction> sortBy(vector<Transaction> transactions) = 0;
};

bool dateComparator(Transaction& transaction1, Transaction& transaction2) {
    return transaction1.getDate() > transaction2.getDate();
}

class DateTransactionStrategy : public TransactionSortStrategy {
public:
    vector<Transaction> sortBy(vector<Transaction> transactions) {
        vector<Transaction> sortedTransactions = transactions;
        sort(sortedTransactions.begin(), sortedTransactions.end(), dateComparator);
        return sortedTransactions;
    }
};

class TransactionService {
private:
    TransactionSortStrategy* transactionSortStrategy;
public:
    void setTransactionFilterStrategy(TransactionSortStrategy* transactionSortStrategy) {
        this->transactionSortStrategy = transactionSortStrategy;
    }
};


class UserService {
private:
    unordered_map<string, User*> users;
    int cnt = 0;
    TransactionService transactionService;
public:
    User* registerUser(string name, string number) {
        string id = to_string(++cnt);
        User* user = new User(id, name, number);
        users[id] = user;
        return user;
    }

    void topUpWallet(string userId, double amount, string paymentType) {
        User* user = users[userId];
        cout << "User: " << user->getName() << " added amount" << amount << " using payment method " << paymentType << "\n";
        user->addBalance(amount);
    }

    double fetchBalance(string userId) {
        return users[userId]->getBalance();
    }

    void sendMoney(string senderId, string recieverId, double amount) {
        if(amount <= 0) throw "Amount should be greater than 0";
        User* sender = users[senderId];
        if(sender->getBalance() < amount) throw "Sender don't have enough balance in wallet.";
        User* reciever = users[recieverId];
        Transaction* transaction = new Transaction(senderId, recieverId, amount);
        reciever->addBalance(amount);
        sender->addBalance(-amount);
        transaction->setStatus(Status::COMPLETED);
        if(transaction->getStatus() == Status::COMPLETED) reciever->addTransaction(*transaction);
        sender->addTransaction(*transaction);
        if(!sender->isFirstTransaction() && amount > 100) {
            Transaction* giftTransaction = new Transaction("gift", senderId, amount*10.0/100);
            sender->addBalance(amount*10.0/100);
            giftTransaction->setStatus(Status::COMPLETED);
            sender->setFirstTransaction();
            sender->addTransaction(*giftTransaction);
            cout << "Cashback of amount " << amount*10.0/100 << " is added to wallet\n";
        }
    }

    vector<Transaction> getTransactions(string userId, TransactionSortStrategy transactionSortStrategy) {
        User* user = users[userId];
        vector<Transaction> transactions = user->getTransactions();
        return transactions;
    }
};

int main() {
    UserService* userService = new UserService();
    User* user1 = userService->registerUser("sachin", "123");
    User* user2 = userService->registerUser("abcd", "9876");
    userService->topUpWallet(user1->getId(), 500.0, "UPI");
    userService->sendMoney(user1->getId(), user2->getId(), 130.0);
    cout << "Balance of User 1 " << user1->getBalance() << "\n";
    cout << "Balance of User 2 " << user2->getBalance() << "\n";
    return 0;
}