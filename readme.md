# MicroChat

MicroChat is a HTTP server with HTML client which allows people write messages to each other

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

---

Required packages:
+ `make`  
+ `g++`  
+ `libmicrohttpd-dev`  
+ `mysql-server`  
+ `mysql-client`  
+ `libmysqlcppconn-dev`  
