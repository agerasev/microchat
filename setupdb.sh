
echo "Creating database and user..."

echo "Enter your MySQL root password:"
echo \
"create user if not exists microchat@localhost;" \
"create database if not exists microchatdb;" \
"grant all on microchatdb.* to microchat@localhost;" \
| mysql -u root -p

echo "Creating tables..."

#echo "drop table messages;" | mysql "microchatdb" -u "microchat"
#echo "drop table conversations;" | mysql "microchatdb" -u "microchat"
#echo "drop table accounts;" | mysql "microchatdb" -u "microchat"

echo \
"create table if not exists" \
"accounts (" \
"id int auto_increment not null," \
"username varchar(64) not null," \
"password varchar(32) not null," \
"first_name varchar(32)," \
"last_name varchar(32)," \
"unique(username)," \
"primary key(id)" \
");" \
| mysql "microchatdb" -u "microchat"
		
echo \
"create table if not exists" \
"conversations (" \
"id int auto_increment not null," \
"name varchar(64)," \
"messages_number int not null default 0," \
"last_update datetime," \
"unique (name)," \
"primary key(id)" \
");" \
| mysql "microchatdb" -u "microchat"
		
echo \
"create table if not exists" \
"messages (" \
"id int auto_increment not null," \
"author_id int not null," \
"conversation_id int not null," \
"text varchar(4096)," \
"send_time datetime," \
"primary key(id)," \
"foreign key(author_id) references accounts(id)," \
"foreign key(conversation_id) references conversations(id)" \
");" \
| mysql "microchatdb" -u "microchat"

echo "Done."
