#include <arpa/inet.h>
#include <limits>
#include <algorithm>
#include <cctype>
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


		void PostMessage(const char* message)
		{
			if (strlen(message) > 1024)
				cout << "Message is too long\n"; //throw exception or error
			else
			{
				if (send(this->ClientSocket, message, strlen(message), 0) == -1)
				{
					cout << "Message failed to send\n";
				}
				else
				{
					char ResponseMessage[256] = {0};
                                	if (recv(this->ClientSocket, ResponseMessage, 255, 0) == -1)
                                	{
                                         	close(this->ClientSocket);
                                         	cout << "Message not received";
                                	}
                                	else
                                	{        cout << ResponseMessage << endl;
                                	}

				}
			}
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


		void ConnectWithServer()
		{	sockaddr_in SvrAddr;
			SvrAddr.sin_family = AF_INET;
			SvrAddr.sin_port = htons(this->portNum);
			SvrAddr.sin_addr.s_addr = inet_addr("172.16.5.12");

			int Connection = connect(this->ClientSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr));
			if (Connection == -1)
			{
				close(this->ClientSocket);
				cout << "Connection with server unsuccessful\n"; //throw exception or error
			}
			else
			{
				cout <<"Connection Successful" << endl;//use the positive connection value when confirming connection
								   // in the GUI class
			}
		}

	};

void displayMainMenu()
{
system("cls");
printf("\n\n\n\n\n\n\n");
printf("                               ********************************************************\n");
printf("                               *                                                      *\n");
printf("                               *            Welcome to discussion board               *\n");
printf("                               *                                                      *\n");
printf("                               *                  Select option:                      *\n");
printf("                               *                                                      *\n");
printf("                               ********************************************************\n");
printf("                               *                                                      *\n");
printf("                               *    1.  Write a single post to discussion board       *\n");
printf("                               *    2.  Write multiple posts to discussion board      *\n");
printf("                               *    3.  Display all posts from discussion board       *\n");
printf("                               *    4.  Display list of filtered posts                *\n");
printf("                               *                                                      *\n");
printf("                               *    0.  EXIT                                          *\n");
printf("                               *                                                      *\n");
printf("                               ********************************************************\n");
}


void MainMenu(myClient& Clt1) {
    int end = 0;
    int choice;
    string author, topic, message;
    const char* newMessage;

    // Allocate memory for temp
    char temp[1024] = {0}; // Ensure enough space for the concatenated message

    while (end == 0) {
	displayMainMenu();
        switch (choice) {
            case 1:
                cout << "Enter author: ";
                cin >> author;

                cout << "Enter topic: ";
                cin >> topic;

                cout << "Enter post: ";
                cin.ignore(); // Consume the newline character left in the buffer
                getline(cin, message);

                // Construct the message in the required format
                newMessage = ("POST_SINGLE|" + author + "|" + topic + "|" + message).c_str();
                strcpy(temp, newMessage);

                Clt1.PostMessage(temp);
                break;
            case 2:
                // Multiple post logic here
                break;
            case 3:
                // Get posts from the server and print;
                break;
            case 4:
                // Introduce filter
                break;
            case 0:
                end = 1;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
}

int main(void)
{
	char name[100] = "Betty";
	myClient Clt1(56000, name, "1234");
        Clt1.ConnectWithServer();
	//Clt1.PostMessage("POST_SINGLE|anonymous|Topic1|My name is sunshine but i'm in love with the rain. Ironic right?");
	return 0;
}
