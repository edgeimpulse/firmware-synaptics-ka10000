/*------------------------------------------------------------------------------
(C) 2014-2018 Conexant Systems Inc.
  All rights reserved.

  CONEXANT SYSTEMS, INC. CONFIDENTIAL AND PROPRIETARY

  The information contained in this source code file
  is strictly confidential and proprietary to Conexant Systems, Inc.
  ("Conexant")

  No part of this file may be possessed, reproduced or distributed, in
  any form or by any means for any purpose, without the express written
  permission of Conexant Systems Inc.

  Except as otherwise specifically provided through an express agreement
  with Conexant that governs the confidentiality, possession, use
  and distribution of the information contained in this file, CONEXANT
  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
  AND REPRESENTATIONS.  IN_ NO EVENT WILL CONEXANT BE LIABLE FOR ANY DAMAGES
  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN_ THIS FILE.
--------------------------------------------------------------------------------

  File Name: thin_client.c

  Description:

------------------------------------------------------------------------------*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef WIN32
//#define ENABLE_USB_CONTROL 1
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#endif
#include "AudioSmart_definitions.h"
#if ENABLE_USB_CONTROL
#include "AudioSmart_usb_interface_api.h"
#elif ENABLE_I2C_CONTROL
#include "AudioSmart_i2c_interface_api.h"
#endif
#include "AudioSmart_sendcmd.h"
#include "thin_client.h"

#define DEFAULT_BUFFER_LEN 4096
#define DEFAULT_PORT 7777         // Default listening port
#define DEFAULT_PROTO SOCK_STREAM // Default to TCP protocol
//#define INVALID_SOCKET          -1
#define SLAVE_ADDRESS 0x41 //reno device

typedef unsigned long DWORD;
#ifndef WIN32
#define INVALID_SOCKET -1
typedef void *HANDLE;
typedef int SOCKET;
#endif
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_storage SOCKADDR_STORAGE;
typedef struct AppData
{
  int run;
  int duration;
  unsigned int profile_cnt;
  unsigned long bufferCount;

  int sock_desc;
    //int *new_sock;
  int card_num;
  int codec_type; // 0: Solana , 1: Falcon
#ifdef WIN32
  HANDLE network_thread;
#else
  pthread_t network_thread;
#endif
} AppData_t;

/* Globals */
AppData_t gAppData;
/* Forward Decleration */

#ifdef WIN32
void usleep(__int64 usec)
{
  HANDLE timer;
  LARGE_INTEGER ft;

  ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

  timer = CreateWaitableTimer(NULL, TRUE, NULL);
  SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
  WaitForSingleObject(timer, INFINITE);
  CloseHandle(timer);
}
#endif


void sys_mdelay(uint32_t ms_delay)
{
    usleep(ms_delay * 1000);
}

unsigned int intf_usb = 0;
unsigned int intf_setpid = 0x1494;
unsigned int intf_setvid = 0x06CB;

// slave_device_address - the 7-bit I2C slave address
// register_address_len - length of the I2C register address in bytes.
// the register address is sent MSB first.

/*--------------------------------------------------------------------------------
 *
 * Function: SendCmd
 *
 */
int SendCmd(Command *cmd)
{
#if defined (ENABLE_I2C_CONTROL) || defined (ENABLE_USB_CONTROL)
    return AudioSmart_mid_sendcmd((uint32_t *)cmd);
#endif
  return 0;
}

/*--------------------------------------------------------------------------------
 *
 * Function: process_user_sendcmd
 *
 */
