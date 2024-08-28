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
static char *snaptoquadrant[] = {
/* columns rows colors chars-per-pixel */
"128 128 225 2 ",
"   c black",
".  c #010101",
"X  c #020202",
"o  c gray1",
"O  c #040404",
"+  c gray2",
"@  c #060606",
"#  c #070707",
"$  c gray3",
"%  c #090909",
"&  c gray4",
"*  c #0B0B0B",
"=  c #0C0C0C",
"-  c gray5",
";  c #0E0E0E",
":  c gray6",
">  c #101010",
",  c #111111",
"<  c gray7",
"1  c #131313",
"2  c gray8",
"3  c #151515",
"4  c #161616",
"5  c gray9",
"6  c #181818",
"7  c #191919",
"8  c gray10",
"9  c #1B1B1B",
"0  c gray11",
"q  c #1D1D1D",
"w  c #1E1E1E",
"e  c gray12",
"r  c #202020",
"t  c gray13",
"y  c #222222",
"u  c #232323",
"i  c gray14",
"p  c #252525",
"a  c gray15",
"s  c #272727",
"d  c #282828",
"f  c gray16",
"g  c #2A2A2A",
"h  c #2C2C2C",
"j  c #2D2D2D",
"k  c gray18",
"l  c #2F2F2F",
"z  c gray19",
"x  c #313131",
"c  c gray20",
"v  c #343434",
"b  c #353535",
"n  c #373737",
"m  c gray22",
"M  c #393939",
"N  c #3A3A3A",
"B  c gray23",
"V  c #3C3C3C",
"C  c gray24",
"Z  c #3E3E3E",
"A  c #3F3F3F",
"S  c gray25",
"D  c #414141",
"F  c gray26",
"G  c #434343",
"H  c #444444",
"J  c gray27",
"K  c #464646",
"L  c gray28",
"P  c #484848",
"I  c #494949",
"U  c gray29",
"Y  c #4B4B4B",
"T  c #4C4C4C",
"R  c #4E4E4E",
"E  c gray32",
"W  c #535353",
"Q  c #555555",
"!  c #565656",
"~  c gray34",
"^  c gray35",
"/  c #5A5A5A",
"(  c #5B5B5B",
")  c gray36",
"_  c #5D5D5D",
"`  c gray37",
"'  c #5F5F5F",
"]  c #606060",
"[  c gray38",
"{  c #626262",
"}  c gray39",
"|  c #646464",
" . c #656565",
".. c gray40",
"X. c #676767",
"o. c #686868",
"O. c DimGray",
"+. c #6A6A6A",
"@. c gray42",
"#. c #6C6C6C",
"$. c gray43",
"%. c #6F6F6F",
"&. c gray44",
"*. c #727272",
"=. c gray45",
"-. c #747474",
";. c gray46",
":. c #767676",
">. c #777777",
",. c gray47",
"<. c #797979",
"1. c gray48",
"2. c #7B7B7B",
"3. c gray49",
"4. c #7E7E7E",
"5. c gray50",
"6. c #808080",
"7. c #818181",
"8. c gray51",
"9. c #838383",
"0. c #848484",
"q. c gray52",
"w. c gray55",
"e. c #8E8E8E",
"r. c #909090",
"t. c #929292",
"y. c #939393",
"u. c gray59",
"i. c #989898",
"p. c #9D9D9D",
"a. c #9F9F9F",
"s. c #A0A0A0",
"d. c gray63",
"f. c #A2A2A2",
"g. c gray64",
"h. c #A5A5A5",
"j. c gray66",
"k. c #A9A9A9",
"l. c #AAAAAA",
"z. c gray67",
"x. c #ACACAC",
"c. c gray68",
"v. c #AEAEAE",
"b. c #AFAFAF",
"n. c gray69",
"m. c #B1B1B1",
"M. c #B2B2B2",
"N. c gray70",
"B. c #B4B4B4",
"V. c gray71",
"C. c #B6B6B6",
"Z. c #B7B7B7",
"A. c gray72",
"S. c #B9B9B9",
"D. c gray73",
"F. c #BBBBBB",
"G. c #BCBCBC",
"H. c gray74",
"J. c gray",
"K. c gray75",
"L. c #C0C0C0",
"P. c #C1C1C1",
"I. c gray76",
"U. c #C3C3C3",
"Y. c gray77",
"T. c #C5C5C5",
"R. c #C6C6C6",
"E. c gray78",
"W. c #C8C8C8",
"Q. c gray79",
"!. c #CACACA",
"~. c #CBCBCB",
"^. c gray80",
"/. c #CDCDCD",
"(. c #CECECE",
"). c gray81",
"_. c #D0D0D0",
"`. c gray82",
"'. c #D2D2D2",
"]. c LightGray",
"[. c gray83",
"{. c #D5D5D5",
"}. c gray84",
"|. c #D7D7D7",
" X c #D8D8D8",
".X c gray85",
"XX c #DADADA",
"oX c gray86",
"OX c gainsboro",
"+X c #DDDDDD",
"@X c gray87",
"#X c #DFDFDF",
"$X c gray88",
"%X c #E1E1E1",
"&X c #E2E2E2",
"*X c gray89",
"=X c #E4E4E4",
"-X c gray90",
";X c #E6E6E6",
":X c #E7E7E7",
">X c gray91",
",X c #E9E9E9",
"<X c #EAEAEA",
"1X c gray92",
"2X c #ECECEC",
"3X c gray93",
"4X c #EEEEEE",
"5X c #EFEFEF",
"6X c gray94",
"7X c #F1F1F1",
"8X c gray95",
"9X c #F3F3F3",
"0X c #F4F4F4",
"qX c gray96",
"wX c #F6F6F6",
"eX c gray97",
"rX c #F8F8F8",
"tX c #F9F9F9",
"yX c gray98",
"uX c #FBFBFB",
"iX c gray99",
"pX c #FDFDFD",
"aX c #FEFEFE",
"sX c white",
"dX c None",
/* pixels */
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX              dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX                          dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      @ @ : 3 3 3 : @         dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      : s U %.0.e.6.O.H y :       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    @ y { m.&X3X,X=X=X&X_.s.Q e @     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    % b u.3X8X3X3X3X3X=X&X#X.X_.3.f @     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    @ b k.8X8X8XwX8X8X3X,X=X&X#X}._.e.f @     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    e u.8X8XwXwXrXrXwX8X8X,X&X#X}.}./.2.3     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    % { ,X8XwXwXrXsXsXrXrX8X3X=X#X}._./.I.U @   dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    e m.8XwXwXrXrXsXsXsXsXwX3X,X&X.X_.Q.Y.e.3     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  @ F &X3XwXwXrXsXsXsXsXsXrX8X,X&X.X_.Q.Y.m.c     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX        % O.3X3X8XwXsXsXsXsXsXsXrXwX3X=X.X_.Q.I.A.R @       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX        % Z   : 6.,X,X8XwXsXsXsXsXsXsXrX8X,X&X}./.Y.I.B.{ %             dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX        8 (  .c     , 0.=X,X3X8XrXrXsXsXsXsXrX8X=X#X_./.Y.K.B. .%   e             dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX        W =.b           : 2.&X=X,X3X8XwXrXrXsXrX8X,X&X.X/.Y.I.A.B.` %       , :           dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      3 O.{                 @ ` #X=X=X,X,X8X8XwX8X8X,X&X.X_.Q.I.A.B.c.H @             e           dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    e >.Q               dXdX  @ B /.&X&X&X=X=X,X3X,X=X&X.X_.Q.I.K.B.m.s.f                   3           dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    % >.`             dXdXdXdXdX    8 u..X.X.X#X#X&X&X#X#X.X_.Q.Y.K.A.B.c.>.,     dX                          dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX       .>.            dXdXdXdXdXdXdX    % U Q..X.X}..X.X}..X}./.Y.Y.K.A.B.m.s.B @   dXdXdXdXdXdX                      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    l 3.:           dXdXdXdXdXdXdXdXdXdX    3 >._._._._._._._.Q.Y.I.K.A.B.B.k.` :     dXdXdXdXdXdXdXdX                    dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    O.O.          dXdXdXdXdXdXdXdXdXdXdXdX      y 0.Q./.Q.Q.Y.Y.I.K.A.A.B.m.k.O.8     dXdXdXdXdXdXdXdXdXdXdX                    dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  @ 6.8         dXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    @ y %.A.I.Y.I.I.K.A.A.m.c.a.` 8       dXdXdXdXdXdXdXdXdXdXdXdXdX                  dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  f 6.          dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      3 F 0.k.B.m.m.c.k.a.>.b :       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX                dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    Z 3.        dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      @ 3 l U ` { ( H f , @       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX                dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    U =.        dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX          @ @ @ @ @           dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX                dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    U %.        dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX                    dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX                dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  F =.        dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      @       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  l 2.        dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      %       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  : 3.        dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    3.        dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    %.:       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      @       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  B {       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      @       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    3.      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    O.        dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  3 =.      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX              dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    2.      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  s Q       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    ,       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    2.      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  y U       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    3       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    =.      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  : U       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    ,       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    {       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    :       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX     .      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX          dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  Z       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    e       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    O.      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    `       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX          dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  f 3     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    %       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  Q       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdX                      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX                      dXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdX        @ % : : : % @         dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX        @ % : : : % @         dXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdX      % y H O.6.0.2.` B 8 %       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      % 8 Z { 3.0.6. .F e :       dXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdX    % e ` m.&X3X,X=X&X#X/.i.U 3       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    @ 8 W s.#X,X,X,X=X&X_.s.Q e @     dXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdX    % c u.,X8X3X3X3X,X=X&X#X.XQ.>.y @     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    @ f 0.=X8X8X3X3X,X=X&X#X.X}.6.l %     dXdXdXdXdXdXdX",
"dXdXdXdXdXdX    % b k.8X8X8XwX8X8X3X,X=X#X#X}./.0.y       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      s i.,X8X8X8X8X8X8X,X=X#X#X}.}.u.f @     dXdXdXdXdXdX",
"dXdXdXdXdX      y u.8X8XwXwXwXwXwXwX8X,X&X.X.X_.Q.%.3     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    3 6.,X8XwXwXrXwXwX8X3X,X=X#X}._._.6.e       dXdXdXdXdX",
"dXdXdXdXdX    : { 3X8XwXwXrXsXrXrXrX8X,X=X#X.X_.Q.A.F @     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    % R #X8XwXwXrXrXsXrXrXwX3X=X#X}.}._.Y.W :     dXdXdXdXdX",
"dXdXdXdXdX    f m.8XwXwXrXrXsXsXsXsXwX8X=X&X.X_./.I.0.3     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    8 a.,X8XwXwXrXsXsXsXsXwX8X,X&X.X}.Q.Q.y.y     dXdXdXdXdX",
"dXdXdXdX    @ U =X3X8XrXsXsXsXsXsXsXrX8X,X&X.X_.Q.Y.k.l     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    b }.3X8XwXrXsXsXsXsXsXrX8X,X&X.X_./.Y.A.Z @     dXdXdXdX",
"dXdXdXdX    : %.3X3XwXwXrXsXsXsXsXsXrX8X,X&X}._.Q.I.B.U @   dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  @ ( #X3X8XwXsXsXsXsXsXsXsXwX,X=X.X_.Q.I.I.( %     dXdXdXdX",
"dXdXdXdX    3 0.,X3X8XwXsXsXsXsXsXsXrXwX,X&X}./.Y.K.m.` %   dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  @ =.&X3X8XwXrXsXsXsXsXsXsXwX3X&X.X_.Y.K.A.O.:     dXdXdXdX",
"dXdXdXdX    3 e.,X3X3XwXrXsXsXsXsXsXwX8X=X#X}./.I.K.m.{ %   dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  % 2.#X,X3X8XrXsXsXsXsXsXrX8X,X&X}./.Y.K.A.%.,     dXdXdXdX",
"dXdXdXdX    , 6.=X=X,X3X8XrXrXrXrXrX8X,X&X}./.Y.K.A.c.( %   dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  % %..X=X,X3X8XwXwXrXsXrX8X,X&X.X/.Y.I.A.B. .:     dXdXdXdX",
"dXdXdXdX    :  .&X&X=X,X3X3X8XwXwX8X,X&X.X_.Y.I.A.B.k.H @   dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  @ Q }.=X=X,X3X3X8X8X8X8X3X&X.X_.Q.I.K.B.B.Q %     dXdXdXdX",
"dXdXdXdX    @ H _.#X&X&X=X,X,X,X,X=X&X.X_.Q.I.K.A.m.a.f     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  @ c I.#X&X&X=X,X,X3X,X=X=X.X}./.Y.K.A.B.k.b @     dXdXdXdX",
"dXdXdXdXdX    y s..X#X#X#X#X&X&X&X#X}._.Q.Y.K.A.B.c.>.:     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    3 e.}..X#X#X#X&X&X&X#X.X_./.Y.I.A.B.m.6.8 @   dXdXdXdXdX",
"dXdXdXdXdX    : Q _.}.}..X.X.X.X}.}./.Q.I.K.A.B.m.s.b @   dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  @ F Y.}.}.}.}..X.X}.}._.Q.Y.I.A.B.B.k.H %     dXdXdXdXdX",
"dXdXdXdXdX      e 3._._._._._._./.Q.Y.I.K.B.B.m.k.` :     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    , =.Q._.}._._._././.Y.I.K.A.B.B.c.O.3       dXdXdXdXdX",
"dXdXdXdXdXdX    @ f y./././.Q.Q.Y.I.K.K.A.B.B.k.O.8       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      e 3.Q./././.Q.Y.I.I.K.A.B.m.m.2.y @     dXdXdXdXdXdX",
"dXdXdXdXdXdXdX    @ f 2.I.I.Y.I.I.K.A.A.B.m.s.` 8       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    @ e O.A.Q.Y.Y.I.A.A.B.B.m.k.O.y @     dXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdX    @ 3 H e.B.A.B.m.m.c.s.>.b : @     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      , Z 6.m.A.A.B.B.m.h.3.F , @     dXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdX      % 3 c R  . .` H f , @       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      @ 3 l U `  .{ R l 3 @       dXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdX          @ % % @ @         dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX        @ @ % @ @           dXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdX                  dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX                  dXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    8       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  3 s       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    3       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  (       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    %.      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    ,       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  b s       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    2.      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX          dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  , Q       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    %       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    O.      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    {       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    %       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  `       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX     .      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  O.      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX              dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  s `       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    3.      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    @       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  { 3       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX              dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  y %.      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    3.      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  >.        dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX              dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  { B       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX              dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  R {       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX              dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  H =.      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX              dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  H >.      dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      %       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX                    dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  R >.        dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      8         dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX          % % % % @ @         dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    { {         dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      3         dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      @ 8 B ( =.2.=.W c , @       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    >.B       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX                dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      3 W a._.#X&X#X.X}.I.y.H ,       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    e 3.        dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX        %         dXdXdXdXdXdXdXdXdXdXdXdXdXdX    @ s 6.#X,X3X3X,X,X=X&X#X}.Y.%.e @     dXdXdXdXdXdXdXdXdXdXdXdXdX    ( %.        dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX        @           dXdXdXdXdXdXdXdXdXdXdX    @ f i.,X8X8X8X8X8X8X,X=X#X.X}.Q.3.e     dXdXdXdXdXdXdXdXdXdXdX    8 3.s         dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX        y           dXdXdXdXdXdXdXdXdX    8 0.3X8XwXwXwXwXwX8X3X,X&X#X}._.Q.%.,     dXdXdXdXdXdXdXdX       .%.          dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX          8           dXdXdXdXdXdX    % Q &X8XwXwXrXrXrXsXrX8X3X=X#X}._./.A.Z @   dXdXdXdXdXdX      R 3.          dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX            :           dXdXdXdX    e s.3XwXwXwXrXsXsXsXsXwX3X,X&X.X_./.Y.0.,     dXdX        U >.8           dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX                          dX    Z #X8X8XrXrXsXsXsXsXsXwX8X,X&X.X_./.Y.c.l           : (  .            dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX              @           % { 3X3X8XrXrXsXsXsXsXsXrXwX,X&X.X_.Q.I.A.U @   @ U O.B               dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX              3 e     : 3.3X3X8XrXrXsXsXsXsXsXsX8X,X#X}./.Y.K.A.{ %   Z               dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX                  : 0.=X,X8X8XwXsXsXsXsXsXrX8X,X#X}./.Y.K.B. .%               dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX              : 3.=X=X3X3XwXwXrXsXsXrXwX,X&X.X_.Y.K.A.m.{ @           dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX        @  .&X=X=X,X3X3XwXwXwX8X3X&X.X_./.I.K.B.m.U       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  @ F _.#X&X&X=X,X,X,X3X=X&X.X}.Q.Y.K.B.m.h.c     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    e s..X#X#X#X&X&X=X&X&X.X_./.Y.K.A.B.m.3.3     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    : Q }.}.}..X.X.X.X.X}._.Q.Y.I.A.A.m.k.F @   dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX  @ 8 0._.}._._.}._./.Q.Q.I.K.B.B.B.m.O.3     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    @ l y._./.Q./.Q.Q.Y.K.K.A.B.B.c.>.y @     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    @ f 6.Q.Q.Y.I.K.K.A.A.m.B.c.O.s @     dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX    @ e W y.B.K.K.B.B.B.k.6.H 8       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX      : y B Q O.=. .Q b e %       dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX        @ : , : , % @         dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX                          dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX              dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX",
"dXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdXdX"
};
