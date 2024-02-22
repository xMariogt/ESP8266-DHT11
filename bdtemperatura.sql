drop database if exists practica3;
create database practica3;
use practica3;

create table temperatura(
	id int not null primary key,
	temp int not null
    );
    
select * from temperatura;

insert into temperatura(id, temp) values (1,40);
    
