/* This tests processing abnormal packet. */


#include   "tx_api.h"
#include   "nx_api.h"


extern void test_control_return(UINT status);

#if !defined(NX_DISABLE_IPV4)

#include   "nxd_smtp_client.h"

#define     DEMO_STACK_SIZE         2048
#define     SERVER_IPADR            IP_ADDRESS(10, 0, 0, 1)
#define     CLIENT_IPADR            IP_ADDRESS(10, 0, 0, 10)

/* Define the ThreadX and NetX object control blocks...  */

static TX_THREAD               thread_client;
static TX_THREAD               thread_server;

static NX_PACKET_POOL          client_packet_pool;
static NX_IP                   client_ip;
static NX_IP                   server_ip;


static NX_PACKET_POOL          server_packet_pool;
static NX_TCP_SOCKET           server_socket;
static NX_SMTP_CLIENT          smtp_client;

#define SUBJECT_LINE           "NetX Duo SMTP Client Demo" 
#define MAIL_BODY              ".NetX Duo SMTP client is a simple SMTP client implementation  \r\n" \
                               ".that allow embedded devices to send email to an SMTP server. \r\n" \
                               "This feature is intended to allow a device to send simple status\r\n " \
                               "reports using the most universal Internet application email.\r\n" \
                               "The rest of this narrative is merely to supply this most interesting quote: \r\n" \
                               "Perhaps the most vivid recollection of my youth is that of the local wheelmen, \r\n" \
                               "led by my father, stopping at our home to eat pone, sip mint juleps, and flog \r\n" \
                               "the field hands. This more than anything cultivated my life-long aversion \r\n"  \
                               "to bicycles. ~~ Tennessee Williams.\r\n\r\n" \
                               "Nothing compares to the simple pleasure of a bike ride. --John F. Kennedy, (surely you've heard of him?)\r\n\r\n" \
                               "Perhaps the most vivid recollection of my youth is that of being flogged \r\n" \
                               "by the local wheelmen, along with the field hands, the postman, and a \r\n" \
                               "young Tennessee Williams. This more than anything cultivated my life-long " \
                               "aversion to his plays. -Truman Capote\r\n" \
                               "When I see an adult on a bicycle, I do not despair for the future of the human race. H.G. Wells \r\n"  \
                               "during the Second World War, if the United States had retooled its \r\n" \
                               "factories for manufacturing bicycles instead of munitions, we'd be one of \r\n" \
                               "the healthiest, least oil-dependent, and most environmentally-sound \r\n" \
                               "constituents in the Nazi empire today. -Ralph Nader." 


#define PASSWORD               "testpwd"
#define RECIPIENT_ADDRESS      "recipient@domain.com"
#define LOCAL_DOMAIN           "domain.com"
#define FROM_ADDRESS           "recipient@domain.com"
#define USERNAME               FROM_ADDRESS
#define SMTP_SERVER_PORT       25


/* See the NetX Duo SMTP Client User Guide for how to set the authentication type.  */
#define CLIENT_AUTHENTICATION_TYPE NX_SMTP_CLIENT_AUTH_PLAIN

#if defined(__PRODUCT_NETXDUO__) 
static NXD_ADDRESS server_ip_address;
#else
static ULONG server_ip_address;
#endif


static UINT server_complete = NX_FALSE;

/* Define the counters used in the demo application...  */

static UINT                    status;
static ULONG                   error_counter = 0;
static ULONG                   notify_calls =  0;

/* Define thread prototypes.  */

static void    thread_0_entry(ULONG thread_input);
static void    thread_1_entry(ULONG thread_input);
extern void    _nx_ram_network_driver_1024(struct NX_IP_DRIVER_STRUCT *driver_req);
static void    receive_packet_function(NX_TCP_SOCKET *socket_ptr);

/* SMTP Tests.  */
static void    smtp_test_initialize();

/* Send SMTP server response.  */
static UINT    nx_smtp_response_packet_send(NX_TCP_SOCKET *server_socket, UINT packet_number);

extern char response_221_bye_pkt[80];
extern int response_221_bye_size;

