# Sample content file format:
# { "param_name": [sample1, sample2, ...], ... }
from countries import countries
from content import content
sample_values = {
        "country": countries,
        "age": range(14, 100),
        "content": content.values()
}
