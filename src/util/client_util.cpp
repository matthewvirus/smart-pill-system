#include "client_util.h"

String generate_client_id()
{
    String client_id = CLIENT_ID;
    client_id += (random(0xffff), HEX);
    return client_id;
}