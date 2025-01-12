#include <thread>
#include "io/net_io.h"
#include "math/group.h"
#include "mpc/protocols/ot/npot.h"

constexpr int data_size {16};

void npot_one_n_server(const char *address, const int port, qst::math::algstruct::Group* group) {
  /* Create NetIO with the address and port */
  const qst::io::NetIO server{address, port, true};

  /* Create NPOT object */
  qst::mpc::protocols::NPOT<qst::io::NetIO> npot {server, group, data_size};

  /* Generate 4 data samples (tuples): [('a','A'), ('b','B'), ('c','C'), ('d','D')] */
  const auto lower_case_letters = new qst::types::Data[4];
  const auto upper_case_letters = new qst::types::Data[4];

  // for (int i=0; i < data_size; i++) {
  //   lower_case_letters[0].m_bytes.push_back('a');
  //   lower_case_letters[1].m_bytes.push_back('b');
  //   lower_case_letters[2].m_bytes.push_back('c');
  //   lower_case_letters[3].m_bytes.push_back('d');
  //
  //   upper_case_letters[0].m_bytes.push_back('A');
  //   upper_case_letters[1].m_bytes.push_back('B');
  //   upper_case_letters[2].m_bytes.push_back('C');
  //   upper_case_letters[3].m_bytes.push_back('D');
  // }
  //
  //
  // /* Send */
  // npot.send_n(lower_case_letters, upper_case_letters, 4);
}


void npot_one_n_client(const char *address, const int port, qst::math::algstruct::Group* group) {
  /* Create NetIO with the address and port */
  const qst::io::NetIO client{address, port, false};

  qst::mpc::protocols::NPOT<qst::io::NetIO> npot {client, group, 16};

  /* List of choices */
  const auto choices = new bool[4];
  choices[0] = true;
  choices[1] = false;
  choices[2] = false;
  choices[3] = true;


  // /* List of letters retrieved from the OT */
  // const auto letters = new qst::types::Data[4];
  //
  // /* Recv */
  // npot.recv_n(letters, choices,4);
  // std::cout << letters[0].m_bytes[0] << std::endl;
  // std::cout << letters[1].m_bytes[0] << std::endl;
  // std::cout << letters[2].m_bytes[0] << std::endl;
  // std::cout << letters[3].m_bytes[0] << std::endl;

}



void npot_one_two_server(const char *address, const int port, qst::math::algstruct::Group* group) {
  /* Create NetIO with the address and port */
  const qst::io::NetIO server{address, port, true};

  /* Create NPOT object */
  qst::mpc::protocols::NPOT<qst::io::NetIO> npot {server, group, 16};

  /* Generate 2 data samples */
  // qst::types::Data data1 {}, data2 {};
  // for (int i=0; i < data_size; i++) {
  //   data1.m_bytes.push_back('A');
  //   data2.m_bytes.push_back('B');
  // }

  /* Send */
  // npot.send(data1, data2);
}

void npot_one_two_client(const char *address, const int port, qst::math::algstruct::Group* group) {
  /* Create NetIO with the address and port */
  const qst::io::NetIO server{address, port, false};

  /* Create NPOT object */
  qst::mpc::protocols::NPOT<qst::io::NetIO> npot {server, group, 16};

  /* Recv */
  constexpr bool choice {false};
  const qst::types::Data data {npot.recv(choice)};

  // std::cout << data.m_bytes[0] << std::endl;
}



int main() {
  /* Defining the IP address and port number for server*/
  auto server_listen_address = "127.0.0.1";
  int server_listen_port = 9500;

  /* Defining the IP address and port number for client*/
  auto client_dest_address = "127.0.0.1";
  int client_dest_port = 9500;


  /* Test 1-2 OT */
  /* Crate two threads */
  std::thread server_thread_12(npot_one_two_server, server_listen_address, server_listen_port, nullptr);
  std::thread client_thread_12(npot_one_two_client, client_dest_address, client_dest_port, nullptr);

  server_thread_12.join();
  client_thread_12.join();



  /* Test 1-N OT */
  /* Crate two threads */
  std::thread server_thread_1n(npot_one_n_server, server_listen_address, server_listen_port, nullptr);
  std::thread client_thread_1n(npot_one_n_client, client_dest_address, client_dest_port, nullptr);

  server_thread_1n.join();
  client_thread_1n.join();

  return 0;
}
