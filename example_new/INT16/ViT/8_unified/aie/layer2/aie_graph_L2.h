#ifndef __GRAPH_L2_H__
#define __GRAPH_L2_H__

#include <adf.h>
#include "para_L2.h"
#include <stdio.h>
using namespace adf;


template <int COL_OFFSET, int ROW_OFFSET>
class mm_k1_B3_L2: public graph {
  private:
    kernel mm[L2_NUM_KERNEL];
    adf::pktsplit<L2_NUM_KERNELS_PER_PAC>  sp_a0[L2_NUM_PACK_IN];
    adf::pktsplit<L2_NUM_KERNELS_PER_PAC>  sp_b0[L2_NUM_PACK_IN];

  public:
    port<input> in0[L2_NUM_PACK_IN],in1[L2_NUM_PACK_IN];
    port<output> out;
    
    mm_k1_B3_L2() {
      
      for (int j=0; j<L2_NUM_PACK_IN; j++){
        sp_a0[j]  = adf::pktsplit<L2_NUM_KERNELS_PER_PAC>::create();
		    sp_b0[j]  = adf::pktsplit<L2_NUM_KERNELS_PER_PAC>::create();
		    adf::connect< adf::pktstream > (in0[j], sp_a0[j].in[0]);
		    adf::connect< adf::pktstream > (in1[j], sp_b0[j].in[0]);
      }

      for (int i=0; i<L2_NUM_KERNEL;i++){
        if(i==0){
          mm[i] = kernel::create(mm5_kernel0_L2);
          source(mm[i]) = "mm5_kernel0_L2.cc";
        }
        else if(i==L2_NUM_KERNEL-1){
          mm[i] = kernel::create(mm5_kernel2_L2);
          source(mm[i]) = "mm5_kernel2_L2.cc";
        }
        
        else{
          mm[i] = kernel::create(mm5_kernel1_L2);
          source(mm[i]) = "mm5_kernel1_L2.cc";
        }
        
        runtime<ratio>(mm[i]) = 1;
        if ((ROW_OFFSET%2)==0){
          adf::location<kernel>(mm[i]) = adf::tile(COL_OFFSET+i,ROW_OFFSET);
        }
        else{
          adf::location<kernel>(mm[i]) = adf::tile(COL_OFFSET-i,ROW_OFFSET);
        }
      }

      for (int i=0; i<L2_NUM_KERNEL;i++){
        if ((ROW_OFFSET%2)==0){
          location<buffer>(mm[i].in[0]) =
          { address(COL_OFFSET+i, ROW_OFFSET+1, 0x0000), 
            address(COL_OFFSET+i, ROW_OFFSET+1, 0x2000)};
          location<buffer>(mm[i].in[1]) =
          { address(COL_OFFSET+i, ROW_OFFSET, 0x4000), 
            address(COL_OFFSET+i, ROW_OFFSET, 0x6000)};
          if(i==L2_NUM_KERNEL-1){
            location<buffer>(mm[i].out[0]) =
            { address(COL_OFFSET+i-1, ROW_OFFSET, 0x4C00), 
              address(COL_OFFSET+i-1, ROW_OFFSET, 0x6C00)};
          }  
          location<stack>(mm[i]) = address(COL_OFFSET+i, ROW_OFFSET, 0x3000);
        }
        else{
          location<buffer>(mm[i].in[0]) =
          { address(COL_OFFSET-i, ROW_OFFSET-1, 0x0000), 
            address(COL_OFFSET-i, ROW_OFFSET-1, 0x2000)};
          location<buffer>(mm[i].in[1]) =
          { address(COL_OFFSET-i, ROW_OFFSET, 0x4000), 
            address(COL_OFFSET-i, ROW_OFFSET, 0x6000)};
          if(i==L2_NUM_KERNEL-1){
            location<buffer>(mm[i].out[0]) =
            { address(COL_OFFSET-i+1, ROW_OFFSET, 0x4C00), 
              address(COL_OFFSET-i+1, ROW_OFFSET, 0x6C00)};
          }
          location<stack>(mm[i]) = address(COL_OFFSET-i, ROW_OFFSET, 0x3000);
        }
      }

      for (int j=0; j<L2_NUM_PACK_IN; j++){
        for (int i=0; i<L2_NUM_KERNELS_PER_PAC;i++){
          connect<pktstream,window<L2_h1*L2_w1*L2_Byte>>(sp_a0[j].out[i], mm[i+j*L2_NUM_KERNELS_PER_PAC].in[0]);
          connect<pktstream,window<L2_w1*L2_w2*L2_Byte>>(sp_b0[j].out[i], mm[i+j*L2_NUM_KERNELS_PER_PAC].in[1]);
        }
      }

      for (int i=0; i<L2_NUM_KERNEL;i++){
        if(i==L2_NUM_KERNEL-1){
          connect<window<L2_h1*L2_w2*L2_Byte>>(mm[i].out[0], out);
        }
        else{
          connect<cascade>(mm[i].out[0], mm[i+1].in[2]);
        }
      }

    };
    
};

#endif /**********__GRAPH_H__**********/