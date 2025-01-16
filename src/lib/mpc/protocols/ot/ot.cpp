// #include <iostream>
// #include <thread>
//
// #include "crypto/sha2.h"
// #include "io/net_io.h"
// #include "math/group.h"
// #include "mpc/protocols/ot/npot.h"
//
// // Assuming there are functions to run server and client I/O operations
// void server_function(const char* address, int port, qst::math::algstruct::Group* group) {
//   qst::io::NetIO server{address, port};
//
//   // Add server handling code here
//   std::cout << "Server running on port " << port << "\n";
//
//   qst::mpc::protocols::NPOT<qst::io::NetIO> npot {1, server, group};
//
//   qst::types::Data d1 {}, d2 {};
//   for (int i=0; i < 16; i++) {
//     d1.m_bytes.push_back(0xad);
//     d2.m_bytes.push_back(0xdd);
//
//   }
//
//
//   npot.send_onetwo(d1, d2);
//   std::cout << "After send? \n";
//   while (true) {
//
//
//
//   };
// }
//
// void client_function(const char* address, int port, qst::math::algstruct::Group* group) {
//   qst::io::NetIO client{"127.0.0.1", port};
//
//   qst::mpc::protocols::NPOT<qst::io::NetIO> npot {1, client, group};
//   //
//    npot.recv_onetwo(true);
//   // std::cout << "RECV: " << recved.m_bytes[0] << "\n";
//
//   // npot.send_onetwo(d1, d2);
//
//   while (true) {
//     // Client loop
//   };
// }
//
//
//
// int main(int argc, char ** argv) {
//
//   //   auto server_address = "127.0.0.1";
// //   auto client_address = "127.0.0.1";
// //
// //   int server_port = 9500;
// //   int client_port = 9500;
// //
// //   qst::math::algstruct::Group group {};
// //
// //   std::cout << argv[1] << "\n";
// //   if (std::strcmp(argv[1],"S")==0)
// //   {
// //     std::cout << "Server running on port " << server_port << "\n";
// //     server_function(server_address, server_port, &group);
// // }
// //   else
// //     client_function(client_address, client_port, &group);
//
//
//
//   // // Launch server and client in separate threads
//   // std::thread server_thread(server_function, server_address, server_port, &group);
//   // std::thread client_thread(client_function, client_address, client_port, &group);
//   //
//   //
//   //
//   // server_thread.join();
//   // client_thread.join();
//
//   return 0;
// }
