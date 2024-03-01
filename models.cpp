#include <string>
using namespace std;

class User
{
public:
    bool isAuthorised = false;
    int id;
    string record;
    string username;
    string password;
    string role;
    int fine;
    User(string username, string password, string role)
    {
        this->username = username;
        this->password = password;
        this->role = role;
    }
    void setId(int id)
    {
        this->id = id;
    }
    void authorise()
    {
        isAuthorised = true;
    }
    void setFine(int i){
        this->fine = i;
    }
    void setRecord(string rec){
        this->record = rec;
    }
};

class Car
{
public:
    string model;
    int id;
    string health;
    string availability;
    int price;
    string due_date;
    Car(string model, string health, int price)
    {
        this->model = model;
        this->health = health;
        this->price = price;
    }
    void setId(int id)
    {
        this->id = id;
    }
    void setDate(string due_date)
    {
        this->due_date = due_date;
    }
    void setAvailability(string availability)
    {
        this->availability = availability;
    }
};