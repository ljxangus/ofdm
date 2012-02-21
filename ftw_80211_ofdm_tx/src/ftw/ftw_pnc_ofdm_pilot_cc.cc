/* -*- c++ -*- */
/*
 * Copyright 2004 Free Software Foundation, Inc.
 * 
 * This file is part of GNU Radio
 * 
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

/*
 * config.h is generated by configure.  It contains the results
 * of probing for features, options etc.  It should be the first
 * file included in your .cc file.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ftw_pnc_ofdm_pilot_cc.h>
#include <gr_io_signature.h>
#include <iostream>
#include <stdexcept>
#include <string.h>

/*
 * Create a new instance of ftw_pnc_ofdm_pilot_cc and return
 * a boost shared_ptr.  This is effectively the public constructor.
 */
ftw_pnc_ofdm_pilot_cc_sptr 
ftw_make_pnc_ofdm_pilot_cc (int tones, int user) //////////////////////modified by taotao
{
  return ftw_pnc_ofdm_pilot_cc_sptr (new ftw_pnc_ofdm_pilot_cc (tones, user));    //////////////////////modified by taotao
}

/*
 * Specify constraints on number of input and output streams.
 * This info is used to construct the input and output signatures
 * (2nd & 3rd args to gr_block's constructor).  The input and
 * output signatures are used by the runtime system to
 * check that a valid number and type of inputs and outputs
 * are connected to this block.  In this case, we accept
 * only 1 input and 1 output.
 */
static const int MIN_IN = 1;	// mininum number of input streams
static const int MAX_IN = 1;	// maximum number of input streams
static const int MIN_OUT = 1;	// minimum number of output streams
static const int MAX_OUT = 1;	// maximum number of output streams

/*
 * The private constructor
 */
ftw_pnc_ofdm_pilot_cc::ftw_pnc_ofdm_pilot_cc (int tones, int user)                //////////////////////modified by taotao
  : gr_block ("ofdm_pilot_cc",
	      gr_make_io_signature (MIN_IN, MAX_IN, tones * sizeof (gr_complex)),
	      gr_make_io_signature (MIN_OUT, MAX_OUT, (5 + tones) * sizeof (gr_complex))),
	      d_tones(tones),
              d_user(user), i(0), offset(0)                                                //////////////////////modified by taotao
{
   
}

/*
 * Our virtual destructor.
 */
ftw_pnc_ofdm_pilot_cc::~ftw_pnc_ofdm_pilot_cc ()
{
  // nothing else required in this example
}



int 
ftw_pnc_ofdm_pilot_cc::general_work (int noutput_items,
			       gr_vector_int &ninput_items,
			       gr_vector_const_void_star &input_items,
			       gr_vector_void_star &output_items)
{
  const gr_complex *in = (const gr_complex *) input_items[0];
  gr_complex *out = (gr_complex *) output_items[0]; 
  

/*  static gr_complex polarity[127] = 
    { 1, 1, 1, 1,-1,-1,-1, 1,-1,-1,-1,-1, 1, 1,-1, 1,
     -1,-1, 1, 1,-1, 1, 1,-1, 1, 1, 1, 1, 1, 1,-1, 1,
      1, 1,-1, 1, 1,-1,-1, 1, 1, 1,-1, 1,-1,-1,-1, 1,
     -1, 1,-1,-1, 1,-1,-1, 1, 1, 1, 1, 1,-1,-1, 1, 1,
     -1,-1, 1,-1, 1,-1, 1, 1,-1,-1,-1, 1, 1,-1,-1,-1,
     -1, 1,-1,-1, 1,-1, 1, 1, 1, 1,-1, 1,-1, 1,-1, 1,
     -1,-1,-1,-1,-1, 1,-1, 1, 1,-1, 1,-1, 1, 1, 1,-1,
     -1, 1,-1,-1,-1, 1, 1, 1,-1,-1,-1,-1,-1,-1,-1 };
*/  
// modified by taotao
/*   static gr_complex polarity[127] = 
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
*/

  int count = 0;
  /*
   * Sanity check
   */
  if(d_tones != 48) 
    throw std::invalid_argument("ftw_pnc_ofdm_pilot_cc: IEEE 802.11p requires 48 subcarriers\n");

//*************************************************added by taotao
  gr_complex polarity[4]={1,0,1,0};

  if(d_user != 1)         // for user two 
     polarity={0,1,0,1};

  if(d_user == 0)
     polarity={1,1,1,1};  // do not use polarity, but fill all pilots

//*************************************************    

  while (count < noutput_items) {
	unsigned int out_offset = count * (d_tones + 5);
	unsigned int in_offset = count * d_tones;
	// unsigned int pilot_offset = (count + offset) % 127;     modified by taotao
        

    // Subcarriers -26 to -22
    for(i = 0 ; i <= 4 ; i++)
  	  out[i + out_offset] = in[i + in_offset];
	
    // Pilot 1 (-21)
    out[5 + out_offset] = polarity[0];                           //----------------------------------------------------------


    // Subcarriers -20 to -8
    for(i = 6 ; i <= 18; i++)
  	  out[i + out_offset] = in[i - 1 + in_offset];

    // Pilot 2 (-7)
    out[19 + ((count)*(d_tones + 5))] = polarity[1];             //----------------------------------------------------------

	
    // Subcarriers -6 to -1
    for(i = 20 ; i <= 25 ; i++)
  	  out[i + out_offset] = in[i - 2 + in_offset];

    // DC (always zero)
    out[26 + out_offset] = 0;

    // Subcarriers 1 to 6
    for(i = 27 ; i <= 32 ; i++)
      out[i + out_offset] = in[i - 3 + in_offset];

    // Pilot 3 (7)
    out[33 + out_offset] = polarity[2];                           //----------------------------------------------------------


    // Subcarriers 8 to 20
    for(i = 34 ; i <= 46 ; i++)
      out[i + out_offset] = in[i - 4 + in_offset];

    // Pilot 4 (21)
    // out[47 + out_offset] = -polarity[pilot_offset];            //----------------------------------------------------------
    out[47 + out_offset] = polarity[3];  
  
    //Subcarriers 22 to 26
    for(i = 48 ; i <= 52; i++)
  	  out[i + out_offset] = in[i - 5 + in_offset];	
  	
    count++;
  }
  offset += count;

  // Tell runtime system how many input items we consumed on
  // each input stream.
  consume_each (noutput_items);
  // Tell runtime system how many output items we produced.
  return noutput_items;
}

