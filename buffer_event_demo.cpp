#include <event2/event.h>
#include <event2/bufferevent.h>
#include <sys/socket.h>
#include <string.h>

#include <event2/buffer.h>
#include <event2/util.h>

#define MAX_BUF 1024
struct Client
{
    char writeBuf[MAX_BUF];
    char readBuf[MAX_BUF];
};

void readcb(struct bufferevent *bev, void *ptr)
{
    char buf[1024];
    Client* cl = (Client*)ptr;
    int n;
    struct evbuffer *input = bufferevent_get_input(bev);
    while ((n = evbuffer_remove(input, cl->readBuf, sizeof(cl->readBuf))) > 0) {
        fwrite(cl->readBuf, 1, n, stdout);
    }

    struct evbuffer* evreturn = evbuffer_new();
    evbuffer_add_printf(evreturn, "client has got it: %s\n", cl->readBuf);
    //write buffer
    bufferevent_write_buffer(bev, evreturn);
    evbuffer_free(evreturn);
}

void writecb(struct bufferevent *bev, void *ptr)
{
    Client* cl = (Client*)ptr;
    if(cl->writeBuf[0] == '\0') {
        //already sent
        bufferevent_disable(bev, EV_WRITE);
        return;
    }

    struct evbuffer *output = bufferevent_get_output(bev);
    //we just set string to output buffer, do need to call bufferevent_write_buffer
    evbuffer_add(output, cl->writeBuf, strlen(cl->writeBuf)+1);
    memset(cl->writeBuf, 0, MAX_BUF);
    //bufferevent_write_buffer(bev, output);
}

void eventcb(struct bufferevent *bev, short events, void *ptr)
{
    if (events & BEV_EVENT_CONNECTED) {
         /* We're connected to 127.0.0.1:8080.   Ordinarily we'd do
            something here, like start reading or writing. */
        printf("connected!\n");
    } else if (events & BEV_EVENT_ERROR) {
         /* An error occured while connecting. */
         printf("Closing\n");
    }
}

/*
server
echo "abc" | nc -l -p 8080
*/


int main(void)
{
    struct event_base *base;
    struct bufferevent *bev;
    struct sockaddr_in sin;

    base = event_base_new();

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    //sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_addr.s_addr = htonl(0x7f000001); /* 127.0.0.1 */
    sin.sin_port = htons(8080); /* Port 8080 */

    bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);

    Client* cl = new Client();
    memset(cl->readBuf, 0, MAX_BUF);
    strcpy(cl->writeBuf, "I want to drink coke!\n");

    bufferevent_setcb(bev, readcb, writecb, eventcb, cl);
    bufferevent_enable(bev, EV_READ|EV_WRITE);

    if (bufferevent_socket_connect(bev,
        (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        /* Error starting connection */
        bufferevent_free(bev);
        return -1;
    }

    event_base_dispatch(base);
    return 0;
}