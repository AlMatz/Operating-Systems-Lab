# Operating-Systems
Operating Systems Class

Lab 3:

       Part 1: Prints the date and time of the Ubuntu Linux machine by using "execl". 
       
       Part 2: Lists the files in the current directory. States name of file, permissions, size, date modified, location, etc. Also lists hidden files.
       
       Part 3: Created two programs (Process_P1 and Process_P2) that work together in a parent program (Parent_Process) to read from a source file and write to two different destination files depending on how many bytes are read and what is read.
       
Lab 4: 
       
       Part A: A command interpreter for Ubuntu Linux that you can type any command into and it will execute it. Quits the interpreter when you type "quit".
      
       Part B: Grade calculator that will calculate the average and max value of the grades for homeworks in each chapter. You can change x (chapters), y (hws per chapter), and   students, to allow for different values. You must also change the columns and rows in the grades.txt file if you change x, y, or students. 
      
Lab 5: 
       
       Part A: Used P(sem) and V(sem) functions described in sem.h file to synchronize a program that utilizes three processes. The processes consist of a father who adds money into an account, and two children who try to take money from the account. The P and V functions are used so that no process tries to take or add money when another process is changing the balance of the account. 
      
       Part B: Counted how often process are waiting to run while other processes are running. This is done by using addittional semaphores to act as counter and flags. 

Lab 6: 
       
       Part A: Used P(sem) and V(sem) functions described in sem.h file to synchronize a smoking program that utilizes an agent that places smoking objets on a table and three smoking processes that use the items the agent placed. The three processes would be activated depending on which object the agent put down, i.e., the paper, match, or tobacco. The P and V functions are used to create synchronization so that no two processes try to run at the same time. 
      
       Part B: Using Pthreads instead of semaphores to synchronize the processes for the same program stated above. The pthreads were very similar, except this time the pthread_unlock and pthread_lock functions were used. The synchronization was the same for both the pthread and semaphore implementation.


