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
        res.append(re.escape(re.subn(';','',line.strip())[0]))
    return res

sample_values = {
        #'empty': [''],
        'age': ("num", [str(a) for a in range(14, 100)]),
        'os': ("str", parse('operating_systems')),
        'country': ("str", parse('countries')),
        'content': ("str", parse('content')),
        'browser': ("str", parse('browser_ids')),
        'resolution': ("str", parse('resolutions')),
        'language': ("str", parse('languages')),
        'sex': ("str", parse('sex')),
        'job': ("str", parse('jobs')),
        'name': ("str", parse('names')),
        'music': ("str", parse('music')),
        'hobby': ("str", parse('hobbies')),
        'movie_genre': ("str", parse('movie_genres')),
        'religion': ("str", parse('religions'))
}
