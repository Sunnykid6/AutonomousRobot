
// Basic simulate imprecise measurement code in RobotC
int min_value = 0;
int max_value = 100;

// Experiment parameters
int total_trials = 1000;

// Random number storage.
int true_value;
int total_meas = 0;

/**********************************************************
 ** void generate_new_value
 ** Generates new true_value
 **********************************************************/
void generate_new_value()
{
	// Random number parameter

	int temp_number = rand();

	int range = max_value - min_value;

	// Kludge to handle fact that RobotC does not have unsigned ints.
	temp_number = ( temp_number < 0 ) ? -1 * temp_number : temp_number;

	true_value = temp_number % range + min_value;
}

/**********************************************************
 ** int resolve_measurement( int meas_point )
 ** Resolve Measurement at set point.
 ** Inputs:
 **   meas_point	Set point of measurment
 ** Outputs:
 **   0						If value is greater than meas_point
 **   -1          If value is less then meas_point
 **   1           If true value is less than meas_pont
 ** Global Interaction:
 **   Keeps track of number of measurements in total_meas
 **
 **********************************************************/
int resolve_measurement( int meas_point ) {

  // Increment total number of guesses.
	total_meas++;
	if( true_value == 0 || true_value == 99){
		return(0);
	}
	else if ( meas_point > true_value ) {
		// Return 1 if measurement is too large.
		return(1);
	} else if ( meas_point < true_value ) {
	  // Return -1 if measurment is too small.
	  return(-1);
	} else {
	  // Return 0 if measurement point matches
	  return(0);
	}
}

task main()
{
	// Run multiple trials
	int current_trial;

	// Range to search for true value
	int min_point = min_value;
	int max_point = max_value;


	// For each trial
	for ( current_trial = 0; current_trial < total_trials ; current_trial++ ) {

	  // Current measurement point set to lowest value.
		int current_meas_point = 50;


		// Generate new value to measure
		generate_new_value();

		// Sweep through range from lowest to highest possible value.
		// Stop when measurment_point matches true value.
		while ( resolve_measurement( current_meas_point ) != 0  ) {
			if (resolve_measurement( current_meas_point) == 1) {
				  max_point = current_meas_point + 1;
			}
			if (resolve_measurement( current_meas_point) == -1) {
					min_point = current_meas_point - 1;
			}
			current_meas_point = (max_point + min_point)/2;
		}

		// Write output to debug stream.
		writeDebugStream("%d trials and %d guesses\n",current_trial,total_meas);
		min_point=0;
		max_point=100;


	}

	// Wait for an infinite amount of time.
	while(1);
}
