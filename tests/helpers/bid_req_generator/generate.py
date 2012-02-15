#!/usr/bin/env python
# Simple script generating test data - the csv-represented-
# bid-requests batch file. Source content file should consist
# of name, type of field and corresponding sample data in json format.
import json
import random

class BidRequestGenerator:
    def __init__(self, filename, seed):
        random.seed(seed)
        self.load_content(filename)
    def load_content(self,filename):
        self.bid_content = json.load(open(filename, 'r'))
    def gen_bid_req(self, params):
        bid = []
        for p in params:
            """ let 5% of fields be empty"""
            if random.randint(1,20) == 1:
                random_param_content = ''
            else:
                random_param_content = random.choice(self.bid_content[p])
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

a = BidRequestGenerator("content.json", 10)
a.gen_bid_req_batch_file('result1.csv', ['bar','foo','baz'], 100)
