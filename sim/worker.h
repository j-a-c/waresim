#ifndef SIM_WORKER_H
#define SIM_WORKER_H

/**
 * Represents a worker within the factory.
 */
class Worker
{
    public:
        Worker(int);
        ~Worker();

    private:
        // The worker's position within the factory.
        int pos;
};

#endif
