#include <iomanip>
#include <chrono>
#include <string>
#include <ctime>
#include <iostream>
using namespace std;



string getCurrentDate()
{
    time_t currentTime = time(nullptr);
    tm *localTime = localtime(&currentTime);
    ostringstream oss;
    oss << put_time(localTime, "%Y-%m-%d");
    return oss.str();
}
string today;

chrono::system_clock::time_point stringToTimePoint(const string &dateStr)
{
    tm tm = {};
    istringstream ss(dateStr);
    ss >> get_time(&tm, "%Y-%m-%d");
    time_t tt = mktime(&tm);
    return chrono::system_clock::from_time_t(tt);
}

int dateDiffInDays(const string &dateStr1)
{
    auto timePoint1 = stringToTimePoint(dateStr1);
    auto timePoint2 = stringToTimePoint(today);
    auto diffInSeconds = chrono::duration_cast<chrono::seconds>(timePoint2 - timePoint1).count();
    auto t = (diffInSeconds / (24 * 60 * 60));
    return t > 0 ? t : 0;
}

pair<string, string> handleSignIn()
{
    pair<string, string> data;
    cout << "Enter Your Username: ";
    cin >> data.first;
    cout << "Enter Your Password: ";
    cin >> data.second;
    return data;
}

int ManagerOptions()
{
    cout << "What You want to do?? " << endl;
    cout << "1. Add a User" << endl;
    cout << "2. Delete a User" << endl;
    cout << "3. Update a User" << endl;
    cout << "4. Add a Car" << endl;
    cout << "5. Update a car" << endl;
    cout << "6. Delete a Car" << endl;
    cout << "7. See All cars" << endl;
    cout << "8. Rent car" << endl;
    cout << "9. Return car"<<endl;
    cout<<"10. See All Users"<<endl;
    cout << "11. Exit" << endl;
    cout << "Select one of the options: ";
    int t;
    cin >> t;
    cout << "\n";
    return t;
}

int Options()
{
    cout<<"What you want to do??\n";
    cout<<"1. Change Password\n";
    cout<<"2. See Rented Cars\n";
    cout<<"3. See Available Cars\n";
    cout<<"4. Rent a car\n";
    cout<<"5. Return a car\n";
    cout<<"6. See Fine\n";
    cout<<"7. Exit\n";
    cout<<"Select one of the options: ";
    int t;cin>>t;cout<<"\n";
    return t;
}

string SelectRole()
{
    cout << "Choose Role: " << endl;
    cout << "1. Manager" << endl;
    cout << "2. Employee" << endl;
    cout << "3. Customer" << endl;
    cout << "Select one of the options: ";
    int i;
    cin >> i;
    cout << "\n";
    switch (i)
    {
    case 1:
        return "manager";
        break;
    case 2:
        return "employee";
        break;
    case 3:
        return "customer";
        break;
    }
    return "";
}

void printDashes(int n)
{
    cout << endl;
    while (n--)
    {
        cout << "----";
    }
    cout << endl;
}

int CarUpdate()
{
    cout << "Which of the following you want to update?\n";
    cout << "1. Health\n2. Price\n";
    cout << "Select one of the above: ";
    int i;
    cin >> i;
    cout << "\n";
    return i;
};

