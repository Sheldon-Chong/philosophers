## Philosophers:
The Philosophers project is a simulation of the Dining Philosophers Problem. In this problem, multiple philosophers sit around a table, cycling between thinking, eating, and sleeping, while sharing a limited number of forks. Each philosopher needs two forks to eat, but since forks are shared, improper coordination starvation (where a philosopher never gets to eat as forks are occupied by others). (See below for a more thorough explanation)

## 📖 Topics covered
- Threads, Mutex locks, Dataraces, Deadlocks
- Forking processes, semaphores

## ⬇️ Clone
```
git clone git@github.com:Sheldon-Chong/philosophers.git
```

## ⏯️ Compile and run
```
make
./philosophers <no. of philos> <lifespan time> <time taken to eat> <time taken to sleep> [no. of times all philosophers must eat]
```


## Breaking down the problem this project aims to solve

The subject pdf is a bit of a riddle, but essentially the idea is that a program must simulate a specified amount of threads (called "philosophers"), that run parallel to each other, accessing a **shared resource** (A variable that two or more threads are able to access). These resources should not be accessed simultaneously, as it will result in a problem known as a **"datarace"**, hence, when multiple threads access a shared resource, it must be coordinated and scheduled using "**mutex locks**".

To make things more complex, the pdf specifies that...
* The user must provide the following parameters when running the program: time_to_sleep, time_to_eat, ....
* Each thread must not only access a shared resource, but have their own buffer waiting times between the next time they try to access this resource, which are actions labelled as "sleeping" and "thinking". 
* Furthermore, threads are not allowed to communicate with each other, meaning they must work independently.
* Finally, all threads have their own lifespan. A Threads will die if they don't get to access the shared resource within the remaining lifespan they have. However, the moment they are able to access the shared resource, their lifespan is replenished. The moment a thread dies, the program ends.


## My experience with Philosophers
Two major ch is optimizing the time taken in-between actions that each individual thread take. Dataraces and deadlocks, as they are hard to track
