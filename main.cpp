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

// Config files
#include "config.cpp"



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
    
    vector<User> getAllUsers()
    {
        vector<User> u;
        try
        {
            sql::PreparedStatement *pstmt = conn->prepareStatement("SELECT * FROM users");
            sql::ResultSet *rs = pstmt->executeQuery();
            while (rs->next())
            {
                string username = rs->getString("username");
                string password = rs->getString("password");
                string record = rs->getString("record");
                int id = rs->getInt("id");
                string role = rs->getString("role");
                if(role=="manager") continue;
                int fine = rs->getInt("fine");
                User u1(username, password, role);
                if (password == pass)
                    u1.authorise();
                u1.setId(id);
                u1.setFine(fine);
                u1.setRecord(record);
                u.push_back(u1);
            }
        }
        catch (const sql::SQLException &e)
        {
            cerr << "SQL error: " << e.what() << '\n';
        }
        return u;
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
                string record = rs->getString("record");
                int fine = rs->getInt("fine");
                User u1(username, password, role);
                if (password == pass)
                    u1.authorise();
                u1.setId(id);
                u1.setFine(fine);
                u1.setRecord(record);
                return u1;
            }
        }
        catch (const sql::SQLException &e)
        {
            cerr << "SQL error: " << e.what() << '\n';
        }
        User defaultUser("", "", "");
        return defaultUser;
    }
    bool saveUser(User u)
    {
        try
        {
            sql::PreparedStatement *pstmt = conn->prepareStatement("UPDATE users SET fine = ?,record=? WHERE username = ?");
            pstmt->setInt(1, u.fine);
            pstmt->setString(2,u.record);
            pstmt->setString(3, u.username);
            pstmt->execute();
            delete pstmt;
            return true;
        }
        catch (const sql::SQLException &e)
        {
            cerr << "SQL error: " << e.what() << '\n';
            return false;
        }
    }
    bool changePass(string username, string password)
    {
        try
        {
            sql::PreparedStatement *pstmt = conn->prepareStatement("UPDATE users SET password = ? WHERE username = ?");
            pstmt->setString(1, password);
            pstmt->setString(2, username);
            pstmt->execute();
            delete pstmt;
            return true;
        }
        catch (const sql::SQLException &e)
        {
            cerr << "SQL Error: " << e.what() << '\n';
            return false;
        }
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
            cerr << "SQL Error: " << e.what() << '\n';
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
    vector<Car> showCars()
    {
        vector<Car> result;
        try
        {
            sql::PreparedStatement *pt = conn->prepareStatement("SELECT * FROM cars;");
            sql::ResultSet *r = pt->executeQuery();

            while (r->next())
            {
                int id = r->getInt("id");
                string model = r->getString("model");
                string health = r->getString("health");
                string availability = r->getString("availability");
                int price = r->getInt("price");
                string due_date = r->getString("due_date");
                Car temp(model, health, price);
                temp.setAvailability(availability);
                temp.setDate(due_date);
                temp.setId(id);
                result.push_back(temp);
            }
            delete r;
            delete pt;
        }
        catch (const sql::SQLException &e)
        {
            std::cerr << e.what() << '\n';
        }
        return result;
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
            sql::PreparedStatement *pt = conn->prepareStatement("UPDATE cars SET model = ?, health = ?, price = ?,availability = ? , due_date = ? WHERE id = ?;");
            pt->setString(1, cr.model);
            pt->setString(2, cr.health);
            pt->setInt(3, cr.price);
            pt->setString(4, cr.availability);
            if (cr.due_date == "")
            {
                pt->setNull(5, sql::DataType::DATE);
            }
            else
            {
                pt->setString(5, cr.due_date);
            }
            pt->setInt(6, cr.id);
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
    bool rentCar(int id, string username, string due_date)
    {
        try
        {
            sql::PreparedStatement *pt = conn->prepareStatement("UPDATE cars SET availability = ?, due_date = ? WHERE id = ?;");
            pt->setString(1, username);
            pt->setString(2, due_date);
            pt->setInt(3, id);
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
        vector<Car> v1 = cartb->showCars();
        for (auto i : v1)
        {
            printDashes(20);
            PrintCarDetails(i);
        }
        printDashes(20);
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
        printDashes(20);
        PrintCarDetails(currCar);
        printDashes(20);
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
    void RentCar()
    {
        cout << "Enter Id of the car: ";
        int id;
        cin >> id;
        Car currCar = cartb->getCar(id);
        if (currCar.availability != "admin")
        {
            cout << "==> Car Not Available!! Rented By: " << currCar.availability << "\n";
            return;
        }
        cout << "\nEnter Username: ";
        string username;
        cin >> username;
        User u = tb->getUserByUsername(username, "");
        if (u.username == "")
        {
            cout << "\n\nUsername doesn't exist!!\n\n";
            return;
        }
        cout << "Enter Due-Date(YYYY-MM-DD): ";
        cin >> currCar.due_date;
        currCar.availability = username;
        cartb->updateCarDetails(currCar);
        cout << "\nRented Successfully!!\n\n";
    }
    void returnCar()
    {
        cout << "Enter Id of the car: ";
        int id;
        cin >> id;
        Car currCar = cartb->getCar(id);
        if (currCar.availability == "admin")
        {
            cout << "==> Car is Already in store\n";
            return;
        }
        currCar.availability = "admin";
        currCar.due_date = "";
        cartb->updateCarDetails(currCar);
        cout << "Car Returned Successfully!!\n\n";
    }
    void SeeAllUsers(){
        vector<User> uu = tb->getAllUsers();
        if(uu.size()==0) return;
        for(auto i: uu){
            printDashes(20);
            PrintUsers(i);
        }
        printDashes(20);
    }
    void UpdateUser(){
        cout<<"Enter Username: "; string username;cin>>username;
        auto u = tb->getUserByUsername(username,"");
        if(u.username == "") {
            cout<<"No user found\n\n";
            return;
        }
        string s;
        cout<<"\nUser Details: ";PrintUsers(u);printDashes(20);cout<<endl;
        int i; cout<<"1. Update Fine: \n2. Update Record:\n Choose One: ";cin>>i;
        switch (i)
        {
        case 1:
            cout<<"Enter fine: ";cin>>i; u.setFine(i);
            break;
        case 2: 
            cout<<"Enter Record(good,moderate,bad): ";cin>>s;u.setRecord(s);
            break;
        default:
            break;
        }
        tb->saveUser(u);
        cout<<"User updated !!!"<<endl;
    }
};

class EmployeeCustomer
{
private:
    UserTable *utb;
    CarTable *cartb;
    User *user;
    float priceFactor = 1;

public:
    EmployeeCustomer(UserTable *tb, CarTable *car, User *u)
    {
        this->utb = tb;
        this->cartb = car;
        this->user = u;
        if(u->role=="employee"){
            priceFactor = employeedis;
        }
    }
    void changePass()
    {
        string password;
        cout << "Enter Password: ";
        cin >> password;
        cout << "\n";
        if (utb->changePass(this->user->username, password))
        {
            cout << "Password Changed Successfully\n\n";
        }
        else
        {
            cout << "Error Changing pass\n\n";
        }
    }
    void seeRentedCars()
    {
        cout << "Your Rented cars:\n\n";
        vector<Car> v1 = cartb->showCars();
        int t = 0;
        for (auto i : v1)
        {
            i.price = priceFactor * i.price;
            if (i.availability == user->username)
            {
                printDashes(20);
                PrintCarDetails(i);
                t++;
            }
        }
        printDashes(20);
        if (t == 0)
        {
            cout << "0 Rented Cars\n";
        }
        printDashes(20);
    }
    void seeAvailableCars()
    {
        vector<Car> v1 = cartb->showCars();
        int t = 0;
        for (auto i : v1)
        {
            if (i.availability == "admin")
            {
                i.price = priceFactor * i.price;
                printDashes(20);
                PrintCarDetails(i);
                t++;
            }
        }
        printDashes(20);

        if (t == 0)
        {
            cout << "0 Available cars\n";
        }
        printDashes(20);
    }
    void rentCar()
    {
        if (user->record == "bad")
        {
            cout << "You record is bad sorry!!\n\n";
            return;
        }
        cout << "List of available cars:\n";
        this->seeAvailableCars();
        int id;
        cout << "Enter id of Car you want to Rent: ";
        cin >> id;
        Car c = cartb->getCar(id);
        if (c.availability == "admin")
        {
            c.availability = this->user->username;
            string date;
            cout << "Enter due date(YYYY-MM-DD): ";
            cin >> date;
            c.due_date = date;
            cartb->updateCarDetails(c);
            cout << "Car rented successfully\n";
        }
        else
        {
            cout << "Car not available\n"
                 << endl;
        }
    }
    void returnCar()
    {
        this->seeRentedCars();
        cout << "Enter id of car to return: ";
        int id;
        cin >> id;
        auto c = cartb->getCar(id);
        if (c.availability == this->user->username)
        {
            int fine = dateDiffInDays(c.due_date) * finePerDay;
            this->user->fine += fine;
            utb->saveUser(*(this->user));
            c.due_date = "";
            c.availability = "admin";
            cartb->updateCarDetails(c);
            cout << "Car returned successfully\n";
            this->seeFine();
        }
        else
        {
            cout << "This car is not with you\n";
        }
    }
    void seeFine()
    {
        auto u = utb->getUserByUsername(user->username, user->password);
        cout << "==>  Your fine is " << u.fine << "\n\n";
    }
};

int main(int argc, const char **argv)
{
    today = getCurrentDate();
    cout << "Today is " << today << endl;
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
            case 3:
                mang.UpdateUser();
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
            case 9:
                mang.returnCar();
                break;
            case 10:
                mang.SeeAllUsers();
                break;
            default:
                return 0;
                break;
            }
        }
        else if (currUser.role == "employee" || currUser.role == "customer")
        {
            EmployeeCustomer employee(&users, &cars, &currUser);
            int opt = Options();
            switch (opt)
            {
            case 1:
                employee.changePass();
                break;
            case 2:
                employee.seeRentedCars();
                break;
            case 3:
                employee.seeAvailableCars();
                break;
            case 4:
                employee.rentCar();
                break;
            case 5:
                employee.returnCar();
                break;
            case 6:
                employee.seeFine();
                break;
            default:
                return 0;
                break;
            }
        }
    }
    return 0;
}
