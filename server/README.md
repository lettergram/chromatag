ChromaTags Server
====

See chromaTag description in above directory.

This implmentation provides a server that another person can connect to:

Connect via: IP Address + portnumber (MY_PORT variable, default 9499)

Sends RT Matrix in return:

> [R_00,R10,R20,T0,R01,R11,R21,T1,R02,R12,R22,T2]\n

### To Build:  
1. `chromatags/chromatagServer $ make`  
2. `mkdir build && cd build`  
3. `cmake .. && make`  

####Invoke Realtime ServerTracking

$ cd build

$ ./chromatagServer