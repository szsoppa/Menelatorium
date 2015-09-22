#include "staff.h"


Staff::Staff()
{
}

Staff::Staff(int positions_number, MPI_Comm COMM_WORLD, MPI_Comm COMM_STAFF)
{
  this->positions_number = positions_number;
  this->COMM_WORLD = COMM_WORLD;
  this->COMM_STAFF = COMM_STAFF;
  this->staff_rank = get_group_id();
  this->iteration = 0;
  this->last_taken_out = -1;

  MPI_Comm_size(this->COMM_STAFF, &staff_group_size);
  MPI_Comm_size(this->COMM_WORLD, &world_group_size);

  this->management_id = world_group_size - 1;

  for (int i=0; i<this->staff_group_size; i++) this->staff.push_back(i);
  for (int i=0; i<this->staff_group_size; i++) this->colors.push_back(STAFF::WHITE);

}

bool sort_asc(const Menel_request &first, const Menel_request &second)
{
        return first.id > second.id;
}
void Staff::get_menels_status()
{
  MPI_Status message_status;
  for (int i=0; i<this->positions_number; i++)
  {
      read(p.get(), p.get_size(), MPI_ANY_SOURCE, MENEL::STATE, COMM_WORLD, &message_status);
      if (p.get_message() == MENEL::DRUNK){


        Menel_request m;
        m.id = message_status.MPI_SOURCE;
        m.weight = p.get_timestamp();
        m.destroy_count = p.get_timestamp();
        drunk_list.push_back(m);

      }




  }
  sort (drunk_list.begin(), drunk_list.end(), sort_asc);

  //cout << "MENEL LIST SIZE " << drunk_list.size() << endl;

}

void Staff::participate()
{
  int staff_timestamp = 0;
  init_priority_list(staff);
  MPI_Status message_status;
  this->iteration=0;
  while(true){
    this->iteration++;


    read(p.get(), p.get_size(), management_id, MANAGEMENT::OPEN_EXHIBITION, COMM_WORLD, &message_status);



  drunk_list.clear();
  get_menels_status();

  set_all_to_white();
  this->my_status = STAFF::NORMAL;
  clear_iteration_data();

  staff_timestamp = get_timestamp();

  update_other_timestamp(get_group_id(), staff_timestamp);

  p.set(staff_timestamp, CONSTANT::NOT_IMPORTANT);
  broadcast(p.get(), p.get_size(), STAFF::TIMESTAMP , this->COMM_STAFF);

  for (int i=0; i<this->staff_group_size; i++)
  {
      if (get_group_id() == i) continue;
      read(p.get(), p.get_size(), i, STAFF::TIMESTAMP, COMM_STAFF, &message_status);
      update_other_timestamp(message_status.MPI_SOURCE, p.get_timestamp());
  }

  while(drunk_list.size()>0){

    sort_list_by_id();

    if(get_color(get_group_id())==STAFF::WHITE && my_status==STAFF::NORMAL){
        gather_info();
      }

    if(drunk_list.size()>0){
      listen();
    }

  }

  send(p.get(), p.get_size(), management_id, STAFF::CONSENSUS, this->COMM_WORLD);
  cout << " END "<< get_group_id() << endl;
  set_all_to_white();
  clear_iteration_data();

}
}

