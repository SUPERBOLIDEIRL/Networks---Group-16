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
		const char* username;
                const char* password;

        public:
		 myClient(int port_num, const char* name, const char* password )
                {
                        int clt_socket = setClientSocket();
			cout << clt_socket << endl; //testing
                        int prt_num = setPortNumber(port_num);
			cout << prt_num << endl; //testing

			const char* myUsername = setUsername(name);
                        cout << myUsername << endl; //testing
                        const char* myPassword = setPassword(password);
                        cout << myPassword << endl; //testing

                }

		myClient()
		{
			ClientSocket = 0;
			portNum = 0;
			username = "Enter your name here....";
                        password = "Enter your password here...";

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

		const char* setUsername(const char* name)
                {
                        username = name;
                        return username;
                }

                const char* setPassword(const char* Password)
                {
                        password = Password;
                        return password;
                }


		const char* Message(const char* message)
		{
			if (strlen(message) > 1024)
				return ""; //throw exception or error
			else
				return message;
		}



		int sendLoginsToServer()
                {
                        //application layer protocol
			char* temp;
			strcpy (temp, this->username);
			const char* LoginsInfo = strncat(temp, this->password, 1024);
			if (send(this->ClientSocket, LoginsInfo, strlen(LoginsInfo), 0) == -1)
			{
				close(this->ClientSocket);
				cout << "Falied to send login info" << endl;
				return -1;
			}
			else
			{
				char ConfirmationMessage[256] = {0};
                       	        if (recv(this->ClientSocket, ConfirmationMessage, 255, 0) == -1)
                       	        {
                               		 close(this->ClientSocket);
                               		 cout << "Login Unsuccessful" << endl;
                               		 return -1;
                       		}
                       		else
                                {	 cout << ConfirmationMessage << endl;
					 return 0;
				}

			}
                }

		int ConnectWithServer()
		{	sockaddr_in SvrAddr;
			SvrAddr.sin_family = AF_INET;
			SvrAddr.sin_port = htons(this->portNum);
			SvrAddr.sin_addr.s_addr = inet_addr("172.16.5.12");

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



int main(void)
{
	char name[100] = "Betty";
	myClient Clt1(30000, name, "1234");
	const char* message = Clt1.Message("I actually love my morning hot chocolate\nIt keeps me warm and starts my day on the right track\n");
	cout << message << endl;

	int Logins = Clt1.sendLoginsToServer();


	return 0;
}
