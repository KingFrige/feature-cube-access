#include<stdio.h>
#include<sstream>
#include<iostream>
#include<math.h>

using namespace std;

int get_skip_addr(int base_addr, int skip){
  int raw_addr = base_addr + skip;
  int raw_addr_bankIdx = (raw_addr>>19) & 0xf;
  int skip_addr = raw_addr & (~(-1<<22));

  return skip_addr;
}

int *get_ref_addr(int base_addr, int unit_skip, int slice_skip, int plane_skip, int cube_skip,
                  int unit_num, int slice_num, int plane_num, int cube_num){
  int *cubeAddrPoint = new int[cube_num*plane_num*slice_num*unit_num];

  cout <<"unit_num:" <<unit_num<<", slice_num:" <<slice_num<<", plane_num:" <<plane_num<<", cube_num:"<<cube_num<< endl;
  cout <<"refrecen slice_skip: "<<unit_num*unit_skip<< endl;
  cout <<"refrecen plane_skip: "<<slice_num*unit_num*unit_skip<< endl;
  cout <<"refrecen cube_skip:  "<<plane_num*slice_num*unit_num*unit_skip<< endl;

  int cube_addr  = base_addr;
  int plane_addr = cube_addr;
  int slice_addr = cube_addr;
  int unit_addr  = cube_addr;

  cout<<dec<<"base_addr= "<<base_addr<<", unit_skip= "<<unit_skip<<", slice_skip= "<<slice_skip<<", plane_skip= "<<plane_skip<<", cube_skip= "<<cube_skip<<endl;
  for(int cubeCnt=0; cubeCnt<cube_num;cubeCnt++){
    cout << "cubeCnt= " << cubeCnt << endl;
    plane_addr = cube_addr;
    cube_addr = get_skip_addr(cube_addr, cube_skip);
    for(int planeCnt=0; planeCnt<plane_num;planeCnt++){
      cout << "planeCnt = " << planeCnt << endl;
      slice_addr = plane_addr;
      plane_addr = get_skip_addr(plane_addr, plane_skip);
      for(int sliceCnt=0; sliceCnt<slice_num;sliceCnt++){
        unit_addr  = slice_addr;
        slice_addr = get_skip_addr(slice_addr, slice_skip);
        for(int unitCnt=0; unitCnt<unit_num;unitCnt++){
          int idx = cubeCnt*plane_num*slice_num*unit_num + planeCnt*slice_num*unit_num + sliceCnt*unit_num + unitCnt;
          cout << "idx = " << idx << ", unit_addr = " << dec << unit_addr << endl;
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

  unit_num  = 2; slice_num = 2; plane_num = 2; cube_num  = 2;
  int normal_skip_param_array[][5] = {
          {   8,         16,            32,               0},
          {  16,         32,            64,               0},
          {  32,         64,           128,               0},
          {  64,        128,           256,               0},
          { 640,        128,        524288,               0},
          {  32,        448,        524288,               0},
          {  64,        768,        524288,               0},
          {  64,        524288*1,      128,               0},
          { 640,        524288*1,     1280,               0},
          {  64,        524288*2,      128,               0},
          { 640,        524288*2,     1280,               0},
          {  64,        524288*4,      128,               0},
          { 640,        524288*4,     1280,               0},
          {   8,         16,            32,           65536},
          {  64,        128,           256,           65536},
          { 640,        128,        524288,           65536},
          {  32,        448,        524288,           65536},
          {  64,        768,        524288,           65536},
          {   8,         16,            32,        524288*1},
          {  64,        128,           256,        524288*1},
          {   8,         16,            32,        524288*2},
          {  64,        128,           256,        524288*2},
          {   8,         16,            32,        524288*3},
          {  64,        128,           256,        524288*3},
          {   8,         16,            32,        524288*4},
          {  64,        128,           256,        524288*4},
          { 640,        128,      524288*2,               0},
          {  32,        448,      524288*2,               0},
          {  64,        768,      524288*2,               0},
          { 640,        128,      524288*3,               0},
          {  32,        448,      524288*3,               0},
          {  64,        768,      524288*3,               0},
          { 640,        128,      524288*4,               0},
          {  32,        448,      524288*4,               0},
          {  64,        768,      524288*4,               0}
      };
  for(int i=0;i<35;i++){
    int unit_skip  = normal_skip_param_array[i][0];
    int slice_skip = normal_skip_param_array[i][1];
    int plane_skip = normal_skip_param_array[i][2];
    int cube_skip  = normal_skip_param_array[i][3];
    get_ref_addr(base_addr, unit_skip, slice_skip, plane_skip, cube_skip, unit_num, slice_num, plane_num, cube_num);
  }

  // instance
  int *p = get_ref_addr(base_addr, 8, 16, 32, 0, unit_num, slice_num, plane_num, cube_num);
  for(int i=0;i<unit_num*slice_num*plane_num*cube_num;i++){
    cout << "p = " << p[i] << endl;
  }
  int *tmpPoint = get_ref_addr(262144, 64, 768, 524288, 0, 12, 1, 12, 1);
  for(int i=0;i<12*1*12*1;i++){
    cout << "tmpPoint = " << tmpPoint[i] << endl;
  }
}
