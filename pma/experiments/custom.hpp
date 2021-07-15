#ifndef PMA_CUSTOM_HPP_
#define PMA_CUSTOM_HPP_

#include "pma/experiment.hpp"

#include <memory>
#include <vector>

namespace pma {

class Interface;

/**
 * Experiment: Perform N range queries over multiple intervals
 */
struct ExperimentCustom: public Experiment {
    std::shared_ptr<Interface> interface; // the data structure to evaluate
    bool thread_pinned = false; // keep track if we have pinned the thread

    void do_inserts(Interface* pma);

protected:
    /**
     * Initialise the distribution
     */
    void preprocess() override;

    /**
     * Execute the experiment
     */
    void run() override;

public:
    /**
     * Initialise the experiment
     * @param pma the data structure to evaluate
     */
    ExperimentCustom(std::shared_ptr<Interface> pma);

    /**
     * Destructor
     */
    virtual ~ExperimentCustom();
};

} // namespace pma



#endif /* PMA_CUSTOM_HPP_ */
