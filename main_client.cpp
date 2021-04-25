#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <exception>
using boost::asio::ip::tcp;
boost::asio::io_service io_service;
boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 1234);

size_t read_complete(char* buff, const boost::system::error_code & error, size_t bytes)
{
  if(error)
  {
	return 0;
  }
  else
  {
	bool found = std::find(buff, buff+bytes,'\n') < buff+bytes;
	return found ? 0 : 1;
  }
}

void client_send()
{

  while(true)
  {
	  std::string inputMessage;
	  std::getline(std::cin,inputMessage);
	  inputMessage +="\n";
	  boost::asio::ip::tcp::socket sock(io_service);
	  sock.connect(ep);
	  sock.write_some(boost::asio::buffer(inputMessage));
	  char buff[1024];
	  int bytes = read(sock, boost::asio::buffer(buff),boost::bind(read_complete, buff,_1,_2));
	  std::string outputMessage(buff,bytes -1);
	  std::cout<<outputMessage<<std::endl;
	  sock.close();
	}

  }


int main()
{
  client_send();
  return 0;
}
