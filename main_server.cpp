#define BOOST_ALL_NO_LIB
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <thread>
#include "sqlite-autoconf-3340100/sqlite3.h"


using boost::asio::ip::tcp;
boost::asio::io_service io_service;
std::string SQLReturn;
static int callback(void* data, int argc, char** argv, char** azColName)
{
  for(int i=0;i<argc;i++)
  {
	SQLReturn = *azColName;
	SQLReturn +=" ";
	SQLReturn.push_back(*argv[i]);
  }
 return 0;
}
std::string answerFromBD(std::string& query, sqlite3* db)
{
  std::string returnString;
  std::string data("CALLBACK FUNCTION");
  std::cout<<query<<std::endl;
  int i = sqlite3_exec(db,query.c_str(),callback,(void*)data.c_str(),NULL);
  if(i!= SQLITE_OK)
  {
	std::cout<<"error";
	returnString = "ERROR\n";
  }
  {
	std::cout<<SQLReturn<<std::endl;
	returnString = SQLReturn +"\n";
	 SQLReturn.clear();
  }
  return returnString;
}
size_t read_comlete(char* buff, const boost::system::error_code& error, size_t bytes)
{
  if(error)
  {
	return 0;
  }
  else
  {
	bool found = std::find(buff, buff + bytes, '\n') < buff+bytes;
	return found ? 0 : 1;
  }
}

void adminInput(sqlite3* db)
{

  while(true)
  {
	std::string SQL;
	std::cin>>SQL;
	sqlite3_exec(db,SQL.c_str(),0,0,0);
  }
}

void connections(sqlite3* db)
{
  boost::asio::ip::tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));
  char buff[1024];
  while(true)
  {
	ip:tcp::socket sock(io_service);
	acceptor.accept(sock);
	int bytes = read(sock, boost::asio::buffer(buff), boost::bind(read_comlete,buff,_1,_2));
	std::string msg(buff,bytes);
	std::string answer = answerFromBD(msg,db);
	sock.write_some(boost::asio::buffer(answer));
	sock.close();
  }
}

int main()
{
  std::cout<<"server started"<<std::endl;
  sqlite3 *db=0;
  char*err = 0;
  char* SQL = "CREATE TABLE faculty(number_of_faculty INTEGER PRIMAL KEY);";
  if(sqlite3_open("database.bdlite",&db))
  {
	std::cout<<"Error to open/creating DB\n";
  }
  else
  {
	if(sqlite3_exec(db,SQL,0,0,&err))
	{
	  std::thread t1(adminInput,db);
	  connections(db);
	  t1.join();
	  sqlite3_close(db);
	}
  }
  std::cout<<"server shutdown"<<std::endl;
  return 0;
}
