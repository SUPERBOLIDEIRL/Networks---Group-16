#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

    class myClient
    {
        private:
                int ClientSocket;
                int portNum;
        public:
		 myClient(int port_num)
                {
                        int clt_socket = setClientSocket();
			cout << clt_socket << endl; //testing
                        int prt_num = setPortNumber(port_num);
			cout << prt_num << endl; //testing
                }

		myClient()
		{
			ClientSocket = 0;
			portNum = 0;
		}

                int setClientSocket()
                {
                        ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

                        if (ClientSocket == -1)
                        {
                                //throw exception or error
                                close(ClientSocket);
			        return -1;
                        }
                        else
                                return ClientSocket;
                }

		  int setPortNumber(int port_num)
                {
                        portNum = port_num;
                        return portNum;
                }

		string Message(const char* message)
		{
			if (strlen(message) > 1024)
				return ""; //throw exception or error
			else
				return message;
		}

		int ConnectWithServer()
		{
			sockaddr_in SvrAddr;
			SvrAddr.sin_family = AF_INET;
			SvrAddr.sin_port = htons(this->portNum);
			SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //change IP address

			int Connection = connect(this->ClientSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr));
			if (Connection == -1)
			{
				close(this->ClientSocket);
				cout << "Terrible" << endl;
				return -1; //throw exception or error
			}
			else
			{
				cout << Connection << endl;//use the positive connection value when confirming connection
								   // in the GUI class
				return Connection;
			}
		}

	};

	class Account: public myClient
	{
	  private:
		 string username;
		 string password;
	  public:
		Account(const char* name, const char* password, int x = 0) : myClient(x)
		{
			string myUsername = setUsername(name);
			cout << myUsername << endl; //testing
			string myPassword = setPassword(password);
			cout << myPassword << endl; //testing
		}

		Account()
		{
			username = "Enter your name here....";
			password = "Enter your password here...";
		}

		string setUsername(const char* name)
		{
			username = name;
			return username;
		}

		string setPassword(const char* Password)
		{
			password = Password;
			return password;
		}

		void sendLoginsToServer()
		{
			
		}
	};


int main(void)
{
	myClient Clt1(30000);
	string message = Clt1.Message("I actually love my morning hot chocolate\nIt keeps me warm and starts my day on the right track\n");
	cout << message << endl;

	int connecting = Clt1.ConnectWithServer();
	cout << connecting << endl;
	
	Account A1("Betty", "1234");


	return 0;
}

