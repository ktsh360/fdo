	; Don't even think of reading this code
	; It was automatically generated by sha1-586.pl
	; Which is a perl program used to generate the x86 assember for
	; any of ELF, a.out, COFF, Win32, ...
	; eric <eay@cryptsoft.com>
	; 
	TITLE	sha1-586.asm
        .486
.model	FLAT
_TEXT$	SEGMENT PAGE 'CODE'

PUBLIC	_sha1_block_asm_data_order

_sha1_block_asm_data_order PROC NEAR
	mov	ecx,		DWORD PTR 12[esp]
	push	esi
	shl	ecx,		6
	mov	esi,		DWORD PTR 12[esp]
	push	ebp
	add	ecx,		esi
	push	ebx
	mov	ebp,		DWORD PTR 16[esp]
	push	edi
	mov	edx,		DWORD PTR 12[ebp]
	sub	esp,		108
	mov	edi,		DWORD PTR 16[ebp]
	mov	ebx,		DWORD PTR 8[ebp]
	mov	DWORD PTR 68[esp],ecx
	; First we need to setup the X array
$L000start:
	; First, load the words onto the stack in network byte order
	mov	eax,		DWORD PTR [esi]
	mov	ecx,		DWORD PTR 4[esi]
	bswap	eax
	bswap	ecx
	mov	DWORD PTR [esp],eax
	mov	DWORD PTR 4[esp],ecx
	mov	eax,		DWORD PTR 8[esi]
	mov	ecx,		DWORD PTR 12[esi]
	bswap	eax
	bswap	ecx
	mov	DWORD PTR 8[esp],eax
	mov	DWORD PTR 12[esp],ecx
	mov	eax,		DWORD PTR 16[esi]
	mov	ecx,		DWORD PTR 20[esi]
	bswap	eax
	bswap	ecx
	mov	DWORD PTR 16[esp],eax
	mov	DWORD PTR 20[esp],ecx
	mov	eax,		DWORD PTR 24[esi]
	mov	ecx,		DWORD PTR 28[esi]
	bswap	eax
	bswap	ecx
	mov	DWORD PTR 24[esp],eax
	mov	DWORD PTR 28[esp],ecx
	mov	eax,		DWORD PTR 32[esi]
	mov	ecx,		DWORD PTR 36[esi]
	bswap	eax
	bswap	ecx
	mov	DWORD PTR 32[esp],eax
	mov	DWORD PTR 36[esp],ecx
	mov	eax,		DWORD PTR 40[esi]
	mov	ecx,		DWORD PTR 44[esi]
	bswap	eax
	bswap	ecx
	mov	DWORD PTR 40[esp],eax
	mov	DWORD PTR 44[esp],ecx
	mov	eax,		DWORD PTR 48[esi]
	mov	ecx,		DWORD PTR 52[esi]
	bswap	eax
	bswap	ecx
	mov	DWORD PTR 48[esp],eax
	mov	DWORD PTR 52[esp],ecx
	mov	eax,		DWORD PTR 56[esi]
	mov	ecx,		DWORD PTR 60[esi]
	bswap	eax
	bswap	ecx
	mov	DWORD PTR 56[esp],eax
	mov	DWORD PTR 60[esp],ecx
	; We now have the X array on the stack
	; starting at sp-4
	mov	DWORD PTR 132[esp],esi
