#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <event.h>
#include <evhttp.h>

void generic_request_handler(struct evhttp_request *req, void *arg)
{
    struct evbuffer *returnbuffer = evbuffer_new();

    evbuffer_add_printf(returnbuffer, "Thanks for the request!");
    evhttp_send_reply(req, HTTP_OK, "Client", returnbuffer);
    evbuffer_free(returnbuffer);
    return;
}

int main(int argc, char **argv)
{
    short          http_port = 8082;
    const char     *http_addr = "127.0.0.1";

    struct event_base * base = event_base_new();

    struct evhttp * http_server = evhttp_new(base);
    if(!http_server)
    {
        printf("create http server fails\n");
        return -1;
    }

    int ret = evhttp_bind_socket(http_server,http_addr,http_port);
    if(ret!=0)
    {
        printf("bind fails\n");
        return -1;
    }

    evhttp_set_gencb(http_server, generic_request_handler, NULL);

    //fprintf(stderr, "Server started on port %d\n", http_port);
    event_dispatch();

    return(0);
}
