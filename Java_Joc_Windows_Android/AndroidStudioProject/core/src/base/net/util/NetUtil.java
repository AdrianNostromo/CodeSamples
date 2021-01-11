package base.net.util;

// General math utility functions.
public class NetUtil {

    public NetUtil() {
        super();

        //void
    }

    public static DTIpPortFamily stringToIpPortFamily(String src) {
        String ip;
        String portString;
        int port;
        String family;

        if(src == null || src.length() < 5) {
            return null;
        }

        int i = src.indexOf(":");
        if(i < 0) {
            return null;
        }
        ip = src.substring(0, i);
        if(ip.length() <= 0) {
            return null;
        }

        int ii = src.indexOf(":", i + 1);
        if(ii < 0) {
            return null;
        }
        portString = src.substring(i + 1, ii);
        try {
            port = Integer.parseInt(portString);
        }catch(NumberFormatException x) {
            return null;
        }

        family = src.substring(ii + 1, src.length());
        if(family.length() <= 0) {
            return null;
        }

        DTIpPortFamily ret = new DTIpPortFamily(ip, port, family);

        return ret;
    }

}
