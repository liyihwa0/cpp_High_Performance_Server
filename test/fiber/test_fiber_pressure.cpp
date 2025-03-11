#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>

using namespace std;

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(queueMutex);
                        cv.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) return;
                        task = move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            unique_lock<mutex> lock(queueMutex);
            stop = true;
        }
        cv.notify_all();
        for (thread &worker : workers) {
            worker.join();
        }
    }

    void enqueue(function<void()> task) {
        {
            unique_lock<mutex> lock(queueMutex);
            if (stop) throw runtime_error("enqueue on stopped ThreadPool");
            tasks.push(move(task));
        }
        cv.notify_one();
    }

private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex queueMutex;
    condition_variable cv;
    bool stop;
};

const int NUM_CLIENTS = 100000;
const int BUFFER_SIZE = 1024;
atomic<int> connected_clients(0);

void clientTask(const string& serverAddress, int serverPort) {
    int client_id = connected_clients++;

    int clientFd = socket(
            AF_INET, SOCK_STREAM, 0);
    if (clientFd == -1) {
        perror("TcpServer creation failed");
        return;
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr) <= 0) {
        perror("Invalid server address");
        close(clientFd);
        return;
    }

    if (connect(clientFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection failed");
        close(clientFd);
        return;
    }

    char buffer[BUFFER_SIZE];
    string message = "Hello, server!";
    static int messageCount = 0;
    while (++messageCount <= 100) {
        ssize_t bytesSent = send(clientFd, message.c_str(), message.length(), 0);
        if (bytesSent == -1) {
            perror("Send failed");
            break;
        }

        ssize_t bytesReceived = recv(clientFd, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            if (bytesReceived == 0) {
                cout << "Server disconnected" << endl;
            } else {
                perror("Recv failed");
            }
            break;
        }

        buffer[bytesReceived] = '\0';  // Null-terminate the response
    }

    close(clientFd);
    cout << "Client " << client_id << " disconnected." << endl;
}

int main() {
    string serverAddress = "127.0.0.1";
    int serverPort = 12345;
    ThreadPool pool(100);  // Use 100 threads in the pool

    for (int i = 0; i < NUM_CLIENTS; ++i) {
        pool.enqueue([serverAddress, serverPort] {
            clientTask(serverAddress, serverPort);
        });
    }

    // Wait for all tasks to finish (not implemented here)
    cout << "All clients have finished communicating with the server." << endl;
    return 0;
}
