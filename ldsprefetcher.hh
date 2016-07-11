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

#ifndef __MEM_CACHE_LDSP_LDSPREFETCH_HH__
#define __MEM_CACHE_LDSP_LDSPREFETCH_HH__

#include  <list>
#include "params/LdsPrefetcher.hh"

class LDSPrefetcher
{
    protected:
    struct DeferredPacket {
        Tick tick;
        PacketPtr pkt;
        DeferredPacket(Tick t, PacketPtr p) : tick(t), pkt(p) {}
    };

    std::list<DeferredPacket> pfq;

    public:
      enum LDS_status{
        Active,
        Inactive
    };
 
    private:
    /* LDS_prefetcher status*/
      LDS_status  _status;
    /* pointer to L2 cache.*/
      L2cache  *l2cache;
    /* entity of the pattern table. */
      Pattern_table  _pattern_table;
    /* entity of the pattern table. */
      TRB   _trb;
    /* entity of the bitmap */
      BQ    _bq;
    /* entity of target_reg_bitmap TRB */
      Dire_Pred   _dire_pred;

    // STATS
    Stats::Scalar LDSpref_senttoL2;
    Stats::Scalar LDSpref_useful;
    Stats::Scalar LDSpref_LateToUse;
    Stats::Scalar LDSDemandMissCausedByPref;
    Stats::Scalar LDSTotalDemandMiss;
    Stats::Scalar LDSPref_BQFullcycles;
    Stats::Scalar LDSPref_Squashcycles;
    public:
      LDSPrefetcher(const LDSPrefetcherParams *p);
       ~LDSPrefetcher();
     
      std::string name() const;
     // void  setstats();
      void  setPattern_Table(Pattern_Table *pattern_table);
      void  setTRB(TRB *trb);
      void  setBQ(BQ * bq);
      void  setDirection_Predictor(Dire_Pred * dire_pred);

      void  drainsanitycheck() const;
      void  isdrained() const;
      void  takeoverfrom();
      void  squash(const InstSeqNum  &squash_seq_num,
                         ThreadID tid);
      void  dumpHistory();

    //commem for prefetchers
      Tick  notyfy(const PacketPtr &pkt);
      PacketPtr getPacket();
      void  Regstats();
      void  calculatePrefetch(const PacketPtr &pkt, std::vector<Addr> &addresses );
  };
#endif
      
