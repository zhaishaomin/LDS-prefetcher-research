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

#ifndef __MEM_CACHE_LDSP_PATTERN_TABLE_HH__
#define __MEM_CACHE_LDSP_PATTERN_TABLE_HH__

#include  <list>
#include  <vector>

class Pattern_Table
{ 
      protected:
               const int pattern_table_set;

               const int pt_maxnumpointers;

               struct  PointerField{
                       // dest reg of the potential pointer
                       int dest_reg;

                       // value of the pointer,also the addr of the next node
                       int addr;
                   
                       // offset compared to the first addr in the structure
                       int offset;

                       // prefetch or not ,used to save info for direction predictor or
                       // guide prefetcher to decide whether to prefetch or not
                       bool  pref;
                       
                       // Is the addr valid or not
                       bool addr_valid;
                       
                       // valid or not
                       bool valid;
                       
                       // the addr is been translating to paddr
                       bool translating;
                    
                       // the pref has been issued
                       bool pref_issued;

                       // the pref has been done!
                       bool pref_done; 
                };

               struct PT_entry{

                      // the entry is valid?
                      bool valid;

                      // current base src reg of the load ,used to identify 
                      // whether the incoming inst is a load accessing something within the node
                      int src_base_reg ;

                      enum PTE_status {
                          idle;
                          busy;
                          meet_mispred;
                      };
                      PTE_status  pte_status;

                      // maxnum pointerfield 
                      PointerField  pf[_pt_pointerfield_maxnum];
                };
               
               std::vector<PT_entry>  pattern_table;
     private:
            TRB * trb;
            Dire_Pred * dp;
            lds_prefetcher * ldsp;
            ImplCPU *cpu;
            // used to allocate anew entry for the potential lds 
            void allocNewEntry();
            

     public:
            Pattern_Table(int _pattern_table_set, int _pt_maxnumpointers, TRB * _trb, Dire_Pred * _dp, lds_prefetcher * _ldsp );
            ~Pattern_Table() {};

            // insts after decode stage will access Pattern Table to update something in the corresponding entry
            // such as allocating a new entry, write the potential pointer and its dest reg and offset and so on.
            void DecodedInstAccess(DynInstPtr &inst);

            // when do prefetching, we might need to load the useful value of some pointer and its dest reg to go ahead to prefetch next one 
            void NewsFromL1D(DynInstPtr &inst);

            // pattern table should update the addr of useful pointer and do aggresive lds prefetches by using returned L2 data 
            void NewsFromL2(DynInstPtr &inst);
            
            // issue a pointer infos access to data cache after a DTLB access requiried by LDSP
            Packetptr get_ptr_infos();
            
            //calculate packet for future LDS prefetch
            PacketPtr calculatePref();
            
            // update physical addr if DTLB is finished!
            void updatePaddr();
            
            // if we need to access DTLB to translate the virtual addr to physical addr
            bool needAccessTLB();
            
            // if we need to translate vaadr to paddr via DTLB, go to DTLB 
            void AccessTLB();
            
            // if the pointer information is inside data cache
            bool PtrInsideDC();
            
            // if pointer is inside data cache, we need to go to data cache just to access data array.
            void AccessDC();
            
            // if we meet a branch mispredction while doing a surcusive prefetch, we should stop prefetching.
            void stopPref();
      
            // return if the cache block is still inside the data cache, according to pred mode, one (true)or two(false)
            bool PtrInsideDC(int addr, bool one_mode);
            
            // this method is used to capture the real pointer in runtime 
            void capPattern(DynInstPtr inst);

            // we need to stop prefetching data ahead because a branch mispred happened which means we might go to the wrong LDS path.
            void stopPref();

};  
#endif
