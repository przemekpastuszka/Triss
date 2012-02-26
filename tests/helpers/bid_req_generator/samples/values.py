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
        'os': parse('samples/operating_systems'),
        'country': parse('samples/countries'),
        'content': parse('samples/content'),
        'browser': parse('samples/browser_ids'),
        'resolution': parse('samples/resolutions'),
        'language': parse('samples/languages'),
        'sex': parse('samples/sex'),
        'job': parse('samples/jobs'),
        'name': parse('samples/names'),
        'music': parse('samples/music'),
        'hobby': parse('samples/hobbies'),
        'movie_genre': parse('samples/movie_genres'),
        'religion': parse('samples/religions')
}
