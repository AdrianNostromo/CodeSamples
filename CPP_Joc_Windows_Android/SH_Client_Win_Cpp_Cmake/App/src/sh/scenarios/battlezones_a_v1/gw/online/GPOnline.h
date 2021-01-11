#pragma once

#include <base/gh.h>
#include "GPOServer.h"

namespace battlezones_a_v1 {
class GPOnline : public GPOServer {priv typedef GPOServer super;pub dCtor(GPOnline);
	priv class NetCodes {
		pub static const int SC_C_SetSelfStatus = 200100;
        // status: int;
		pub static const int CS_R_SetSelfStatus_OK = 200101;
    };
	
	priv static const bool IS_DEBUG = true;

    pub explicit GPOnline(base::IGameWorld* gw);

	prot bool processMsg(base::Net::IGeneralTcpCon& con, int msgLen, int msgType, base::StreamBuffer& msgNetBuffer) override;

	priv void sendMsg_CS_R_SetSelfStatus_OK(base::Net::IGeneralTcpCon& con);

	pub ~GPOnline() override;
};
};
