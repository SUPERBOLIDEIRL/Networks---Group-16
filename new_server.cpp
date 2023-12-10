#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#define PORT 56000

using namespace std;


vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    istringstream tokenStream(str);
    string token;
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

struct Post {
    int id;
    string author;
    string topic;
    string content;
};

class DiscussionBoard {
private:
    const string postsFile = "posts.txt";
    int nextPostId = 1;

    void savePostToFile(const Post& post) {
        ofstream file(postsFile, ios::app);
        if (file.is_open()) {
            file << post.id << "|" << post.author << "|" << post.topic << "|" << post.content << "\n";
            file.close();
        }
    }

public:
    string processRequest(const string& request) {
        auto tokens = split(request, '|');
        if (tokens.empty()) {
            return "ERROR|Invalid request format";
        }

        if (tokens[0] == "POST_SINGLE") {
            if (tokens.size() != 4) return "ERROR|Invalid POST_SINGLE format";
            Post post = {nextPostId++, tokens[1], tokens[2], tokens[3]};
            savePostToFile(post);
            return "ACK_SINGLE|" + to_string(post.id);
        } else if (tokens[0] == "GET_ALL_POSTS") {
            ifstream file(postsFile);
            string line;
            string response = "ALL_POSTS";
            while (getline(file, line)) {
                response += "|" + line;
            }
            file.close();
            return response;
        } else {
            return "ERROR|Unsupported request type";
        }
    }
};

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    //address.sin_addr.s_addr = inet_addr("192.168.1.100");  // Replace with your server's IP


    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    cout << "Starting to listen on port " << PORT << "..." << endl;

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout << "Server is listening for connections..." << endl;

    DiscussionBoard board;

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *) &addrlen)) < 0) {
            perror("accept");
            continue;
        }

        char buffer[4096] = {0};
        int valread = recv(new_socket, buffer, 4095, 0);
        //int valread = read(new_socket, buffer, 4096);
        if (valread > 0) {
            string response = board.processRequest(string(buffer));
            send(new_socket, response.c_str(), response.length(), 0);
        }

        

        close(new_socket);
    }

    close(server_fd);
    return 0;
}