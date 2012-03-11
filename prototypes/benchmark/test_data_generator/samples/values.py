# Sample-content file should consis of values
# for chosen parameter - each in new line
import re
import os
# gets the prefix of the path where script residues
# to help open() function open files with relative path
PATH_PREFIX = os.path.split(os.path.abspath(__file__))[0]


def parse(filename):
    """ parse sample-content files """
    res = []
    for line in open(os.path.join(PATH_PREFIX, filename), 'r'):
        res.append(re.escape(line.strip()))
    return res

sample_values = {
        #'empty': [''],
        'age': [str(a) for a in range(14, 100)],
        'os': parse('operating_systems'),
        'country': parse('countries'),
        'content': parse('content'),
        'browser': parse('browser_ids'),
        'resolution': parse('resolutions'),
        'language': parse('languages'),
        'sex': parse('sex'),
        'job': parse('jobs'),
        'name': parse('names'),
        'music': parse('music'),
        'hobby': parse('hobbies'),
        'movie_genre': parse('movie_genres'),
        'religion': parse('religions')
}
