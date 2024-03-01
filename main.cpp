#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

// MySQL Libraries
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

// Helper files
#include "basic.cpp"
#include "models.cpp"

const string host = "localhost";
const string user = "sameer";
const string pass = "firstnews";
const string database = "rental";

class DatabaseConn
{
private:
    sql::Driver *driver;

public:
    sql::Connection *con;
    DatabaseConn(string url, string user, string pass, string database)
    {
        driver = get_driver_instance();
        con = driver->connect(url, user, pass);
        con->setSchema(database);
    }
    ~DatabaseConn()
    {
        delete con;
    }
};

class UserTable
{
private:
    sql::Connection *conn;

public:
    UserTable(DatabaseConn *db)
    {
        this->conn = db->con;
    }
    User getUserByUsername(string username, string password)
    {
        try
        {
            sql::PreparedStatement *pstmt = conn->prepareStatement("SELECT * FROM users WHERE username = ?");
            pstmt->setString(1, username);
            sql::ResultSet *rs = pstmt->executeQuery();
            if (rs->next())
            {
                string pass = rs->getString("password");
                int id = rs->getInt("id");
                string role = rs->getString("role");
                User u1(username, password, role);
                if (password == pass)
                    u1.authorise();
                u1.setId(id);
                return u1;
            }
        }
        catch (const sql::SQLException &e)
        {
            std::cerr << "SQL error: " << e.what() << '\n';
        }
        User defaultUser("", "", "");
        return defaultUser;
    }
    bool addUser(string username, string password, string role)
    {
        try
        {
            sql::PreparedStatement *pstmt = conn->prepareStatement("INSERT INTO  users(username,password,role) VALUES (?,?,?);");
            pstmt->setString(1, username);
            pstmt->setString(2, password);
            pstmt->setString(3, role);
            pstmt->execute();
            delete pstmt;
            return true;
        }
        catch (const sql::SQLException &e)
        {
            std::cerr << "SQL Error: " << e.what() << '\n';
            return false;
        }
    }
    bool deleteUser(string username)
    {
        try
        {
            sql::PreparedStatement *pstmt = conn->prepareStatement("DELETE FROM users where username = ?;");
            pstmt->setString(1, username);
            pstmt->execute();
            delete pstmt;
            return true;
        }
        catch (const sql::SQLException &e)
        {
            std::cerr << "SQL Error: " << e.what() << '\n';
            return false;
        }
    }
};

void PrintCarDetails(Car currCarr)
{
    cout << "Car ID: " << currCarr.id << endl;
    cout << "Model: " << currCarr.model << endl;
    cout << "Health: " << currCarr.health << endl;
    cout << "Availability: " << (currCarr.availability == "admin" ? "Available For Renting" : currCarr.availability)  << endl;
    cout << "Price: " << currCarr.price << endl;
    cout << "Due Date: " << (currCarr.due_date == "" ? "Not Rented" : currCarr.due_date) << endl;
}

