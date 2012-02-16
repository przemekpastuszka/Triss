#!/usr/bin/env python
# Simple script generating test data - the csv-represented-
# bid-requests batch file. Source content file should consist
# of name, type of field and corresponding sample data in json format.
import random
from samples.content import sample_content

class BidRequestGenerator:
    def __init__(self, seed):
        random.seed(seed)
    def gen_bid_req(self, params):
        bid = []
        for p in params:
            """ let 5% of fields be empty"""
            if random.randint(1,20) == 1:
                random_param_content = ''
            else:
                random_param_content = random.choice(sample_content[p])
            bid.append(str(random_param_content))
        return ';'.join(bid) + '\n'
    def gen_bid_req_batch_file(self, filename, params, nrequests):
        try:
            f = open(filename, 'a')
            for i in xrange(nrequests):
                bid_req = self.gen_bid_req(params)
                f.write(bid_req)
        finally:
            f.close()

if __name__ == '__main__':
    import sys
    if len(sys.argv) < 4:
        print "Usage: ./generate.py <seed> <outfile> <ndocs> <par1> <par2> ."
        sys.exit(1)
    seed, outfile, ndocs = sys.argv[1:4]
    params = sys.argv[4:]
    a = BidRequestGenerator(seed)
    a.gen_bid_req_batch_file(outfile, params, int(ndocs))
