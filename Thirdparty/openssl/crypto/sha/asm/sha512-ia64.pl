#!/usr/bin/env perl
#
# ====================================================================
# Written by Andy Polyakov <appro@fy.chalmers.se> for the OpenSSL
# project. Rights for redistribution and usage in source and binary
# forms are granted according to the OpenSSL license.
# ====================================================================
#
# SHA256/512_Transform for Itanium.
#
# sha512_block runs in 1003 cycles on Itanium 2, which is almost 50%
# faster than gcc and >60%(!) faster than code generated by HP-UX
# compiler (yes, HP-UX is generating slower code, because unlike gcc,
# it failed to deploy "shift right pair," 'shrp' instruction, which
# substitutes for 64-bit rotate).
#
# 924 cycles long sha256_block outperforms gcc by over factor of 2(!)
# and HP-UX compiler - by >40% (yes, gcc won sha512_block, but lost
# this one big time). Note that "formally" 924 is about 100 cycles
# too much. I mean it's 64 32-bit rounds vs. 80 virtually identical
# 64-bit ones and 1003*64/80 gives 802. Extra cycles, 2 per round,
# are spent on extra work to provide for 32-bit rotations. 32-bit
# rotations are still handled by 'shrp' instruction and for this
# reason lower 32 bits are deposited to upper half of 64-bit register
# prior 'shrp' issue. And in order to minimize the amount of such
# operations, X[16] values are *maintained* with copies of lower
# halves in upper halves, which is why you'll spot such instructions
# as custom 'mux2', "parallel 32-bit add," 'padd4' and "parallel
# 32-bit unsigned right shift," 'pshr4.u' instructions here.
#
# Rules of engagement.
#
# There is only one integer shifter meaning that if I have two rotate,
# deposit or extract instructions in adjacent bundles, they shall
# split [at run-time if they have to]. But note that variable and
# parallel shifts are performed by multi-media ALU and *are* pairable
# with rotates [and alike]. On the backside MMALU is rather slow: it
# takes 2 extra cycles before the result of integer operation is
# available *to* MMALU and 2(*) extra cycles before the result of MM
# operation is available "back" *to* integer ALU, not to mention that
# MMALU itself has 2 cycles latency. However! I explicitly scheduled
# these MM instructions to avoid MM stalls, so that all these extra
# latencies get "hidden" in instruction-level parallelism.
#
# (*) 2 cycles on Itanium 1 and 1 cycle on Itanium 2. But I schedule
#     for 2 in order to provide for best *overall* performance,
#     because on Itanium 1 stall on MM result is accompanied by
#     pipeline flush, which takes 6 cycles:-(
#
# Resulting performance numbers for 900MHz Itanium 2 system:
#
# The 'numbers' are in 1000s of bytes per second processed.
# type     16 bytes    64 bytes   256 bytes  1024 bytes  8192 bytes
# sha1(*)   6210.14k   20376.30k   52447.83k   85870.05k  105478.12k
# sha256    7476.45k   20572.05k   41538.34k   56062.29k   62093.18k
# sha512    4996.56k   20026.28k   47597.20k   85278.79k  111501.31k
#
# (*) SHA1 numbers are for HP-UX compiler and are presented purely
#     for reference purposes. I bet it can improved too...
#
# To generate code, pass the file name with either 256 or 512 in its
# name and compiler flags.

$output=shift;

if ($output =~ /512.*\.[s|asm]/) {
	$SZ=8;
	$BITS=8*$SZ;
	$LDW="ld8";
	$STW="st8";
	$ADD="add";
	$SHRU="shr.u";
	$TABLE="K512";
	$func="sha512_block";
	@Sigma0=(28,34,39);
	@Sigma1=(14,18,41);
	@sigma0=(1,  8, 7);
	@sigma1=(19,61, 6);
	$rounds=80;
} elsif ($output =~ /256.*\.[s|asm]/) {
	$SZ=4;
	$BITS=8*$SZ;
	$LDW="ld4";
	$STW="st4";
	$ADD="padd4";
	$SHRU="pshr4.u";
	$TABLE="K256";
	$func="sha256_block";
	@Sigma0=( 2,13,22);
	@Sigma1=( 6,11,25);
	@sigma0=( 7,18, 3);
	@sigma1=(17,19,10);
	$rounds=64;
} else { die "nonsense $output"; }

open STDOUT,">$output" || die "can't open $output: $!";

