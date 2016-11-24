#pragma once

namespace constants {
	const int FROM_INPUT_VALUE = 100;
	const int FROM_EDGE = 101;

	const double MAX_NEURON_ACTIVATION_SIGNAL_TIME = 1;

	const int DEFAULT_ACTIVATION_INTERVAL = 0;
	const int MIN_ACTIVATION_INTERVAL = 0;
	const int MAX_ACTIVATION_INTERVAL = 0;

	const int DEFAULT_INPUT_STEP = 3;
	const int DEFAULT_OUTPUT_STEP = 3;

	const int EVALUATE_ON_FIRST_SIGNAL = 1001; //evlauate a network on when an output node receives output, regardless of the states of other output nodes.
	const int EVALUATE_ON_EACH_NODE = 1002; //evaluate a network on each output nodes when enough result has been calculated, regardless of the time step,.
	const int EVALUATE_ON_GLOBAL_OFFSET = 1003; //evaluate a network on a globally set time offset, regardless of the the states of the nodes.
}