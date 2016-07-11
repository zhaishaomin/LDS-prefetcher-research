/* Copyright (c) 2016 ZhaiShaoMin Limited
 * All rights reserved.
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Author:zhaishaomin
*/

#ifndef _MEM_CACHE_LDSP_DIRECTION_PREDICTOR_HH_
#define _MEM_CACHE_LDSP_DIRECTION_PREDICTOR_HH_

class Direction_Predictor
{
      private:
          
             struct dp_entry{
                    //pref or not 
                    bool   pref1;
                    //pref or not
                    bool   pref2;
                    //pref bit used for caculate prefetch accuracy
                    bool   prefed;
                    //pollution of not 
                    bool   pollution;
            };

            // pred_or_not , prefed bit and pollution bit array 
            std::vector<dp_entry> dp_array;

            // number of dp_entry in the dp_table
            unsigned  Dire_PredSize;

            // mask used to caculate index for dp_table
            unsigned  Dire_PredMask;
  
            // cache block size of log2(cacheblksize) , in bits
            unsigned  cache_block_size;
          
            //@ valueable to do index exploration

            // calculate index of two dp_entry mode
            unsigned  calcu2DP_Eidx(unsigned addr);

            // calculate index of one dp_entry mode
            unsigned  calcu1DP_Eidx(unsigned addr);

            // return the dp_entry result of prediction
            dp_entry  getPrediction(unsigned addr );

            // update the entry indexed by the addr
            void  updateDP_array(unsigned addr, dp_entry _dp_entry);
           
            // assign dp_entry values to a new entry
            void  assign_DP_entry(dp_entry &new_entry, dp_entry old_entry); 
            
            //stats
            Stats::scalar  DP_TotalPred;
            Stats::scalar  DP_GoodPred;
            Stats::Scalar  DP_Mispred;
            Stats::scalar  Dp_Pollution;

      public:
            Direction_Predictor(unsigned  Dire_PredSize， unsigned  cache_block_size);
            ~Direction_Predictor() {};
       
            // update the DP_array according to pred mode two entry or one entry
            void  update_array(unsigned addr, dp_entry dp_entry[2], bool two_mode);

            // return the dp_entry result of prediction acording to the pred mode
            void  getPrediction(unsigned addr, dp_entry &dp_entry[2], bool two_mode); 
};
#endif
