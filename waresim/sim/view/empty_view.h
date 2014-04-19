#include "view.h"

/**
 * An empty view. Nothing will be displayed.
 * The overriden methods will not do anything.
 */

class EmptyView : public View
{
    public:
        EmptyView();
        ~EmptyView();

        // This method will not do anything.
        void setup() override;

        // This method will not do anything.
        void run() override;

        // This method will not do anything.
        void set_warehouse(Warehouse *) override;
        // This method will not do anything.
        void set_sim_params(time_t, int) override;
};
