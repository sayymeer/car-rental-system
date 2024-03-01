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


`sudo g++ -Wall -I/usr/include/cppconn -o main main.cpp -L/usr/lib -lmysqlcppconn -Wno-deprecated-declarations`