$L001shortcut::
	; 
	; Start processing
	mov	eax,		DWORD PTR [ebp]
	mov	ecx,		DWORD PTR 4[ebp]
	; 00_15 0
	mov	esi,		ebx
	mov	ebp,		eax
	rol	ebp,		5
	xor	esi,		edx
	and	esi,		ecx
	add	ebp,		edi
	mov	edi,		DWORD PTR [esp]
	xor	esi,		edx
	ror	ecx,		2
	lea	ebp,		DWORD PTR 1518500249[edi*1+ebp]
	add	ebp,		esi
	; 00_15 1
	mov	edi,		ecx
	mov	esi,		ebp
	rol	ebp,		5
	xor	edi,		ebx
	and	edi,		eax
	add	ebp,		edx
	mov	edx,		DWORD PTR 4[esp]
	xor	edi,		ebx
	ror	eax,		2
	lea	ebp,		DWORD PTR 1518500249[edx*1+ebp]
	add	ebp,		edi
	; 00_15 2
	mov	edx,		eax
	mov	edi,		ebp
	rol	ebp,		5
	xor	edx,		ecx
	and	edx,		esi
	add	ebp,		ebx
	mov	ebx,		DWORD PTR 8[esp]
	xor	edx,		ecx
	ror	esi,		2
	lea	ebp,		DWORD PTR 1518500249[ebx*1+ebp]
	add	ebp,		edx
	; 00_15 3
	mov	ebx,		esi
	mov	edx,		ebp
	rol	ebp,		5
	xor	ebx,		eax
	and	ebx,		edi
	add	ebp,		ecx
	mov	ecx,		DWORD PTR 12[esp]
	xor	ebx,		eax
	ror	edi,		2
	lea	ebp,		DWORD PTR 1518500249[ecx*1+ebp]
	add	ebp,		ebx
	; 00_15 4
	mov	ecx,		edi
	mov	ebx,		ebp
	rol	ebp,		5
	xor	ecx,		esi
	and	ecx,		edx
	add	ebp,		eax
	mov	eax,		DWORD PTR 16[esp]
	xor	ecx,		esi
	ror	edx,		2
	lea	ebp,		DWORD PTR 1518500249[eax*1+ebp]
	add	ebp,		ecx
	; 00_15 5
	mov	eax,		edx
	mov	ecx,		ebp
	rol	ebp,		5
	xor	eax,		edi
	and	eax,		ebx
	add	ebp,		esi
	mov	esi,		DWORD PTR 20[esp]
	xor	eax,		edi
	ror	ebx,		2
	lea	ebp,		DWORD PTR 1518500249[esi*1+ebp]
	add	ebp,		eax
	; 00_15 6
	mov	esi,		ebx
	mov	eax,		ebp
	rol	ebp,		5
	xor	esi,		edx
	and	esi,		ecx
	add	ebp,		edi
	mov	edi,		DWORD PTR 24[esp]
	xor	esi,		edx
	ror	ecx,		2
	lea	ebp,		DWORD PTR 1518500249[edi*1+ebp]
	add	ebp,		esi
	; 00_15 7
	mov	edi,		ecx
	mov	esi,		ebp
	rol	ebp,		5
	xor	edi,		ebx
	and	edi,		eax
	add	ebp,		edx
	mov	edx,		DWORD PTR 28[esp]
	xor	edi,		ebx
	ror	eax,		2
	lea	ebp,		DWORD PTR 1518500249[edx*1+ebp]
	add	ebp,		edi
	; 00_15 8
	mov	edx,		eax
	mov	edi,		ebp
	rol	ebp,		5
	xor	edx,		ecx
	and	edx,		esi
	add	ebp,		ebx
	mov	ebx,		DWORD PTR 32[esp]
	xor	edx,		ecx
	ror	esi,		2
	lea	ebp,		DWORD PTR 1518500249[ebx*1+ebp]
	add	ebp,		edx
	; 00_15 9
	mov	ebx,		esi
	mov	edx,		ebp
	rol	ebp,		5
	xor	ebx,		eax
	and	ebx,		edi
	add	ebp,		ecx
	mov	ecx,		DWORD PTR 36[esp]
	xor	ebx,		eax
	ror	edi,		2
	lea	ebp,		DWORD PTR 1518500249[ecx*1+ebp]
	add	ebp,		ebx
	; 00_15 10
	mov	ecx,		edi
	mov	ebx,		ebp
	rol	ebp,		5
	xor	ecx,		esi
	and	ecx,		edx
	add	ebp,		eax
	mov	eax,		DWORD PTR 40[esp]
	xor	ecx,		esi
	ror	edx,		2
	lea	ebp,		DWORD PTR 1518500249[eax*1+ebp]
	add	ebp,		ecx
	; 00_15 11
	mov	eax,		edx
	mov	ecx,		ebp
	rol	ebp,		5
	xor	eax,		edi
	and	eax,		ebx
	add	ebp,		esi
	mov	esi,		DWORD PTR 44[esp]
	xor	eax,		edi
	ror	ebx,		2
	lea	ebp,		DWORD PTR 1518500249[esi*1+ebp]
	add	ebp,		eax
	; 00_15 12
	mov	esi,		ebx
	mov	eax,		ebp
	rol	ebp,		5
	xor	esi,		edx
	and	esi,		ecx
	add	ebp,		edi
	mov	edi,		DWORD PTR 48[esp]
	xor	esi,		edx
	ror	ecx,		2
	lea	ebp,		DWORD PTR 1518500249[edi*1+ebp]
	add	ebp,		esi
	; 00_15 13
	mov	edi,		ecx
	mov	esi,		ebp
	rol	ebp,		5
	xor	edi,		ebx
	and	edi,		eax
	add	ebp,		edx
	mov	edx,		DWORD PTR 52[esp]
	xor	edi,		ebx
	ror	eax,		2
	lea	ebp,		DWORD PTR 1518500249[edx*1+ebp]
	add	ebp,		edi
	; 00_15 14
	mov	edx,		eax
	mov	edi,		ebp
	rol	ebp,		5
	xor	edx,		ecx
	and	edx,		esi
	add	ebp,		ebx
	mov	ebx,		DWORD PTR 56[esp]
	xor	edx,		ecx
	ror	esi,		2
	lea	ebp,		DWORD PTR 1518500249[ebx*1+ebp]
	add	ebp,		edx
	; 00_15 15
	mov	ebx,		esi
	mov	edx,		ebp
	rol	ebp,		5
	xor	ebx,		eax
	and	ebx,		edi
	add	ebp,		ecx
	mov	ecx,		DWORD PTR 60[esp]
	xor	ebx,		eax
	ror	edi,		2
	lea	ebp,		DWORD PTR 1518500249[ecx*1+ebp]
	add	ebx,		ebp
	; 16_19 16
	mov	ecx,		DWORD PTR 8[esp]
	mov	ebp,		edi
	xor	ecx,		DWORD PTR [esp]
	xor	ebp,		esi
	xor	ecx,		DWORD PTR 32[esp]
	and	ebp,		edx
	ror	edx,		2
	xor	ecx,		DWORD PTR 52[esp]
	rol	ecx,		1
	xor	ebp,		esi
	mov	DWORD PTR [esp],ecx
	lea	ecx,		DWORD PTR 1518500249[eax*1+ecx]
	mov	eax,		ebx
	rol	eax,		5
	add	ecx,		ebp
	add	ecx,		eax
	; 16_19 17
	mov	eax,		DWORD PTR 12[esp]
	mov	ebp,		edx
	xor	eax,		DWORD PTR 4[esp]
	xor	ebp,		edi
	xor	eax,		DWORD PTR 36[esp]
	and	ebp,		ebx
	ror	ebx,		2
	xor	eax,		DWORD PTR 56[esp]
	rol	eax,		1
	xor	ebp,		edi
	mov	DWORD PTR 4[esp],eax
	lea	eax,		DWORD PTR 1518500249[esi*1+eax]
	mov	esi,		ecx
	rol	esi,		5
	add	eax,		ebp
	add	eax,		esi
	; 16_19 18
	mov	esi,		DWORD PTR 16[esp]
	mov	ebp,		ebx
	xor	esi,		DWORD PTR 8[esp]
	xor	ebp,		edx
	xor	esi,		DWORD PTR 40[esp]
	and	ebp,		ecx
	ror	ecx,		2
	xor	esi,		DWORD PTR 60[esp]
	rol	esi,		1
	xor	ebp,		edx
	mov	DWORD PTR 8[esp],esi
	lea	esi,		DWORD PTR 1518500249[edi*1+esi]
	mov	edi,		eax
	rol	edi,		5
	add	esi,		ebp
	add	esi,		edi
	; 16_19 19
	mov	edi,		DWORD PTR 20[esp]
	mov	ebp,		ecx
	xor	edi,		DWORD PTR 12[esp]
	xor	ebp,		ebx
	xor	edi,		DWORD PTR 44[esp]
	and	ebp,		eax
	ror	eax,		2
	xor	edi,		DWORD PTR [esp]
	rol	edi,		1
	xor	ebp,		ebx
	mov	DWORD PTR 12[esp],edi
	lea	edi,		DWORD PTR 1518500249[edx*1+edi]
	mov	edx,		esi
	rol	edx,		5
	add	edi,		ebp
	add	edi,		edx
	; 20_39 20
	mov	ebp,		esi
	mov	edx,		DWORD PTR 16[esp]
	ror	esi,		2
	xor	edx,		DWORD PTR 24[esp]
	xor	ebp,		eax
	xor	edx,		DWORD PTR 48[esp]
	xor	ebp,		ecx
	xor	edx,		DWORD PTR 4[esp]
	rol	edx,		1
	add	ebp,		ebx
	mov	DWORD PTR 16[esp],edx
	mov	ebx,		edi
	rol	ebx,		5
	lea	edx,		DWORD PTR 1859775393[ebp*1+edx]
	add	edx,		ebx
	; 20_39 21
	mov	ebp,		edi
	mov	ebx,		DWORD PTR 20[esp]
	ror	edi,		2
	xor	ebx,		DWORD PTR 28[esp]
	xor	ebp,		esi
	xor	ebx,		DWORD PTR 52[esp]
	xor	ebp,		eax
	xor	ebx,		DWORD PTR 8[esp]
	rol	ebx,		1
	add	ebp,		ecx
	mov	DWORD PTR 20[esp],ebx
	mov	ecx,		edx
	rol	ecx,		5
	lea	ebx,		DWORD PTR 1859775393[ebp*1+ebx]
	add	ebx,		ecx
	; 20_39 22
	mov	ebp,		edx
	mov	ecx,		DWORD PTR 24[esp]
	ror	edx,		2
	xor	ecx,		DWORD PTR 32[esp]
	xor	ebp,		edi
	xor	ecx,		DWORD PTR 56[esp]
	xor	ebp,		esi
	xor	ecx,		DWORD PTR 12[esp]
	rol	ecx,		1
	add	ebp,		eax
	mov	DWORD PTR 24[esp],ecx
	mov	eax,		ebx
	rol	eax,		5
	lea	ecx,		DWORD PTR 1859775393[ebp*1+ecx]
	add	ecx,		eax
	; 20_39 23
	mov	ebp,		ebx
	mov	eax,		DWORD PTR 28[esp]
	ror	ebx,		2
	xor	eax,		DWORD PTR 36[esp]
	xor	ebp,		edx
	xor	eax,		DWORD PTR 60[esp]
	xor	ebp,		edi
	xor	eax,		DWORD PTR 16[esp]
	rol	eax,		1
	add	ebp,		esi
	mov	DWORD PTR 28[esp],eax
	mov	esi,		ecx
	rol	esi,		5
	lea	eax,		DWORD PTR 1859775393[ebp*1+eax]
	add	eax,		esi
	; 20_39 24
	mov	ebp,		ecx
	mov	esi,		DWORD PTR 32[esp]
	ror	ecx,		2
	xor	esi,		DWORD PTR 40[esp]
	xor	ebp,		ebx
	xor	esi,		DWORD PTR [esp]
	xor	ebp,		edx
	xor	esi,		DWORD PTR 20[esp]
	rol	esi,		1
	add	ebp,		edi
	mov	DWORD PTR 32[esp],esi
	mov	edi,		eax
	rol	edi,		5
	lea	esi,		DWORD PTR 1859775393[ebp*1+esi]
	add	esi,		edi
	; 20_39 25
	mov	ebp,		eax
	mov	edi,		DWORD PTR 36[esp]
	ror	eax,		2
	xor	edi,		DWORD PTR 44[esp]
	xor	ebp,		ecx
	xor	edi,		DWORD PTR 4[esp]
	xor	ebp,		ebx
	xor	edi,		DWORD PTR 24[esp]
	rol	edi,		1
	add	ebp,		edx
	mov	DWORD PTR 36[esp],edi
	mov	edx,		esi
	rol	edx,		5
	lea	edi,		DWORD PTR 1859775393[ebp*1+edi]
	add	edi,		edx
	; 20_39 26
	mov	ebp,		esi
	mov	edx,		DWORD PTR 40[esp]
	ror	esi,		2
	xor	edx,		DWORD PTR 48[esp]
	xor	ebp,		eax
	xor	edx,		DWORD PTR 8[esp]
	xor	ebp,		ecx
	xor	edx,		DWORD PTR 28[esp]
	rol	edx,		1
	add	ebp,		ebx
	mov	DWORD PTR 40[esp],edx
	mov	ebx,		edi
	rol	ebx,		5
	lea	edx,		DWORD PTR 1859775393[ebp*1+edx]
	add	edx,		ebx
	; 20_39 27
	mov	ebp,		edi
	mov	ebx,		DWORD PTR 44[esp]
	ror	edi,		2
	xor	ebx,		DWORD PTR 52[esp]
	xor	ebp,		esi
	xor	ebx,		DWORD PTR 12[esp]
	xor	ebp,		eax
	xor	ebx,		DWORD PTR 32[esp]
	rol	ebx,		1
	add	ebp,		ecx
	mov	DWORD PTR 44[esp],ebx
	mov	ecx,		edx
	rol	ecx,		5
	lea	ebx,		DWORD PTR 1859775393[ebp*1+ebx]
	add	ebx,		ecx
	; 20_39 28
	mov	ebp,		edx
	mov	ecx,		DWORD PTR 48[esp]
	ror	edx,		2
	xor	ecx,		DWORD PTR 56[esp]
	xor	ebp,		edi
	xor	ecx,		DWORD PTR 16[esp]
	xor	ebp,		esi
	xor	ecx,		DWORD PTR 36[esp]
	rol	ecx,		1
	add	ebp,		eax
	mov	DWORD PTR 48[esp],ecx
	mov	eax,		ebx
	rol	eax,		5
	lea	ecx,		DWORD PTR 1859775393[ebp*1+ecx]
	add	ecx,		eax
	; 20_39 29
	mov	ebp,		ebx
	mov	eax,		DWORD PTR 52[esp]
	ror	ebx,		2
	xor	eax,		DWORD PTR 60[esp]
	xor	ebp,		edx
	xor	eax,		DWORD PTR 20[esp]
	xor	ebp,		edi
	xor	eax,		DWORD PTR 40[esp]
	rol	eax,		1
	add	ebp,		esi
	mov	DWORD PTR 52[esp],eax
	mov	esi,		ecx
	rol	esi,		5
	lea	eax,		DWORD PTR 1859775393[ebp*1+eax]
	add	eax,		esi
	; 20_39 30
	mov	ebp,		ecx
	mov	esi,		DWORD PTR 56[esp]
	ror	ecx,		2
	xor	esi,		DWORD PTR [esp]
	xor	ebp,		ebx
	xor	esi,		DWORD PTR 24[esp]
	xor	ebp,		edx
	xor	esi,		DWORD PTR 44[esp]
	rol	esi,		1
	add	ebp,		edi
	mov	DWORD PTR 56[esp],esi
	mov	edi,		eax
	rol	edi,		5
	lea	esi,		DWORD PTR 1859775393[ebp*1+esi]
	add	esi,		edi
	; 20_39 31
	mov	ebp,		eax
	mov	edi,		DWORD PTR 60[esp]
	ror	eax,		2
	xor	edi,		DWORD PTR 4[esp]
	xor	ebp,		ecx
	xor	edi,		DWORD PTR 28[esp]
	xor	ebp,		ebx
	xor	edi,		DWORD PTR 48[esp]
	rol	edi,		1
	add	ebp,		edx
	mov	DWORD PTR 60[esp],edi
	mov	edx,		esi
	rol	edx,		5
	lea	edi,		DWORD PTR 1859775393[ebp*1+edi]
	add	edi,		edx
	; 20_39 32
	mov	ebp,		esi
	mov	edx,		DWORD PTR [esp]
	ror	esi,		2
	xor	edx,		DWORD PTR 8[esp]
	xor	ebp,		eax
	xor	edx,		DWORD PTR 32[esp]
	xor	ebp,		ecx
	xor	edx,		DWORD PTR 52[esp]
	rol	edx,		1
	add	ebp,		ebx
	mov	DWORD PTR [esp],edx
	mov	ebx,		edi
	rol	ebx,		5
	lea	edx,		DWORD PTR 1859775393[ebp*1+edx]
	add	edx,		ebx
	; 20_39 33
	mov	ebp,		edi
	mov	ebx,		DWORD PTR 4[esp]
	ror	edi,		2
	xor	ebx,		DWORD PTR 12[esp]
	xor	ebp,		esi
	xor	ebx,		DWORD PTR 36[esp]
	xor	ebp,		eax
	xor	ebx,		DWORD PTR 56[esp]
	rol	ebx,		1
	add	ebp,		ecx
	mov	DWORD PTR 4[esp],ebx
	mov	ecx,		edx
	rol	ecx,		5
	lea	ebx,		DWORD PTR 1859775393[ebp*1+ebx]
	add	ebx,		ecx
	; 20_39 34
	mov	ebp,		edx
	mov	ecx,		DWORD PTR 8[esp]
	ror	edx,		2
	xor	ecx,		DWORD PTR 16[esp]
	xor	ebp,		edi
	xor	ecx,		DWORD PTR 40[esp]
	xor	ebp,		esi
	xor	ecx,		DWORD PTR 60[esp]
	rol	ecx,		1
	add	ebp,		eax
	mov	DWORD PTR 8[esp],ecx
	mov	eax,		ebx
	rol	eax,		5
	lea	ecx,		DWORD PTR 1859775393[ebp*1+ecx]
	add	ecx,		eax
	; 20_39 35
	mov	ebp,		ebx
	mov	eax,		DWORD PTR 12[esp]
	ror	ebx,		2
	xor	eax,		DWORD PTR 20[esp]
	xor	ebp,		edx
	xor	eax,		DWORD PTR 44[esp]
	xor	ebp,		edi
	xor	eax,		DWORD PTR [esp]
	rol	eax,		1
	add	ebp,		esi
	mov	DWORD PTR 12[esp],eax
	mov	esi,		ecx
	rol	esi,		5
	lea	eax,		DWORD PTR 1859775393[ebp*1+eax]
	add	eax,		esi
	; 20_39 36
	mov	ebp,		ecx
	mov	esi,		DWORD PTR 16[esp]
	ror	ecx,		2
	xor	esi,		DWORD PTR 24[esp]
	xor	ebp,		ebx
	xor	esi,		DWORD PTR 48[esp]
	xor	ebp,		edx
	xor	esi,		DWORD PTR 4[esp]
	rol	esi,		1
	add	ebp,		edi
	mov	DWORD PTR 16[esp],esi
	mov	edi,		eax
	rol	edi,		5
	lea	esi,		DWORD PTR 1859775393[ebp*1+esi]
	add	esi,		edi
	; 20_39 37
	mov	ebp,		eax
	mov	edi,		DWORD PTR 20[esp]
	ror	eax,		2
	xor	edi,		DWORD PTR 28[esp]
	xor	ebp,		ecx
	xor	edi,		DWORD PTR 52[esp]
	xor	ebp,		ebx
	xor	edi,		DWORD PTR 8[esp]
	rol	edi,		1
	add	ebp,		edx
	mov	DWORD PTR 20[esp],edi
	mov	edx,		esi
	rol	edx,		5
	lea	edi,		DWORD PTR 1859775393[ebp*1+edi]
	add	edi,		edx
	; 20_39 38
	mov	ebp,		esi
	mov	edx,		DWORD PTR 24[esp]
	ror	esi,		2
	xor	edx,		DWORD PTR 32[esp]
	xor	ebp,		eax
	xor	edx,		DWORD PTR 56[esp]
	xor	ebp,		ecx
	xor	edx,		DWORD PTR 12[esp]
	rol	edx,		1
	add	ebp,		ebx
	mov	DWORD PTR 24[esp],edx
	mov	ebx,		edi
	rol	ebx,		5
	lea	edx,		DWORD PTR 1859775393[ebp*1+edx]
	add	edx,		ebx
	; 20_39 39
	mov	ebp,		edi
	mov	ebx,		DWORD PTR 28[esp]
	ror	edi,		2
	xor	ebx,		DWORD PTR 36[esp]
	xor	ebp,		esi
	xor	ebx,		DWORD PTR 60[esp]
	xor	ebp,		eax
	xor	ebx,		DWORD PTR 16[esp]
	rol	ebx,		1
	add	ebp,		ecx
	mov	DWORD PTR 28[esp],ebx
	mov	ecx,		edx
	rol	ecx,		5
	lea	ebx,		DWORD PTR 1859775393[ebp*1+ebx]
	add	ebx,		ecx
	; 40_59 40
	mov	ecx,		DWORD PTR 32[esp]
	mov	ebp,		DWORD PTR 40[esp]
	xor	ecx,		ebp
	mov	ebp,		DWORD PTR [esp]
	xor	ecx,		ebp
	mov	ebp,		DWORD PTR 20[esp]
	xor	ecx,		ebp
	mov	ebp,		edx
	rol	ecx,		1
	or	ebp,		edi
	mov	DWORD PTR 32[esp],ecx
	and	ebp,		esi
	lea	ecx,		DWORD PTR 2400959708[eax*1+ecx]
	mov	eax,		edx
	ror	edx,		2
	and	eax,		edi
	or	ebp,		eax
	mov	eax,		ebx
	rol	eax,		5
	add	ecx,		ebp
	add	ecx,		eax
	; 40_59 41
	mov	eax,		DWORD PTR 36[esp]
	mov	ebp,		DWORD PTR 44[esp]
	xor	eax,		ebp
	mov	ebp,		DWORD PTR 4[esp]
	xor	eax,		ebp
	mov	ebp,		DWORD PTR 24[esp]
	xor	eax,		ebp
	mov	ebp,		ebx
	rol	eax,		1
	or	ebp,		edx
	mov	DWORD PTR 36[esp],eax
	and	ebp,		edi
	lea	eax,		DWORD PTR 2400959708[esi*1+eax]
	mov	esi,		ebx
	ror	ebx,		2
	and	esi,		edx
	or	ebp,		esi
	mov	esi,		ecx
	rol	esi,		5
	add	eax,		ebp
	add	eax,		esi
	; 40_59 42
	mov	esi,		DWORD PTR 40[esp]
	mov	ebp,		DWORD PTR 48[esp]
	xor	esi,		ebp
	mov	ebp,		DWORD PTR 8[esp]
	xor	esi,		ebp
	mov	ebp,		DWORD PTR 28[esp]
	xor	esi,		ebp
	mov	ebp,		ecx
	rol	esi,		1
	or	ebp,		ebx
	mov	DWORD PTR 40[esp],esi
	and	ebp,		edx
	lea	esi,		DWORD PTR 2400959708[edi*1+esi]
	mov	edi,		ecx
	ror	ecx,		2
	and	edi,		ebx
	or	ebp,		edi
	mov	edi,		eax
	rol	edi,		5
	add	esi,		ebp
	add	esi,		edi
	; 40_59 43
	mov	edi,		DWORD PTR 44[esp]
	mov	ebp,		DWORD PTR 52[esp]
	xor	edi,		ebp
	mov	ebp,		DWORD PTR 12[esp]
	xor	edi,		ebp
	mov	ebp,		DWORD PTR 32[esp]
	xor	edi,		ebp
	mov	ebp,		eax
	rol	edi,		1
	or	ebp,		ecx
	mov	DWORD PTR 44[esp],edi
	and	ebp,		ebx
	lea	edi,		DWORD PTR 2400959708[edx*1+edi]
	mov	edx,		eax
	ror	eax,		2
	and	edx,		ecx
	or	ebp,		edx
	mov	edx,		esi
	rol	edx,		5
	add	edi,		ebp
	add	edi,		edx
	; 40_59 44
	mov	edx,		DWORD PTR 48[esp]
	mov	ebp,		DWORD PTR 56[esp]
	xor	edx,		ebp
	mov	ebp,		DWORD PTR 16[esp]
	xor	edx,		ebp
	mov	ebp,		DWORD PTR 36[esp]
	xor	edx,		ebp
	mov	ebp,		esi
	rol	edx,		1
	or	ebp,		eax
	mov	DWORD PTR 48[esp],edx
	and	ebp,		ecx
	lea	edx,		DWORD PTR 2400959708[ebx*1+edx]
	mov	ebx,		esi
	ror	esi,		2
	and	ebx,		eax
	or	ebp,		ebx
	mov	ebx,		edi
	rol	ebx,		5
	add	edx,		ebp
	add	edx,		ebx
	; 40_59 45
	mov	ebx,		DWORD PTR 52[esp]
	mov	ebp,		DWORD PTR 60[esp]
	xor	ebx,		ebp
	mov	ebp,		DWORD PTR 20[esp]
	xor	ebx,		ebp
	mov	ebp,		DWORD PTR 40[esp]
	xor	ebx,		ebp
	mov	ebp,		edi
	rol	ebx,		1
	or	ebp,		esi
	mov	DWORD PTR 52[esp],ebx
	and	ebp,		eax
	lea	ebx,		DWORD PTR 2400959708[ecx*1+ebx]
	mov	ecx,		edi
	ror	edi,		2
	and	ecx,		esi
	or	ebp,		ecx
	mov	ecx,		edx
	rol	ecx,		5
	add	ebx,		ebp
	add	ebx,		ecx
	; 40_59 46
	mov	ecx,		DWORD PTR 56[esp]
	mov	ebp,		DWORD PTR [esp]
	xor	ecx,		ebp
	mov	ebp,		DWORD PTR 24[esp]
	xor	ecx,		ebp
	mov	ebp,		DWORD PTR 44[esp]
	xor	ecx,		ebp
	mov	ebp,		edx
	rol	ecx,		1
	or	ebp,		edi
	mov	DWORD PTR 56[esp],ecx
	and	ebp,		esi
	lea	ecx,		DWORD PTR 2400959708[eax*1+ecx]
	mov	eax,		edx
	ror	edx,		2
	and	eax,		edi
	or	ebp,		eax
	mov	eax,		ebx
	rol	eax,		5
	add	ecx,		ebp
	add	ecx,		eax
	; 40_59 47
	mov	eax,		DWORD PTR 60[esp]
	mov	ebp,		DWORD PTR 4[esp]
	xor	eax,		ebp
	mov	ebp,		DWORD PTR 28[esp]
	xor	eax,		ebp
	mov	ebp,		DWORD PTR 48[esp]
	xor	eax,		ebp
	mov	ebp,		ebx
	rol	eax,		1
	or	ebp,		edx
	mov	DWORD PTR 60[esp],eax
	and	ebp,		edi
	lea	eax,		DWORD PTR 2400959708[esi*1+eax]
	mov	esi,		ebx
	ror	ebx,		2
	and	esi,		edx
	or	ebp,		esi
	mov	esi,		ecx
	rol	esi,		5
	add	eax,		ebp
	add	eax,		esi
	; 40_59 48
	mov	esi,		DWORD PTR [esp]
	mov	ebp,		DWORD PTR 8[esp]
	xor	esi,		ebp
	mov	ebp,		DWORD PTR 32[esp]
	xor	esi,		ebp
	mov	ebp,		DWORD PTR 52[esp]
	xor	esi,		ebp
	mov	ebp,		ecx
	rol	esi,		1
	or	ebp,		ebx
	mov	DWORD PTR [esp],esi
	and	ebp,		edx
	lea	esi,		DWORD PTR 2400959708[edi*1+esi]
	mov	edi,		ecx
	ror	ecx,		2
	and	edi,		ebx
	or	ebp,		edi
	mov	edi,		eax
	rol	edi,		5
	add	esi,		ebp
	add	esi,		edi
	; 40_59 49
	mov	edi,		DWORD PTR 4[esp]
	mov	ebp,		DWORD PTR 12[esp]
	xor	edi,		ebp
	mov	ebp,		DWORD PTR 36[esp]
	xor	edi,		ebp
	mov	ebp,		DWORD PTR 56[esp]
	xor	edi,		ebp
	mov	ebp,		eax
	rol	edi,		1
	or	ebp,		ecx
	mov	DWORD PTR 4[esp],edi
	and	ebp,		ebx
	lea	edi,		DWORD PTR 2400959708[edx*1+edi]
	mov	edx,		eax
	ror	eax,		2
	and	edx,		ecx
	or	ebp,		edx
	mov	edx,		esi
	rol	edx,		5
	add	edi,		ebp
	add	edi,		edx
	; 40_59 50
	mov	edx,		DWORD PTR 8[esp]
	mov	ebp,		DWORD PTR 16[esp]
	xor	edx,		ebp
	mov	ebp,		DWORD PTR 40[esp]
	xor	edx,		ebp
	mov	ebp,		DWORD PTR 60[esp]
	xor	edx,		ebp
	mov	ebp,		esi
	rol	edx,		1
	or	ebp,		eax
	mov	DWORD PTR 8[esp],edx
	and	ebp,		ecx
	lea	edx,		DWORD PTR 2400959708[ebx*1+edx]
	mov	ebx,		esi
	ror	esi,		2
	and	ebx,		eax
	or	ebp,		ebx
	mov	ebx,		edi
	rol	ebx,		5
	add	edx,		ebp
	add	edx,		ebx
	; 40_59 51
	mov	ebx,		DWORD PTR 12[esp]
	mov	ebp,		DWORD PTR 20[esp]
	xor	ebx,		ebp
	mov	ebp,		DWORD PTR 44[esp]
	xor	ebx,		ebp
	mov	ebp,		DWORD PTR [esp]
	xor	ebx,		ebp
	mov	ebp,		edi
	rol	ebx,		1
	or	ebp,		esi
	mov	DWORD PTR 12[esp],ebx
	and	ebp,		eax
	lea	ebx,		DWORD PTR 2400959708[ecx*1+ebx]
	mov	ecx,		edi
	ror	edi,		2
	and	ecx,		esi
	or	ebp,		ecx
	mov	ecx,		edx
	rol	ecx,		5
	add	ebx,		ebp
	add	ebx,		ecx
	; 40_59 52
	mov	ecx,		DWORD PTR 16[esp]
	mov	ebp,		DWORD PTR 24[esp]
	xor	ecx,		ebp
	mov	ebp,		DWORD PTR 48[esp]
	xor	ecx,		ebp
	mov	ebp,		DWORD PTR 4[esp]
	xor	ecx,		ebp
	mov	ebp,		edx
	rol	ecx,		1
	or	ebp,		edi
	mov	DWORD PTR 16[esp],ecx
	and	ebp,		esi
	lea	ecx,		DWORD PTR 2400959708[eax*1+ecx]
	mov	eax,		edx
	ror	edx,		2
	and	eax,		edi
	or	ebp,		eax
	mov	eax,		ebx
	rol	eax,		5
	add	ecx,		ebp
	add	ecx,		eax
	; 40_59 53
	mov	eax,		DWORD PTR 20[esp]
	mov	ebp,		DWORD PTR 28[esp]
	xor	eax,		ebp
	mov	ebp,		DWORD PTR 52[esp]
	xor	eax,		ebp
	mov	ebp,		DWORD PTR 8[esp]
	xor	eax,		ebp
	mov	ebp,		ebx
	rol	eax,		1
	or	ebp,		edx
	mov	DWORD PTR 20[esp],eax
	and	ebp,		edi
	lea	eax,		DWORD PTR 2400959708[esi*1+eax]
	mov	esi,		ebx
	ror	ebx,		2
	and	esi,		edx
	or	ebp,		esi
	mov	esi,		ecx
	rol	esi,		5
	add	eax,		ebp
	add	eax,		esi
	; 40_59 54
	mov	esi,		DWORD PTR 24[esp]
	mov	ebp,		DWORD PTR 32[esp]
	xor	esi,		ebp
	mov	ebp,		DWORD PTR 56[esp]
	xor	esi,		ebp
	mov	ebp,		DWORD PTR 12[esp]
	xor	esi,		ebp
	mov	ebp,		ecx
	rol	esi,		1
	or	ebp,		ebx
	mov	DWORD PTR 24[esp],esi
	and	ebp,		edx
	lea	esi,		DWORD PTR 2400959708[edi*1+esi]
	mov	edi,		ecx
	ror	ecx,		2
	and	edi,		ebx
	or	ebp,		edi
	mov	edi,		eax
	rol	edi,		5
	add	esi,		ebp
	add	esi,		edi
	; 40_59 55
	mov	edi,		DWORD PTR 28[esp]
	mov	ebp,		DWORD PTR 36[esp]
	xor	edi,		ebp
	mov	ebp,		DWORD PTR 60[esp]
	xor	edi,		ebp
	mov	ebp,		DWORD PTR 16[esp]
	xor	edi,		ebp
	mov	ebp,		eax
	rol	edi,		1
	or	ebp,		ecx
	mov	DWORD PTR 28[esp],edi
	and	ebp,		ebx
	lea	edi,		DWORD PTR 2400959708[edx*1+edi]
	mov	edx,		eax
	ror	eax,		2
	and	edx,		ecx
	or	ebp,		edx
	mov	edx,		esi
	rol	edx,		5
	add	edi,		ebp
	add	edi,		edx
	; 40_59 56
	mov	edx,		DWORD PTR 32[esp]
	mov	ebp,		DWORD PTR 40[esp]
	xor	edx,		ebp
	mov	ebp,		DWORD PTR [esp]
	xor	edx,		ebp
	mov	ebp,		DWORD PTR 20[esp]
	xor	edx,		ebp
	mov	ebp,		esi
	rol	edx,		1
	or	ebp,		eax
	mov	DWORD PTR 32[esp],edx
	and	ebp,		ecx
	lea	edx,		DWORD PTR 2400959708[ebx*1+edx]
	mov	ebx,		esi
	ror	esi,		2
	and	ebx,		eax
	or	ebp,		ebx
	mov	ebx,		edi
	rol	ebx,		5
	add	edx,		ebp
	add	edx,		ebx
	; 40_59 57
	mov	ebx,		DWORD PTR 36[esp]
	mov	ebp,		DWORD PTR 44[esp]
	xor	ebx,		ebp
	mov	ebp,		DWORD PTR 4[esp]
	xor	ebx,		ebp
	mov	ebp,		DWORD PTR 24[esp]
	xor	ebx,		ebp
	mov	ebp,		edi
	rol	ebx,		1
	or	ebp,		esi
	mov	DWORD PTR 36[esp],ebx
	and	ebp,		eax
	lea	ebx,		DWORD PTR 2400959708[ecx*1+ebx]
	mov	ecx,		edi
	ror	edi,		2
	and	ecx,		esi
	or	ebp,		ecx
	mov	ecx,		edx
	rol	ecx,		5
	add	ebx,		ebp
	add	ebx,		ecx
	; 40_59 58
	mov	ecx,		DWORD PTR 40[esp]
	mov	ebp,		DWORD PTR 48[esp]
	xor	ecx,		ebp
	mov	ebp,		DWORD PTR 8[esp]
	xor	ecx,		ebp
	mov	ebp,		DWORD PTR 28[esp]
	xor	ecx,		ebp
	mov	ebp,		edx
	rol	ecx,		1
	or	ebp,		edi
	mov	DWORD PTR 40[esp],ecx
	and	ebp,		esi
	lea	ecx,		DWORD PTR 2400959708[eax*1+ecx]
	mov	eax,		edx
	ror	edx,		2
	and	eax,		edi
	or	ebp,		eax
	mov	eax,		ebx
	rol	eax,		5
	add	ecx,		ebp
	add	ecx,		eax
	; 40_59 59
	mov	eax,		DWORD PTR 44[esp]
	mov	ebp,		DWORD PTR 52[esp]
	xor	eax,		ebp
	mov	ebp,		DWORD PTR 12[esp]
	xor	eax,		ebp
	mov	ebp,		DWORD PTR 32[esp]
	xor	eax,		ebp
	mov	ebp,		ebx
	rol	eax,		1
	or	ebp,		edx
	mov	DWORD PTR 44[esp],eax
	and	ebp,		edi
	lea	eax,		DWORD PTR 2400959708[esi*1+eax]
	mov	esi,		ebx
	ror	ebx,		2
	and	esi,		edx
	or	ebp,		esi
	mov	esi,		ecx
	rol	esi,		5
	add	eax,		ebp
	add	eax,		esi
	; 20_39 60
	mov	ebp,		ecx
	mov	esi,		DWORD PTR 48[esp]
	ror	ecx,		2
	xor	esi,		DWORD PTR 56[esp]
	xor	ebp,		ebx
	xor	esi,		DWORD PTR 16[esp]
	xor	ebp,		edx
	xor	esi,		DWORD PTR 36[esp]
	rol	esi,		1
	add	ebp,		edi
	mov	DWORD PTR 48[esp],esi
	mov	edi,		eax
	rol	edi,		5
	lea	esi,		DWORD PTR 3395469782[ebp*1+esi]
	add	esi,		edi
	; 20_39 61
	mov	ebp,		eax
	mov	edi,		DWORD PTR 52[esp]
	ror	eax,		2
	xor	edi,		DWORD PTR 60[esp]
	xor	ebp,		ecx
	xor	edi,		DWORD PTR 20[esp]
	xor	ebp,		ebx
	xor	edi,		DWORD PTR 40[esp]
	rol	edi,		1
	add	ebp,		edx
	mov	DWORD PTR 52[esp],edi
	mov	edx,		esi
	rol	edx,		5
	lea	edi,		DWORD PTR 3395469782[ebp*1+edi]
	add	edi,		edx
	; 20_39 62
	mov	ebp,		esi
	mov	edx,		DWORD PTR 56[esp]
	ror	esi,		2
	xor	edx,		DWORD PTR [esp]
	xor	ebp,		eax
	xor	edx,		DWORD PTR 24[esp]
	xor	ebp,		ecx
	xor	edx,		DWORD PTR 44[esp]
	rol	edx,		1
	add	ebp,		ebx
	mov	DWORD PTR 56[esp],edx
	mov	ebx,		edi
	rol	ebx,		5
	lea	edx,		DWORD PTR 3395469782[ebp*1+edx]
	add	edx,		ebx
	; 20_39 63
	mov	ebp,		edi
	mov	ebx,		DWORD PTR 60[esp]
	ror	edi,		2
	xor	ebx,		DWORD PTR 4[esp]
	xor	ebp,		esi
	xor	ebx,		DWORD PTR 28[esp]
	xor	ebp,		eax
	xor	ebx,		DWORD PTR 48[esp]
	rol	ebx,		1
	add	ebp,		ecx
	mov	DWORD PTR 60[esp],ebx
	mov	ecx,		edx
	rol	ecx,		5
	lea	ebx,		DWORD PTR 3395469782[ebp*1+ebx]
	add	ebx,		ecx
	; 20_39 64
	mov	ebp,		edx
	mov	ecx,		DWORD PTR [esp]
	ror	edx,		2
	xor	ecx,		DWORD PTR 8[esp]
	xor	ebp,		edi
	xor	ecx,		DWORD PTR 32[esp]
	xor	ebp,		esi
	xor	ecx,		DWORD PTR 52[esp]
	rol	ecx,		1
	add	ebp,		eax
	mov	DWORD PTR [esp],ecx
	mov	eax,		ebx
	rol	eax,		5
	lea	ecx,		DWORD PTR 3395469782[ebp*1+ecx]
	add	ecx,		eax
	; 20_39 65
	mov	ebp,		ebx
	mov	eax,		DWORD PTR 4[esp]
	ror	ebx,		2
	xor	eax,		DWORD PTR 12[esp]
	xor	ebp,		edx
	xor	eax,		DWORD PTR 36[esp]
	xor	ebp,		edi
	xor	eax,		DWORD PTR 56[esp]
	rol	eax,		1
	add	ebp,		esi
	mov	DWORD PTR 4[esp],eax
	mov	esi,		ecx
	rol	esi,		5
	lea	eax,		DWORD PTR 3395469782[ebp*1+eax]
	add	eax,		esi
	; 20_39 66
	mov	ebp,		ecx
	mov	esi,		DWORD PTR 8[esp]
	ror	ecx,		2
	xor	esi,		DWORD PTR 16[esp]
	xor	ebp,		ebx
	xor	esi,		DWORD PTR 40[esp]
	xor	ebp,		edx
	xor	esi,		DWORD PTR 60[esp]
	rol	esi,		1
	add	ebp,		edi
	mov	DWORD PTR 8[esp],esi
	mov	edi,		eax
	rol	edi,		5
	lea	esi,		DWORD PTR 3395469782[ebp*1+esi]
	add	esi,		edi
	; 20_39 67
	mov	ebp,		eax
	mov	edi,		DWORD PTR 12[esp]
	ror	eax,		2
	xor	edi,		DWORD PTR 20[esp]
	xor	ebp,		ecx
	xor	edi,		DWORD PTR 44[esp]
	xor	ebp,		ebx
	xor	edi,		DWORD PTR [esp]
	rol	edi,		1
	add	ebp,		edx
	mov	DWORD PTR 12[esp],edi
	mov	edx,		esi
	rol	edx,		5
	lea	edi,		DWORD PTR 3395469782[ebp*1+edi]
	add	edi,		edx
	; 20_39 68
	mov	ebp,		esi
	mov	edx,		DWORD PTR 16[esp]
	ror	esi,		2
	xor	edx,		DWORD PTR 24[esp]
	xor	ebp,		eax
	xor	edx,		DWORD PTR 48[esp]
	xor	ebp,		ecx
	xor	edx,		DWORD PTR 4[esp]
	rol	edx,		1
	add	ebp,		ebx
	mov	DWORD PTR 16[esp],edx
	mov	ebx,		edi
	rol	ebx,		5
	lea	edx,		DWORD PTR 3395469782[ebp*1+edx]
	add	edx,		ebx
	; 20_39 69
	mov	ebp,		edi
	mov	ebx,		DWORD PTR 20[esp]
	ror	edi,		2
	xor	ebx,		DWORD PTR 28[esp]
	xor	ebp,		esi
	xor	ebx,		DWORD PTR 52[esp]
	xor	ebp,		eax
	xor	ebx,		DWORD PTR 8[esp]
	rol	ebx,		1
	add	ebp,		ecx
	mov	DWORD PTR 20[esp],ebx
	mov	ecx,		edx
	rol	ecx,		5
	lea	ebx,		DWORD PTR 3395469782[ebp*1+ebx]
	add	ebx,		ecx
	; 20_39 70
	mov	ebp,		edx
	mov	ecx,		DWORD PTR 24[esp]
	ror	edx,		2
	xor	ecx,		DWORD PTR 32[esp]
	xor	ebp,		edi
	xor	ecx,		DWORD PTR 56[esp]
	xor	ebp,		esi
	xor	ecx,		DWORD PTR 12[esp]
	rol	ecx,		1
	add	ebp,		eax
	mov	DWORD PTR 24[esp],ecx
	mov	eax,		ebx
	rol	eax,		5
	lea	ecx,		DWORD PTR 3395469782[ebp*1+ecx]
	add	ecx,		eax
	; 20_39 71
	mov	ebp,		ebx
	mov	eax,		DWORD PTR 28[esp]
	ror	ebx,		2
	xor	eax,		DWORD PTR 36[esp]
	xor	ebp,		edx
	xor	eax,		DWORD PTR 60[esp]
	xor	ebp,		edi
	xor	eax,		DWORD PTR 16[esp]
	rol	eax,		1
	add	ebp,		esi
	mov	DWORD PTR 28[esp],eax
	mov	esi,		ecx
	rol	esi,		5
	lea	eax,		DWORD PTR 3395469782[ebp*1+eax]
	add	eax,		esi
	; 20_39 72
	mov	ebp,		ecx
	mov	esi,		DWORD PTR 32[esp]
	ror	ecx,		2
	xor	esi,		DWORD PTR 40[esp]
	xor	ebp,		ebx
	xor	esi,		DWORD PTR [esp]
	xor	ebp,		edx
	xor	esi,		DWORD PTR 20[esp]
	rol	esi,		1
	add	ebp,		edi
	mov	DWORD PTR 32[esp],esi
	mov	edi,		eax
	rol	edi,		5
	lea	esi,		DWORD PTR 3395469782[ebp*1+esi]
	add	esi,		edi
	; 20_39 73
	mov	ebp,		eax
	mov	edi,		DWORD PTR 36[esp]
	ror	eax,		2
	xor	edi,		DWORD PTR 44[esp]
	xor	ebp,		ecx
	xor	edi,		DWORD PTR 4[esp]
	xor	ebp,		ebx
	xor	edi,		DWORD PTR 24[esp]
	rol	edi,		1
	add	ebp,		edx
	mov	DWORD PTR 36[esp],edi
	mov	edx,		esi
	rol	edx,		5
	lea	edi,		DWORD PTR 3395469782[ebp*1+edi]
	add	edi,		edx
	; 20_39 74
	mov	ebp,		esi
	mov	edx,		DWORD PTR 40[esp]
	ror	esi,		2
	xor	edx,		DWORD PTR 48[esp]
	xor	ebp,		eax
	xor	edx,		DWORD PTR 8[esp]
	xor	ebp,		ecx
	xor	edx,		DWORD PTR 28[esp]
	rol	edx,		1
	add	ebp,		ebx
	mov	DWORD PTR 40[esp],edx
	mov	ebx,		edi
	rol	ebx,		5
	lea	edx,		DWORD PTR 3395469782[ebp*1+edx]
	add	edx,		ebx
	; 20_39 75
	mov	ebp,		edi
	mov	ebx,		DWORD PTR 44[esp]
	ror	edi,		2
	xor	ebx,		DWORD PTR 52[esp]
	xor	ebp,		esi
	xor	ebx,		DWORD PTR 12[esp]
	xor	ebp,		eax
	xor	ebx,		DWORD PTR 32[esp]
	rol	ebx,		1
	add	ebp,		ecx
	mov	DWORD PTR 44[esp],ebx
	mov	ecx,		edx
	rol	ecx,		5
	lea	ebx,		DWORD PTR 3395469782[ebp*1+ebx]
	add	ebx,		ecx
	; 20_39 76
	mov	ebp,		edx
	mov	ecx,		DWORD PTR 48[esp]
	ror	edx,		2
	xor	ecx,		DWORD PTR 56[esp]
	xor	ebp,		edi
	xor	ecx,		DWORD PTR 16[esp]
	xor	ebp,		esi
	xor	ecx,		DWORD PTR 36[esp]
	rol	ecx,		1
	add	ebp,		eax
	mov	DWORD PTR 48[esp],ecx
	mov	eax,		ebx
	rol	eax,		5
	lea	ecx,		DWORD PTR 3395469782[ebp*1+ecx]
	add	ecx,		eax
	; 20_39 77
	mov	ebp,		ebx
	mov	eax,		DWORD PTR 52[esp]
	ror	ebx,		2
	xor	eax,		DWORD PTR 60[esp]
	xor	ebp,		edx
	xor	eax,		DWORD PTR 20[esp]
	xor	ebp,		edi
	xor	eax,		DWORD PTR 40[esp]
	rol	eax,		1
	add	ebp,		esi
	mov	DWORD PTR 52[esp],eax
	mov	esi,		ecx
	rol	esi,		5
	lea	eax,		DWORD PTR 3395469782[ebp*1+eax]
	add	eax,		esi
	; 20_39 78
	mov	ebp,		ecx
	mov	esi,		DWORD PTR 56[esp]
	ror	ecx,		2
	xor	esi,		DWORD PTR [esp]
	xor	ebp,		ebx
	xor	esi,		DWORD PTR 24[esp]
	xor	ebp,		edx
	xor	esi,		DWORD PTR 44[esp]
	rol	esi,		1
	add	ebp,		edi
	mov	DWORD PTR 56[esp],esi
	mov	edi,		eax
	rol	edi,		5
	lea	esi,		DWORD PTR 3395469782[ebp*1+esi]
	add	esi,		edi
	; 20_39 79
	mov	ebp,		eax
	mov	edi,		DWORD PTR 60[esp]
	ror	eax,		2
	xor	edi,		DWORD PTR 4[esp]
	xor	ebp,		ecx
	xor	edi,		DWORD PTR 28[esp]
	xor	ebp,		ebx
	xor	edi,		DWORD PTR 48[esp]
	rol	edi,		1
	add	ebp,		edx
	mov	DWORD PTR 60[esp],edi
	mov	edx,		esi
	rol	edx,		5
	lea	edi,		DWORD PTR 3395469782[ebp*1+edi]
	add	edi,		edx
	; End processing
	; 
	mov	ebp,		DWORD PTR 128[esp]
	mov	edx,		DWORD PTR 12[ebp]
	add	edx,		ecx
	mov	ecx,		DWORD PTR 4[ebp]
	add	ecx,		esi
	mov	esi,		eax
	mov	eax,		DWORD PTR [ebp]
	mov	DWORD PTR 12[ebp],edx
	add	eax,		edi
	mov	edi,		DWORD PTR 16[ebp]
	add	edi,		ebx
	mov	ebx,		DWORD PTR 8[ebp]
	add	ebx,		esi
	mov	DWORD PTR [ebp],eax
	mov	esi,		DWORD PTR 132[esp]
	mov	DWORD PTR 8[ebp],ebx
	add	esi,		64
	mov	eax,		DWORD PTR 68[esp]
	mov	DWORD PTR 16[ebp],edi
	cmp	esi,		eax
	mov	DWORD PTR 4[ebp],ecx
	jb	$L000start
	add	esp,		108
	pop	edi
	pop	ebx
	pop	ebp
	pop	esi
	ret
