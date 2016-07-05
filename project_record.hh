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
  
