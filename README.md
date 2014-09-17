image-hash
==========

ImageHash server with Vantage Point Tree data structure for fast hamming distance calculation of image phashes

Vantage Point Tree
==================
Or VPTree is a data structure that allows fast hamming distance calculation. This repo contains a python script which acts as a server and takes a command line argument for the data file on startup, which contains values of interest (domain) one on each line. The server listens on port 8880 and accepts a TCP socket connection to query. Given a value via the socket it  quickly returns top matches that fall within the specified threshold (`rad` variable in `vp_searchtxt.py`) for the Hamming Distance.

Future Iteration
================
It would be ideal to be able to dynamically add value the the VPTree without having to restart the server.