int process_user_sendcmd(char *usr_msg, char *resp)
{
  int cnt = 0, ret = -1, print_skip = 0;
  char *ptr, *resp_ptr, *endPtr = NULL;
  Command command;
  unsigned int *num = (unsigned int *)&command;

  if ((strncmp(usr_msg, SENDCMD_TAG, SENDCMD_TAG_LEN) == 0))
  {
    ptr = usr_msg + SENDCMD_TAG_LEN;
    while (*ptr != 0 && *ptr != '\n')
    {
      *num = (unsigned int)strtoul(ptr, &endPtr, 16);
      ptr += 9;
      num++;
      cnt++;
    }
    ret = SendCmd(&command);

    print_skip = 1;
  }

  if (resp)
  {
    if (command.head.num_32b_words >= 0)
    {
      cnt = command.head.num_32b_words + 2;
    } else if (ret != SENDCMD_RC_REPLY_TIMEOUT){
      cnt = 2;
    } else {
      return SENDCMD_RC_REPLY_TIMEOUT;
    }
    ret = cnt;
    num = (unsigned int *)(&(command));
    resp_ptr = (char *)resp;
    while ((cnt)-- > 0)
    {
      sprintf(resp_ptr, "%08x ", *num);
      num++;
      resp_ptr += 9;
    }
    sprintf(resp_ptr, "\n");
    resp_ptr++;
    *resp_ptr = 0; // send NULL at the end
  }

  return ret;
}

/*--------------------------------------------------------------------------------
 *
 * Function: network_save_sock_desc_new_sock
 *
 */
void network_save_sock_desc_new_sock(int socket_desc)
{
  gAppData.sock_desc = socket_desc;
}

/*--------------------------------------------------------------------------------
 *
 * Function: network_save_thread_id
 *
 */
#ifdef WIN32
void network_save_thread_id(HANDLE thread_id)
{
  gAppData.network_thread = thread_id;
}
#else
void network_save_thread_id(pthread_t thread_id)
{
  gAppData.network_thread = thread_id;
}
#endif
/*--------------------------------------------------------------------------------
 *
 * Function: server_thread
 *
 */
#ifdef WIN32
DWORD WINAPI server_thread(void *param)
#else
void *server_thread(void *param)
#endif
{
  int    socket_desc, client_sock, c, bind_inc = 0;//*new_sock = NULL, 
  struct sockaddr_in server, client;
  int status = 0;
#ifdef WIN32
  const char optVal = 1;
#else
  const int optVal = 1;
#endif
  const socklen_t optLen = sizeof(optVal);

  //int  sock;
  int read_sz, len;
  char client_message[DEFAULT_BUFFER_LEN];
  char response_msg[DEFAULT_BUFFER_LEN];

  //Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1)
  {
    fprintf(stderr, "Could not create socket; So terminating the 'server_thread'");
    goto end;
  }
  fprintf(stdout, "Socket created\n");
#ifdef WIN32
//  setsockopt(socket_desc, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, &optVal, optLen);
#else
//  setsockopt(socket_desc, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (void *)&optVal, optLen);
#endif
  //Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(DEFAULT_PORT);

  //Bind
  status = bind(socket_desc, (struct sockaddr *)&server, sizeof(server));
  while (status != 0)
  {
    //print the error message
    perror("bind failed. Error: ");
    bind_inc++;
    server.sin_port = htons(DEFAULT_PORT + bind_inc);
    fprintf(stdout, "Use new_port = %d\n", DEFAULT_PORT + bind_inc);
    status = bind(socket_desc, (struct sockaddr *)&server, sizeof(server));
    //return 1;
  }
  fprintf(stdout, "Server IP:\n");

  //Listen
  listen(socket_desc, 3);

  //Accept and incoming connection
#ifdef WIN32
  char szHost[256];  
  gethostname(szHost,256);  
  hostent *pHost=gethostbyname(szHost);  
  in_addr addr;  
         
  char *p=pHost->h_addr_list[0];  
  memcpy(&addr.S_un.S_addr,p,pHost->h_length);  
  char *szIp=inet_ntoa(addr);  
  fprintf(stdout,"\t %s\n",szIp);
#else
#ifdef HOST_PLATFORM_ANDRROID
     system("ip route | awk 'NR==1 {print $9}' "); 
#else   
  system("hostname -I");
#endif  
#endif

  fprintf(stdout, "\nWaiting for incoming connections...");
  fprintf(stdout, "\n##########################################\n");
  fprintf(stdout, "Use TCP/IP Port Num :=     %d  \n", DEFAULT_PORT + bind_inc);
  fprintf(stdout, "##########################################\n\n");

  c = sizeof(struct sockaddr_in);
  while(gAppData.run)
  {
#ifdef WIN32
    client_sock = accept (socket_desc, (struct sockaddr *) &client, & c);
#else
    client_sock = accept (socket_desc, (struct sockaddr *) &client, (socklen_t *) & c);
#endif
    if (client_sock>0)
    {
      fprintf (stdout,"Connection accepted\n");

      network_save_sock_desc_new_sock(socket_desc);//, new_sock);

      memset(&client_message, 0, sizeof(client_message));
      memset(&response_msg, 0, sizeof(response_msg));

      //Receive a message from client
      while ((read_sz = recv (client_sock, client_message, 2000, 0)) > 0) 
      {
        if ((len = process_user_sendcmd (client_message, response_msg)) >= 0)
        {
          //Send the message back to client
#ifdef WIN32
          send (client_sock, response_msg, (len * 9) + 1, 0);
#else
          write (client_sock, response_msg, (len * 9) + 1);
#endif
        } else if (len == SENDCMD_RC_REPLY_TIMEOUT) {
          fprintf(stderr, "WARNING: Timeout happened\n");
        }
        memset (&client_message, 0, read_sz + 4);
      }

      if (read_sz == 0)
      {
          fprintf (stdout,"Client disconnected\n");
          fflush (stdout);
      }
      else if (read_sz == -1)
      {
        perror("recv failed");
      }
    }

    if (client_sock < 0)
    {
      perror ("accept failed");
      goto end;
    }
  }

end:

#ifdef WIN32
  ExitThread(0);
#else
  pthread_exit(NULL);
#endif
  return NULL;
}

