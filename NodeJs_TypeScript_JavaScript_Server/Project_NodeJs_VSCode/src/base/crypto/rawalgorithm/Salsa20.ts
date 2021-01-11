import { IAlgorithm } from "../algorithm/IAlgorithm";

// Source url: https://github.com/neoatlantis/node-salsa20/blob/master/salsa20.js
// Implementation example: https://github.com/neoatlantis/node-salsa20/blob/master/example.js
/*
 * A Salsa20 implementation in pure JavaScript for NodeJS
 * ======================================================
 * Please view README.md in the same directory or at
 *   <https://github.com/neoatlantis/node-salsa20/blob/master/salsa20.js>
 * for more information.
 */

export class Salsa20 implements IAlgorithm {
    private static readonly rounds: number = 20;
    
    public constructor() {
        //void
    }

    private coreFuncX: Uint32Array = new Uint32Array(16);
    
    private coreFunc(ina: Uint32Array, ret: Uint32Array){
        function R(a: number, b: number) {
            return (((a) << (b)) | ((a) >>> (32 - (b))));
        };
        // Salsa20 Core Word Specification
        let i; //ret = new Uint32Array(16);
        let x: Uint32Array = this.coreFuncX;
        for (i=0; i<16; i++) {
            x[i] = ina[i];
        }

        // 10 loops × 2 rounds/loop = 20 rounds
        for (i=0; i<Salsa20.rounds; i+=2){
            x[ 4] ^= R(x[ 0]+x[12], 7);
            x[ 8] ^= R(x[ 4]+x[ 0], 9);
            x[12] ^= R(x[ 8]+x[ 4],13);
            x[ 0] ^= R(x[12]+x[ 8],18);
            x[ 9] ^= R(x[ 5]+x[ 1], 7);
            x[13] ^= R(x[ 9]+x[ 5], 9);
            x[ 1] ^= R(x[13]+x[ 9],13);
            x[ 5] ^= R(x[ 1]+x[13],18);
            x[14] ^= R(x[10]+x[ 6], 7);
            x[ 2] ^= R(x[14]+x[10], 9);

            x[ 6] ^= R(x[ 2]+x[14],13);
            x[10] ^= R(x[ 6]+x[ 2],18);
            x[ 3] ^= R(x[15]+x[11], 7);
            x[ 7] ^= R(x[ 3]+x[15], 9);
            x[11] ^= R(x[ 7]+x[ 3],13);
            x[15] ^= R(x[11]+x[ 7],18);
            x[ 1] ^= R(x[ 0]+x[ 3], 7);
            x[ 2] ^= R(x[ 1]+x[ 0], 9);
            x[ 3] ^= R(x[ 2]+x[ 1],13);
            x[ 0] ^= R(x[ 3]+x[ 2],18);
            
            x[ 6] ^= R(x[ 5]+x[ 4], 7);
            x[ 7] ^= R(x[ 6]+x[ 5], 9);
            x[ 4] ^= R(x[ 7]+x[ 6],13);
            x[ 5] ^= R(x[ 4]+x[ 7],18);
            x[11] ^= R(x[10]+x[ 9], 7);
            x[ 8] ^= R(x[11]+x[10], 9);
            x[ 9] ^= R(x[ 8]+x[11],13);
            x[10] ^= R(x[ 9]+x[ 8],18);
            x[12] ^= R(x[15]+x[14], 7);
            x[13] ^= R(x[12]+x[15], 9);

            x[14] ^= R(x[13]+x[12],13);
            x[15] ^= R(x[14]+x[13],18);
        };

        for(i=0; i<16; i++) {
            ret[i] = x[i] + ina[i];
        }

        ++this._keyExpanBuffer[8];
        this._keyExpanBuffer[9] += this._keyExpanBuffer[8] == 0 ? 1 : 0;
    }

    /* key expansion */

    private _keyExpanBuffer: Uint32Array = new Uint32Array(16);

    /* key expansion for 8 words(32 bytes) key */
    private _salsa20BufferFillKey8(key8: Uint32Array, nonce2: Uint32Array){
        let input: Uint32Array = this._keyExpanBuffer;
        let sigma: Uint32Array = new Uint32Array(
            [0x61707865, 0x3320646e, 0x79622d32, 0x6b206574]
        );

        input[0]  = sigma[0];
        input[1]  = key8[0];
        input[2]  = key8[1];
        input[3]  = key8[2];
        input[4]  = key8[3];
        input[5]  = sigma[1];

        input[6]  = nonce2[0];
        input[7]  = nonce2[1];

        input[8]  = 0;
        input[9]  = 0;

        input[10] = sigma[2];
        input[11] = key8[4];
        input[12] = key8[5];
        input[13] = key8[6];
        input[14] = key8[7];
        input[15] = sigma[3];
    }

    private _salsa20ExpansionKey8(ret: Uint32Array){
        var input = this._keyExpanBuffer;

        return this.coreFunc(input, ret);
    }

    //////////////////////////////////////////////////////////////////////

    private blockGenerator(ret: Uint32Array) {
        this._salsa20ExpansionKey8(ret);
    }

    private _initialize(key: Uint32Array, nonce: Uint32Array){
        this._salsa20BufferFillKey8(key, nonce);
    }

    public processBytes(inBuf: ArrayBuffer, inBytesOffset: number, 
        outBuf: ArrayBuffer, outBytesOffset: number, 
        bytesCount: number): void 
    {
        let blocksCount = Math.floor(bytesCount / 64) + 1;
        let block = new Uint32Array(16);    // holder of new generated block
        
        // This is in shared memory view mode.
        let inStream: Uint8Array = new Uint8Array(inBuf, inBytesOffset, bytesCount);
        // This is in shared memory view mode.
        let outStream: Uint8Array = new Uint8Array(outBuf, outBytesOffset, bytesCount);
        let xorStream: Uint8Array = new Uint8Array(outStream.length + 64);
        
        var b = 0;
        for(let i: number=0; i<blocksCount; i++){
            this.blockGenerator(block);
            for(let j: number=0; j<16; j++){
                xorStream[b++] = (block[j] >> 0) & 0xff;
                xorStream[b++] = (block[j] >> 8) & 0xff;
                xorStream[b++] = (block[j] >> 16) & 0xff;
                xorStream[b++] = (block[j] >> 24) & 0xff;
            };
        };

        for(let i: number=0; i<bytesCount; i++) {
            outStream[i] = inStream[i] ^ xorStream[i];
        }
    }

    public setKeyIv(key: Uint8Array, iv: Uint8Array) {
        if(key.byteLength != 32) {
            throw new Error('invalid-key');
        }
        if(iv.byteLength != 8) {
            throw new Error('invalid-iv');
        }

        // Use (byteLength / 4) because it is converting from bytes_count to uints_count;
        let parsedKey: Uint32Array = new Uint32Array(key.buffer, key.byteOffset, key.byteLength / 4)
        let parsedIv: Uint32Array = new Uint32Array(iv.buffer, iv.byteOffset, iv.byteLength / 4)

        this._initialize(parsedKey, parsedIv);
    };

}
