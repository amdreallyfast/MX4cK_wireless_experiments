#include "my_scoket.h"

#include <iostream>
using std::cout;
using std::endl;

#define DEFAULT_BUF_LEN 512

int main(void)
{
   int app_return_val = 0;
   int num_accelerometer_reads = 0;
   char tx_buffer[DEFAULT_BUF_LEN];
   char rx_buffer[DEFAULT_BUF_LEN];
   my_scoket S;


   S.init("10.10.10.126", "5");
   //S.send_data(tx_buffer, strlen(tx_buffer));

   while (1)
   {
      S.receive_data(rx_buffer, DEFAULT_BUF_LEN);
      system("cls");
      cout << "'" << rx_buffer << "'" << endl;
   }

   return 0;
}