static char invliad_response_220_greetings_pkt[] = {
0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, /* ...v..22 */
0x30, 0x2d, 0x65, 0x78, 0x70, 0x72, 0x65, 0x73, /* 0-expres */
0x73, 0x6c, 0x6f, 0x67, 0x69, 0x63, 0x2e, 0x63, /* slogic.c */
0x6f, 0x6d, 0x20, 0x45, 0x53, 0x4d, 0x54, 0x50, /* om ESMTP */
0x20, 0x4d, 0x44, 0x61, 0x65, 0x6d, 0x6f, 0x6e, /*  MDaemon */
0x20, 0x31, 0x33, 0x2e, 0x30, 0x2e, 0x33, 0x3b, /*  13.0.3; */
0x20, 0x53, 0x61, 0x74, 0x2c, 0x20, 0x32, 0x33, /*  Sat, 23 */
0x20, 0x4d, 0x61, 0x72, 0x20, 0x32, 0x30, 0x31, /*  Mar 201 */
0x33, 0x20, 0x32, 0x32, 0x3a, 0x35, 0x38, 0x3a, /* 3 22:58: */
0x31, 0x35, 0x20, 0x2d, 0x30, 0x37, 0x30, 0x30, /* 15 -0700 */
0x0d, 0x0a, 0x32, 0x32, 0x30, 0x2d, 0x41, 0x6c, /* ..220-Al */
0x6c, 0x20, 0x74, 0x72, 0x61, 0x6e, 0x73, 0x61, /* l transa */
0x63, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20, 0x61, /* ctions a */
0x6e, 0x64, 0x20, 0x49, 0x50, 0x20, 0x61, 0x64, /* nd IP ad */
0x64, 0x72, 0x65, 0x73, 0x73, 0x65, 0x73, 0x20, /* dresses  */
0x61, 0x72, 0x65, 0x20, 0x6c, 0x6f, 0x67, 0x67, /* are logg */
0x65, 0x64, 0x2e, 0x0d, 0x0a, 0x32, 0x32, 0x30, /* ed...220 */
0x2d, 0x2a, 0x0d, 0x0a, 0x32, 0x32, 0x30, 0x20, /* -*..220  */
0x55, 0x6e, 0x61, 0x75, 0x74, 0x68, 0x6f, 0x72, /* Unauthor */
0x69, 0x7a, 0x65, 0x64, 0x20, 0x72, 0x65, 0x6c, /* ized rel */
0x61, 0x79, 0x69, 0x6e, 0x67, 0x20, 0x69, 0x73, /* aying is */
0x20, 0x73, 0x74, 0x72, 0x69, 0x63, 0x74, 0x6c, /*  strictl */
0x79, 0x20, 0x70, 0x72, 0x6f, 0x68, 0x69, 0x62, /* y prohib */
0x69, 0x74, 0x65, 0x64, 0x2e, 0x0d, 0x0a        /* ited... */
};

typedef struct SMTP_TEST_STRUCT
{
    char          *smtp_test_pkt_data;
    int           smtp_test_pkt_size;
} SMTP_TEST;


static UINT msg_count = 0;

static SMTP_TEST smtp_test[9];


