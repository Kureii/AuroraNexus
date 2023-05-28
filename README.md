# AuroraNexus
AuroraNexus is a content management system (CMS) based on the crown framework

## Install

Update and upgrade package system.
```bash
# Debian based
sudo apt update && sudo apt upgrade
```

Install libraries
```bash
# Debian based
sudo apt-get install libboost-all-dev 
```
download and install [Connector/C++ 8.0.33](https://dev.mysql.com/downloads/connector/cpp/)

Install MySQL
```bash
sudo apt install mysql-server 
```
configure MySql
```bash
sudo mysql
```

set password for root – replace 'your_password' with your password
```sql
ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'your_password';
FLUSH PRIVILEGES;
exit
```

login as root user
```bash
mysql -u root -p
```

create database and user, 'mypassword' replace with your password for aurora-nexus user
```sql
CREATE DATABASE aurora_nexus;
CREATE USER 'aurora_nexus'@'localhost' IDENTIFIED BY 'mypassword';
exit
```

