Hamming Distance Server in C++
==============================

This Hamming Distance server can be used to find matching values in a corpse of data based on hamming distance closeness.


Compile and Run
---------------
`make` will create the executable in the current working directory. Running the following will start the server:

```
./server [PORT] [DATA_FILE]
```

* `PORT` port to start the server on.
* `DATA_FILE` one value per line, values must be 64-bit integers.

Querying
--------
Make a TCP connection to above port on the client. The input format is 

```
VALUE_TO_FIND_HAMMING_DISTANCE_FOR,THRESHOLD,COUNT
```

* `VALUE_TO_FIND_HAMMING_DISTANCE_FOR` is just the value that you'd like to search the corpse of data for.
* `THRESHOLD` acceptable match threshold value, values with this many or less differing bits are returned.
* `COUNT` server will only return this many matches, there is no ordering.


Example
-------
From the command line with server running:

```
echo '2736476346,20,10' | nc 10.23.1.24 11600
```