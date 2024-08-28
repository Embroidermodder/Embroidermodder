/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 */
/* XPM */
static char *locator_snaptointersection[] = {
/* columns rows colors chars-per-pixel */
"128 128 181 2 ",
"   c #5F6000",
".  c #606000",
"X  c #616200",
"o  c #626300",
"O  c #656700",
"+  c #6A6A00",
"@  c #6F7100",
"#  c #747500",
"$  c #747600",
"%  c #757600",
"&  c #757700",
"*  c #767700",
"=  c #777900",
"-  c #787900",
";  c #7A7B00",
":  c #7C7D00",
">  c #7D7F00",
",  c #808100",
"<  c #808200",
"1  c #848500",
"2  c #868700",
"3  c #868800",
"4  c #878800",
"5  c #878900",
"6  c #888A00",
"7  c #898A00",
"8  c #898B00",
"9  c #8A8B00",
"0  c #8B8B00",
"q  c #8A8C00",
"w  c #8B8D00",
"e  c #8C8D00",
"r  c #8D8E00",
"t  c #8D8F00",
"y  c #8E8F00",
"u  c #8E9000",
"i  c #8F9000",
"p  c #8F9100",
"a  c #909100",
"s  c #909200",
"d  c #919200",
"f  c #919300",
"g  c #929300",
"h  c #929400",
"j  c #939400",
"k  c #949500",
"l  c #949600",
"z  c #959700",
"x  c #969700",
"c  c #969800",
"v  c #979800",
"b  c #979900",
"n  c #989A00",
"m  c #9A9B00",
"M  c #9A9C00",
"N  c #9B9C00",
"B  c #9B9D00",
"V  c #9D9E00",
"C  c #9D9F00",
"Z  c #9E9F00",
"A  c #9EA000",
"S  c #9FA100",
"D  c #A0A100",
"F  c #A1A200",
"G  c #A1A300",
"H  c #A2A400",
"J  c #A3A400",
"K  c #A4A500",
"L  c #A4A600",
"P  c #A5A700",
"I  c #A6A800",
"U  c #A7A800",
"Y  c #A7A900",
"T  c #A8AA00",
"R  c #A9AA00",
"E  c #A9AB00",
"W  c #AAAB00",
"Q  c #AAAC00",
"!  c #ABAD00",
"~  c #ACAE00",
"^  c #ADAE00",
"/  c #ADAF00",
"(  c #AEB000",
")  c #AFB100",
"_  c #B0B200",
"`  c #B1B200",
"'  c #B1B300",
"]  c #B2B400",
"[  c #B3B500",
"{  c #B4B500",
"}  c #B4B600",
"|  c #B5B700",
" . c #B6B800",
".. c #B7B800",
"X. c #B7B900",
"o. c #B8BA00",
"O. c #B9BB00",
"+. c #BABC00",
"@. c #BBBD00",
"#. c #BCBE00",
"$. c #BDBF00",
"%. c #BEC000",
"&. c #BFC100",
"*. c #C0C200",
"=. c #C1C300",
"-. c #C2C400",
";. c #C2C401",
":. c #C2C402",
">. c #C3C503",
",. c #C3C504",
"<. c #C3C505",
"1. c #C3C506",
"2. c #C4C607",
"3. c #C4C608",
"4. c #C4C609",
"5. c #C5C70B",
"6. c #C5C70C",
"7. c #C5C70D",
"8. c #C6C810",
"9. c #C6C811",
"0. c #C6C812",
"q. c #C7C813",
"w. c #C7C914",
"e. c #C7C915",
"r. c #C7C916",
"t. c #C8C917",
"y. c #C8CA18",
"u. c #C8CA19",
"i. c #C8CA1A",
"p. c #C9CB1D",
"a. c #C9CB1E",
"s. c #CACB20",
"d. c #CACC21",
"f. c #CACC22",
"g. c #CACC23",
"h. c #CCCD28",
"j. c #CCCD29",
"k. c #CCCE2A",
"l. c #CCCE2B",
"z. c #CDCE2C",
"x. c #CDCF2E",
"c. c #CDCF2F",
"v. c #CED034",
"b. c #CFD035",
"n. c #CFD036",
"m. c #D0D13A",
"M. c #D0D23B",
"N. c #D0D23C",
"B. c #D1D23D",
"V. c #D1D23E",
"C. c #D1D340",
"Z. c #D3D447",
"A. c #D3D548",
"S. c #D4D54A",
"D. c #D4D54B",
"F. c #D4D64C",
"G. c #D5D750",
"H. c #D6D752",
"J. c #D6D753",
"K. c #D8D95A",
"L. c #D8D95B",
"P. c #D8DA5E",
"I. c #D9DA60",
"U. c #D9DA61",
"Y. c #DADB64",
"T. c #DADB65",
"R. c #DADC66",
"E. c #DBDC67",
"W. c #DBDD6A",
"Q. c #DCDD6B",
"!. c #DCDD6C",
"~. c #DCDD6E",
"^. c #DDDE72",
"/. c #DEDF74",
"(. c #DFE078",
"). c #DFE079",
"_. c #DFE07B",
"`. c #E0E17C",
"'. c #E0E17E",
"]. c #E0E17F",
"[. c None",
/* pixels */
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.| _ [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.| / [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o. .) P [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.o.| / P [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.+.+.o._ T V [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[. .+.$.+._ I V [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.&.| T G k [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[. .+.&.=.&. .Q G b [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.S.s.$.| P G k [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.o.=.a.U.d.&.] T G k [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.R.'.S.=.+._ I A g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.o.=.h./.'.B.=.+.] I G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.R.'.Q.n.>.&.o.] I G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.o.&.h._.].L.h.=.=.o._ T G j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.R.'.!.B.y.=.=.&. .) I A i [.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.o.o.=.s.'.].L.x.8.=.=.=.o._ T G k [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.>.T.'.!.B.i.2.=.=.=.&. ./ G B y [.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.o.+.$.7./._.L.x.0.>.>.=.=.=.o._ I G j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.R.'.!.B.i.3.>.=.=.=.&.+.) G m k y [.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.o. .o.$.0.I.J.x.0.,.=.=.>.=.=.=.o._ T G k [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.>.T.'.!.B.i.,.=.=.>.&.&.&. .T V j g 7 [.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.| _ ]  .$.=.3.3.=.>.=.>.=.=.=.=.=.o._ I G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.Y.'.Q.B.y.2.>.=.>.=.&.&. .( G g y > [.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.T T Q ] o.&.=.=.=.>.=.>.=.>.=.=.=.o._ T G k [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.T.'.Q.B.y.2.>.=.=.=.=.&. .Q A g 7 * [.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.A H I _ o.$.=.=.>.=.>.>.=.>.=.=.=.o._ T G j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.].!.B.i.,.>.=.=.=.&.&. .Q A p 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.k G P _ o.$.=.=.>.=.=.>.=.>.>.=.=.o._ T G j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.T.].!.B.i.4.>.=.>.=.&.&. .Q A j 6 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.i G P _ o.=.=.=.>.>.=.>.=.=.=.=.=.o._ U G k [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.].!.B.i.,.>.=.>.=.&.&. .Q A j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.0 A P _ o.$.=.=.=.>.=.>.>.=.=.=.=.+._ T G j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.>.T.].!.B.i.,.>.=.=.=.=.&. .Q A j 6 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P _ o.$.=.>.=.>.=.=.>.=.>.=.%.o._ P G j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.T.].!.B.y.,.=.=.>.=.=.$. ./ A j 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P _ o.&.=.>.=.>.>.=.>.=.-.=.=.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.].!.B.i.3.=.>.=.=.=.$.| Q A j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 V I _ o.$.=.>.=.=.>.=.>.-.-.=.%.o.` T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.>.Y.'.!.B.i.,.>.>.=.=.=.$.| / A g 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P / o.=.=.=.>.=.>.=.-.-.-.=.=.+._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.'.Q.B.i.,.>.>.=.=.=.$. ./ V g 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P ) o.&.=.=.=.=.>.=.>.=.>.=.=.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.R.].!.B.y.2.>.=.=.=.=.$.| / A g 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I ) o.$.=.=.>.=.>.=.>.=.>.=.=.o._ I G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.R.].!.B.y.2.>.=.=.=.=.$. ./ V j 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G I ) o.&.=.=.=.=.>.=.>.>.=.=.=.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.R.'.!.B.y.2.=.>.=.=.=.$. ./ A g 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I ) o.=.=.=.>.=.>.=.=.=.=.=.=.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[. .&.=.T.].!.B.y.2.=.>.=.&.&.$. ./ V g 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P ) o.&.=.=.>.=.>.=.>.=.>.=.%.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[. .&.=.R.].Q.B.y.3.>.=.=.=.=.%. .Q A j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I )  .&.=.=.>.=.>.=.>.=.-.=.=.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.T.].!.B.y.3.=.=.>.&.&.&. .Q A g 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A P ) o.=.=.=.=.=.>.=.>.-.-.=.=.o._ U G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.R.'.Q.B.y.2.>.=.>.=.&.&. .( A j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G I ) o.&.=.=.>.=.>.=.-.-.-.=.=.o._ U G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.'.!.B.y.2.>.=.=.=.=.&. .Q A j 6 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G I / o.&.=.>.=.>.=.-.-.-.-.=.%.o.` U F j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.T.].!.B.i.,.>.=.=.=.=.%. .Q A p 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I _ o.$.=.>.=.>.-.-.-.-.-.=.=.+.` U G j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.].!.B.i.,.>.=.>.&.&.&. .( A j 6 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P _ o.&.=.>.=.-.-.-.-.-.-.=.%.+._ U G j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.>.Y.'.!.B.i.4.>.=.>.=.&.%. .Q A j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P _ o.$.=.=.-.-.-.-.-.-.-.=.%.+._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.'.Q.B.i.,.=.=.=.=.=.%. .( A p 6 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I _ o.$.=.-.-.-.-.-.-.-.-.=.%.o._ P G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.>.R.'.Q.B.y.2.=.=.>.=.=.$. .Q A j 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A P _ o.$.=.-.-.-.-.-.-.-.-.=.=.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.'.!.B.y.2.=.>.=.=.=.$. .Q A j 6 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 D P ) o.%.=.-.-.-.-.-.-.-.-.=.%.o.` T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.R.'.!.B.y.2.>.=.=.=.&.$. ./ A g 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 D U ) o.%.=.-.-.-.-.-.-.-.-.=.=.+._ I G g [.[.[.[.[.[.[.[.[.[.[.[.o.$.=.R.'.!.B.i.2.>.=.>.=.&.&. ./ V j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G I ) o.&.=.=.=.>.=.>.=.>.=.=.=.o._ T G j [.[.[.[.[.[.[.[.[.[.o.$.=.T.].!.B.i.,.>.>.=.=.&.&. .Q A p 6 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I )  .&.=.=.=.>.=.>.=.>.>.=.=.+._ T G g [.[.[.[.[.[.[.[.o.&.=.T.].!.B.i.4.>.>.=.=.&.&. .Q A j 6 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G I ) o.&.=.=.=.>.=.>.=.=.=.=.&.o._ T G k [.[.[.[.[.[.o.$.=.T.'.!.B.i.,.=.>.=.=.&.&. .Q A j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I ) o.&.=.=.=.>.=.>.>.=.=.=.=.o._ T I V [.[.[.] +.$.=.T.].!.B.i.,.>.>.=.=.=.&. .Q A j 6 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G I ) o.$.=.>.=.>.=.=.>.=.>.=.=.o.] / / P [.Q $.=.1.E.].Q.B.i.,.>.=.=.=.=.$. ./ A j 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I ) o.&.=.>.=.>.>.=.>.=.>.=.=.+.o.|  .| $.=.c.^.].!.B.i.,.>.=.=.=.=.$. .Q A j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I ) o.&.=.>.=.=.>.=.>.>.=.=.=.=.&.=.=.i.J.'.'.E.B.i.4.>.>.=.=.=.$. ./ A g 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I ) o.&.=.=.>.=.>.=.=.>.=.=.=.,.t.n.L.!.!.L.N.i.1.=.>.=.=.&.$. ./ V g 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I ) o.&.=.-.-.-.-.-.-.-.,.8.a.z.B.S.A.M.z.t.,.=.>.=.=.=.$.| / A g 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G I ) o.$.=.=.-.-.-.-.-.-.,.4.8.i.i.t.8.3.,.=.>.=.=.=.$.| / V g 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A P ) o.$.=.-.-.-.-.-.-.=.>.=.>.=.>.>.=.>.=.=.=.=.$. ./ A g 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G I ) o.=.=.-.,.-.-.-.>.=.>.=.>.=.=.>.=.>.=.&.$.o./ A g 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 H T | $.=.4.4.=.=.>.=.>.=.>.=.>.=.>.>.=.=.=.+._ H j 7 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.g T _ o.=.8.t.,.>.=.>.=.>.=.>.=.>.=.=.>.=.&. .Q B y = [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.k _ o.=.s.a.4.=.>.=.>.=.>.=.>.=.>.=.=.&.$.| P b > [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.G $.=.n.z.4.>.=.>.=.>.=.>.=.>.=.=.=.=.$._ P 5 [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.I =.z.H.n.7.=.>.=.>.=.>.=.>.=.>.=.>.=.$.] T 7 [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.) &.=.L.U.v.3.>.=.>.=.>.=.>.=.>.=.>.=.=.&.o./ G 2 [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.| +.&.C.`.L.z.3.=.>.=.>.=.>.=.>.=.>.=.>.=.=.+.| T G 7 [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.e.(.'.H.d.,.>.=.>.=.>.=.>.=.>.=.>.=.=.=.=.o._ T G y [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[. .&.=.Q.'.R.n.8.,.=.>.=.=.=.>.=.>.=.>.=.>.=.>.=.%.o._ I G y [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.T.'.!.B.y.3.>.>.=.=.=.>.=.>.=.>.=.>.=.>.=.-.=.=.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.R.'.!.B.y.2.=.>.=.>.=.=.=.=.=.=.=.=.=.>.=.>.-.-.=.=.+._ P G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.'.!.B.y.2.=.>.=.=.=.=.&.&.&.=.=.=.=.>.=.>.=.-.-.=.=.=.o._ P G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.T.].!.B.i.,.>.=.=.=.&.&.+.+. . .o.+.$.=.>.=.>.=.-.-.-.-.=.%.o.` T G j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.].!.B.i.,.>.=.>.=.&.&. .] ) Q Q _ | o.=.=.>.=.>.-.-.-.-.-.=.=.+.` U D j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.>.Y.'.!.B.i.4.>.=.>.=.&.&. .) H A V b F T _ o.&.=.>.=.-.-.-.-.-.-.=.%.+._ U G j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.'.Q.B.i.,.=.=.=.=.=.&. .Q A b j < [.* G P _ o.$.=.=.-.-.-.-.-.-.-.=.%.+._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.>.R.'.Q.B.y.2.=.=.>.=.=.$. .Q G j q ; [.[.[.- G P _ o.$.=.-.-.-.-.-.-.-.-.=.=.o._ U G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.'.!.B.y.2.=.>.=.=.=.$. .Q A j 5 = [.[.[.[.[.- G P _ o.$.=.-.-.-.-.-.-.-.-.=.%.o._ U G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.R.'.!.B.y.2.>.=.=.=.&.$. ./ A g 5 * [.[.[.[.[.[.[., D P ) o.%.=.-.-.-.-.-.-.-.-.=.=.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.R.'.!.B.i.2.>.=.>.=.&.&. ./ V j 5 # [.[.[.[.[.[.[.[.[.1 D U ) o.%.=.-.-.-.-.-.-.=.=.=.=.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.'.!.B.i.,.>.>.=.=.&.&. .Q A p 6 * [.[.[.[.[.[.[.[.[.[.[.7 G P _ o.$.=.=.=.=.>.=.>.=.>.=.=.o._ T G j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.T.].!.B.i.4.>.>.=.=.&.&. .Q A j 6 # [.[.[.[.[.[.[.[.[.[.[.[.[.7 A P _ o.&.=.=.>.=.>.=.>.=.=.=.=.+.` U D j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.].!.B.i.,.=.>.=.=.&.&. .Q A j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A P ) o.=.=.=.>.=.>.=.>.>.=.=.&.+._ U G j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.>.T.].!.B.i.,.>.>.=.=.=.&. .Q A j 6 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P _ o.$.=.=.>.=.>.=.=.=.=.=.%.+._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.T.].!.B.i.,.>.=.=.=.=.$. ./ A j 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P ) o.&.=.=.>.=.>.>.=.>.=.=.=.o.` T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.].!.B.i.,.>.=.=.=.=.$. .Q A j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A P ) o.$.=.=.>.=.=.>.=.>.-.=.%.o._ P G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.>.T.'.!.B.i.,.>.>.=.=.=.$. ./ A g 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P _ o.=.=.=.>.>.=.>.=.-.-.=.%.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.'.Q.B.i.,.>.>.=.=.&.$. ./ V g 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A P _ o.$.=.>.=.>.=.>.-.-.-.=.=.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.R.].!.B.y.2.>.=.=.=.=.$.| / A g 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P _ o.$.=.=.>.=.>.=.>.=.>.=.=.o._ I G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.R.].!.B.y.2.>.=.=.=.=.$.| / V g 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A P _ o.$.=.=.>.=.>.=.>.=.>.=.=.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.R.'.!.B.y.2.=.>.=.=.=.$. ./ A g 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A P _ o.$.=.=.=.=.>.=.>.>.=.=.=.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[. .&.=.T.].!.B.y.2.=.>.=.&.&.$. ./ A g 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P _ o.=.=.=.>.=.>.=.=.=.=.=.=.o._ U G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[. .&.=.R.].Q.B.y.3.>.=.=.=.=.%. .Q A j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A P _ o.&.=.=.>.=.>.=.>.=.>.=.%.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.T.].!.B.y.3.=.=.>.&.&.&. .Q A g 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P ) o.$.=.=.>.=.>.=.>.=.-.=.=.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.R.'.Q.B.y.2.>.=.>.=.&.%. .Q A j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P _ o.=.=.=.>.=.>.=.>.-.-.=.=.o._ U G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.'.!.B.y.2.>.=.=.=.=.%. .( A j 6 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A P _ o.$.=.=.>.=.>.=.-.-.-.=.=.o._ U G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.'.!.B.i.,.>.>.=.=.&.&. .Q A p 6 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I ) o.$.=.>.=.>.=.-.-.-.-.=.%.o.` T D j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.T.].!.B.i.4.>.>.=.=.&.&. .Q A j 6 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G I ) o.&.=.>.=.>.-.-.-.-.-.=.=.+.` U D j [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.].!.B.i.,.=.>.=.=.&.&. .Q A j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I ) o.&.=.=.=.-.-.-.-.-.-.=.%.o._ U G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.>.T.].!.B.i.,.>.>.=.=.=.&. .Q A j 6 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G I ) o.&.=.=.-.-.-.-.-.-.-.=.=.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.&.=.T.].!.B.i.,.>.=.=.=.=.$. ./ A j 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I ) o.&.=.-.-.-.-.-.-.-.-.=.=.o._ U G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.T.].!.B.i.,.>.=.=.=.=.$. .Q A j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I )  .&.=.-.-.-.-.-.-.-.-.=.%.o._ P G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.R.'.!.B.i.,.>.>.=.=.=.$. ./ A g 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P ) o.%.=.-.-.-.-.-.-.-.-.=.=.o._ T D g [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.R.'.!.B.i.,.>.>.=.=.&.$. ./ V g 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A I ) o.%.=.-.-.-.-.-.-.=.=.=.=.o._ T G g [.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.U.].Q.B.y.2.>.=.=.=.=.$.| / A g 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P ) o.$.=.=.>.=.>.=.>.=.>.=.=.o._ T A g [.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.o.$.=.D.].Q.B.y.2.>.=.=.=.=.$.| / V g 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A U ) o.%.=.=.>.=.>.=.>.=.=.=.&.o.) H V i [.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.| | o.=.d.F.n.y.2.=.>.=.=.=.$. ./ A g 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 D P _ o.$.=.=.=.=.>.=.>.=.>.=.$.] I V m y [.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[._ ) _ | $.=.>.>.=.>.=.&.&.$. ./ A g 5 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P ) o.=.=.=.>.=.>.=.=.=.$.o._ P m k k [.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.I I T ] +.&.=.=.=.=.=.%. .Q A j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A P _ o.&.=.=.>.=.=.=.=.+.| T B g y @ [.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.V G T ] +.&.=.&.&.&. .Q A g 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P ) o.$.=.=.=.=.=.+.| T B i 5 O [.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.b G I ) O.&.&.%. .Q A j 5 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G P _ o.=.=.=.=.+.| T m y 2 X [.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.j G I )  .O. .( A j 6 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A P _  .$.&.+.| I m y 2 X [.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.g A I / _ T A g 7 # [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 G I /  . .] I B i 2   [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.g V G H V k 7 * [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A H Q / I B i 2   [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.g B m j y - [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 A A G B i 5   [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.y k g > [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.7 B b j q o [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.y 5 [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.2 k g + [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.5 = [.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.",
"[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[.[."
};