_sha1_block_asm_data_order ENDP
PUBLIC	_sha1_block_asm_host_order

_sha1_block_asm_host_order PROC NEAR
	mov	ecx,		DWORD PTR 12[esp]
	push	esi
	shl	ecx,		6
	mov	esi,		DWORD PTR 12[esp]
	push	ebp
	add	ecx,		esi
	push	ebx
	mov	ebp,		DWORD PTR 16[esp]
	push	edi
	mov	edx,		DWORD PTR 12[ebp]
	sub	esp,		108
	mov	edi,		DWORD PTR 16[ebp]
	mov	ebx,		DWORD PTR 8[ebp]
	mov	DWORD PTR 68[esp],ecx
	; First we need to setup the X array
	mov	eax,		DWORD PTR [esi]
	mov	ecx,		DWORD PTR 4[esi]
	mov	DWORD PTR [esp],eax
	mov	DWORD PTR 4[esp],ecx
	mov	eax,		DWORD PTR 8[esi]
	mov	ecx,		DWORD PTR 12[esi]
	mov	DWORD PTR 8[esp],eax
	mov	DWORD PTR 12[esp],ecx
	mov	eax,		DWORD PTR 16[esi]
	mov	ecx,		DWORD PTR 20[esi]
	mov	DWORD PTR 16[esp],eax
	mov	DWORD PTR 20[esp],ecx
	mov	eax,		DWORD PTR 24[esi]
	mov	ecx,		DWORD PTR 28[esi]
	mov	DWORD PTR 24[esp],eax
	mov	DWORD PTR 28[esp],ecx
	mov	eax,		DWORD PTR 32[esi]
	mov	ecx,		DWORD PTR 36[esi]
	mov	DWORD PTR 32[esp],eax
	mov	DWORD PTR 36[esp],ecx
	mov	eax,		DWORD PTR 40[esi]
	mov	ecx,		DWORD PTR 44[esi]
	mov	DWORD PTR 40[esp],eax
	mov	DWORD PTR 44[esp],ecx
	mov	eax,		DWORD PTR 48[esi]
	mov	ecx,		DWORD PTR 52[esi]
	mov	DWORD PTR 48[esp],eax
	mov	DWORD PTR 52[esp],ecx
	mov	eax,		DWORD PTR 56[esi]
	mov	ecx,		DWORD PTR 60[esi]
	mov	DWORD PTR 56[esp],eax
	mov	DWORD PTR 60[esp],ecx
	jmp	$L001shortcut
_sha1_block_asm_host_order ENDP
_TEXT$	ENDS
END
