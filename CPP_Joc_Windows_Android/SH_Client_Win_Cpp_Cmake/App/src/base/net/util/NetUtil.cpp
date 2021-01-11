#include "NetUtil.h"
#include <base/MM.h>

using namespace base::Net;

//asdA3;
//public class NetUtil {
//
//    public NetUtil() {
//        super();
//
//        //void
//    }
//
//    public static IpPort stringToIpPort(String src) {
//        String ip;
//        String portString;
//        int port;
//
//        if(src == null || src.length() < 5) {
//            return null;
//        }
//
//        int i = src.indexOf(":");
//        if(i < 0) {
//            return null;
//        }
//        ip = src.substring(0, i);
//        if(ip.length() <= 0) {
//            return null;
//        }
//
//        int ii = src.indexOf(":", i + 1);
//        if(ii < 0) {
//            return null;
//        }
//        portString = src.substring(i + 1, ii);
//        try {
//            port = Integer.parseInt(portString);
//        }catch(NumberFormatException x) {
//            return null;
//        }
//
//        IpPort ret = new IpPort(ip, port);
//
//        return ret;
//    }
//
//    public static void sendSimpleNetMsg(GeneralTcpConnection con, int netMsgCode) throws Exception {
//        NetMsgsBlock netBlock = new NetMsgsBlock(1, 4);
//        try {
//            netBlock.startMsgWrite();
//            // START : msg data;
//            netBlock.writeInt(netMsgCode);
//            // END : msg data;
//            netBlock.endMsgWrite();
//
//            con.sendBlock(netBlock);
//        } catch (Exception x) {
//            throw x;
//        }
//    }
//
//}
