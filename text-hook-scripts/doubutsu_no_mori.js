/**
 * game-code: ADMJ
 * name: おいでよ どうぶつの森
 */

let lastUpdateTime = Date.now();
let str = "";
const flushStringMilliseconds = 500;

bpManager.registerBreakPoint(0x0206b682, () => {
    const char = bpManager.readRegister(1);
    const pad = char > 0xFF ? 4 : 2;
    str += char.toString(16).padStart(pad, '0');
    lastUpdateTime = Date.now();
    bpManager.continueExecution();
});

bpManager.registerUpdateFunction(() => {
    if (Date.now() - lastUpdateTime > flushStringMilliseconds && str.length > 0) {
        const decoded = bpManager.decodeHex(str, 'Shift-JIS');
        bpManager.copyToClipboard(decoded);
        str = "";
        lastUpdateTime = Date.now();
    }
});