class CarTable
{
private:
    sql::Connection *conn;

public:
    CarTable(DatabaseConn *db)
    {
        this->conn = db->con;
    }
    bool addCar(string model, string health, int price)
    {
        try
        {
            sql::PreparedStatement *pstmt = conn->prepareStatement("INSERT INTO cars(model,health,price) VALUES (?,?,?);");
            pstmt->setString(1, model);
            pstmt->setString(2, health);
            pstmt->setUInt(3, price);
            pstmt->execute();
            delete pstmt;
            return true;
        }
        catch (const sql::SQLException &e)
        {
            std::cerr << "SQL Error: " << e.what() << '\n';
            return false;
        }
    }
    void showCars()
    {
        try
        {
            sql::PreparedStatement *pt = conn->prepareStatement("SELECT * FROM cars;");
            sql::ResultSet *r = pt->executeQuery();

            while (r->next())
            {
                printDashes(20);
                int id = r->getInt("id");
                string model = r->getString("model");
                string health = r->getString("health");
                string availability = r->getString("availability");
                int price = r->getInt("price");
                string due_date = r->getString("due_date");
                Car temp(model,health,price);temp.setAvailability(availability);temp.setDate(due_date);temp.setId(id);
                PrintCarDetails(temp);
            }
            printDashes(20);

            delete r;
            delete pt;
        }
        catch (const sql::SQLException &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    bool deleteCar(int id)
    {
        try
        {
            sql::PreparedStatement *pt = conn->prepareStatement("DELETE FROM cars WHERE id = ?;");
            pt->setInt(1, id);
            pt->execute();
            delete pt;
            return true;
        }
        catch (const sql::SQLException &e)
        {
            std::cerr << "SQL Error: " << e.what() << '\n';
            return false;
        }
    }
    Car getCar(int id)
    {
        try
        {
            sql::PreparedStatement *pt = conn->prepareStatement("SELECT * FROM cars WHERE id = ?;");
            pt->setInt(1, id);
            sql::ResultSet *rs = pt->executeQuery();
            if (rs->next())
            {
                string model, health, availability, due_date;
                int id, price;
                model = rs->getString("model");
                health = rs->getString("health");
                availability = rs->getString("availability");
                id = rs->getInt("id");
                price = rs->getInt("price");
                due_date = rs->getString("due_date");
                Car currCar(model, health, price);
                currCar.setId(id);
                currCar.setAvailability(availability);
                currCar.setDate(due_date);
                return currCar;
            }
            delete pt;
        }
        catch (const sql::SQLException &e)
        {
            std::cerr << "SQL Error: " << e.what() << '\n';
        }
        Car cr("", "", 0);
        return cr;
    }
    bool updateCarDetails(Car cr)
    {
        try
        {
            sql::PreparedStatement *pt = conn->prepareStatement("UPDATE cars SET model = ?, health = ?, price = ? WHERE id = ?;");
            pt->setString(1, cr.model);
            pt->setString(2, cr.health);
            pt->setInt(3, cr.price);
            pt->setInt(4, cr.id);
            pt->execute();
            delete pt;
            return true;
        }
        catch (const sql::SQLException &e)
        {
            std::cerr << "SQL Error: " << e.what() << '\n';
            return false;
        }
    }
    bool rentCar(int id,string username,string due_date){
        try
        {
            sql::PreparedStatement *pt = conn->prepareStatement("UPDATE cars SET availability = ?, due_date = ? WHERE id = ?;");
            pt->setString(1,username);
            pt->setString(2,due_date);
            pt->setInt(3,id);
            pt->execute();
            delete pt;
            return true;
        }
        catch (const sql::SQLException &e)
        {
            std::cerr << "SQL Error: " << e.what() << '\n';
            return false;
        }
    }
};

class Manager
{
private:
    UserTable *tb;
    CarTable *cartb;

public:
    Manager(UserTable *tb, CarTable *car)
    {
        this->tb = tb;
        this->cartb = car;
    }
    void UserRegister()
    {
        string role = SelectRole();
        string username, password;
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;
        tb->addUser(username, password, role);
    }
    void DeleteUser()
    {
        string username;
        cout << "Enter Username to delete: ";
        cin >> username;
        cout << "\n";
        if ((tb->deleteUser(username)))
        {
            cout << "User " << username << " successfully deleted!!\n";
        }
        else
        {
            cout << "Error deleting user\n";
        }
    }
    void AddCar()
    {
        string model, health;
        int price;
        cout << "Enter Model Name: ";
        cin >> model;
        cout << "\nEnter Health(good,moderate,bad): ";
        cin >> health;
        cout << "\nEnter Price: ";
        cin >> price;
        cout << "\n";
        cartb->addCar(model, health, price);
    }
    void SeeCars()
    {
        cartb->showCars();
    }
    void DeleteCarById()
    {
        int id;
        cout << "Enter id: ";
        cin >> id;
        cout << "\n";
        if ((cartb->deleteCar(id)))
        {
            cout << "Deleted Successfully!\n";
        }
        else
        {
            cout << "Error occured\n";
        }
    }
    void UpdateCar()
    {
        cout << "Enter Id of the car: ";
        int id;
        cin >> id;
        cout << "\n";
        Car currCar = cartb->getCar(id);
        printDashes(20);PrintCarDetails(currCar);printDashes(20);
        int n = CarUpdate();
        switch (n)
        {
        case 1:
            cout << "Enter Health(good,moderate,bad): ";
            cin >> currCar.health;
            break;
        case 2:
            cout << "Enter Price: ";
            cin >> currCar.price;
            break;
        default:
            break;
        }
        cartb->updateCarDetails(currCar);
        cout << "\nUpdated!!!\n\n";
    }
    void RentCar(){
        cartb->rentCar(1,"dopa","12, July, 2023");
    }
};

void EmployeeHandler(){

};

void CustomerHandler(){

};

int main(int argc, const char **argv)
{
    DatabaseConn RentalDB(host, user, pass, database);
    UserTable users(&RentalDB);
    CarTable cars(&RentalDB);
    auto creds = handleSignIn();
    User currUser = users.getUserByUsername(creds.first, creds.second);
    if (!currUser.isAuthorised)
    {
        cout << "Error Signing You!! \n\n";
        return 0;
    }
    cout << "Signed in Successfully!!\n\n"
         << "Your Role: " << currUser.role << "\n\n";
    while (true)
    {
        if (currUser.role == "manager")
        {
            Manager mang(&users, &cars);
            int opt = ManagerOptions();
            switch (opt)
            {
            case 1:
                mang.UserRegister();
                break;
            case 2:
                mang.DeleteUser();
                break;
            case 4:
                mang.AddCar();
                break;
            case 5:
                mang.UpdateCar();
                break;
            case 6:
                mang.DeleteCarById();
                break;
            case 7:
                mang.SeeCars();
                break;
            case 8:
                mang.RentCar();
                break;
            default:
                return 0;
                break;
            }
        }
        else if (currUser.role == "employee")
        {
            EmployeeHandler();
        }
        else if (currUser.role == "customer")
        {
            CustomerHandler();
        }
    }
    return 0;
}
