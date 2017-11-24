================
Syncronization 1
================

Write a program that uses semaphores to implement a readers/writers solution. Your program should:

    - be written in C and use the standard kernel IPC mechanisms (semget,semop,shmget etc)
    - be written as a single top level source file compiled as:  gcc –o myprog  myprog.c
    - take two arguments on the command line:   myprog NR NW  where NR,NW are each integers specifying the number of reader/writer processes respectively
    - use fork/exec to create the readers/writers

The shared memory segment should be 16k bytes in size, with all bytes initialized to 0x30

Reader:

.. code ::

  open a file (for append) named reader.N, where N is the reader number
  while (1) {
        for (i=0; i<16k; i++) {
                  read the next byte of the shared memory segment
                  write that byte to the file
                  flush the file
        }
        sleep a random number of seconds, between 0 and N inclusive
  }

Writer:

.. code ::

        while (1) {
	        for (i=0; i<16k; i++)
	        	shared memory segment[i] = N + 0x30;
        	sleep a random number of seconds between 0 and 2*N inclusive
        }

readers and writers should be mutually exclusive
multiple concurrent readers are allowed
writers have priority over readers
writers are mutually exclusive relative to each other
