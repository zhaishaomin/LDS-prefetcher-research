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
        PacketPtr pkt1 = strideprefetcher->getPacket();//getpacket() now should only return a pkt and left pop_work for later process
        PacketPtr pkt2 = ldsprefetcher->getPacket();//getpacket() now should only return a pkt and left pop_work for later process
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
 * there are some qusetiones for accessing DTLB
 * 1, when can LDS-Prefetcher use the DTLB to generate a lds prefech?  
 *    answer: may be in the idle cycle of DTLB 
 * 
 * 2, The access of priority between demand L1 access and LDSP to DTLB?
 *    answer:
 * 
 * 3, what if a LDS_prefetcher is in a more critical path than demand access while LSQunit recently dominate the DTLB? we need to raise up the priority of LDS Prefetchcompared 
 *    to demand access. Now I think I need a counter to track the past access history to determine which one can use the DTLB.
 * 
 * 4, we should take L2 request queue condition into account of determination in terms of urgency of launching a LDS Prefetch.
 *    eg: it will be less useful to req a DTLB access for a lds prefetch in a upgrade mode if L2 request queue is recently full.
 * 
 * 5, IS it necessary to filter the cache line request, which contians useless pointer data for future prefetch.
 *    answer: It depend on the accuracy of direction predictor.
 *            If the recent accuracy of the LDSP is higher than a therhold, just turn on filterring; otherwise turn off it!.
 * 
 * 
 *  I think I need to design a lab to observe the element locations of  abig structure, and try to find out the regular pattern.
 * 
 
  
