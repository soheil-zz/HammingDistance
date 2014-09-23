image-hash
==========

ImageHash server for hamming distance calculation of image phashes.

Compile and Run
===============
`make` will create the executable in the current working directory. Running the following will start the server:

```
./server [PORT] [DATA_FILE]
```


Querying
========
Make a TCP connection to above port on the client. The input format is 

```
VALUE_TO_FIND_HAMMING_DISTANCE_FOR,THRESHOLD,COUNT
```

* `VALUE_TO_FIND_HAMMING_DISTANCE_FOR` is just the value that you'd like to search the corpse of data for.
* `THRESHOLD` acceptable match threshold value, values with this many or less differing bits are returned.
* `COUNT` server will only return this many matches, there is no ordering.