void Staff::gather_info()
{
  bool stop = false;
  while(stop == false && drunk_list.size()>0){
    sort_list_by_id();
    if(enough_participants(drunk_list[0].weight))
    {
      if(get_my_position()<=drunk_list[0].weight){

        set_all_to_white();

        for(int i=0; i<drunk_list[0].weight; i++)
        {
          int id = get_id_by_position(i);
          color(id, STAFF::GREEN);
        }

        p.set(get_timestamp(), STAFF::LEAVING_FOR);
        p.set_data(2, this->iteration);

        broadcast(p.get(), p.get_size(), drunk_list[0].id+100, this->COMM_STAFF);


        take_menel_out(drunk_list[0].id);


        return;
      }
      else
      {
        for(int i=0; i<drunk_list[0].weight; i++)
        {
          int id = get_id_by_position(i);
          update_other_timestamp(id, CONSTANT::NOT_PARTICIPATING);


        }
        remove_menel(drunk_list[0].id);

      }

    }
    else{
      //
      stop = true;

    }
  }

  sort_list_by_id();
  for (int i=0; i < active_participants_count(); i++){
    int id = get_id_by_position(i);
    if(id == get_group_id()) continue;
    my_group.push_back(id);
  }

  target_menel_id = drunk_list[0].id;
  target_menel_weight = drunk_list[0].weight;
  remaining_requests = drunk_list[0].weight - (my_group.size()+1);
  remaining_consensus_requests = remaining_requests*(my_group.size());

  cout << get_timestamp() <<"  -- " << this->iteration << " Proces " << get_group_id() << " i dobieram dodatkowych " << remaining_requests << " do " << drunk_list[0].id << " ktory wazy "<< drunk_list[0].weight;


  cout << " Moja grupa to = ";
  for(unsigned int i=0; i<my_group.size(); i++){
    cout << " " << my_group[i];
  }
  cout << endl;

  this->my_status = STAFF::REQUESTING_ADDITIONAL;

  force_timestamps();

}

void Staff::listen()
{
  MPI_Status message_status;

  if(this->my_status==STAFF::PENDING){

    read(p.get(), p.get_size(), MPI_ANY_SOURCE, this->target_menel_id+100, COMM_STAFF, &message_status);
    switch (p.get_message())
    {

      case STAFF::ACCEPTED:
        {

          if (acc_count==0){
            for(int i=0; i<staff_group_size; i++){

            }
            update_other_timestamp(get_group_id(), get_timestamp());

          }
          acc_count++;
          my_group.push_back(message_status.MPI_SOURCE);
          update_other_timestamp(message_status.MPI_SOURCE, p.get_timestamp());
        if(acc_count==p.get_data(2)){
            update_group_info(p.get_data(2));
          }
        break;
        }

      case STAFF::LEAVING_FOR:{
        if(p.get_data(2)!=this->iteration){

        break;
        }
        lm_count++;
        color(message_status.MPI_SOURCE, STAFF::RED);
        update_other_timestamp(message_status.MPI_SOURCE, p.get_timestamp());

        if(lm_count == target_menel_weight){



          update_iteration(this->target_menel_id, message_status.MPI_SOURCE);
        }
        break;
        }

      case STAFF::FORCE:{
          if (get_color(get_group_id())==STAFF::GREEN && get_color(message_status.MPI_SOURCE)!=STAFF::GREEN){

            if(p.get_data(2)!=this->iteration){

            break;
            }
            //cout << get_timestamp() <<"  -- " << this->iteration << " Jestem " << get_group_id() << " otrzymalem rządanie timestampa od " << message_status.MPI_SOURCE << endl;

            p.set(get_timestamp(), STAFF::TIMESTAMP);
            p.set_data(2, this->iteration);

            send(p.get(), p.get_size(), message_status.MPI_SOURCE, this->target_menel_id+100, this->COMM_STAFF);

          }
          break;
        }



    }
  } else if (my_status==STAFF::REQUESTING_ADDITIONAL){
    read(p.get(), p.get_size(), MPI_ANY_SOURCE, this->target_menel_id+100, COMM_STAFF, &message_status);
    switch (p.get_message()) {


    case STAFF::TIMESTAMP:{
      //cout << get_timestamp() <<"  -- " << this->iteration << " Jestem " << get_group_id() << " otrzymalem timestamp od " << message_status.MPI_SOURCE;

        if(p.get_data(2)!=this->iteration){

        break;
        }
        timestamp_request(message_status.MPI_SOURCE, p.get_timestamp());

      break;
    }



  }

  } else {
    read(p.get(), p.get_size(), MPI_ANY_SOURCE, STAFF::CONSENSUS, COMM_STAFF, &message_status);
      switch (p.get_message()) {
        case STAFF::CONSENSUS:
        //cout << get_timestamp() <<"  -- " << this->iteration << " Jestem " << get_group_id() << " otrzymalem informacje od innego dobierajacego" << message_status.MPI_SOURCE << endl;

          consensus_data_exchange(p.get_timestamp(), p.get_data(2));
          break;
      }

  }

}

