#include "condition_variable"
#include "mutex"

#include "queue"
template<class T>
class Channel{
    std::queue<T> data_;
    std::mutex lock_;


    bool read(T & t){

    }

    bool write(T &t){

    }

    void close(){

    }
};

int main(){

}