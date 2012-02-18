#!/usr/bin/env python
# Simple script generating test data - the csv-represented-
# bid-requests batch file. Sample content file consist of
# dict in format: "param_name":[list_of_sample_values].
import random
from samples.values import sample_values


class BidRequestGenerator:
    def __init__(self, seed):
        random.seed(seed)

    def gen_bid_req(self, params):
        bid = []
        for p in params:
            """ let 5% of fields be empty"""
            if random.randint(1, 20) == 1:
                rand_param_content = ''
            else:
                # special case for list value
                if type(sample_values[p][0]) == type([]):
                    tmp = random.choice(sample_values[p])
                    nelem = random.randint(1, len(tmp))
                    tmp = random.sample(tmp, nelem)
                    # convert it to comma delimited string
                    rand_param_value = ','.join(tmp)
                else:
                    rand_param_value = random.choice(sample_values[p])
            bid.append(str(rand_param_value))
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
