#pragma once
#include "channel.h"
class RayleighChannel : public Channel
{
	public:
		RayleighChannel(void);
		~RayleighChannel(void);
		void transmit(Communication* comm);
};

