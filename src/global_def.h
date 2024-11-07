#include <cstdint>
#include <sys/socket.h> // 套接字 API，例如 socket(), bind(), connect(), listen(), accept() 等
#include <climits>  // 包含系统相关的限制信息，比如 `CHAR_BIT`
#include <barrier>
#include <sys/ucontext.h>
#include "functional"
#include <chrono>
#include <memory> // 引入智能指针

typedef std::function<void()> Task;
using Timestamp = std::chrono::steady_clock::time_point;

typedef int FileDescriptor;
typedef struct sockaddr_in  SockAddrV4;
typedef struct sockaddr SockAddr;
typedef struct epoll_event EpollEvent;
typedef socklen_t SockLen;
typedef pthread_t PThread;
typedef pthread_mutex_t PMutex;
typedef ucontext_t  UContext;

typedef pid_t Tid;
typedef pid_t Pid;


typedef void (*SignalHandler)(int);
typedef struct sigaction SignalAction;


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



typedef bool Bool;

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


