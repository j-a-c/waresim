#include "dispatch_algo.h"
#include "../rand/rand.h"

/**
 * Encapsulates an algorithm that randomly selects a worker in the factory to
 * process the order.
 */
class RandDispatchAlgo : public DispatchAlgo
{
    public:
        RandDispatchAlgo();
        RandDispatchAlgo(Rand);
        ~RandDispatchAlgo();

        void assign_order(Factory *, Order) override;
    private:
        Rand rand;
};
