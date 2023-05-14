#pragma once

#include <vector>

#define CHAR_BUFFER		10000

class ComPort {
public:
	ComPort() {
		RetrieveComPorts(mComPortNumbers);
	}
	~ComPort() {}

// Attributes
protected:
	std::vector<int> mComPortNumbers;

// Implementation
public:
	bool RetrieveComPorts(std::vector<int>& comPortNumbers) const;
};