/*--------------------------------------------------------------------------------
 *
 * Function: start_sock_server
 *
 */
int start_sock_server(void)
{
#ifdef WIN32
  HANDLE sniffer_thread;
  DWORD sniffer_thread_id;
  sniffer_thread = CreateThread(NULL, 0, server_thread, (void *)NULL, 0, &sniffer_thread_id);
  if (sniffer_thread < 0)
  {
    perror("could not create thread");
    return 1;
  }
#else
  pthread_t sniffer_thread;

  if (pthread_create(&sniffer_thread, NULL, server_thread, (void *)NULL) < 0)
  {
    perror("could not create thread");
    return 1;
  }
#endif
  network_save_thread_id(sniffer_thread);

  fprintf(stdout, "Server Thread Created Successfully\n");

  return 0;
}

/*--------------------------------------------------------------------------------
 *
 * Function: network_startup
 *
 * Description:
 *  Create a server socket for each address (interfaces). and create a server thread
 *  for each address to listen on port DEFAULT_PORT.
 */
int network_startup(void)
{
#ifdef WIN32
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;
#endif
  struct addrinfo hints, *results = NULL, *addrptr = NULL;
  //pthread_t *server_threads = NULL;
  //char      hoststr[NI_MAXHOST], servstr[NI_MAXSERV];
  char *intr = NULL;
  char port[] = "7777";
  int socket_type = DEFAULT_PROTO, address_family = AF_UNSPEC, // Default to any (IPv4 or IPv6)
      socket_count = 0, retval;

#ifdef WIN32
  /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
  wVersionRequested = MAKEWORD(2, 2);

  err = WSAStartup(wVersionRequested, &wsaData);
  if (err != 0)
  {
    /* Tell the user that we could not find a usable */
    /* Winsock DLL.                                  */
    printf("WSAStartup failed with error: %d\n", err);
    return 1;
  }

  /* Confirm that the WinSock DLL supports 2.2.*/
  /* Note that if the DLL supports versions greater    */
  /* than 2.2 in addition to 2.2, it will still return */
  /* 2.2 in wVersion since that is the version we      */
  /* requested.                                        */

  if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
  {
    /* Tell the user that we could not find a usable */
    /* WinSock DLL.                                  */
    printf("Could not find a usable version of Winsock.dll\n");
    WSACleanup();
    return 1;
  }
#endif
  /* Parse arguments */
  address_family = AF_INET;
  socket_type = SOCK_STREAM;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = address_family;
  hints.ai_socktype = socket_type;
  hints.ai_protocol = ((socket_type == SOCK_DGRAM) ? IPPROTO_UDP : IPPROTO_TCP);
  // If intr is NULL then request the passive "bind" address
  hints.ai_flags = ((intr == NULL) ? AI_PASSIVE : 0);

  retval = getaddrinfo(intr, port, &hints, &results);
  if (retval != 0)
  {
    fprintf(stderr, "getaddrinfo failed: %d\n", retval);
    goto ERR;
  }

  // Make sure we got at least one address back
  if (results == NULL)
  {
    fprintf(stderr, "Unable to resolve the address of local host\n");
    goto ERR;
  }

  // Count how many addresses were returned
  addrptr = results;
  while (addrptr)
  {
    socket_count++;
    addrptr = addrptr->ai_next;
  }
  freeaddrinfo(results);

#ifdef DEBUG_DEMOAPP
  fprintf(stdout, "socket_count = %d\n", socket_count);
#endif

  retval = start_sock_server();

  if (retval != 0)
    return retval;

  return 0;

ERR:
  return -1;
}

