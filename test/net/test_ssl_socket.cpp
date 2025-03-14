#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>

#include <arpa/inet.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFAULT_BUF_SIZE 64

void handle_error(const char *file, int lineno, const char *msg) {
    fprintf(stderr, "** %s:%i %s\n", file, lineno, msg);
    ERR_print_errors_fp(stderr);
    exit(1);
}

#define int_error(msg) handle_error(__FILE__, __LINE__, msg)

void die(const char *msg) {
    perror(msg);
    exit(1);
}

void print_unencrypted_data(char *buf, size_t len) {
    printf("%.*s", (int)len, buf);
}

/* Global SSL context */
SSL_CTX *ctx;

/* This structure manages the data and functions associated with a SSL/TLS
   socket.  A single instance of this struct is used through out the example
   because the program only allow a single connection at any time. The struct 
   stores the file descriptor, the SSL objects, and data which is waiting to 
   be either written to socket or encrypted. */
struct ssl_client
{
    int fd;

    SSL *ssl;

    BIO *rbio; /* SSL reads from, we write to. */
    BIO *wbio; /* SSL writes to, we read from. */

    /* Bytes waiting to be written to socket. This is data that has been generated
     * by the SSL object, either due to encryption of user input, or, writes
     * requires due to peer-requested SSL renegotiation. */
    char* write_buf;
    size_t write_len;

    /* Bytes waiting to be fed into the SSL object for encryption. */
    char* encrypt_buf;
    size_t encrypt_len;

    /* Method to invoke when unencrypted bytes are available. */
    void (*io_on_read)(char *buf, size_t len);
} client;

void ssl_client_init(struct ssl_client *p)
{
    memset(p, 0, sizeof(struct ssl_client));

    p->rbio = BIO_new(BIO_s_mem());
    p->wbio = BIO_new(BIO_s_mem());

    p->ssl = SSL_new(ctx);

    SSL_set_accept_state(p->ssl); /* sets ssl to work in server mode. */
    SSL_set_bio(p->ssl, p->rbio, p->wbio);

    p->io_on_read = print_unencrypted_data;
}

void ssl_client_cleanup(struct ssl_client *p)
{
    SSL_free(p->ssl);   /* free the SSL object and its BIO's */
    free(p->write_buf);
    free(p->encrypt_buf);
}

int ssl_client_want_write(struct ssl_client *cp) {
    return (cp->write_len>0);
}

/* Obtain the return value of an SSL operation and convert into a simplified
 * error code, which is easier to examine for failure. */
enum sslstatus { SSLSTATUS_OK, SSLSTATUS_WANT_IO, SSLSTATUS_FAIL};

static enum sslstatus get_sslstatus(SSL* ssl, int n)
{
    switch (SSL_get_error(ssl, n))
    {
        case SSL_ERROR_NONE:
            return SSLSTATUS_OK;
        case SSL_ERROR_WANT_WRITE:
        case SSL_ERROR_WANT_READ:
            return SSLSTATUS_WANT_IO;
        case SSL_ERROR_ZERO_RETURN:
        case SSL_ERROR_SYSCALL:
        default:
            return SSLSTATUS_FAIL;
    }
}

/* Handle request to send unencrypted data to the SSL.  All we do here is just
 * queue the data into the encrypt_buf for later processing by the SSL
 * object. */
void send_unencrypted_bytes(const char *buf, size_t len)
{
    client.encrypt_buf = (char*)realloc(client.encrypt_buf, client.encrypt_len + len);
    memcpy(client.encrypt_buf+client.encrypt_len, buf, len);
    client.encrypt_len += len;
}

/* Queue encrypted bytes for socket write. Should only be used when the SSL
 * object has requested a write operation. */
void queue_encrypted_bytes(const char *buf, size_t len)
{
    client.write_buf = (char*)realloc(client.write_buf, client.write_len + len);
    memcpy(client.write_buf+client.write_len, buf, len);
    client.write_len += len;
}

/* Process SSL bytes received from the peer. The data needs to be fed into the
   SSL object to be unencrypted.  On success returns 0, on SSL error -1. */
