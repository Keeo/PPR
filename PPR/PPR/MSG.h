#pragma once

#include <string>

enum MSG {

	MSG_WORK_REQUEST = 66,
	MSG_WORK_SENT,
	MSG_WORK_NOWORK,
	
	MSG_WORKING,
	MSG_NOT_WORKING,
	MSG_FINISH,

	MSG_BEST_FOUND,
	MSG_GET_SOLUTION,

	MSG_STATUS,

	MSG_COUNT,

};

inline const std::string msg_to_string(MSG msg)
{
	switch (msg)
	{
		case MSG_WORK_REQUEST: return "MSG_WORK_REQUEST";
		case MSG_WORK_SENT: return "MSG_WORK_SENT";
		case MSG_WORK_NOWORK: return "MSG_WORK_NOWORK";

		case MSG_WORKING: return "MSG_WORKING";
		case MSG_NOT_WORKING: return "MSG_NOT_WORKING";
		case MSG_FINISH: return "MSG_FINISH";

		case MSG_BEST_FOUND: return "MSG_BEST_FOUND";
		case MSG_GET_SOLUTION: return "MSG_GET_SOLUTION";

		case MSG_STATUS: return "MSG_STATUS";

		case MSG_COUNT: return "MSG_COUNT";
	}
}