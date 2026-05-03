*This project has been created as part of the 42 curriculum by ayhammou.*

## Description
Codexion is a multithreading simulation based on the Dining Philosophers problem, adapted with coders and dongles. The project highlights shared resource management and scheduling (FIFO and EDF).

## Instructions
- Compilation : `make`
- Execution : `./codexion [nb_coders] [burnout] [compile] [debug] [refactor] [nb_compiles] [cooldown] [fifo/edf]`
- Example : `./codexion 5 800 200 200 10 0 5 fifo`

## Resources
- Official documentation of `<pthread.h>`
- Tutorials on thread synchronization (Mutex and Condition Variables).
- Use of AI (Gemini): The AI was used as a tutor to explain how "Condition Variables" work, the concept of Busy Waiting for my `ms_sleep` function, and to help debug the min-heap scheduling for the dongles' local queue.

## Blocking cases handled
- **Deadlock :** Avoided by enforcing strict asymmetry (even threads pick up the left dongle first, odd threads pick up the right dongle first).
- **Starvation / Livelock :** Managed through the implementation of local queues per dongle (Min-Heap) with FIFO or EDF priority handling.
- **Cooldown delay :** Managed with a micro-sleep system (`usleep`) that periodically checks availability time without monopolizing the CPU (avoids blocking busy waiting).

## Thread synchronization mechanisms
- **Mutex (`pthread_mutex_t`) :** Used to protect sensitive variables (simulation state, coder statistics) and to lock dongles during manipulation.
- **Condition variables (`pthread_cond_t`) :** Used in tandem with mutexes to deeply sleep threads waiting for a dongle (`pthread_cond_wait`) and wake them selectively when a dongle is put back on the table (`pthread_cond_broadcast`), thus saving CPU resources.