int on_read_cb(char* src, size_t len)
{
    char buf[DEFAULT_BUF_SIZE]; /* used for copying bytes out of SSL/BIO */
    enum sslstatus status;
    int n;

    while (len > 0) {
        n = BIO_write(client.rbio, src, len);

        if (n<=0)
            return -1; /* if BIO write fails, assume unrecoverable */

        src += n;
        len -= n;

        if (!SSL_is_init_finished(client.ssl)) {
            n = SSL_accept(client.ssl);
            status = get_sslstatus(client.ssl, n);

            /* Did SSL request to write bytes? */
            if (status == SSLSTATUS_WANT_IO)
                do {
                    n = BIO_read(client.wbio, buf, sizeof(buf));
                    if (n > 0)
                        queue_encrypted_bytes(buf, n);
                    else if (!BIO_should_retry(client.wbio))
                        return -1;
                } while (n>0);

            if (status == SSLSTATUS_FAIL)
                return -1;

            if (!SSL_is_init_finished(client.ssl))
                return 0;
        }

        /* The encrypted data is now in the input bio so now we can perform actual
         * read of unencrypted data. */

        do {
            n = SSL_read(client.ssl, buf, sizeof(buf));
            if (n > 0)
                client.io_on_read(buf, (size_t)n);
        } while (n > 0);

        status = get_sslstatus(client.ssl, n);

        /* Did SSL request to write bytes? This can happen if peer has requested SSL
         * renegotiation. */
        if (status == SSLSTATUS_WANT_IO)
            do {
                n = BIO_read(client.wbio, buf, sizeof(buf));
                if (n > 0)
                    queue_encrypted_bytes(buf, n);
                else if (!BIO_should_retry(client.wbio))
                    return -1;
            } while (n>0);

        if (status == SSLSTATUS_FAIL)
            return -1;
    }

    return 0;
}

/* Process outbound unencrypted data that are waiting to be encrypted.  The
 * waiting data resides in encrypt_buf.  It needs to be passed into the SSL
 * object for encryption, which in turn generates the encrypted bytes that then
 * will be queued for later socket write. */
int do_encrypt()
{
    char buf[DEFAULT_BUF_SIZE];
    enum sslstatus status;

    if (!SSL_is_init_finished(client.ssl))
        return 0;

    while (client.encrypt_len>0) {
        int n = SSL_write(client.ssl, client.encrypt_buf, client.encrypt_len);
        status = get_sslstatus(client.ssl, n);

        if (n>0) {
            /* consume the waiting bytes that have been used by SSL */
            if ((size_t)n<client.encrypt_len)
                memmove(client.encrypt_buf, client.encrypt_buf+n, client.encrypt_len-n);
            client.encrypt_len -= n;
            client.encrypt_buf = (char*)realloc(client.encrypt_buf, client.encrypt_len);

            /* take the output of the SSL object and queue it for socket write */
            do {
                n = BIO_read(client.wbio, buf, sizeof(buf));
                if (n > 0)
                    queue_encrypted_bytes(buf, n);
                else if (!BIO_should_retry(client.wbio))
                    return -1;
            } while (n>0);
        }

        if (status == SSLSTATUS_FAIL)
            return -1;

        if (n==0)
            break;
    }
    return 0;
}

/* Read bytes from stdin and queue for later encryption. */
void do_stdin_read()
{
    char buf[DEFAULT_BUF_SIZE];
    ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
    if (n>0)
        send_unencrypted_bytes(buf, (size_t)n);
}

/* Read encrypted bytes from socket. */
int do_sock_read()
{
    char buf[DEFAULT_BUF_SIZE];
    ssize_t n = read(client.fd, buf, sizeof(buf));
    if (n>0)
        return on_read_cb(buf, (size_t)n);
    else
        return -1;
}

