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

#include"mem/cache/ldsp/ldsprefetcher.hh"

LDSPrefetcher(const LDSPrefetcherParams *p)
{
     // init child class
     _trb(p->bitmap_size);
     _pattern_table(p->pattern_table_set, p->pt_maxnumpointers, &_trb, &_dire_pred, this);
     __dire_pred(p->dire_predsize,p->cache_block_size);
     //set up child class
     _trb.setPT(&_pattern_table);
     
}

std::string 
LDSPrefetcher::name()
{
     return cpu->name() + ".LDSPrefetcher";
}

void  
LDSPrefetcher::Regstats()
{
     LDSpref_senttoL2
        .name(name() + ".pref_senttoL2")
        .desc("Number of prefs sent to L2 cache")
        .prereq(ldspref_senttoL2);

     LDSpref_useful
        .name(name() + ".pref_useful")
        .desc("Number of prefs useful")
        .prereq(LDSpref_useful);

     LDSpref_LateToUse
        .name(name() + ".pref_LateToUse")
        .desc("Number of prefs late To Use")
        .prereq(LDSpref_LateToUse);

     LDSDemandMissCausedByPref
        .name(name() + ".DemandMissCausedByPref")
        .desc("Number of DemandMiss Caused By Prefetch")
        .prereq(LDSDemandMissCausedByPref);

     LDSTotalDemandMiss
        .name(name() + ".TotalDemandMiss")
        .desc("Number of Total DemandMiss")
        .prereq(LDSTotalDemandMiss);

     LDSPref_BQFullcycles
        .name(name() + ".Pref_BQFullcycles")
        .desc("Number of cycles that BQ is Full!")
        .prereq(LDSPref_BQFullcycles);

     LDSPref_Squashcycles
        .name(name() + ".pref_Squashcycles")
        .desc("Number  of cycles that lds_pref is Squashed!")
        .prereq(LDSPref_Squashcycles);
     
}

void  
LDSPrefetcher::drainSanityCheck() const
{
    /*
      assert(_pattern_table->pt_line.empty());
      assert(_bq->_BQ.empty());
    */  
  
}

void   
LDSPrefetcher::isdrained() const
{
     
}

void  
LDSPrefetcher::takeoverfrom()
{ 
      
}

void
LDSPrefetcher::needAccessTLB()
{ 
   pt->needAccessTLB();
}


void
LDSPrefetcher::AccessTLB()
{
   pt->AccessTLB();
}