if ($^O eq "hpux") {
    $ADDP="addp4";
    for (@ARGV) { $ADDP="add" if (/[\+DD|\-mlp]64/); }
} else { $ADDP="add"; }
for (@ARGV)  {	$big_endian=1 if (/\-DB_ENDIAN/);
		$big_endian=0 if (/\-DL_ENDIAN/);  }
if (!defined($big_endian))
             {	$big_endian=(unpack('L',pack('N',1))==1);  }

$code=<<___;
.ident  \"$output, version 1.0\"
.ident  \"IA-64 ISA artwork by Andy Polyakov <appro\@fy.chalmers.se>\"
.explicit
.text

prsave=r14;
K=r15;
A=r16;	B=r17;	C=r18;	D=r19;
E=r20;	F=r21;	G=r22;	H=r23;
T1=r24;	T2=r25;
s0=r26;	s1=r27;	t0=r28;	t1=r29;
Ktbl=r30;
ctx=r31;	// 1st arg
input=r48;	// 2nd arg
num=r49;	// 3rd arg
sgm0=r50;	sgm1=r51;	// small constants

// void $func (SHA_CTX *ctx, const void *in,size_t num[,int host])
.global	$func#
.proc	$func#
.align	32
$func:
	.prologue
	.fframe	0
	.save	ar.pfs,r2
	.save	ar.lc,r3
	.save	pr,prsave
{ .mmi;	alloc	r2=ar.pfs,3,17,0,16
	$ADDP	ctx=0,r32		// 1st arg
	mov	r3=ar.lc	}
{ .mmi;	$ADDP	input=0,r33		// 2nd arg
	addl	Ktbl=\@ltoff($TABLE#),gp
	mov	prsave=pr	};;

	.body
{ .mii;	ld8	Ktbl=[Ktbl]
	mov	num=r34		};;	// 3rd arg

{ .mib;	add	r8=0*$SZ,ctx
	add	r9=1*$SZ,ctx
	brp.loop.imp	.L_first16,.L_first16_ctop
				}
{ .mib;	add	r10=2*$SZ,ctx
	add	r11=3*$SZ,ctx
	brp.loop.imp	.L_rest,.L_rest_ctop
				};;
// load A-H
{ .mmi;	$LDW	A=[r8],4*$SZ
	$LDW	B=[r9],4*$SZ
	mov	sgm0=$sigma0[2]	}
{ .mmi;	$LDW	C=[r10],4*$SZ
	$LDW	D=[r11],4*$SZ
	mov	sgm1=$sigma1[2]	};;
{ .mmi;	$LDW	E=[r8]
	$LDW	F=[r9]		}
{ .mmi;	$LDW	G=[r10]
	$LDW	H=[r11]
	cmp.ne	p15,p14=0,r35	};;	// used in sha256_block

.L_outer:
{ .mii;	mov	ar.lc=15
	mov	ar.ec=1		};;
.align	32
.L_first16:
.rotr	X[16]
___
$t0="t0", $t1="t1", $code.=<<___ if ($BITS==32);
{ .mib;	(p14)	add	r9=1,input
	(p14)	add	r10=2,input	}
{ .mib;	(p14)	add	r11=3,input
	(p15)	br.dptk.few	.L_host	};;
{ .mmi;	(p14)	ld1	r8=[input],$SZ
	(p14)	ld1	r9=[r9]		}
{ .mmi;	(p14)	ld1	r10=[r10]
	(p14)	ld1	r11=[r11]	};;
{ .mii;	(p14)	dep	r9=r8,r9,8,8
	(p14)	dep	r11=r10,r11,8,8	};;
{ .mib;	(p14)	dep	X[15]=r9,r11,16,16 };;
.L_host:
{ .mib;	(p15)	$LDW	X[15]=[input],$SZ	// X[i]=*input++
		dep.z	$t1=E,32,32	}
{ .mib;		$LDW	K=[Ktbl],$SZ
		zxt4	E=E		};;
{ .mmi;		or	$t1=$t1,E
		and	T1=F,E
		and	T2=A,B		}
{ .mmi;		andcm	r8=G,E
		and	r9=A,C
		mux2	$t0=A,0x44	};;	// copy lower half to upper
{ .mib;		xor	T1=T1,r8		// T1=((e & f) ^ (~e & g))
		_rotr	r11=$t1,$Sigma1[0] }	// ROTR(e,14)
{ .mib;		and	r10=B,C
		xor	T2=T2,r9	};;
___
$t0="A", $t1="E", $code.=<<___ if ($BITS==64);
{ .mmi;		$LDW	X[15]=[input],$SZ	// X[i]=*input++
		and	T1=F,E
		and	T2=A,B		}
{ .mmi;		$LDW	K=[Ktbl],$SZ
		andcm	r8=G,E
		and	r9=A,C		};;
{ .mmi;		xor	T1=T1,r8		//T1=((e & f) ^ (~e & g))
		and	r10=B,C
		_rotr	r11=$t1,$Sigma1[0] }	// ROTR(e,14)
{ .mmi;		xor	T2=T2,r9
		mux1	X[15]=X[15],\@rev };;	// eliminated in big-endian
___
$code.=<<___;
{ .mib;		add	T1=T1,H			// T1=Ch(e,f,g)+h
		_rotr	r8=$t1,$Sigma1[1] }	// ROTR(e,18)
{ .mib;		xor	T2=T2,r10		// T2=((a & b) ^ (a & c) ^ (b & c))
		mov	H=G		};;
{ .mib;		xor	r11=r8,r11
		_rotr	r9=$t1,$Sigma1[2] }	// ROTR(e,41)
{ .mib;		mov	G=F
		mov	F=E		};;
{ .mib;		xor	r9=r9,r11		// r9=Sigma1(e)
		_rotr	r10=$t0,$Sigma0[0] }	// ROTR(a,28)
{ .mib;		add	T1=T1,K			// T1=Ch(e,f,g)+h+K512[i]
		mov	E=D		};;
{ .mib;		add	T1=T1,r9		// T1+=Sigma1(e)
		_rotr	r11=$t0,$Sigma0[1] }	// ROTR(a,34)
{ .mib;		mov	D=C
		mov	C=B		};;
{ .mib;		add	T1=T1,X[15]		// T1+=X[i]
		_rotr	r8=$t0,$Sigma0[2] }	// ROTR(a,39)
{ .mib;		xor	r10=r10,r11
		mux2	X[15]=X[15],0x44 };;	// eliminated in 64-bit
{ .mmi;		xor	r10=r8,r10		// r10=Sigma0(a)
		mov	B=A
		add	A=T1,T2		};;
.L_first16_ctop:
{ .mib;		add	E=E,T1
		add	A=A,r10			// T2=Maj(a,b,c)+Sigma0(a)
	br.ctop.sptk	.L_first16	};;

{ .mib;	mov	ar.lc=$rounds-17	}
{ .mib;	mov	ar.ec=1			};;
.align	32
.L_rest:
.rotr	X[16]
{ .mib;		$LDW	K=[Ktbl],$SZ
		_rotr	r8=X[15-1],$sigma0[0] }	// ROTR(s0,1)
{ .mib; 	$ADD	X[15]=X[15],X[15-9]	// X[i&0xF]+=X[(i+9)&0xF]
		$SHRU	s0=X[15-1],sgm0	};;	// s0=X[(i+1)&0xF]>>7
{ .mib;		and	T1=F,E
		_rotr	r9=X[15-1],$sigma0[1] }	// ROTR(s0,8)
{ .mib;		andcm	r10=G,E
		$SHRU	s1=X[15-14],sgm1 };;	// s1=X[(i+14)&0xF]>>6
{ .mmi;		xor	T1=T1,r10		// T1=((e & f) ^ (~e & g))
		xor	r9=r8,r9
		_rotr	r10=X[15-14],$sigma1[0] };;// ROTR(s1,19)
{ .mib;		and	T2=A,B		
		_rotr	r11=X[15-14],$sigma1[1] }// ROTR(s1,61)
{ .mib;		and	r8=A,C		};;
___
$t0="t0", $t1="t1", $code.=<<___ if ($BITS==32);
// I adhere to mmi; in order to hold Itanium 1 back and avoid 6 cycle
// pipeline flush in last bundle. Note that even on Itanium2 the
// latter stalls for one clock cycle...
{ .mmi;		xor	s0=s0,r9		// s0=sigma0(X[(i+1)&0xF])
		dep.z	$t1=E,32,32	}
{ .mmi;		xor	r10=r11,r10
		zxt4	E=E		};;
{ .mmi;		or	$t1=$t1,E
		xor	s1=s1,r10		// s1=sigma1(X[(i+14)&0xF])
		mux2	$t0=A,0x44	};;	// copy lower half to upper
{ .mmi;		xor	T2=T2,r8
		_rotr	r9=$t1,$Sigma1[0] }	// ROTR(e,14)
{ .mmi;		and	r10=B,C
		add	T1=T1,H			// T1=Ch(e,f,g)+h
		$ADD	X[15]=X[15],s0	};;	// X[i&0xF]+=sigma0(X[(i+1)&0xF])
___
$t0="A", $t1="E", $code.=<<___ if ($BITS==64);
{ .mib;		xor	s0=s0,r9		// s0=sigma0(X[(i+1)&0xF])
		_rotr	r9=$t1,$Sigma1[0] }	// ROTR(e,14)
{ .mib;		xor	r10=r11,r10
		xor	T2=T2,r8	};;
{ .mib;		xor	s1=s1,r10		// s1=sigma1(X[(i+14)&0xF])
		add	T1=T1,H		}
{ .mib;		and	r10=B,C
		$ADD	X[15]=X[15],s0	};;	// X[i&0xF]+=sigma0(X[(i+1)&0xF])
___
$code.=<<___;
{ .mmi;		xor	T2=T2,r10		// T2=((a & b) ^ (a & c) ^ (b & c))
		mov	H=G
		_rotr	r8=$t1,$Sigma1[1] };;	// ROTR(e,18)
{ .mmi;		xor	r11=r8,r9
		$ADD	X[15]=X[15],s1		// X[i&0xF]+=sigma1(X[(i+14)&0xF])
		_rotr	r9=$t1,$Sigma1[2] }	// ROTR(e,41)
{ .mmi;		mov	G=F
		mov	F=E		};;
{ .mib;		xor	r9=r9,r11		// r9=Sigma1(e)
		_rotr	r10=$t0,$Sigma0[0] }	// ROTR(a,28)
{ .mib;		add	T1=T1,K			// T1=Ch(e,f,g)+h+K512[i]
		mov	E=D		};;
{ .mib;		add	T1=T1,r9		// T1+=Sigma1(e)
		_rotr	r11=$t0,$Sigma0[1] }	// ROTR(a,34)
{ .mib;		mov	D=C
		mov	C=B		};;
{ .mmi;		add	T1=T1,X[15]		// T1+=X[i]
		xor	r10=r10,r11
		_rotr	r8=$t0,$Sigma0[2] };;	// ROTR(a,39)
{ .mmi;		xor	r10=r8,r10		// r10=Sigma0(a)
		mov	B=A
		add	A=T1,T2		};;
.L_rest_ctop:
{ .mib;		add	E=E,T1
		add	A=A,r10			// T2=Maj(a,b,c)+Sigma0(a)
	br.ctop.sptk	.L_rest	};;

{ .mib;	add	r8=0*$SZ,ctx
	add	r9=1*$SZ,ctx		}
{ .mib;	add	r10=2*$SZ,ctx
	add	r11=3*$SZ,ctx		};;
{ .mmi;	$LDW	r32=[r8],4*$SZ
	$LDW	r33=[r9],4*$SZ		}
{ .mmi;	$LDW	r34=[r10],4*$SZ
	$LDW	r35=[r11],4*$SZ
	cmp.ltu	p6,p7=1,num		};;
{ .mmi;	$LDW	r36=[r8],-4*$SZ
	$LDW	r37=[r9],-4*$SZ
(p6)	add	Ktbl=-$SZ*$rounds,Ktbl	}
{ .mmi;	$LDW	r38=[r10],-4*$SZ
	$LDW	r39=[r11],-4*$SZ
(p7)	mov	ar.lc=r3		};;
{ .mmi;	add	A=A,r32
	add	B=B,r33
	add	C=C,r34			}
{ .mmi;	add	D=D,r35
	add	E=E,r36
	add	F=F,r37			};;
{ .mmi;	$STW	[r8]=A,4*$SZ
	$STW	[r9]=B,4*$SZ
	add	G=G,r38			}
{ .mmi;	$STW	[r10]=C,4*$SZ
	$STW	[r11]=D,4*$SZ
	add	H=H,r39			};;
{ .mmi;	$STW	[r8]=E
	$STW	[r9]=F
(p6)	add	num=-1,num		}
{ .mmb;	$STW	[r10]=G
	$STW	[r11]=H
(p6)	br.dptk.many	.L_outer	};;

{ .mib;	mov	pr=prsave,0x1ffff
	br.ret.sptk.many	b0	};;
.endp	$func#
___

$code =~ s/\`([^\`]*)\`/eval $1/gem;
$code =~ s/_rotr(\s+)([^=]+)=([^,]+),([0-9]+)/shrp$1$2=$3,$3,$4/gm;
if ($BITS==64) {
    $code =~ s/mux2(\s+)\S+/nop.i$1 0x0/gm;
    $code =~ s/mux1(\s+)\S+/nop.i$1 0x0/gm if ($big_endian);
}

print $code;

print<<___ if ($BITS==32);
.align	64
.type	K256#,\@object
K256:	data4	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5
	data4	0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5
	data4	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3
	data4	0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174
	data4	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc
	data4	0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da
	data4	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7
	data4	0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967
	data4	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13
	data4	0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85
	data4	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3
	data4	0xd192e819,0xd6990624,0xf40e3585,0x106aa070
	data4	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5
	data4	0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3
	data4	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208
	data4	0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
.size	K256#,$SZ*$rounds
___
print<<___ if ($BITS==64);
.align	64
.type	K512#,\@object
K512:	data8	0x428a2f98d728ae22,0x7137449123ef65cd
	data8	0xb5c0fbcfec4d3b2f,0xe9b5dba58189dbbc
	data8	0x3956c25bf348b538,0x59f111f1b605d019
	data8	0x923f82a4af194f9b,0xab1c5ed5da6d8118
	data8	0xd807aa98a3030242,0x12835b0145706fbe
	data8	0x243185be4ee4b28c,0x550c7dc3d5ffb4e2
	data8	0x72be5d74f27b896f,0x80deb1fe3b1696b1
	data8	0x9bdc06a725c71235,0xc19bf174cf692694
	data8	0xe49b69c19ef14ad2,0xefbe4786384f25e3
	data8	0x0fc19dc68b8cd5b5,0x240ca1cc77ac9c65
	data8	0x2de92c6f592b0275,0x4a7484aa6ea6e483
	data8	0x5cb0a9dcbd41fbd4,0x76f988da831153b5
	data8	0x983e5152ee66dfab,0xa831c66d2db43210
	data8	0xb00327c898fb213f,0xbf597fc7beef0ee4
	data8	0xc6e00bf33da88fc2,0xd5a79147930aa725
	data8	0x06ca6351e003826f,0x142929670a0e6e70
	data8	0x27b70a8546d22ffc,0x2e1b21385c26c926
	data8	0x4d2c6dfc5ac42aed,0x53380d139d95b3df
	data8	0x650a73548baf63de,0x766a0abb3c77b2a8
	data8	0x81c2c92e47edaee6,0x92722c851482353b
	data8	0xa2bfe8a14cf10364,0xa81a664bbc423001
	data8	0xc24b8b70d0f89791,0xc76c51a30654be30
	data8	0xd192e819d6ef5218,0xd69906245565a910
	data8	0xf40e35855771202a,0x106aa07032bbd1b8
	data8	0x19a4c116b8d2d0c8,0x1e376c085141ab53
	data8	0x2748774cdf8eeb99,0x34b0bcb5e19b48a8
	data8	0x391c0cb3c5c95a63,0x4ed8aa4ae3418acb
	data8	0x5b9cca4f7763e373,0x682e6ff3d6b2b8a3
	data8	0x748f82ee5defb2fc,0x78a5636f43172f60
	data8	0x84c87814a1f0ab72,0x8cc702081a6439ec
	data8	0x90befffa23631e28,0xa4506cebde82bde9
	data8	0xbef9a3f7b2c67915,0xc67178f2e372532b
	data8	0xca273eceea26619c,0xd186b8c721c0c207
	data8	0xeada7dd6cde0eb1e,0xf57d4f7fee6ed178
	data8	0x06f067aa72176fba,0x0a637dc5a2c898a6
	data8	0x113f9804bef90dae,0x1b710b35131c471b
	data8	0x28db77f523047d84,0x32caab7b40c72493
	data8	0x3c9ebe0a15c9bebc,0x431d67c49c100d4c
	data8	0x4cc5d4becb3e42b6,0x597f299cfc657e2a
	data8	0x5fcb6fab3ad6faec,0x6c44198c4a475817
.size	K512#,$SZ*$rounds
___
