#ifndef STAFF_H
#define STAFF_H

#include "communicator.h"
#include "lamport.h"

struct Menel_request{
  int id;
  int weight;
  int destroy_count;

};

class Staff : public Communicator, public Lamport
{
public:
  Staff();
  Staff(int positions_number,MPI_Comm COMM_WORLD, MPI_Comm COMM_STAFF);

  void participate();
  void get_positions_number();

private:
  int iteration;

  int management_id;
  int staff_rank;
  int positions_number;
  int world_group_size;
  int staff_group_size;

  int last_taken_out;

  vector<int> staff;
  vector<int> colors;

  deque<Menel_request> drunk_list;
  MPI_Comm COMM_STAFF, COMM_WORLD;
  Packet p;

  bool locked;
  bool new_iteration;
  int expected_blacks;

  int remaining_requests;
  int remaining_consensus_requests;

  int target_menel_id;
  int target_menel_weight;

  int lm_count;
  int acc_count;

  vector<int> my_group;

  int my_status;


  Lamport requests_lamport;

  void get_menels_status();
  void listen();
  void timestamp_request(int source, int timestamp);
  void wait_for_requests();
  void consensus_data_exchange(int subject, int subject_timestamp);
  void update_iteration(int target, int source);
  void seek_consensus();
  void decide();
  void take_menel_out(int target);
  void gather_info();
  void clear_iteration_data();

  bool menel_exists(int id);
  int remove_menel(int id);
  int get_menel_weight(int id);
  int get_menel_counter(int id);
  void lower_menel_counter(int id);

  void update_group_info(int target);

  void color(int id, int color);
  int get_color(int id);
  int count_whites();
  int count_blacks();
  void green_to_white();
  void force_timestamps();
  void set_all_to_white();

  void set_flag(bool value);
  bool get_flag();

  void clear_channel();

};

#endif
