#include <iostream>
using namespace std;

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
    cout << "9. Exit" << endl;
    cout << "Select one of the options: ";
    int t;
    cin >> t;
    cout << "\n";
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