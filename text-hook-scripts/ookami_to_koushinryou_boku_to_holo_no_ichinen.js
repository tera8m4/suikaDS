/**
 * game-code: YU5J
 * name: [JP] Spice and Wolf My Year With Holo
 */

const breakAddr = 0x0203735c;
bpManager.registerBreakPoint(breakAddr, function() {
	bpManager.log("breakpoint callback");
	// string ptr
	// 02037344  24 00 98 e5                  ldr                      r0,[r8,#0x24]
	// 02037348  14 10 98 e5                  ldr                      r1,[r8,#0x14]
	// 0203734c  0c 20 90 e5                  ldr                      r2,[r0,#0xc]
	// 02037350  00 00 a0 e3                  mov                    r0,#0x0
	// 02037354  20 20 82 e2                  add                     r2,r2,#0x20
	// 02037358  01 10 82 e0                  add                     r1,r2,r1
	
	const r1 = bpManager.readRegister(1);
	const str = bpManager.readString(r1, 'Shift-JIS') || "";
	
	bpManager.copyToClipboard(str.split("▼")[0]);
	bpManager.continueExecution();
});