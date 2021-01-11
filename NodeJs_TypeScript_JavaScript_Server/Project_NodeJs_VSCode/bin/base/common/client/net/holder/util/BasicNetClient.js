"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class BasicNetClient {
    constructor(con) {
        this.con = null;
        this.receiveIddleTimeS = 0;
        // if >= 0; Connection will be removed on timeout expiration.
        this.forceDisconnectTimeoutS = -1;
        this.markForRemoval = false;
        // The same behaviour as above but used by a different mechanic.
        this.currentCommandTimeoutS = -1;
        this.isPingAllowed = false;
        this.isPingInProgress = false;
        if (con != null) {
            this.setCon(con);
        }
    }
    getCon() {
        return this.con;
    }
    // Thic can be used to extract the con and using it somewhere else without closing it.
    setCon(con) {
        if (this.con != null) {
            this.con.setCbClose(null);
            this.con.setCbError(null);
            if (this.con.getUserData("client") != this) {
                this.con.setUserData("client", null);
            }
            this.con = null;
        }
        this.con = con;
        if (this.con != null) {
            this.con.setUserData("client", this);
            this.remotePort = this.con.getSocket().remotePort;
        }
    }
}
exports.BasicNetClient = BasicNetClient;
//# sourceMappingURL=BasicNetClient.js.map