/* Write encrypted bytes to the socket. */
int do_sock_write()
{
    ssize_t n = write(client.fd, client.write_buf, client.write_len);
    if (n>0) {
        if ((size_t)n<client.write_len)
            memmove(client.write_buf, client.write_buf+n, client.write_len-n);
        client.write_len -= n;
        client.write_buf = (char*)realloc(client.write_buf, client.write_len);
        return 0;
    }
    else
        return -1;
}

void ssl_init() {
    printf("initialising SSL\n");

    /* SSL library initialisation */
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();

    /* create the SSL server context */
    ctx = SSL_CTX_new(SSLv23_server_method());
    if (!ctx)
        die("SSL_CTX_new()");

    /* Load certificate and private key files, and check consistency  */
    int err;
    err = SSL_CTX_use_certificate_file(ctx, "/root/server.cert",  SSL_FILETYPE_PEM);
    if (err != 1)
        int_error("SSL_CTX_use_certificate_file failed");
    else
        printf("certificate file loaded ok\n");

    /* Indicate the key file to be used */
    err = SSL_CTX_use_PrivateKey_file(ctx, "/root/server.key", SSL_FILETYPE_PEM);
    if (err != 1)
        int_error("SSL_CTX_use_PrivateKey_file failed");
    else
        printf("private-key file loaded ok\n");

    /* Make sure the key and certificate file match. */
    if (SSL_CTX_check_private_key(ctx) != 1)
        int_error("SSL_CTX_check_private_key failed");
    else
        printf("private key verified ok\n");

    /* Recommended to avoid SSLv2 & SSLv3 */
    SSL_CTX_set_options(ctx, SSL_OP_ALL|SSL_OP_NO_SSLv2|SSL_OP_NO_SSLv3);
}

int main(int argc, char **argv)
{
    char str[INET_ADDRSTRLEN];
    int port = (argc>1)? atoi(argv[1]):55555;

    int servfd = socket(AF_INET, SOCK_STREAM, 0);
    if (servfd < 0)
        die("socket()");

    int enable = 1;
    if (setsockopt(servfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0)
        die("setsockopt(SO_REUSEADDR)");

    /* Specify socket address */
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(servfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        die("bind()");

    if (listen(servfd, 128) < 0)
        die("listen()");

    int clientfd;
    struct sockaddr_in peeraddr;
    socklen_t peeraddr_len = sizeof(peeraddr);

    struct pollfd fdset[2];
    memset(&fdset, 0, sizeof(fdset));

    fdset[0].fd = STDIN_FILENO;
    fdset[0].events = POLLIN;

    ssl_init();

    while (1) {
        printf("waiting for next connection on port %d\n", port);

        clientfd = accept(servfd, (struct sockaddr *)&peeraddr, &peeraddr_len);
        if (clientfd < 0)
            die("accept()");

        ssl_client_init(&client);
        client.fd = clientfd;

        inet_ntop(peeraddr.sin_family, &peeraddr.sin_addr, str, INET_ADDRSTRLEN);
        printf("new connection from %s:%d\n", str, ntohs(peeraddr.sin_port));

        fdset[1].fd = clientfd;

        /* event loop */

        fdset[1].events = POLLERR | POLLHUP | POLLNVAL | POLLIN;
#ifdef POLLRDHUP
        fdset[1].events |= POLLRDHUP;
#endif

        while (1) {
            fdset[1].events &= ~POLLOUT;
            fdset[1].events |= (ssl_client_want_write(&client)? POLLOUT : 0);

            int nready = poll(&fdset[0], 2, -1);

            if (nready == 0)
                continue; /* no fd ready */

            int revents = fdset[1].revents;
            if (revents & POLLIN)
                if (do_sock_read() == -1)
                    break;
            if (revents & POLLOUT)
                if (do_sock_write() == -1)
                    break;
            if (revents & (POLLERR | POLLHUP | POLLNVAL))
                break;

#ifdef POLLRDHUP
            if (revents & POLLRDHUP)
                break;
#endif

            if (fdset[0].revents & POLLIN)
                do_stdin_read();

            if (client.encrypt_len>0)
                do_encrypt();
        }

        close(fdset[1].fd);
        ssl_client_cleanup(&client);
    }

    return 0;
}