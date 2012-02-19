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
            # let 5% of fields be empty
            if random.randint(1, 20) == 1:
                rand_param_value = ''
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

    def gen_bid_req_batch_file(self, fd, params, nrequests):
        try:
            for i in xrange(nrequests):
                bid_req = self.gen_bid_req(params)
                fd.write(bid_req)
        finally:
            fd.close()

if __name__ == '__main__':
    import sys
    import argparse
    parser = argparse.ArgumentParser(
        description='Generate bid request batch file')
    parser.add_argument('-seed', metavar='S', type=int, nargs='?',
                        default=1, help='a seed for generator')
    parser.add_argument('-outfile', metavar='filename',
                        type=argparse.FileType('w'), nargs='?',
                        default=sys.stdout,
                        help='name of file - where the result will be saved')
    parser.add_argument('-ndocs', metavar='N', type=int, nargs='?',
                        default=1,
                        help='number of documents you want to generate')
    parser.add_argument('-params', metavar='param', type=str, nargs='+',
                        required=True,
                        help='each document will be build from given\
                                parameters (in given order)')
    args = parser.parse_args()
    a = BidRequestGenerator(args.seed)
    a.gen_bid_req_batch_file(args.outfile, args.params, args.ndocs)
