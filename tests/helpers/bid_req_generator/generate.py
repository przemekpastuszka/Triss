#!/usr/bin/env python
# Simple script generating test data - the csv-represented-
# bid-requests batch file. Sample content file consist of
# dict in format: "param_name":[list_of_sample_values].
import random
import sys
import argparse
from samples.values import sample_values


class BidRequestGenerator:
    def __init__(self, seed):
        random.seed(seed)

    def gen_bid_req(self, params):
        bid = []
        for pname, ptype, max_len in params:
            # let 5% of fields be empty
            if random.randint(1, 20) == 1:
                rand_param_value = ''
            else:
                # special case for list value
                #if type(sample_values[p][0]) == type([]):
                if ptype == 'list':
                    vals = sample_values[pname]
                    nelem = random.randint(1, max_len)
                    vals = random.sample(vals, nelem)
                    # convert it to comma delimited string
                    rand_param_value = ','.join(vals)
                else:
                    rand_param_value = random.choice(sample_values[pname])
            bid.append(str(rand_param_value))
        return ';'.join(bid) + '\n'

    def gen_bid_req_batch_file(self, fd, params, nrequests):
        try:
            for i in xrange(nrequests):
                bid_req = self.gen_bid_req(params)
                fd.write(bid_req)
        finally:
            fd.close()


def three_colon_separated_vals(string):
    """ param argument parser """
    try:
        pname, ptype, max_len = string.split(':')
        if pname not in sample_values.keys():
            msg = 'Invalid param name %s' % pname
            raise argparse.ArgumentTypeError(msg)
        if ptype == 'list':
            max_len = int(max_len)
            if max_len < 0 or max_len > len(sample_values[pname]):
                max_len = len(sample_values[pname])
        elif ptype != '':
            msg = 'Invalid param type %s\n' % ptype
            msg += 'Should be "list" or empty'
            raise argparse.ArgumentTypeError(msg)
        return [pname, ptype, max_len]
    except ValueError, msg:
        raise argparse.ArgumentTypeError(msg)


class ListParamNames(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        print "Param names (total %s):" % len(sample_values.keys())
        for pname in sorted(sample_values.keys()):
            print pname
        sys.exit()

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Generate bid request batch file')
    parser.add_argument('-s', '--seed', metavar='S', type=int, nargs='?',
                        default=1, help='a seed for generator')
    parser.add_argument('-o', '--outfile', metavar='filename',
                        type=argparse.FileType('w'), nargs='?',
                        default=sys.stdout,
                        help='name of file - where the result will be saved')
    parser.add_argument('-n', '--ndocs', metavar='N', type=int, nargs='?',
                        default=1,
                        help='number of documents you want to generate')
    parser.add_argument('-p', '--params', metavar='pname:ptype:max_len',
                        type=three_colon_separated_vals, nargs='+',
                        required=True,
                        help='each document will be build from given\
                                parameters (in given order)')
    parser.add_argument('-l', '--list-param-names',
                        action=ListParamNames, nargs=0,
                        help='lists available parameter names and exits')
    args = parser.parse_args()
    a = BidRequestGenerator(args.seed)
    a.gen_bid_req_batch_file(args.outfile, args.params, args.ndocs)
