#include<stdio.h>
#include<sstream>
#include<iostream>
#include<math.h>

using namespace std;

int get_skip_addr(int base_addr, bool isInterleave, int wrap_skip, int normal_skip){
  int raw_addr = base_addr + normal_skip;
  int raw_addr_bankIdx = (raw_addr>>19) & 0xf;
  int normal_skip_addr = raw_addr & (~(-1<<22));

  int interleave_skip_addr = normal_skip_addr;
  if(raw_addr_bankIdx>7){
    interleave_skip_addr = interleave_skip_addr + wrap_skip;
  }
  int skip_addr = isInterleave ? interleave_skip_addr:normal_skip_addr;

  return skip_addr;
}

int *get_ref_addr(int base_addr, bool isInterleave, int unit_skip, int slice_skip, int plane_skip, int cube_skip, int wrap_skip,
                  int unit_num, int slice_num, int plane_num, int cube_num){
  int *cubeAddrPoint = new int[cube_num*plane_num*slice_num*unit_num];

  cout <<"unit_num:" <<unit_num<<", slice_num:" <<slice_num<<", plane_num:" <<plane_num<<", cube_num:"<<cube_num<< endl;
  cout <<"refrecen slice_skip: "<<unit_num*unit_skip<< endl;
  cout <<"refrecen plane_skip: "<<slice_num*unit_num*unit_skip<< endl;
  cout <<"refrecen normal cube_skip:  "<<plane_num*slice_num*unit_num*unit_skip<< endl;
  cout <<"refrecen interleave cube_skip:  "<<(floor(plane_num/8)+1)*slice_num*unit_num*unit_skip<< endl;

  int cube_addr  = base_addr;
  int plane_addr = cube_addr;
  int slice_addr = cube_addr;
  int unit_addr  = cube_addr;

  cout<<"base_addr= "<<base_addr<<", unit_skip= "<<unit_skip<<", slice_skip= "<<slice_skip<<", plane_skip= "<<plane_skip<<", cube_skip= "<<cube_skip<<", wrap_skip= "<<wrap_skip<<endl;
  cout << "isInterleave = " << isInterleave << endl;
  for(int cubeCnt=0; cubeCnt<cube_num;cubeCnt++){
    cout << "cubeCnt= " << cubeCnt << endl;
    plane_addr = cube_addr;
    cube_addr = get_skip_addr(cube_addr, isInterleave, wrap_skip, cube_skip);
    for(int planeCnt=0; planeCnt<plane_num;planeCnt++){
      cout << "planeCnt = " << planeCnt << endl;
      slice_addr = plane_addr;
      plane_addr = get_skip_addr(plane_addr, isInterleave, wrap_skip, plane_skip);
      for(int sliceCnt=0; sliceCnt<slice_num;sliceCnt++){
        unit_addr  = slice_addr;
        slice_addr = get_skip_addr(slice_addr, isInterleave, wrap_skip, slice_skip);
        for(int unitCnt=0; unitCnt<unit_num;unitCnt++){
          int idx = cubeCnt*plane_num*slice_num*unit_num + planeCnt*slice_num*unit_num + sliceCnt*unit_num + unitCnt;
          cout << "idx = " << idx << ", unit_addr = " << unit_addr << endl;
          cubeAddrPoint[idx] = unit_addr;
          unit_addr += unit_skip;
        }
      }
    }
  }

  return cubeAddrPoint;
}

