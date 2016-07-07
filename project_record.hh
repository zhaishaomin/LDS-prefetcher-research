/*****today is 2016/7/01 now is pm 23:03
I want to record my step of this project, computer architecture research!

First,  I have done the most of work about encoding code. and the frame 
        of LDSPrefetcher class has come into being.

Second, because I choose GEM5 as the platform to evaluate the algorithm
        occured in my mind. Although I have read and analysed the 
        resource code before, but it's still a tough work to implement 
        my method into the original prototype.

Third,  till now I have understood the process of cache system, but I 
        still need to pay attention to the problem where I should get
        the prefetched data just to mimic the hehavior of real cache 
        sysytem as identical as possible in term of cycle level.

fourth, how can i launch a LDSPrefetch in the getNEXTMSHR() functon of 
        cache class.

        such as followings

    origin: // If we have a miss queue slot, we can try a prefetch
        PacketPtr pkt = prefetcher->getPacket();
        if (pkt) {
            Addr pf_addr = blockAlign(pkt->getAddr());
            if (!tags->findBlock(pf_addr, pkt->isSecure()) &&
                !mshrQueue.findMatch(pf_addr, pkt->isSecure()) &&
                !writeBuffer.findMatch(pf_addr, pkt->isSecure())) {
                // Update statistic on number of prefetches issued
                // (hwpf_mshr_misses)
                assert(pkt->req->masterId() < system->maxMasters());
                mshr_misses[pkt->cmdToIndex()][pkt->req->masterId()]++;
                // Don't request bus, since we already have it
                return allocateMissBuffer(pkt, curTick(), false);
            } else {
                // free the request and packet
                delete pkt->req;
                delete pkt;
            }
        }   

   changed:// If we have a miss queue slot, we can try a prefetch from stride prefetcher or LDS prefetcher
        PacketPtr pkt1 = strideprefetcher->getPacket();
        //getpacket() now should only return a pkt and left pop_work for later process
        PacketPtr pkt2 = ldsprefetcher->getPacket();
        //getpacket() now should only return a pkt and left pop_work for later process
        
        //added code is:
        if (pkt2) {

            Addr pf_addr = blockAlign(pkt->getAddr());
            if (!tags->findBlock(pf_addr, pkt->isSecure()) &&
                !mshrQueue.findMatch(pf_addr, pkt->isSecure()) &&
                !writeBuffer.findMatch(pf_addr, pkt->isSecure())) {
                // Update statistic on number of prefetches issued
                // (hwpf_mshr_misses)
                assert(pkt->req->masterId() < system->maxMasters());
                mshr_misses[pkt->cmdToIndex()][pkt->req->masterId()]++;
                ldsprefetcher->popPacket();
                // Don't request bus, since we already have it
                return allocateMissBuffer(pkt, curTick(), false);
                
            } 
        }else if(pkt1) {

            Addr pf_addr = blockAlign(pkt->getAddr());
            if (!tags->findBlock(pf_addr, pkt->isSecure()) &&
                !mshrQueue.findMatch(pf_addr, pkt->isSecure()) &&
                !writeBuffer.findMatch(pf_addr, pkt->isSecure())) {
                // Update statistic on number of prefetches issued
                // (hwpf_mshr_misses)
                assert(pkt->req->masterId() < system->maxMasters());
                mshr_misses[pkt->cmdToIndex()][pkt->req->masterId()]++;
                strideprefetcher->popPacket()
                // Don't request bus, since we already have it
                return allocateMissBuffer(pkt, curTick(), false);

              }
        }else{

            // free the request and packet
                delete pkt->req;
                delete pkt;

        }



/********today is 2016/7/2, now is at pm 23:35
Fifth  today I find that my LDSPrefetcher prepare a prefetch by using TLB, 
       in which case I should take cycle-occupied features into timing model.
       And I need to figure out how the TLB module is called during data 
       access, pipelined or other ways.

       I also need to work out the effection to additional access to data cache when
       prepare pointer infomations. that is where should I change in the cache_impl.cc
       to support additional access to data without bring cycle mistakes to data access
       stream.

       Having read the source code, I find it difficult to change the program to mimics the
       accurate cycle hehavior of cache access.
  
/***********today is 2016/7/5 pm 18:11
 *         Mainly about interface between LDSP and DTLB
 * 
 * there are some qusetiones for accessing DTLB
 * 1, when can LDS-Prefetcher use the DTLB to generate a lds prefech?  
 *    answer: may be in the idle cycle of DTLB 
 * 
 * 2, The access of priority between demand L1 access and LDSP to DTLB?
 *    answer:
 * 
 * 3, what if a LDS_prefetcher is in a more critical path than demand access 
 *    while LSQunit recently dominate the DTLB? we need to raise up the priority 
 *    of LDS Prefetchcompared to demand access. Now I think I need a counter to 
 *    track the past access history to determine which one can use the DTLB.
 * 
 * 4, we should take L2 request queue condition into account of determination 
 *    in terms of urgency of launching a LDS Prefetch.
 * 
 *    eg: it will be less useful to req a DTLB access for a lds prefetch in a upgrade mode
 *        if L2 request queue is recently full.
 * 
 * 5, IS it necessary to filter the cache line request, which contians useless pointer data for future prefetch.
 *    answer: It depend on the accuracy of direction predictor.
 *            If the recent accuracy of the LDSP is higher than a therhold, just turn on filterring; otherwise turn off it!.
 * 
 * 
 *  I think I need to design a lab to observe the element locations of a big structure, and try to find out the regular pattern.
 * 
 
 Solution! 
 /*********2016/7/5  pm 22:34
  * There will be a list of interfaces between LDS_prefetcher and original GEM5
  *
  *
  * interface 1, static information such as inst type, src reg id, imm value, dest reg id.
  *              The prolem is how can I get imm value from dyn_inst.
  *              solution 1: imm=inst->paddr-inst->src_reg_value.
  *              solution 2: create a imm filed in the staticinst class.
  * 
  * interface 2, when can I launch a addr translation for pointer info for LDS_prefetcher pattern using DTLB.
  *              case 1: There are enough idle cycles for LDSP to do a few of addr translations.
  *              case 2: There are few of idle cycles for LDSP to do any addr translations but the load-to-use 
  *                      of LDS data is critical, so we need to upgrade the priority of pointer addr translation.
  * 
  * interface 3, between prefetcher and L1Data cache ,which is used to train pattern table
  *              case 1: the active pointer infos can be extrated from L1 data cache directly, L1 cache hit.
  *                      ****This case can be implemented in satisfied block of recvTimingreq(  ) of cache class.
  *                case 1.1 :the pointer infos can be get from the same cache line via data access of the same linked structure.
  *                
  *                case 1.2 :the pointer infos only should be acquiried via specific data access to the cache line.
  *                
  *              case 2: the active pointer infos is evicted from L1 data cache before, so L1 cache miss;
  *                      but once missed data return from lower level cache, data will be redirected to pattern table(LDSP).
  *                      ****This case should be handled after handfill action or simutaneously in 
  *                          recvTimingresp(  ) of cache class.
  * 
  * interface 4, between prefetcher and commit state, which is provide commited load infos for LDSP to
  *              update direction predictor and pattern table.    
  * 
  *              solution: add a pointer of LDSP to commit state. When some loads are commited from ROB,
  *                        it's the right time to use the loads to update LDSP.
  * 
  *              Eg:  insert the code below into line 1324 of commit_impl.hh in commithead(  ).
  * 
  *                    // If this was a load, call ldsp->update_pt(  ).
  *                        if (head_inst->isLoad())        
  *                             ldsp->update_pt(head_inst);
  * 
  * interface 5, between MSHR ,L2 request queue condition and prefetcher.
  *              solution: try to figure out what action should occurred in MSHR and Prefetch.
  *                        need to figure out raletionship between MSHR and stride_prefetcher.
  * 
  * end by PM 23:36
    


/********************* today is 2016/7/7 , now PM 22:39******************************/
About interface 2&3

Note!  

The interface between data cache and LDS_prefetcher should be optimized like followings

Case 1: the data cache is plpelined. eg: First DTLB access and tag access, then data access.
        In this case, the LDSP can issue a pointer infos access to data cache after a DTLB access requiried by LDSP.
Case 2: the data cache is one cycle latency.
        In this case, the LDSP can issue a pointer infos access and DTLB access.
(eg: intel core i7 four cycles latencies, pipelined)
        
