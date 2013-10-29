// ############################################### //
// ############################################### //
// ####                                       #### //
// ####    Program:  Stamp Dispenser          #### //
// ####    Language: C++                      #### //
// ####    Author:   Georgi Baghdasaryan      #### //
// ####    Email:    baghdasaryan@ucla.edu    #### //
// ####                                       #### //
// ############################################### //
// ############################################### //

#include <stdlib.h>
#include <assert.h>

/// <summary>
/// Facilitates dispensing stamps for a postage stamp machine.
/// </summary>
class StampDispenser
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="StampDispenser"/> class that will be 
    /// able to dispense the given types of stamps.
    /// </summary>
    /// <param name="stampDenominations">
    /// The values of the types of stamps that the machine has.  
    /// Should be sorted in descending order and contain at least a 1.
    /// </param>
    /// <param name="numStampDenominations">
    /// The number of types of stamps in the stampDenominations array. 
    /// </param>
    StampDispenser(const int* stampDenominations, size_t numStampDenominations);
    
	/// <summary>
	/// Cleans up memory
	/// </summary>
	~StampDispenser();

    /// <summary>
    /// Returns the minimum number of stamps that the machine can dispense to
    /// fill the given request. Program returns -1 in case the request value is
	/// negative.
    /// </summary>
    /// <param name="request">
    /// The total value of the stamps to be dispensed.
    /// </param>
    /// <returns>
    /// The minimum number of stamps needed to fill the given request.
    /// </returns>
    int CalcNumStampsToFillRequest(int request);

private:
	int *m_stampDenominations;
	size_t m_numStampDenominations;

}; 

StampDispenser::StampDispenser(const int* stampDenominations, size_t numStampDenominations) {
	// Array does not contain stump with value 1
	if(*(int*) ((char*) stampDenominations + ((numStampDenominations - 1) * 4)) != 1)
		exit(1);
	
	m_stampDenominations = new int[numStampDenominations];
	// multiply numStampDenominations by sizeof(int) -> 4
	m_numStampDenominations = numStampDenominations << 2;
	
	for(int i = 0; i < m_numStampDenominations; i += 4) {
		// Unsorted array
		if(i != 0 && *(int*) ((char*) stampDenominations + i) >= *(int*) ((char*) stampDenominations + i - 4))
			exit(1);

		*(int*) ((char*) m_stampDenominations + i) = *(int*) ((char*) stampDenominations + i);
	}
}

StampDispenser::~StampDispenser() {
	delete [] m_stampDenominations;
}

// Complexity: O(N^2)
int StampDispenser::CalcNumStampsToFillRequest(int request) {
	// Take care of the special cases
	if(request < 0)			return -1;
	else if(request == 0)	return 0;

	int numStamps = 0,
		tempStamps,
		tempRequest;

	// get the smallest number of stamps out of all subarrays
	for (size_t i = 0; i < m_numStampDenominations - 4; i += 4) {
		// iterate through the addresses of array elements
		tempStamps = 0;
		tempRequest = request;

		// iterate through a subarray
		for (size_t j = i; j < m_numStampDenominations && tempRequest > 0; j += 4) {
			if(tempRequest >= *(int*) ((char*) m_stampDenominations + j)) {
				tempStamps += tempRequest / *(int*) ((char*) m_stampDenominations + j);
				tempRequest %= *(int*) ((char*) m_stampDenominations + j);
				
				// stop computation if there is only a 1 cent stamp left
				if (tempRequest == 1) {
					tempRequest--;
					tempStamps++;
					break;
				}
			}
		}
		
		// if necessary, update the number of stamps to be returned
		if (tempRequest == 0 && (numStamps == 0 || tempStamps < numStamps))
			numStamps = tempStamps;
	}

	return numStamps;
}

int main()
{
    int stampDenominations[] = {90, 30, 24, 10, 6, 2, 1};
    StampDispenser stampDispenser(stampDenominations, 7);
    assert(stampDispenser.CalcNumStampsToFillRequest(18) == 3);

	// Wrong input
	assert(stampDispenser.CalcNumStampsToFillRequest(-10) == -1);
	assert(stampDispenser.CalcNumStampsToFillRequest(-1) == -1);
	assert(stampDispenser.CalcNumStampsToFillRequest(0) == 0);

	// Additional tests
	assert(stampDispenser.CalcNumStampsToFillRequest(1) == 1);
	assert(stampDispenser.CalcNumStampsToFillRequest(2) == 1);
	assert(stampDispenser.CalcNumStampsToFillRequest(5) == 3);
	assert(stampDispenser.CalcNumStampsToFillRequest(34) == 2);
	assert(stampDispenser.CalcNumStampsToFillRequest(72) == 3);

    return 0;
}
