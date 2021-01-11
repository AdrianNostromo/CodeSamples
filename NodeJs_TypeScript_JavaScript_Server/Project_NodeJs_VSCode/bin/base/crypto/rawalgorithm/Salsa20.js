"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
// Source url: https://github.com/neoatlantis/node-salsa20/blob/master/salsa20.js
// Implementation example: https://github.com/neoatlantis/node-salsa20/blob/master/example.js
/*
 * A Salsa20 implementation in pure JavaScript for NodeJS
 * ======================================================
 * Please view README.md in the same directory or at
 *   <https://github.com/neoatlantis/node-salsa20/blob/master/salsa20.js>
 * for more information.
 */
class Salsa20 {
    constructor() {
        this.coreFuncX = new Uint32Array(16);
        /* key expansion */
        this._keyExpanBuffer = new Uint32Array(16);
        //void
    }
    coreFunc(ina, ret) {
        function R(a, b) {
            return (((a) << (b)) | ((a) >>> (32 - (b))));
        }
        ;
        // Salsa20 Core Word Specification
        let i; //ret = new Uint32Array(16);
        let x = this.coreFuncX;
        for (i = 0; i < 16; i++) {
            x[i] = ina[i];
        }
        // 10 loops × 2 rounds/loop = 20 rounds
        for (i = 0; i < Salsa20.rounds; i += 2) {
            x[4] ^= R(x[0] + x[12], 7);
            x[8] ^= R(x[4] + x[0], 9);
            x[12] ^= R(x[8] + x[4], 13);
            x[0] ^= R(x[12] + x[8], 18);
            x[9] ^= R(x[5] + x[1], 7);
            x[13] ^= R(x[9] + x[5], 9);
            x[1] ^= R(x[13] + x[9], 13);
            x[5] ^= R(x[1] + x[13], 18);
            x[14] ^= R(x[10] + x[6], 7);
            x[2] ^= R(x[14] + x[10], 9);
            x[6] ^= R(x[2] + x[14], 13);
            x[10] ^= R(x[6] + x[2], 18);
            x[3] ^= R(x[15] + x[11], 7);
            x[7] ^= R(x[3] + x[15], 9);
            x[11] ^= R(x[7] + x[3], 13);
            x[15] ^= R(x[11] + x[7], 18);
            x[1] ^= R(x[0] + x[3], 7);
            x[2] ^= R(x[1] + x[0], 9);
            x[3] ^= R(x[2] + x[1], 13);
            x[0] ^= R(x[3] + x[2], 18);
            x[6] ^= R(x[5] + x[4], 7);
            x[7] ^= R(x[6] + x[5], 9);
            x[4] ^= R(x[7] + x[6], 13);
            x[5] ^= R(x[4] + x[7], 18);
            x[11] ^= R(x[10] + x[9], 7);
            x[8] ^= R(x[11] + x[10], 9);
            x[9] ^= R(x[8] + x[11], 13);
            x[10] ^= R(x[9] + x[8], 18);
            x[12] ^= R(x[15] + x[14], 7);
            x[13] ^= R(x[12] + x[15], 9);
            x[14] ^= R(x[13] + x[12], 13);
            x[15] ^= R(x[14] + x[13], 18);
        }
        ;
        for (i = 0; i < 16; i++) {
            ret[i] = x[i] + ina[i];
        }
        ++this._keyExpanBuffer[8];
        this._keyExpanBuffer[9] += this._keyExpanBuffer[8] == 0 ? 1 : 0;
    }
    /* key expansion for 8 words(32 bytes) key */
    _salsa20BufferFillKey8(key8, nonce2) {
        let input = this._keyExpanBuffer;
        let sigma = new Uint32Array([0x61707865, 0x3320646e, 0x79622d32, 0x6b206574]);
        input[0] = sigma[0];
        input[1] = key8[0];
        input[2] = key8[1];
        input[3] = key8[2];
        input[4] = key8[3];
        input[5] = sigma[1];
        input[6] = nonce2[0];
        input[7] = nonce2[1];
        input[8] = 0;
        input[9] = 0;
        input[10] = sigma[2];
        input[11] = key8[4];
        input[12] = key8[5];
        input[13] = key8[6];
        input[14] = key8[7];
        input[15] = sigma[3];
    }
    _salsa20ExpansionKey8(ret) {
        var input = this._keyExpanBuffer;
        return this.coreFunc(input, ret);
    }
    //////////////////////////////////////////////////////////////////////
    blockGenerator(ret) {
        this._salsa20ExpansionKey8(ret);
    }
    _initialize(key, nonce) {
        this._salsa20BufferFillKey8(key, nonce);
    }
    processBytes(inBuf, inBytesOffset, outBuf, outBytesOffset, bytesCount) {
        let blocksCount = Math.floor(bytesCount / 64) + 1;
        let block = new Uint32Array(16); // holder of new generated block
        // This is in shared memory view mode.
        let inStream = new Uint8Array(inBuf, inBytesOffset, bytesCount);
        // This is in shared memory view mode.
        let outStream = new Uint8Array(outBuf, outBytesOffset, bytesCount);
        let xorStream = new Uint8Array(outStream.length + 64);
        var b = 0;
        for (let i = 0; i < blocksCount; i++) {
            this.blockGenerator(block);
            for (let j = 0; j < 16; j++) {
                xorStream[b++] = (block[j] >> 0) & 0xff;
                xorStream[b++] = (block[j] >> 8) & 0xff;
                xorStream[b++] = (block[j] >> 16) & 0xff;
                xorStream[b++] = (block[j] >> 24) & 0xff;
            }
            ;
        }
        ;
        for (let i = 0; i < bytesCount; i++) {
            outStream[i] = inStream[i] ^ xorStream[i];
        }
    }
    setKeyIv(key, iv) {
        if (key.byteLength != 32) {
            throw new Error('invalid-key');
        }
        if (iv.byteLength != 8) {
            throw new Error('invalid-iv');
        }
        // Use (byteLength / 4) because it is converting from bytes_count to uints_count;
        let parsedKey = new Uint32Array(key.buffer, key.byteOffset, key.byteLength / 4);
        let parsedIv = new Uint32Array(iv.buffer, iv.byteOffset, iv.byteLength / 4);
        this._initialize(parsedKey, parsedIv);
    }
    ;
}
Salsa20.rounds = 20;
exports.Salsa20 = Salsa20;
//# sourceMappingURL=Salsa20.js.map