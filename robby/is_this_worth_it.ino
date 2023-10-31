int position[3] = {0, 0, 0}; // {x-coordinate,y, direction N0 E1 S2 W3}



void count_junction(){
  int junction_array[12][2] = {//arranged in order of encounter in phase 1 loop
  {0,0}, {0, 0}, {0, 1}, {1, 1}, {2,1}, {2,2}, {1,2}, {0,2}, {-1,2}, {-2,2}, {-2,1}, {-1,1},
  };//0th item is just a placeholder
  currentJunction++;
  position[0] = junction_array[currentJunction][0];
  position[1] = junction_array[currentJunction][1];
}

void navigo(int x, int y, int z){//go to x,y location facing z direction
}