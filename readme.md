# MicroChat

MicroChat is a HTTP server with you HTML client which allows people write messages to each other

1. Build:
`cd /path/to/microchat/`
`$ make`
2. Run:
`./run`

## Required packages:
+ `make`
+ `g++`
+ `libmicrohttpd-dev`
+ `mysql-server`
+ `libmysqlcppconn-dev`

MySQL database must have user 'microchat@localhost' and 'microchatdb' database
All privileges on microchatdb must be granted to 'microchat@localhost'
`create user microchat@localhost;`
`create database microchatdb;`
`grant all on microchatdb.* to microchat@localhost;`

To access access the server enter in your browser:
`localhost:8888`
