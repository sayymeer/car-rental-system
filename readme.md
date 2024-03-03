# Car Rental System

My machine is `Ubuntu Linux 22.04`

Tech Stack used:

1. C++
2. MySQL
3. MySQL CPP connector for Ubuntu Linux 22.04(x86, 64-bit)

## Setup

> Make sure your machine has MySQL Server installed in it.

Follow the below instructions to install MySQL CPP Connector:

```bash
# Installing MySQL Connector
wget https://dev.mysql.com/get/Downloads/Connector-C++/libmysqlcppconn9_8.3.0-1ubuntu22.04_amd64.deb
sudo dpkg -i libmysqlcppconn9_8.3.0-1ubuntu22.04_amd64.deb
sudo apt-get update
sudo apt-get install libmysqlcppconn-dev
```

After this create a database `rental` which will have following tables. Also Update the username and password of database in `config.cpp`.

```sql
CREATE TABLE cars (
    id INT AUTO_INCREMENT PRIMARY KEY,
    model VARCHAR(255),
    health ENUM('good', 'moderate', 'bad'),
    availability VARCHAR(255) DEFAULT 'admin',
    price INT UNSIGNED,
    due_date DATE DEFAULT NULL
);

CREATE TABLE user (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(255) UNIQUE,
    password VARCHAR(255),
    role ENUM('employee', 'manager', 'customer'),
    fine INT UNSIGNED,
    record ENUM('good','moderate','bad')
);

INSERT INTO users (username, password, role)
VALUES ('admin', 'admin', 'manager');

```

Now compile the main.cpp using below command.

```bash
sudo g++ -Wall -I/usr/include/cppconn -o main main.cpp -L/usr/lib -lmysqlcppconn -Wno-deprecated-declarations
```

Now run the executable you get `./main` and Login using username `admin` and password `admin`. Now you can create user or cars.
