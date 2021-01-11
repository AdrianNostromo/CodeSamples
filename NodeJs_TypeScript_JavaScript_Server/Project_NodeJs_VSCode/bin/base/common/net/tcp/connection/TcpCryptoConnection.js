"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const TcpRequestBlocksConnection_1 = require("./TcpRequestBlocksConnection");
class TcpCryptoConnection extends TcpRequestBlocksConnection_1.TcpRequestBlocksConnection {
    constructor(socket) {
        super(socket);
        this.encryptionCipher = null;
        this.decryptionCipher = null;
        this.isEnabledAutoCryptoRead = false;
        this.isEnabledAutoCryptoWrite = false;
        //void
    }
    enabledAutoCryptoRead() {
        this.isEnabledAutoCryptoRead = true;
    }
    enabledAutoCryptoWrite() {
        this.isEnabledAutoCryptoWrite = true;
    }
    extractConfirmedReceivedBlock(blockLen) {
        let blockNetBuffer = super.extractConfirmedReceivedBlock(blockLen);
        if (this.isEnabledAutoCryptoRead) {
            // Decrypt.
            // The first 2 int values are not encrypted.
            let blockBuffer = blockNetBuffer.getBuffer();
            let cryptoOffset = 4 + 4;
            let cryptoDataLen = blockBuffer.length - cryptoOffset;
            this.decryptionCipher.processBytes(blockBuffer.buffer, cryptoOffset, blockBuffer.buffer, cryptoOffset, cryptoDataLen);
        }
        return blockNetBuffer;
    }
    sendBlock(blockDataNetBuffer) {
        if (this.isEnabledAutoCryptoWrite) {
            // Encrypt.
            // The first 2 int values are not encrypted.
            let blockBuffer = blockDataNetBuffer.getBuffer();
            let cryptoOffset = 4 + 4;
            let cryptoDataLen = blockBuffer.length - cryptoOffset;
            this.encryptionCipher.processBytes(blockBuffer.buffer, cryptoOffset, blockBuffer.buffer, cryptoOffset, cryptoDataLen);
        }
        super.sendBlock(blockDataNetBuffer);
    }
}
exports.TcpCryptoConnection = TcpCryptoConnection;
//# sourceMappingURL=TcpCryptoConnection.js.map