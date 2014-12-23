# MicroChat

MicroChat is a HTTP server with HTML client which allows people write messages to each other

---

Required packages:
+ `make`  
+ `g++`  
+ `libmicrohttpd-dev`  
+ `mysql-server`  
+ `mysql-client`  
+ `libmysqlcppconn-dev`  

---

0. Go to MicroChat directory:  
`$ cd /path/to/microchat/`

1. Build server:  
`$ make`

2. Prepare MySQL database:  
`$ ./setupdb.sh`

3. Run:  
`$ ./run`

---

Access the server in your browser at:  
`http://localhost:8888`

The client tested only in `Firefox` browser version >= 31 
and may be shown incorrect in other browsers
