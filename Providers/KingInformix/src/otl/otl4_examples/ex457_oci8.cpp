#include <iostream>
using namespace std;

#include <stdio.h>

//#define OTL_ORA7 // Compile OTL 4.0/OCI7
#define OTL_ORA8I // Compile OTL 4.0/OCI8
//#define OTL_ORA9I // Compile OTL 4.0/OCI9i
//#define OTL_ORA10G // Compile OTL 4.0/OCI10g
//#define OTL_ORA10G_R2 // Compile OTL 4.0/OCI10g
#define OTL_STREAM_READ_ITERATOR_ON
#include <otlv4.h> // include the OTL 4.0 header file

otl_connect db; // connect object

void insert()
// insert rows into table
{ 

 otl_long_string f2(700); // define long string variable
 otl_stream o(10, // buffer size
              "insert into test_tab values(:f1<int>,:f2<raw[700]>)",
                 // SQL statement
              db // connect object
             );
 for(int i=1;i<=20;++i){
  for(int j=0;j<699;++j)
   f2[j]='*';
  f2[699]='?';
  f2.set_len(700);
  o<<i<<f2;
 }

}

void select()
{ 
 otl_long_string f2(700); // define long string variable
 otl_stream i(10, // buffer size
              "select * from test_tab where f1>=:f11<int> and f1<=:f12<int>*2",
                 // SELECT statement
              db // connect object
             ); 
   // create select stream
 
 int f1;
 otl_stream_read_iterator<otl_stream,otl_exception,otl_lob_stream> rs;

 rs.attach(i); // attach the iterator "rs" to the stream "i".
 i<<8<<8; // assigning :f11 = 8, :f12 = 8
   // SELECT automatically executes when all input variables are
   // assigned. First portion of output rows is fetched to the buffer

 while(rs.next_row()){ // while not end-of-data
  rs.get(1,f1);
  rs.get(2,f2);
  cout<<"f1="<<f1<<", f2="<<f2[0]<<f2[f2.len()-1]<<", len="<<f2.len()<<endl;
 }

 i<<4<<4; // assigning :f11 = 4, :f12 = 4
   // SELECT automatically executes when all input variables are
   // assigned. First portion of output rows is fetched to the buffer

 while(rs.next_row()){ // while not end-of-data
  rs.get(1,f1);
  rs.get(2,f2);
  cout<<"f1="<<f1<<", f2="<<f2[0]<<f2[f2.len()-1]<<", len="<<f2.len()<<endl;
 }

 rs.detach(); // detach the itertor from the stream

}

int main()
{
 otl_connect::otl_initialize(); // initialize the database API environment
 try{

  db.rlogon("scott/tiger"); // connect to Oracle

  otl_cursor::direct_exec
   (
    db,
    "drop table test_tab",
    otl_exception::disabled // disable OTL exceptions
   ); // drop table

  otl_cursor::direct_exec
   (
    db,
    "create table test_tab(f1 number, f2 raw(700))"
    );  // create table

  insert(); // insert records into table
  select(); // select records from table

 }

 catch(otl_exception& p){ // intercept OTL exceptions
  cerr<<p.msg<<endl; // print out error message
  cerr<<p.stm_text<<endl; // print out SQL that caused the error
  cerr<<p.var_info<<endl; // print out the variable that caused the error
 }

 db.logoff(); // disconnect from the database

 return 0;

}
