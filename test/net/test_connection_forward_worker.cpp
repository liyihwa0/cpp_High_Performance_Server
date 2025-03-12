#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "src/net/socket.h"
using namespace wa;
void testConnectionForwardWorker() {
    SP<ConnectionSocket> inSocket(new ConnectionSocket());

    inSocket->addDataToRecvBuffer("Hello, ");
    inSocket->addDataToRecvBuffer("world!");

    ConnectionForwardWorker forwardWorker(inSocket, outSocket, 1024);

    // Step 4: Run the forward worker in a separate thread
    std::thread workerThread([&forwardWorker]() {
        forwardWorker.run();
    });

    // Step 5: Allow time for the worker to process and transfer data
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Step 6: Stop the worker thread (clean up)
    workerThread.join();

    // Step 7: Check the output by verifying that data was "sent"
    // In the test, we're printing the data to console, so we would manually check if the output was correct.
}

int main() {
    // Run the test
    testConnectionForwardWorker();
    return 0;
}