void Staff::timestamp_request(int source, int timestamp)
{

  if(this->my_status==STAFF::REQUESTING_ADDITIONAL){
    if (requests_lamport.get_timestamp_by_id(source)!=-1){

      return;
    }
    remaining_requests--;
    requests_lamport.add(source);

    requests_lamport.update_other_timestamp(source, timestamp);

    if(remaining_requests==0){
      if(my_group.size()==0) decide();
      else{
        seek_consensus();
        this->my_status=STAFF::CONSENSUS;
      }

    }

  }
}

void Staff::consensus_data_exchange(int subject, int subject_timestamp)
{

    int timestamp = requests_lamport.get_timestamp_by_id(subject);
    if(timestamp==-1)
    {
      requests_lamport.add(subject);
      requests_lamport.update_other_timestamp(subject, subject_timestamp);
    }
    else
    {
      if (timestamp < subject_timestamp)
      requests_lamport.update_other_timestamp(subject,subject_timestamp);
    }

    remaining_consensus_requests--;

    if (remaining_consensus_requests==0){
      decide();
    }

}

void Staff::seek_consensus()
{
  for(unsigned int i=0; i<my_group.size(); i++){

    for(int j=0; j<requests_lamport.size(); j++){
      int position = requests_lamport.get_id_by_position(j);
      int timestamp = requests_lamport.get_timestamp_by_id(position);
      p.set(position, STAFF::CONSENSUS);
      p.set_data(2, timestamp);
      send(p.get(), p.get_size(), my_group[i], STAFF::CONSENSUS, this->COMM_STAFF);
    }
  }
}



void Staff::take_menel_out(int target)
{
  cout << get_timestamp() <<"  -- it " << this->iteration << " Proces " << get_group_id() << " I wychodze po " << drunk_list[0].id << " ktory wazy " << drunk_list[0].weight << endl;


  p.set(get_timestamp(), STAFF::HELP);
  send(p.get(), p.get_size(), target, STAFF::HELP, this->COMM_WORLD);

  if(remove_menel(target)==-1) cout << " MENEL ERROR " << endl;



  update_other_timestamp(get_group_id(), get_timestamp());


  if(drunk_list.size()>0){
    this->my_status = STAFF::PENDING;
    this->target_menel_id = drunk_list[0].id;
    this->target_menel_weight = drunk_list[0].weight;
    lm_count=0;
    acc_count=0;

  }else{
    this->my_status = STAFF::NORMAL;
    return;
  }

  if(count_whites()==0 && drunk_list.size()>0 && staff_group_size==4){
    set_all_to_white();
    sort_list_by_id();
    my_status = STAFF::NORMAL;


    return;
  }

}

void Staff::clear_iteration_data()
{
  my_group.clear();
  requests_lamport.clear();

}

void Staff::decide(){

  requests_lamport.sort_list_by_id();
  int needed = drunk_list[0].weight - (my_group.size()+1);
  int a = my_group.size()+1;

  for(int i=0; i<needed; i++)
  {
    int tmp_id = requests_lamport.get_id_by_position(i);
    my_group.push_back(tmp_id);
    update_other_timestamp(tmp_id, requests_lamport.get_timestamp_by_id(tmp_id));
    p.set(get_timestamp(), STAFF::ACCEPTED);
    p.set_data(2, a);



    send(p.get(),p.get_size(), requests_lamport.get_id_by_position(i), target_menel_id+100, this->COMM_STAFF);

  }

  set_all_to_white();

  for(unsigned int i=0; i<my_group.size(); i++){
    color(my_group[i],STAFF::GREEN);
  }
  color(get_group_id(),STAFF::GREEN);
  clear_iteration_data();



  p.set(get_timestamp(), STAFF::LEAVING_FOR);
  p.set_data(2, this->iteration);
  broadcast(p.get(), p.get_size(), target_menel_id+100, this->COMM_STAFF);

  take_menel_out(drunk_list[0].id);
}

