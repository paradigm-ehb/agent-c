#ifndef NET_H
#define NET_H

enum TransportLayerType
{
    TRANSPORT_TCP,
    TRANSPORT_UDP,
    TRANSPORT_SCTP,
};


struct Port
{
    char name[BUFFER_SIZE_SMALL];

    int                     port;
    enum TransportLayerType type;
    char                    alias[BUFFER_SIZE_DEFAULT];
    char                    comment[BUFFER_SIZE_DEFAULT];
};

#endif /*NET_H*/