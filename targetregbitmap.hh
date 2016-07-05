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

#ifndef __MEM_CACHE_LDSP_TARGETREGBITMAP_HH__
#define __MEM_CACHE_LDSP_TARGETREGBITMAP_HH__

#include  <list>
#include  <vector>

class Target_Reg_Bitmap 
{
    public:
           /* a interface to do something about TRB depending on what kinds of insts we are process */
           void access_TRB(DynInstPtr &inst);
           /* we need to keep a checkpoint of current TRB if encountering a branch to enable rollbacking from mispred br */
           void EnqTRB_ToBQ(DynInstPtr &inst);
           /* we need to kick off head  checkpoint of previous TRB since having solving br or meeting a branch mispred */
           void Deq_BQ(DynInstPtr &inst);
           
    private:
           /* a load will set a bit of trb indexed by the dest_reg of the load */
           void Ld_Set_TRB(int  dest_reg_id);
           /* a non_load inst will reset a bit of trb indexed by the dest_reg of non_load inst */ 
           void Nld_Reset_TRB(int dest_reg_id);
           /* Move insts will move the value from previous load dest reg to current dest reg */ 
           void Move_Set_TRB(int src_reg_id, int dest_reg_id);
           typedef std::bitset<32> _trb;
           std::bitset<32> _TRB;
           std::list<_trb>  _BQ;
           
           //stats
           stats::scalar  BQFullcycles;
           stats::scalar  BQUsed;
};
#endif
