# Sample-content file should consis of values
# for chosen parameter - each in new line
import re


def parse(filename):
    """ parse sample-content files """
    res = []
    for line in open(filename, 'r'):
        res.append(re.escape(line.strip()))
    return res

sample_values = {
        'empty': [''],
        'age': [str(a) for a in range(14, 100)],
        'country': parse('samples/countries'),
        'content': parse('samples/content'),
        'browser': parse('samples/browser_ids')
}
