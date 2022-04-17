#pragma once
#include "Communication.h"
#include "utility.h"
class Channel
{
	public:
		Channel(void);
		~Channel(void);
		virtual void transmit(Communication* comm);
};

