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

#include"mem/cache/ldsp/pattern_table.hh"

void Pattern_Table::DecodedInstAccess(DynInstPtr &inst)
{
     if(inst is load)
     {
         if(pattern table is empty)
         { //now we need to allocate a new entry
           PT_entry  *temp_entry=new PT_entry;

           temp_entry->src_base_reg= inst->srcRegIdx(0);
       
           temp_entry->valid=true;

           pattern_table.push_back(*temp_entry);
       
           delete temp_entry;
         }   
         else //pattern table is not empty
         {
           // get the src_reg of load inst
           int src_regid=inst->srcRegIdx(0);
       
           list<PT_entry>::Iterator pt_iter;

           for(pt_iter=pattern_table.begin();pt_iter!=pattern_table.begin();pt_iter++)
           {
               if(src_regid==pt_iter->src_base_reg)
               {
                 for(int i=0;i<pt_maxnumpointers;i++)
                    {
                        PointerField * p=pt_iter->pointer[i];
                        if(p->valid==false)
                        {
                          p->valid=true;
                          //get the dest reg of load
                          p->dest_reg=inst->destRegIdx(0);
                          p->offset=inst->effaddr-inst->imm;
                          return;
                        }
                    }
               }
               else//src_regid!=pt_iter->src_base_reg
               {
                   for(int j=0;j<pt_maxnumpointers;j++)
                   {
                      PointerField * p=pt_iter->pointer[i];
                      if(p->valid==true&&p->dest_reg==src_regid)
                       {
                           p->v_pointer=true;
                       }

                   }
               }//else
           }//for
       }//else
    }//inst is load
    else//inst is not a load
    {   
       int src_regid=inst->srcRegIdx(0);
       
       list<PT_entry>::Iterator pt_iter;

       for(pt_iter=pattern_table.begin();pt_iter!=pattern_table.begin();pt_iter++)
       {
          for(int i=0;i<pt_maxnumpointers;i++)
          {
             PointerField * p=pt_iter->pointer[i];
             if(inst->destRegIdx(0)==p->dest_reg)
              {
                  p[i]->valid=false;
                  return;
              }
          }
      }
    }//inst isn't a load !
}


void Pattern_Table::NewsFromL1D(DynInstPtr &inst)
{
   
}

void Pattern_Table::NewsFromL2(DynInstPtr &inst)
{


}

PacketPtr  Pattern_Table::get_ptr_infos()
{  
     if（!pointer->addr_valid&&pointer->pref）
     {
          PacketPtr ptr_access=new packet();
          pointer->pending=1;
         // pointer->accessIsssued=1;
     }
     return ptr_access;
}

PacketPtr Pattern_Table::calculatePref()
{
     list<PT_entry>::Iterator pt_iter;

     for(pt_iter=pattern_table.begin();pt_iter!=pattern_table.begin();pt_iter++
     {
          if(pt_iter->PT_entry_valid)
          for(int i=0;i<pt_maxnumpointers;i++)
          {
             PointerField * p=pt_iter->pointer[i];
             //we should issue a LDS prefetch 
             //if 1. this pointer filed is valid;
             //   2. the physical addr is ready;
             //   3. this is the first time to issue a pref.
             //   4. the pointer is predicted to prefetch a LDS. 
             if(p->valid&&p->addr_valid&&(!p->pref_issued)&&p->pref)
              {
                  p->pref_issued=ture;
                  
                  // here we should add some new metrics to original packet.
                  Packet  * pkt=new Packet();
                  return  pkt;
              }
     }
}


void Pattern_table::updatePaddr(PointerField  * ptr, int paddr, PT_entry * pte)
{
     
}
