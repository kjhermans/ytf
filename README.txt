Ytf (Yours Truly Format) is a specification for several
data serialization formats that can be cross translated
from any format to any format for their own purposes.

The format has the complexity of JSON, that is:

- Datatypes null, boolean, float, int, binary string,
  array and hashtable.
- Infinite depth through compound types.

Purposes include:

- JSON is a standard format which can be used by
  other tooling.
- Flat YTF is a format that is more easily parseable
  but still human readable.
- Binary YTF is a format based on completely meaningful
  encoding, that can always be parsed in an error-free way.