/*--------------------------------------------------------------------------------
 *
 * Function: network_cleanup
 *
 * Description:
 *  Create a server socket for each address (interfaces). and create a server thread
 *  for each address to listen on port DEFAULT_PORT.
 */
#ifdef WIN32
void network_cleanup(int socket_desc, HANDLE thread_id)//, int * new_sock
{
  //TODO: change pthread
  if (socket_desc >= 0)
    closesocket(socket_desc);
        /*
        if (new_sock){
    free((void *)new_sock);
            new_sock = NULL;
        }*/

  if (thread_id)
    CloseHandle(thread_id);
}
#else
void network_cleanup(int socket_desc, pthread_t thread_id)//, int * new_sock
{
  if (socket_desc >= 0)
    close(socket_desc);
       /*
  if (new_sock)
    free((void *)new_sock);
            new_sock = NULL;
        }*/

  if (thread_id)
#if defined __ANDROID__ || defined ANDROID
    pthread_kill(thread_id, SIGKILL);
#else
    pthread_cancel(thread_id);
#endif
}
#endif

void pretty_quit(int ret)
{
  network_cleanup(gAppData.sock_desc, gAppData.network_thread);//, gAppData.new_sock
  fprintf (stdout,"exit service \n");


  if (intf_usb)
  {
#ifdef ENABLE_USB_CONTROL
    AudioSmart_usb_interface_close();
#endif
  } else {
#ifdef ENABLE_I2C_CONTROL
    AudioSmart_i2c_interface_close();
#endif
  }
  exit(ret);
}

/*--------------------------------------------------------------------------------
 *
 * Function: signalCallBack
 *
 */
void signalCallBack(int signo)
{
  //#ifdef DEBUG_DEMOAPP
  fprintf(stdout, "\nSignal Received ; SigNo=%d\n", signo);
  //#endif
  gAppData.run = 0;
  //network_cleanup(gAppData.sock_desc, gAppData.new_sock, gAppData.network_thread);
  //exit(signo);
}

/*--------------------------------------------------------------------------------
 *
 * Function: main
 *
 */
