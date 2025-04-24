#pragma once


#include <csignal>
#include <atomic>


struct SignalHandler {


	SignalHandler() = delete;

	inline static void sigint_handler(int sig_num) {
		
		if (sig_num != SIGINT) {
			return;
		}

		interrupt_recieved.store(true);
	}

	inline static void setup_sigint_handler() {
		signal(SIGINT, sigint_handler); // this is not correct really!
	}

	inline static bool isInterrupted() {
		return interrupt_recieved.load();
	}



private:

	inline static std::atomic_bool interrupt_recieved{false}; // I guess that it is atomic because multiple processes can send us SIGINT at the same time

};
