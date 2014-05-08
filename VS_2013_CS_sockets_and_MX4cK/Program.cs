using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;
using System.IO;

namespace VS_2013_CS_wireless_access_point
{
   // this class courtesy of 
   class ShowIP
   {
      //public static void Main(string[] args)
      public static void show_IP_main()
      {
         //string name = (args.Length < 1) ? Dns.GetHostName() : args[0];
         string name = Dns.GetHostName();
         try
         {
            //IPAddress[] addrs = Dns.Resolve(name).AddressList;
            IPAddress[] addrs = Dns.GetHostEntry(name).AddressList;
            foreach (IPAddress addr in addrs)
               Console.WriteLine("{0}/{1}", name, addr);
         }
         catch (Exception e)
         {
            Console.WriteLine(e.Message);
         }
      }
   }

   class Program
   {
      static void Main(string[] args)
      {
         //ShowIP.show_IP_main();

         TcpClient client = new TcpClient("10.10.10.126", 5);
         Stream s = client.GetStream();
         StreamReader sr = new StreamReader(s);
         StreamWriter sw = new StreamWriter(s);
         sw.AutoFlush = true;

         string my_string = Console.ReadLine();
         //string new_string = String.Format("--You wrote: '{0}'", my_string);
         //Console.WriteLine(new_string);

         sw.WriteLine(my_string);
         my_string = sr.ReadLine();
         while ("" == my_string)
         {
            my_string = sr.ReadLine();
         }
         string new_string = String.Format("--You wrote: '{0}'", my_string);

         Console.WriteLine(new_string);

         s.Close();
         client.Close();
      }
   }
}
