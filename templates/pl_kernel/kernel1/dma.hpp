
#ifndef _DMA{{layer}}_H_
#define _DMA{{layer}}_H_
#include <cstring>
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define DATA_TYPE {{data_type}}

#define AXI_WIDTH_A 512
#define AXI_WIDTH_B 512
#define AXI_WIDTH_C 512
#define PLIO_WIDTH 128

#define H1 {{h1}}
#define W1 {{w1}}
#define W2 {{w2}}
#define A {{A}}
#define B {{B}}
#define C {{C}}
#define X {{X}}
#define Y {{Y}}
#define Z {{Z}}
#define PACK_IN {{PACK_IN}}
#define PACK_OUT {{PACK_OUT}}

const int M =H1*A*X;
const int K =W1*B*Y;
const int N =W2*C*Z;

const int A_PER_TRA=AXI_WIDTH_A/DATA_TYPE;
const int B_PER_TRA=AXI_WIDTH_B/DATA_TYPE;
const int C_PER_TRA=AXI_WIDTH_C/DATA_TYPE;

const int NUM_PER_TRA = PLIO_WIDTH/DATA_TYPE;

const int BUFFA_FACTOR= AXI_WIDTH_A/PLIO_WIDTH/2;
const int BUFFB_FACTOR= AXI_WIDTH_B/PLIO_WIDTH/2;
const int BUFFC_FACTOR= AXI_WIDTH_C/PLIO_WIDTH/2;

typedef ap_uint<PLIO_WIDTH> data_t;
typedef ap_axiu<PLIO_WIDTH, 0, 0, 0> axis_pkt;
typedef hls::stream<axis_pkt> axis_stream;

typedef hls::stream<ap_uint<32>> axis_stream_32;
typedef hls::stream<ap_uint<AXI_WIDTH_A>> axis_stream_A;
typedef hls::stream<ap_uint<AXI_WIDTH_B>> axis_stream_B;
typedef hls::stream<ap_uint<AXI_WIDTH_C>> axis_stream_C;

const int LEFT_SIZE=W1*(H1/NUM_PER_TRA);
const int RIGHT_SIZE=W2*(W1/NUM_PER_TRA);
const int OUT_SIZE=W2*(H1/NUM_PER_TRA);

const int boundA=M*K/A_PER_TRA;
const int boundB=K*N/B_PER_TRA;
const int boundC=M*N/C_PER_TRA;

typedef union{
    float data_float;
    unsigned int data_uint;
} fp_int;

#endif

