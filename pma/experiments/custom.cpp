#include "custom.hpp"

#include <cassert>
#include <iostream>
#include <random>

#include "configuration.hpp"
#include "console_arguments.hpp"
#include "database.hpp"
#include "errorhandling.hpp"
#include "miscellaneous.hpp"
#include "timer.hpp"

#include "pma/interface.hpp"

#define RAISE(message) RAISE_EXCEPTION(pma::ExperimentError, message)

using namespace std;

namespace pma {

	using KeyValue = std::pair<int64_t, int64_t>;

	ExperimentCustom::ExperimentCustom(std::shared_ptr<Interface> pma) :
		interface(pma){
			if(pma.get() == nullptr) RAISE("The pointer data structure is NULL");
		}

	ExperimentCustom::~ExperimentCustom() {
		if(thread_pinned){ unpin_thread(); }
	}

	void ExperimentCustom::preprocess() {
		auto initial_size = ARGREF(int64_t, "initial_size");
		if(initial_size.is_set() && initial_size > 0){
			std::cout << "[ExperimentCustom::preprocess] WARNING: parameter initial size ignored in this experiment\n";
		}

		// initialize the elements that we need to add
		LOG_VERBOSE("Generating the set of elements to insert ... ");

		pin_thread_to_cpu();
		thread_pinned = true;
		LOG_VERBOSE("Experiment ready to begin");
	}

	void ExperimentCustom::do_inserts(Interface* pma){
		// modify the array here to customize what's inserted
		int key = 0, value = 0;
		while(!cin.eof()){
			cout << "Enter the key" << endl;
			cin >> key;
			cout << "Enter the value" << endl;
			cin >> value;
			pma->insert(key, value);
		}
		//KeyValue kv[] = {{10, 10}, {-2,11}, {20, 20}, {-14,14}, {21, 21}, {15, 15}, {-16, 16}, {19, 19},{22, 22}, {-5, 25},
				//{13,5},{7,12},{8,9},{-3,4},{-18,21},{-20,-20},{-19,19},{-25,1},{-23,8}, {-24, 0}, {-30,5}};
		//for(size_t i = 0; i < N_inserts; i++){
		//	pma->insert(kv[i].first, kv[i].second);
		//}
	}

	void ExperimentCustom::run() {
		auto pma = interface.get();
		Timer aux_timer;

		// cout << "Inserting " << N_inserts << " elements ..." << endl;
		aux_timer.reset(true);
		do_inserts(pma); 
		aux_timer.stop();
		size_t t_insert = aux_timer.milliseconds();
		cout << "# Insertion time: " << t_insert << " millisecs" << endl;

		// save the result
		// config().db()->add("custom")
		//	("type", "insert")
	//		("initial_size", (size_t) 0)
	//		("elements", N_inserts)
	//		("time", t_insert);


		aux_timer.reset(true);
		pma->build(); // in case of the Static-ABTree we build the tree only after calling #build()
		aux_timer.stop();
		size_t t_build = aux_timer.milliseconds();
		if(t_build > 0){
			cout << "# Build time: " << t_build << " millisecs" << endl;
		}
	}

} /* namespace pma */