/* Define what the initial system looks like.  */

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_smtp_abnormal_packet_test_application_define(void *first_unused_memory)
#endif
{

CHAR    *pointer;
    
    /* Setup the working pointer.  */
    pointer =  (CHAR *) first_unused_memory;
    
    /* Create the SMTP client thread.  */
    tx_thread_create(&thread_client, "thread client", thread_0_entry, 0,  
            pointer, DEMO_STACK_SIZE, 
            4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer =  pointer + DEMO_STACK_SIZE;

    tx_thread_create(&thread_server, "thread server", thread_1_entry, 0,  
            pointer, DEMO_STACK_SIZE, 
            3, 3, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer =  pointer + DEMO_STACK_SIZE;

    /* Initialize the NetX system.  */
    nx_system_initialize();

    /* Create client packet pool.  */
    status =  nx_packet_pool_create(&client_packet_pool, "NetX Main Packet Pool", 1536, pointer, 10*1536);
    pointer = pointer + (10*1536);
        
    /* Check for pool creation error.  */
    if (status)
        return;

    /* Create server packet pool.  */
    status =  nx_packet_pool_create(&server_packet_pool, "NetX Main Packet Pool", 1536, pointer, 10*1536);
    pointer = pointer + (10*1536);
        
    /* Check for pool creation error.  */
    if (status)
        return;

    /* Create an IP instance.  */
    status = nx_ip_create(&client_ip, "SMTP CLient IP", CLIENT_IPADR, 0xFFFFFF00UL, &client_packet_pool, _nx_ram_network_driver_1024,
                    pointer, 2048, 1);
    pointer =  pointer + 2048;

    /* Create another IP instance.  */
    status += nx_ip_create(&server_ip, "SMTP Server IP", SERVER_IPADR, 0xFFFFFF00UL, &server_packet_pool, _nx_ram_network_driver_1024,
                    pointer, 2048, 1);
    pointer =  pointer + 2048;

    /* Check for IP create errors.  */
    if (status)
        error_counter++;

    /* Enable ARP and supply ARP cache memory for IP Instance 0.  */
    status =  nx_arp_enable(&client_ip, (void *) pointer, 1024);
    pointer = pointer + 1024;

    /* Enable ARP and supply ARP cache memory for IP Instance 1.  */
    status +=  nx_arp_enable(&server_ip, (void *) pointer, 1024);
    pointer = pointer + 1024;

    /* Check for ARP enable errors.  */
    if (status)
        error_counter++;

    /* Enable TCP traffic.  */
    status =  nx_tcp_enable(&client_ip);
    status += nx_tcp_enable(&server_ip);

    /* Check for TCP enable errors.  */
    if (status)
        error_counter++;


    /* The demo client username and password is the authentication 
   data used when the server attempts to authentication the client. */

#if defined(__PRODUCT_NETXDUO__) 
    server_ip_address.nxd_ip_version = NX_IP_VERSION_V4;
    server_ip_address.nxd_ip_address.v4 = SERVER_IPADR;

    status =  nxd_smtp_client_create(&smtp_client, &client_ip, &client_packet_pool,
                                     USERNAME, 
                                     PASSWORD,
                                     FROM_ADDRESS,
                                     LOCAL_DOMAIN, CLIENT_AUTHENTICATION_TYPE, 
                                     &server_ip_address, SMTP_SERVER_PORT);
#else
    server_ip_address = SERVER_IPADR;

    status =  nx_smtp_client_create(&smtp_client, &client_ip, &client_packet_pool,
                                     USERNAME, 
                                     PASSWORD,
                                     FROM_ADDRESS,
                                     LOCAL_DOMAIN, CLIENT_AUTHENTICATION_TYPE, 
                                     server_ip_address, SMTP_SERVER_PORT);
#endif

    if (status != NX_SUCCESS)
    {
        error_counter++;
    }

    return;
}


/* Define the test threads.  */

static void    thread_0_entry(ULONG thread_input)
{

ULONG       actual_status;
UINT        wait_timeout = 300;


    printf("NetX Test:   SMTP Abnormal Packet Test.................................");

    /* Check for earlier errors.  */
    if(error_counter)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Ensure the IP instance has been initialized.  */
    status = nx_ip_status_check(&client_ip, NX_IP_INITIALIZE_DONE, &actual_status, 100);

    /* Check status...*/
    if(status != NX_SUCCESS)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Let the server get set up first. */
    tx_thread_sleep(20);


    /* The SMTP test initialize.  */
    smtp_test_initialize();

     /* Create a mail instance with the above text message and recipient info. */
    status =  nx_smtp_mail_send(&smtp_client, RECIPIENT_ADDRESS, NX_SMTP_MAIL_PRIORITY_NORMAL, 
                                SUBJECT_LINE, MAIL_BODY, strlen(MAIL_BODY));

    /* Create a mail instance with the above text message and recipient info. */

    /* Check for errors. */
    if (status == NX_SUCCESS)
    {
        error_counter++;
    }

    /* Give the server time to disconnect. */
    while(wait_timeout)
    {

        if (server_complete)
            break;

        tx_thread_sleep(20);

        wait_timeout -= 20;
    }

    /* Release threadx resources used by client. */
    status = nx_smtp_client_delete(&smtp_client);

    /* Return the test result.  */
    if (error_counter)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }
    else
    {

        printf("SUCCESS!\n");
        test_control_return(0);
    }
}

/* This thread task simulates SMTP server response to client requests. */
static void    thread_1_entry(ULONG thread_input)
{

UINT        i;
ULONG      actual_status;
NX_PACKET   *my_packet;


    /* Check for earlier errors.  */
    if(error_counter)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Ensure the IP instance has been initialized.  */
    status = nx_ip_status_check(&server_ip, NX_IP_INITIALIZE_DONE, &actual_status, 100);

    /* Check status...*/
    if(status != NX_SUCCESS)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Create a TCP socket act as the SMTP server.  */
    status = nx_tcp_socket_create(&server_ip, &server_socket, "Socket 1", 
                                  NX_IP_NORMAL, NX_FRAGMENT_OKAY, NX_IP_TIME_TO_LIVE, 
                                  8000, NX_NULL, NX_NULL);

    /* Check status.  */
    if (status)
    {
        error_counter++;
    }

    /* Register the receive notify function.  */
    status =  nx_tcp_socket_receive_notify(&server_socket, receive_packet_function);

    /* Check status.  */
    if (status)
    {
        error_counter++;
    }

    /* Bind the TCP socket to the SMTP port.  */
    status =  nx_tcp_server_socket_listen(&server_ip, SMTP_SERVER_PORT, &server_socket, 5, NX_NULL);

    /* Check status.  */
    if (status)
    {
        error_counter++;
    }   

    status = nx_tcp_server_socket_accept(&server_socket, NX_WAIT_FOREVER);

    /* Check status.  */
    if (status)
    {
        error_counter++;
    }   

    /* Act as the SMTP server to receive the SMTP Client query and send the SMTP response.  */
    for (i = 0; i < msg_count; i++)
    {

        if (i != 0)
        {
            /* Receive a TCP packet.  */
            status =  nx_tcp_socket_receive(&server_socket, &my_packet, 10 * NX_IP_PERIODIC_RATE);
    
            /* Check status.  */
            if (status)
            {
                error_counter++;
                continue;
            }

            /* Release the packet.  */
            nx_packet_release(my_packet);
        }

        /* Send the SMTP response packet.  */
        status = nx_smtp_response_packet_send(&server_socket, i);

        /* Check status.  */
        if (status)
        {        
            error_counter++; 
        }  
    }

    /* Wait for Client process last message. */
    tx_thread_sleep(20);

    /* Unlisten and Unbind the TCP socket.  */

    status = nx_tcp_server_socket_unaccept(&server_socket);
    status += nx_tcp_server_socket_unlisten(&server_ip, SMTP_SERVER_PORT);

    /* Check status.  */
    if (status)
    {        
        error_counter++;   
    }

    /* Delete the TCP socket.  */
    status =  nx_tcp_socket_delete(&server_socket);

    /* Check status.  */
    if (status)
    {        
        error_counter++;
    }

    server_complete = NX_TRUE;
    
}

static void    receive_packet_function(NX_TCP_SOCKET *socket_ptr)
{

    if (socket_ptr == &server_socket)
        notify_calls++;
}

static UINT   nx_smtp_response_packet_send(NX_TCP_SOCKET *server_socket, UINT packet_number)
{
UINT        status;
NX_PACKET   *response_packet;

    /* Allocate a response packet.  */
    status =  nx_packet_allocate(&server_packet_pool, &response_packet, NX_TCP_PACKET, 200);
    
    /* Check status.  */
    if (status)
    {
        error_counter++;
        return status;
    }

    memset(response_packet -> nx_packet_prepend_ptr, 0, (response_packet -> nx_packet_data_end - response_packet -> nx_packet_prepend_ptr));

    /* Write the SMTP response messages into the packet payload!  */
    memcpy(response_packet -> nx_packet_prepend_ptr, 
           smtp_test[packet_number].smtp_test_pkt_data, 
           smtp_test[packet_number].smtp_test_pkt_size); 

    /* Adjust the write pointer.  */
    response_packet -> nx_packet_length =  smtp_test[packet_number].smtp_test_pkt_size; 
    response_packet -> nx_packet_append_ptr =  response_packet -> nx_packet_prepend_ptr + response_packet -> nx_packet_length;

    /* Send the TCP packet with the correct port.  */
    status =  nx_tcp_socket_send(server_socket, response_packet, 100);

    /* Check the status.  */
    if (status)      
    {
        error_counter++;
        nx_packet_release(response_packet);         
    }

    return status;
}

static void     smtp_test_initialize()
{
    smtp_test[msg_count].smtp_test_pkt_data = &invliad_response_220_greetings_pkt[0];
    smtp_test[msg_count++].smtp_test_pkt_size = sizeof(invliad_response_220_greetings_pkt);

    smtp_test[msg_count].smtp_test_pkt_data = &response_221_bye_pkt[0];
    smtp_test[msg_count++].smtp_test_pkt_size = response_221_bye_size;  
}
#else

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_smtp_abnormal_packet_test_application_define(void *first_unused_memory)
#endif
{

    /* Print out test information banner.  */
    printf("NetX Test:   SMTP Abnormal Packet Test.................................N/A\n"); 

    test_control_return(3);  
}      
#endif