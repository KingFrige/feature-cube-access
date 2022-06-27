#include<stdio.h>
#include<sstream>
#include<iostream>

using namespace std;

int *get_ref_addr(int base_addr, bool isInterleave, int unit_skip, int slice_skip, int plane_skip, int wrap_skip, int cube_skip){
  int cube_num  = 2;
  int plane_num = 2;
  int slice_num = 2;
  int unit_num  = 2;
  int *cubeAddrPoint = new int[cube_num*plane_num*slice_num*unit_num];

  int cube_addr  = base_addr;
  int plane_addr = cube_addr;
  int slice_addr = cube_addr;
  int unit_addr  = cube_addr;

  cout<<"base_addr= "<<base_addr<<", unit_skip= "<<unit_skip<<", slice_skip= "<<slice_skip<<", plane_skip= "<<plane_skip<<", wrap_skip= "<<wrap_skip<<", cube_skip= "<<cube_skip<<endl;
  for(int cubeCnt=0; cubeCnt<cube_num;cubeCnt++){
    cube_addr = base_addr + cube_skip * cubeCnt;
    for(int planeCnt=0; planeCnt<plane_num;planeCnt++){
      plane_addr = cube_addr + plane_skip * planeCnt;
      cout << "planeCnt = " << planeCnt << endl;
      for(int sliceCnt=0; sliceCnt<slice_num;sliceCnt++){
        slice_addr = plane_addr + slice_skip * sliceCnt;
        for(int unitCnt=0; unitCnt<unit_num;unitCnt++){
          int raw_addr = slice_addr + unit_skip * unitCnt;
          int raw_addr_bankIdx = (raw_addr>>19) & 0xf;
          int normal_skip_addr = raw_addr & (~(-1<<22));

          int interleave_skip_addr = normal_skip_addr;
          if(raw_addr_bankIdx>7){
            interleave_skip_addr = interleave_skip_addr + wrap_skip;
          }
          unit_addr = isInterleave ? interleave_skip_addr:normal_skip_addr;
          int idx = cubeCnt*plane_num*slice_num*unit_num + planeCnt*slice_num*unit_num + sliceCnt*unit_num + unitCnt;
          cout << "idx = " << idx << ", unit_addr = " << unit_addr << endl;
          cubeAddrPoint[idx] = unit_addr;
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

  //           base_addr  isInterleave  unit_skip  slice_skip     plane_skip   wrap_skip  cube_skip
  get_ref_addr(base_addr,        false,         8,         16,            32,          0,        0);
  get_ref_addr(base_addr,        false,        16,         32,            64,          0,        0);
  get_ref_addr(base_addr,        false,        32,         64,           128,          0,        0);
  get_ref_addr(base_addr,        false,        64,        128,           256,          0,        0);
  get_ref_addr(base_addr,        false,       640,        128,        524288,          0,        0);
  get_ref_addr(base_addr,        false,        32,        448,        524288,          0,        0);
  get_ref_addr(base_addr,        false,        64,        768,        524288,          0,        0);

  get_ref_addr(base_addr,        false,        64,        524288*1,      128,          0,        0);
  get_ref_addr(base_addr,        false,       640,        524288*1,     1280,          0,        0);
  get_ref_addr(base_addr,        false,        64,        524288*2,      128,          0,        0);
  get_ref_addr(base_addr,        false,       640,        524288*2,     1280,          0,        0);
  get_ref_addr(base_addr,        false,        64,        524288*4,      128,          0,        0);
  get_ref_addr(base_addr,        false,       640,        524288*4,     1280,          0,        0);

  get_ref_addr(base_addr,        false,         8,         16,            32,          0,        65536);
  get_ref_addr(base_addr,        false,        64,        128,           256,          0,        65536);
  get_ref_addr(base_addr,        false,       640,        128,        524288,          0,        65536);
  get_ref_addr(base_addr,        false,        32,        448,        524288,          0,        65536);
  get_ref_addr(base_addr,        false,        64,        768,        524288,          0,        65536);

  get_ref_addr(base_addr,        false,         8,         16,            32,          0,        524288*1);
  get_ref_addr(base_addr,        false,        64,        128,           256,          0,        524288*1);
  get_ref_addr(base_addr,        false,         8,         16,            32,          0,        524288*2);
  get_ref_addr(base_addr,        false,        64,        128,           256,          0,        524288*2);
  get_ref_addr(base_addr,        false,         8,         16,            32,          0,        524288*3);
  get_ref_addr(base_addr,        false,        64,        128,           256,          0,        524288*3);
  get_ref_addr(base_addr,        false,         8,         16,            32,          0,        524288*4);
  get_ref_addr(base_addr,        false,        64,        128,           256,          0,        524288*4);

  get_ref_addr(base_addr,        false,       640,        128,      524288*2,          0,        0);
  get_ref_addr(base_addr,        false,        32,        448,      524288*2,          0,        0);
  get_ref_addr(base_addr,        false,        64,        768,      524288*2,          0,        0);
  get_ref_addr(base_addr,        false,       640,        128,      524288*3,          0,        0);
  get_ref_addr(base_addr,        false,        32,        448,      524288*3,          0,        0);
  get_ref_addr(base_addr,        false,        64,        768,      524288*3,          0,        0);
  get_ref_addr(base_addr,        false,       640,        128,      524288*4,          0,        0);
  get_ref_addr(base_addr,        false,        32,        448,      524288*4,          0,        0);
  get_ref_addr(base_addr,        false,        64,        768,      524288*4,          0,        0);

  get_ref_addr(base_addr,         true,        64,        768,        524288,        768,        0);
}

