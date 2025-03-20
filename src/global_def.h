#include <cstdint>
#include <sys/socket.h> // 套接字 API，例如 socket(), bind(), connect(), listen(), accept() 等
#include <climits>  // 包含系统相关的限制信息，比如 `CHAR_BIT`
#include <sys/ucontext.h>
#include "functional"
#include <chrono>
#include <memory> // 引入智能指针
#include <vector>
#include <unordered_map>
#include "fstream"
typedef std::function<void()> Task;
typedef std::ofstream OFStream;
template<typename T>
using Vector = std::vector<T>; // 使用 using 创建别名
template<typename K,typename V>
using HashMap=std::unordered_map<K,V>;

#define NULLPTR nullptr
typedef std::exception Exception;
typedef int FileDescriptor;
typedef struct sockaddr_in  SockAddrV4;
typedef struct sockaddr SockAddr;
typedef struct epoll_event EpollEvent;
typedef socklen_t SockLen;
typedef pthread_t PThread;
typedef pthread_mutex_t PMutex;
typedef ucontext_t  UContext;



typedef void (*SignalHandler)(int);
typedef struct sigaction SignalAction;

typedef std::string String;
typedef std::ostream OStream;

typedef void Void;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;


typedef size_t Size;


typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;

typedef float F32;
typedef double F64;
typedef U32 RWEvent;



typedef bool Bool;
typedef char Char;

#define UNDEFINED_STRING "undefined"


#define TRUE true
#define FALSE false


#define SizeOf(a) sizeof(a)


#define TARGET_PLATFORM_BYTES_64
// #define TARGET_PLATFORM_BYTES_32

#ifdef TARGET_PLATFORM_BYTES_64  
    typedef I64 Int;      
    typedef U64 UInt;
#elif TARGET_PLATFORM_BYTES_32  
    typedef I32 Int;     
    typedef U32 UInt;
#endif

typedef Int ID;

