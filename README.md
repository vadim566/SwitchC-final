## Basic Switch Emulator

The program simulate a switch that recive a packet file to each port ,while in this
version there are 4 in ports, after the switch emulator will read the 4 input ports
it will build a routing table and will deside in which port each packet from input
ports will route to. each packet have a QOS element /priority element that will 
decide in which queue to put the packet in the output port.
In addition the the emulator will check each packet if the packet is not corputed
, it will happend threw XOR on all packet element except the checkSum frame.
after that the emulator will send all the packets by the Time Line (TIME FIFO) and by 
the QOS/priority elemt when the first to out will be these with 
priority 0 and X time after that priority 1 with X time, the product will be exported to files Port i.out .

## Running and Exeute
the emulator can be run from command line by the command EXAMPLE:C:\Users\David\source\repos\switch\Debug\switch.exe route.txt port1.in port2.in port3.in port4.in
while 
switch.exe - name of the EXE file
route.txt - logic of the routing table - 1st column:a= add ,d =delete , 2nd column:da elemnt/frame ,3rd column(exist only while cloumn 1==a) output port
porti.in- i=1 or 2 or 3 or 4 the input file that simulate packet in the in ports 
each packet has frame of  [Time][Source Adress][Destination Adress][Priority][Data Length][Data(size of Data Length)][checksum]

after each use the emulator will free all the memory that was heap located for the emulator
*/