void Staff::update_iteration(int target, int source){

  remove_menel(target);


  for(int i=0; i<staff_group_size; i++){
    if(get_color(i)==STAFF::RED){
      update_other_timestamp(i, CONSTANT::NOT_PARTICIPATING);
      color(i,STAFF::BLACK);
    }
  }

  if(drunk_list.size()>0){

    this->target_menel_id = drunk_list[0].id;
    this->target_menel_weight = drunk_list[0].weight;
    lm_count=0;
    acc_count=0;

  }

  if(count_whites()==0 && drunk_list.size()>0){
    set_all_to_white();
    sort_list_by_id();
    my_status = STAFF::NORMAL;


    return;
  }


}



void Staff::update_group_info(int additional_group_members){
  MPI_Status message_status;


  p.set(get_timestamp(), STAFF::LEAVING_FOR);
  p.set_data(2, this->iteration);
  broadcast(p.get(), p.get_size(), target_menel_id+100, this->COMM_STAFF);

  lm_count++;

  while(lm_count<target_menel_weight){
    read(p.get(), p.get_size(), MPI_ANY_SOURCE, target_menel_id+100, COMM_STAFF, &message_status);
    if(p.get_message()==STAFF::LEAVING_FOR){
      lm_count++;
      update_other_timestamp(message_status.MPI_SOURCE,p.get_timestamp());

      if(find(my_group.begin(),my_group.end(), message_status.MPI_SOURCE)==my_group.end()){
        my_group.push_back(message_status.MPI_SOURCE);

      }

    }
  }





  for(int i=0; i<staff_group_size; i++){
    if(i==get_group_id())continue;
    if(get_color(i)==STAFF::RED){
      if(find(my_group.begin(),my_group.end(), i)==my_group.end()){
        my_group.push_back(i);

    }

  }
  }

  set_all_to_white();

  for(unsigned int i=0; i<my_group.size(); i++){

    color(my_group[i],STAFF::GREEN);
  }

  color(get_group_id(),STAFF::GREEN);


    for(int i=0; i<staff_group_size; i++){

      if(get_color(i)==STAFF::WHITE){
      update_other_timestamp(i, CONSTANT::NOT_PARTICIPATING);

    }
    }
  cout << get_timestamp() <<"  -- " << this->iteration << " Jestem " << get_group_id() << " i zostalem dobrany do wyniesienia  " << target_menel_id  << endl;

  take_menel_out(target_menel_id);

  clear_iteration_data();




}

int Staff::remove_menel(int id){
  for(unsigned int i=0; i<drunk_list.size(); i++){
    if (drunk_list[i].id == id)
    {
      int weight = drunk_list[i].weight;
      drunk_list.erase(drunk_list.begin()+i);

      return weight;

    }

  }
  return -1;
}

int Staff::get_menel_weight(int id){
  for(unsigned int i=0; i<drunk_list.size(); i++){
    if (drunk_list[i].id == id)
    { return drunk_list[i].weight;}
  }
  return -1;
}

void Staff::color(int id, int color){
  this->colors[id]=color;
}

int Staff::get_color(int id){
  return this->colors[id];
}

int Staff::count_whites(){
  int counter = 0;
  for(int i=0; i<staff_group_size; i++){
    if(get_color(i)==STAFF::WHITE) counter++;
  }

  return counter;
}



void Staff::force_timestamps(){
  p.set(get_timestamp(), STAFF::FORCE);
  p.set_data(2, this->iteration);
  broadcast(p.get(), p.get_size(), this->target_menel_id+100, this->COMM_STAFF);

}

bool Staff::menel_exists(int id){
  for(unsigned int i=0; i<drunk_list.size(); i++){
    if (drunk_list[i].id == id) return true;
  }

  return false;
}

void Staff::set_all_to_white(){
  for(int i=0; i<staff_group_size; i++){
    color(i, STAFF::WHITE);

  }
}
