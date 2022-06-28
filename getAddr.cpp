#include<stdio.h>
#include<sstream>
#include<iostream>

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

int *get_ref_addr(int base_addr, bool isInterleave, int unit_skip, int slice_skip, int plane_skip, int wrap_skip, int cube_skip){
  int cube_num  = 1;
  int plane_num = 9;
  int slice_num = 9;
  int unit_num  = 2;
  int *cubeAddrPoint = new int[cube_num*plane_num*slice_num*unit_num];

  int cube_addr  = base_addr;
  int plane_addr = cube_addr;
  int slice_addr = cube_addr;
  int unit_addr  = cube_addr;

  cout<<"base_addr= "<<base_addr<<", unit_skip= "<<unit_skip<<", slice_skip= "<<slice_skip<<", plane_skip= "<<plane_skip<<", wrap_skip= "<<wrap_skip<<", cube_skip= "<<cube_skip<<endl;
  cout << "isInterleave = " << isInterleave << endl;
  for(int cubeCnt=0; cubeCnt<cube_num;cubeCnt++){
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

  int *p = get_ref_addr(base_addr, false, 8, 16, 32, 0, 0);

  for(int i=0;i<16;i++){
    cout << ">>> " << p[i] << endl;
  }

  // normal skip
  //   base_addr  isInterleave  unit_skip  slice_skip     plane_skip   wrap_skip  cube_skip
  int normal_skip_param_array[][5] = {
          {   8,         16,            32,          0,        0},
          {  16,         32,            64,          0,        0},
          {  32,         64,           128,          0,        0},
          {  64,        128,           256,          0,        0},
          { 640,        128,        524288,          0,        0},
          {  32,        448,        524288,          0,        0},
          {  64,        768,        524288,          0,        0},
          {  64,        524288*1,      128,          0,        0},
          { 640,        524288*1,     1280,          0,        0},
          {  64,        524288*2,      128,          0,        0},
          { 640,        524288*2,     1280,          0,        0},
          {  64,        524288*4,      128,          0,        0},
          { 640,        524288*4,     1280,          0,        0},
          {   8,         16,            32,          0,        65536},
          {  64,        128,           256,          0,        65536},
          { 640,        128,        524288,          0,        65536},
          {  32,        448,        524288,          0,        65536},
          {  64,        768,        524288,          0,        65536},
          {   8,         16,            32,          0,        524288*1},
          {  64,        128,           256,          0,        524288*1},
          {   8,         16,            32,          0,        524288*2},
          {  64,        128,           256,          0,        524288*2},
          {   8,         16,            32,          0,        524288*3},
          {  64,        128,           256,          0,        524288*3},
          {   8,         16,            32,          0,        524288*4},
          {  64,        128,           256,          0,        524288*4},
          { 640,        128,      524288*2,          0,        0},
          {  32,        448,      524288*2,          0,        0},
          {  64,        768,      524288*2,          0,        0},
          { 640,        128,      524288*3,          0,        0},
          {  32,        448,      524288*3,          0,        0},
          {  64,        768,      524288*3,          0,        0},
          { 640,        128,      524288*4,          0,        0},
          {  32,        448,      524288*4,          0,        0},
          {  64,        768,      524288*4,          0,        0}
      };

  for(int i=0;i<35;i++){
    bool isInterleave = false;
    int unit_skip  = normal_skip_param_array[i][0];
    int slice_skip = normal_skip_param_array[i][1];
    int plane_skip = normal_skip_param_array[i][2];
    int wrap_skip  = normal_skip_param_array[i][3];
    int cube_skip  = normal_skip_param_array[i][4];
    get_ref_addr(base_addr, isInterleave, unit_skip, slice_skip, plane_skip, wrap_skip, cube_skip);
  }

  // interleave skip
  //   base_addr  isInterleave  unit_skip  slice_skip     plane_skip   wrap_skip  cube_skip
  int interleave_skip_param_array[][5] = {
          {   8,         16,        524288,         32,        0},
          {  16,         32,        524288,         64,        0},
          {  32,         64,        524288,        128,        0},
          {  64,        128,        524288,        256,        0},
          {1280,         64,        524288,        128,        0},
          {  32,       1792,        524288,         64,        0},
          {  64,       3072,        524288,        128,        0},
          {   8,     524288,            32,         16,        0},
          {  16,     524288,            64,         32,        0},
          {  32,     524288,           128,         64,        0},
          {  64,     524288,           256,        128,        0},
          {1280,     524288,           128,         64,        0},
          {  32,     524288,            64,       1792,        0},
          {  64,     524288,           128,       3072,        0}
      };

  for(int i=0;i<14;i++){
    bool isInterleave = true;
    int unit_skip  = interleave_skip_param_array[i][0];
    int slice_skip = interleave_skip_param_array[i][1];
    int plane_skip = interleave_skip_param_array[i][2];
    int wrap_skip  = interleave_skip_param_array[i][3];
    int cube_skip  = interleave_skip_param_array[i][4];
    get_ref_addr(base_addr, isInterleave, unit_skip, slice_skip, plane_skip, wrap_skip, cube_skip);
  }

}