int main(){
  int base_addr = 0;
  int unit_num  = 2;
  int slice_num = 10;
  int plane_num = 10;
  int cube_num  = 2;

  cout << "normal skip" << endl;
  //   base_addr  isInterleave  unit_skip  slice_skip  plane_skip cube_skip wrap_skip  
  unit_num  = 2; slice_num = 2; plane_num = 2; cube_num  = 2;
  int normal_skip_param_array[][5] = {
          {   8,         16,            32,               0,          0},
          {  16,         32,            64,               0,          0},
          {  32,         64,           128,               0,          0},
          {  64,        128,           256,               0,          0},
          { 640,        128,        524288,               0,          0},
          {  32,        448,        524288,               0,          0},
          {  64,        768,        524288,               0,          0},
          {  64,        524288*1,      128,               0,          0},
          { 640,        524288*1,     1280,               0,          0},
          {  64,        524288*2,      128,               0,          0},
          { 640,        524288*2,     1280,               0,          0},
          {  64,        524288*4,      128,               0,          0},
          { 640,        524288*4,     1280,               0,          0},
          {   8,         16,            32,           65536,          0},
          {  64,        128,           256,           65536,          0},
          { 640,        128,        524288,           65536,          0},
          {  32,        448,        524288,           65536,          0},
          {  64,        768,        524288,           65536,          0},
          {   8,         16,            32,        524288*1,          0},
          {  64,        128,           256,        524288*1,          0},
          {   8,         16,            32,        524288*2,          0},
          {  64,        128,           256,        524288*2,          0},
          {   8,         16,            32,        524288*3,          0},
          {  64,        128,           256,        524288*3,          0},
          {   8,         16,            32,        524288*4,          0},
          {  64,        128,           256,        524288*4,          0},
          { 640,        128,      524288*2,               0,          0},
          {  32,        448,      524288*2,               0,          0},
          {  64,        768,      524288*2,               0,          0},
          { 640,        128,      524288*3,               0,          0},
          {  32,        448,      524288*3,               0,          0},
          {  64,        768,      524288*3,               0,          0},
          { 640,        128,      524288*4,               0,          0},
          {  32,        448,      524288*4,               0,          0},
          {  64,        768,      524288*4,               0,          0}
      };
  for(int i=0;i<35;i++){
    bool isInterleave = false;
    int unit_skip  = normal_skip_param_array[i][0];
    int slice_skip = normal_skip_param_array[i][1];
    int plane_skip = normal_skip_param_array[i][2];
    int cube_skip  = normal_skip_param_array[i][3];
    int wrap_skip  = normal_skip_param_array[i][4];
    get_ref_addr(base_addr, isInterleave, unit_skip, slice_skip, plane_skip, cube_skip, wrap_skip, unit_num, slice_num, plane_num, cube_num);
  }

  cout << "interleave skip" << endl;
  //   base_addr  isInterleave  unit_skip  slice_skip plane_skip cube_skip  wrap_skip  
  unit_num  = 2; slice_num = 10; plane_num = 10; cube_num  = 2;
  int interleave_skip_param_array[][5] = {
          {   8,     524288,            32,      320,         16},
          {  16,     524288,            64,      640,         32},
          {  32,     524288,           128,     1280,         64},
          {  64,     524288,           256,     2560,        128},
          {1280,     524288,           128,     2560,         64},
          {  32,     524288,            64,     1280,       1792},
          {  64,     524288,           128,     2560,       3072},
          {   8,         16,        524288,      320,        160},
          {  16,         32,        524288,      640,        320},
          {  32,         64,        524288,     1280,        640},
          {  64,        128,        524288,     2560,       1280},
          {1280,         64,        524288,     2560,        640},
          {  32,       1792,        524288,      128,         64},
          {  64,       3072,        524288,      256,        128},
          {   8,         16,           160,   524288,       1600},
          {  16,         32,           320,   524288,       3200},
          {  32,         64,           640,   524288,       6400},
          {  64,        128,          1280,   524288,      12800},
          {1280,         64,           640,   524288,       6400},
          {  32,       1792,            64,   524288,        640},
          {  64,       3072,           128,   524288,       1280}
      };
  for(int i=0;i<21;i++){
    bool isInterleave = true;
    int unit_skip  = interleave_skip_param_array[i][0];
    int slice_skip = interleave_skip_param_array[i][1];
    int plane_skip = interleave_skip_param_array[i][2];
    int cube_skip  = interleave_skip_param_array[i][3];
    int wrap_skip  = interleave_skip_param_array[i][4];
    get_ref_addr(base_addr, isInterleave, unit_skip, slice_skip, plane_skip, cube_skip, wrap_skip, unit_num, slice_num, plane_num, cube_num);
  }

  cout << "cube interleave skip" << endl;
  unit_num  = 2; slice_num = 4; plane_num = 4; cube_num  = 10;
  int interleave_cube_skip_param_array[][5] = {
          {   8,         16,            64,   524288,       256},
          {  16,         32,           128,   524288,       512},
          {  32,         64,           256,   524288,      1024},
          {  64,        128,           512,   524288,      2048}
      };
  for(int i=0;i<4;i++){
    bool isInterleave = true;
    int unit_skip  = interleave_cube_skip_param_array[i][0];
    int slice_skip = interleave_cube_skip_param_array[i][1];
    int plane_skip = interleave_cube_skip_param_array[i][2];
    int cube_skip  = interleave_cube_skip_param_array[i][3];
    int wrap_skip  = interleave_cube_skip_param_array[i][4];
    get_ref_addr(base_addr, isInterleave, unit_skip, slice_skip, plane_skip, cube_skip, wrap_skip, unit_num, slice_num, plane_num, cube_num);
  }

  // instance
  int *p = get_ref_addr(base_addr, false, 8, 16, 32, 0, 0, unit_num, slice_num, plane_num, cube_num);
  for(int i=0;i<16;i++){
    cout << ">>> " << p[i] << endl;
  }
}

