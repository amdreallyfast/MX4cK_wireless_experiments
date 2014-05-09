#include "my_header.h"

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "5"
#define DEFAULT_BUFLEN 512

// tutorial courtesy of MSDN's "Creating a Basic Winsock Application"
// http://msdn.microsoft.com/en-us/library/windows/desktop/ms737629(v=vs.85).aspx
int main(void)
{
   WSADATA wsa_data;

   int iResult;
   struct addrinfo *result = NULL;
   struct addrinfo *ptr = NULL;
   struct addrinfo hints;
   SOCKET ConnectSocket = INVALID_SOCKET;

   int rx_byte_count = 0;
   int tx_byte_count = 0;
   int recvbuflen = DEFAULT_BUFLEN;
   char *sendbuf = "this is a test";
   char recvbuf[DEFAULT_BUFLEN];


   // Initialize Winsock
   iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
   if (iResult != 0) {
      printf("WSAStartup failed: %d\n", iResult);
   }

   if (0 == iResult)
   {
      ZeroMemory(&hints, sizeof(hints));
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;

      // Resolve the server address and port
      iResult = getaddrinfo("10.10.10.126", DEFAULT_PORT, &hints, &result);
      if (iResult != 0) {
         printf("getaddrinfo failed: %d\n", iResult);
      }
   }

   if (0 == iResult)
   {
      // Attempt to connect to the first address returned by
      // the call to getaddrinfo
      ptr = result;

      // Create a SOCKET for connecting to server
      ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

      if (ConnectSocket == INVALID_SOCKET) {
         printf("Error at socket(): %ld\n", WSAGetLastError());
         iResult = -111;
      }
   }

   if (0 == iResult)
   {
      // Connect to server.
      iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
      if (iResult == SOCKET_ERROR) {
         printf("Error at socket(): %ld\n", WSAGetLastError());
         iResult = -222;
      }

      // Should really try the next address returned by getaddrinfo
      // if the connect call failed
      // But for this simple example we just free the resources
      // returned by getaddrinfo and print an error message

      if (ConnectSocket == INVALID_SOCKET) {
         printf("Unable to connect to server!\n");
         printf("Error at socket(): %ld\n", WSAGetLastError());
         iResult = -333;
      }
   }

   if (0 == iResult)
   {
      // Send an initial buffer
      tx_byte_count = strlen(sendbuf);
      cout << "sending '" << sendbuf << "', length '" << tx_byte_count << "'" << endl;
      iResult = send(ConnectSocket, sendbuf, tx_byte_count, 0);
      if (iResult == SOCKET_ERROR) {
         printf("send failed: %d\n", WSAGetLastError());
      }
      else
      {
         printf("Bytes Sent: %ld\n", iResult);
      }
   }

   if (0 == iResult)
   {
      // shutdown the connection for sending since no more data will be sent
      // the client can still use the ConnectSocket for receiving data
      iResult = shutdown(ConnectSocket, SD_SEND);
      if (iResult == SOCKET_ERROR) {
         printf("shutdown failed: %d\n", WSAGetLastError());
      }
   }

   


   // Receive data
   do {
      rx_byte_count = recv(ConnectSocket, recvbuf, recvbuflen, 0);
      if (rx_byte_count > 0)
      {
         printf("Bytes received: %d\n", rx_byte_count);
         recvbuf[rx_byte_count] = 0;
         cout << recvbuf << endl;
         break;
      }         
      else if (rx_byte_count == 0)
      {
         printf("Connection closed\n");
      }
      else
      {
         // rx_byte_count < 0
         printf("recv failed: %d\n", WSAGetLastError());
         iResult = -444;
      }
         
   } while (rx_byte_count > 0);


   if (0 == iResult)
   {
      // shutdown the send half of the connection since no more data will be sent
      iResult = shutdown(ConnectSocket, SD_SEND);
      if (iResult == SOCKET_ERROR) {
         printf("shutdown failed: %d\n", WSAGetLastError());
         iResult = -555;
      }
   }

   cout << "result is '" << iResult << "'" << endl;

   if (INVALID_SOCKET != ConnectSocket)
   {
      closesocket(ConnectSocket);
      ConnectSocket = INVALID_SOCKET;
   }

   freeaddrinfo(result);

   // clean up anything left over in WS2_32.dll
   WSACleanup();


   return 0;
}