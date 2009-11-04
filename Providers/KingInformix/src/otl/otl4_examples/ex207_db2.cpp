#include <iostream>
using namespace std;
#include <stdio.h>

const int BASE_EXCEPTION=1; // "Base exception" class Id.
const int DB_BASE_EXCEPTION=2; // "DB Base exception" class Id.

// Let's assume that the following class is the base of 
// the system exception class hierarchy
class my_base_exception{
public:

  my_base_exception(){}
  virtual ~my_base_exception(){}

  virtual int getType(void) const
  {
    return BASE_EXCEPTION;
  }

  virtual int getErrorCode(void) const
  {
    return 0;
  }

  virtual const unsigned char* getErrorMessage(void) const
  {
    return reinterpret_cast<const unsigned char*>("");
  }


};

#define OTL_DB2_CLI // Compile OTL 4.0/DB2-CLI

// This #define connects the otl_exception class to the exception
// class hierarchy. 
#define OTL_EXCEPTION_DERIVED_FROM my_base_exception

// This #define defines a set of new members in the otl_exception
// class
#define OTL_EXCEPTION_HAS_MEMBERS                               \
  virtual int getType(void) const                               \
  {                                                             \
    return DB_BASE_EXCEPTION;                                   \
  }                                                             \
                                                                \
  virtual int getErrorCode(void) const                          \
  {                                                             \
    return this->code;                                          \
  }                                                             \
                                                                \
  virtual const unsigned char*  getErrorMessage(void) const     \
  {                                                             \
    return this->msg;                                           \
  }

#include <otlv4.h> // include the OTL 4.0 header file

otl_connect db; // connect object

void insert()
// insert rows into table
{ 
 otl_stream o(50, // buffer size
              "insert into test_tab values(:f1<float>,:f2<char[31]>)", 
                 // SQL statement
              db // connect object
             );
 char tmp[32];

 for(int i=1;i<=100;++i){
  sprintf(tmp,"Name%d",i);
  o<<static_cast<float>(i)<<tmp;
 }
}

void select()
{ 
 otl_stream i(50, // buffer size
              "select * from test_tab where f1>=:f11<int> and f1<=:f12<int>*2",
                 // SELECT statement
              db // connect object
             ); 
   // create select stream
 
 float f1;
 char f2[31];

 i<<8<<8; // assigning :f12 = 8, :f12 = 8
   // SELECT automatically executes when all input variables are
   // assigned. First portion of output rows is fetched to the buffer

 while(!i.eof()){ // while not end-of-data
  i>>f2>>f1; // a typo: f2 instead of f1
  cout<<"f1="<<f1<<", f2="<<f2<<endl;
 }

 i<<4<<4;
   // SELECT automatically executes when all input variables are

   // assigned. First portion of output rows is fetched to the buffer

 while(!i.eof()){ // while not end-of-data
  i>>f1>>f2;
  cout<<"f1="<<f1<<", f2="<<f2<<endl;
 }

}

int main()
{
 otl_connect::otl_initialize(); // initialize DB2-CLI environment
 try{

  db.rlogon("scott/tiger@db2sql"); // connect to DB2

  otl_cursor::direct_exec
   (
    db,
    "drop table test_tab",
    otl_exception::disabled // disable OTL exceptions
   ); // drop table

  otl_cursor::direct_exec
   (
    db,
    "create table test_tab(f1 int, f2 varchar(30))"
    );  // create table

  insert(); // insert records into table
  select(); // select records from table

 }
 catch(my_base_exception& ex){ // intercept the base exception
   if(ex.getType()==DB_BASE_EXCEPTION){ 
     // "database exception" is recognized
     // here, the otl_exception can be analyzed, or simply printed out
     cerr<<"Database exception:"<<endl;
     cerr<<"Code: "<<ex.getErrorCode()<<endl; // print out error code
     cerr<<"Message: "
         <<reinterpret_cast<const char*>(ex.getErrorMessage())
         <<endl; // print out the error message
   }else{
     // otherwise, do something else
     cerr<<"Base exception was caught..."<<endl;
   }
 }

 db.logoff(); // disconnect from DB2

 return 0;

}