int main(const int argc, char *const argv[])
{
  int ret = 0;
  int rc;
  char *card_name = NULL;

#ifdef ENABLE_USB_CONTROL
        intf_usb = 1;
#endif 

  if (argc > 1)
  {
    /* parsing the command line arguments */
    for (int i = 1; i < argc; i++)
    {
#ifdef ENABLE_USB_CONTROL
      intf_usb = 1;
      if (strcmp(argv[i], "-p") == 0 && intf_usb == 1)
      {
        intf_setpid = strtol(argv[++i], NULL, 16);
      }
      else if (strcmp(argv[i], "-v") == 0 && intf_usb == 1)
      {
        intf_setvid = strtol(argv[++i], NULL, 16);
      }
#elif ENABLE_I2C_CONTROL
      if (strcmp(argv[i], "-i") == 0)
      {
        intf_usb = 0;
      }
      else if (strcmp(argv[i], "-d") == 0 && intf_usb == 0)
      {
        if ((i = i + 1) >= argc)
        {
          fprintf(stderr, "ERROR: Please enter i2c dev path name\n");
          return -1;
        }
        else
        {
          card_name = argv[i];
        }
      }
#endif
      else if (strcmp(argv[i], "-h") == 0)
      {
        printf("Usage:\n");
        printf("=====\n");
        printf("  thin_client <Options>\n");
        printf("  Options:\n");
        printf("---------\n");
        printf("  -h:  Help\n");
#ifdef ENABLE_I2C_CONTROL
        printf("  -i:  Use DEV I2C interface for sending commands\n");
        printf("      -d: I2C path for DEV I2C interface\n");
#endif
#ifdef ENABLE_USB_CONTROL
        printf("      -p <HEX>: PID of the device to connect to via USB-HID\n");
        printf("      -v <HEX>: VID of the device to connect to via USB-HID\n");
#endif
        printf("\n");
        exit(0);
      }
    }
  }

  if (intf_usb)
  {
#ifdef ENABLE_USB_CONTROL
    rc = AudioSmart_usb_interface_init(intf_setvid, intf_setpid);
    if (rc < 0)
	{
	   printf("\nWarning: Fail to open with VID(0x%04X) PID(0x%04X)\n",intf_setvid,intf_setpid);
	   printf("\nTry with (vid:0x%04X pid:0x%04X) \n",0x0572,0xFFFF);
	   rc = AudioSmart_usb_interface_init(0x0572, 0xFFFF);
	   if (rc < 0)
	   {
		printf("\nError: Fail to open with VID(0x%04X) PID(0x%04X)\n",0x0572,0xFFFF);
		return (rc);
	   }
	}
#else
    fprintf(stderr, "ERROR: USB control not compiled.\n");
    return -1;
#endif
  }
  else
  {
#ifdef ENABLE_I2C_CONTROL
    // Set the offset length (register address length) to 2 bytes
    if ((rc = AudioSmart_i2c_interface_init(SLAVE_ADDRESS, 2, (int8_t *)card_name)) < 0)
      return (rc);
#else
    fprintf(stderr, "ERROR: I2C control not compiled.\n");
    return -1;
#endif
  }

#if defined(ENABLE_I2C_CONTROL)|| defined(ENABLE_USB_CONTROL)
   /* Note we need to  register the delay functions to initial midlle level api */ 
	if(AudioSmart_mid_level_init(&sys_mdelay)) return rc;
#endif 

  if (network_startup())
  {
#ifdef WIN32
    fprintf(stderr, "%s() ; network_startup failed !!\n", __FUNCTION__);
#else
    fprintf(stderr, "%s() ; network_startup failed !!\n", __func__);
#endif
    pretty_quit(-1);
  }

  signal(SIGINT, signalCallBack);
  signal(SIGTERM, signalCallBack);
#ifndef WIN32
  signal(SIGQUIT, signalCallBack);
  signal(SIGKILL, signalCallBack);
#endif

  gAppData.run = 1;
  while (gAppData.run)
  {
    usleep(10000);
  }

  pretty_quit(ret);

  return ret;
}
