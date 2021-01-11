#pragma once

#include "GPOServer.h"

namespace startarea_a_v1 {
class GPOnline : public GPOServer {
private: typedef GPOServer super;
private:
    class NetCodes {
    public:
        static const int SC_C_SetSelfStatus = 200100;
        // status: int;
        static const int CS_R_SetSelfStatus_OK = 200101;
    };
private:
    static const bool IS_DEBUG = true;

public:
    GPOnline(GPOnline const&) = delete;
    GPOnline(GPOnline &&) = default;
    GPOnline& operator=(GPOnline const&) = delete;
    GPOnline& operator=(GPOnline &&) = default;

    explicit GPOnline(base::IGameWorld* gw);

    ~GPOnline() override;
protected:
    bool processMsg(base::Net::IGeneralTcpCon& con, int msgLen, int msgType, base::StreamBuffer& msgNetBuffer) override;

private:
    void sendMsg_CS_R_SetSelfStatus_OK(base::Net::IGeneralTcpCon& con);

};
};
