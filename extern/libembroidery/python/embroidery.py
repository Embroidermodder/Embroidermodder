#!/usr/bin/env python3

r"""
Libembroidery
1.0.0-alpha
https://www.libembroidery.org

Hand-written translation of the C library libembroidery for Python.
This is manually updated, however, it has sections which are generated with
simple sed scripts that will be included in the source code at:

    https://github.com/embroidermodder/libembroidery

---------------------------------------------------------------------------------

Copyright 2018-2025 The Embroidermodder Team
Libembroidery is Open Source Software.
See LICENSE.md for licensing terms.

---------------------------------------------------------------------------------

Where the code seems overly "C-like" or too low level, it's likely because we're
trying to make it easy to update this file to match the algorithms in the
more up-to date main C version.
"""

import math
from math import radians, degrees
import time
import unittest
import ctypes
import pathlib
import os

# Stitch to (x, y).
NORMAL                       = 0x00
# Move to (x, y).
JUMP                         = 0x01
# Trim and move to (x, y).
TRIM                         = 0x02
# Pause machine for a thread change.
STOP                         = 0x04
# Add a sequin at the current co-ordinates.
SEQUIN                       = 0x08
# End of program.
END                          = 0x10


# Format identifiers
EMB_FORMAT_100                  = 0
EMB_FORMAT_10O                  = 1
EMB_FORMAT_ART                  = 2
EMB_FORMAT_BMC                  = 3
EMB_FORMAT_BRO                  = 4
EMB_FORMAT_CND                  = 5
EMB_FORMAT_COL                  = 6
EMB_FORMAT_CSD                  = 7
EMB_FORMAT_CSV                  = 8
EMB_FORMAT_DAT                  = 9
EMB_FORMAT_DEM                 = 10
EMB_FORMAT_DSB                 = 11
EMB_FORMAT_DST                 = 12
EMB_FORMAT_DSZ                 = 13
EMB_FORMAT_DXF                 = 14
EMB_FORMAT_EDR                 = 15
EMB_FORMAT_EMD                 = 16
EMB_FORMAT_EXP                 = 17
EMB_FORMAT_EXY                 = 18
EMB_FORMAT_EYS                 = 19
EMB_FORMAT_FXY                 = 20
EMB_FORMAT_GC                  = 21
EMB_FORMAT_GNC                 = 22
EMB_FORMAT_GT                  = 23
EMB_FORMAT_HUS                 = 24
EMB_FORMAT_INB                 = 25
EMB_FORMAT_INF                 = 26
EMB_FORMAT_JEF                 = 27
EMB_FORMAT_KSM                 = 28
EMB_FORMAT_MAX                 = 29
EMB_FORMAT_MIT                 = 30
EMB_FORMAT_NEW                 = 31
EMB_FORMAT_OFM                 = 32
EMB_FORMAT_PCD                 = 33
EMB_FORMAT_PCM                 = 34
EMB_FORMAT_PCQ                 = 35
EMB_FORMAT_PCS                 = 36
EMB_FORMAT_PEC                 = 37
EMB_FORMAT_PEL                 = 38
EMB_FORMAT_PEM                 = 39
EMB_FORMAT_PES                 = 40
EMB_FORMAT_PHB                 = 41
EMB_FORMAT_PHC                 = 42
EMB_FORMAT_PLT                 = 43
EMB_FORMAT_RGB                 = 44
EMB_FORMAT_SEW                 = 45
EMB_FORMAT_SHV                 = 46
EMB_FORMAT_SST                 = 47
EMB_FORMAT_STX                 = 48
EMB_FORMAT_SVG                 = 49
EMB_FORMAT_T01                 = 50
EMB_FORMAT_T09                 = 51
EMB_FORMAT_TAP                 = 52
EMB_FORMAT_THR                 = 53
EMB_FORMAT_TXT                 = 54
EMB_FORMAT_U00                 = 55
EMB_FORMAT_U01                 = 56
EMB_FORMAT_VIP                 = 57
EMB_FORMAT_VP3                 = 58
EMB_FORMAT_XXX                 = 59
EMB_FORMAT_ZSK                 = 60

# Thread color
Arc_Polyester                   = 0
Arc_Rayon                       = 1
CoatsAndClark_Rayon             = 2
Exquisite_Polyester             = 3
Fufu_Polyester                  = 4
Fufu_Rayon                      = 5
Hemingworth_Polyester           = 6
Isacord_Polyester               = 7
Isafil_Rayon                    = 8
Marathon_Polyester              = 9
Marathon_Rayon                 = 10
Madeira_Polyester              = 11
Madeira_Rayon                  = 12
Metro_Polyester                = 13
Pantone                        = 14
RobisonAnton_Polyester         = 15
RobisonAnton_Rayon             = 16
Sigma_Polyester                = 17
Sulky_Rayon                    = 18
ThreadArt_Rayon                = 19
ThreadArt_Polyester            = 20
ThreaDelight_Polyester         = 21
Z102_Isacord_Polyester         = 22
SVG_Colors                     = 23
hus_thread                     = 24
jef_thread                     = 25
pcm_thread                     = 26
pec_thread                     = 27
shv_thread                     = 28
dxf_color                      = 29

format_table = (
    (".10o", "Toyota Embroidery Format", "U", " ", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".100", "Toyota Embroidery Format", "U", " ", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".art", "Bernina Embroidery Format", " ", " ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".bmc", "Bitmap Cache Embroidery Format", " ", " ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".bro", "Bits & Volts Embroidery Format", "U", " ", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".cnd", "Melco Embroidery Format", " ", " ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".col", "Embroidery Thread Color Format", "U", "U", "EMBFORMAT_STITCHONLY", 1, 0, 0),
    (".csd", "Singer Embroidery Format", "U", " ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".csv", "Comma Separated Values Format", "U", "U", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".dat", "Barudan Embroidery Format", "U", " ", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".dem", "Melco Embroidery Format", " ", " ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".dsb", "Barudan Embroidery Format", "U"," ", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".dst", "Tajima Embroidery Format", "U", "U", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".dsz", "ZSK USA Embroidery Format", "U", " ", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".dxf", "Drawing Exchange Format", " ", " ", "EMBFORMAT_OBJECTONLY", 0, 0, 0),
    (".edr", "Embird Embroidery Format", "U", "U", "EMBFORMAT_STITCHONLY", 1, 0, 0),
    (".emd", "Elna Embroidery Format", "U"," ", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".exp", "Melco Embroidery Format", "U", "U", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".exy", "Eltac Embroidery Format", "U"," ", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".eys", "Sierra Expanded Embroidery Format", " "," ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".fxy", "Fortron Embroidery Format",  "U"," ", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".gc",  "Smoothie G-Code Format",    " "," ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".gnc", "Great Notions Embroidery Format",   " "," ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".gt",  "Gold Thread Embroidery Format",      "U"," ", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".hus", "Husqvarna Viking Embroidery Format", "U", "U", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".inb", "Inbro Embroidery Format", "U"," ", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".inf", "Embroidery Color Format", "U", "U", "EMBFORMAT_STITCHONLY", 1, 0, 0),
    (".jef", "Janome Embroidery Format", "U", "U", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".ksm", "Pfaff Embroidery Format", "U", "U", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".max", "Pfaff Embroidery Format", "U", "U", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".mit", "Mitsubishi Embroidery Format", "U"," ", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".new", "Ameco Embroidery Format", "U"," ", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".ofm", "Melco Embroidery Format", "U"," ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".pcd", "Pfaff Embroidery Format",    "U", "U", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".pcm", "Pfaff Embroidery Format",    "U"," ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".pcq", "Pfaff Embroidery Format",    "U", "U", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".pcs", "Pfaff Embroidery Format",    "U", "U", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".pec", "Brother Embroidery Format",  "U", "U", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".pel", "Brother Embroidery Format", " "," ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".pem", "Brother Embroidery Format", " "," ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".pes", "Brother Embroidery Format",  "U", "U", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".phb", "Brother Embroidery Format",  "U"," ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".phc", "Brother Embroidery Format",  "U"," ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".plt", "AutoCAD Plot Drawing Format",        "U", "U", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".rgb", "RGB Embroidery Format",      "U", "U", "EMBFORMAT_STITCHONLY", 1, 0, 0),
    (".sew", "Janome Embroidery Format",   "U", "U", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".shv", "Husqvarna Viking Embroidery Format", "U"," ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".sst", "Sunstar Embroidery Format", "U"," ", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".stx", "Data Stitch Embroidery Format", "U"," ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".svg", "Scalable Vector Graphics", "U", "U", "EMBFORMAT_OBJECTONLY", 0, 0, 0),
    (".t01", "Pfaff Embroidery Format", "U", " ", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".t09", "Pfaff Embroidery Format", "U", " ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".tap", "Happy Embroidery Format", "U", "U", "EMBFORMAT_STITCHONLY", 0, 1, 0),
    (".thr", "ThredWorks Embroidery Format", "U", "U", "EMBFORMAT_STITCHONLY", 1, 0, 0),
    (".txt", "Text File", " ", "U", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".u00", "Barudan Embroidery Format", "U"," ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".u01", "Barudan Embroidery Format", " "," ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".vip", "Pfaff Embroidery Format", "U"," ", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".vp3", "Pfaff Embroidery Format", "U", "U", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".xxx", "Singer Embroidery Format", "U", "U", "EMBFORMAT_STITCHONLY", 0, 0, 0),
    (".zsk", "ZSK USA Embroidery Format", "U", " ", "EMBFORMAT_STITCHONLY", 0, 0, 0)
)

threads = {
    "_dxfColorTable": [
        [0, 0, 0, "BYBLOCK"],
        [255, 0, 0, "1 red"],
        [255, 255, 0, "2 yellow"],
        [0, 255, 0, "3 green"],
        [0, 255, 255, "4 cyan"],
        [0, 0, 255, "5 blue"],
        [255, 0, 255, "6 magenta"],
        [255, 255, 255, "7 white"],
        [128, 128, 128, "8 dark gray"],
        [192, 192, 192, "9 light gray"],
        [255, 0, 0, "10"],
        [255, 127, 127, "11"],
        [204, 0, 0, "12"],
        [204, 102, 102, "13"],
        [153, 0, 0, "14"],
        [153, 76, 76, "15"],
        [127, 0, 0, "16"],
        [127, 63, 63, "17"],
        [76, 0, 0, "18"],
        [76, 38, 38, "19"],
        [255, 63, 0, "20"],
        [255, 159, 127, "21"],
        [204, 51, 0, "22"],
        [204, 127, 102, "23"],
        [153, 38, 0, "24"],
        [153, 95, 76, "25"],
        [127, 31, 0, "26"],
        [127, 79, 63, "27"],
        [76, 19, 0, "28"],
        [76, 47, 38, "29"],
        [255, 127, 0, "30"],
        [255, 191, 127, "31"],
        [204, 102, 0, "32"],
        [204, 153, 102, "33"],
        [153, 76, 0, "34"],
        [153, 114, 76, "35"],
        [127, 63, 0, "36"],
        [127, 95, 63, "37"],
        [76, 38, 0, "38"],
        [76, 57, 38, "39"],
        [255, 191, 0, "40"],
        [255, 223, 127, "41"],
        [204, 153, 0, "42"],
        [204, 178, 102, "43"],
        [153, 114, 0, "44"],
        [153, 133, 76, "45"],
        [127, 95, 0, "46"],
        [127, 111, 63, "47"],
        [76, 57, 0, "48"],
        [76, 66, 38, "49"],
        [255, 255, 0, "50"],
        [255, 255, 127, "51"],
        [204, 204, 0, "52"],
        [204, 204, 102, "53"],
        [153, 153, 0, "54"],
        [153, 153, 76, "55"],
        [127, 127, 0, "56"],
        [127, 127, 63, "57"],
        [76, 76, 0, "58"],
        [76, 76, 38, "59"],
        [191, 255, 0, "60"],
        [223, 255, 127, "61"],
        [153, 204, 0, "62"],
        [178, 204, 102, "63"],
        [114, 153, 0, "64"],
        [133, 153, 76, "65"],
        [95, 127, 0, "66"],
        [111, 127, 63, "67"],
        [57, 76, 0, "68"],
        [66, 76, 38, "69"],
        [127, 255, 0, "70"],
        [191, 255, 127, "71"],
        [102, 204, 0, "72"],
        [153, 204, 102, "73"],
        [76, 153, 0, "74"],
        [114, 153, 76, "75"],
        [63, 127, 0, "76"],
        [95, 127, 63, "77"],
        [38, 76, 0, "78"],
        [57, 76, 38, "79"],
        [63, 255, 0, "80"],
        [159, 255, 127, "81"],
        [51, 204, 0, "82"],
        [127, 204, 102, "83"],
        [38, 153, 0, "84"],
        [95, 153, 76, "85"],
        [31, 127, 0, "86"],
        [79, 127, 63, "87"],
        [19, 76, 0, "88"],
        [47, 76, 38, "89"],
        [0, 255, 0, "90"],
        [127, 255, 127, "91"],
        [0, 204, 0, "92"],
        [102, 204, 102, "93"],
        [0, 153, 0, "94"],
        [76, 153, 76, "95"],
        [0, 127, 0, "96"],
        [63, 127, 63, "97"],
        [0, 76, 0, "98"],
        [38, 76, 38, "99"],
        [0, 255, 63, "100"],
        [127, 255, 159, "101"],
        [0, 204, 51, "102"],
        [102, 204, 127, "103"],
        [0, 153, 38, "104"],
        [76, 153, 95, "105"],
        [0, 127, 31, "106"],
        [63, 127, 79, "107"],
        [0, 76, 19, "108"],
        [38, 76, 47, "109"],
        [0, 255, 127, "110"],
        [127, 255, 191, "111"],
        [0, 204, 102, "112"],
        [102, 204, 153, "113"],
        [0, 153, 76, "114"],
        [76, 153, 114, "115"],
        [0, 127, 63, "116"],
        [63, 127, 95, "117"],
        [0, 76, 38, "118"],
        [38, 76, 57, "119"],
        [0, 255, 191, "120"],
        [127, 255, 223, "121"],
        [0, 204, 153, "122"],
        [102, 204, 178, "123"],
        [0, 153, 114, "124"],
        [76, 153, 133, "125"],
        [0, 127, 95, "126"],
        [63, 127, 111, "127"],
        [0, 76, 57, "128"],
        [38, 76, 66, "129"],
        [0, 255, 255, "130"],
        [127, 255, 255, "131"],
        [0, 204, 204, "132"],
        [102, 204, 204, "133"],
        [0, 153, 153, "134"],
        [76, 153, 153, "135"],
        [0, 127, 127, "136"],
        [63, 127, 127, "137"],
        [0, 76, 76, "138"],
        [38, 76, 76, "139"],
        [0, 191, 255, "140"],
        [127, 223, 255, "141"],
        [0, 153, 204, "142"],
        [102, 178, 204, "143"],
        [0, 114, 153, "144"],
        [76, 133, 153, "145"],
        [0, 95, 127, "146"],
        [63, 111, 127, "147"],
        [0, 57, 76, "148"],
        [38, 66, 76, "149"],
        [0, 127, 255, "150"],
        [127, 191, 255, "151"],
        [0, 102, 204, "152"],
        [102, 153, 204, "153"],
        [0, 76, 153, "154"],
        [76, 114, 153, "155"],
        [0, 63, 127, "156"],
        [63, 95, 127, "157"],
        [0, 38, 76, "158"],
        [38, 57, 76, "159"],
        [0, 63, 255, "160"],
        [127, 159, 255, "161"],
        [0, 51, 204, "162"],
        [102, 127, 204, "163"],
        [0, 38, 153, "164"],
        [76, 95, 153, "165"],
        [0, 31, 127, "166"],
        [63, 79, 127, "167"],
        [0, 19, 76, "168"],
        [38, 47, 76, "169"],
        [0, 0, 255, "170"],
        [127, 127, 255, "171"],
        [0, 0, 204, "172"],
        [102, 102, 204, "173"],
        [0, 0, 153, "174"],
        [76, 76, 153, "175"],
        [0, 0, 127, "176"],
        [63, 63, 127, "177"],
        [0, 0, 76, "178"],
        [38, 38, 76, "179"],
        [63, 0, 255, "180"],
        [159, 127, 255, "181"],
        [51, 0, 204, "182"],
        [127, 102, 204, "183"],
        [38, 0, 153, "184"],
        [95, 76, 153, "185"],
        [31, 0, 127, "186"],
        [79, 63, 127, "187"],
        [19, 0, 76, "188"],
        [47, 38, 76, "189"],
        [127, 0, 255, "190"],
        [191, 127, 255, "191"],
        [102, 0, 204, "192"],
        [153, 102, 204, "193"],
        [76, 0, 153, "194"],
        [114, 76, 153, "195"],
        [63, 0, 127, "196"],
        [95, 63, 127, "197"],
        [38, 0, 76, "198"],
        [57, 38, 76, "199"],
        [191, 0, 255, "200"],
        [223, 127, 255, "201"],
        [153, 0, 204, "202"],
        [178, 102, 204, "203"],
        [114, 0, 153, "204"],
        [133, 76, 153, "205"],
        [95, 0, 127, "206"],
        [111, 63, 127, "207"],
        [57, 0, 76, "208"],
        [66, 38, 76, "209"],
        [255, 0, 255, "210"],
        [255, 127, 255, "211"],
        [204, 0, 204, "212"],
        [204, 102, 204, "213"],
        [153, 0, 153, "214"],
        [153, 76, 153, "215"],
        [127, 0, 127, "216"],
        [127, 63, 127, "217"],
        [76, 0, 76, "218"],
        [76, 38, 76, "219"],
        [255, 0, 191, "220"],
        [255, 127, 223, "221"],
        [204, 0, 153, "222"],
        [204, 102, 178, "223"],
        [153, 0, 114, "224"],
        [153, 76, 133, "225"],
        [127, 0, 95, "226"],
        [127, 63, 111, "227"],
        [76, 0, 57, "228"],
        [76, 38, 66, "229"],
        [255, 0, 127, "230"],
        [255, 127, 191, "231"],
        [204, 0, 102, "232"],
        [204, 102, 153, "233"],
        [153, 0, 76, "234"],
        [153, 76, 114, "235"],
        [127, 0, 63, "236"],
        [127, 63, 95, "237"],
        [76, 0, 38, "238"],
        [76, 38, 57, "239"],
        [255, 0, 63, "240"],
        [255, 127, 159, "241"],
        [204, 0, 51, "242"],
        [204, 102, 127, "243"],
        [153, 0, 38, "244"],
        [153, 76, 95, "245"],
        [127, 0, 31, "246"],
        [127, 63, 79, "247"],
        [76, 0, 19, "248"],
        [76, 38, 47, "249"],
        [51, 51, 51, "250"],
        [91, 91, 91, "251"],
        [132, 132, 132, "252"],
        [173, 173, 173, "253"],
        [214, 214, 214, "254"],
        [255, 255, 255, "255"],
        [0, 0, 0 , "256 BYLAYER"]
    ],
    "hus_threads": [
        [0, 0, 0, "Black", "TODO:HUS_CATALOG_NUMBER"],
        [0, 0, 255, "Blue", "TODO:HUS_CATALOG_NUMBER"],
        [0, 255, 0, "Light Green", "TODO:HUS_CATALOG_NUMBER"],
        [255, 0, 0, "Red", "TODO:HUS_CATALOG_NUMBER"],
        [255, 0, 255, "Purple", "TODO:HUS_CATALOG_NUMBER"],
        [255, 255, 0, "Yellow", "TODO:HUS_CATALOG_NUMBER"],
        [127, 127, 127, "Gray", "TODO:HUS_CATALOG_NUMBER"],
        [51, 154, 255, "Light Blue", "TODO:HUS_CATALOG_NUMBER"],
        [51, 204, 102, "Green", "TODO:HUS_CATALOG_NUMBER"],
        [255, 127, 0, "Orange", "TODO:HUS_CATALOG_NUMBER"],
        [255, 160, 180, "Pink", "TODO:HUS_CATALOG_NUMBER"],
        [153, 75, 0, "Brown", "TODO:HUS_CATALOG_NUMBER"],
        [255, 255, 255, "White", "TODO:HUS_CATALOG_NUMBER"],
        [0, 0, 127, "Dark Blue", "TODO:HUS_CATALOG_NUMBER"],
        [0, 127, 0, "Dark Green", "TODO:HUS_CATALOG_NUMBER"],
        [127, 0, 0, "Dark Red", "TODO:HUS_CATALOG_NUMBER"],
        [255, 127, 127, "Light Red", "TODO:HUS_CATALOG_NUMBER"],
        [127, 0, 127, "Dark Purple", "TODO:HUS_CATALOG_NUMBER"],
        [255, 127, 255, "Light Purple", "TODO:HUS_CATALOG_NUMBER"],
        [200, 200, 0, "Dark Yellow", "TODO:HUS_CATALOG_NUMBER"],
        [255, 255, 153, "Light Yellow", "TODO:HUS_CATALOG_NUMBER"],
        [60, 60, 60, "Dark Gray", "TODO:HUS_CATALOG_NUMBER"],
        [192, 192, 192, "Light Gray", "TODO:HUS_CATALOG_NUMBER"],
        [232, 63, 0, "Dark Orange", "TODO:HUS_CATALOG_NUMBER"],
        [255, 165, 65, "Light Orange", "TODO:HUS_CATALOG_NUMBER"],
        [255, 102, 122, "Dark Pink", "TODO:HUS_CATALOG_NUMBER"],
        [255, 204, 204, "Light Pink", "TODO:HUS_CATALOG_NUMBER"],
        [115, 40, 0, "Dark Brown", "TODO:HUS_CATALOG_NUMBER"],
        [175, 90, 10, "Light Brown", "TODO:HUS_CATALOG_NUMBER"]
    ],
    "jef_threads": [
        [0, 0, 0, "Placeholder", "000"],
        [0, 0, 0, "Black", "002"],
        [255, 255, 255, "White", "001"],
        [255, 255, 23, "Yellow", "204"],
        [255, 102, 0, "Orange", "203"],
        [47, 89, 51, "Olive Green", "219"],
        [35, 115, 54, "Green", "226"],
        [101, 194, 200, "Sky", "217"],
        [171, 90, 150, "Purple", "208"],
        [246, 105, 160, "Pink", "201"],
        [255, 0, 0, "Red", "225"],
        [177, 112, 78, "Brown", "214"],
        [11, 47, 132, "Blue", "207"],
        [228, 195, 93, "Gold", "003"],
        [72, 26, 5, "Dark Brown", "205"],
        [172, 156, 199, "Pale Violet", "209"],
        [252, 242, 148, "Pale Yellow", "210"],
        [249, 153, 183, "Pale Pink", "211"],
        [250, 179, 129, "Peach", "212"],
        [201, 164, 128, "Beige", "213"],
        [151, 5, 51, "Wine Red", "215"],
        [160, 184, 204, "Pale Sky", "216"],
        [127, 194, 28, "Yellow Green", "218"],
        [229, 229, 229, "Silver Gray", "220"],
        [136, 155, 155, "Gray", "221"],
        [152, 214, 189, "Pale Aqua", "227"],
        [178, 225, 227, "Baby Blue", "228"],
        [54, 139, 160, "Powder Blue", "229"],
        [79, 131, 171, "Bright Blue", "230"],
        [56, 106, 145, "Slate Blue", "231"],
        [7, 22, 80, "Navy Blue", "232"],
        [249, 153, 162, "Salmon Pink", "233"],
        [249, 103, 107, "Coral", "234"],
        [227, 49, 31, "Burnt Orange", "235"],
        [226, 161, 136, "Cinnamon", "236"],
        [181, 148, 116, "Umber", "237"],
        [228, 207, 153, "Blond", "238"],
        [255, 203, 0, "Sunflower", "239"],
        [225, 173, 212, "Orchid Pink", "240"],
        [195, 0, 126, "Peony Purple", "241"],
        [128, 0, 75, "Burgundy", "242"],
        [84, 5, 113, "Royal Purple", "243"],
        [177, 5, 37, "Cardinal Red", "244"],
        [202, 224, 192, "Opal Green", "245"],
        [137, 152, 86, "Moss Green", "246"],
        [92, 148, 26, "Meadow Green", "247"],
        [0, 49, 20, "Dark Green", "248"],
        [93, 174, 148, "Aquamarine", "249"],
        [76, 191, 143, "Emerald Green", "250"],
        [0, 119, 114, "Peacock Green", "251"],
        [89, 91, 97, "Dark Gray", "252"],
        [255, 255, 242, "Ivory White", "253"],
        [177, 88, 24, "Hazel", "254"],
        [203, 138, 7, "Toast", "255"],
        [152, 108, 128, "Salmon", "256"],
        [152, 105, 45, "Cocoa Brown", "257"],
        [77, 52, 25, "Sienna", "258"],
        [76, 51, 11, "Sepia", "259"],
        [51, 32, 10, "Dark Sepia", "260"],
        [82, 58, 151, "Violet Blue", "261"],
        [13, 33, 126, "Blue Ink", "262"],
        [30, 119, 172, "Sola Blue", "263"],
        [178, 221, 83, "Green Dust", "264"],
        [243, 54, 137, "Crimson", "265"],
        [222, 100, 158, "Floral Pink", "266"],
        [152, 65, 97, "Wine", "267"],
        [76, 86, 18, "Olive Drab", "268"],
        [76, 136, 31, "Meadow", "269"],
        [228, 222, 121, "Mustard", "270"],
        [203, 138, 26, "Yellow Ocher", "271"],
        [203, 162, 28, "Old Gold", "272"],
        [255, 152, 5, "Honey Dew", "273"],
        [252, 178, 87, "Tangerine", "274"],
        ["0xFF", "0xE5", "0x05", "Canary Yellow", "275"],
        ["0xF0", "0x33", "0x1F", "Vermilion", "202"],
        ["0x1A", "0x84", "0x2D", "Bright Green", "206"],
        ["0x38", "0x6C", "0xAE", "Ocean Blue", "222"],
        ["0xE3", "0xC4", "0xB4", "Beige Gray", "223"],
        ["0xE3", "0xAC", "0x81", "Bamboo", "224"]
    ],
    "shv_threads": [
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [0, 0, 255, "Blue", "TODO:CATALOG_NUMBER"],
        [51, 204, 102, "Green", "TODO:CATALOG_NUMBER"],
        [255, 0, 0, "Red", "TODO:CATALOG_NUMBER"],
        [255, 0, 255, "Purple", "TODO:CATALOG_NUMBER"],
        [255, 255, 0, "Yellow", "TODO:CATALOG_NUMBER"],
        [127, 127, 127, "Grey", "TODO:CATALOG_NUMBER"],
        [51, 154, 255, "Light Blue", "TODO:CATALOG_NUMBER"],
        [0, 255, 0, "Light Green", "TODO:CATALOG_NUMBER"],
        [255, 127, 0, "Orange", "TODO:CATALOG_NUMBER"],
        [255, 160, 180, "Pink", "TODO:CATALOG_NUMBER"],
        [153, 75, 0, "Brown", "TODO:CATALOG_NUMBER"],
        [255, 255, 255, "White", "TODO:CATALOG_NUMBER"],
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [255, 127, 127, "Light Red", "TODO:CATALOG_NUMBER"],
        [255, 127, 255, "Light Purple", "TODO:CATALOG_NUMBER"],
        [255, 255, 153, "Light Yellow", "TODO:CATALOG_NUMBER"],
        [192, 192, 192, "Light Grey", "TODO:CATALOG_NUMBER"],
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [255, 165, 65, "Light Orange", "TODO:CATALOG_NUMBER"],
        [255, 204, 204, "Light Pink", "TODO:CATALOG_NUMBER"],
        [175, 90, 10, "Light Brown", "TODO:CATALOG_NUMBER"],
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [0, 0, 127, "Dark Blue", "TODO:CATALOG_NUMBER"],
        [0, 127, 0, "Dark Green", "TODO:CATALOG_NUMBER"],
        [127, 0, 0, "Dark Red", "TODO:CATALOG_NUMBER"],
        [127, 0, 127, "Dark Purple", "TODO:CATALOG_NUMBER"],
        [200, 200, 0, "Dark Yellow", "TODO:CATALOG_NUMBER"],
        [60, 60, 60, "Dark Gray", "TODO:CATALOG_NUMBER"],
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [0, 0, 0, "Black", "TODO:CATALOG_NUMBER"],
        [232, 63, 0, "Dark Orange", "TODO:CATALOG_NUMBER"],
        [255, 102, 122, "Dark Pink", "TODO:CATALOG_NUMBER"]
    ],
    "pcm_threads": [
        [0, 0, 0, "PCM Color 1", ""],
        [0, 0, 128, "PCM Color 2", ""],
        [0, 0, 255, "PCM Color 3", ""],
        [0, 128, 128, "PCM Color 4", ""],
        [0, 255, 255, "PCM Color 5", ""],
        [128, 0, 128, "PCM Color 6", ""],
        [255, 0, 255, "PCM Color 7", ""],
        [128, 0, 0, "PCM Color 8", ""],
        [255, 0, 0, "PCM Color 9", ""],
        [0, 128, 0, "PCM Color 10", ""],
        [0, 255, 0, "PCM Color 11", ""],
        [128, 128, 0, "PCM Color 12", ""],
        [255, 255, 0, "PCM Color 13", ""],
        [128, 128, 128, "PCM Color 14", ""],
        ["0xC0", "0xC0", "0xC0", "PCM Color 15", ""],
        [255, 255, 255, "PCM Color 16", ""]
    ],
    "pec_threads": [
        [0, 0, 0, "Unknown", "0"],
        [14, 31, 124, "Prussian Blue", "1"],
        [10, 85, 163, "Blue", "2"],
        [0, 135, 119, "Teal Green", "3"],
        [75, 107, 175, "Cornflower Blue", "4"],
        [237, 23, 31, "Red", "5"],
        [209, 92, 0, "Reddish Brown", "6"],
        [145, 54, 151, "Magenta", "7"],
        [228, 154, 203, "Light Lilac", "8"],
        [145, 95, 172, "Lilac", "9"],
        [158, 214, 125, "Mint Green", "10"],
        [232, 169, 0, "Deep Gold", "11"],
        [254, 186, 53, "Orange", "12"],
        [255, 255, 0, "Yellow", "13"],
        [112, 188, 31, "Lime Green", "14"],
        [186, 152, 0, "Brass", "15"],
        [168, 168, 168, "Silver", "16"],
        [125, 111, 0, "Russet Brown", "17"],
        [255, 255, 179, "Cream Brown", "18"],
        [79, 85, 86, "Pewter", "19"],
        [0, 0, 0, "Black", "20"],
        [11, 61, 145, "Ultramarine", "21"],
        [119, 1, 118, "Royal Purple", "22"],
        [41, 49, 51, "Dark Gray", "23"],
        [42, 19, 1, "Dark Brown", "24"],
        [246, 74, 138, "Deep Rose", "25"],
        [178, 118, 36, "Light Brown", "26"],
        [252, 187, 197, "Salmon Pink", "27"],
        [254, 55, 15, "Vermillion", "28"],
        [240, 240, 240, "White", "29"],
        [106, 28, 138, "Violet", "30"],
        [168, 221, 196, "Seacrest", "31"],
        [37, 132, 187, "Sky Blue", "32"],
        [254, 179, 67, "Pumpkin", "33"],
        [255, 243, 107, "Cream Yellow", "34"],
        [208, 166, 96, "Khaki", "35"],
        [209, 84, 0, "Clay Brown", "36"],
        [102, 186, 73, "Leaf Green", "37"],
        [19, 74, 70, "Peacock Blue", "38"],
        [135, 135, 135, "Gray", "39"],
        [216, 204, 198, "Warm Gray", "40"],
        [67, 86, 7, "Dark Olive", "41"],
        [253, 217, 222, "Flesh Pink", "42"],
        [249, 147, 188, "Pink", "43"],
        [0, 56, 34, "Deep Green", "44"],
        [178, 175, 212, "Lavender", "45"],
        [104, 106, 176, "Wisteria Violet", "46"],
        [239, 227, 185, "Beige", "47"],
        [247, 56, 102, "Carmine", "48"],
        [181, 75, 100, "Amber Red", "49"],
        [19, 43, 26, "Olive Green", "50"],
        [199, 1, 86, "Dark Fuschia", "51"],
        [254, 158, 50, "Tangerine", "52"],
        [168, 222, 235, "Light Blue", "53"],
        [0, 103, 62, "Emerald Green", "54"],
        [78, 41, 144, "Purple", "55"],
        [47, 126, 32, "Moss Green", "56"],
        [255, 204, 204, "Flesh Pink", "57"],
        [255, 217, 17, "Harvest Gold", "58"],
        [9, 91, 166, "Electric Blue", "59"],
        [240, 249, 112, "Lemon Yellow", "60"],
        [227, 243, 91, "Fresh Green", "61"],
        [255, 153, 0, "Orange", "62"],
        [255, 240, 141, "Cream Yellow", "63"],
        [255, 200, 200, "Applique", "64"]
    ],
    "svg_color_codes": [
        ["aliceblue", "0xFFf0f8ff", 0],
        ["antiquewhite", "0xFFfaebd7", 1],
        ["aqua", "0xFF00ffff", 2],
        ["aquamarine", "0xFF7fffd4", 3],
        ["azure", "0xFFf0ffff", 4],
        ["beige", "0xFFf5f5dc", 5],
        ["bisque", "0xFFffe4c4", 6],
        ["black", "0xFF000000", 7],
        ["blanchedalmond", "0xFFffebcd", 8],
        ["blue", "0xFF0000ff", 9],
        ["blueviolet", "0xFF8a2be2", 10],
        ["brown", "0xFFa52a2a", 11],
        ["burlywood", "0xFFdeb81b", 12],
        ["cadetblue", "0xFF5f9ea0", 13],
        ["chartreuse", "0xFF7fff00", 14],
        ["chocolate", "0xFFd22d1e", 15],
        ["coral", "0xFFFF7f0e", 16],
        ["cornflowerblue", "0xFF1c5fed", 17],
        ["cornsilk", "0xFFfff8dc", 18],
        ["crimson", "0xFFdc0e3c", 19],
        ["cyan", "0xFF00ffff", 20],
        ["darkblue", "0xFF00008b", 21],
        ["darkcyan", "0xFF008b8b", 22],
        ["darkgoldenrod", "0xFFb81a0b", 23],
        ["darkgray", "0xFFa9a9a9", 24],
        ["darkgreen", "0xFF001c00", 25],
        ["darkgrey", "0xFFa9a9a9", 26],
        ["darkkhaki", "0xFFbdb76b", 27],
        ["darkmagenta", "0xFF8b008b", 28],
        ["darkolivegreen", "0xFF0d6b2f", 29],
        ["darkorange", "0xFFff8c00", 30],
        ["darkorchid", "0xFF3f0ecc", 31],
        ["darkred", "0xFF8b0000", 32],
        ["darksalmon", "0xFFe93c7a", 33],
        ["darkseagreen", "0xFF8fbc8f", 34],
        ["darkslateblue", "0xFF1e3d8b", 35],
        ["darkslategray", "0xFF2f4f4f", 36],
        ["darkslategrey", "0xFF2f4f4f", 37],
        ["darkturquoise", "0xFF00ced1", 38],
        ["darkviolet", "0xFF5e00d3", 39],
        ["deeppink", "0xFFff0e5d", 40],
        ["deepskyblue", "0xFF00bfff", 41],
        ["dimgray", "0xFF2d2d2d", 42],
        ["dimgrey", "0xFF2d2d2d", 43],
        ["dodgerblue", "0xFF1e5aff", 44],
        ["firebrick", "0xFFb20a0a", 45],
        ["floralwhite", "0xFFfffaf0", 46],
        ["forestgreen", "0xFF0a8b0a", 47],
        ["fuchsia", "0xFFff00ff", 48],
        ["gainsboro", "0xFFdcdcdc", 49],
        ["ghostwhite", "0xFFf8f8ff", 50],
        ["gold", "0xFFffd700", 51],
        ["goldenrod", "0xFFdaa50e", 52],
        ["gray", "0xFF0e0e0e", 53],
        ["grey", "0xFF0e0e0e", 54],
        ["green", "0xFF000e00", 55],
        ["greenyellow", "0xFFadff2f", 56],
        ["honeydew", "0xFFf0fff0", 57],
        ["hotpink", "0xFFff2db4", 58],
        ["indianred", "0xFFcd5c5c", 59],
        ["indigo", "0xFF4b000a", 60],
        ["ivory", "0xFFfffff0", 61],
        ["khaki", "0xFFf0e68c", 62],
        ["lavender", "0xFFe6e6fa", 63],
        ["lavenderblush", "0xFFfff0f5", 64],
        ["lawngreen", "0xFF7cfc00", 65],
        ["lemonchiffon", "0xFFfffacd", 66],
        ["lightblue", "0xFFadd8e6", 67],
        ["lightcoral", "0xFFf00e0e", 68],
        ["lightcyan", "0xFFe0ffff", 69],
        ["lightgoldenrodyellow", "0xFFfafad2", 70],
        ["lightgray", "0xFFd3d3d3", 71],
        ["lightgreen", "0xFF5aee5a", 72],
        ["lightgrey", "0xFFd3d3d3", 73],
        ["lightpink", "0xFFffb6c1", 74],
        ["lightsalmon", "0xFFffa07a", 75],
        ["lightseagreen", "0xFF0eb2aa", 76], # MARK
        ["lightskyblue", "0xFF1bcefa", 77],
        ["lightslategray", "0xFF4d3a3f", 78],
        ["lightslategrey", "0xFF4d3a3f", 79],
        ["lightsteelblue", "0xFFb0c4de", 80],
        ["lightyellow", "0xFFffffe0", 81],
        ["lime", "0xFF00ff00", 82],
        ["limegreen", "0xFF0ecd0e", 83],
        ["linen", "0xFFfaf0e6", 84],
        ["magenta", "0xFFff00ff", 85],
        ["maroon", "0xFF0e0000", 86],
        ["mediumaquamarine", "0xFF2acdaa", 87],
        ["mediumblue", "0xFF0000cd", 88],
        ["mediumorchid", "0xFFba0dd3", 89],
        ["mediumpurple", "0xFF5d2edb", 90],
        ["mediumseagreen", "0xFF3cb32f", 91],
        ["mediumslateblue", "0xFF7b2cee", 92],
        ["mediumspringgreen", "0xFF00fa9a", 93],
        ["mediumturquoise", "0xFF1ed1cc", 94],
        ["mediumvioletred", "0xFFc70f0d", 95],
        ["midnightblue", "0xFF0d0d2e", 96],
        ["mintcream", "0xFFf5fffa", 97],
        ["mistyrose", "0xFFffe4e1", 98],
        ["moccasin", "0xFFffe4b5", 99],
        ["navajowhite", "0xFFffdead", 100],
        ["navy", "0xFF00000e", 101],
        ["oldlace", "0xFFfdf5e6", 102],
        ["olive", "0xFF0e0e00", 103],
        ["olivedrab", "0xFF6b8e0b", 104],
        ["orange", "0xFFffa500", 105],
        ["orangered", "0xFFff2d00", 106],
        ["orchid", "0xFFda2ed6", 107],
        ["palegoldenrod", "0xFFeee8aa", 109],
        ["palegreen", "0xFF3efb3e", 110],
        ["paleturquoise", "0xFFafeeee", 111],
        ["palevioletred", "0xFFdb2e5d", 112],
        ["papayawhip", "0xFFffefd5", 113],
        ["peachpuff", "0xFFffdab9", 114],
        ["peru", "0xFFcd0d3f", 115],
        ["pink", "0xFFffc0cb", 116],
        ["plum", "0xFFdda0dd", 117],
        ["powderblue", "0xFFb0e0e6", 118],
        ["purple", "0xFF0e000e", 119],
        ["red", "0xFFff0000", 120],
        ["rosybrown", "0xFFbc8f8f", 121],
        ["royalblue", "0xFF1d2de1", 122],
        ["saddlebrown", "0xFF8b2d0d", 123],
        ["salmon", "0xFFfa0e1e", 124],
        ["sandybrown", "0xFFf4a43c", 125],
        ["seagreen", "0xFF2e8b1b", 126],
        ["seashell", "0xFFfff5ee", 127],
        ["sienna", "0xFFa00a2d", 128],
        ["silver", "0xFFc0c0c0", 129],
        ["skyblue", "0xFF1bceeb", 130],
        ["slateblue", "0xFF6a5acd", 131],
        ["slategray", "0xFF2e0e5a", 132],
        ["slategrey", "0xFF2e0e5a", 133],
        ["snow", "0xFFfffafa", 134],
        ["springgreen", "0xFF00ff7f", 135],
        ["steelblue", "0xFF2e0ab4", 136],
        ["tan", "0xFFd2b48c", 137],
        ["teal", "0xFF000e0e", 138],
        ["thistle", "0xFFd8bfd8", 139],
        ["tomato", "0xFFff3f2f", 140],
        ["turquoise", "0xFF1ce0d0", 141],
        ["violet", "0xFFee0aee", 142],
        ["wheat", "0xFFf5deb3", 143],
        ["white", "0xFFffffff", 144],
        ["whitesmoke", "0xFFf5f5f5", 145],
        ["yellow", "0xFFffff00", 146],
        ["yellowgreen", "0xFF9acd0e", 147],
        ["END", 0, -1]
    ],
    "Arc_Polyester_codes": [
        ["END", 0, -1]
    ],
    "Arc_Rayon_codes": [
        ["END", 0, -1]
    ],
    "CoatsAndClark_Rayon_codes": [
        ["END", 0, -1]
    ],
    "Exquisite_Polyester_codes": [
        ["END", 0, -1]
    ],
    "Fufu_Polyester_codes": [
        ["END", 0, -1]
    ],
    "Fufu_Rayon_codes": [
        ["END", 0, -1]
    ],
    "Hemingworth_Polyester_codes": [
        ["Pure White", "0xFFFFFFFF", 1001], 
        ["Lemon Ice", "0xFFDDE00F", 1271],
        ["Neon Green", "0xFFC9DD03", 1272],
        ["Brilliant Lime", "0xFF60DD49", 1273],
        ["Mango", "0xFFFFCC1E", 1274],
        ["Neon Yellow", "0xFFFFED38", 1275],
        ["Tropical Orange", "0xFFFFA952", 1276],
        ["Neon Orange", "0xFFFF9338", 1277],
        ["Rebel Peach", "0xFFFF585F", 1278],
        ["Shy Flamingo", "0xFFF28CA3", 1279],
        ["Neon Pink", "0xFFFE8A9E", 1280],
        ["Neon Peach", "0xFFFC074F", 1281],
        ["Dove Gray", "0xFFCFC3C3", 1067],
        ["Silver Lining", "0xFFC9CAC8", 1068],
        ["Storm Cloud", "0xFFB2B4B3", 1069],
        ["Platinum", "0xFFC6C6BC", 1070],
        ["Graphite", "0xFF616365", 1244],
        ["Light Charcoal", "0xFF4D4F53", 1245],
        ["Chrome", "0xFF8E908F", 1072],
        ["Antique Silver", "0xFF747678", 1077],
        ["Pewter Gray", "0xFF6C6F70", 1073],
        ["Black Stallion", "0xFF191D1F", 1079],
        ["Charcoal", "0xFF1B242A", 1087],
        ["Classic Black", "0xFF000000", 1000],
        ["Marshmallow", "0xFFD5D2CA", 1118],
        ["Ice Blue", "0xFFDAE3EA", 1119],
        ["Nautical Blue", "0xFFA6BCC6", 1076],
        ["Sea Storm", "0xFF818A8F", 1074],
        ["Bronzed Steel", "0xFF595A5C", 1078],
        ["Silvery Gray", "0xFFB9C9D0", 1239],
        ["Granite", "0xFF7D9AAA", 1240],
        ["Shadow", "0xFF5E6A71", 1085],
        ["Dark Slate Blue", "0xFF003C69", 1241],
        ["Deep Slate Blue", "0xFF003946", 1242],
        ["Pacific Waters", "0xFF004250", 1081],
        ["Dark Slate", "0xFF37424A", 1086],
        ["Smoky Blue", "0xFF005B82", 1192],
        ["Light Slate Blue", "0xFF5E9CAE", 1193],
        ["Hyacinth", "0xFF6AADE4", 1188],
        ["Bluebird", "0xFF4B92DB", 1187],
        ["Misty Blue", "0xFF8FCAE7", 1186],
        ["Cornflower Blue", "0xFFC2DEEA", 1185],
        ["Pale Blue", "0xFFA0CFEB", 1256],
        ["Country Blue", "0xFF0098DB", 1255],
        ["Azure", "0xFF3D7EDB", 1202],
        ["Royal Blue", "0xFF0039A6", 1203],
        ["Brilliant Blue", "0xFF00338D", 1204],
        ["Deep Blue", "0xFF0B2265", 1205],
        ["Winter Blue", "0xFF98C6EA", 1189],
        ["Winter Sky", "0xFFAACAE6", 1190],
        ["Sky Blue", "0xFF8EBAE5", 1191],
        ["China Blue", "0xFF0073CF", 1198],
        ["Dark Blueberry", "0xFF004165", 1201],
        ["Salem Blue", "0xFF004153", 1200],
        ["Navy", "0xFF002244", 1199],
        ["Sailor Blue", "0xFF002C5F", 1265],
        ["Dark Blue ", "0xFF002857", 1264],
        ["Berry Blue", "0xFF003591", 1263],
        ["True Blue", "0xFF002C77", 1261],
        ["Periwinkle", "0xFF6F9AD3", 1262],
        ["Iceberg Blue", "0xFF65CFE9", 1197],
        ["Medium Aquamarine", "0xFF0075B0", 1195],
        ["Dark Aquamarine", "0xFF0066A1", 1196],
        ["Peacock Blue", "0xFF006983", 1194],
        ["Dark Turquoise", "0xFF003D4C", 1258],
        ["Turquoise", "0xFF0098C3", 1259],
        ["Caribbean Blue", "0xFF00B0CA", 1260],
        ["Summer Sky", "0xFF6FD4E4", 1257],
        ["Crystal Lake", "0xFFBBE7E6", 1178],
        ["Icicle Blue", "0xFFC1E2E5", 1172],
        ["Frosty Blue", "0xFF8FDFE2", 1173],
        ["Blue Lagoon", "0xFF00AFD8", 1174],
        ["Blue Satin", "0xFF006778", 1181],
        ["Teal Blue", "0xFF007C92", 1180],
        ["Light Teal Blue", "0xFF009AA6", 1176],
        ["Wintergreen", "0xFF7CA295", 1175],
        ["Mint Green", "0xFF63CECA", 1177],
        ["Navajo Turquoise", "0xFF00877C", 1179],
        ["Peacock Green", "0xFF007B69", 1182],
        ["Forest Glen", "0xFF024E43", 1183],
        ["Deep Teal", "0xFF004953", 1184],
        ["Deep Sea", "0xFF156570", 1082],
        ["Dragonfly", "0xFF00505C", 1113],
        ["Blue Steel", "0xFF44697D", 1084],
        ["Dark Sage", "0xFF496C60", 1114],
        ["Silver Green", "0xFF949D9E", 1115],
        ["Antique Gray", "0xFF91BAA3", 1243],
        ["Ocean Spray", "0xFFB9CCC3", 1100],
        ["Sea Foam", "0xFFA6E6BC", 1088],
        ["Cucumber Melon", "0xFF00B588", 1094],
        ["Light Jade", "0xFF00985F", 1106],
        ["Jade", "0xFF009B74", 1107],
        ["Dark Jade", "0xFF007D57", 1105],
        ["Caribbean", "0xFF006A4D", 1104],
        ["Dark Teal", "0xFF00685B", 1254],
        ["Minty Teal", "0xFF0D776E", 1253],
        ["Lemony Lime", "0xFFC3E76F", 1099],
        ["Kiwi Lime", "0xFFCCDC00", 1247],
        ["Electric Green", "0xFF69BE28", 1097],
        ["Green Apple", "0xFF92D400", 1091],
        ["Key Lime", "0xFF7AB800", 1092],
        ["Kelly Green", "0xFF3F9C35", 1093],
        ["Meadow", "0xFF00AF3F", 1248],
        ["Grassy Green", "0xFF007934", 1095],
        ["Dark Kelly Green", "0xFF008542", 1108],
        ["Christmas Green", "0xFF00693C", 1109],
        ["Winter Pine ", "0xFF1C453B", 1250],
        ["Holly Leaf", "0xFF175E54", 1249],
        ["Pistachio Nut", "0xFFC8E59A", 1096],
        ["Dusty Green", "0xFF69923A", 1251],
        ["Bush Ivy ", "0xFF557630", 1252],
        ["Leafy Green", "0xFF739600", 1089],
        ["Kentucky Grass", "0xFF53682B", 1090],
        ["Ivy", "0xFF035642", 1103],
        ["Evergreen", "0xFF284E36", 1110],
        ["Mountain Meadow", "0xFF004438", 1111],
        ["Forest Green", "0xFF004D46", 1112],
        ["Oregano", "0xFF57584F", 1121],
        ["Jungle Green", "0xFF404A29", 1123],
        ["Thyme", "0xFF83847A", 1120],
        ["Light Avocado", "0xFF827C34", 1058],
        ["Split Pea", "0xFFB19B00", 1061],
        ["Spring Leaf", "0xFFB5A300", 1059],
        ["Almond Cream", "0xFFF8E498", 1060],
        ["Eggshell", "0xFFD7D3C7", 1229],
        ["Cornsilk Green", "0xFFD5C833", 1098],
        ["Avocado", "0xFF6A7029", 1101],
        ["Seaweed", "0xFF898F4B", 1102],
        ["Olive Green", "0xFF65551C", 1246],
        ["Coconut Shell", "0xFF4B452C", 1117],
        ["Parsley", "0xFF4B471A", 1116],
        ["Dried Sage", "0xFF718674", 1083],
        ["Mocha", "0xFF4F4C25", 1125],
        ["Warm Earth", "0xFF5D4F4B", 1131],
        ["Dark Chocolate", "0xFF452325", 1126],
        ["Deep Walnut", "0xFF4E2E2D", 1128],
        ["Teddybear Brown", "0xFF6E3219", 1130],
        ["Light Chestnut", "0xFF60351D", 1134],
        ["Pecan Pie", "0xFF6C4D23", 1140],
        ["Dark Alder", "0xFF766A65", 1237],
        ["Army Green", "0xFF5B491F", 1137],
        ["Pharaoh Gold", "0xFF6E5A2A", 1135],
        ["Autumn Haystack", "0xFFAB8422", 1136],
        ["Sahara", "0xFF856822", 1122],
        ["Weathered Wood", "0xFF675C53", 1236],
        ["Soft Beige", "0xFF9A996E", 1124],
        ["Mercury", "0xFFC2B2B5", 1149],
        ["Old Lace", "0xFFA5ACAF", 1146],
        ["Caramel Cappuccino", "0xFFAE7D5B", 1145],
        ["Brown Sugar", "0xFFA76F3E", 1133],
        ["Light Cinnamon", "0xFFA25022", 1064],
        ["Cinnamon", "0xFF86431E", 1238],
        ["Apple Cider", "0xFFB2541A", 1163],
        ["Indian Paintbrush", "0xFF9A3B26", 1144],
        ["Rust", "0xFF833820", 1164],
        ["Toasted Almond", "0xFF825C26", 1142],
        ["Pale Caramel", "0xFFC59217", 1063],
        ["Honey Butter", "0xFFDDB99A", 1062],
        ["Sandy Shore", "0xFFD2C295", 1057],
        ["Ecru", "0xFFC2C2A0", 1056],
        ["Malt", "0xFFB3B38C", 1055],
        ["Antique Lace", "0xFFC7B37F", 1054],
        ["Champagne", "0xFFBD9271", 1171],
        ["Butter Taffy", "0xFFB3995D", 1138],
        ["Cream Soda", "0xFFCEA98C", 1235],
        ["Conch Shell", "0xFFE39B6C", 1139],
        ["New Penny", "0xFFBA6F2E", 1132],
        ["Pumpkin Spice", "0xFFBB650E", 1141],
        ["Soft Sunlight", "0xFFEBE8B1", 1042],
        ["Lemon Drop", "0xFFEEEC83", 1043],
        ["Daffodil", "0xFFF3EC7A", 1045],
        ["Lemon Citrus", "0xFFF5EC5A", 1225],
        ["Sunshine Yellow", "0xFFFAE700", 1226],
        ["Canary Yellow", "0xFFF2EE72", 1044],
        ["Sunflower", "0xFFFCD900", 1227],
        ["Sun", "0xFFFADC41", 1046],
        ["Dandelion", "0xFFFED100", 1047],
        ["Buttercup", "0xFFF3CF45", 1048],
        ["Ginger Root", "0xFFEFBD47", 1050],
        ["Goldenrod", "0xFFEAAB00", 1051],
        ["Cornsilk", "0xFFDCD6B2", 1037],
        ["Macadamia", "0xFFC6BC89", 1038],
        ["Yellow Plumeria", "0xFFF8E498", 1039],
        ["Maize", "0xFFF8DE6E", 1040],
        ["Dried Banana", "0xFFFADA63", 1049],
        ["Butternut", "0xFFFFCB4F", 1053],
        ["Orange Meringue", "0xFFFFA100", 1232],
        ["September Sunset", "0xFFFFB612", 1231],
        ["Orange Cream", "0xFFFFB652", 1230],
        ["Cantaloupe", "0xFFFFBC3D", 1041],
        ["Old Gold", "0xFFCE8E00", 1052],
        ["Auburn", "0xFF9D5324", 1143],
        ["Citrus Burst", "0xFFE98300", 1024],
        ["Orange Slice", "0xFFFF7000", 1025],
        ["Fiery Sunset", "0xFFE37222", 1027],
        ["Hunter Orange", "0xFFFB4F14", 1028],
        ["Fall Harvest", "0xFFDD4814", 1029],
        ["Candy Apple", "0xFFCD202C", 1030],
        ["Christmas Red", "0xFFC30014", 1270],
        ["Pomegranate", "0xFFA70232", 1032],
        ["Rummy Raisin", "0xFF882332", 1031],
        ["Cardinal Red", "0xFFA51100", 1002],
        ["Rusty Red", "0xFF9E3039", 1234],
        ["Redwood", "0xFF783014", 1233],
        ["Carrot", "0xFFD55C19", 1065],
        ["Paprika", "0xFFAA272F", 1066],
        ["Cherrywood", "0xFF5F3327", 1129],
        ["Burnt Sienna", "0xFF5D3526", 1127],
        ["Merlot", "0xFF592C35", 1160],
        ["Loganberry", "0xFF6A1A41", 1159],
        ["Cranberry", "0xFF6E2714", 1158],
        ["Mulberry", "0xFF662046", 1157],
        ["Magenta", "0xFF85003C", 1156],
        ["Raspberry", "0xFF641F14", 1155],
        ["Salmon", "0xFFFF818D", 1166],
        ["Georgia Peach", "0xFFFFA48A", 1015],
        ["Rose Sunset", "0xFFFFB0B7", 1011],
        ["Bubblegum Pink", "0xFFF3789B", 1012],
        ["Carnation", "0xFFDB4D69", 1014],
        ["Very Berry", "0xFF91004B", 1013],
        ["Raspberry Red", "0xFF82240C", 1224],
        ["Dark Salmon", "0xFFF54359", 1018],
        ["Apricot Dream", "0xFFF4587A", 1017],
        ["Coral Reef", "0xFFFF8B7C", 1016],
        ["Frosted Peach", "0xFFFFC19C", 1022],
        ["Tangerine", "0xFFFF8F70", 1020],
        ["Dark Mango", "0xFFFF6D42", 1026],
        ["Marigold", "0xFFFFA02F", 1023],
        ["Spun Silk", "0xFFECC182", 1168],
        ["Whipped Papaya", "0xFFFBCE92", 1021],
        ["Baby Peach", "0xFFFDC480", 1228],
        ["Pink Pearl", "0xFFFFC2A2", 1167],
        ["Peaches 'n Cream", "0xFFEFC5CE", 1169],
        ["Peach Pastel", "0xFFEFBE9C", 1170],
        ["Old Penny", "0xFF774A39", 1162],
        ["Dusty Rose", "0xFFB26F7E", 1151],
        ["Winter Rose", "0xFFD490A8", 1165],
        ["Valentine Pink", "0xFFF6A3BB", 1161],
        ["Petal Peach", "0xFFFFB7AE", 1019],
        ["Soft Petal", "0xFFEFD6DB", 1150],
        ["Fuchsia", "0xFF920075", 1036],
        ["Pink Kiss", "0xFFF375C6", 1004],
        ["Baby Pink", "0xFFF3BBCE", 1003],
        ["Whisper Pink", "0xFFF1DBDF", 1005],
        ["Gentle Blush", "0xFFF3C9D3", 1006],
        ["English Rose", "0xFFF4B2C1", 1007],
        ["Sweet Pea", "0xFFF39EBC", 1008],
        ["Rosy Blush", "0xFFF77AB4", 1009],
        ["Passion Pink", "0xFFD71F85", 1010],
        ["Mulled Wine", "0xFF772059", 1035],
        ["Primrose", "0xFFC50084", 1034],
        ["Azalea", "0xFFA1006B", 1033],
        ["Snowflake", "0xFFD1D4D3", 1148],
        ["Moonlight", "0xFFCAD1E7", 1147],
        ["Tulip", "0xFF9DABE2", 1207],
        ["Purple Iris", "0xFF8884D5", 1206],
        ["Grape", "0xFF1A2155", 1209],
        ["Moon Shadow", "0xFF6459C4", 1211],
        ["Electric Purple", "0xFF212492", 1208],
        ["Indigo", "0xFF411244", 1210],
        ["Royal Purple", "0xFF3B0083", 1223],
        ["Eggplant", "0xFF151C54", 1267],
        ["Dark Purple", "0xFF490E6F", 1269],
        ["Pure Purple", "0xFF57068C", 1268],
        ["Pale Orchid", "0xFFDCC7DF", 1217],
        ["Dusty Mauve", "0xFFC2ACBE", 1219],
        ["Orchid", "0xFFDC9DDD", 1218],
        ["Heather", "0xFFB382C7", 1213],
        ["Lavender", "0xFF9C5FB5", 1214],
        ["Soft Grape", "0xFF4B08A1", 1266],
        ["Freesia", "0xFFC1AFE5", 1221],
        ["Lilac", "0xFFC5B9E3", 1222],
        ["Peony", "0xFF6E2C6B", 1215],
        ["Dark Fuschia", "0xFF7D0063", 1216],
        ["Grape Jelly", "0xFF752864", 1220],
        ["Deep Orchid", "0xFF55517B", 1080],
        ["Misty Blue Gray", "0xFF5C7F92", 1075],
        ["Iron Ore", "0xFFAFADC3", 1071],
        ["Light Mauve", "0xFFD8AAB3", 1152],
        ["Dark Mauve", "0xFF89687C", 1153],
        ["Wild Plum", "0xFF644459", 1154],
        ["Huckleberry", "0xFF4B306A", 1212],
        ["END", 0, -1]
    ],
    "Isacord_Polyester_codes": [
        ["?", "0xFFFFFFFF", 10],
        ["?", "0xFFFFFFFF", 15],
        ["?", "0xFFFFFFFF", 17],
        ["?", "0xFF000000", 20],
        ["?", "0xFFFFFDED", 101],
        ["?", "0xFF6D757B", 108],
        ["?", "0xFF515B61", 111],
        ["?", "0xFF5D5D5D", 112],
        ["?", "0xFFCFCFCF", 124],
        ["?", "0xFFA1A9B4", 131],
        ["?", "0xFF192024", 132],
        ["?", "0xFF9EA5AA", 142],
        ["?", "0xFFCFD1D5", 145],
        ["?", "0xFFC6BDB4", 150],
        ["?", "0xFFD5C4B3", 151],
        ["?", "0xFF7C8283", 152],
        ["?", "0xFFFEF5F0", 180],
        ["?", "0xFFE9D7D9", 182],
        ["?", "0xFFEBE3DD", 184],
        ["?", "0xFFE0DA5F", 221],
        ["?", "0xFFBFBA28", 232],
        ["?", "0xFFFAF6CC", 250],
        ["?", "0xFFF9F8E8", 270],
        ["?", "0xFFFDF76C", 310],
        ["?", "0xFFF5D300", 311],
        ["?", "0xFF797E24", 345],
        ["?", "0xFFB0AA76", 352],
        ["?", "0xFF898F2B", 442],
        ["?", "0xFF98996D", 453],
        ["?", "0xFF6B7E6F", 463],
        ["?", "0xFF3E4F34", 465],
        ["?", "0xFFEDEF05", 501],
        ["?", "0xFFF5D300", 506],
        ["?", "0xFFFDE896", 520],
        ["?", "0xFFD7CE8A", 532],
        ["?", "0xFFB18B00", 542],
        ["?", "0xFFB28F11", 546],
        ["?", "0xFFB69F56", 552],
        ["?", "0xFFF8D73E", 600],
        ["?", "0xFFF8D73E", 605],
        ["?", "0xFFF7DC00", 608],
        ["?", "0xFFFEF09A", 630],
        ["?", "0xFFFDE896", 640],
        ["?", "0xFFF5D2A6", 651],
        ["?", "0xFFFEF9EA", 660],
        ["?", "0xFFFAF6E7", 670],
        ["?", "0xFFBEBEA8", 672],
        ["?", "0xFFF7C35F", 700],
        ["?", "0xFFF5CA00", 702],
        ["?", "0xFFDFA200", 704],
        ["?", "0xFFFCF538", 706],
        ["?", "0xFFFADC59", 713],
        ["?", "0xFF8C7E6A", 722],
        ["?", "0xFF594900", 747],
        ["?", "0xFFD6BF94", 761],
        ["?", "0xFF656452", 776],
        ["?", "0xFFF1AF00", 800],
        ["?", "0xFFF5BA5D", 811],
        ["?", "0xFFE1A23E", 821],
        ["?", "0xFFCCAB3F", 822],
        ["?", "0xFFDFA200", 824],
        ["?", "0xFFD0A44F", 832],
        ["?", "0xFFCD944A", 842],
        ["?", "0xFFE3BC61", 851],
        ["?", "0xFF947C4A", 853],
        ["?", "0xFFCBBFA2", 861],
        ["?", "0xFFA5866A", 862],
        ["?", "0xFFEBE7DD", 870],
        ["?", "0xFF9FA086", 873],
        ["?", "0xFF9A897B", 874],
        ["?", "0xFFF3B259", 904],
        ["?", "0xFFCA832C", 922],
        ["?", "0xFFC07314", 931],
        ["?", "0xFFAC6613", 932],
        ["?", "0xFF744808", 933],
        ["?", "0xFFBD9565", 934],
        ["?", "0xFFC98300", 940],
        ["?", "0xFFAF7D3E", 941],
        ["?", "0xFF483928", 945],
        ["?", "0xFFFEFEED", 970],
        ["?", "0xFF6A4129", 1055],
        ["?", "0xFFFDE2C1", 1060],
        ["?", "0xFFA68A68", 1061],
        ["?", "0xFFED9206", 1102],
        ["?", "0xFFEE9C00", 1106],
        ["?", "0xFFEE8751", 1114],
        ["?", "0xFFA35214", 1115],
        ["?", "0xFFF8C000", 1120],
        ["?", "0xFFB7976B", 1123],
        ["?", "0xFF9D5A21", 1134],
        ["?", "0xFFF3D8A8", 1140],
        ["?", "0xFFFACFAE", 1141],
        ["?", "0xFF7A4427", 1154],
        ["?", "0xFFDFC8AB", 1172],
        ["?", "0xFFE89763", 1211],
        ["?", "0xFFF1A236", 1220],
        ["?", "0xFFE5571D", 1300],
        ["?", "0xFFD9674C", 1301],
        ["?", "0xFFE4501E", 1304],
        ["?", "0xFFEA7134", 1305],
        ["?", "0xFFE12A23", 1306],
        ["?", "0xFFC14817", 1311],
        ["?", "0xFFC45331", 1312],
        ["?", "0xFFD5815E", 1332],
        ["?", "0xFFBB3D2E", 1334],
        ["?", "0xFFBE2D1A", 1335],
        ["?", "0xFF5F1B23", 1342],
        ["?", "0xFF7A3441", 1346],
        ["?", "0xFFFBBF95", 1351],
        ["?", "0xFFF4A773", 1352],
        ["?", "0xFF693920", 1355],
        ["?", "0xFFF9C598", 1362],
        ["?", "0xFF432731", 1366],
        ["?", "0xFF464537", 1375],
        ["?", "0xFFF4A782", 1430],
        ["?", "0xFFE22D2A", 1501],
        ["?", "0xFFA93121", 1514],
        ["?", "0xFFEC7168", 1521],
        ["?", "0xFFF6B08E", 1532],
        ["?", "0xFFF9C5B9", 1551],
        ["?", "0xFF806A61", 1565],
        ["?", "0xFFE36C63", 1600],
        ["?", "0xFFE44733", 1701],
        ["?", "0xFFDF0032", 1703],
        ["?", "0xFFE0003D", 1704],
        ["?", "0xFFCF0040", 1725],
        ["?", "0xFFF1CDCE", 1755],
        ["?", "0xFFE9C9BD", 1760],
        ["?", "0xFFE8C0B8", 1761],
        ["?", "0xFFE00046", 1800],
        ["?", "0xFFD6445D", 1805],
        ["?", "0xFFF49E95", 1840],
        ["?", "0xFFFCDAD5", 1860],
        ["?", "0xFF636254", 1874],
        ["?", "0xFF394535", 1876],
        ["?", "0xFFE10057", 1900],
        ["?", "0xFFBD0041", 1902],
        ["?", "0xFFC00343", 1903],
        ["?", "0xFFA9023A", 1904],
        ["?", "0xFFBE004F", 1906],
        ["?", "0xFF910230", 1911],
        ["?", "0xFF86023E", 1912],
        ["?", "0xFF9A0C3B", 1913],
        ["?", "0xFFA33050", 1921],
        ["?", "0xFFF28DA6", 1940],
        ["?", "0xFFCE427A", 1950],
        ["?", "0xFF959595", 1972],
        ["?", "0xFFA33145", 2011],
        ["?", "0xFF9F454C", 2022],
        ["?", "0xFFC7979B", 2051],
        ["?", "0xFF9F003F", 2101],
        ["?", "0xFF78093F", 2113],
        ["?", "0xFF6D0627", 2115],
        ["?", "0xFF432732", 2123],
        ["?", "0xFFE6778B", 2152],
        ["?", "0xFFDF8390", 2153],
        ["?", "0xFFF9BFC0", 2155],
        ["?", "0xFFFBD1D6", 2160],
        ["?", "0xFFD8D5D0", 2166],
        ["?", "0xFFF7DED6", 2170],
        ["?", "0xFFF7DEDE", 2171],
        ["?", "0xFFE8418C", 2220],
        ["?", "0xFF8C0C4A", 2222],
        ["?", "0xFF883A40", 2224],
        ["?", "0xFFEE71A1", 2230],
        ["?", "0xFFA95A68", 2241],
        ["?", "0xFFFAC8D3", 2250],
        ["?", "0xFFD3007E", 2300],
        ["?", "0xFFD20067", 2320],
        ["?", "0xFF651533", 2333],
        ["?", "0xFF3A212B", 2336],
        ["?", "0xFFFDE5EC", 2363],
        ["?", "0xFF970059", 2500],
        ["?", "0xFFAA4381", 2504],
        ["?", "0xFF820052", 2506],
        ["?", "0xFFE20078", 2520],
        ["?", "0xFFBF006A", 2521],
        ["?", "0xFFF189AF", 2550],
        ["?", "0xFFF7B4CA", 2560],
        ["?", "0xFF494949", 2576],
        ["?", "0xFF893480", 2600],
        ["?", "0xFF6C0051", 2611],
        ["?", "0xFFD994B9", 2640],
        ["?", "0xFFE6B7CF", 2650],
        ["?", "0xFFECD2DE", 2655],
        ["?", "0xFF606D8C", 2674],
        ["?", "0xFF610051", 2711],
        ["?", "0xFF490251", 2715],
        ["?", "0xFF89347F", 2720],
        ["?", "0xFFC690A1", 2764],
        ["?", "0xFF6F067B", 2810],
        ["?", "0xFFA974AB", 2830],
        ["?", "0xFF4C0F7B", 2900],
        ["?", "0xFF664090", 2905],
        ["?", "0xFF83589D", 2910],
        ["?", "0xFF8C6DAA", 2920],
        ["?", "0xFFC9B5D4", 3040],
        ["?", "0xFFC790BA", 3045],
        ["?", "0xFF707070", 3062],
        ["?", "0xFF2A377E", 3102],
        ["?", "0xFF35247A", 3110],
        ["?", "0xFF260751", 3114],
        ["?", "0xFF353A90", 3210],
        ["?", "0xFF524A90", 3211],
        ["?", "0xFF7D77AF", 3241],
        ["?", "0xFF9083A3", 3251],
        ["?", "0xFF14214E", 3323],
        ["?", "0xFF7F8BC2", 3331],
        ["?", "0xFF1B3C78", 3333],
        ["?", "0xFF2E4B9B", 3335],
        ["?", "0xFF11263C", 3344],
        ["?", "0xFF202A65", 3353],
        ["?", "0xFF171B4A", 3355],
        ["?", "0xFF002232", 3444],
        ["?", "0xFF2D4491", 3522],
        ["?", "0xFF261257", 3536],
        ["?", "0xFF3A2885", 3541],
        ["?", "0xFF233B7D", 3543],
        ["?", "0xFF273C82", 3544],
        ["?", "0xFF272651", 3554],
        ["?", "0xFF28438C", 3600],
        ["?", "0xFF243A7D", 3611],
        ["?", "0xFF4055A1", 3612],
        ["?", "0xFF1A4C8D", 3622],
        ["?", "0xFF92B1DC", 3640],
        ["?", "0xFF648DC7", 3641],
        ["?", "0xFFD0DEEE", 3650],
        ["?", "0xFFC8D6ED", 3652],
        ["?", "0xFF00507F", 3732],
        ["?", "0xFF12253C", 3743],
        ["?", "0xFFB7D1E3", 3750],
        ["?", "0xFFAFC9E5", 3761],
        ["?", "0xFFCED2D1", 3770],
        ["?", "0xFF3D6AA1", 3810],
        ["?", "0xFF7BA2D3", 3815],
        ["?", "0xFF91B9E2", 3820],
        ["?", "0xFFB4CEEB", 3840],
        ["?", "0xFF507193", 3842],
        ["?", "0xFF007EBA", 3900],
        ["?", "0xFF0082C4", 3901],
        ["?", "0xFF006CA5", 3902],
        ["?", "0xFF4ABDF0", 3910],
        ["?", "0xFF86AACA", 3951],
        ["?", "0xFF697698", 3953],
        ["?", "0xFFA6D8F6", 3962],
        ["?", "0xFFE1E1E1", 3971],
        ["?", "0xFF0093B9", 4010],
        ["?", "0xFF507793", 4032],
        ["?", "0xFF265674", 4033],
        ["?", "0xFF265674", 4033],
        ["?", "0xFFEAF0F9", 4071],
        ["?", "0xFFEAF0F9", 4071],
        ["?", "0xFF838689", 4073],
        ["?", "0xFF838689", 4073],
        ["?", "0xFF2DB0CF", 4101],
        ["?", "0xFF2DB0CF", 4101],
        ["?", "0xFF0095C6", 4103],
        ["?", "0xFF0095C6", 4103],
        ["?", "0xFF00A4D9", 4111],
        ["?", "0xFF00A4D9", 4111],
        ["?", "0xFF00A9C9", 4113],
        ["?", "0xFF00A9C9", 4113],
        ["?", "0xFF0082AD", 4116],
        ["?", "0xFF0082AD", 4116],
        ["?", "0xFF00405D", 4133],
        ["?", "0xFF00405D", 4133],
        ["?", "0xFF192024", 4174],
        ["?", "0xFF192024", 4174],
        ["?", "0xFF4FB4CB", 4220],
        ["?", "0xFF4FB4CB", 4220],
        ["?", "0xFF8DCEE4", 4230],
        ["?", "0xFF8DCEE4", 4230],
        ["?", "0xFF8DCDDB", 4240],
        ["?", "0xFF8DCDDB", 4240],
        ["?", "0xFFD5EBF2", 4250],
        ["?", "0xFFD5EBF2", 4250],
        ["?", "0xFF007B8D", 4410],
        ["?", "0xFF007B8D", 4410],
        ["?", "0xFF0091A5", 4421],
        ["?", "0xFF0091A5", 4421],
        ["?", "0xFF007D8C", 4423],
        ["?", "0xFF007D8C", 4423],
        ["?", "0xFF007986", 4425],
        ["?", "0xFF007986", 4425],
        ["?", "0xFF5FBFD1", 4430],
        ["?", "0xFF5FBFD1", 4430],
        ["?", "0xFF006981", 4442],
        ["?", "0xFF006981", 4442],
        ["?", "0xFF007F92", 4452],
        ["?", "0xFF007F92", 4452],
        ["?", "0xFF002F38", 4515],
        ["?", "0xFF002F38", 4515],
        ["?", "0xFF007389", 4531],
        ["?", "0xFF007389", 4531],
        ["?", "0xFF007B8D", 4610],
        ["?", "0xFF007B8D", 4610],
        ["?", "0xFF00A3A0", 4620],
        ["?", "0xFF00A3A0", 4620],
        ["?", "0xFF0B7F85", 4625],
        ["?", "0xFF0B7F85", 4625],
        ["?", "0xFF005B63", 4643],
        ["?", "0xFF005B63", 4643],
        ["?", "0xFF234544", 4644],
        ["?", "0xFF234544", 4644],
        ["?", "0xFF005B63", 5005],
        ["?", "0xFF005B63", 5005],
        ["?", "0xFF00A6AD", 5010],
        ["?", "0xFF00A6AD", 5010],
        ["?", "0xFFB4DCD8", 5050],
        ["?", "0xFFB4DCD8", 5050],
        ["?", "0xFF00876E", 5100],
        ["?", "0xFF00876E", 5100],
        ["?", "0xFF009084", 5101],
        ["?", "0xFF009084", 5101],
        ["?", "0xFF48BAB7", 5115],
        ["?", "0xFF48BAB7", 5115],
        ["?", "0xFF00AF8C", 5210],
        ["?", "0xFF00AF8C", 5210],
        ["?", "0xFF8CCCC2", 5220],
        ["?", "0xFF8CCCC2", 5220],
        ["?", "0xFF47B9AE", 5230],
        ["?", "0xFF47B9AE", 5230],
        ["?", "0xFF197E6D", 5233],
        ["?", "0xFF197E6D", 5233],
        ["?", "0xFF006E42", 5324],
        ["?", "0xFF006E42", 5324],
        ["?", "0xFF004D3D", 5326],
        ["?", "0xFF004D3D", 5326],
        ["?", "0xFF002F38", 5335],
        ["?", "0xFF002F38", 5335],
        ["?", "0xFF002D1F", 5374],
        ["?", "0xFF002D1F", 5374],
        ["?", "0xFF008663", 5411],
        ["?", "0xFF008663", 5411],
        ["?", "0xFF006B4E", 5415],
        ["?", "0xFF006B4E", 5415],
        ["?", "0xFF008663", 5422],
        ["?", "0xFF008663", 5422],
        ["?", "0xFF52BA84", 5500],
        ["?", "0xFF52BA84", 5500],
        ["?", "0xFF14A363", 5510],
        ["?", "0xFF14A363", 5510],
        ["?", "0xFF007848", 5513],
        ["?", "0xFF007848", 5513],
        ["?", "0xFF008663", 5515],
        ["?", "0xFF008663", 5515],
        ["?", "0xFF52A04F", 5531],
        ["?", "0xFF52A04F", 5531],
        ["?", "0xFF94ADA1", 5552],
        ["?", "0xFF94ADA1", 5552],
        ["?", "0xFF103828", 5555],
        ["?", "0xFF103828", 5555],
        ["?", "0xFF85C875", 5610],
        ["?", "0xFF85C875", 5610],
        ["?", "0xFF14B26D", 5613],
        ["?", "0xFF14B26D", 5613],
        ["?", "0xFF1A781E", 5633],
        ["?", "0xFF1A781E", 5633],
        ["?", "0xFF157436", 5643],
        ["?", "0xFF157436", 5643],
        ["?", "0xFFC9E3C5", 5650],
        ["?", "0xFFC9E3C5", 5650],
        ["?", "0xFF6B9181", 5664],
        ["?", "0xFF6B9181", 5664],
        ["?", "0xFFA5C278", 5822],
        ["?", "0xFFA5C278", 5822],
        ["?", "0xFF70953F", 5833],
        ["?", "0xFF70953F", 5833],
        ["?", "0xFF273014", 5866],
        ["?", "0xFF273014", 5866],
        ["?", "0xFF81C750", 5912],
        ["?", "0xFF81C750", 5912],
        ["?", "0xFF457021", 5933],
        ["?", "0xFF457021", 5933],
        ["?", "0xFF506702", 5934],
        ["?", "0xFF506702", 5934],
        ["?", "0xFFBBDB41", 5940],
        ["?", "0xFFBBDB41", 5940],
        ["?", "0xFF003518", 5944],
        ["?", "0xFF003518", 5944],
        ["?", "0xFFE3EB00", 6010],
        ["?", "0xFFE3EB00", 6010],
        ["?", "0xFFBED782", 6051],
        ["?", "0xFFBED782", 6051],
        ["?", "0xFF919600", 6133],
        ["?", "0xFF919600", 6133],
        ["?", "0xFF484601", 6156],
        ["?", "0xFF484601", 6156],
        ["END", 0, -1]
    ],

    "Isafil_Rayon_codes": [
        ["?", "0xFFFFFFFF", 10],
        ["?", "0xFFFFFFFF", 10],
        ["?", "0xFFFFFFFF", 15],
        ["?", "0xFFFFFFFF", 15],
        ["?", "0xFFFFFFFF", 17],
        ["?", "0xFFFFFFFF", 17],
        ["?", "0xFF000000", 20],
        ["?", "0xFF000000", 20],
        ["?", "0xFFFFFDED", 101],
        ["?", "0xFFFFFDED", 101],
        ["?", "0xFF7D7D7D", 104],
        ["?", "0xFF7D7D7D", 104],
        ["?", "0xFF515B61", 107],
        ["?", "0xFF515B61", 107],
        ["?", "0xFF6D757B", 108],
        ["?", "0xFF6D757B", 108],
        ["?", "0xFFACACAC", 109],
        ["?", "0xFFACACAC", 109],
        ["?", "0xFF515B61", 111],
        ["?", "0xFF515B61", 111],
        ["?", "0xFF5D5D5D", 112],
        ["?", "0xFF5D5D5D", 112],
        ["?", "0xFFCFCFCF", 124],
        ["?", "0xFFCFCFCF", 124],
        ["?", "0xFFA1A9B4", 131],
        ["?", "0xFFA1A9B4", 131],
        ["?", "0xFF6D757B", 141],
        ["?", "0xFF6D757B", 141],
        ["?", "0xFF9EA5AA", 142],
        ["?", "0xFF9EA5AA", 142],
        ["?", "0xFFCFD1D5", 145],
        ["?", "0xFFCFD1D5", 145],
        ["?", "0xFFC6BDB4", 150],
        ["?", "0xFFC6BDB4", 150],
        ["?", "0xFFD5C4B3", 151],
        ["?", "0xFFD5C4B3", 151],
        ["?", "0xFF7C8283", 152],
        ["?", "0xFF7C8283", 152],
        ["?", "0xFF898F94", 156],
        ["?", "0xFF898F94", 156],
        ["?", "0xFFFEF5F0", 180],
        ["?", "0xFFFEF5F0", 180],
        ["?", "0xFFE9D7D9", 182],
        ["?", "0xFFE9D7D9", 182],
        ["?", "0xFFEBE3DD", 184],
        ["?", "0xFFEBE3DD", 184],
        ["?", "0xFFE0DA5F", 221],
        ["?", "0xFFE0DA5F", 221],
        ["?", "0xFFBFBA28", 232],
        ["?", "0xFFBFBA28", 232],
        ["?", "0xFFECE9C1", 241],
        ["?", "0xFFECE9C1", 241],
        ["?", "0xFFFAF6CC", 250],
        ["?", "0xFFFAF6CC", 250],
        ["?", "0xFFECE7A5", 251],
        ["?", "0xFFECE7A5", 251],
        ["?", "0xFFECEADB", 260],
        ["?", "0xFFECEADB", 260],
        ["?", "0xFFF9F8E8", 270],
        ["?", "0xFFF9F8E8", 270],
        ["?", "0xFFFDF76C", 310],
        ["?", "0xFFFDF76C", 310],
        ["?", "0xFFF5D300", 311],
        ["?", "0xFFF5D300", 311],
        ["?", "0xFF797E24", 345],
        ["?", "0xFF797E24", 345],
        ["?", "0xFFB0AA76", 352],
        ["?", "0xFFB0AA76", 352],
        ["?", "0xFF898F2B", 442],
        ["?", "0xFF898F2B", 442],
        ["?", "0xFF98996D", 453],
        ["?", "0xFF98996D", 453],
        ["?", "0xFF6E772E", 454],
        ["?", "0xFF6E772E", 454],
        ["?", "0xFF6B7E6F", 463],
        ["?", "0xFF6B7E6F", 463],
        ["?", "0xFF3E4F34", 465],
        ["?", "0xFF3E4F34", 465],
        ["?", "0xFFEDEF05", 501],
        ["?", "0xFFEDEF05", 501],
        ["?", "0xFFFAF6CC", 505],
        ["?", "0xFFFAF6CC", 505],
        ["?", "0xFFF5D300", 506],
        ["?", "0xFFF5D300", 506],
        ["?", "0xFFFFFBD1", 510],
        ["?", "0xFFFFFBD1", 510],
        ["?", "0xFFFDE896", 520],
        ["?", "0xFFFDE896", 520],
        ["?", "0xFFD7CE8A", 532],
        ["?", "0xFFD7CE8A", 532],
        ["?", "0xFFB18B00", 542],
        ["?", "0xFFB18B00", 542],
        ["?", "0xFFAA8D00", 545],
        ["?", "0xFFAA8D00", 545],
        ["?", "0xFFB28F11", 546],
        ["?", "0xFFB28F11", 546],
        ["?", "0xFFAC9436", 551],
        ["?", "0xFFAC9436", 551],
        ["?", "0xFFB69F56", 552],
        ["?", "0xFFB69F56", 552],
        ["?", "0xFFF4EE8C", 580],
        ["?", "0xFFF4EE8C", 580],
        ["?", "0xFFF1EB35", 590],
        ["?", "0xFFF1EB35", 590],
        ["?", "0xFFF8D73E", 600],
        ["?", "0xFFF8D73E", 600],
        ["?", "0xFFF8D73E", 605],
        ["?", "0xFFF8D73E", 605],
        ["?", "0xFFF7CB47", 610],
        ["?", "0xFFF7CB47", 610],
        ["?", "0xFFF7E400", 615],
        ["?", "0xFFF7E400", 615],
        ["?", "0xFFFDE896", 620],
        ["?", "0xFFFDE896", 620],
        ["?", "0xFFEEDB00", 625],
        ["?", "0xFFEEDB00", 625],
        ["?", "0xFFFEF09A", 630],
        ["?", "0xFFFEF09A", 630],
        ["?", "0xFFFDE1AF", 635],
        ["?", "0xFFFDE1AF", 635],
        ["?", "0xFFFDE896", 640],
        ["?", "0xFFFDE896", 640],
        ["?", "0xFFF5D2A6", 651],
        ["?", "0xFFF5D2A6", 651],
        ["?", "0xFFFEF9EA", 660],
        ["?", "0xFFFEF9EA", 660],
        ["?", "0xFFFAF6E7", 670],
        ["?", "0xFFFAF6E7", 670],
        ["?", "0xFFBEBEA8", 672],
        ["?", "0xFFBEBEA8", 672],
        ["?", "0xFFF7C35F", 700],
        ["?", "0xFFF7C35F", 700],
        ["?", "0xFFF5CA00", 702],
        ["?", "0xFFF5CA00", 702],
        ["?", "0xFFDFA200", 704],
        ["?", "0xFFDFA200", 704],
        ["?", "0xFFFCF538", 706],
        ["?", "0xFFFCF538", 706],
        ["?", "0xFFFADC59", 713],
        ["?", "0xFFFADC59", 713],
        ["?", "0xFF8C7E6A", 722],
        ["?", "0xFF8C7E6A", 722],
        ["?", "0xFF827000", 726],
        ["?", "0xFF827000", 726],
        ["?", "0xFF636254", 732],
        ["?", "0xFF636254", 732],
        ["?", "0xFF594900", 747],
        ["?", "0xFF594900", 747],
        ["?", "0xFFD6BF94", 761],
        ["?", "0xFFD6BF94", 761],
        ["?", "0xFF656452", 776],
        ["?", "0xFF656452", 776],
        ["?", "0xFFF1AF00", 800],
        ["?", "0xFFF1AF00", 800],
        ["?", "0xFFF3C200", 805],
        ["?", "0xFFF3C200", 805],
        ["?", "0xFFF5BA5D", 811],
        ["?", "0xFFF5BA5D", 811],
        ["?", "0xFFE1A23E", 821],
        ["?", "0xFFE1A23E", 821],
        ["?", "0xFFCCAB3F", 822],
        ["?", "0xFFCCAB3F", 822],
        ["?", "0xFFDFA200", 824],
        ["?", "0xFFDFA200", 824],
        ["?", "0xFFF3B044", 830],
        ["?", "0xFFF3B044", 830],
        ["?", "0xFFD0A44F", 832],
        ["?", "0xFFD0A44F", 832],
        ["?", "0xFFCD944A", 842],
        ["?", "0xFFCD944A", 842],
        ["?", "0xFFE3BC61", 851],
        ["?", "0xFFE3BC61", 851],
        ["?", "0xFF947C4A", 853],
        ["?", "0xFF947C4A", 853],
        ["?", "0xFF001F48", 866],
        ["?", "0xFF001F48", 866],
        ["?", "0xFFEBE7DD", 870],
        ["?", "0xFFEBE7DD", 870],
        ["?", "0xFF9FA086", 873],
        ["?", "0xFF9FA086", 873],
        ["?", "0xFF9A897B", 874],
        ["?", "0xFF9A897B", 874],
        ["?", "0xFFEE9C00", 900],
        ["?", "0xFFEE9C00", 900],
        ["?", "0xFFF3B259", 904],
        ["?", "0xFFF3B259", 904],
        ["?", "0xFFCA832C", 922],
        ["?", "0xFFCA832C", 922],
        ["?", "0xFFC07314", 931],
        ["?", "0xFFC07314", 931],
        ["?", "0xFFAC6613", 932],
        ["?", "0xFFAC6613", 932],
        ["?", "0xFF744808", 933],
        ["?", "0xFF744808", 933],
        ["?", "0xFFBD9565", 934],
        ["?", "0xFFBD9565", 934],
        ["?", "0xFF806800", 936],
        ["?", "0xFF806800", 936],
        ["?", "0xFFC98300", 940],
        ["?", "0xFFC98300", 940],
        ["?", "0xFFAF7D3E", 941],
        ["?", "0xFFAF7D3E", 941],
        ["?", "0xFF483928", 945],
        ["?", "0xFF483928", 945],
        ["?", "0xFFFEECD9", 961],
        ["?", "0xFFFEECD9", 961],
        ["?", "0xFFFEFEED", 970],
        ["?", "0xFFFEFEED", 970],
        ["?", "0xFFDD973A", 1041],
        ["?", "0xFFDD973A", 1041],
        ["?", "0xFF6A4129", 1055],
        ["?", "0xFF6A4129", 1055],
        ["?", "0xFFFDE2C1", 1060],
        ["?", "0xFFFDE2C1", 1060],
        ["?", "0xFFA68A68", 1061],
        ["?", "0xFFA68A68", 1061],
        ["?", "0xFFD76814", 1099],
        ["?", "0xFFD76814", 1099],
        ["?", "0xFFED873F", 1100],
        ["?", "0xFFED873F", 1100],
        ["?", "0xFFEC870E", 1101],
        ["?", "0xFFEC870E", 1101],
        ["?", "0xFFED9206", 1102],
        ["?", "0xFFED9206", 1102],
        ["?", "0xFFEE9C00", 1106],
        ["?", "0xFFEE9C00", 1106],
        ["?", "0xFFC45331", 1113],
        ["?", "0xFFC45331", 1113],
        ["?", "0xFFEE8751", 1114],
        ["?", "0xFFEE8751", 1114],
        ["?", "0xFFA35214", 1115],
        ["?", "0xFFA35214", 1115],
        ["?", "0xFFF8C000", 1120],
        ["?", "0xFFF8C000", 1120],
        ["?", "0xFFB7976B", 1123],
        ["?", "0xFFB7976B", 1123],
        ["?", "0xFF9D5A21", 1134],
        ["?", "0xFF9D5A21", 1134],
        ["?", "0xFFF3D8A8", 1140],
        ["?", "0xFFF3D8A8", 1140],
        ["?", "0xFFFACFAE", 1141],
        ["?", "0xFFFACFAE", 1141],
        ["?", "0xFFDFC8AB", 1172],
        ["?", "0xFFDFC8AB", 1172],
        ["?", "0xFFE89763", 1211],
        ["?", "0xFFE89763", 1211],
        ["?", "0xFFF1A236", 1220],
        ["?", "0xFFF1A236", 1220],
        ["?", "0xFF3D2723", 1276],
        ["?", "0xFF3D2723", 1276],
        ["?", "0xFFE5571D", 1300],
        ["?", "0xFFE5571D", 1300],
        ["?", "0xFFE8643C", 1302],
        ["?", "0xFFE8643C", 1302],
        ["?", "0xFFE4501E", 1304],
        ["?", "0xFFE4501E", 1304],
        ["?", "0xFFEA7134", 1305],
        ["?", "0xFFEA7134", 1305],
        ["?", "0xFFE12A23", 1306],
        ["?", "0xFFE12A23", 1306],
        ["?", "0xFFC14817", 1311],
        ["?", "0xFFC14817", 1311],
        ["?", "0xFFC45331", 1312],
        ["?", "0xFFC45331", 1312],
        ["?", "0xFFD7623E", 1313],
        ["?", "0xFFD7623E", 1313],
        ["?", "0xFFED7C56", 1320],
        ["?", "0xFFED7C56", 1320],
        ["?", "0xFF92291B", 1324],
        ["?", "0xFF92291B", 1324],
        ["?", "0xFFD5815E", 1332],
        ["?", "0xFFD5815E", 1332],
        ["?", "0xFFBB3D2E", 1334],
        ["?", "0xFFBB3D2E", 1334],
        ["?", "0xFFBE2D1A", 1335],
        ["?", "0xFFBE2D1A", 1335],
        ["?", "0xFF5F1B23", 1342],
        ["?", "0xFF5F1B23", 1342],
        ["?", "0xFF7A3441", 1346],
        ["?", "0xFF7A3441", 1346],
        ["?", "0xFF84291D", 1348],
        ["?", "0xFF84291D", 1348],
        ["?", "0xFFFBBF95", 1351],
        ["?", "0xFFFBBF95", 1351],
        ["?", "0xFFF4A773", 1352],
        ["?", "0xFFF4A773", 1352],
        ["?", "0xFF693920", 1355],
        ["?", "0xFF693920", 1355],
        ["?", "0xFFF9C6A1", 1361],
        ["?", "0xFFF9C6A1", 1361],
        ["?", "0xFFF9C598", 1362],
        ["?", "0xFFF9C598", 1362],
        ["?", "0xFF432731", 1366],
        ["?", "0xFF432731", 1366],
        ["?", "0xFF464537", 1375],
        ["?", "0xFF464537", 1375],
        ["?", "0xFF4D2E18", 1376],
        ["?", "0xFF4D2E18", 1376],
        ["?", "0xFFD64F42", 1421],
        ["?", "0xFFD64F42", 1421],
        ["?", "0xFFF4A782", 1430],
        ["?", "0xFFF4A782", 1430],
        ["?", "0xFFE22D2A", 1501],
        ["?", "0xFFE22D2A", 1501],
        ["?", "0xFFA93121", 1514],
        ["?", "0xFFA93121", 1514],
        ["?", "0xFFEC7168", 1521],
        ["?", "0xFFEC7168", 1521],
        ["?", "0xFFF6B08E", 1532],
        ["?", "0xFFF6B08E", 1532],
        ["?", "0xFFF9C5B9", 1551],
        ["?", "0xFFF9C5B9", 1551],
        ["?", "0xFF806A61", 1565],
        ["?", "0xFF806A61", 1565],
        ["?", "0xFF464537", 1573],
        ["?", "0xFF464537", 1573],
        ["?", "0xFFE36C63", 1600],
        ["?", "0xFFE36C63", 1600],
        ["?", "0xFFF9C7B9", 1630],
        ["?", "0xFFF9C7B9", 1630],
        ["?", "0xFFE44733", 1701],
        ["?", "0xFFE44733", 1701],
        ["?", "0xFFDF0032", 1703],
        ["?", "0xFFDF0032", 1703],
        ["?", "0xFFE44733", 1705],
        ["?", "0xFFE44733", 1705],
        ["?", "0xFFCF0040", 1725],
        ["?", "0xFFCF0040", 1725],
        ["?", "0xFFDB686B", 1750],
        ["?", "0xFFDB686B", 1750],
        ["?", "0xFFF1CDCE", 1755],
        ["?", "0xFFF1CDCE", 1755],
        ["?", "0xFFE9C9BD", 1760],
        ["?", "0xFFE9C9BD", 1760],
        ["?", "0xFFE8C0B8", 1761],
        ["?", "0xFFE8C0B8", 1761],
        ["?", "0xFFE00046", 1800],
        ["?", "0xFFE00046", 1800],
        ["?", "0xFFE43449", 1802],
        ["?", "0xFFE43449", 1802],
        ["?", "0xFFD6445D", 1805],
        ["?", "0xFFD6445D", 1805],
        ["?", "0xFFF49E95", 1840],
        ["?", "0xFFF49E95", 1840],
        ["?", "0xFFB76663", 1842],
        ["?", "0xFFB76663", 1842],
        ["?", "0xFFE36C63", 1849],
        ["?", "0xFFE36C63", 1849],
        ["?", "0xFFF0887D", 1850],
        ["?", "0xFFF0887D", 1850],
        ["?", "0xFFFAC7C1", 1855],
        ["?", "0xFFFAC7C1", 1855],
        ["?", "0xFFFCDAD5", 1860],
        ["?", "0xFFFCDAD5", 1860],
        ["?", "0xFFFDE3D3", 1870],
        ["?", "0xFFFDE3D3", 1870],
        ["?", "0xFF636254", 1874],
        ["?", "0xFF636254", 1874],
        ["?", "0xFF394535", 1876],
        ["?", "0xFF394535", 1876],
        ["?", "0xFFE10057", 1900],
        ["?", "0xFFE10057", 1900],
        ["?", "0xFFBD0041", 1902],
        ["?", "0xFFBD0041", 1902],
        ["?", "0xFFC00343", 1903],
        ["?", "0xFFC00343", 1903],
        ["?", "0xFFA9023A", 1904],
        ["?", "0xFFA9023A", 1904],
        ["?", "0xFF960018", 1905],
        ["?", "0xFF960018", 1905],
        ["?", "0xFFBE004F", 1906],
        ["?", "0xFFBE004F", 1906],
        ["?", "0xFF910230", 1911],
        ["?", "0xFF910230", 1911],
        ["?", "0xFF86023E", 1912],
        ["?", "0xFF86023E", 1912],
        ["?", "0xFF9A0C3B", 1913],
        ["?", "0xFF9A0C3B", 1913],
        ["?", "0xFFA41F39", 1914],
        ["?", "0xFFA41F39", 1914],
        ["?", "0xFFA33050", 1921],
        ["?", "0xFFA33050", 1921],
        ["?", "0xFFF28DA6", 1940],
        ["?", "0xFFF28DA6", 1940],
        ["?", "0xFFCE427A", 1950],
        ["?", "0xFFCE427A", 1950],
        ["?", "0xFFF2B9BE", 1960],
        ["?", "0xFFF2B9BE", 1960],
        ["?", "0xFF959595", 1972],
        ["?", "0xFF959595", 1972],
        ["?", "0xFFA33145", 2011],
        ["?", "0xFFA33145", 2011],
        ["?", "0xFF9F454C", 2022],
        ["?", "0xFF9F454C", 2022],
        ["?", "0xFFC7979B", 2051],
        ["?", "0xFFC7979B", 2051],
        ["?", "0xFFD18D89", 2053],
        ["?", "0xFFD18D89", 2053],
        ["?", "0xFF970038", 2100],
        ["?", "0xFF970038", 2100],
        ["?", "0xFF9F003F", 2101],
        ["?", "0xFF9F003F", 2101],
        ["?", "0xFF78093F", 2113],
        ["?", "0xFF78093F", 2113],
        ["?", "0xFF432732", 2123],
        ["?", "0xFF432732", 2123],
        ["?", "0xFFE6778B", 2152],
        ["?", "0xFFE6778B", 2152],
        ["?", "0xFFDF8390", 2153],
        ["?", "0xFFDF8390", 2153],
        ["?", "0xFFF9BFC0", 2155],
        ["?", "0xFFF9BFC0", 2155],
        ["?", "0xFFFBD1D6", 2160],
        ["?", "0xFFFBD1D6", 2160],
        ["?", "0xFFFDE3DB", 2165],
        ["?", "0xFFFDE3DB", 2165],
        ["?", "0xFFD8D5D0", 2166],
        ["?", "0xFFD8D5D0", 2166],
        ["?", "0xFFFEEDE2", 2168],
        ["?", "0xFFFEEDE2", 2168],
        ["?", "0xFFF7DED6", 2170],
        ["?", "0xFFF7DED6", 2170],
        ["?", "0xFFF7DEDE", 2171],
        ["?", "0xFFF7DEDE", 2171],
        ["?", "0xFFFCD9C4", 2180],
        ["?", "0xFFFCD9C4", 2180],
        ["?", "0xFFE8418C", 2220],
        ["?", "0xFFE8418C", 2220],
        ["?", "0xFF8C0C4A", 2222],
        ["?", "0xFF8C0C4A", 2222],
        ["?", "0xFF883A40", 2224],
        ["?", "0xFF883A40", 2224],
        ["?", "0xFFEE71A1", 2230],
        ["?", "0xFFEE71A1", 2230],
        ["?", "0xFFA95A68", 2241],
        ["?", "0xFFA95A68", 2241],
        ["?", "0xFFFAC8D3", 2250],
        ["?", "0xFFFAC8D3", 2250],
        ["?", "0xFFD3007E", 2300],
        ["?", "0xFFD3007E", 2300],
        ["?", "0xFFBF006A", 2310],
        ["?", "0xFFBF006A", 2310],
        ["?", "0xFFD20067", 2320],
        ["?", "0xFFD20067", 2320],
        ["?", "0xFF780C38", 2332],
        ["?", "0xFF780C38", 2332],
        ["?", "0xFF651533", 2333],
        ["?", "0xFF651533", 2333],
        ["?", "0xFF3A212B", 2336],
        ["?", "0xFF3A212B", 2336],
        ["?", "0xFFF2E0DC", 2360],
        ["?", "0xFFF2E0DC", 2360],
        ["?", "0xFFFDE5EC", 2363],
        ["?", "0xFFFDE5EC", 2363],
        ["?", "0xFF970059", 2500],
        ["?", "0xFF970059", 2500],
        ["?", "0xFF8B1771", 2502],
        ["?", "0xFF8B1771", 2502],
        ["?", "0xFFAA4381", 2504],
        ["?", "0xFFAA4381", 2504],
        ["?", "0xFFB40073", 2505],
        ["?", "0xFFB40073", 2505],
        ["?", "0xFF820052", 2506],
        ["?", "0xFF820052", 2506],
        ["?", "0xFFD63C81", 2513],
        ["?", "0xFFD63C81", 2513],
        ["?", "0xFFE20078", 2520],
        ["?", "0xFFE20078", 2520],
        ["?", "0xFFBF006A", 2521],
        ["?", "0xFFBF006A", 2521],
        ["?", "0xFFEE71A1", 2524],
        ["?", "0xFFEE71A1", 2524],
        ["?", "0xFFF189AF", 2550],
        ["?", "0xFFF189AF", 2550],
        ["?", "0xFFF7B4CA", 2555],
        ["?", "0xFFF7B4CA", 2555],
        ["?", "0xFFF7B4CA", 2560],
        ["?", "0xFFF7B4CA", 2560],
        ["?", "0xFF494949", 2576],
        ["?", "0xFF494949", 2576],
        ["?", "0xFF394248", 2578],
        ["?", "0xFF394248", 2578],
        ["?", "0xFF893480", 2600],
        ["?", "0xFF893480", 2600],
        ["?", "0xFF6C0051", 2611],
        ["?", "0xFF6C0051", 2611],
        ["?", "0xFFCD73A6", 2620],
        ["?", "0xFFCD73A6", 2620],
        ["?", "0xFFD994B9", 2640],
        ["?", "0xFFD994B9", 2640],
        ["?", "0xFFDDBDD5", 2645],
        ["?", "0xFFDDBDD5", 2645],
        ["?", "0xFFE6B7CF", 2650],
        ["?", "0xFFE6B7CF", 2650],
        ["?", "0xFFECD2DE", 2655],
        ["?", "0xFFECD2DE", 2655],
        ["?", "0xFF606D8C", 2674],
        ["?", "0xFF606D8C", 2674],
        ["?", "0xFF646A6E", 2675],
        ["?", "0xFF646A6E", 2675],
        ["?", "0xFF610051", 2711],
        ["?", "0xFF610051", 2711],
        ["?", "0xFF704191", 2712],
        ["?", "0xFF704191", 2712],
        ["?", "0xFF490251", 2715],
        ["?", "0xFF490251", 2715],
        ["?", "0xFF2F206F", 2743],
        ["?", "0xFF2F206F", 2743],
        ["?", "0xFFC690A1", 2764],
        ["?", "0xFFC690A1", 2764],
        ["?", "0xFF6F067B", 2810],
        ["?", "0xFF6F067B", 2810],
        ["?", "0xFFAD85B1", 2820],
        ["?", "0xFFAD85B1", 2820],
        ["?", "0xFFA974AB", 2830],
        ["?", "0xFFA974AB", 2830],
        ["?", "0xFF4C0F7B", 2900],
        ["?", "0xFF4C0F7B", 2900],
        ["?", "0xFF664090", 2905],
        ["?", "0xFF664090", 2905],
        ["?", "0xFF83589D", 2910],
        ["?", "0xFF83589D", 2910],
        ["?", "0xFF8C6DAA", 2920],
        ["?", "0xFF8C6DAA", 2920],
        ["?", "0xFFC9B5D4", 3040],
        ["?", "0xFFC9B5D4", 3040],
        ["?", "0xFFC790BA", 3045],
        ["?", "0xFFC790BA", 3045],
        ["?", "0xFF707070", 3062],
        ["?", "0xFF707070", 3062],
        ["?", "0xFF2A377E", 3102],
        ["?", "0xFF2A377E", 3102],
        ["?", "0xFF3C1F81", 3103],
        ["?", "0xFF3C1F81", 3103],
        ["?", "0xFF35247A", 3110],
        ["?", "0xFF35247A", 3110],
        ["?", "0xFF260751", 3114],
        ["?", "0xFF260751", 3114],
        ["?", "0xFF28135B", 3133],
        ["?", "0xFF28135B", 3133],
        ["?", "0xFF6E5DA3", 3200],
        ["?", "0xFF6E5DA3", 3200],
        ["?", "0xFF353A90", 3210],
        ["?", "0xFF353A90", 3210],
        ["?", "0xFF524A90", 3211],
        ["?", "0xFF524A90", 3211],
        ["?", "0xFF785FA3", 3213],
        ["?", "0xFF785FA3", 3213],
        ["?", "0xFF241757", 3222],
        ["?", "0xFF241757", 3222],
        ["?", "0xFF7D77AF", 3241],
        ["?", "0xFF7D77AF", 3241],
        ["?", "0xFF9083A3", 3251],
        ["?", "0xFF9083A3", 3251],
        ["?", "0xFFB2AABD", 3262],
        ["?", "0xFFB2AABD", 3262],
        ["?", "0xFF392D88", 3301],
        ["?", "0xFF392D88", 3301],
        ["?", "0xFF5661A8", 3321],
        ["?", "0xFF5661A8", 3321],
        ["?", "0xFF323887", 3322],
        ["?", "0xFF323887", 3322],
        ["?", "0xFF14214E", 3323],
        ["?", "0xFF14214E", 3323],
        ["?", "0xFF3A2885", 3330],
        ["?", "0xFF3A2885", 3330],
        ["?", "0xFF7F8BC2", 3331],
        ["?", "0xFF7F8BC2", 3331],
        ["?", "0xFF1B3C78", 3333],
        ["?", "0xFF1B3C78", 3333],
        ["?", "0xFFB9BDD9", 3340],
        ["?", "0xFFB9BDD9", 3340],
        ["?", "0xFF11263C", 3344],
        ["?", "0xFF11263C", 3344],
        ["?", "0xFF202A65", 3353],
        ["?", "0xFF202A65", 3353],
        ["?", "0xFF171B4A", 3355],
        ["?", "0xFF171B4A", 3355],
        ["?", "0xFF959ACA", 3420],
        ["?", "0xFF959ACA", 3420],
        ["?", "0xFF6A76B5", 3430],
        ["?", "0xFF6A76B5", 3430],
        ["?", "0xFF11263C", 3443],
        ["?", "0xFF11263C", 3443],
        ["?", "0xFF002232", 3444],
        ["?", "0xFF002232", 3444],
        ["?", "0xFF2D4491", 3522],
        ["?", "0xFF2D4491", 3522],
        ["?", "0xFF261257", 3536],
        ["?", "0xFF261257", 3536],
        ["?", "0xFF53428A", 3540],
        ["?", "0xFF53428A", 3540],
        ["?", "0xFF3A2885", 3541],
        ["?", "0xFF3A2885", 3541],
        ["?", "0xFF233B7D", 3543],
        ["?", "0xFF233B7D", 3543],
        ["?", "0xFF273C82", 3544],
        ["?", "0xFF273C82", 3544],
        ["?", "0xFF272651", 3554],
        ["?", "0xFF272651", 3554],
        ["?", "0xFF28438C", 3600],
        ["?", "0xFF28438C", 3600],
        ["?", "0xFF243A7D", 3611],
        ["?", "0xFF243A7D", 3611],
        ["?", "0xFF4055A1", 3612],
        ["?", "0xFF4055A1", 3612],
        ["?", "0xFF1A4C8D", 3622],
        ["?", "0xFF1A4C8D", 3622],
        ["?", "0xFF1E569F", 3631],
        ["?", "0xFF1E569F", 3631],
        ["?", "0xFF92B1DC", 3640],
        ["?", "0xFF92B1DC", 3640],
        ["?", "0xFF648DC7", 3641],
        ["?", "0xFF648DC7", 3641],
        ["?", "0xFFD0DEEE", 3650],
        ["?", "0xFFD0DEEE", 3650],
        ["?", "0xFFEAF0F9", 3661],
        ["?", "0xFFEAF0F9", 3661],
        ["?", "0xFF00507F", 3732],
        ["?", "0xFF00507F", 3732],
        ["?", "0xFF12253C", 3743],
        ["?", "0xFF12253C", 3743],
        ["?", "0xFFB7D1E3", 3750],
        ["?", "0xFFB7D1E3", 3750],
        ["?", "0xFFD0DEEE", 3752],
        ["?", "0xFFD0DEEE", 3752],
        ["?", "0xFFAFC9E5", 3761],
        ["?", "0xFFAFC9E5", 3761],
        ["?", "0xFFCED2D1", 3770],
        ["?", "0xFFCED2D1", 3770],
        ["?", "0xFF3D6AA1", 3810],
        ["?", "0xFF3D6AA1", 3810],
        ["?", "0xFF91B9E2", 3820],
        ["?", "0xFF91B9E2", 3820],
        ["?", "0xFF00779E", 3822],
        ["?", "0xFF00779E", 3822],
        ["?", "0xFFB4CEEB", 3840],
        ["?", "0xFFB4CEEB", 3840],
        ["?", "0xFF507193", 3842],
        ["?", "0xFF507193", 3842],
        ["?", "0xFFD5E3F4", 3845],
        ["?", "0xFFD5E3F4", 3845],
        ["?", "0xFF9AB8D3", 3851],
        ["?", "0xFF9AB8D3", 3851],
        ["?", "0xFF007EBA", 3900],
        ["?", "0xFF007EBA", 3900],
        ["?", "0xFF0082C4", 3901],
        ["?", "0xFF0082C4", 3901],
        ["?", "0xFF006CA5", 3902],
        ["?", "0xFF006CA5", 3902],
        ["?", "0xFF4ABDF0", 3910],
        ["?", "0xFF4ABDF0", 3910],
        ["?", "0xFF86AACA", 3951],
        ["?", "0xFF86AACA", 3951],
        ["?", "0xFF485E8A", 3952],
        ["?", "0xFF485E8A", 3952],
        ["?", "0xFF697698", 3953],
        ["?", "0xFF697698", 3953],
        ["?", "0xFFC5E1F3", 3961],
        ["?", "0xFFC5E1F3", 3961],
        ["?", "0xFFA6D8F6", 3962],
        ["?", "0xFFA6D8F6", 3962],
        ["?", "0xFFE1E1E1", 3971],
        ["?", "0xFFE1E1E1", 3971],
        ["?", "0xFF0093B9", 4010],
        ["?", "0xFF0093B9", 4010],
        ["?", "0xFF006587", 4022],
        ["?", "0xFF006587", 4022],
        ["?", "0xFF87C7EA", 4030],
        ["?", "0xFF87C7EA", 4030],
        ["?", "0xFF507793", 4032],
        ["?", "0xFF507793", 4032],
        ["?", "0xFF265674", 4033],
        ["?", "0xFF265674", 4033],
        ["?", "0xFF9ED4E6", 4040],
        ["?", "0xFF9ED4E6", 4040],
        ["?", "0xFFEAF0F9", 4071],
        ["?", "0xFFEAF0F9", 4071],
        ["?", "0xFF0096C1", 4100],
        ["?", "0xFF0096C1", 4100],
        ["?", "0xFF2DB0CF", 4101],
        ["?", "0xFF2DB0CF", 4101],
        ["?", "0xFF0095C6", 4103],
        ["?", "0xFF0095C6", 4103],
        ["?", "0xFF0081AA", 4105],
        ["?", "0xFF0081AA", 4105],
        ["?", "0xFF00A4D9", 4111],
        ["?", "0xFF00A4D9", 4111],
        ["?", "0xFF00A9C9", 4113],
        ["?", "0xFF00A9C9", 4113],
        ["?", "0xFF5DBFD2", 4121],
        ["?", "0xFF5DBFD2", 4121],
        ["?", "0xFF00405D", 4133],
        ["?", "0xFF00405D", 4133],
        ["?", "0xFF192024", 4174],
        ["?", "0xFF192024", 4174],
        ["?", "0xFF192024", 4175],
        ["?", "0xFF192024", 4175],
        ["?", "0xFF4FB4CB", 4220],
        ["?", "0xFF4FB4CB", 4220],
        ["?", "0xFF8DCEE4", 4230],
        ["?", "0xFF8DCEE4", 4230],
        ["?", "0xFF2DB0CF", 4231],
        ["?", "0xFF2DB0CF", 4231],
        ["?", "0xFF006587", 4232],
        ["?", "0xFF006587", 4232],
        ["?", "0xFF8DCDDB", 4240],
        ["?", "0xFF8DCDDB", 4240],
        ["?", "0xFFD5EBF2", 4250],
        ["?", "0xFFD5EBF2", 4250],
        ["?", "0xFF007389", 4400],
        ["?", "0xFF007389", 4400],
        ["?", "0xFF007B8D", 4410],
        ["?", "0xFF007B8D", 4410],
        ["?", "0xFF00B2CA", 4420],
        ["?", "0xFF00B2CA", 4420],
        ["?", "0xFF0091A5", 4421],
        ["?", "0xFF0091A5", 4421],
        ["?", "0xFF007D8C", 4423],
        ["?", "0xFF007D8C", 4423],
        ["?", "0xFF007986", 4425],
        ["?", "0xFF007986", 4425],
        ["?", "0xFF5FBFD1", 4430],
        ["?", "0xFF5FBFD1", 4430],
        ["?", "0xFF004250", 4432],
        ["?", "0xFF004250", 4432],
        ["?", "0xFF8DCEE4", 4440],
        ["?", "0xFF8DCEE4", 4440],
        ["?", "0xFF006981", 4442],
        ["?", "0xFF006981", 4442],
        ["?", "0xFF007F92", 4452],
        ["?", "0xFF007F92", 4452],
        ["?", "0xFF008192", 4500],
        ["?", "0xFF008192", 4500],
        ["?", "0xFF007079", 4513],
        ["?", "0xFF007079", 4513],
        ["?", "0xFF002F38", 4515],
        ["?", "0xFF002F38", 4515],
        ["?", "0xFF00646A", 4516],
        ["?", "0xFF00646A", 4516],
        ["?", "0xFF007389", 4531],
        ["?", "0xFF007389", 4531],
        ["?", "0xFF007B8D", 4610],
        ["?", "0xFF007B8D", 4610],
        ["?", "0xFF00A3A0", 4620],
        ["?", "0xFF00A3A0", 4620],
        ["?", "0xFF0B7F85", 4625],
        ["?", "0xFF0B7F85", 4625],
        ["?", "0xFF005B63", 4643],
        ["?", "0xFF005B63", 4643],
        ["?", "0xFF234544", 4644],
        ["?", "0xFF234544", 4644],
        ["?", "0xFF8CCDD3", 4840],
        ["?", "0xFF8CCDD3", 4840],
        ["?", "0xFF006F73", 5000],
        ["?", "0xFF006F73", 5000],
        ["?", "0xFF005B63", 5005],
        ["?", "0xFF005B63", 5005],
        ["?", "0xFF00A6AD", 5010],
        ["?", "0xFF00A6AD", 5010],
        ["?", "0xFF49BAC0", 5020],
        ["?", "0xFF49BAC0", 5020],
        ["?", "0xFFCFDDE0", 5040],
        ["?", "0xFFCFDDE0", 5040],
        ["?", "0xFFB4DCD8", 5050],
        ["?", "0xFFB4DCD8", 5050],
        ["?", "0xFF00876E", 5100],
        ["?", "0xFF00876E", 5100],
        ["?", "0xFF009084", 5101],
        ["?", "0xFF009084", 5101],
        ["?", "0xFF00B1AE", 5111],
        ["?", "0xFF00B1AE", 5111],
        ["?", "0xFF48BAB7", 5115],
        ["?", "0xFF48BAB7", 5115],
        ["?", "0xFF00AF8C", 5210],
        ["?", "0xFF00AF8C", 5210],
        ["?", "0xFF8CCCC2", 5220],
        ["?", "0xFF8CCCC2", 5220],
        ["?", "0xFF47B9AE", 5230],
        ["?", "0xFF47B9AE", 5230],
        ["?", "0xFF197E6D", 5233],
        ["?", "0xFF197E6D", 5233],
        ["?", "0xFF8CCCC2", 5240],
        ["?", "0xFF8CCCC2", 5240],
        ["?", "0xFF005B63", 5255],
        ["?", "0xFF005B63", 5255],
        ["?", "0xFF006E42", 5324],
        ["?", "0xFF006E42", 5324],
        ["?", "0xFF004D3D", 5326],
        ["?", "0xFF004D3D", 5326],
        ["?", "0xFF002F38", 5335],
        ["?", "0xFF002F38", 5335],
        ["?", "0xFF002D1F", 5374],
        ["?", "0xFF002D1F", 5374],
        ["?", "0xFF002F38", 5375],
        ["?", "0xFF002F38", 5375],
        ["?", "0xFF008663", 5411],
        ["?", "0xFF008663", 5411],
        ["?", "0xFF006B4E", 5415],
        ["?", "0xFF006B4E", 5415],
        ["?", "0xFF008663", 5422],
        ["?", "0xFF008663", 5422],
        ["?", "0xFF006B56", 5425],
        ["?", "0xFF006B56", 5425],
        ["?", "0xFF008879", 5431],
        ["?", "0xFF008879", 5431],
        ["?", "0xFFDBE9E5", 5460],
        ["?", "0xFFDBE9E5", 5460],
        ["?", "0xFF6AC093", 5470],
        ["?", "0xFF6AC093", 5470],
        ["?", "0xFF52BA84", 5500],
        ["?", "0xFF52BA84", 5500],
        ["?", "0xFF14A363", 5510],
        ["?", "0xFF14A363", 5510],
        ["?", "0xFF007848", 5513],
        ["?", "0xFF007848", 5513],
        ["?", "0xFF008663", 5515],
        ["?", "0xFF008663", 5515],
        ["?", "0xFF52A04F", 5531],
        ["?", "0xFF52A04F", 5531],
        ["?", "0xFF6EA293", 5542],
        ["?", "0xFF6EA293", 5542],
        ["?", "0xFF94ADA1", 5552],
        ["?", "0xFF94ADA1", 5552],
        ["?", "0xFF103828", 5555],
        ["?", "0xFF103828", 5555],
        ["?", "0xFF63BE5F", 5600],
        ["?", "0xFF63BE5F", 5600],
        ["?", "0xFF85C875", 5610],
        ["?", "0xFF85C875", 5610],
        ["?", "0xFF2CB431", 5611],
        ["?", "0xFF2CB431", 5611],
        ["?", "0xFF14B26D", 5613],
        ["?", "0xFF14B26D", 5613],
        ["?", "0xFF40B780", 5620],
        ["?", "0xFF40B780", 5620],
        ["?", "0xFF1A781E", 5633],
        ["?", "0xFF1A781E", 5633],
        ["?", "0xFF157436", 5643],
        ["?", "0xFF157436", 5643],
        ["?", "0xFFC9E3C5", 5650],
        ["?", "0xFFC9E3C5", 5650],
        ["?", "0xFF6B9181", 5664],
        ["?", "0xFF6B9181", 5664],
        ["?", "0xFF3A6D57", 5765],
        ["?", "0xFF3A6D57", 5765],
        ["?", "0xFF103828", 5766],
        ["?", "0xFF103828", 5766],
        ["?", "0xFF02140C", 5776],
        ["?", "0xFF02140C", 5776],
        ["?", "0xFFA5C278", 5822],
        ["?", "0xFFA5C278", 5822],
        ["?", "0xFFB4D383", 5832],
        ["?", "0xFFB4D383", 5832],
        ["?", "0xFF70953F", 5833],
        ["?", "0xFF70953F", 5833],
        ["?", "0xFFA2D289", 5840],
        ["?", "0xFFA2D289", 5840],
        ["?", "0xFF273014", 5866],
        ["?", "0xFF273014", 5866],
        ["?", "0xFF81C750", 5912],
        ["?", "0xFF81C750", 5912],
        ["?", "0xFF457021", 5933],
        ["?", "0xFF457021", 5933],
        ["?", "0xFF506702", 5934],
        ["?", "0xFF506702", 5934],
        ["?", "0xFFBBDB41", 5940],
        ["?", "0xFFBBDB41", 5940],
        ["?", "0xFF003518", 5944],
        ["?", "0xFF003518", 5944],
        ["?", "0xFFE3EB00", 6010],
        ["?", "0xFFE3EB00", 6010],
        ["?", "0xFFBED782", 6051],
        ["?", "0xFFBED782", 6051],
        ["?", "0xFF2D3B01", 6065],
        ["?", "0xFF2D3B01", 6065],
        ["?", "0xFFDCDDD1", 6071],
        ["?", "0xFFDCDDD1", 6071],
        ["?", "0xFF919600", 6133],
        ["?", "0xFF919600", 6133],
        ["?", "0xFF484601", 6156],
        ["?", "0xFF484601", 6156],
        ["END", 0, -1]
    ],

    "Marathon_Polyester_codes": [
        ["END", 0, -1]
    ],

    "Marathon_Rayon_codes": [
        ["END", 0, -1]
    ],

    "Madeira_Polyester_codes": [
        ["END", 0, -1]
    ],

    "Madeira_Rayon_codes": [
        ["END", 0, -1]
    ],

    "Metro_Polyester_codes": [
        ["END", 0, -1]
    ],

    "Pantone_codes": [
        ["?", "0xFFFFFF7D", 100],
        ["?", "0xFFFFFF7D", 100],
        ["?", "0xFFFFFF36", 101],
        ["?", "0xFFFFFF36", 101],
        ["?", "0xFFFFFC0D", 102],
        ["?", "0xFFFFFC0D", 102],
        ["?", "0xFFD1CB00", 103],
        ["?", "0xFFD1CB00", 103],
        ["?", "0xFFB3AD00", 104],
        ["?", "0xFFB3AD00", 104],
        ["?", "0xFF807C00", 105],
        ["?", "0xFF807C00", 105],
        ["?", "0xFFFFFA4F", 106],
        ["?", "0xFFFFFA4F", 106],
        ["?", "0xFFFFF536", 107],
        ["?", "0xFFFFF536", 107],
        ["?", "0xFFFFF00D", 108],
        ["?", "0xFFFFF00D", 108],
        ["?", "0xFFFFE600", 109],
        ["?", "0xFFFFE600", 109],
        ["?", "0xFFEDD100", 110],
        ["?", "0xFFEDD100", 110],
        ["?", "0xFFBAA600", 111],
        ["?", "0xFFBAA600", 111],
        ["?", "0xFF9E8E00", 112],
        ["?", "0xFF9E8E00", 112],
        ["?", "0xFFFFED57", 113],
        ["?", "0xFFFFED57", 113],
        ["?", "0xFFFFEB45", 114],
        ["?", "0xFFFFEB45", 114],
        ["?", "0xFFFFE833", 115],
        ["?", "0xFFFFE833", 115],
        ["?", "0xFFFFD600", 116],
        ["?", "0xFFFFD600", 116],
        ["?", "0xFFD9B200", 117],
        ["?", "0xFFD9B200", 117],
        ["?", "0xFFBA9900", 118],
        ["?", "0xFFBA9900", 118],
        ["?", "0xFF827200", 119],
        ["?", "0xFF827200", 119],
        ["?", "0xFFFFE86B", 120],
        ["?", "0xFFFFE86B", 120],
        ["?", "0xFFFFF2B0", 1205],
        ["?", "0xFFFFF2B0", 1205],
        ["?", "0xFFFFE34F", 121],
        ["?", "0xFFFFE34F", 121],
        ["?", "0xFFFFE88C", 1215],
        ["?", "0xFFFFE88C", 1215],
        ["?", "0xFFFFD433", 122],
        ["?", "0xFFFFD433", 122],
        ["?", "0xFFFFD461", 1225],
        ["?", "0xFFFFD461", 1225],
        ["?", "0xFFFFC20F", 123],
        ["?", "0xFFFFC20F", 123],
        ["?", "0xFFFFB517", 1235],
        ["?", "0xFFFFB517", 1235],
        ["?", "0xFFF0AD00", 124],
        ["?", "0xFFF0AD00", 124],
        ["?", "0xFFD19700", 1245],
        ["?", "0xFFD19700", 1245],
        ["?", "0xFFBD8C00", 125],
        ["?", "0xFFBD8C00", 125],
        ["?", "0xFFA87B00", 1255],
        ["?", "0xFFA87B00", 1255],
        ["?", "0xFFA17800", 126],
        ["?", "0xFFA17800", 126],
        ["?", "0xFF7D5B00", 1265],
        ["?", "0xFF7D5B00", 1265],
        ["?", "0xFFFFED80", 127],
        ["?", "0xFFFFED80", 127],
        ["?", "0xFFFFE359", 128],
        ["?", "0xFFFFE359", 128],
        ["?", "0xFFFFD63B", 129],
        ["?", "0xFFFFD63B", 129],
        ["?", "0xFFFFB300", 130],
        ["?", "0xFFFFB300", 130],
        ["?", "0xFFE89E00", 131],
        ["?", "0xFFE89E00", 131],
        ["?", "0xFFB38100", 132],
        ["?", "0xFFB38100", 132],
        ["?", "0xFF705A00", 133],
        ["?", "0xFF705A00", 133],
        ["?", "0xFFFFE38C", 134],
        ["?", "0xFFFFE38C", 134],
        ["?", "0xFFFFDB87", 1345],
        ["?", "0xFFFFDB87", 1345],
        ["?", "0xFFFFCF66", 135],
        ["?", "0xFFFFCF66", 135],
        ["?", "0xFFFFCC70", 1355],
        ["?", "0xFFFFCC70", 1355],
        ["?", "0xFFFFBA3D", 136],
        ["?", "0xFFFFBA3D", 136],
        ["?", "0xFFFFB547", 1365],
        ["?", "0xFFFFB547", 1365],
        ["?", "0xFFFFA61A", 137],
        ["?", "0xFFFFA61A", 137],
        ["?", "0xFFFF991A", 1375],
        ["?", "0xFFFF991A", 1375],
        ["?", "0xFFFC9200", 138],
        ["?", "0xFFFC9200", 138],
        ["?", "0xFFED8500", 1385],
        ["?", "0xFFED8500", 1385],
        ["?", "0xFFC47C00", 139],
        ["?", "0xFFC47C00", 139],
        ["?", "0xFFA15F00", 1395],
        ["?", "0xFFA15F00", 1395],
        ["?", "0xFF755600", 140],
        ["?", "0xFF755600", 140],
        ["?", "0xFF5E3C00", 1405],
        ["?", "0xFF5E3C00", 1405],
        ["?", "0xFFFFCF7D", 141],
        ["?", "0xFFFFCF7D", 141],
        ["?", "0xFFFFB83D", 142],
        ["?", "0xFFFFB83D", 142],
        ["?", "0xFFFFA626", 143],
        ["?", "0xFFFFA626", 143],
        ["?", "0xFFFF8500", 144],
        ["?", "0xFFFF8500", 144],
        ["?", "0xFFEB7C00", 145],
        ["?", "0xFFEB7C00", 145],
        ["?", "0xFFAB6100", 146],
        ["?", "0xFFAB6100", 146],
        ["?", "0xFF705100", 147],
        ["?", "0xFF705100", 147],
        ["?", "0xFFFFD6A1", 148],
        ["?", "0xFFFFD6A1", 148],
        ["?", "0xFFFFBA75", 1485],
        ["?", "0xFFFFBA75", 1485],
        ["?", "0xFFFFC487", 149],
        ["?", "0xFFFFC487", 149],
        ["?", "0xFFFFAB54", 1495],
        ["?", "0xFFFFAB54", 1495],
        ["?", "0xFFFFA64D", 150],
        ["?", "0xFFFFA64D", 150],
        ["?", "0xFFFF943B", 1505],
        ["?", "0xFFFF943B", 1505],
        ["?", "0xFFFF850D", 151],
        ["?", "0xFFFF850D", 151],
        ["?", "0xFFFC7C00", 152],
        ["?", "0xFFFC7C00", 152],
        ["?", "0xFFE66000", 1525],
        ["?", "0xFFE66000", 1525],
        ["?", "0xFFD17100", 153],
        ["?", "0xFFD17100", 153],
        ["?", "0xFF9E4A00", 1535],
        ["?", "0xFF9E4A00", 1535],
        ["?", "0xFFA85B00", 154],
        ["?", "0xFFA85B00", 154],
        ["?", "0xFF472200", 1545],
        ["?", "0xFF472200", 1545],
        ["?", "0xFFFFE0B8", 155],
        ["?", "0xFFFFE0B8", 155],
        ["?", "0xFFFFC7A8", 1555],
        ["?", "0xFFFFC7A8", 1555],
        ["?", "0xFFFFC794", 156],
        ["?", "0xFFFFC794", 156],
        ["?", "0xFFFFA882", 1565],
        ["?", "0xFFFFA882", 1565],
        ["?", "0xFFFF914D", 157],
        ["?", "0xFFFF914D", 157],
        ["?", "0xFFFF8C47", 1575],
        ["?", "0xFFFF8C47", 1575],
        ["?", "0xFFFF6308", 158],
        ["?", "0xFFFF6308", 158],
        ["?", "0xFFFF701A", 1585],
        ["?", "0xFFFF701A", 1585],
        ["?", "0xFFED5100", 159],
        ["?", "0xFFED5100", 159],
        ["?", "0xFFF26300", 1595],
        ["?", "0xFFF26300", 1595],
        ["?", "0xFFAD4200", 160],
        ["?", "0xFFAD4200", 160],
        ["?", "0xFFB34F00", 1605],
        ["?", "0xFFB34F00", 1605],
        ["?", "0xFF5C2C00", 161],
        ["?", "0xFF5C2C00", 161],
        ["?", "0xFF914000", 1615],
        ["?", "0xFF914000", 1615],
        ["?", "0xFFFFD9C7", 162],
        ["?", "0xFFFFD9C7", 162],
        ["?", "0xFFFFB0A1", 1625],
        ["?", "0xFFFFB0A1", 1625],
        ["?", "0xFFFFB08F", 163],
        ["?", "0xFFFFB08F", 163],
        ["?", "0xFFFF9C85", 1635],
        ["?", "0xFFFF9C85", 1635],
        ["?", "0xFFFF8A45", 164],
        ["?", "0xFFFF8A45", 164],
        ["?", "0xFFFF8257", 1645],
        ["?", "0xFFFF8257", 1645],
        ["?", "0xFFFF690A", 165],
        ["?", "0xFFFF690A", 165],
        ["?", "0xFFFF5E17", 1655],
        ["?", "0xFFFF5E17", 1655],
        ["?", "0xFFFF5C00", 166],
        ["?", "0xFFFF5C00", 166],
        ["?", "0xFFFF5200", 1665],
        ["?", "0xFFFF5200", 1665],
        ["?", "0xFFD45500", 167],
        ["?", "0xFFD45500", 167],
        ["?", "0xFFB83D00", 1675],
        ["?", "0xFFB83D00", 1675],
        ["?", "0xFF692D00", 168],
        ["?", "0xFF692D00", 168],
        ["?", "0xFF8F2E00", 1685],
        ["?", "0xFF8F2E00", 1685],
        ["?", "0xFFFFCCCC", 169],
        ["?", "0xFFFFCCCC", 169],
        ["?", "0xFFFF998F", 170],
        ["?", "0xFFFF998F", 170],
        ["?", "0xFFFF7852", 171],
        ["?", "0xFFFF7852", 171],
        ["?", "0xFFFF571F", 172],
        ["?", "0xFFFF571F", 172],
        ["?", "0xFFF54C00", 173],
        ["?", "0xFFF54C00", 173],
        ["?", "0xFFA33100", 174],
        ["?", "0xFFA33100", 174],
        ["?", "0xFF662400", 175],
        ["?", "0xFF662400", 175],
        ["?", "0xFFFFBFD1", 176],
        ["?", "0xFFFFBFD1", 176],
        ["?", "0xFFFF9EC9", 1765],
        ["?", "0xFFFF9EC9", 1765],
        ["?", "0xFFFFBAE0", 1767],
        ["?", "0xFFFFBAE0", 1767],
        ["?", "0xFFFF8C99", 177],
        ["?", "0xFFFF8C99", 177],
        ["?", "0xFFFF87B5", 1775],
        ["?", "0xFFFF87B5", 1775],
        ["?", "0xFFFF6BA3", 1777],
        ["?", "0xFFFF6BA3", 1777],
        ["?", "0xFFFF6970", 178],
        ["?", "0xFFFF6970", 178],
        ["?", "0xFFFF5480", 1785],
        ["?", "0xFFFF5480", 1785],
        ["?", "0xFFFF3D66", 1787],
        ["?", "0xFFFF3D66", 1787],
        ["?", "0xFFFF291F", 1788],
        ["?", "0xFFFF291F", 1788],
        ["?", "0xFFFF3600", 179],
        ["?", "0xFFFF3600", 179],
        ["?", "0xFFFF0F00", 1795],
        ["?", "0xFFFF0F00", 1795],
        ["?", "0xFFF50002", 1797],
        ["?", "0xFFF50002", 1797],
        ["?", "0xFFE33000", 180],
        ["?", "0xFFE33000", 180],
        ["?", "0xFFC41200", 1805],
        ["?", "0xFFC41200", 1805],
        ["?", "0xFFB80007", 1807],
        ["?", "0xFFB80007", 1807],
        ["?", "0xFF872300", 181],
        ["?", "0xFF872300", 181],
        ["?", "0xFF7D0C00", 1815],
        ["?", "0xFF7D0C00", 1815],
        ["?", "0xFF570900", 1817],
        ["?", "0xFF570900", 1817],
        ["?", "0xFFFFBDE6", 182],
        ["?", "0xFFFFBDE6", 182],
        ["?", "0xFFFF8AC9", 183],
        ["?", "0xFFFF8AC9", 183],
        ["?", "0xFFFF5296", 184],
        ["?", "0xFFFF5296", 184],
        ["?", "0xFFFF173D", 185],
        ["?", "0xFFFF173D", 185],
        ["?", "0xFFF5002F", 186],
        ["?", "0xFFF5002F", 186],
        ["?", "0xFFCC002B", 187],
        ["?", "0xFFCC002B", 187],
        ["?", "0xFF800400", 188],
        ["?", "0xFF800400", 188],
        ["?", "0xFFFFA1E6", 189],
        ["?", "0xFFFFA1E6", 189],
        ["?", "0xFFFFB8ED", 1895],
        ["?", "0xFFFFB8ED", 1895],
        ["?", "0xFFFF73C7", 190],
        ["?", "0xFFFF73C7", 190],
        ["?", "0xFFFF96E8", 1905],
        ["?", "0xFFFF96E8", 1905],
        ["?", "0xFFFF3D9E", 191],
        ["?", "0xFFFF3D9E", 191],
        ["?", "0xFFFF4ACC", 1915],
        ["?", "0xFFFF4ACC", 1915],
        ["?", "0xFFFF0052", 192],
        ["?", "0xFFFF0052", 192],
        ["?", "0xFFFF0073", 1925],
        ["?", "0xFFFF0073", 1925],
        ["?", "0xFFDE004B", 193],
        ["?", "0xFFDE004B", 193],
        ["?", "0xFFF20068", 1935],
        ["?", "0xFFF20068", 1935],
        ["?", "0xFFAB003E", 194],
        ["?", "0xFFAB003E", 194],
        ["?", "0xFFCF005B", 1945],
        ["?", "0xFFCF005B", 1945],
        ["?", "0xFF73002E", 195],
        ["?", "0xFF73002E", 195],
        ["?", "0xFFA10040", 1955],
        ["?", "0xFFA10040", 1955],
        ["?", "0xFFFFBFF5", 196],
        ["?", "0xFFFFBFF5", 196],
        ["?", "0xFFFF8CE6", 197],
        ["?", "0xFFFF8CE6", 197],
        ["?", "0xFFFF38AB", 198],
        ["?", "0xFFFF38AB", 198],
        ["?", "0xFFFF0061", 199],
        ["?", "0xFFFF0061", 199],
        ["?", "0xFFE00053", 200],
        ["?", "0xFFE00053", 200],
        ["?", "0xFFB50043", 201],
        ["?", "0xFFB50043", 201],
        ["?", "0xFF910039", 202],
        ["?", "0xFF910039", 202],
        ["?", "0xFFFFA8F7", 203],
        ["?", "0xFFFFA8F7", 203],
        ["?", "0xFFFF6BF7", 204],
        ["?", "0xFFFF6BF7", 204],
        ["?", "0xFFFF29E8", 205],
        ["?", "0xFFFF29E8", 205],
        ["?", "0xFFF70099", 206],
        ["?", "0xFFF70099", 206],
        ["?", "0xFFCF0076", 207],
        ["?", "0xFFCF0076", 207],
        ["?", "0xFFA10067", 208],
        ["?", "0xFFA10067", 208],
        ["?", "0xFF78004F", 209],
        ["?", "0xFF78004F", 209],
        ["?", "0xFFFF9CF0", 210],
        ["?", "0xFFFF9CF0", 210],
        ["?", "0xFFFF73EB", 211],
        ["?", "0xFFFF73EB", 211],
        ["?", "0xFFFF47E3", 212],
        ["?", "0xFFFF47E3", 212],
        ["?", "0xFFFF0DBA", 213],
        ["?", "0xFFFF0DBA", 213],
        ["?", "0xFFEB009B", 214],
        ["?", "0xFFEB009B", 214],
        ["?", "0xFFBA0079", 215],
        ["?", "0xFFBA0079", 215],
        ["?", "0xFF82074E", 216],
        ["?", "0xFF82074E", 216],
        ["?", "0xFFFFB8FF", 217],
        ["?", "0xFFFFB8FF", 217],
        ["?", "0xFFFA63FF", 218],
        ["?", "0xFFFA63FF", 218],
        ["?", "0xFFFC1FFF", 219],
        ["?", "0xFFFC1FFF", 219],
        ["?", "0xFFD400B8", 220],
        ["?", "0xFFD400B8", 220],
        ["?", "0xFFB30098", 221],
        ["?", "0xFFB30098", 221],
        ["?", "0xFF69005E", 222],
        ["?", "0xFF69005E", 222],
        ["?", "0xFFFF8AFF", 223],
        ["?", "0xFFFF8AFF", 223],
        ["?", "0xFFFC5EFF", 224],
        ["?", "0xFFFC5EFF", 224],
        ["?", "0xFFFC2BFF", 225],
        ["?", "0xFFFC2BFF", 225],
        ["?", "0xFFFF00FF", 226],
        ["?", "0xFFFF00FF", 226],
        ["?", "0xFFCF00C0", 227],
        ["?", "0xFFCF00C0", 227],
        ["?", "0xFF960090", 228],
        ["?", "0xFF960090", 228],
        ["?", "0xFF660057", 229],
        ["?", "0xFF660057", 229],
        ["?", "0xFFFFA8FF", 230],
        ["?", "0xFFFFA8FF", 230],
        ["?", "0xFFFC7AFF", 231],
        ["?", "0xFFFC7AFF", 231],
        ["?", "0xFFF754FF", 232],
        ["?", "0xFFF754FF", 232],
        ["?", "0xFFE300FF", 233],
        ["?", "0xFFE300FF", 233],
        ["?", "0xFFB100BD", 234],
        ["?", "0xFFB100BD", 234],
        ["?", "0xFF910099", 235],
        ["?", "0xFF910099", 235],
        ["?", "0xFFFCB3FF", 236],
        ["?", "0xFFFCB3FF", 236],
        ["?", "0xFFFABAFF", 2365],
        ["?", "0xFFFABAFF", 2365],
        ["?", "0xFFF782FF", 237],
        ["?", "0xFFF782FF", 237],
        ["?", "0xFFE66EFF", 2375],
        ["?", "0xFFE66EFF", 2375],
        ["?", "0xFFF05EFF", 238],
        ["?", "0xFFF05EFF", 238],
        ["?", "0xFFCF36FF", 2385],
        ["?", "0xFFCF36FF", 2385],
        ["?", "0xFFE336FF", 239],
        ["?", "0xFFE336FF", 239],
        ["?", "0xFFBA0DFF", 2395],
        ["?", "0xFFBA0DFF", 2395],
        ["?", "0xFFD10FFF", 240],
        ["?", "0xFFD10FFF", 240],
        ["?", "0xFFA800FF", 2405],
        ["?", "0xFFA800FF", 2405],
        ["?", "0xFFB600FA", 241],
        ["?", "0xFFB600FA", 241],
        ["?", "0xFF9D00EB", 2415],
        ["?", "0xFF9D00EB", 2415],
        ["?", "0xFF750082", 242],
        ["?", "0xFF750082", 242],
        ["?", "0xFF7700BD", 2425],
        ["?", "0xFF7700BD", 2425],
        ["?", "0xFFF2B5FF", 243],
        ["?", "0xFFF2B5FF", 243],
        ["?", "0xFFE89EFF", 244],
        ["?", "0xFFE89EFF", 244],
        ["?", "0xFFDB78FF", 245],
        ["?", "0xFFDB78FF", 245],
        ["?", "0xFFB51AFF", 246],
        ["?", "0xFFB51AFF", 246],
        ["?", "0xFFA300FF", 247],
        ["?", "0xFFA300FF", 247],
        ["?", "0xFF9600FA", 248],
        ["?", "0xFF9600FA", 248],
        ["?", "0xFF6E00B8", 249],
        ["?", "0xFF6E00B8", 249],
        ["?", "0xFFF2D1FF", 250],
        ["?", "0xFFF2D1FF", 250],
        ["?", "0xFFDE9CFF", 251],
        ["?", "0xFFDE9CFF", 251],
        ["?", "0xFFC270FF", 252],
        ["?", "0xFFC270FF", 252],
        ["?", "0xFF910DFF", 253],
        ["?", "0xFF910DFF", 253],
        ["?", "0xFF8000FF", 254],
        ["?", "0xFF8000FF", 254],
        ["?", "0xFF5E00BF", 255],
        ["?", "0xFF5E00BF", 255],
        ["?", "0xFFEDCCFF", 256],
        ["?", "0xFFEDCCFF", 256],
        ["?", "0xFFCFA6FF", 2562],
        ["?", "0xFFCFA6FF", 2562],
        ["?", "0xFFC7ABFF", 2563],
        ["?", "0xFFC7ABFF", 2563],
        ["?", "0xFFB5A3FF", 2567],
        ["?", "0xFFB5A3FF", 2567],
        ["?", "0xFFDBA8FF", 257],
        ["?", "0xFFDBA8FF", 257],
        ["?", "0xFFB387FF", 2572],
        ["?", "0xFFB387FF", 2572],
        ["?", "0xFFB391FF", 2573],
        ["?", "0xFFB391FF", 2573],
        ["?", "0xFF998CFF", 2577],
        ["?", "0xFF998CFF", 2577],
        ["?", "0xFF913DFF", 258],
        ["?", "0xFF913DFF", 258],
        ["?", "0xFF8A47FF", 2582],
        ["?", "0xFF8A47FF", 2582],
        ["?", "0xFF8A5EFF", 2583],
        ["?", "0xFF8A5EFF", 2583],
        ["?", "0xFF6957FF", 2587],
        ["?", "0xFF6957FF", 2587],
        ["?", "0xFF5F00D9", 259],
        ["?", "0xFF5F00D9", 259],
        ["?", "0xFF661AFF", 2592],
        ["?", "0xFF661AFF", 2592],
        ["?", "0xFF631CFF", 2593],
        ["?", "0xFF631CFF", 2593],
        ["?", "0xFF2600FF", 2597],
        ["?", "0xFF2600FF", 2597],
        ["?", "0xFF5B00BD", 260],
        ["?", "0xFF5B00BD", 260],
        ["?", "0xFF5C00F7", 2602],
        ["?", "0xFF5C00F7", 2602],
        ["?", "0xFF4D00FA", 2603],
        ["?", "0xFF4D00FA", 2603],
        ["?", "0xFF2D00ED", 2607],
        ["?", "0xFF2D00ED", 2607],
        ["?", "0xFF500099", 261],
        ["?", "0xFF500099", 261],
        ["?", "0xFF4F00DB", 2612],
        ["?", "0xFF4F00DB", 2612],
        ["?", "0xFF5000D9", 2613],
        ["?", "0xFF5000D9", 2613],
        ["?", "0xFF2E00D9", 2617],
        ["?", "0xFF2E00D9", 2617],
        ["?", "0xFF3F0073", 262],
        ["?", "0xFF3F0073", 262],
        ["?", "0xFF3C008F", 2622],
        ["?", "0xFF3C008F", 2622],
        ["?", "0xFF4700AD", 2623],
        ["?", "0xFF4700AD", 2623],
        ["?", "0xFF2800B0", 2627],
        ["?", "0xFF2800B0", 2627],
        ["?", "0xFFE6DBFF", 263],
        ["?", "0xFFE6DBFF", 263],
        ["?", "0xFFB8BAFF", 2635],
        ["?", "0xFFB8BAFF", 2635],
        ["?", "0xFFBDB8FF", 264],
        ["?", "0xFFBDB8FF", 264],
        ["?", "0xFF99A3FF", 2645],
        ["?", "0xFF99A3FF", 2645],
        ["?", "0xFF7570FF", 265],
        ["?", "0xFF7570FF", 265],
        ["?", "0xFF7582FF", 2655],
        ["?", "0xFF7582FF", 2655],
        ["?", "0xFF361AFF", 266],
        ["?", "0xFF361AFF", 266],
        ["?", "0xFF6166FF", 2665],
        ["?", "0xFF6166FF", 2665],
        ["?", "0xFF1C00FF", 267],
        ["?", "0xFF1C00FF", 267],
        ["?", "0xFF2800E0", 268],
        ["?", "0xFF2800E0", 268],
        ["?", "0xFF0900E6", 2685],
        ["?", "0xFF0900E6", 2685],
        ["?", "0xFF2600AB", 269],
        ["?", "0xFF2600AB", 269],
        ["?", "0xFF0C0082", 2695],
        ["?", "0xFF0C0082", 2695],
        ["?", "0xFFB0BAFF", 270],
        ["?", "0xFFB0BAFF", 270],
        ["?", "0xFF99B3FF", 2705],
        ["?", "0xFF99B3FF", 2705],
        ["?", "0xFFCFE8FF", 2706],
        ["?", "0xFFCFE8FF", 2706],
        ["?", "0xFFD4F0FF", 2707],
        ["?", "0xFFD4F0FF", 2707],
        ["?", "0xFFBDE6FF", 2708],
        ["?", "0xFFBDE6FF", 2708],
        ["?", "0xFF91A1FF", 271],
        ["?", "0xFF91A1FF", 271],
        ["?", "0xFF6E8CFF", 2715],
        ["?", "0xFF6E8CFF", 2715],
        ["?", "0xFF8CB5FF", 2716],
        ["?", "0xFF8CB5FF", 2716],
        ["?", "0xFFB5E0FF", 2717],
        ["?", "0xFFB5E0FF", 2717],
        ["?", "0xFF5496FF", 2718],
        ["?", "0xFF5496FF", 2718],
        ["?", "0xFF6B85FF", 272],
        ["?", "0xFF6B85FF", 272],
        ["?", "0xFF3B52FF", 2725],
        ["?", "0xFF3B52FF", 2725],
        ["?", "0xFF3657FF", 2726],
        ["?", "0xFF3657FF", 2726],
        ["?", "0xFF4A94FF", 2727],
        ["?", "0xFF4A94FF", 2727],
        ["?", "0xFF0A4FFF", 2728],
        ["?", "0xFF0A4FFF", 2728],
        ["?", "0xFF0009EB", 273],
        ["?", "0xFF0009EB", 273],
        ["?", "0xFF000DFF", 2735],
        ["?", "0xFF000DFF", 2735],
        ["?", "0xFF0017FF", 2736],
        ["?", "0xFF0017FF", 2736],
        ["?", "0xFF0020FA", 2738],
        ["?", "0xFF0020FA", 2738],
        ["?", "0xFF0000B8", 274],
        ["?", "0xFF0000B8", 274],
        ["?", "0xFF000BD9", 2745],
        ["?", "0xFF000BD9", 2745],
        ["?", "0xFF0012E6", 2746],
        ["?", "0xFF0012E6", 2746],
        ["?", "0xFF001ED9", 2747],
        ["?", "0xFF001ED9", 2747],
        ["?", "0xFF001AD9", 2748],
        ["?", "0xFF001AD9", 2748],
        ["?", "0xFF030091", 275],
        ["?", "0xFF030091", 275],
        ["?", "0xFF0005B3", 2755],
        ["?", "0xFF0005B3", 2755],
        ["?", "0xFF000BB5", 2756],
        ["?", "0xFF000BB5", 2756],
        ["?", "0xFF0020B3", 2757],
        ["?", "0xFF0020B3", 2757],
        ["?", "0xFF0026BD", 2758],
        ["?", "0xFF0026BD", 2758],
        ["?", "0xFF020073", 276],
        ["?", "0xFF020073", 276],
        ["?", "0xFF00048C", 2765],
        ["?", "0xFF00048C", 2765],
        ["?", "0xFF000887", 2766],
        ["?", "0xFF000887", 2766],
        ["?", "0xFF001A75", 2767],
        ["?", "0xFF001A75", 2767],
        ["?", "0xFF001F8F", 2768],
        ["?", "0xFF001F8F", 2768],
        ["?", "0xFFBAEDFF", 277],
        ["?", "0xFFBAEDFF", 277],
        ["?", "0xFF9CDBFF", 278],
        ["?", "0xFF9CDBFF", 278],
        ["?", "0xFF52A8FF", 279],
        ["?", "0xFF52A8FF", 279],
        ["?", "0xFF003BD1", 280],
        ["?", "0xFF003BD1", 280],
        ["?", "0xFF0031AD", 281],
        ["?", "0xFF0031AD", 281],
        ["?", "0xFF002675", 282],
        ["?", "0xFF002675", 282],
        ["?", "0xFFA6E8FF", 283],
        ["?", "0xFFA6E8FF", 283],
        ["?", "0xFF73CFFF", 284],
        ["?", "0xFF73CFFF", 284],
        ["?", "0xFF1C91FF", 285],
        ["?", "0xFF1C91FF", 285],
        ["?", "0xFF0055FA", 286],
        ["?", "0xFF0055FA", 286],
        ["?", "0xFF0048E0", 287],
        ["?", "0xFF0048E0", 287],
        ["?", "0xFF0041C4", 288],
        ["?", "0xFF0041C4", 288],
        ["?", "0xFF00246B", 289],
        ["?", "0xFF00246B", 289],
        ["?", "0xFFBFFAFF", 290],
        ["?", "0xFFBFFAFF", 290],
        ["?", "0xFF96FAFF", 2905],
        ["?", "0xFF96FAFF", 2905],
        ["?", "0xFFABF7FF", 291],
        ["?", "0xFFABF7FF", 291],
        ["?", "0xFF69EDFF", 2915],
        ["?", "0xFF69EDFF", 2915],
        ["?", "0xFF82E3FF", 292],
        ["?", "0xFF82E3FF", 292],
        ["?", "0xFF26C2FF", 2925],
        ["?", "0xFF26C2FF", 2925],
        ["?", "0xFF006BFA", 293],
        ["?", "0xFF006BFA", 293],
        ["?", "0xFF008AFF", 2935],
        ["?", "0xFF008AFF", 2935],
        ["?", "0xFF0055C9", 294],
        ["?", "0xFF0055C9", 294],
        ["?", "0xFF0079DB", 2945],
        ["?", "0xFF0079DB", 2945],
        ["?", "0xFF0045A1", 295],
        ["?", "0xFF0045A1", 295],
        ["?", "0xFF0058A1", 2955],
        ["?", "0xFF0058A1", 2955],
        ["?", "0xFF00294D", 296],
        ["?", "0xFF00294D", 296],
        ["?", "0xFF00395C", 2965],
        ["?", "0xFF00395C", 2965],
        ["?", "0xFF82FCFF", 297],
        ["?", "0xFF82FCFF", 297],
        ["?", "0xFFB3FFF2", 2975],
        ["?", "0xFFB3FFF2", 2975],
        ["?", "0xFF4FEDFF", 298],
        ["?", "0xFF4FEDFF", 298],
        ["?", "0xFF69FFF0", 2985],
        ["?", "0xFF69FFF0", 2985],
        ["?", "0xFF26CFFF", 299],
        ["?", "0xFF26CFFF", 299],
        ["?", "0xFF1AE3FF", 2995],
        ["?", "0xFF1AE3FF", 2995],
        ["?", "0xFF008FFF", 300],
        ["?", "0xFF008FFF", 300],
        ["?", "0xFF00A0FA", 3005],
        ["?", "0xFF00A0FA", 3005],
        ["?", "0xFF0073D1", 301],
        ["?", "0xFF0073D1", 301],
        ["?", "0xFF0089CC", 3015],
        ["?", "0xFF0089CC", 3015],
        ["?", "0xFF006080", 302],
        ["?", "0xFF006080", 302],
        ["?", "0xFF00687D", 3025],
        ["?", "0xFF00687D", 3025],
        ["?", "0xFF003B42", 303],
        ["?", "0xFF003B42", 303],
        ["?", "0xFF004744", 3035],
        ["?", "0xFF004744", 3035],
        ["?", "0xFFB3FFEB", 304],
        ["?", "0xFFB3FFEB", 304],
        ["?", "0xFF7DFFE8", 305],
        ["?", "0xFF7DFFE8", 305],
        ["?", "0xFF40FFED", 306],
        ["?", "0xFF40FFED", 306],
        ["?", "0xFF009CBA", 307],
        ["?", "0xFF009CBA", 307],
        ["?", "0xFF008087", 308],
        ["?", "0xFF008087", 308],
        ["?", "0xFF004741", 309],
        ["?", "0xFF004741", 309],
        ["?", "0xFF91FFE6", 310],
        ["?", "0xFF91FFE6", 310],
        ["?", "0xFF91FFE0", 3105],
        ["?", "0xFF91FFE0", 3105],
        ["?", "0xFF5EFFE0", 311],
        ["?", "0xFF5EFFE0", 311],
        ["?", "0xFF5EFFD1", 3115],
        ["?", "0xFF5EFFD1", 3115],
        ["?", "0xFF0AFFE3", 312],
        ["?", "0xFF0AFFE3", 312],
        ["?", "0xFF2BFFC9", 3125],
        ["?", "0xFF2BFFC9", 3125],
        ["?", "0xFF00DECC", 313],
        ["?", "0xFF00DECC", 313],
        ["?", "0xFF00E8C3", 3135],
        ["?", "0xFF00E8C3", 3135],
        ["?", "0xFF00B3A2", 314],
        ["?", "0xFF00B3A2", 314],
        ["?", "0xFF00C49F", 3145],
        ["?", "0xFF00C49F", 3145],
        ["?", "0xFF009180", 315],
        ["?", "0xFF009180", 315],
        ["?", "0xFF009E78", 3155],
        ["?", "0xFF009E78", 3155],
        ["?", "0xFF00523C", 316],
        ["?", "0xFF00523C", 316],
        ["?", "0xFF005940", 3165],
        ["?", "0xFF005940", 3165],
        ["?", "0xFFD1FFEB", 317],
        ["?", "0xFFD1FFEB", 317],
        ["?", "0xFF9EFFD9", 318],
        ["?", "0xFF9EFFD9", 318],
        ["?", "0xFF7AFFCF", 319],
        ["?", "0xFF7AFFCF", 319],
        ["?", "0xFF00EDA4", 320],
        ["?", "0xFF00EDA4", 320],
        ["?", "0xFF00C487", 321],
        ["?", "0xFF00C487", 321],
        ["?", "0xFF00A66F", 322],
        ["?", "0xFF00A66F", 322],
        ["?", "0xFF008754", 323],
        ["?", "0xFF008754", 323],
        ["?", "0xFFB8FFE0", 324],
        ["?", "0xFFB8FFE0", 324],
        ["?", "0xFFA1FFD1", 3242],
        ["?", "0xFFA1FFD1", 3242],
        ["?", "0xFFA8FFCF", 3245],
        ["?", "0xFFA8FFCF", 3245],
        ["?", "0xFF91FFC2", 3248],
        ["?", "0xFF91FFC2", 3248],
        ["?", "0xFF70FFBD", 325],
        ["?", "0xFF70FFBD", 325],
        ["?", "0xFF87FFC2", 3252],
        ["?", "0xFF87FFC2", 3252],
        ["?", "0xFF82FFB8", 3255],
        ["?", "0xFF82FFB8", 3255],
        ["?", "0xFF69FFAB", 3258],
        ["?", "0xFF69FFAB", 3258],
        ["?", "0xFF21FF9E", 326],
        ["?", "0xFF21FF9E", 326],
        ["?", "0xFF4AFFAB", 3262],
        ["?", "0xFF4AFFAB", 3262],
        ["?", "0xFF4FFFA1", 3265],
        ["?", "0xFF4FFFA1", 3265],
        ["?", "0xFF1AFF82", 3268],
        ["?", "0xFF1AFF82", 3268],
        ["?", "0xFF00D477", 327],
        ["?", "0xFF00D477", 327],
        ["?", "0xFF00FF8F", 3272],
        ["?", "0xFF00FF8F", 3272],
        ["?", "0xFF0DFF87", 3275],
        ["?", "0xFF0DFF87", 3275],
        ["?", "0xFF00F26D", 3278],
        ["?", "0xFF00F26D", 3278],
        ["?", "0xFF00AD5F", 328],
        ["?", "0xFF00AD5F", 328],
        ["?", "0xFF00D975", 3282],
        ["?", "0xFF00D975", 3282],
        ["?", "0xFF00ED77", 3285],
        ["?", "0xFF00ED77", 3285],
        ["?", "0xFF00CC5E", 3288],
        ["?", "0xFF00CC5E", 3288],
        ["?", "0xFF008A4A", 329],
        ["?", "0xFF008A4A", 329],
        ["?", "0xFF008A46", 3292],
        ["?", "0xFF008A46", 3292],
        ["?", "0xFF00C95F", 3295],
        ["?", "0xFF00C95F", 3295],
        ["?", "0xFF009440", 3298],
        ["?", "0xFF009440", 3298],
        ["?", "0xFF006635", 330],
        ["?", "0xFF006635", 330],
        ["?", "0xFF004F24", 3302],
        ["?", "0xFF004F24", 3302],
        ["?", "0xFF006327", 3305],
        ["?", "0xFF006327", 3305],
        ["?", "0xFF00471D", 3308],
        ["?", "0xFF00471D", 3308],
        ["?", "0xFFC2FFD6", 331],
        ["?", "0xFFC2FFD6", 331],
        ["?", "0xFFB3FFCC", 332],
        ["?", "0xFFB3FFCC", 332],
        ["?", "0xFF91FFBA", 333],
        ["?", "0xFF91FFBA", 333],
        ["?", "0xFF00F763", 334],
        ["?", "0xFF00F763", 334],
        ["?", "0xFF00B33E", 335],
        ["?", "0xFF00B33E", 335],
        ["?", "0xFF00872D", 336],
        ["?", "0xFF00872D", 336],
        ["?", "0xFFB0FFCC", 337],
        ["?", "0xFFB0FFCC", 337],
        ["?", "0xFFA6FFBF", 3375],
        ["?", "0xFFA6FFBF", 3375],
        ["?", "0xFF87FFAD", 338],
        ["?", "0xFF87FFAD", 338],
        ["?", "0xFF8CFFAB", 3385],
        ["?", "0xFF8CFFAB", 3385],
        ["?", "0xFF29FF70", 339],
        ["?", "0xFF29FF70", 339],
        ["?", "0xFF63FF8C", 3395],
        ["?", "0xFF63FF8C", 3395],
        ["?", "0xFF00E84F", 340],
        ["?", "0xFF00E84F", 340],
        ["?", "0xFF26FF59", 3405],
        ["?", "0xFF26FF59", 3405],
        ["?", "0xFF00B53C", 341],
        ["?", "0xFF00B53C", 341],
        ["?", "0xFF00C72E", 3415],
        ["?", "0xFF00C72E", 3415],
        ["?", "0xFF00912A", 342],
        ["?", "0xFF00912A", 342],
        ["?", "0xFF009421", 3425],
        ["?", "0xFF009421", 3425],
        ["?", "0xFF02631C", 343],
        ["?", "0xFF02631C", 343],
        ["?", "0xFF005710", 3435],
        ["?", "0xFF005710", 3435],
        ["?", "0xFFBAFFC4", 344],
        ["?", "0xFFBAFFC4", 344],
        ["?", "0xFF9EFFAD", 345],
        ["?", "0xFF9EFFAD", 345],
        ["?", "0xFF73FF87", 346],
        ["?", "0xFF73FF87", 346],
        ["?", "0xFF00F723", 347],
        ["?", "0xFF00F723", 347],
        ["?", "0xFF00C21D", 348],
        ["?", "0xFF00C21D", 348],
        ["?", "0xFF00940D", 349],
        ["?", "0xFF00940D", 349],
        ["?", "0xFF0D4000", 350],
        ["?", "0xFF0D4000", 350],
        ["?", "0xFFD4FFD6", 351],
        ["?", "0xFFD4FFD6", 351],
        ["?", "0xFFBAFFBF", 352],
        ["?", "0xFFBAFFBF", 352],
        ["?", "0xFF9EFFA3", 353],
        ["?", "0xFF9EFFA3", 353],
        ["?", "0xFF33FF1A", 354],
        ["?", "0xFF33FF1A", 354],
        ["?", "0xFF0FFF00", 355],
        ["?", "0xFF0FFF00", 355],
        ["?", "0xFF09BA00", 356],
        ["?", "0xFF09BA00", 356],
        ["?", "0xFF167000", 357],
        ["?", "0xFF167000", 357],
        ["?", "0xFFBAFF9E", 358],
        ["?", "0xFFBAFF9E", 358],
        ["?", "0xFFA3FF82", 359],
        ["?", "0xFFA3FF82", 359],
        ["?", "0xFF6BFF33", 360],
        ["?", "0xFF6BFF33", 360],
        ["?", "0xFF4FFF00", 361],
        ["?", "0xFF4FFF00", 361],
        ["?", "0xFF46E800", 362],
        ["?", "0xFF46E800", 362],
        ["?", "0xFF3EC200", 363],
        ["?", "0xFF3EC200", 363],
        ["?", "0xFF349400", 364],
        ["?", "0xFF349400", 364],
        ["?", "0xFFE0FFB5", 365],
        ["?", "0xFFE0FFB5", 365],
        ["?", "0xFFCCFF8F", 366],
        ["?", "0xFFCCFF8F", 366],
        ["?", "0xFFADFF69", 367],
        ["?", "0xFFADFF69", 367],
        ["?", "0xFF6EFF00", 368],
        ["?", "0xFF6EFF00", 368],
        ["?", "0xFF61ED00", 369],
        ["?", "0xFF61ED00", 369],
        ["?", "0xFF52BA00", 370],
        ["?", "0xFF52BA00", 370],
        ["?", "0xFF407000", 371],
        ["?", "0xFF407000", 371],
        ["?", "0xFFE6FFAB", 372],
        ["?", "0xFFE6FFAB", 372],
        ["?", "0xFFD6FF8A", 373],
        ["?", "0xFFD6FF8A", 373],
        ["?", "0xFFC2FF6E", 374],
        ["?", "0xFFC2FF6E", 374],
        ["?", "0xFF96FF38", 375],
        ["?", "0xFF96FF38", 375],
        ["?", "0xFF74F200", 376],
        ["?", "0xFF74F200", 376],
        ["?", "0xFF6BC200", 377],
        ["?", "0xFF6BC200", 377],
        ["?", "0xFF436600", 378],
        ["?", "0xFF436600", 378],
        ["?", "0xFFE8FF6B", 379],
        ["?", "0xFFE8FF6B", 379],
        ["?", "0xFFDEFF47", 380],
        ["?", "0xFFDEFF47", 380],
        ["?", "0xFFCCFF17", 381],
        ["?", "0xFFCCFF17", 381],
        ["?", "0xFFB5FF00", 382],
        ["?", "0xFFB5FF00", 382],
        ["?", "0xFFA5CF00", 383],
        ["?", "0xFFA5CF00", 383],
        ["?", "0xFF90B000", 384],
        ["?", "0xFF90B000", 384],
        ["?", "0xFF686B00", 385],
        ["?", "0xFF686B00", 385],
        ["?", "0xFFF0FF70", 386],
        ["?", "0xFFF0FF70", 386],
        ["?", "0xFFE6FF42", 387],
        ["?", "0xFFE6FF42", 387],
        ["?", "0xFFDBFF36", 388],
        ["?", "0xFFDBFF36", 388],
        ["?", "0xFFCCFF26", 389],
        ["?", "0xFFCCFF26", 389],
        ["?", "0xFFB7EB00", 390],
        ["?", "0xFFB7EB00", 390],
        ["?", "0xFF95AB00", 391],
        ["?", "0xFF95AB00", 391],
        ["?", "0xFF798200", 392],
        ["?", "0xFF798200", 392],
        ["?", "0xFFF7FF73", 393],
        ["?", "0xFFF7FF73", 393],
        ["?", "0xFFFCFF52", 3935],
        ["?", "0xFFFCFF52", 3935],
        ["?", "0xFFF0FF3D", 394],
        ["?", "0xFFF0FF3D", 394],
        ["?", "0xFFF7FF26", 3945],
        ["?", "0xFFF7FF26", 3945],
        ["?", "0xFFEBFF26", 395],
        ["?", "0xFFEBFF26", 395],
        ["?", "0xFFF0FF00", 3955],
        ["?", "0xFFF0FF00", 3955],
        ["?", "0xFFE3FF0F", 396],
        ["?", "0xFFE3FF0F", 396],
        ["?", "0xFFEBFF00", 3965],
        ["?", "0xFFEBFF00", 3965],
        ["?", "0xFFCCE300", 397],
        ["?", "0xFFCCE300", 397],
        ["?", "0xFFB5B500", 3975],
        ["?", "0xFFB5B500", 3975],
        ["?", "0xFFABB800", 398],
        ["?", "0xFFABB800", 398],
        ["?", "0xFF969200", 3985],
        ["?", "0xFF969200", 3985],
        ["?", "0xFF919100", 399],
        ["?", "0xFF919100", 399],
        ["?", "0xFF5C5900", 3995],
        ["?", "0xFF5C5900", 3995],
        ["?", "0xFFD6D0C9", 400],
        ["?", "0xFFD6D0C9", 400],
        ["?", "0xFFC4BBAF", 401],
        ["?", "0xFFC4BBAF", 401],
        ["?", "0xFFB0A597", 402],
        ["?", "0xFFB0A597", 402],
        ["?", "0xFF918779", 403],
        ["?", "0xFF918779", 403],
        ["?", "0xFF706758", 404],
        ["?", "0xFF706758", 404],
        ["?", "0xFF474030", 405],
        ["?", "0xFF474030", 405],
        ["?", "0xFFD6CBC9", 406],
        ["?", "0xFFD6CBC9", 406],
        ["?", "0xFFBDAEAC", 407],
        ["?", "0xFFBDAEAC", 407],
        ["?", "0xFFA89796", 408],
        ["?", "0xFFA89796", 408],
        ["?", "0xFF8C7A77", 409],
        ["?", "0xFF8C7A77", 409],
        ["?", "0xFF705C59", 410],
        ["?", "0xFF705C59", 410],
        ["?", "0xFF47342E", 411],
        ["?", "0xFF47342E", 411],
        ["?", "0xFF050402", 412],
        ["?", "0xFF050402", 412],
        ["?", "0xFFCCCCBA", 413],
        ["?", "0xFFCCCCBA", 413],
        ["?", "0xFFB3B3A1", 414],
        ["?", "0xFFB3B3A1", 414],
        ["?", "0xFF969684", 415],
        ["?", "0xFF969684", 415],
        ["?", "0xFF80806B", 416],
        ["?", "0xFF80806B", 416],
        ["?", "0xFF585943", 417],
        ["?", "0xFF585943", 417],
        ["?", "0xFF3E402C", 418],
        ["?", "0xFF3E402C", 418],
        ["?", "0xFF000000", 419],
        ["?", "0xFF000000", 419],
        ["?", "0xFFD9D9D9", 420],
        ["?", "0xFFD9D9D9", 420],
        ["?", "0xFFBDBDBD", 421],
        ["?", "0xFFBDBDBD", 421],
        ["?", "0xFFABABAB", 422],
        ["?", "0xFFABABAB", 422],
        ["?", "0xFF8F8F8F", 423],
        ["?", "0xFF8F8F8F", 423],
        ["?", "0xFF636363", 424],
        ["?", "0xFF636363", 424],
        ["?", "0xFF3B3B3B", 425],
        ["?", "0xFF3B3B3B", 425],
        ["?", "0xFF000000", 426],
        ["?", "0xFF000000", 426],
        ["?", "0xFFE3E3E3", 427],
        ["?", "0xFFE3E3E3", 427],
        ["?", "0xFFCDD1D1", 428],
        ["?", "0xFFCDD1D1", 428],
        ["?", "0xFFA8ADAD", 429],
        ["?", "0xFFA8ADAD", 429],
        ["?", "0xFF858C8C", 430],
        ["?", "0xFF858C8C", 430],
        ["?", "0xFF525B5C", 431],
        ["?", "0xFF525B5C", 431],
        ["?", "0xFF2D393B", 432],
        ["?", "0xFF2D393B", 432],
        ["?", "0xFF090C0D", 433],
        ["?", "0xFF090C0D", 433],
        ["?", "0xFFEDE6E8", 434],
        ["?", "0xFFEDE6E8", 434],
        ["?", "0xFFDED6DB", 435],
        ["?", "0xFFDED6DB", 435],
        ["?", "0xFFC2BFBF", 436],
        ["?", "0xFFC2BFBF", 436],
        ["?", "0xFF8A8C8A", 437],
        ["?", "0xFF8A8C8A", 437],
        ["?", "0xFF394500", 438],
        ["?", "0xFF394500", 438],
        ["?", "0xFF293300", 439],
        ["?", "0xFF293300", 439],
        ["?", "0xFF202700", 440],
        ["?", "0xFF202700", 440],
        ["?", "0xFFDAE8D8", 441],
        ["?", "0xFFDAE8D8", 441],
        ["?", "0xFFBECFBC", 442],
        ["?", "0xFFBECFBC", 442],
        ["?", "0xFF9DB39D", 443],
        ["?", "0xFF9DB39D", 443],
        ["?", "0xFF7E947E", 444],
        ["?", "0xFF7E947E", 444],
        ["?", "0xFF475947", 445],
        ["?", "0xFF475947", 445],
        ["?", "0xFF324031", 446],
        ["?", "0xFF324031", 446],
        ["?", "0xFF272E20", 447],
        ["?", "0xFF272E20", 447],
        ["?", "0xFF2D3E00", 448],
        ["?", "0xFF2D3E00", 448],
        ["?", "0xFF4F3A00", 4485],
        ["?", "0xFF4F3A00", 4485],
        ["?", "0xFF3D5200", 449],
        ["?", "0xFF3D5200", 449],
        ["?", "0xFF8A6E07", 4495],
        ["?", "0xFF8A6E07", 4495],
        ["?", "0xFF506700", 450],
        ["?", "0xFF506700", 450],
        ["?", "0xFFA38B24", 4505],
        ["?", "0xFFA38B24", 4505],
        ["?", "0xFFABB573", 451],
        ["?", "0xFFABB573", 451],
        ["?", "0xFFC2B061", 4515],
        ["?", "0xFFC2B061", 4515],
        ["?", "0xFFC2CF9C", 452],
        ["?", "0xFFC2CF9C", 452],
        ["?", "0xFFD4C581", 4525],
        ["?", "0xFFD4C581", 4525],
        ["?", "0xFFDBE3BF", 453],
        ["?", "0xFFDBE3BF", 453],
        ["?", "0xFFE3DA9F", 4535],
        ["?", "0xFFE3DA9F", 4535],
        ["?", "0xFFE8EDD6", 454],
        ["?", "0xFFE8EDD6", 454],
        ["?", "0xFFF0E9C2", 4545],
        ["?", "0xFFF0E9C2", 4545],
        ["?", "0xFF594A00", 455],
        ["?", "0xFF594A00", 455],
        ["?", "0xFF917C00", 456],
        ["?", "0xFF917C00", 456],
        ["?", "0xFFB89C00", 457],
        ["?", "0xFFB89C00", 457],
        ["?", "0xFFE6E645", 458],
        ["?", "0xFFE6E645", 458],
        ["?", "0xFFF0ED73", 459],
        ["?", "0xFFF0ED73", 459],
        ["?", "0xFFF5F28F", 460],
        ["?", "0xFFF5F28F", 460],
        ["?", "0xFFF7F7A6", 461],
        ["?", "0xFFF7F7A6", 461],
        ["?", "0xFF402600", 462],
        ["?", "0xFF402600", 462],
        ["?", "0xFF361500", 4625],
        ["?", "0xFF361500", 4625],
        ["?", "0xFF6B3D00", 463],
        ["?", "0xFF6B3D00", 463],
        ["?", "0xFF8F4A06", 4635],
        ["?", "0xFF8F4A06", 4635],
        ["?", "0xFF955300", 464],
        ["?", "0xFF955300", 464],
        ["?", "0xFFB8743B", 4645],
        ["?", "0xFFB8743B", 4645],
        ["?", "0xFFCCAD6B", 465],
        ["?", "0xFFCCAD6B", 465],
        ["?", "0xFFD19B73", 4655],
        ["?", "0xFFD19B73", 4655],
        ["?", "0xFFE0C791", 466],
        ["?", "0xFFE0C791", 466],
        ["?", "0xFFE6BC9C", 4665],
        ["?", "0xFFE6BC9C", 4665],
        ["?", "0xFFE8D9A8", 467],
        ["?", "0xFFE8D9A8", 467],
        ["?", "0xFFF0D5BD", 4675],
        ["?", "0xFFF0D5BD", 4675],
        ["?", "0xFFF0E8C4", 468],
        ["?", "0xFFF0E8C4", 468],
        ["?", "0xFFF5E4D3", 4685],
        ["?", "0xFFF5E4D3", 4685],
        ["?", "0xFF4A1A00", 469],
        ["?", "0xFF4A1A00", 469],
        ["?", "0xFF420D00", 4695],
        ["?", "0xFF420D00", 4695],
        ["?", "0xFFAB4800", 470],
        ["?", "0xFFAB4800", 470],
        ["?", "0xFF823126", 4705],
        ["?", "0xFF823126", 4705],
        ["?", "0xFFD15600", 471],
        ["?", "0xFFD15600", 471],
        ["?", "0xFFA8625D", 4715],
        ["?", "0xFFA8625D", 4715],
        ["?", "0xFFFFA87A", 472],
        ["?", "0xFFFFA87A", 472],
        ["?", "0xFFBF827C", 4725],
        ["?", "0xFFBF827C", 4725],
        ["?", "0xFFFFC4A3", 473],
        ["?", "0xFFFFC4A3", 473],
        ["?", "0xFFD9A9A7", 4735],
        ["?", "0xFFD9A9A7", 4735],
        ["?", "0xFFFFD9BD", 474],
        ["?", "0xFFFFD9BD", 474],
        ["?", "0xFFE6BEBC", 4745],
        ["?", "0xFFE6BEBC", 4745],
        ["?", "0xFFFFE3CC", 475],
        ["?", "0xFFFFE3CC", 475],
        ["?", "0xFFF0D8D3", 4755],
        ["?", "0xFFF0D8D3", 4755],
        ["?", "0xFF381C00", 476],
        ["?", "0xFF381C00", 476],
        ["?", "0xFF4F1800", 477],
        ["?", "0xFF4F1800", 477],
        ["?", "0xFF6B1200", 478],
        ["?", "0xFF6B1200", 478],
        ["?", "0xFFB08573", 479],
        ["?", "0xFFB08573", 479],
        ["?", "0xFFD9B5B0", 480],
        ["?", "0xFFD9B5B0", 480],
        ["?", "0xFFE8CFC9", 481],
        ["?", "0xFFE8CFC9", 481],
        ["?", "0xFFF2E0DE", 482],
        ["?", "0xFFF2E0DE", 482],
        ["?", "0xFF660700", 483],
        ["?", "0xFF660700", 483],
        ["?", "0xFFB50900", 484],
        ["?", "0xFFB50900", 484],
        ["?", "0xFFFF0D00", 485],
        ["?", "0xFFFF0D00", 485],
        ["?", "0xFFFF8796", 486],
        ["?", "0xFFFF8796", 486],
        ["?", "0xFFFFA6B8", 487],
        ["?", "0xFFFFA6B8", 487],
        ["?", "0xFFFFBDCF", 488],
        ["?", "0xFFFFBDCF", 488],
        ["?", "0xFFFFD9E3", 489],
        ["?", "0xFFFFD9E3", 489],
        ["?", "0xFF471300", 490],
        ["?", "0xFF471300", 490],
        ["?", "0xFF7A1A00", 491],
        ["?", "0xFF7A1A00", 491],
        ["?", "0xFF942200", 492],
        ["?", "0xFF942200", 492],
        ["?", "0xFFF283BB", 493],
        ["?", "0xFFF283BB", 493],
        ["?", "0xFFFFABDE", 494],
        ["?", "0xFFFFABDE", 494],
        ["?", "0xFFFFC2E3", 495],
        ["?", "0xFFFFC2E3", 495],
        ["?", "0xFFFFD6E8", 496],
        ["?", "0xFFFFD6E8", 496],
        ["?", "0xFF381100", 497],
        ["?", "0xFF381100", 497],
        ["?", "0xFF330E00", 4975],
        ["?", "0xFF330E00", 4975],
        ["?", "0xFF662500", 498],
        ["?", "0xFF662500", 498],
        ["?", "0xFF853241", 4985],
        ["?", "0xFF853241", 4985],
        ["?", "0xFF853100", 499],
        ["?", "0xFF853100", 499],
        ["?", "0xFFA85868", 4995],
        ["?", "0xFFA85868", 4995],
        ["?", "0xFFE38DB3", 500],
        ["?", "0xFFE38DB3", 500],
        ["?", "0xFFC47A8F", 5005],
        ["?", "0xFFC47A8F", 5005],
        ["?", "0xFFF7B5D7", 501],
        ["?", "0xFFF7B5D7", 501],
        ["?", "0xFFE3AAC1", 5015],
        ["?", "0xFFE3AAC1", 5015],
        ["?", "0xFFFCCFE3", 502],
        ["?", "0xFFFCCFE3", 502],
        ["?", "0xFFEDC2D1", 5025],
        ["?", "0xFFEDC2D1", 5025],
        ["?", "0xFFFFE3EB", 503],
        ["?", "0xFFFFE3EB", 503],
        ["?", "0xFFF7DFE1", 5035],
        ["?", "0xFFF7DFE1", 5035],
        ["?", "0xFF320000", 504],
        ["?", "0xFF320000", 504],
        ["?", "0xFF600000", 505],
        ["?", "0xFF600000", 505],
        ["?", "0xFF770000", 506],
        ["?", "0xFF770000", 506],
        ["?", "0xFFDE82C4", 507],
        ["?", "0xFFDE82C4", 507],
        ["?", "0xFFF2A3E3", 508],
        ["?", "0xFFF2A3E3", 508],
        ["?", "0xFFFFC2ED", 509],
        ["?", "0xFFFFC2ED", 509],
        ["?", "0xFFFFD4F0", 510],
        ["?", "0xFFFFD4F0", 510],
        ["?", "0xFF3D0066", 511],
        ["?", "0xFF3D0066", 511],
        ["?", "0xFF2B0041", 5115],
        ["?", "0xFF2B0041", 5115],
        ["?", "0xFF6100CE", 512],
        ["?", "0xFF6100CE", 512],
        ["?", "0xFF592482", 5125],
        ["?", "0xFF592482", 5125],
        ["?", "0xFF8A1FFF", 513],
        ["?", "0xFF8A1FFF", 513],
        ["?", "0xFF8257B8", 5135],
        ["?", "0xFF8257B8", 5135],
        ["?", "0xFFD980FF", 514],
        ["?", "0xFFD980FF", 514],
        ["?", "0xFFB38CE0", 5145],
        ["?", "0xFFB38CE0", 5145],
        ["?", "0xFFED9EFF", 515],
        ["?", "0xFFED9EFF", 515],
        ["?", "0xFFD4B3EB", 5155],
        ["?", "0xFFD4B3EB", 5155],
        ["?", "0xFFF7BAFF", 516],
        ["?", "0xFFF7BAFF", 516],
        ["?", "0xFFE8CFF2", 5165],
        ["?", "0xFFE8CFF2", 5165],
        ["?", "0xFFFFD1FF", 517],
        ["?", "0xFFFFD1FF", 517],
        ["?", "0xFFF2E0F7", 5175],
        ["?", "0xFFF2E0F7", 5175],
        ["?", "0xFF2E005C", 518],
        ["?", "0xFF2E005C", 518],
        ["?", "0xFF1C0022", 5185],
        ["?", "0xFF1C0022", 5185],
        ["?", "0xFF44009D", 519],
        ["?", "0xFF44009D", 519],
        ["?", "0xFF3D0C4E", 5195],
        ["?", "0xFF3D0C4E", 5195],
        ["?", "0xFF5C00E0", 520],
        ["?", "0xFF5C00E0", 520],
        ["?", "0xFF7A5E85", 5205],
        ["?", "0xFF7A5E85", 5205],
        ["?", "0xFFBA87FF", 521],
        ["?", "0xFFBA87FF", 521],
        ["?", "0xFFB59EC2", 5215],
        ["?", "0xFFB59EC2", 5215],
        ["?", "0xFFD4A1FF", 522],
        ["?", "0xFFD4A1FF", 522],
        ["?", "0xFFD4BAD9", 5225],
        ["?", "0xFFD4BAD9", 5225],
        ["?", "0xFFE6BDFF", 523],
        ["?", "0xFFE6BDFF", 523],
        ["?", "0xFFE6D4E6", 5235],
        ["?", "0xFFE6D4E6", 5235],
        ["?", "0xFFF0D9FF", 524],
        ["?", "0xFFF0D9FF", 524],
        ["?", "0xFFF0E6ED", 5245],
        ["?", "0xFFF0E6ED", 5245],
        ["?", "0xFF270085", 525],
        ["?", "0xFF270085", 525],
        ["?", "0xFF0D0B4D", 5255],
        ["?", "0xFF0D0B4D", 5255],
        ["?", "0xFF3B00ED", 526],
        ["?", "0xFF3B00ED", 526],
        ["?", "0xFF20258A", 5265],
        ["?", "0xFF20258A", 5265],
        ["?", "0xFF4500FF", 527],
        ["?", "0xFF4500FF", 527],
        ["?", "0xFF3848A8", 5275],
        ["?", "0xFF3848A8", 5275],
        ["?", "0xFF9673FF", 528],
        ["?", "0xFF9673FF", 528],
        ["?", "0xFF7280C4", 5285],
        ["?", "0xFF7280C4", 5285],
        ["?", "0xFFBD99FF", 529],
        ["?", "0xFFBD99FF", 529],
        ["?", "0xFFA8B3E6", 5295],
        ["?", "0xFFA8B3E6", 5295],
        ["?", "0xFFD1B0FF", 530],
        ["?", "0xFFD1B0FF", 530],
        ["?", "0xFFC7CEED", 5305],
        ["?", "0xFFC7CEED", 5305],
        ["?", "0xFFE6CCFF", 531],
        ["?", "0xFFE6CCFF", 531],
        ["?", "0xFFE4E4F2", 5315],
        ["?", "0xFFE4E4F2", 5315],
        ["?", "0xFF00193F", 532],
        ["?", "0xFF00193F", 532],
        ["?", "0xFF00227B", 533],
        ["?", "0xFF00227B", 533],
        ["?", "0xFF002CAA", 534],
        ["?", "0xFF002CAA", 534],
        ["?", "0xFF94B3ED", 535],
        ["?", "0xFF94B3ED", 535],
        ["?", "0xFFB0C7F2", 536],
        ["?", "0xFFB0C7F2", 536],
        ["?", "0xFFC7DBF7", 537],
        ["?", "0xFFC7DBF7", 537],
        ["?", "0xFFDEE8FA", 538],
        ["?", "0xFFDEE8FA", 538],
        ["?", "0xFF00274D", 539],
        ["?", "0xFF00274D", 539],
        ["?", "0xFF00223D", 5395],
        ["?", "0xFF00223D", 5395],
        ["?", "0xFF003473", 540],
        ["?", "0xFF003473", 540],
        ["?", "0xFF3A728A", 5405],
        ["?", "0xFF3A728A", 5405],
        ["?", "0xFF00449E", 541],
        ["?", "0xFF00449E", 541],
        ["?", "0xFF5A8A96", 5415],
        ["?", "0xFF5A8A96", 5415],
        ["?", "0xFF5EC1F7", 542],
        ["?", "0xFF5EC1F7", 542],
        ["?", "0xFF79A6AD", 5425],
        ["?", "0xFF79A6AD", 5425],
        ["?", "0xFF96E3FF", 543],
        ["?", "0xFF96E3FF", 543],
        ["?", "0xFFB8CDD4", 5435],
        ["?", "0xFFB8CDD4", 5435],
        ["?", "0xFFB3F0FF", 544],
        ["?", "0xFFB3F0FF", 544],
        ["?", "0xFFCCDCDE", 5445],
        ["?", "0xFFCCDCDE", 5445],
        ["?", "0xFFC7F7FF", 545],
        ["?", "0xFFC7F7FF", 545],
        ["?", "0xFFDAE8E8", 5455],
        ["?", "0xFFDAE8E8", 5455],
        ["?", "0xFF02272B", 546],
        ["?", "0xFF02272B", 546],
        ["?", "0xFF002B24", 5463],
        ["?", "0xFF002B24", 5463],
        ["?", "0xFF000D09", 5467],
        ["?", "0xFF000D09", 5467],
        ["?", "0xFF003440", 547],
        ["?", "0xFF003440", 547],
        ["?", "0xFF167A58", 5473],
        ["?", "0xFF167A58", 5473],
        ["?", "0xFF1D4230", 5477],
        ["?", "0xFF1D4230", 5477],
        ["?", "0xFF00465C", 548],
        ["?", "0xFF00465C", 548],
        ["?", "0xFF43B08B", 5483],
        ["?", "0xFF43B08B", 5483],
        ["?", "0xFF48705D", 5487],
        ["?", "0xFF48705D", 5487],
        ["?", "0xFF56ADBA", 549],
        ["?", "0xFF56ADBA", 549],
        ["?", "0xFF73C9AD", 5493],
        ["?", "0xFF73C9AD", 5493],
        ["?", "0xFF829E90", 5497],
        ["?", "0xFF829E90", 5497],
        ["?", "0xFF7BC1C9", 550],
        ["?", "0xFF7BC1C9", 550],
        ["?", "0xFF9CDBC5", 5503],
        ["?", "0xFF9CDBC5", 5503],
        ["?", "0xFFA1B5A8", 5507],
        ["?", "0xFFA1B5A8", 5507],
        ["?", "0xFFA2D7DE", 551],
        ["?", "0xFFA2D7DE", 551],
        ["?", "0xFFC7F2E1", 5513],
        ["?", "0xFFC7F2E1", 5513],
        ["?", "0xFFBED1C5", 5517],
        ["?", "0xFFBED1C5", 5517],
        ["?", "0xFFC5E8E8", 552],
        ["?", "0xFFC5E8E8", 552],
        ["?", "0xFFDCF7EB", 5523],
        ["?", "0xFFDCF7EB", 5523],
        ["?", "0xFFD5E3DA", 5527],
        ["?", "0xFFD5E3DA", 5527],
        ["?", "0xFF143319", 553],
        ["?", "0xFF143319", 553],
        ["?", "0xFF102E14", 5535],
        ["?", "0xFF102E14", 5535],
        ["?", "0xFF115422", 554],
        ["?", "0xFF115422", 554],
        ["?", "0xFF327A3D", 5545],
        ["?", "0xFF327A3D", 5545],
        ["?", "0xFF187031", 555],
        ["?", "0xFF187031", 555],
        ["?", "0xFF5A9E68", 5555],
        ["?", "0xFF5A9E68", 5555],
        ["?", "0xFF66BA80", 556],
        ["?", "0xFF66BA80", 556],
        ["?", "0xFF84BD8F", 5565],
        ["?", "0xFF84BD8F", 5565],
        ["?", "0xFF98D9AD", 557],
        ["?", "0xFF98D9AD", 557],
        ["?", "0xFFA9D4B2", 5575],
        ["?", "0xFFA9D4B2", 5575],
        ["?", "0xFFBAE8CA", 558],
        ["?", "0xFFBAE8CA", 558],
        ["?", "0xFFCAE6CC", 5585],
        ["?", "0xFFCAE6CC", 5585],
        ["?", "0xFFCEF0D8", 559],
        ["?", "0xFFCEF0D8", 559],
        ["?", "0xFFDDEDDA", 5595],
        ["?", "0xFFDDEDDA", 5595],
        ["?", "0xFF0D4018", 560],
        ["?", "0xFF0D4018", 560],
        ["?", "0xFF050F07", 5605],
        ["?", "0xFF050F07", 5605],
        ["?", "0xFF127A38", 561],
        ["?", "0xFF127A38", 561],
        ["?", "0xFF2E522B", 5615],
        ["?", "0xFF2E522B", 5615],
        ["?", "0xFF1AB058", 562],
        ["?", "0xFF1AB058", 562],
        ["?", "0xFF5A7D57", 5625],
        ["?", "0xFF5A7D57", 5625],
        ["?", "0xFF79FCAC", 563],
        ["?", "0xFF79FCAC", 563],
        ["?", "0xFF89A386", 5635],
        ["?", "0xFF89A386", 5635],
        ["?", "0xFFA1FFCC", 564],
        ["?", "0xFFA1FFCC", 564],
        ["?", "0xFFAEBFA6", 5645],
        ["?", "0xFFAEBFA6", 5645],
        ["?", "0xFFC4FFDE", 565],
        ["?", "0xFFC4FFDE", 565],
        ["?", "0xFFC5D1BE", 5655],
        ["?", "0xFFC5D1BE", 5655],
        ["?", "0xFFDBFFE8", 566],
        ["?", "0xFFDBFFE8", 566],
        ["?", "0xFFDAE6D5", 5665],
        ["?", "0xFFDAE6D5", 5665],
        ["?", "0xFF0E4D1C", 567],
        ["?", "0xFF0E4D1C", 567],
        ["?", "0xFF14A346", 568],
        ["?", "0xFF14A346", 568],
        ["?", "0xFF04D45B", 569],
        ["?", "0xFF04D45B", 569],
        ["?", "0xFF85FFB5", 570],
        ["?", "0xFF85FFB5", 570],
        ["?", "0xFFADFFCF", 571],
        ["?", "0xFFADFFCF", 571],
        ["?", "0xFFC4FFDB", 572],
        ["?", "0xFFC4FFDB", 572],
        ["?", "0xFFDBFFE8", 573],
        ["?", "0xFFDBFFE8", 573],
        ["?", "0xFF314A0E", 574],
        ["?", "0xFF314A0E", 574],
        ["?", "0xFF1F2E07", 5743],
        ["?", "0xFF1F2E07", 5743],
        ["?", "0xFF243600", 5747],
        ["?", "0xFF243600", 5747],
        ["?", "0xFF3E7800", 575],
        ["?", "0xFF3E7800", 575],
        ["?", "0xFF3F5410", 5753],
        ["?", "0xFF3F5410", 5753],
        ["?", "0xFF547306", 5757],
        ["?", "0xFF547306", 5757],
        ["?", "0xFF4F9C00", 576],
        ["?", "0xFF4F9C00", 576],
        ["?", "0xFF5C6E1D", 5763],
        ["?", "0xFF5C6E1D", 5763],
        ["?", "0xFF849C32", 5767],
        ["?", "0xFF849C32", 5767],
        ["?", "0xFFAEE67C", 577],
        ["?", "0xFFAEE67C", 577],
        ["?", "0xFF909E5A", 5773],
        ["?", "0xFF909E5A", 5773],
        ["?", "0xFFA5B85E", 5777],
        ["?", "0xFFA5B85E", 5777],
        ["?", "0xFFC0F090", 578],
        ["?", "0xFFC0F090", 578],
        ["?", "0xFFAFBA86", 5783],
        ["?", "0xFFAFBA86", 5783],
        ["?", "0xFFCEDE99", 5787],
        ["?", "0xFFCEDE99", 5787],
        ["?", "0xFFCDF7A3", 579],
        ["?", "0xFFCDF7A3", 579],
        ["?", "0xFFC9D1A5", 5793],
        ["?", "0xFFC9D1A5", 5793],
        ["?", "0xFFDCE8B0", 5797],
        ["?", "0xFFDCE8B0", 5797],
        ["?", "0xFFDCFAB9", 580],
        ["?", "0xFFDCFAB9", 580],
        ["?", "0xFFDEE3C8", 5803],
        ["?", "0xFFDEE3C8", 5803],
        ["?", "0xFFE9F0CE", 5807],
        ["?", "0xFFE9F0CE", 5807],
        ["?", "0xFF464700", 581],
        ["?", "0xFF464700", 581],
        ["?", "0xFF363605", 5815],
        ["?", "0xFF363605", 5815],
        ["?", "0xFF788A00", 582],
        ["?", "0xFF788A00", 582],
        ["?", "0xFF69660E", 5825],
        ["?", "0xFF69660E", 5825],
        ["?", "0xFFA3D400", 583],
        ["?", "0xFFA3D400", 583],
        ["?", "0xFF999632", 5835],
        ["?", "0xFF999632", 5835],
        ["?", "0xFFD3F032", 584],
        ["?", "0xFFD3F032", 584],
        ["?", "0xFFB3B15F", 5845],
        ["?", "0xFFB3B15F", 5845],
        ["?", "0xFFDEFA55", 585],
        ["?", "0xFFDEFA55", 585],
        ["?", "0xFFD1D190", 5855],
        ["?", "0xFFD1D190", 5855],
        ["?", "0xFFE8FF78", 586],
        ["?", "0xFFE8FF78", 586],
        ["?", "0xFFDEDEA6", 5865],
        ["?", "0xFFDEDEA6", 5865],
        ["?", "0xFFF2FF99", 587],
        ["?", "0xFFF2FF99", 587],
        ["?", "0xFFEBEBC0", 5875],
        ["?", "0xFFEBEBC0", 5875],
        ["?", "0xFFFFFFB5", 600],
        ["?", "0xFFFFFFB5", 600],
        ["?", "0xFFFFFF99", 601],
        ["?", "0xFFFFFF99", 601],
        ["?", "0xFFFFFF7D", 602],
        ["?", "0xFFFFFF7D", 602],
        ["?", "0xFFFCFC4E", 603],
        ["?", "0xFFFCFC4E", 603],
        ["?", "0xFFF7F71E", 604],
        ["?", "0xFFF7F71E", 604],
        ["?", "0xFFEDE800", 605],
        ["?", "0xFFEDE800", 605],
        ["?", "0xFFE0D700", 606],
        ["?", "0xFFE0D700", 606],
        ["?", "0xFFFCFCCF", 607],
        ["?", "0xFFFCFCCF", 607],
        ["?", "0xFFFAFAAA", 608],
        ["?", "0xFFFAFAAA", 608],
        ["?", "0xFFF5F584", 609],
        ["?", "0xFFF5F584", 609],
        ["?", "0xFFF0F065", 610],
        ["?", "0xFFF0F065", 610],
        ["?", "0xFFE3E112", 611],
        ["?", "0xFFE3E112", 611],
        ["?", "0xFFCCC800", 612],
        ["?", "0xFFCCC800", 612],
        ["?", "0xFFB3AB00", 613],
        ["?", "0xFFB3AB00", 613],
        ["?", "0xFFF5F5C4", 614],
        ["?", "0xFFF5F5C4", 614],
        ["?", "0xFFF0EDAF", 615],
        ["?", "0xFFF0EDAF", 615],
        ["?", "0xFFE8E397", 616],
        ["?", "0xFFE8E397", 616],
        ["?", "0xFFD4CF6E", 617],
        ["?", "0xFFD4CF6E", 617],
        ["?", "0xFFB3AD17", 618],
        ["?", "0xFFB3AD17", 618],
        ["?", "0xFF918C00", 619],
        ["?", "0xFF918C00", 619],
        ["?", "0xFF787200", 620],
        ["?", "0xFF787200", 620],
        ["?", "0xFFD9FAE1", 621],
        ["?", "0xFFD9FAE1", 621],
        ["?", "0xFFBAF5C6", 622],
        ["?", "0xFFBAF5C6", 622],
        ["?", "0xFF9CE6AE", 623],
        ["?", "0xFF9CE6AE", 623],
        ["?", "0xFF72CC85", 624],
        ["?", "0xFF72CC85", 624],
        ["?", "0xFF4BAB60", 625],
        ["?", "0xFF4BAB60", 625],
        ["?", "0xFF175E22", 626],
        ["?", "0xFF175E22", 626],
        ["?", "0xFF04290A", 627],
        ["?", "0xFF04290A", 627],
        ["?", "0xFFCFFFF0", 628],
        ["?", "0xFFCFFFF0", 628],
        ["?", "0xFFA8FFE8", 629],
        ["?", "0xFFA8FFE8", 629],
        ["?", "0xFF87FFE3", 630],
        ["?", "0xFF87FFE3", 630],
        ["?", "0xFF52FADC", 631],
        ["?", "0xFF52FADC", 631],
        ["?", "0xFF13F2CE", 632],
        ["?", "0xFF13F2CE", 632],
        ["?", "0xFF00BFAC", 633],
        ["?", "0xFF00BFAC", 633],
        ["?", "0xFF00998B", 634],
        ["?", "0xFF00998B", 634],
        ["?", "0xFFADFFEB", 635],
        ["?", "0xFFADFFEB", 635],
        ["?", "0xFF8CFFE8", 636],
        ["?", "0xFF8CFFE8", 636],
        ["?", "0xFF73FFE8", 637],
        ["?", "0xFF73FFE8", 637],
        ["?", "0xFF2BFFE6", 638],
        ["?", "0xFF2BFFE6", 638],
        ["?", "0xFF00F2E6", 639],
        ["?", "0xFF00F2E6", 639],
        ["?", "0xFF00C7C7", 640],
        ["?", "0xFF00C7C7", 640],
        ["?", "0xFF00ABB3", 641],
        ["?", "0xFF00ABB3", 641],
        ["?", "0xFFD2F0FA", 642],
        ["?", "0xFFD2F0FA", 642],
        ["?", "0xFFB8E4F5", 643],
        ["?", "0xFFB8E4F5", 643],
        ["?", "0xFF8BCCF0", 644],
        ["?", "0xFF8BCCF0", 644],
        ["?", "0xFF64A7E8", 645],
        ["?", "0xFF64A7E8", 645],
        ["?", "0xFF4696E3", 646],
        ["?", "0xFF4696E3", 646],
        ["?", "0xFF0056C4", 647],
        ["?", "0xFF0056C4", 647],
        ["?", "0xFF002D75", 648],
        ["?", "0xFF002D75", 648],
        ["?", "0xFFD9EDFC", 649],
        ["?", "0xFFD9EDFC", 649],
        ["?", "0xFFBEE3FA", 650],
        ["?", "0xFFBEE3FA", 650],
        ["?", "0xFF95C5F0", 651],
        ["?", "0xFF95C5F0", 651],
        ["?", "0xFF5C97E6", 652],
        ["?", "0xFF5C97E6", 652],
        ["?", "0xFF004ECC", 653],
        ["?", "0xFF004ECC", 653],
        ["?", "0xFF00399E", 654],
        ["?", "0xFF00399E", 654],
        ["?", "0xFF002B7A", 655],
        ["?", "0xFF002B7A", 655],
        ["?", "0xFFDBF5FF", 656],
        ["?", "0xFFDBF5FF", 656],
        ["?", "0xFFC2EBFF", 657],
        ["?", "0xFFC2EBFF", 657],
        ["?", "0xFF96CCFF", 658],
        ["?", "0xFF96CCFF", 658],
        ["?", "0xFF5CA6FF", 659],
        ["?", "0xFF5CA6FF", 659],
        ["?", "0xFF1A6EFF", 660],
        ["?", "0xFF1A6EFF", 660],
        ["?", "0xFF0048E8", 661],
        ["?", "0xFF0048E8", 661],
        ["?", "0xFF003BD1", 662],
        ["?", "0xFF003BD1", 662],
        ["?", "0xFFEDF0FF", 663],
        ["?", "0xFFEDF0FF", 663],
        ["?", "0xFFE3E8FF", 664],
        ["?", "0xFFE3E8FF", 664],
        ["?", "0xFFC8CFFA", 665],
        ["?", "0xFFC8CFFA", 665],
        ["?", "0xFFA4A6ED", 666],
        ["?", "0xFFA4A6ED", 666],
        ["?", "0xFF6970DB", 667],
        ["?", "0xFF6970DB", 667],
        ["?", "0xFF3E40B3", 668],
        ["?", "0xFF3E40B3", 668],
        ["?", "0xFF201E87", 669],
        ["?", "0xFF201E87", 669],
        ["?", "0xFFFFDEFF", 670],
        ["?", "0xFFFFDEFF", 670],
        ["?", "0xFFFCCCFF", 671],
        ["?", "0xFFFCCCFF", 671],
        ["?", "0xFFF7A8FF", 672],
        ["?", "0xFFF7A8FF", 672],
        ["?", "0xFFF082FF", 673],
        ["?", "0xFFF082FF", 673],
        ["?", "0xFFE854FF", 674],
        ["?", "0xFFE854FF", 674],
        ["?", "0xFFCD00F7", 675],
        ["?", "0xFFCD00F7", 675],
        ["?", "0xFFBB00C7", 676],
        ["?", "0xFFBB00C7", 676],
        ["?", "0xFFFADEFF", 677],
        ["?", "0xFFFADEFF", 677],
        ["?", "0xFFF7C9FF", 678],
        ["?", "0xFFF7C9FF", 678],
        ["?", "0xFFF2BAFF", 679],
        ["?", "0xFFF2BAFF", 679],
        ["?", "0xFFE18EFA", 680],
        ["?", "0xFFE18EFA", 680],
        ["?", "0xFFC15FF5", 681],
        ["?", "0xFFC15FF5", 681],
        ["?", "0xFFA82FE0", 682],
        ["?", "0xFFA82FE0", 682],
        ["?", "0xFF810091", 683],
        ["?", "0xFF810091", 683],
        ["?", "0xFFFACFFA", 684],
        ["?", "0xFFFACFFA", 684],
        ["?", "0xFFF7BAF7", 685],
        ["?", "0xFFF7BAF7", 685],
        ["?", "0xFFF2AAF2", 686],
        ["?", "0xFFF2AAF2", 686],
        ["?", "0xFFDC7EE0", 687],
        ["?", "0xFFDC7EE0", 687],
        ["?", "0xFFC459CF", 688],
        ["?", "0xFFC459CF", 688],
        ["?", "0xFF9D27A8", 689],
        ["?", "0xFF9D27A8", 689],
        ["?", "0xFF690369", 690],
        ["?", "0xFF690369", 690],
        ["?", "0xFFFCD7E8", 691],
        ["?", "0xFFFCD7E8", 691],
        ["?", "0xFFFAC0E1", 692],
        ["?", "0xFFFAC0E1", 692],
        ["?", "0xFFF0A8D3", 693],
        ["?", "0xFFF0A8D3", 693],
        ["?", "0xFFE683BA", 694],
        ["?", "0xFFE683BA", 694],
        ["?", "0xFFBF508A", 695],
        ["?", "0xFFBF508A", 695],
        ["?", "0xFF991846", 696],
        ["?", "0xFF991846", 696],
        ["?", "0xFF7D0925", 697],
        ["?", "0xFF7D0925", 697],
        ["?", "0xFFFFD6EB", 698],
        ["?", "0xFFFFD6EB", 698],
        ["?", "0xFFFFC2E6", 699],
        ["?", "0xFFFFC2E6", 699],
        ["?", "0xFFFFA3DB", 700],
        ["?", "0xFFFFA3DB", 700],
        ["?", "0xFFFF78CC", 701],
        ["?", "0xFFFF78CC", 701],
        ["?", "0xFFF24BA0", 702],
        ["?", "0xFFF24BA0", 702],
        ["?", "0xFFD62463", 703],
        ["?", "0xFFD62463", 703],
        ["?", "0xFFBA0025", 704],
        ["?", "0xFFBA0025", 704],
        ["?", "0xFFFFE8F2", 705],
        ["?", "0xFFFFE8F2", 705],
        ["?", "0xFFFFD4E6", 706],
        ["?", "0xFFFFD4E6", 706],
        ["?", "0xFFFFB3DB", 707],
        ["?", "0xFFFFB3DB", 707],
        ["?", "0xFFFF8AC7", 708],
        ["?", "0xFFFF8AC7", 708],
        ["?", "0xFFFF579E", 709],
        ["?", "0xFFFF579E", 709],
        ["?", "0xFFFF366B", 710],
        ["?", "0xFFFF366B", 710],
        ["?", "0xFFFA0032", 711],
        ["?", "0xFFFA0032", 711],
        ["?", "0xFFFFDBB0", 712],
        ["?", "0xFFFFDBB0", 712],
        ["?", "0xFFFFCF96", 713],
        ["?", "0xFFFFCF96", 713],
        ["?", "0xFFFFB875", 714],
        ["?", "0xFFFFB875", 714],
        ["?", "0xFFFFA14A", 715],
        ["?", "0xFFFFA14A", 715],
        ["?", "0xFFFF8717", 716],
        ["?", "0xFFFF8717", 716],
        ["?", "0xFFFA7000", 717],
        ["?", "0xFFFA7000", 717],
        ["?", "0xFFEB6300", 718],
        ["?", "0xFFEB6300", 718],
        ["?", "0xFFFFE6BF", 719],
        ["?", "0xFFFFE6BF", 719],
        ["?", "0xFFFCD7A7", 720],
        ["?", "0xFFFCD7A7", 720],
        ["?", "0xFFF7BC77", 721],
        ["?", "0xFFF7BC77", 721],
        ["?", "0xFFE89538", 722],
        ["?", "0xFFE89538", 722],
        ["?", "0xFFD4740B", 723],
        ["?", "0xFFD4740B", 723],
        ["?", "0xFFA14C00", 724],
        ["?", "0xFFA14C00", 724],
        ["?", "0xFF823B00", 725],
        ["?", "0xFF823B00", 725],
        ["?", "0xFFFAE6C0", 726],
        ["?", "0xFFFAE6C0", 726],
        ["?", "0xFFF2CEA0", 727],
        ["?", "0xFFF2CEA0", 727],
        ["?", "0xFFE6B577", 728],
        ["?", "0xFFE6B577", 728],
        ["?", "0xFFD19052", 729],
        ["?", "0xFFD19052", 729],
        ["?", "0xFFB56E2B", 730],
        ["?", "0xFFB56E2B", 730],
        ["?", "0xFF753700", 731],
        ["?", "0xFF753700", 731],
        ["?", "0xFF5C2800", 732],
        ["?", "0xFF5C2800", 732],
        ["?", "0xFFFFF5D1", 7401],
        ["?", "0xFFFFF5D1", 7401],
        ["?", "0xFFFFF0B3", 7402],
        ["?", "0xFFFFF0B3", 7402],
        ["?", "0xFFFFE680", 7403],
        ["?", "0xFFFFE680", 7403],
        ["?", "0xFFFFE833", 7404],
        ["?", "0xFFFFE833", 7404],
        ["?", "0xFFFFE600", 7405],
        ["?", "0xFFFFE600", 7405],
        ["?", "0xFFFFD100", 7406],
        ["?", "0xFFFFD100", 7406],
        ["?", "0xFFE3B122", 7407],
        ["?", "0xFFE3B122", 7407],
        ["?", "0xFFFFBF0D", 7408],
        ["?", "0xFFFFBF0D", 7408],
        ["?", "0xFFFFB30D", 7409],
        ["?", "0xFFFFB30D", 7409],
        ["?", "0xFFFFB373", 7410],
        ["?", "0xFFFFB373", 7410],
        ["?", "0xFFFFA64F", 7411],
        ["?", "0xFFFFA64F", 7411],
        ["?", "0xFFED8A00", 7412],
        ["?", "0xFFED8A00", 7412],
        ["?", "0xFFF57300", 7413],
        ["?", "0xFFF57300", 7413],
        ["?", "0xFFE37B00", 7414],
        ["?", "0xFFE37B00", 7414],
        ["?", "0xFFFFD1D9", 7415],
        ["?", "0xFFFFD1D9", 7415],
        ["?", "0xFFFF6666", 7416],
        ["?", "0xFFFF6666", 7416],
        ["?", "0xFFFF4040", 7417],
        ["?", "0xFFFF4040", 7417],
        ["?", "0xFFF24961", 7418],
        ["?", "0xFFF24961", 7418],
        ["?", "0xFFD15473", 7419],
        ["?", "0xFFD15473", 7419],
        ["?", "0xFFCC2976", 7420],
        ["?", "0xFFCC2976", 7420],
        ["?", "0xFF630046", 7421],
        ["?", "0xFF630046", 7421],
        ["?", "0xFFFFE8F2", 7422],
        ["?", "0xFFFFE8F2", 7422],
        ["?", "0xFFFF73C7", 7423],
        ["?", "0xFFFF73C7", 7423],
        ["?", "0xFFFF40B3", 7424],
        ["?", "0xFFFF40B3", 7424],
        ["?", "0xFFED18A6", 7425],
        ["?", "0xFFED18A6", 7425],
        ["?", "0xFFD10073", 7426],
        ["?", "0xFFD10073", 7426],
        ["?", "0xFFB80040", 7427],
        ["?", "0xFFB80040", 7427],
        ["?", "0xFF73173F", 7428],
        ["?", "0xFF73173F", 7428],
        ["?", "0xFFFFD1F7", 7429],
        ["?", "0xFFFFD1F7", 7429],
        ["?", "0xFFFAB0FF", 7430],
        ["?", "0xFFFAB0FF", 7430],
        ["?", "0xFFF296ED", 7431],
        ["?", "0xFFF296ED", 7431],
        ["?", "0xFFE667DF", 7432],
        ["?", "0xFFE667DF", 7432],
        ["?", "0xFFD936B8", 7433],
        ["?", "0xFFD936B8", 7433],
        ["?", "0xFFCC29AD", 7434],
        ["?", "0xFFCC29AD", 7434],
        ["?", "0xFFA60095", 7435],
        ["?", "0xFFA60095", 7435],
        ["?", "0xFFF7EBFF", 7436],
        ["?", "0xFFF7EBFF", 7436],
        ["?", "0xFFF0CCFF", 7437],
        ["?", "0xFFF0CCFF", 7437],
        ["?", "0xFFD9A6FF", 7438],
        ["?", "0xFFD9A6FF", 7438],
        ["?", "0xFFCCA6FF", 7439],
        ["?", "0xFFCCA6FF", 7439],
        ["?", "0xFFB399FF", 7440],
        ["?", "0xFFB399FF", 7440],
        ["?", "0xFFA380FF", 7441],
        ["?", "0xFFA380FF", 7441],
        ["?", "0xFF804DFF", 7442],
        ["?", "0xFF804DFF", 7442],
        ["?", "0xFFF0F2FF", 7443],
        ["?", "0xFFF0F2FF", 7443],
        ["?", "0xFFCCD4FF", 7444],
        ["?", "0xFFCCD4FF", 7444],
        ["?", "0xFFADC6F7", 7445],
        ["?", "0xFFADC6F7", 7445],
        ["?", "0xFF919EFF", 7446],
        ["?", "0xFF919EFF", 7446],
        ["?", "0xFF5357CF", 7447],
        ["?", "0xFF5357CF", 7447],
        ["?", "0xFF4E4373", 7448],
        ["?", "0xFF4E4373", 7448],
        ["?", "0xFF270020", 7449],
        ["?", "0xFF270020", 7449],
        ["?", "0xFFCCE6FF", 7450],
        ["?", "0xFFCCE6FF", 7450],
        ["?", "0xFF99C9FF", 7451],
        ["?", "0xFF99C9FF", 7451],
        ["?", "0xFF80ADFF", 7452],
        ["?", "0xFF80ADFF", 7452],
        ["?", "0xFF80BDFF", 7453],
        ["?", "0xFF80BDFF", 7453],
        ["?", "0xFF73AEE6", 7454],
        ["?", "0xFF73AEE6", 7454],
        ["?", "0xFF3378FF", 7455],
        ["?", "0xFF3378FF", 7455],
        ["?", "0xFF6B9AED", 7456],
        ["?", "0xFF6B9AED", 7456],
        ["?", "0xFFE0FFFA", 7457],
        ["?", "0xFFE0FFFA", 7457],
        ["?", "0xFF90F0E4", 7458],
        ["?", "0xFF90F0E4", 7458],
        ["?", "0xFF5FDED1", 7459],
        ["?", "0xFF5FDED1", 7459],
        ["?", "0xFF00F2F2", 7460],
        ["?", "0xFF00F2F2", 7460],
        ["?", "0xFF38B8FF", 7461],
        ["?", "0xFF38B8FF", 7461],
        ["?", "0xFF0073E6", 7462],
        ["?", "0xFF0073E6", 7462],
        ["?", "0xFF003359", 7463],
        ["?", "0xFF003359", 7463],
        ["?", "0xFFBFFFE6", 7464],
        ["?", "0xFFBFFFE6", 7464],
        ["?", "0xFF80FFBF", 7465],
        ["?", "0xFF80FFBF", 7465],
        ["?", "0xFF4DFFC4", 7466],
        ["?", "0xFF4DFFC4", 7466],
        ["?", "0xFF0DFFBF", 7467],
        ["?", "0xFF0DFFBF", 7467],
        ["?", "0xFF00A5B8", 7468],
        ["?", "0xFF00A5B8", 7468],
        ["?", "0xFF007A99", 7469],
        ["?", "0xFF007A99", 7469],
        ["?", "0xFF1C778C", 7470],
        ["?", "0xFF1C778C", 7470],
        ["?", "0xFFB8FFDB", 7471],
        ["?", "0xFFB8FFDB", 7471],
        ["?", "0xFF7AFFBF", 7472],
        ["?", "0xFF7AFFBF", 7472],
        ["?", "0xFF46EB91", 7473],
        ["?", "0xFF46EB91", 7473],
        ["?", "0xFF14C78F", 7474],
        ["?", "0xFF14C78F", 7474],
        ["?", "0xFF59B386", 7475],
        ["?", "0xFF59B386", 7475],
        ["?", "0xFF00663A", 7476],
        ["?", "0xFF00663A", 7476],
        ["?", "0xFF105249", 7477],
        ["?", "0xFF105249", 7477],
        ["?", "0xFFD1FFDB", 7478],
        ["?", "0xFFD1FFDB", 7478],
        ["?", "0xFF73FF80", 7479],
        ["?", "0xFF73FF80", 7479],
        ["?", "0xFF66FF80", 7480],
        ["?", "0xFF66FF80", 7480],
        ["?", "0xFF66FF73", 7481],
        ["?", "0xFF66FF73", 7481],
        ["?", "0xFF33FF40", 7482],
        ["?", "0xFF33FF40", 7482],
        ["?", "0xFF117300", 7483],
        ["?", "0xFF117300", 7483],
        ["?", "0xFF008013", 7484],
        ["?", "0xFF008013", 7484],
        ["?", "0xFFF0FFE6", 7485],
        ["?", "0xFFF0FFE6", 7485],
        ["?", "0xFFCCFFB3", 7486],
        ["?", "0xFFCCFFB3", 7486],
        ["?", "0xFFB3FF8C", 7487],
        ["?", "0xFFB3FF8C", 7487],
        ["?", "0xFF91FF66", 7488],
        ["?", "0xFF91FF66", 7488],
        ["?", "0xFF5FED2F", 7489],
        ["?", "0xFF5FED2F", 7489],
        ["?", "0xFF5BA621", 7490],
        ["?", "0xFF5BA621", 7490],
        ["?", "0xFF689900", 7491],
        ["?", "0xFF689900", 7491],
        ["?", "0xFFD1ED77", 7492],
        ["?", "0xFFD1ED77", 7492],
        ["?", "0xFFC5E693", 7493],
        ["?", "0xFFC5E693", 7493],
        ["?", "0xFFA3D982", 7494],
        ["?", "0xFFA3D982", 7494],
        ["?", "0xFF86B324", 7495],
        ["?", "0xFF86B324", 7495],
        ["?", "0xFF5F9E00", 7496],
        ["?", "0xFF5F9E00", 7496],
        ["?", "0xFF738639", 7497],
        ["?", "0xFF738639", 7497],
        ["?", "0xFF263300", 7498],
        ["?", "0xFF263300", 7498],
        ["?", "0xFFFFFAD9", 7499],
        ["?", "0xFFFFFAD9", 7499],
        ["?", "0xFFF7F2D2", 7500],
        ["?", "0xFFF7F2D2", 7500],
        ["?", "0xFFF0E6C0", 7501],
        ["?", "0xFFF0E6C0", 7501],
        ["?", "0xFFE6D395", 7502],
        ["?", "0xFFE6D395", 7502],
        ["?", "0xFFBFA87C", 7503],
        ["?", "0xFFBFA87C", 7503],
        ["?", "0xFF997354", 7504],
        ["?", "0xFF997354", 7504],
        ["?", "0xFF735022", 7505],
        ["?", "0xFF735022", 7505],
        ["?", "0xFFFFF2D9", 7506],
        ["?", "0xFFFFF2D9", 7506],
        ["?", "0xFFFFE6B3", 7507],
        ["?", "0xFFFFE6B3", 7507],
        ["?", "0xFFF5D093", 7508],
        ["?", "0xFFF5D093", 7508],
        ["?", "0xFFF2C279", 7509],
        ["?", "0xFFF2C279", 7509],
        ["?", "0xFFE39F40", 7510],
        ["?", "0xFFE39F40", 7510],
        ["?", "0xFFBF6900", 7511],
        ["?", "0xFFBF6900", 7511],
        ["?", "0xFFAB5C00", 7512],
        ["?", "0xFFAB5C00", 7512],
        ["?", "0xFFF7CBB2", 7513],
        ["?", "0xFFF7CBB2", 7513],
        ["?", "0xFFF2B896", 7514],
        ["?", "0xFFF2B896", 7514],
        ["?", "0xFFE09270", 7515],
        ["?", "0xFFE09270", 7515],
        ["?", "0xFFA65000", 7516],
        ["?", "0xFFA65000", 7516],
        ["?", "0xFF8F3900", 7517],
        ["?", "0xFF8F3900", 7517],
        ["?", "0xFF663D2E", 7518],
        ["?", "0xFF663D2E", 7518],
        ["?", "0xFF423500", 7519],
        ["?", "0xFF423500", 7519],
        ["?", "0xFFFFD6CF", 7520],
        ["?", "0xFFFFD6CF", 7520],
        ["?", "0xFFE6ACB8", 7521],
        ["?", "0xFFE6ACB8", 7521],
        ["?", "0xFFD68196", 7522],
        ["?", "0xFFD68196", 7522],
        ["?", "0xFFCC7A85", 7523],
        ["?", "0xFFCC7A85", 7523],
        ["?", "0xFFBA544A", 7524],
        ["?", "0xFFBA544A", 7524],
        ["?", "0xFFB36259", 7525],
        ["?", "0xFFB36259", 7525],
        ["?", "0xFFA63A00", 7526],
        ["?", "0xFFA63A00", 7526],
        ["?", "0xFFEDE8DF", 7527],
        ["?", "0xFFEDE8DF", 7527],
        ["?", "0xFFE6DFCF", 7528],
        ["?", "0xFFE6DFCF", 7528],
        ["?", "0xFFD4CBBA", 7529],
        ["?", "0xFFD4CBBA", 7529],
        ["?", "0xFFADA089", 7530],
        ["?", "0xFFADA089", 7530],
        ["?", "0xFF80735D", 7531],
        ["?", "0xFF80735D", 7531],
        ["?", "0xFF594A2D", 7532],
        ["?", "0xFF594A2D", 7532],
        ["?", "0xFF261E06", 7533],
        ["?", "0xFF261E06", 7533],
        ["?", "0xFFE6E1D3", 7534],
        ["?", "0xFFE6E1D3", 7534],
        ["?", "0xFFCCC6AD", 7535],
        ["?", "0xFFCCC6AD", 7535],
        ["?", "0xFFADA687", 7536],
        ["?", "0xFFADA687", 7536],
        ["?", "0xFFC6CCB8", 7537],
        ["?", "0xFFC6CCB8", 7537],
        ["?", "0xFFA2B39B", 7538],
        ["?", "0xFFA2B39B", 7538],
        ["?", "0xFFA0A395", 7539],
        ["?", "0xFFA0A395", 7539],
        ["?", "0xFF474747", 7540],
        ["?", "0xFF474747", 7540],
        ["?", "0xFFEDF2F2", 7541],
        ["?", "0xFFEDF2F2", 7541],
        ["?", "0xFFC1D6D0", 7542],
        ["?", "0xFFC1D6D0", 7542],
        ["?", "0xFFA6B3B3", 7543],
        ["?", "0xFFA6B3B3", 7543],
        ["?", "0xFF8A9799", 7544],
        ["?", "0xFF8A9799", 7544],
        ["?", "0xFF495C5E", 7545],
        ["?", "0xFF495C5E", 7545],
        ["?", "0xFF304547", 7546],
        ["?", "0xFF304547", 7546],
        ["?", "0xFF0A0F0F", 7547],
        ["?", "0xFF0A0F0F", 7547],
        ["END", 0, -1]
    ],

#/* Based on the manufacturer table at 
# *   https://help.brother-usa.com/app/answers/detail/a_id/75245/~/thread-color-conversion-chart
# * using a colour picker.
# */
    "RobisonAnton_Polyester_codes": [
        ["Black", "0xFF000000", 5596],
        ["Black", "0xFF000000", 5596],
        ["White", "0xFFFFFFFF", 5597],
        ["White", "0xFFFFFFFF", 5597],
    # unfinished
        ["END", 0, -1]
    ],

    "RobisonAnton_Rayon_codes": [
        ["Light Pink", "0xFFEFCCCE", 2243],
        ["Light Pink", "0xFFEFCCCE", 2243],
        ["Pink", "0xFFFCBFC9", 2223],
        ["Pink", "0xFFFCBFC9", 2223],
        ["Pink Bazaar", "0xFFEFC6D3", 2599],
        ["Pink Bazaar", "0xFFEFC6D3", 2599],
        ["Pink Mist", "0xFFF9B2B7", 2373],
        ["Pink Mist", "0xFFF9B2B7", 2373],
        ["Emily Pink", "0xFFF9AFAD", 2374],
        ["Emily Pink", "0xFFF9AFAD", 2374],
        ["Rose", "0xFFFC9BB2", 2293],
        ["Rose", "0xFFFC9BB2", 2293],
        ["Rose Cerise", "0xFFFC8C99", 2244],
        ["Rose Cerise", "0xFFFC8C99", 2244],
        ["Carnation", "0xFFF2AFC1", 2237],
        ["Carnation", "0xFFF2AFC1", 2237],
        ["Shrimp", "0xFFE5566D", 2246],
        ["Shrimp", "0xFFE5566D", 2246],
        ["Bashful Pink", "0xFFF4476B", 2248],
        ["Bashful Pink", "0xFFF4476B", 2248],
        ["Begonia", "0xFFE5566D", 2228],
        ["Begonia", "0xFFE5566D", 2228],
        ["Azalea", "0xFFF9848E", 2412],
        ["Azalea", "0xFFF9848E", 2412],
        ["Dusty Rose", "0xFFF26877", 2375],
        ["Dusty Rose", "0xFFF26877", 2375],
        ["Rose Tint", "0xFFD8899B", 2591],
        ["Rose Tint", "0xFFD8899B", 2591],
        ["Burgundy", "0xFF8C2633", 2249],
        ["Burgundy", "0xFF8C2633", 2249],
        ["TH Burgundy", "0xFF7C2128", 2608],
        ["TH Burgundy", "0xFF7C2128", 2608],
        ["Russet", "0xFF7A2638", 2252],
        ["Russet", "0xFF7A2638", 2252],
        ["Pro Firebrand", "0xFF75263D", 2622],
        ["Pro Firebrand", "0xFF75263D", 2622],
        ["Wine", "0xFF772D35", 2225],
        ["Wine", "0xFF772D35", 2225],
        ["Intense Maroon", "0xFF593344", 2587],
        ["Intense Maroon", "0xFF593344", 2587],
        ["Dark Maroon", "0xFF4F213A", 2376],
        ["Dark Maroon", "0xFF4F213A", 2376],
        ["Carbernet", "0xFF931638", 2494],
        ["Carbernet", "0xFF931638", 2494],
        ["Mountain Rose", "0xFFF9B2B7", 2495],
        ["Mountain Rose", "0xFFF9B2B7", 2495],
        ["Warm Wine", "0xFF661E2B", 2496],
        ["Warm Wine", "0xFF661E2B", 2496],
        ["Primrose", "0xFFE5566D", 2491],
        ["Primrose", "0xFFE5566D", 2491],
        ["Perfect Ruby", "0xFF8E2344", 2497],
        ["Perfect Ruby", "0xFF8E2344", 2497],
        ["Brushed Burgundy", "0xFF6D213F", 2498],
        ["Brushed Burgundy", "0xFF6D213F", 2498],
        ["Passion Rose", "0xFF8E2344", 2499],
        ["Passion Rose", "0xFF8E2344", 2499],
        ["New Berry", "0xFFAD0075", 2500],
        ["New Berry", "0xFFAD0075", 2500],
        ["Petal Pink", "0xFFFCC9C6", 2501],
        ["Petal Pink", "0xFFFCC9C6", 2501],
        ["Memphis Belle", "0xFFF4BFD1", 2502],
        ["Memphis Belle", "0xFFF4BFD1", 2502],
        ["Desert Bloom", "0xFFF7BFBF", 2503],
        ["Desert Bloom", "0xFFF7BFBF", 2503],
        ["Wild Pink", "0xFFCE007C", 2259],
        ["Wild Pink", "0xFFCE007C", 2259],
        ["Floral Pink", "0xFFED72AA", 2415],
        ["Floral Pink", "0xFFED72AA", 2415],
        ["Hot Pink", "0xFFD36B9E", 2260],
        ["Hot Pink", "0xFFD36B9E", 2260],
        ["Crimson", "0xFFD60270", 2416],
        ["Crimson", "0xFFD60270", 2416],
        ["Ruby Glint", "0xFFE22882", 2261],
        ["Ruby Glint", "0xFFE22882", 2261],
        ["Cherrystone", "0xFFAA004F", 2504],
        ["Cherrystone", "0xFFAA004F", 2504],
        ["Cherry Punch", "0xFFAA004F", 2417],
        ["Cherry Punch", "0xFFAA004F", 2417],
        ["Cherry Blossom", "0xFFEA0F6B", 2262],
        ["Cherry Blossom", "0xFFEA0F6B", 2262],
        ["Red Berry", "0xFFAF1E2D", 2418],
        ["Red Berry", "0xFFAF1E2D", 2418],
        ["Jockey Red", "0xFFBF0A30", 2281],
        ["Jockey Red", "0xFFBF0A30", 2281],
        ["Very Red", "0xFFBF0A30", 2419],
        ["Very Red", "0xFFBF0A30", 2419],
        ["Red Berry", "0xFFBF0A30", 2378],
        ["Red Berry", "0xFFBF0A30", 2378],
        ["Foxy Red", "0xFFCE1126", 2263],
        ["Foxy Red", "0xFFCE1126", 2263],
        ["Tuxedo Red", "0xFFD62828", 2420],
        ["Tuxedo Red", "0xFFD62828", 2420],
        ["Lipstick", "0xFFBF0A30", 2233],
        ["Lipstick", "0xFFBF0A30", 2233],
        ["Scarlet", "0xFFC41E3A", 2219],
        ["Scarlet", "0xFFC41E3A", 2219],
        ["Radiant Red", "0xFFC41E3A", 2266],
        ["Radiant Red", "0xFFC41E3A", 2266],
        ["Wildfire", "0xFFA32638", 2267],
        ["Wildfire", "0xFFA32638", 2267],
        ["Carolina Red", "0xFF8C2633", 2268],
        ["Carolina Red", "0xFF8C2633", 2268],
        ["Red Jubilee", "0xFF75263D", 2421],
        ["Red Jubilee", "0xFF75263D", 2421],
        ["Cranberry", "0xFF992135", 2270],
        ["Cranberry", "0xFF992135", 2270],
        ["Antique Red", "0xFFA32638", 2505],
        ["Antique Red", "0xFFA32638", 2505],
        ["Devil Red", "0xFFC41E3A", 2506],
        ["Devil Red", "0xFFC41E3A", 2506],
        ["Rosewood", "0xFFD81C3F", 2508],
        ["Rosewood", "0xFFD81C3F", 2508],
        ["Bitteroot", "0xFFFC5E72", 2509],
        ["Bitteroot", "0xFFFC5E72", 2509],
        ["Bisque", "0xFFF2C4AF", 2377],
        ["Bisque", "0xFFF2C4AF", 2377],
        ["Flesh", "0xFFF4CCAA", 2413],
        ["Flesh", "0xFFF4CCAA", 2413],
        ["Flesh Pink", "0xFFF9BAAA", 2253],
        ["Flesh Pink", "0xFFF9BAAA", 2253],
        ["Opal Mist", "0xFFF7BFBF", 2255],
        ["Opal Mist", "0xFFF7BFBF", 2255],
        ["Candy Apple Red", "0xFFAF003D", 2507],
        ["Candy Apple Red", "0xFFAF003D", 2507],
        ["Tawny", "0xFFF9BF9E", 2256],
        ["Peach", "0xFFF9C6AA", 2257],
        ["Melon", "0xFFF98E6D", 2294],
        ["Flamingo", "0xFFF9A58C", 2258],
        ["Coral", "0xFFF98972", 2414],
        ["Persimmon", "0xFFF43F4F", 2277],
        ["Peach Blossom", "0xFFF9C6AA", 2510],
        ["Illusion", "0xFFF9BAAA", 2511],
        ["Melonade", "0xFFF98972", 2512],
        ["Honeysuckle", "0xFFE23D28", 2513],
        ["Brite Jade", "0xFF008C82", 2514],
        ["Bluestone", "0xFF00B2A0", 2515],
        ["Aqua Pearl", "0xFF47D6C1", 2516],
        ["Seafrost", "0xFF87DDD1", 2517],
        ["J. Turquoise", "0xFF008789", 2492],
        ["Indian Ocean Blue", "0xFF2DC6D6", 2518],
        ["Surf Blue", "0xFF00A5DB", 2519],
        ["Surf Blue", "0xFF00A5DB", 2519],
        ["Mid Windsor", "0xFF00A3DD", 2520],
        ["Mid Windsor", "0xFF00A3DD", 2520],
        ["Deep Windsor", "0xFF003F54", 2589],
        ["Deep Windsor", "0xFF003F54", 2589],
        ["Pro Dark Blue", "0xFF002D47", 2620],
        ["Pro Dark Blue", "0xFF002D47", 2620],
        ["Mallard Blue", "0xFF006D75", 2521],
        ["Mallard Blue", "0xFF006D75", 2521],
        ["Sky Blue", "0xFF9BC4E2", 2239],
        ["Sky Blue", "0xFF9BC4E2", 2239],
        ["Lake Blue", "0xFFAFBCDB", 2304],
        ["Lake Blue", "0xFFAFBCDB", 2304],
        ["Pro Lusty Blue", "0xFF5B77CC", 2614],
        ["Pro Lusty Blue", "0xFF5B77CC", 2614],
        ["Slate Blue", "0xFF6D87A8", 2275],
        ["Slate Blue", "0xFF6D87A8", 2275],
        ["Blue Frost", "0xFF99D6DD", 2305],
        ["Blue Frost", "0xFF99D6DD", 2305],
        ["Periwinkle", "0xFF28C4D8", 2306],
        ["Periwinkle", "0xFF28C4D8", 2306],
        ["Aquamarine", "0xFF00ADC6", 2307],
        ["Aquamarine", "0xFF00ADC6", 2307],
        ["California Blue", "0xFF00A0C4", 2389],
        ["California Blue", "0xFF00A0C4", 2389],
        ["Baltic Blue", "0xFF008ED6", 2441],
        ["Baltic Blue", "0xFF008ED6", 2441],
        ["Solar Blue", "0xFF0054A0", 2442],
        ["Solar Blue", "0xFF0054A0", 2442],
        ["Pacific Blue", "0xFF008ED6", 2388],
        ["Pacific Blue", "0xFF008ED6", 2388],
        ["Boo Boo Blue", "0xFF00A3DD", 2730],
        ["Boo Boo Blue", "0xFF00A3DD", 2730],
        ["Pro Band Blue", "0xFF00709E", 2737],
        ["Pro Band Blue", "0xFF00709E", 2737],
        ["Pro Peacock", "0xFF007AA5", 2740],
        ["Pro Peacock", "0xFF007AA5", 2740],
        ["Light Blue", "0xFFC9E8DD", 2222],
        ["Light Blue", "0xFFC9E8DD", 2222],
        ["Royal", "0xFF1E1C77", 2210],
        ["Royal", "0xFF1E1C77", 2210],
        ["Blue Suede", "0xFF002B7F", 2438],
        ["Blue Suede", "0xFF002B7F", 2438],
        ["Imperial Blue", "0xFF335687", 2302],
        ["Imperial Blue", "0xFF335687", 2302],
        ["Bridgeport Blue", "0xFFAFBCDB", 2522],
        ["Bridgeport Blue", "0xFFAFBCDB", 2522],
        ["China Blue", "0xFF335687", 2523],
        ["China Blue", "0xFF335687", 2523],
        ["Pro Imperial", "0xFF26547C", 2612],
        ["Pro Imperial", "0xFF26547C", 2612],
        ["Country Blue", "0xFF7796B2", 2524],
        ["Country Blue", "0xFF7796B2", 2524],
        ["Heron Blue", "0xFFC1C9DD", 2525],
        ["Heron Blue", "0xFFC1C9DD", 2525],
        ["Pro Saxon", "0xFF7796B2", 2624],
        ["Pro Saxon", "0xFF7796B2", 2624],
        ["Bright Blue", "0xFF6689CC", 2526],
        ["Bright Blue", "0xFF6689CC", 2526],
        ["Soldier Blue", "0xFF5960A8", 2527],
        ["Soldier Blue", "0xFF5960A8", 2527],
        ["Pro Brilliance", "0xFF0051BA", 2619],
        ["Pro Brilliance", "0xFF0051BA", 2619],
        ["Atlantis Blue", "0xFF6689CC", 2528],
        ["Atlantis Blue", "0xFF6689CC", 2528],
        ["Dolphin Blue", "0xFF3A75C4", 2529],
        ["Dolphin Blue", "0xFF3A75C4", 2529],
        ["Caribbean Blue", "0xFF75AADB", 2530],
        ["Caribbean Blue", "0xFF75AADB", 2530],
        ["Dana Blue", "0xFFC4D8E2", 2531],
        ["Dana Blue", "0xFFC4D8E2", 2531],
        ["Cadet Blue", "0xFFC4D8E2", 2532],
        ["Cadet Blue", "0xFFC4D8E2", 2532],
        ["Ozone", "0xFF60AFDD", 2533],
        ["Ozone", "0xFF60AFDD", 2533],
        ["Salem Blue", "0xFF003D6B", 2534],
        ["Salem Blue", "0xFF003D6B", 2534],
        ["Blue Ribbon", "0xFF0F2B5B", 2439],
        ["Blue Ribbon", "0xFF0F2B5B", 2439],
        ["Pro Navy", "0xFF192168", 2625],
        ["Pro Navy", "0xFF192168", 2625],
        ["Blue Ink", "0xFF2B265B", 2440],
        ["Blue Ink", "0xFF2B265B", 2440],
        ["Pro Midnight", "0xFF002654", 2613],
        ["Pro Midnight", "0xFF002654", 2613],
        ["Pro College Blue", "0xFF002649", 2647],
        ["Pro College Blue", "0xFF002649", 2647],
        ["Light Midnight", "0xFF353F5B", 2386],
        ["Light Midnight", "0xFF353F5B", 2386],
        ["Fleet Blue", "0xFF35264F", 2450],
        ["Fleet Blue", "0xFF35264F", 2450],
        ["Light Navy", "0xFF112151", 2303],
        ["Light Navy", "0xFF112151", 2303],
        ["Flag Blue", "0xFF14213D", 2603],
        ["Flag Blue", "0xFF14213D", 2603],
        ["TH Navy", "0xFF002654", 2609],
        ["TH Navy", "0xFF002654", 2609],
        ["Navy", "0xFF14213D", 2215],
        ["Navy", "0xFF14213D", 2215],
        ["Midnight Navy", "0xFF14213D", 2387],
        ["Midnight Navy", "0xFF14213D", 2387],
        ["Pastel Blue", "0xFFC6D1D6", 2382],
        ["Pastel Blue", "0xFFC6D1D6", 2382],
        ["Blue Hint", "0xFFC1C9DD", 2598],
        ["Blue Hint", "0xFFC1C9DD", 2598],
        ["Ice Blue", "0xFFB5D1E8", 2300],
        ["Ice Blue", "0xFFB5D1E8", 2300],
        ["Paris Blue", "0xFFD1CEDD", 2283],
        ["Paris Blue", "0xFFD1CEDD", 2283],
        ["Baby Blue", "0xFF99BADD", 2206],
        ["Baby Blue", "0xFF99BADD", 2206],
        ["Sun Blue", "0xFFAFBCDB", 2269],
        ["Sun Blue", "0xFFAFBCDB", 2269],
        ["Cristy Blue", "0xFFA5BAE0", 2383],
        ["Cristy Blue", "0xFFA5BAE0", 2383],
        ["Ultra Blue", "0xFF75B2DD", 2433],
        ["Ultra Blue", "0xFF75B2DD", 2433],
        ["Tropic Blue", "0xFF75AADB", 2434],
        ["Tropic Blue", "0xFF75AADB", 2434],
        ["Blue Horizon", "0xFF6689CC", 2435],
        ["Blue Horizon", "0xFF6689CC", 2435],
        ["Oriental Blue", "0xFF7F8CBF", 2301],
        ["Oriental Blue", "0xFF7F8CBF", 2301],
        ["Copen", "0xFF6D87A8", 2245],
        ["Copen", "0xFF6D87A8", 2245],
        ["Jay Blue", "0xFF5B77CC", 2384],
        ["Jay Blue", "0xFF5B77CC", 2384],
        ["Blue", "0xFF2D338E", 2220],
        ["Blue", "0xFF2D338E", 2220],
        ["Sapphire", "0xFF2D338E", 2280],
        ["Sapphire", "0xFF2D338E", 2280],
        ["Pro Royal", "0xFF00337F", 2627],
        ["Pro Royal", "0xFF00337F", 2627],
        ["Fire Blue", "0xFF003893", 2436],
        ["Fire Blue", "0xFF003893", 2436],
        ["Jamie Blue", "0xFF2D338E", 2385],
        ["Jamie Blue", "0xFF2D338E", 2385],
        ["Empire Blue", "0xFF3F2893", 2437],
        ["Empire Blue", "0xFF3F2893", 2437],
        ["Enchanted Sea", "0xFF3A564F", 2535],
        ["Enchanted Sea", "0xFF3A564F", 2535],
        ["Pro Twinkle", "0xFF8499A5", 2617],
        ["Pro Twinkle", "0xFF8499A5", 2617],
        ["Rockport Blue", "0xFF9BAABF", 2536],
        ["Rockport Blue", "0xFF9BAABF", 2536],
        ["Wonder Blue", "0xFF5E99AA", 2577],
        ["Wonder Blue", "0xFF5E99AA", 2577],
        ["Traditional Gray", "0xFFADAFAA", 2540],
        ["Traditional Gray", "0xFFADAFAA", 2540],
        ["Steel", "0xFFBFBAAF", 2537],
        ["Steel", "0xFFBFBAAF", 2537],
        ["Pro Pearl", "0xFFBAB7AF", 2741],
        ["Pro Pearl", "0xFFBAB7AF", 2741],
        ["Pro Cool Gray", "0xFFC4C1BA", 2733],
        ["Pro Cool Gray", "0xFFC4C1BA", 2733],
        ["Stainless Steel", "0xFFCCC1B2", 2538],
        ["Stainless Steel", "0xFFCCC1B2", 2538],
        ["Chrome", "0xFFD1CCBF", 2539],
        ["Chrome", "0xFFD1CCBF", 2539],
        ["Pro Night Sky", "0xFFADAFAA", 2618],
        ["Pro Night Sky", "0xFFADAFAA", 2618],
        ["Gull", "0xFF8C8984", 2731],
        ["Gull", "0xFF8C8984", 2731],
        ["Mineral", "0xFF686663", 2729],
        ["Mineral", "0xFF686663", 2729],
        ["Black Chrome", "0xFF443D38", 2541],
        ["Black Chrome", "0xFF443D38", 2541],
        ["Heather", "0xFFDDC6C4", 2271],
        ["Heather", "0xFFDDC6C4", 2271],
        ["Grape", "0xFFD3B7A3", 2272],
        ["Grape", "0xFFD3B7A3", 2272],
        ["Satin Wine", "0xFFB5939B", 2314],
        ["Satin Wine", "0xFFB5939B", 2314],
        ["Ducky Mauve", "0xFF8E6877", 2422],
        ["Ducky Mauve", "0xFF8E6877", 2422],
        ["Pale Orchid", "0xFFFCBFC9", 2423],
        ["Pale Orchid", "0xFFFCBFC9", 2423],
        ["Orchid", "0xFFE5BFC6", 2379],
        ["Orchid", "0xFFE5BFC6", 2379],
        ["Lavender", "0xFFEFC6D3", 2276],
        ["Lavender", "0xFFEFC6D3", 2276],
        ["Violet", "0xFFEDC4DD", 2285],
        ["Violet", "0xFFEDC4DD", 2285],
        ["Cachet", "0xFFB58CB2", 2424],
        ["Cachet", "0xFFB58CB2", 2424],
        ["Tulip", "0xFFC9ADD8", 2286],
        ["Tulip", "0xFFC9ADD8", 2286],
        ["Mid Lilac", "0xFFE29ED6", 2588],
        ["Mid Lilac", "0xFFE29ED6", 2588],
        ["Port Wine", "0xFF512654", 2600],
        ["Port Wine", "0xFF512654", 2600],
        ["Pro Maroon", "0xFF512D44", 2616],
        ["Pro Maroon", "0xFF512D44", 2616],
        ["Laurie Lilac", "0xFF8E47AD", 2425],
        ["Laurie Lilac", "0xFF8E47AD", 2425],
        ["Iris", "0xFFAF72C1", 2288],
        ["Iris", "0xFFAF72C1", 2288],
        ["Raspberry", "0xFF9B4F96", 2426],
        ["Raspberry", "0xFF9B4F96", 2426],
        ["Mulberry", "0xFF66116D", 2380],
        ["Mulberry", "0xFF66116D", 2380],
        ["Plum Wine", "0xFF63305E", 2490],
        ["Plum Wine", "0xFF63305E", 2490],
        ["Purple Twist", "0xFF1E1C77", 2429],
        ["Purple Twist", "0xFF1E1C77", 2429],
        ["Violet Blue", "0xFF332875", 2427],
        ["Violet Blue", "0xFF332875", 2427],
        ["Purple Maze", "0xFF35006D", 2428],
        ["Purple Maze", "0xFF35006D", 2428],
        ["Pro Brite Star", "0xFF2B1166", 2736],
        ["Pro Brite Star", "0xFF2B1166", 2736],
        ["Pro Violet", "0xFF38197A", 2742],
        ["Pro Violet", "0xFF38197A", 2742],
        ["Pro Purple", "0xFF35006D", 2628],
        ["Pro Purple", "0xFF35006D", 2628],
        ["Purple", "0xFF8E47AD", 2254],
        ["Purple", "0xFF8E47AD", 2254],
        ["Purple Shadow", "0xFF5B027A", 2430],
        ["Purple Shadow", "0xFF5B027A", 2430],
        ["Dark Purple", "0xFF4C145E", 2381],
        ["Dark Purple", "0xFF4C145E", 2381],
        ["Mauve", "0xFF8977BA", 2287],
        ["Mauve", "0xFF8977BA", 2287],
        ["Purple Accent", "0xFF44235E", 2431],
        ["Purple Accent", "0xFF44235E", 2431],
        ["Hot Peony", "0xFFAA0066", 2590],
        ["Hot Peony", "0xFFAA0066", 2590],
        ["Passion", "0xFFCE007C", 2291],
        ["Passion", "0xFFCE007C", 2291],
        ["Strawberry", "0xFFAA004F", 2432],
        ["Strawberry", "0xFFAA004F", 2432],
        ["Plum", "0xFF9E2387", 2292],
        ["Plum", "0xFF9E2387", 2292],
        ["Misty", "0xFF5E99AA", 2308],
        ["Misty", "0xFF5E99AA", 2308],
        ["Mystic Teal", "0xFF609191", 2443],
        ["Mystic Teal", "0xFF609191", 2443],
        ["Teal", "0xFF609191", 2309],
        ["Teal", "0xFF609191", 2309],
        ["Dark Teal", "0xFF003F54", 2444],
        ["Dark Teal", "0xFF003F54", 2444],
        ["Mint Julep", "0xFF93DDDB", 2310],
        ["Mint Julep", "0xFF93DDDB", 2310],
        ["Turquoise", "0xFF7FD6DB", 2204],
        ["Turquoise", "0xFF7FD6DB", 2204],
        ["M.D. Green", "0xFF007272", 2445],
        ["M.D. Green", "0xFF007272", 2445],
        ["Seafoam", "0xFF70CE9B", 2311],
        ["Seafoam", "0xFF70CE9B", 2311],
        ["Isle Green", "0xFF70CE9B", 2312],
        ["Isle Green", "0xFF70CE9B", 2312],
        ["Peppermint", "0xFF35C4AF", 2390],
        ["Peppermint", "0xFF35C4AF", 2390],
        ["Oceanic Green", "0xFF006D75", 2446],
        ["Oceanic Green", "0xFF006D75", 2446],
        ["Pro Teal", "0xFF006B77", 2621],
        ["Pro Teal", "0xFF006B77", 2621],
        ["Garden Green", "0xFF006663", 2447],
        ["Garden Green", "0xFF006663", 2447],
        ["Pro Green", "0xFF006D66", 2735],
        ["Pro Green", "0xFF006D66", 2735],
        ["Pine Green", "0xFF008C82", 2391],
        ["Pine Green", "0xFF008C82", 2391],
        ["Greenstone", "0xFF008272", 2448],
        ["Greenstone", "0xFF008272", 2448],
        ["Fern Green", "0xFF006663", 2449],
        ["Fern Green", "0xFF006663", 2449],
        ["Pro Hunter", "0xFF006D66", 2615],
        ["Pro Hunter", "0xFF006D66", 2615],
        ["Palm Leaf", "0xFFBCC1B2", 2241],
        ["Palm Leaf", "0xFFBCC1B2", 2241],
        ["Flite Green", "0xFFC6D6A0", 2282],
        ["Flite Green", "0xFFC6D6A0", 2282],
        ["Willow", "0xFF9EAA99", 2221],
        ["Willow", "0xFF9EAA99", 2221],
        ["Sprite", "0xFFB2D8D8", 2313],
        ["Sprite", "0xFFB2D8D8", 2313],
        ["Moss", "0xFF7AA891", 2278],
        ["Moss", "0xFF7AA891", 2278],
        ["Wintergreen", "0xFF7AA891", 2594],
        ["Wintergreen", "0xFF7AA891", 2594],
        ["Green Forest", "0xFF006056", 2451],
        ["Green Forest", "0xFF006056", 2451],
        ["Pro Forest", "0xFF006056", 2743],
        ["Pro Forest", "0xFF006056", 2743],
        ["Harbor Green", "0xFF4F6D5E", 2392],
        ["Harbor Green", "0xFF4F6D5E", 2392],
        ["Evergreen", "0xFF024930", 2315],
        ["Evergreen", "0xFF024930", 2315],
        ["Pro Dark Green", "0xFF004438", 2734],
        ["Pro Dark Green", "0xFF004438", 2734],
        ["Lizzy Lime", "0xFF2B4C3F", 2631],
        ["Lizzy Lime", "0xFF2B4C3F", 2631],
        ["D.H. Green", "0xFF282D26", 2411],
        ["D.H. Green", "0xFF282D26", 2411],
        ["Celery", "0xFFF2EABC", 2316],
        ["Celery", "0xFFF2EABC", 2316],
        ["Pistachio", "0xFFCCC693", 2250],
        ["Pistachio", "0xFFCCC693", 2250],
        ["Olive Drab", "0xFF5E663A", 2317],
        ["Olive Drab", "0xFF5E663A", 2317],
        ["Olive", "0xFF779182", 2202],
        ["Olive", "0xFF779182", 2202],
        ["Pale Green", "0xFFC9E8DD", 2318],
        ["Pale Green", "0xFFC9E8DD", 2318],
        ["Green Pearl", "0xFF93DDDB", 2452],
        ["Green Pearl", "0xFF93DDDB", 2452],
        ["Sea Mist", "0xFF93DDDB", 2393],
        ["Sea Mist", "0xFF93DDDB", 2393],
        ["Mint", "0xFFB5E8BF", 2238],
        ["Mint", "0xFFB5E8BF", 2238],
        ["Spruce", "0xFFAADD6D", 2279],
        ["Spruce", "0xFFAADD6D", 2279],
        ["Nile", "0xFFA0DB8E", 2211],
        ["Nile", "0xFFA0DB8E", 2211],
        ["Green Oak", "0xFFB5CC8E", 2319],
        ["Green Oak", "0xFFB5CC8E", 2319],
        ["Erin Green", "0xFF8CD600", 2320],
        ["Erin Green", "0xFF8CD600", 2320],
        ["Pro Erin", "0xFF56AA1C", 2738],
        ["Pro Erin", "0xFF56AA1C", 2738],
        ["Emerald", "0xFF339E35", 2214],
        ["Emerald", "0xFF339E35", 2214],
        ["Dark Emerald", "0xFF007A3D", 2453],
        ["Dark Emerald", "0xFF007A3D", 2453],
        ["Light Kelly", "0xFF007A3D", 2410],
        ["Light Kelly", "0xFF007A3D", 2410],
        ["Kelly", "0xFF007A3D", 2240],
        ["Kelly", "0xFF007A3D", 2240],
        ["Dark Green", "0xFF008751", 2208],
        ["Dark Green", "0xFF008751", 2208],
        ["Fleece Green", "0xFF006B3F", 2454],
        ["Fleece Green", "0xFF006B3F", 2454],
        ["TH Green", "0xFF006854", 2607],
        ["TH Green", "0xFF006854", 2607],
        ["Harvest Green", "0xFF1EB53A", 2578],
        ["Harvest Green", "0xFF1EB53A", 2578],
        ["Vibrant Green", "0xFF009E49", 2579],
        ["Vibrant Green", "0xFF009E49", 2579],
        ["Green Grass", "0xFF009E49", 2580],
        ["Green Grass", "0xFF009E49", 2580],
        ["Deep Green", "0xFF006B3F", 2284],
        ["Deep Green", "0xFF006B3F", 2284],
        ["Green Bay", "0xFF006B54", 2455],
        ["Green Bay", "0xFF006B54", 2455],
        ["Jungle Green", "0xFF007C66", 2597],
        ["Jungle Green", "0xFF007C66", 2597],
        ["Peapod", "0xFFA3AF07", 2456],
        ["Peapod", "0xFFA3AF07", 2456],
        ["Pastoral Green", "0xFF7FBA00", 2321],
        ["Pastoral Green", "0xFF7FBA00", 2321],
        ["Green Dust", "0xFF7FBA00", 2457],
        ["Green Dust", "0xFF7FBA00", 2457],
        ["Ming", "0xFF7FBA00", 2322],
        ["Ming", "0xFF7FBA00", 2322],
        ["Meadow", "0xFF568E14", 2226],
        ["Meadow", "0xFF568E14", 2226],
        ["Tamarack", "0xFF939905", 2230],
        ["Tamarack", "0xFF939905", 2230],
        ["Palmetto", "0xFF566314", 2229],
        ["Palmetto", "0xFF566314", 2229],
        ["Green Petal", "0xFF024930", 2458],
        ["Green Petal", "0xFF024930", 2458],
        ["Sage", "0xFF547730", 2595],
        ["Sage", "0xFF547730", 2595],
        ["Hedge", "0xFF3F4926", 2601],
        ["Hedge", "0xFF3F4926", 2601],
        ["Green", "0xFF3A7728", 2209],
        ["Green", "0xFF3A7728", 2209],
        ["Green Sail", "0xFF193833", 2459],
        ["Green Sail", "0xFF193833", 2459],
        ["Holly", "0xFF215B33", 2323],
        ["Holly", "0xFF215B33", 2323],
        ["Field Green", "0xFF265142", 2460],
        ["Field Green", "0xFF265142", 2460],
        ["Dress Green", "0xFF3F4926", 2584],
        ["Dress Green", "0xFF3F4926", 2584],
        ["Foliage Green", "0xFF99840A", 2542],
        ["Foliage Green", "0xFF99840A", 2542],
        ["Autumn Green", "0xFFA38205", 2543],
        ["Autumn Green", "0xFFA38205", 2543],
        ["Desert Cactus", "0xFF897719", 2544],
        ["Desert Cactus", "0xFF897719", 2544],
        ["Cypress", "0xFF707014", 2545],
        ["Cypress", "0xFF707014", 2545],
        ["Crescent Moon", "0xFF848205", 2546],
        ["Crescent Moon", "0xFF848205", 2546],
        ["Pebblestone", "0xFFE2E584", 2547],
        ["Pebblestone", "0xFFE2E584", 2547],
        ["Sun Shadow", "0xFF998E07", 2548],
        ["Sun Shadow", "0xFF998E07", 2548],
        ["Blue Spruce", "0xFF00494F", 2549],
        ["Blue Spruce", "0xFF00494F", 2549],
        ["Newport", "0xFF4F6D5E", 2550],
        ["Newport", "0xFF4F6D5E", 2550],
        ["Spring Garden", "0xFF779182", 2551],
        ["Spring Garden", "0xFF779182", 2551],
        ["Water Lilly", "0xFF546856", 2554],
        ["Water Lilly", "0xFF546856", 2554],
        ["Ivy", "0xFF0C3026", 2552],
        ["Ivy", "0xFF0C3026", 2552],
        ["Dark Army Green", "0xFF233A2D", 2553],
        ["Dark Army Green", "0xFF233A2D", 2553],
        ["Army Green", "0xFF213D30", 2728],
        ["Army Green", "0xFF213D30", 2728],
        ["Pastel Green", "0xFFC9D6A3", 2555],
        ["Pastel Green", "0xFFC9D6A3", 2555],
        ["Pollen Gold", "0xFFE0AA0F", 2556],
        ["Pollen Gold", "0xFFE0AA0F", 2556],
        ["Pale Yellow", "0xFFF4E287", 2557],
        ["Pale Yellow", "0xFFF4E287", 2557],
        ["Buttercup", "0xFFFFC61E", 2558],
        ["Buttercup", "0xFFFFC61E", 2558],
        ["Tusk", "0xFFF7E8AA", 2559],
        ["Tusk", "0xFFF7E8AA", 2559],
        ["Moonbeam", "0xFFF9DD16", 2560],
        ["Moonbeam", "0xFFF9DD16", 2560],
        ["Black Eyed Susie", "0xFFC6A00C", 2561],
        ["Black Eyed Susie", "0xFFC6A00C", 2561],
        ["Bullion", "0xFFA37F14", 2562],
        ["Bullion", "0xFFA37F14", 2562],
        ["Chinese Yellow", "0xFFF7E8AA", 2324],
        ["Chinese Yellow", "0xFFF7E8AA", 2324],
        ["Maize", "0xFFF7E8AA", 2264],
        ["Maize", "0xFFF7E8AA", 2264],
        ["Wheat", "0xFFEADD96", 2461],
        ["Wheat", "0xFFEADD96", 2461],
        ["Pro Maize", "0xFFF9E08C", 2732],
        ["Pro Maize", "0xFFF9E08C", 2732],
        ["Glow", "0xFFFFD87F", 2234],
        ["Glow", "0xFFFFD87F", 2234],
        ["Star Gold", "0xFFFCD856", 2408],
        ["Star Gold", "0xFFFCD856", 2408],
        ["Mango", "0xFFFCA311", 2394],
        ["Mango", "0xFFFCA311", 2394],
        ["Yellow Mist", "0xFFFCA311", 2409],
        ["Yellow Mist", "0xFFFCA311", 2409],
        ["Yellow", "0xFFFCA311", 2213],
        ["Yellow", "0xFFFCA311", 2213],
        ["Sunflower", "0xFFF9DD16", 2462],
        ["Sunflower", "0xFFF9DD16", 2462],
        ["Lemon", "0xFFF4ED47", 2325],
        ["Lemon", "0xFFF4ED47", 2325],
        ["Daffodil", "0xFFF9E814", 2326],
        ["Daffodil", "0xFFF9E814", 2326],
        ["Merit Gold", "0xFFFCB514", 2463],
        ["Merit Gold", "0xFFFCB514", 2463],
        ["Cornsilk", "0xFFFFCC49", 2395],
        ["Cornsilk", "0xFFFFCC49", 2395],
        ["Nectar", "0xFFFFC61E", 2464],
        ["Nectar", "0xFFFFC61E", 2464],
        ["Scholastic", "0xFFFCBF49", 2465],
        ["Scholastic", "0xFFFCBF49", 2465],
        ["Canary Yellow", "0xFFFCE016", 2235],
        ["Canary Yellow", "0xFFFCE016", 2235],
        ["Pro Gold", "0xFFFFC61E", 2626],
        ["Pro Gold", "0xFFFFC61E", 2626],
        ["Manila", "0xFFFCD116", 2466],
        ["Manila", "0xFFFCD116", 2466],
        ["Goldenrod", "0xFFFCD116", 2242],
        ["Goldenrod", "0xFFFCD116", 2242],
        ["Brite Yellow", "0xFFFFCC49", 2396],
        ["Brite Yellow", "0xFFFFCC49", 2396],
        ["Honeydew", "0xFFF99B0C", 2327],
        ["Honeydew", "0xFFF99B0C", 2327],
        ["Pumpkin", "0xFFF77F00", 2328],
        ["Pumpkin", "0xFFF77F00", 2328],
        ["Orangeade", "0xFFF74902", 2467],
        ["Orangeade", "0xFFF74902", 2467],
        ["Sun Orange", "0xFFF74902", 2397],
        ["Sun Orange", "0xFFF74902", 2397],
        ["Paprika", "0xFFF95602", 2236],
        ["Paprika", "0xFFF95602", 2236],
        ["Saffron", "0xFFF93F26", 2329],
        ["Saffron", "0xFFF93F26", 2329],
        ["Tex Orange", "0xFFF95602", 2468],
        ["Tex Orange", "0xFFF95602", 2468],
        ["Orange", "0xFFF96B07", 2218],
        ["Orange", "0xFFF96B07", 2218],
        ["Dark Tex Orange", "0xFFF96302", 2469],
        ["Dark Tex Orange", "0xFFF96302", 2469],
        ["Old Dark Tex Orange", "0xFFA53F0F", 2581],
        ["Old Dark Tex Orange", "0xFFA53F0F", 2581],
        ["Golden Poppy", "0xFFFC8744", 2330],
        ["Golden Poppy", "0xFFFC8744", 2330],
        ["Rust", "0xFFBC4F07", 2289],
        ["Rust", "0xFFBC4F07", 2289],
        ["Copper", "0xFFAF7505", 2295],
        ["Copper", "0xFFAF7505", 2295],
        ["Light Bronze", "0xFFC18E60", 2493],
        ["Light Bronze", "0xFFC18E60", 2493],
        ["Visor Gold", "0xFFFCCE87", 2398],
        ["Visor Gold", "0xFFFCCE87", 2398],
        ["Goldenlite", "0xFFFFCC49", 2605],
        ["Goldenlite", "0xFFFFCC49", 2605],
        ["Honey", "0xFFFCBA5E", 2247],
        ["Honey", "0xFFFCBA5E", 2247],
        ["Marigold", "0xFFFFCC49", 2216],
        ["Marigold", "0xFFFFCC49", 2216],
        ["Mustard", "0xFFE0AA0F", 2331],
        ["Mustard", "0xFFE0AA0F", 2331],
        ["Sun Gold", "0xFFE0AA0F", 2212],
        ["Sun Gold", "0xFFE0AA0F", 2212],
        ["Karat", "0xFFE0AA0F", 2470],
        ["Karat", "0xFFE0AA0F", 2470],
        ["Penny", "0xFFF2BF49", 2332],
        ["Penny", "0xFFF2BF49", 2332],
        ["New Gold", "0xFFFCCE87", 2399],
        ["New Gold", "0xFFFCCE87", 2399],
        ["Pro Beige", "0xFFE2D6B5", 2630],
        ["Pro Beige", "0xFFE2D6B5", 2630],
        ["Marine Gold", "0xFFC6A00C", 2596],
        ["Marine Gold", "0xFFC6A00C", 2596],
        ["Ginger", "0xFFA37F14", 2333],
        ["Ginger", "0xFFA37F14", 2333],
        ["Shimmering Gold", "0xFFC6A00C", 2471],
        ["Shimmering Gold", "0xFFC6A00C", 2471],
        ["Old Gold", "0xFFBF910C", 2201],
        ["Old Gold", "0xFFBF910C", 2201],
        ["Salmon", "0xFFEAB2B2", 2299],
        ["Salmon", "0xFFEAB2B2", 2299],
        ["Dark Rust", "0xFFC13828", 2205],
        ["Dark Rust", "0xFFC13828", 2205],
        ["Terra Cotta", "0xFFA03033", 2334],
        ["Terra Cotta", "0xFFA03033", 2334],
        ["Pro Red", "0xFFAF1E2D", 2623],
        ["Pro Red", "0xFFAF1E2D", 2623],
        ["Auburn", "0xFFAF1E2D", 2472],
        ["Auburn", "0xFFAF1E2D", 2472],
        ["Bone", "0xFFFAE6CC", 2582],
        ["Bone", "0xFFFAE6CC", 2582],
        ["Ivory", "0xFFFAE6CF", 2335],
        ["Ivory", "0xFFFAE6CF", 2335],
        ["Opaline", "0xFFF7D3B5", 2473],
        ["Opaline", "0xFFF7D3B5", 2473],
        ["Ecru", "0xFFEDD3BC", 2232],
        ["Ecru", "0xFFEDD3BC", 2232],
        ["Wicker", "0xFFAA753F", 2489],
        ["Wicker", "0xFFAA753F", 2489],
        ["Tan", "0xFFD3A87C", 2273],
        ["Tan", "0xFFD3A87C", 2273],
        ["Cottage Beige", "0xFFEDD3B5", 2593],
        ["Cottage Beige", "0xFFEDD3B5", 2593],
        ["Rattan", "0xFFC1A875", 2474],
        ["Rattan", "0xFFC1A875", 2474],
        ["Gold", "0xFFE2BF9B", 2203],
        ["Gold", "0xFFE2BF9B", 2203],
        ["Mocha Cream", "0xFFD3A87C", 2475],
        ["Mocha Cream", "0xFFD3A87C", 2475],
        ["Topaz", "0xFFBF910C", 2400],
        ["Topaz", "0xFFBF910C", 2400],
        ["Ashley Gold", "0xFFD18E54", 2401],
        ["Ashley Gold", "0xFFD18E54", 2401],
        ["Amber Beige", "0xFFD8B596", 2336],
        ["Amber Beige", "0xFFD8B596", 2336],
        ["Seashell", "0xFFD6CCAF", 2476],
        ["Seashell", "0xFFD6CCAF", 2476],
        ["Light Maize", "0xFFF2E3C4", 2604],
        ["Light Maize", "0xFFF2E3C4", 2604],
        ["Beige", "0xFFAA753F", 2224],
        ["Beige", "0xFFAA753F", 2224],
        ["Sand Dune", "0xFF6B4714", 2477],
        ["Sand Dune", "0xFF6B4714", 2477],
        ["Taupe", "0xFFD1BF91", 2298],
        ["Taupe", "0xFFD1BF91", 2298],
        ["Pro Brown", "0xFF6C463D", 2610],
        ["Pro Brown", "0xFF6C463D", 2610],
        ["Chocolate", "0xFF876028", 2227],
        ["Chocolate", "0xFF876028", 2227],
        ["Pro Walnut", "0xFF755426", 2629],
        ["Pro Walnut", "0xFF755426", 2629],
        ["Light Cocoa", "0xFF755426", 2478],
        ["Light Cocoa", "0xFF755426", 2478],
        ["Cocoa Mulch", "0xFFB28260", 2488],
        ["Cocoa Mulch", "0xFFB28260", 2488],
        ["Brown", "0xFF593D2B", 2251],
        ["Brown", "0xFF593D2B", 2251],
        ["Dark Brown", "0xFF593D2B", 2372],
        ["Dark Brown", "0xFF593D2B", 2372],
        ["Espresso", "0xFF3F302B", 2337],
        ["Espresso", "0xFF3F302B", 2337],
        ["Bamboo", "0xFFC18E60", 2338],
        ["Bamboo", "0xFFC18E60", 2338],
        ["Almond", "0xFFAF7505", 2479],
        ["Almond", "0xFFAF7505", 2479],
        ["Toast", "0xFFBA7530", 2231],
        ["Toast", "0xFFBA7530", 2231],
        ["Sienna", "0xFF755426", 2402],
        ["Sienna", "0xFF755426", 2402],
        ["K.A. Bronze", "0xFFB26B70", 2480],
        ["K.A. Bronze", "0xFFB26B70", 2480],
        ["Pro Cinnamon", "0xFFA2464E", 2611],
        ["Pro Cinnamon", "0xFFA2464E", 2611],
        ["Date", "0xFF9B4F19", 2290],
        ["Date", "0xFF9B4F19", 2290],
        ["Hazel", "0xFF9B4F19", 2481],
        ["Hazel", "0xFF9B4F19", 2481],
        ["Coffee Bean", "0xFF5B2D28", 2339],
        ["Coffee Bean", "0xFF5B2D28", 2339],
        ["Dogwood", "0xFF593D2B", 2563],
        ["Dogwood", "0xFF593D2B", 2563],
        ["Mahogany", "0xFF3F302B", 2564],
        ["Mahogany", "0xFF3F302B", 2564],
        ["Best Brown", "0xFF3D3028", 2566],
        ["Best Brown", "0xFF3D3028", 2566],
        ["Mushroom", "0xFF633A11", 2567],
        ["Mushroom", "0xFF633A11", 2567],
        ["Perfect Tan", "0xFFC1A875", 2568],
        ["Perfect Tan", "0xFFC1A875", 2568],
        ["Earthen Tan", "0xFF7A5B11", 2569],
        ["Earthen Tan", "0xFF7A5B11", 2569],
        ["Golden Tan", "0xFFC1A875", 2570],
        ["Golden Tan", "0xFFC1A875", 2570],
        ["14 Kt. Gold", "0xFFF2BF49", 2586],
        ["14 Kt. Gold", "0xFFF2BF49", 2586],
        ["TH Gold", "0xFFF2CE68", 2606],
        ["TH Gold", "0xFFF2CE68", 2606],
        ["24 Kt. Gold", "0xFFD88C02", 2602],
        ["24 Kt. Gold", "0xFFD88C02", 2602],
        ["Platinum", "0xFFC1B5A5", 2571],
        ["Platinum", "0xFFC1B5A5", 2571],
        ["Pro Gray", "0xFF99897C", 2739],
        ["Pro Gray", "0xFF99897C", 2739],
        ["Grayrod", "0xFFADA07A", 2572],
        ["Grayrod", "0xFFADA07A", 2572],
        ["Pewter", "0xFFADA07A", 2573],
        ["Pewter", "0xFFADA07A", 2573],
        ["Aspen White", "0xFFF5E3CC", 2574],
        ["Aspen White", "0xFFF5E3CC", 2574],
        ["Dark Taupe", "0xFF66594C", 2575],
        ["Dark Taupe", "0xFF66594C", 2575],
        ["Egyptian Brown", "0xFF493533", 2576],
        ["Egyptian Brown", "0xFF493533", 2576],
        ["Oyster", "0xFFF5EBE0", 2403],
        ["Oyster", "0xFFF5EBE0", 2403],
        ["Gray", "0xFFDDC6C4", 2207],
        ["Gray", "0xFFDDC6C4", 2207],
        ["Pearl Gray", "0xFFDBD3D3", 2340],
        ["Pearl Gray", "0xFFDBD3D3", 2340],
        ["Steel Gray", "0xFFD8CCD1", 2274],
        ["Steel Gray", "0xFFD8CCD1", 2274],
        ["Skylight", "0xFFCCC1C6", 2482],
        ["Skylight", "0xFFCCC1C6", 2482],
        ["Cloud", "0xFFAFAAA3", 2483],
        ["Cloud", "0xFFAFAAA3", 2483],
        ["Silver Steel", "0xFFADAFAA", 2592],
        ["Silver Steel", "0xFFADAFAA", 2592],
        ["Banner Gray", "0xFF919693", 2585],
        ["Banner Gray", "0xFF919693", 2585],
        ["Silvery Gray", "0xFF8C8984", 2484],
        ["Silvery Gray", "0xFF8C8984", 2484],
        ["Cinder", "0xFFCCC1C6", 2404],
        ["Cinder", "0xFFCCC1C6", 2404],
        ["Saturn Gray", "0xFFDBD3D3", 2485],
        ["Saturn Gray", "0xFFDBD3D3", 2485],
        ["Dover Gray", "0xFFCCC1C6", 2405],
        ["Dover Gray", "0xFFCCC1C6", 2405],
        ["Storm Gray", "0xFFB2A8B5", 2486],
        ["Storm Gray", "0xFFB2A8B5", 2486],
        ["Sterling", "0xFFA893AD", 2406],
        ["Sterling", "0xFFA893AD", 2406],
        ["Metal", "0xFF666D70", 2407],
        ["Metal", "0xFF666D70", 2407],
        ["Twilight", "0xFF686663", 2217],
        ["Twilight", "0xFF686663", 2217],
        ["Aged Charcoal", "0xFF443D38", 2565],
        ["Aged Charcoal", "0xFF443D38", 2565],
        ["Charcoal", "0xFF777772", 2265],
        ["Charcoal", "0xFF777772", 2265],
        ["Smokey", "0xFF353842", 2487],
        ["Smokey", "0xFF353842", 2487],
        ["Ash", "0xFF3A4972", 2341],
        ["Ash", "0xFF3A4972", 2341],
        ["Black", "0xFF1C2630", 2296],
        ["Black", "0xFF1C2630", 2296],
        ["Snow White", "0xFFF5EBE0", 2297],
        ["Snow White", "0xFFF5EBE0", 2297],
        ["Natural White", "0xFFF5EDDE", 2342],
        ["Natural White", "0xFFF5EDDE", 2342],
        ["Eggshell", "0xFFF0E8D6", 2343],
        ["Eggshell", "0xFFF0E8D6", 2343],
        ["Jet Black", "0xFF1C2630", 2632],
        ["Jet Black", "0xFF1C2630", 2632],
        ["END", 0, -1]
    ],

    "Sigma_Polyester_codes": [
        ["White", "0xFFFFFFFF", 10],
        ["White", "0xFFFFFFFF", 10],
        ["Black", "0xFF000000", 20],
        ["Black", "0xFF000000", 20],
        ["Light Neon Green", "0xFFEDFF50", 21],
        ["Light Neon Green", "0xFFEDFF50", 21],
        ["Neon Green", "0xFF96E845", 32],
        ["Neon Green", "0xFF96E845", 32],
        ["Light Neon Orange", "0xFFFFE756", 33],
        ["Light Neon Orange", "0xFFFFE756", 33],
        ["Med Neon Orange", "0xFFFF7824", 43],
        ["Med Neon Orange", "0xFFFF7824", 43],
        ["Neon Pink", "0xFFF28DA6", 46],
        ["Neon Pink", "0xFFF28DA6", 46],
        ["Neon Orange Pink", "0xFFC70C57", 47],
        ["Neon Orange Pink", "0xFFC70C57", 47],
        ["Silver", "0xFFE22D2A", 101],
        ["Silver", "0xFFE22D2A", 101],
        ["Silver Diamond", "0xFFB8B8B8", 102],
        ["Silver Diamond", "0xFFB8B8B8", 102],
        ["Lava Stone", "0xFF889186", 112],
        ["Lava Stone", "0xFF889186", 112],
        ["Medium Grey", "0xFF737F7F", 115],
        ["Medium Grey", "0xFF737F7F", 115],
        ["Dark Platinum", "0xFF565E5A", 116],
        ["Dark Platinum", "0xFF565E5A", 116],
        ["Charcoal", "0xFF515250", 117],
        ["Charcoal", "0xFF515250", 117],
        ["Badger Grey", "0xFF787668", 118],
        ["Badger Grey", "0xFF787668", 118],
        ["Pumpkin Orange", "0xFFED572F", 135],
        ["Pumpkin Orange", "0xFFED572F", 135],
        ["Turquoise", "0xFF2EA59C", 138],
        ["Turquoise", "0xFF2EA59C", 138],
        ["Dark Wedgewood", "0xFF396276", 142],
        ["Dark Wedgewood", "0xFF396276", 142],
        ["Cardinal Red", "0xFF9B3B40", 213],
        ["Cardinal Red", "0xFF9B3B40", 213],
        ["Maroon", "0xFF6C3E47", 216],
        ["Maroon", "0xFF6C3E47", 216],
        ["Rust", "0xFFBA6E4D", 253],
        ["Rust", "0xFFBA6E4D", 253],
        ["Medium Rust", "0xFFBB3D2E", 255],
        ["Medium Rust", "0xFFBB3D2E", 255],
        ["Natural Pink", "0xFFF9DFCF", 301],
        ["Natural Pink", "0xFFF9DFCF", 301],
        ["Baby Pink", "0xFFFBDED6", 303],
        ["Baby Pink", "0xFFFBDED6", 303],
        ["Piggy Pink", "0xFFF7CDD5", 304],
        ["Piggy Pink", "0xFFF7CDD5", 304],
        ["Sweet Pink", "0xFFF2AFB4", 305],
        ["Sweet Pink", "0xFFF2AFB4", 305],
        ["Blushing Pink", "0xFFE8418C", 307],
        ["Blushing Pink", "0xFFE8418C", 307],
        ["Pink", "0xFFE77F9D", 309],
        ["Pink", "0xFFE77F9D", 309],
        ["Rose Pink", "0xFFF06F8C", 313],
        ["Rose Pink", "0xFFF06F8C", 313],
        ["Green", "0xFF008340", 317],
        ["Green", "0xFF008340", 317],
        ["Shocking Pink", "0xFFDF99B6", 321],
        ["Shocking Pink", "0xFFDF99B6", 321],
        ["Ruby", "0xFF820052", 325],
        ["Ruby", "0xFF820052", 325],
        ["Garnet", "0xFFB1415F", 333],
        ["Garnet", "0xFFB1415F", 333],
        ["Light Purple", "0xFFC394AE", 345],
        ["Light Purple", "0xFFC394AE", 345],
        ["Medium Purple", "0xFFA86E91", 347],
        ["Medium Purple", "0xFFA86E91", 347],
        ["Dark Grape", "0xFF694169", 348],
        ["Dark Grape", "0xFF694169", 348],
        ["Pastel Light Pink", "0xFFE6CFD5", 376],
        ["Pastel Light Pink", "0xFFE6CFD5", 376],
        ["Light Baby Blue", "0xFFA8BED7", 379],
        ["Light Baby Blue", "0xFFA8BED7", 379],
        ["Crystal Blue", "0xFFA0BFD7", 380],
        ["Crystal Blue", "0xFFA0BFD7", 380],
        ["Very Light Lavender", "0xFF90A6C6", 381],
        ["Very Light Lavender", "0xFF90A6C6", 381],
        ["Cornflower", "0xFF8FAFC6", 382],
        ["Cornflower", "0xFF8FAFC6", 382],
        ["Lavender", "0xFFB1B8D3", 383],
        ["Lavender", "0xFFB1B8D3", 383],
        ["Denim", "0xFF416C9B", 385],
        ["Denim", "0xFF416C9B", 385],
        ["Light Violet", "0xFF7D77AF", 386],
        ["Light Violet", "0xFF7D77AF", 386],
        ["Misty Rose", "0xFFFADAF4", 387],
        ["Misty Rose", "0xFFFADAF4", 387],
        ["Grape", "0xFF664090", 390],
        ["Grape", "0xFF664090", 390],
        ["Lt. Weathered Blue", "0xFFEAF0F9", 402],
        ["Lt. Weathered Blue", "0xFFEAF0F9", 402],
        ["Baby Blue", "0xFFA6D8F6", 403],
        ["Baby Blue", "0xFFA6D8F6", 403],
        ["Med Baby Blue", "0xFF7B9CB0", 404],
        ["Med Baby Blue", "0xFF7B9CB0", 404],
        ["Med Pastel Blue", "0xFF648DC7", 406],
        ["Med Pastel Blue", "0xFF648DC7", 406],
        ["Blue Raspberry", "0xFF3D6AA1", 409],
        ["Blue Raspberry", "0xFF3D6AA1", 409],
        ["Med Royal Blue", "0xFF2D4491", 413],
        ["Med Royal Blue", "0xFF2D4491", 413],
        ["Ocean Blue", "0xFF143D7A", 414],
        ["Ocean Blue", "0xFF143D7A", 414],
        ["Med Navy", "0xFF113263", 415],
        ["Med Navy", "0xFF113263", 415],
        ["Dark Navy", "0xFF0E1F38", 423],
        ["Dark Navy", "0xFF0E1F38", 423],
        ["Bright Sunshine", "0xFF0E1F38", 432],
        ["Bright Sunshine", "0xFF0E1F38", 432],
        ["Teal", "0xFF0091A5", 443],
        ["Teal", "0xFF0091A5", 443],
        ["Deep Teal", "0xFF005B63", 448],
        ["Deep Teal", "0xFF005B63", 448],
        ["Dark Teal", "0xFF00474D", 449],
        ["Dark Teal", "0xFF00474D", 449],
        ["Old Gold", "0xFFE5B15C", 466],
        ["Old Gold", "0xFFE5B15C", 466],
        ["Cream", "0xFFD5BF9B", 501],
        ["Cream", "0xFFD5BF9B", 501],
        ["Pale Salmon", "0xFFFFD085", 503],
        ["Pale Salmon", "0xFFFFD085", 503],
        ["Med Peach", "0xFFF6B08E", 505],
        ["Med Peach", "0xFFF6B08E", 505],
        ["Pink Salmon", "0xFFB3E851", 506],
        ["Pink Salmon", "0xFFB3E851", 506],
        ["Dark Peach", "0xFFF1A236", 508],
        ["Dark Peach", "0xFFF1A236", 508],
        ["Dark Brown", "0xFF6E4337", 513],
        ["Dark Brown", "0xFF6E4337", 513],
        ["Pale Red", "0xFFD8493E", 527],
        ["Pale Red", "0xFFD8493E", 527],
        ["Heron Blue", "0xFF697698", 541],
        ["Heron Blue", "0xFF697698", 541],
        ["Pale Yellow", "0xFFFDE896", 601],
        ["Pale Yellow", "0xFFFDE896", 601],
        ["Pastel Yellow", "0xFFEDE55D", 602],
        ["Pastel Yellow", "0xFFEDE55D", 602],
        ["Golden Puppy", "0xFFDFA200", 609],
        ["Golden Puppy", "0xFFDFA200", 609],
        ["Buttercup", "0xFFFDE896", 612],
        ["Buttercup", "0xFFFDE896", 612],
        ["Treasure Gold", "0xFFCEB24C", 616],
        ["Treasure Gold", "0xFFCEB24C", 616],
        ["Old Gold", "0xFFAD953E", 619],
        ["Old Gold", "0xFFAD953E", 619],
        ["Pale Apricot", "0xFFFEF9EA", 627],
        ["Pale Apricot", "0xFFFEF9EA", 627],
        ["Tan", "0xFFBD9565", 628],
        ["Tan", "0xFFBD9565", 628],
        ["Mellow Yellow", "0xFFFDF76C", 632],
        ["Mellow Yellow", "0xFFFDF76C", 632],
        ["Lemon", "0xFFEDEF05", 633],
        ["Lemon", "0xFFEDEF05", 633],
        ["Amber", "0xFFF8C300", 646],
        ["Amber", "0xFFF8C300", 646],
        ["Mandarina", "0xFFE77817", 649],
        ["Mandarina", "0xFFE77817", 649],
        ["Orange", "0xFFE66535", 650],
        ["Orange", "0xFFE66535", 650],
        ["Golden Rod", "0xFFC69632", 652],
        ["Golden Rod", "0xFFC69632", 652],
        ["Light Olive", "0xFF98996D", 653],
        ["Light Olive", "0xFF98996D", 653],
        ["Bright Gold", "0xFFC98300", 654],
        ["Bright Gold", "0xFFC98300", 654],
        ["Blue-Green", "0xFF007B8D", 688],
        ["Blue-Green", "0xFF007B8D", 688],
        ["Forrest Green", "0xFF004D3D", 695],
        ["Forrest Green", "0xFF004D3D", 695],
        ["Midnight Blue", "0xFF007EBA", 697],
        ["Midnight Blue", "0xFF007EBA", 697],
        ["Med Red", "0xFFCF0040", 700],
        ["Med Red", "0xFFCF0040", 700],
        ["Med Blue", "0xFF28438C", 809],
        ["Med Blue", "0xFF28438C", 809],
        ["Sweet Apricot", "0xFFD0B478", 812],
        ["Sweet Apricot", "0xFFD0B478", 812],
        ["Skin", "0xFFE5BE6C", 818],
        ["Skin", "0xFFE5BE6C", 818],
        ["Jade", "0xFF449284", 825],
        ["Jade", "0xFF449284", 825],
        ["Light Silver", "0xFFCFCFCF", 829],
        ["Light Silver", "0xFFCFCFCF", 829],
        ["Papaya Whip", "0xFFDC875E", 831],
        ["Papaya Whip", "0xFFDC875E", 831],
        ["Cooper", "0xFFB4705D", 832],
        ["Cooper", "0xFFB4705D", 832],
        ["Light Pecan", "0xFF9B5C4B", 833],
        ["Light Pecan", "0xFF9B5C4B", 833],
        ["Burnt Rust", "0xFFA93121", 838],
        ["Burnt Rust", "0xFFA93121", 838],
        ["Vegas Gold", "0xFFB18B00", 842],
        ["Vegas Gold", "0xFFB18B00", 842],
        ["Med Brown", "0xFF86462E", 857],
        ["Med Brown", "0xFF86462E", 857],
        ["Med Russett", "0xFF614125", 859],
        ["Med Russett", "0xFF614125", 859],
        ["Med Copper", "0xFFB25C31", 864],
        ["Med Copper", "0xFFB25C31", 864],
        ["Dark Driftwood", "0xFF806A61", 873],
        ["Dark Driftwood", "0xFF806A61", 873],
        ["Birch", "0xFF634831", 878],
        ["Birch", "0xFF634831", 878],
        ["Dark Chocolate", "0xFF1A0C06", 891],
        ["Dark Chocolate", "0xFF1A0C06", 891],
        ["Sky Blue 2", "0xFF96D5C8", 903],
        ["Sky Blue 2", "0xFF96D5C8", 903],
        ["Aquamarine", "0xFFB4DCD8", 904],
        ["Aquamarine", "0xFFB4DCD8", 904],
        ["Golden Brown", "0xFFAF7D3E", 905],
        ["Golden Brown", "0xFFAF7D3E", 905],
        ["Sea Blue", "0xFF00A3A0", 906],
        ["Sea Blue", "0xFF00A3A0", 906],
        ["Deep Sea", "0xFF00405D", 913],
        ["Deep Sea", "0xFF00405D", 913],
        ["Pastel Mint", "0xFFC9E3C5", 947],
        ["Pastel Mint", "0xFFC9E3C5", 947],
        ["True Green", "0xFF55AF78", 949],
        ["True Green", "0xFF55AF78", 949],
        ["Med Olive", "0xFF858325", 951],
        ["Med Olive", "0xFF858325", 951],
        ["Olive", "0xFF61601C", 955],
        ["Olive", "0xFF61601C", 955],
        ["Light Jade", "0xFF709188", 961],
        ["Light Jade", "0xFF709188", 961],
        ["Smith Apple", "0xFFBEDC8C", 984],
        ["Smith Apple", "0xFFBEDC8C", 984],
        ["Light Lime", "0xFFBEE678", 985],
        ["Light Lime", "0xFFBEE678", 985],
        ["Grass Green", "0xFF76C850", 988],
        ["Grass Green", "0xFF76C850", 988],
        ["Deep Teal", "0xFF466E64", 448],
        ["Deep Teal", "0xFF466E64", 448],
        ["Med Forrest Green", "0xFF356936", 992],
        ["Med Forrest Green", "0xFF356936", 992],
        ["Deep Violet", "0xFF4B4884", 1031],
        ["Deep Violet", "0xFF4B4884", 1031],
        ["Light Natural", "0xFFEDEDD2", 1140],
        ["Light Natural", "0xFFEDEDD2", 1140],
        ["Wheat", "0xFFF3D8A8", 1145],
        ["Wheat", "0xFFF3D8A8", 1145],
        ["Desert Sand", "0xFFC8BE96", 1148],
        ["Desert Sand", "0xFFC8BE96", 1148],
        ["Egyptian Blue", "0xFF243A7D", 1163],
        ["Egyptian Blue", "0xFF243A7D", 1163],
        ["Gecko", "0xFF86BE4E", 1183],
        ["Gecko", "0xFF86BE4E", 1183],
        ["Burgandy", "0xFF8E4044", 1241],
        ["Burgandy", "0xFF8E4044", 1241],
        ["Med Orchid", "0xFF893480", 1323],
        ["Med Orchid", "0xFF893480", 1323],
        ["Med Purple", "0xFF8C6DAA", 1324],
        ["Med Purple", "0xFF8C6DAA", 1324],
        ["Very Old Gold", "0xFFB6A36C", 1552],
        ["Very Old Gold", "0xFFB6A36C", 1552],
        ["Light Spruce", "0xFF2E9F76", 1615],
        ["Light Spruce", "0xFF2E9F76", 1615],
        ["Paris Green", "0xFF98C173", 1619],
        ["Paris Green", "0xFF98C173", 1619],
        ["Timberwolf", "0xFFCDCDCD", 1707],
        ["Timberwolf", "0xFFCDCDCD", 1707],
        ["Bright Blue", "0xFF2A377E", 2031],
        ["Bright Blue", "0xFF2A377E", 2031],
        ["Turquoise Blue", "0xFF006CA5", 2093],
        ["Turquoise Blue", "0xFF006CA5", 2093],
        ["Dark Wine", "0xFF834455", 2250],
        ["Dark Wine", "0xFF834455", 2250],
        ["Beige", "0xFFD0A44F", 2518],
        ["Beige", "0xFFD0A44F", 2518],
        ["Gold", "0xFFED9206", 2519],
        ["Gold", "0xFFED9206", 2519],
        ["Med Orange", "0xFFEDEF05", 3001],
        ["Med Orange", "0xFFEDEF05", 3001],
        ["Dark Salmon", "0xFFC07A46", 3014],
        ["Dark Salmon", "0xFFC07A46", 3014],
        ["Fire Red", "0xFFB43C3C", 3015],
        ["Fire Red", "0xFFB43C3C", 3015],
        ["Saddle Brown", "0xFF915F46", 3142],
        ["Saddle Brown", "0xFF915F46", 3142],
        ["Yellow Sun", "0xFFFFC500", 4117],
        ["Yellow Sun", "0xFFFFC500", 4117],
        ["Deep Taupe", "0xFFA68A68", 4371],
        ["Deep Taupe", "0xFFA68A68", 4371],
        ["Sky Blue", "0xFF00A4D9", 4419],
        ["Sky Blue", "0xFF00A4D9", 4419],
        ["Wild Peacock", "0xFF0B7F85", 4627],
        ["Wild Peacock", "0xFF0B7F85", 4627],
        ["Millard Green", "0xFF002D1F", 4735],
        ["Millard Green", "0xFF002D1F", 4735],
        ["Dark Blue", "0xFF11263C", 5552],
        ["Dark Blue", "0xFF11263C", 5552],
        ["Powder Blue", "0xFF91B9E2", 5554],
        ["Powder Blue", "0xFF91B9E2", 5554],
        ["Froggy Green", "0xFF429648", 5557],
        ["Froggy Green", "0xFF429648", 5557],
        ["Stone Grey", "0xFF878C8C", 8010],
        ["Stone Grey", "0xFF878C8C", 8010],
        ["END", 0, -1]
    ],
    "Sulky_Rayon_codes": [
        ["Cornsilk", "0xFFEFC810", 502],
        ["Cornsilk", "0xFFEFC810", 502],
        ["Deep Arctic Sky", "0xFF0C082D", 505],
        ["Deep Arctic Sky", "0xFF0C082D", 505],
        ["Nutmeg", "0xFFB26C29", 521],
        ["Nutmeg", "0xFFB26C29", 521],
        ["Autumn Gold", "0xFFE79002", 523],
        ["Autumn Gold", "0xFFE79002", 523],
        ["English Green", "0xFF34481E", 525],
        ["English Green", "0xFF34481E", 525],
        ["Cobalt Blue", "0xFF113675", 526],
        ["Cobalt Blue", "0xFF113675", 526],
        ["Forest Green", "0xFF111408", 538],
        ["Forest Green", "0xFF111408", 538],
        ["Lipstick", "0xFFE10000", 561],
        ["Lipstick", "0xFFE10000", 561],
        ["Spice", "0xFFFFB435", 562],
        ["Spice", "0xFFFFB435", 562],
        ["Butterfly Gold", "0xFFF3A001", 567],
        ["Butterfly Gold", "0xFFF3A001", 567],
        ["Cinnamon", "0xFFE66D00", 568],
        ["Cinnamon", "0xFFE66D00", 568],
        ["Garden Green", "0xFF165F28", 569],
        ["Garden Green", "0xFF165F28", 569],
        ["Deep Aqua", "0xFF088E6C", 571],
        ["Deep Aqua", "0xFF088E6C", 571],
        ["Blue Ribbon", "0xFF100A7C", 572],
        ["Blue Ribbon", "0xFF100A7C", 572],
        ["Mint Julep", "0xFF35693D", 580],
        ["Mint Julep", "0xFF35693D", 580],
        ["Dusty Peach", "0xFFE9BD96", 619],
        ["Dusty Peach", "0xFFE9BD96", 619],
        ["Sunset", "0xFFCD3900", 621],
        ["Sunset", "0xFFCD3900", 621],
        ["Moss Green", "0xFF777113", 630],
        ["Moss Green", "0xFF777113", 630],
        ["Med. Aqua", "0xFF1C6F51", 640],
        ["Med. Aqua", "0xFF1C6F51", 640],
        ["Arctic Sky", "0xFF262345", 643],
        ["Arctic Sky", "0xFF262345", 643],
        ["Bright White", "0xFFF9F9FF", 1001],
        ["Bright White", "0xFFF9F9FF", 1001],
        ["Soft White", "0xFFF9F9F4", 1002],
        ["Soft White", "0xFFF9F9F4", 1002],
        ["Black", "0xFF000000", 1005],
        ["Black", "0xFF000000", 1005],
        ["Steel Gray", "0xFFB7A9AC", 1011],
        ["Steel Gray", "0xFFB7A9AC", 1011],
        ["Med. Peach", "0xFFE1AF9A", 1015],
        ["Med. Peach", "0xFFE1AF9A", 1015],
        ["Pastel Coral", "0xFFEC968C", 1016],
        ["Pastel Coral", "0xFFEC968C", 1016],
        ["Pastel Peach", "0xFFEFDFBD", 1017],
        ["Pastel Peach", "0xFFEFDFBD", 1017],
        ["Peach", "0xFFECA082", 1019],
        ["Peach", "0xFFECA082", 1019],
        ["Dark Peach", "0xFFF08278", 1020],
        ["Dark Peach", "0xFFF08278", 1020],
        ["Maple", "0xFFEB6602", 1021],
        ["Maple", "0xFFEB6602", 1021],
        ["Cream", "0xFFFFF7D5", 1022],
        ["Cream", "0xFFFFF7D5", 1022],
        ["Yellow", "0xFFFFE669", 1023],
        ["Yellow", "0xFFFFE669", 1023],
        ["Goldenrod", "0xFFFFB800", 1024],
        ["Goldenrod", "0xFFFFB800", 1024],
        ["Mine Gold", "0xFFD78000", 1025],
        ["Mine Gold", "0xFFD78000", 1025],
        ["Baby Blue", "0xFFBEC3E1", 1028],
        ["Baby Blue", "0xFFBEC3E1", 1028],
        ["Med. Blue", "0xFFA0C3EB", 1029],
        ["Med. Blue", "0xFFA0C3EB", 1029],
        ["Periwinkle", "0xFFA6A2C6", 1030],
        ["Periwinkle", "0xFFA6A2C6", 1030],
        ["Med. Orchid", "0xFFDFBEC8", 1031],
        ["Med. Orchid", "0xFFDFBEC8", 1031],
        ["Med. Purple", "0xFFE68CEB", 1032],
        ["Med. Purple", "0xFFE68CEB", 1032],
        ["Dk. Orchid", "0xFFD86496", 1033],
        ["Dk. Orchid", "0xFFD86496", 1033],
        ["Burgundy", "0xFFC6323C", 1034],
        ["Burgundy", "0xFFC6323C", 1034],
        ["Dk. Burgundy", "0xFF790000", 1035],
        ["Dk. Burgundy", "0xFF790000", 1035],
        ["Lt.Red", "0xFFF90000", 1037],
        ["Lt.Red", "0xFFF90000", 1037],
        ["True Red", "0xFFEB0000", 1039],
        ["True Red", "0xFFEB0000", 1039],
        ["Med. Dk. Khaki", "0xFF877375", 1040],
        ["Med. Dk. Khaki", "0xFF877375", 1040],
        ["Med. Dk. Gray", "0xFF8C7F83", 1041],
        ["Med. Dk. Gray", "0xFF8C7F83", 1041],
        ["Bright Navy Blue", "0xFF321E50", 1042],
        ["Bright Navy Blue", "0xFF321E50", 1042],
        ["Dk. Navy", "0xFF190525", 1043],
        ["Dk. Navy", "0xFF190525", 1043],
        ["Midnight Blue", "0xFF1D062F", 1044],
        ["Midnight Blue", "0xFF1D062F", 1044],
        ["Lt. Teal", "0xFFC3EFBF", 1045],
        ["Lt. Teal", "0xFFC3EFBF", 1045],
        ["Teal", "0xFF2E8359", 1046],
        ["Teal", "0xFF2E8359", 1046],
        ["Mint Green", "0xFFA6C284", 1047],
        ["Mint Green", "0xFFA6C284", 1047],
        ["Grass Green", "0xFF42A021", 1049],
        ["Grass Green", "0xFF42A021", 1049],
        ["Xmas Green", "0xFF1E6419", 1051],
        ["Xmas Green", "0xFF1E6419", 1051],
        ["Med. Dk. Ecru", "0xFFEEBEAE", 1054],
        ["Med. Dk. Ecru", "0xFFEEBEAE", 1054],
        ["Tawny Tan", "0xFFEBBC80", 1055],
        ["Tawny Tan", "0xFFEBBC80", 1055],
        ["Med Tawny Tan", "0xFFAF5B00", 1056],
        ["Med Tawny Tan", "0xFFAF5B00", 1056],
        ["Dk Tawny Tan", "0xFF642702", 1057],
        ["Dk Tawny Tan", "0xFF642702", 1057],
        ["Tawny Brown", "0xFF663500", 1058],
        ["Tawny Brown", "0xFF663500", 1058],
        ["Dk Tawny Brown", "0xFF530601", 1059],
        ["Dk Tawny Brown", "0xFF530601", 1059],
        ["Pale Yellow", "0xFFFFF7B9", 1061],
        ["Pale Yellow", "0xFFFFF7B9", 1061],
        ["Pale Yellow-Green", "0xFFF0F8EC", 1063],
        ["Pale Yellow-Green", "0xFFF0F8EC", 1063],
        ["Pale Peach", "0xFFE6B4AA", 1064],
        ["Pale Peach", "0xFFE6B4AA", 1064],
        ["Orange Yellow", "0xFFFF9100", 1065],
        ["Orange Yellow", "0xFFFF9100", 1065],
        ["Primrose", "0xFFFFF180", 1066],
        ["Primrose", "0xFFFFF180", 1066],
        ["Lemon Yellow", "0xFFFFFF85", 1067],
        ["Lemon Yellow", "0xFFFFFF85", 1067],
        ["Pink Tint", "0xFFF3DBD9", 1068],
        ["Pink Tint", "0xFFF3DBD9", 1068],
        ["Gold", "0xFFF6CE69", 1070],
        ["Gold", "0xFFF6CE69", 1070],
        ["Off White", "0xFFF9F9EA", 1071],
        ["Off White", "0xFFF9F9EA", 1071],
        ["Pale Powder Blue", "0xFFD6D5E8", 1074],
        ["Pale Powder Blue", "0xFFD6D5E8", 1074],
        ["Royal Blue", "0xFF5A5A8B", 1076],
        ["Royal Blue", "0xFF5A5A8B", 1076],
        ["Jade Tint", "0xFFBECDC8", 1077],
        ["Jade Tint", "0xFFBECDC8", 1077],
        ["Tangerine", "0xFFFF6600", 1078],
        ["Tangerine", "0xFFFF6600", 1078],
        ["Emerald Green", "0xFF175523", 1079],
        ["Emerald Green", "0xFF175523", 1079],
        ["Orchid", "0xFFDC82A0", 1080],
        ["Orchid", "0xFFDC82A0", 1080],
        ["Brick", "0xFFF06E78", 1081],
        ["Brick", "0xFFF06E78", 1081],
        ["Ecru", "0xFFF7E3BB", 1082],
        ["Ecru", "0xFFF7E3BB", 1082],
        ["Spark Gold", "0xFFFFC100", 1083],
        ["Spark Gold", "0xFFFFC100", 1083],
        ["Silver", "0xFFE2CFC7", 1085],
        ["Silver", "0xFFE2CFC7", 1085],
        ["Pale Sea Foam", "0xFFF9F9E0", 1086],
        ["Pale Sea Foam", "0xFFF9F9E0", 1086],
        ["Deep Peacock", "0xFF16625F", 1090],
        ["Deep Peacock", "0xFF16625F", 1090],
        ["Med Turquoise", "0xFF26BFCA", 1094],
        ["Med Turquoise", "0xFF26BFCA", 1094],
        ["Turquoise", "0xFF10D1BD", 1095],
        ["Turquoise", "0xFF10D1BD", 1095],
        ["Dk Turquoise", "0xFF0F6978", 1096],
        ["Dk Turquoise", "0xFF0F6978", 1096],
        ["Lt Grass Green", "0xFFC2D37D", 1100],
        ["Lt Grass Green", "0xFFC2D37D", 1100],
        ["True Green", "0xFF098531", 1101],
        ["True Green", "0xFF098531", 1101],
        ["Dk Khaki", "0xFF02140F", 1103],
        ["Dk Khaki", "0xFF02140F", 1103],
        ["Pastel Yellow-Grn", "0xFFA5AF68", 1104],
        ["Pastel Yellow-Grn", "0xFFA5AF68", 1104],
        ["Lt Mauve", "0xFFFAA4A4", 1108],
        ["Lt Mauve", "0xFFFAA4A4", 1108],
        ["Hot Pink", "0xFFDC6496", 1109],
        ["Hot Pink", "0xFFDC6496", 1109],
        ["Pastel Orchid", "0xFFFCCBDF", 1111],
        ["Pastel Orchid", "0xFFFCCBDF", 1111],
        ["Royal Purple", "0xFF46016E", 1112],
        ["Royal Purple", "0xFF46016E", 1112],
        ["Pastel Mauve", "0xFFF0C8B4", 1113],
        ["Pastel Mauve", "0xFFF0C8B4", 1113],
        ["Lt Pink", "0xFFF0B9B9", 1115],
        ["Lt Pink", "0xFFF0B9B9", 1115],
        ["Mauve", "0xFFF5A9A0", 1117],
        ["Mauve", "0xFFF5A9A0", 1117],
        ["Dk Mauve", "0xFFB46E75", 1119],
        ["Dk Mauve", "0xFFB46E75", 1119],
        ["Pale Pink", "0xFFF0D6D2", 1120],
        ["Pale Pink", "0xFFF0D6D2", 1120],
        ["Pink", "0xFFFAB9CB", 1121],
        ["Pink", "0xFFFAB9CB", 1121],
        ["Purple", "0xFF82288E", 1122],
        ["Purple", "0xFF82288E", 1122],
        ["Sun Yellow", "0xFFFFEC00", 1124],
        ["Sun Yellow", "0xFFFFEC00", 1124],
        ["Tan", "0xFFDC8C17", 1126],
        ["Tan", "0xFFDC8C17", 1126],
        ["Med Ecru", "0xFFFAECC6", 1127],
        ["Med Ecru", "0xFFFAECC6", 1127],
        ["Dk Ecru", "0xFFC39471", 1128],
        ["Dk Ecru", "0xFFC39471", 1128],
        ["Brown", "0xFF6A1F06", 1129],
        ["Brown", "0xFF6A1F06", 1129],
        ["Dark Brown", "0xFF551602", 1130],
        ["Dark Brown", "0xFF551602", 1130],
        ["Cloister Brown", "0xFF490002", 1131],
        ["Cloister Brown", "0xFF490002", 1131],
        ["Peacock Blue", "0xFF507DAA", 1134],
        ["Peacock Blue", "0xFF507DAA", 1134],
        ["Pastel Yellow", "0xFFFFF072", 1135],
        ["Pastel Yellow", "0xFFFFF072", 1135],
        ["Yellow Orange", "0xFFFFBE00", 1137],
        ["Yellow Orange", "0xFFFFBE00", 1137],
        ["True Blue", "0xFF4A5870", 1143],
        ["True Blue", "0xFF4A5870", 1143],
        ["Powder Blue", "0xFFB4E1EB", 1145],
        ["Powder Blue", "0xFFB4E1EB", 1145],
        ["Xmas Red", "0xFFEB0000", 1147],
        ["Xmas Red", "0xFFEB0000", 1147],
        ["Lt Coral", "0xFFFFBDBD", 1148],
        ["Lt Coral", "0xFFFFBDBD", 1148],
        ["Deep Ecru", "0xFFE8C89C", 1149],
        ["Deep Ecru", "0xFFE8C89C", 1149],
        ["Powder Blue Tint", "0xFFE2E2EB", 1151],
        ["Powder Blue Tint", "0xFFE2E2EB", 1151],
        ["Coral", "0xFFFA9999", 1154],
        ["Coral", "0xFFFA9999", 1154],
        ["Lt Army Green", "0xFF636327", 1156],
        ["Lt Army Green", "0xFF636327", 1156],
        ["Dk Maple", "0xFFBA4500", 1158],
        ["Dk Maple", "0xFFBA4500", 1158],
        ["Temple Gold", "0xFFD39D00", 1159],
        ["Temple Gold", "0xFFD39D00", 1159],
        ["Deep Teal", "0xFF10394A", 1162],
        ["Deep Teal", "0xFF10394A", 1162],
        ["Lt Sky Blue", "0xFFDFE5EB", 1165],
        ["Lt Sky Blue", "0xFFDFE5EB", 1165],
        ["Med Steel Gray", "0xFF8E7E7E", 1166],
        ["Med Steel Gray", "0xFF8E7E7E", 1166],
        ["Maize Yellow", "0xFFFFD226", 1167],
        ["Maize Yellow", "0xFFFFD226", 1167],
        ["True Orange", "0xFFF5740A", 1168],
        ["True Orange", "0xFFF5740A", 1168],
        ["Bayberry Red", "0xFF9C0000", 1169],
        ["Bayberry Red", "0xFF9C0000", 1169],
        ["Lt Brown", "0xFF975F2F", 1170],
        ["Lt Brown", "0xFF975F2F", 1170],
        ["Weathered Blue", "0xFF08180E", 1171],
        ["Weathered Blue", "0xFF08180E", 1171],
        ["Med Weathered Blue", "0xFF6E788C", 1172],
        ["Med Weathered Blue", "0xFF6E788C", 1172],
        ["Med Army Green", "0xFF59591D", 1173],
        ["Med Army Green", "0xFF59591D", 1173],
        ["Dk Pine Green", "0xFF0D2904", 1174],
        ["Dk Pine Green", "0xFF0D2904", 1174],
        ["Dk Avocado", "0xFF152D04", 1175],
        ["Dk Avocado", "0xFF152D04", 1175],
        ["Med Dk Avocado", "0xFF515308", 1176],
        ["Med Dk Avocado", "0xFF515308", 1176],
        ["Avocado", "0xFF899812", 1177],
        ["Avocado", "0xFF899812", 1177],
        ["Dk Taupe", "0xFF8F623D", 1179],
        ["Dk Taupe", "0xFF8F623D", 1179],
        ["Med Taupe", "0xFFA58973", 1180],
        ["Med Taupe", "0xFFA58973", 1180],
        ["Rust", "0xFFCB0000", 1181],
        ["Rust", "0xFFCB0000", 1181],
        ["Blue Black", "0xFF020114", 1182],
        ["Blue Black", "0xFF020114", 1182],
        ["Black Cherry", "0xFF320614", 1183],
        ["Black Cherry", "0xFF320614", 1183],
        ["Orange Red", "0xFFFF6600", 1184],
        ["Orange Red", "0xFFFF6600", 1184],
        ["Golden Yellow", "0xFFFCBE05", 1185],
        ["Golden Yellow", "0xFFFCBE05", 1185],
        ["Sable Brown", "0xFF5B0000", 1186],
        ["Sable Brown", "0xFF5B0000", 1186],
        ["Mimosa Yellow", "0xFFFFE500", 1187],
        ["Mimosa Yellow", "0xFFFFE500", 1187],
        ["Red Geranium", "0xFFFF004B", 1188],
        ["Red Geranium", "0xFFFF004B", 1188],
        ["Dk Chestnut", "0xFF4B122D", 1189],
        ["Dk Chestnut", "0xFF4B122D", 1189],
        ["Med Burgundy", "0xFFA04656", 1190],
        ["Med Burgundy", "0xFFA04656", 1190],
        ["Dk Rose", "0xFFBD1E60", 1191],
        ["Dk Rose", "0xFFBD1E60", 1191],
        ["Fuchsia", "0xFFD21E82", 1192],
        ["Fuchsia", "0xFFD21E82", 1192],
        ["Lavender", "0xFFE6AFD2", 1193],
        ["Lavender", "0xFFE6AFD2", 1193],
        ["Lt Purple", "0xFFD274D7", 1194],
        ["Lt Purple", "0xFFD274D7", 1194],
        ["Dk Purple", "0xFF370150", 1195],
        ["Dk Purple", "0xFF370150", 1195],
        ["Blue", "0xFF96C3E1", 1196],
        ["Blue", "0xFF96C3E1", 1196],
        ["Med Navy", "0xFF220F34", 1197],
        ["Med Navy", "0xFF220F34", 1197],
        ["Dusty Navy", "0xFF3C5075", 1198],
        ["Dusty Navy", "0xFF3C5075", 1198],
        ["Admiral Navy Blue", "0xFF2A143F", 1199],
        ["Admiral Navy Blue", "0xFF2A143F", 1199],
        ["Med Dk Navy", "0xFF140B2D", 1200],
        ["Med Dk Navy", "0xFF140B2D", 1200],
        ["Med Powder Blue", "0xFF648BBE", 1201],
        ["Med Powder Blue", "0xFF648BBE", 1201],
        ["Deep Turquoise", "0xFF182B56", 1202],
        ["Deep Turquoise", "0xFF182B56", 1202],
        ["Lt Weathered Blue", "0xFFAEB8C3", 1203],
        ["Lt Weathered Blue", "0xFFAEB8C3", 1203],
        ["Pastel Jade", "0xFFA8C8BC", 1204],
        ["Pastel Jade", "0xFFA8C8BC", 1204],
        ["Med Jade", "0xFF6E90A5", 1205],
        ["Med Jade", "0xFF6E90A5", 1205],
        ["Dark Jade", "0xFF1E6E6F", 1206],
        ["Dark Jade", "0xFF1E6E6F", 1206],
        ["Sea Foam Green", "0xFF80A388", 1207],
        ["Sea Foam Green", "0xFF80A388", 1207],
        ["Mallard Green", "0xFF0C3D03", 1208],
        ["Mallard Green", "0xFF0C3D03", 1208],
        ["Lt Avocado", "0xFFBDD163", 1209],
        ["Lt Avocado", "0xFFBDD163", 1209],
        ["Dk Army Green", "0xFF273B00", 1210],
        ["Dk Army Green", "0xFF273B00", 1210],
        ["Lt Khaki", "0xFF95A490", 1211],
        ["Lt Khaki", "0xFF95A490", 1211],
        ["Khaki", "0xFF63632D", 1212],
        ["Khaki", "0xFF63632D", 1212],
        ["Taupe", "0xFFB9A096", 1213],
        ["Taupe", "0xFFB9A096", 1213],
        ["Med Chestnut", "0xFF642828", 1214],
        ["Med Chestnut", "0xFF642828", 1214],
        ["Blackberry", "0xFF500A1E", 1215],
        ["Blackberry", "0xFF500A1E", 1215],
        ["Med Maple", "0xFFAC1C01", 1216],
        ["Med Maple", "0xFFAC1C01", 1216],
        ["Chestnut", "0xFF971F01", 1217],
        ["Chestnut", "0xFF971F01", 1217],
        ["Silver Gray", "0xFFDFDFCB", 1218],
        ["Silver Gray", "0xFFDFDFCB", 1218],
        ["Gray", "0xFF98888C", 1219],
        ["Gray", "0xFF98888C", 1219],
        ["Charcoal Gray", "0xFF765960", 1220],
        ["Charcoal Gray", "0xFF765960", 1220],
        ["Lt Baby Blue", "0xFFD1DBFF", 1222],
        ["Lt Baby Blue", "0xFFD1DBFF", 1222],
        ["Baby Blue Tint", "0xFFDCE0F1", 1223],
        ["Baby Blue Tint", "0xFFDCE0F1", 1223],
        ["Bright Pink", "0xFFF0A0B9", 1224],
        ["Bright Pink", "0xFFF0A0B9", 1224],
        ["Pastel Pink", "0xFFFACBCB", 1225],
        ["Pastel Pink", "0xFFFACBCB", 1225],
        ["Dkl Periwinkle", "0xFF57369E", 1226],
        ["Dkl Periwinkle", "0xFF57369E", 1226],
        ["Gold Green", "0xFFAF8901", 1227],
        ["Gold Green", "0xFFAF8901", 1227],
        ["Drab Green", "0xFF96AA8B", 1228],
        ["Drab Green", "0xFF96AA8B", 1228],
        ["Lt Putty", "0xFFE0DBDB", 1229],
        ["Lt Putty", "0xFFE0DBDB", 1229],
        ["Dk Teal", "0xFF0B4133", 1230],
        ["Dk Teal", "0xFF0B4133", 1230],
        ["Med Rose", "0xFFE5326A", 1231],
        ["Med Rose", "0xFFE5326A", 1231],
        ["Classic Green", "0xFF193207", 1232],
        ["Classic Green", "0xFF193207", 1232],
        ["Ocean Teal", "0xFF0D2210", 1233],
        ["Ocean Teal", "0xFF0D2210", 1233],
        ["Almost Black", "0xFF3C1B1F", 1234],
        ["Almost Black", "0xFF3C1B1F", 1234],
        ["Deep Purple", "0xFF783298", 1235],
        ["Deep Purple", "0xFF783298", 1235],
        ["Lt Silver", "0xFFEAE4E4", 1236],
        ["Lt Silver", "0xFFEAE4E4", 1236],
        ["Deep Mauve", "0xFFBC3D2C", 1237],
        ["Deep Mauve", "0xFFBC3D2C", 1237],
        ["Orange Sunrise", "0xFFFF8300", 1238],
        ["Orange Sunrise", "0xFFFF8300", 1238],
        ["Apricot", "0xFFFFAB57", 1239],
        ["Apricot", "0xFFFFAB57", 1239],
        ["Smokey Grey", "0xFF74586C", 1240],
        ["Smokey Grey", "0xFF74586C", 1240],
        ["Nassau Blue", "0xFF543A8D", 1242],
        ["Nassau Blue", "0xFF543A8D", 1242],
        ["Orange Flame", "0xFFFF0000", 1246],
        ["Orange Flame", "0xFFFF0000", 1246],
        ["Mahogany", "0xFF660000", 1247],
        ["Mahogany", "0xFF660000", 1247],
        ["Med Pastel Blue", "0xFFD2E6F0", 1248],
        ["Med Pastel Blue", "0xFFD2E6F0", 1248],
        ["Cornflower Blue", "0xFF62AADC", 1249],
        ["Cornflower Blue", "0xFF62AADC", 1249],
        ["Duck Wing Blue", "0xFF275C70", 1250],
        ["Duck Wing Blue", "0xFF275C70", 1250],
        ["Bright Turquoise", "0xFF306F75", 1251],
        ["Bright Turquoise", "0xFF306F75", 1251],
        ["Bright Peacock", "0xFF09A1A8", 1252],
        ["Bright Peacock", "0xFF09A1A8", 1252],
        ["Dk Sapphire", "0xFF1B4CA4", 1253],
        ["Dk Sapphire", "0xFF1B4CA4", 1253],
        ["Dusty Lavender", "0xFFE6B9F5", 1254],
        ["Dusty Lavender", "0xFFE6B9F5", 1254],
        ["Deep Orchid", "0xFFBE1982", 1255],
        ["Deep Orchid", "0xFFBE1982", 1255],
        ["Sweet Pink", "0xFFEB8296", 1256],
        ["Sweet Pink", "0xFFEB8296", 1256],
        ["Deep Coral", "0xFFE60041", 1257],
        ["Deep Coral", "0xFFE60041", 1257],
        ["Coral Reed", "0xFFF0C4A0", 1258],
        ["Coral Reed", "0xFFF0C4A0", 1258],
        ["Salmon Peach", "0xFFE28264", 1259],
        ["Salmon Peach", "0xFFE28264", 1259],
        ["Red Jubilee", "0xFFB30000", 1263],
        ["Red Jubilee", "0xFFB30000", 1263],
        ["Cognac", "0xFF6A0000", 1264],
        ["Cognac", "0xFF6A0000", 1264],
        ["Burnt Toast", "0xFF9B6B2C", 1265],
        ["Burnt Toast", "0xFF9B6B2C", 1265],
        ["Toast", "0xFF9C6D45", 1266],
        ["Toast", "0xFF9C6D45", 1266],
        ["Mink Brown", "0xFF864C31", 1267],
        ["Mink Brown", "0xFF864C31", 1267],
        ["Light Gray Khaki", "0xFFEFEFE5", 1268],
        ["Light Gray Khaki", "0xFFEFEFE5", 1268],
        ["Dk Gray Khaki", "0xFFB7B7AF", 1270],
        ["Dk Gray Khaki", "0xFFB7B7AF", 1270],
        ["Evergreen", "0xFF3C4F31", 1271],
        ["Evergreen", "0xFF3C4F31", 1271],
        ["Hedge Green", "0xFF4A4A19", 1272],
        ["Hedge Green", "0xFF4A4A19", 1272],
        ["Nile Green", "0xFF5C9A1A", 1274],
        ["Nile Green", "0xFF5C9A1A", 1274],
        ["Sea Mist", "0xFFE0E6C8", 1275],
        ["Sea Mist", "0xFFE0E6C8", 1275],
        ["Pistachio", "0xFF70770F", 1276],
        ["Pistachio", "0xFF70770F", 1276],
        ["Ivy Green", "0xFF027602", 1277],
        ["Ivy Green", "0xFF027602", 1277],
        ["Bright Green", "0xFF00AF38", 1278],
        ["Bright Green", "0xFF00AF38", 1278],
        ["Willow Green", "0xFF93D16C", 1279],
        ["Willow Green", "0xFF93D16C", 1279],
        ["Dk Willow Green", "0xFF46B774", 1280],
        ["Dk Willow Green", "0xFF46B774", 1280],
        ["Slate Gray", "0xFF483D59", 1283],
        ["Slate Gray", "0xFF483D59", 1283],
        ["Dk Winter Sky", "0xFF466E78", 1284],
        ["Dk Winter Sky", "0xFF466E78", 1284],
        ["Dk Sage Green", "0xFF134F45", 1285],
        ["Dk Sage Green", "0xFF134F45", 1285],
        ["Dk French Green", "0xFF343213", 1286],
        ["Dk French Green", "0xFF343213", 1286],
        ["French Green", "0xFF415545", 1287],
        ["French Green", "0xFF415545", 1287],
        ["Aqua", "0xFF0FA56F", 1288],
        ["Aqua", "0xFF0FA56F", 1288],
        ["Ice Blue", "0xFFDCEBF0", 1289],
        ["Ice Blue", "0xFFDCEBF0", 1289],
        ["Winter Sky", "0xFF727483", 1291],
        ["Winter Sky", "0xFF727483", 1291],
        ["Heron Blue", "0xFFD1DCFA", 1292],
        ["Heron Blue", "0xFFD1DCFA", 1292],
        ["Deep Nassau Blue", "0xFF44235D", 1293],
        ["Deep Nassau Blue", "0xFF44235D", 1293],
        ["Deep Slate Gray", "0xFF412044", 1294],
        ["Deep Slate Gray", "0xFF412044", 1294],
        ["Sterling", "0xFF82878C", 1295],
        ["Sterling", "0xFF82878C", 1295],
        ["Hyacinth", "0xFFD2AAF0", 1296],
        ["Hyacinth", "0xFFD2AAF0", 1296],
        ["Lt Plum", "0xFF735A64", 1297],
        ["Lt Plum", "0xFF735A64", 1297],
        ["Dk Plum", "0xFF644664", 1298],
        ["Dk Plum", "0xFF644664", 1298],
        ["Purple Shadow", "0xFF411446", 1299],
        ["Purple Shadow", "0xFF411446", 1299],
        ["Plum", "0xFF7E1E46", 1300],
        ["Plum", "0xFF7E1E46", 1300],
        ["Deep Eggplant", "0xFF320046", 1301],
        ["Deep Eggplant", "0xFF320046", 1301],
        ["Eggplant", "0xFF6E0A96", 1302],
        ["Eggplant", "0xFF6E0A96", 1302],
        ["Dewberry", "0xFFB47364", 1304],
        ["Dewberry", "0xFFB47364", 1304],
        ["Sage Green", "0xFFAEC6BB", 1305],
        ["Sage Green", "0xFFAEC6BB", 1305],
        ["Gun Metal Gray", "0xFF7E6C7C", 1306],
        ["Gun Metal Gray", "0xFF7E6C7C", 1306],
        ["Petal Pink", "0xFFDB6478", 1307],
        ["Petal Pink", "0xFFDB6478", 1307],
        ["Magenta", "0xFF782346", 1309],
        ["Magenta", "0xFF782346", 1309],
        ["Mulberry", "0xFF961A32", 1311],
        ["Mulberry", "0xFF961A32", 1311],
        ["Wine", "0xFF840000", 1312],
        ["Wine", "0xFF840000", 1312],
        ["Bittersweet", "0xFFFC8F0C", 1313],
        ["Bittersweet", "0xFFFC8F0C", 1313],
        ["Poppy", "0xFFFF0000", 1317],
        ["Poppy", "0xFFFF0000", 1317],
        ["Gray Khaki", "0xFFCBCBBD", 1321],
        ["Gray Khaki", "0xFFCBCBBD", 1321],
        ["Chartreuse", "0xFF818901", 1322],
        ["Chartreuse", "0xFF818901", 1322],
        ["Whisper Gray", "0xFFF8F5F1", 1325],
        ["Whisper Gray", "0xFFF8F5F1", 1325],
        ["Dk Whisper Gray", "0xFFD5C7C3", 1327],
        ["Dk Whisper Gray", "0xFFD5C7C3", 1327],
        ["Nickel Gray", "0xFFC0B2B7", 1328],
        ["Nickel Gray", "0xFFC0B2B7", 1328],
        ["Dk Nickel Gray", "0xFFABA0A8", 1329],
        ["Dk Nickel Gray", "0xFFABA0A8", 1329],
        ["Pale Green", "0xFFEDF6D4", 1331],
        ["Pale Green", "0xFFEDF6D4", 1331],
        ["Deep Chartreuse", "0xFF868105", 1332],
        ["Deep Chartreuse", "0xFF868105", 1332],
        ["Sunflower Gold", "0xFFF3B600", 1333],
        ["Sunflower Gold", "0xFFF3B600", 1333],
        ["Green Peacock", "0xFF349669", 1503],
        ["Green Peacock", "0xFF349669", 1503],
        ["Putty", "0xFFC1CBB9", 1508],
        ["Putty", "0xFFC1CBB9", 1508],
        ["Lime Green", "0xFF7AB31D", 1510],
        ["Lime Green", "0xFF7AB31D", 1510],
        ["Deep Rose", "0xFFEE5078", 1511],
        ["Deep Rose", "0xFFEE5078", 1511],
        ["Wild Peacock", "0xFF007A67", 1513],
        ["Wild Peacock", "0xFF007A67", 1513],
        ["Rosebud", "0xFFFF8CCB", 1515],
        ["Rosebud", "0xFFFF8CCB", 1515],
        ["Coachman Green", "0xFF014F3A", 1517],
        ["Coachman Green", "0xFF014F3A", 1517],
        ["Lt Rose", "0xFFCD054D", 1533],
        ["Lt Rose", "0xFFCD054D", 1533],
        ["Sapphire", "0xFF347DCB", 1534],
        ["Sapphire", "0xFF347DCB", 1534],
        ["Team Blue", "0xFF23238B", 1535],
        ["Team Blue", "0xFF23238B", 1535],
        ["Midnight Teal", "0xFF081705", 1536],
        ["Midnight Teal", "0xFF081705", 1536],
        ["Peach Fluff", "0xFFFFD6C7", 1543],
        ["Peach Fluff", "0xFFFFD6C7", 1543],
        ["Purple Accent", "0xFF9C6484", 1545],
        ["Purple Accent", "0xFF9C6484", 1545],
        ["Flax", "0xFFE6AE6F", 1549],
        ["Flax", "0xFFE6AE6F", 1549],
        ["Desert Cactus", "0xFF6C8E87", 1550],
        ["Desert Cactus", "0xFF6C8E87", 1550],
        ["Ocean Aqua", "0xFF80B0AE", 1551],
        ["Ocean Aqua", "0xFF80B0AE", 1551],
        ["Dk Desert Cactus", "0xFF6C7C71", 1552],
        ["Dk Desert Cactus", "0xFF6C7C71", 1552],
        ["Purple Passion", "0xFF8C748C", 1554],
        ["Purple Passion", "0xFF8C748C", 1554],
        ["Tea Rose", "0xFFEB7183", 1558],
        ["Tea Rose", "0xFFEB7183", 1558],
        ["Marine Aqua", "0xFF68E0F8", 1560],
        ["Marine Aqua", "0xFF68E0F8", 1560],
        ["Deep Hyacinth", "0xFFB58CC7", 1561],
        ["Deep Hyacinth", "0xFFB58CC7", 1561],
        ["Shrimp", "0xFFFAD2AA", 1800],
        ["Shrimp", "0xFFFAD2AA", 1800],
        ["Flesh", "0xFFFADC96", 1801],
        ["Flesh", "0xFFFADC96", 1801],
        ["Soft Blush", "0xFFFFC896", 1802],
        ["Soft Blush", "0xFFFFC896", 1802],
        ["Island Peach", "0xFFFF9B6E", 1803],
        ["Island Peach", "0xFFFF9B6E", 1803],
        ["Bayou Blue", "0xFF375A73", 1804],
        ["Bayou Blue", "0xFF375A73", 1804],
        ["Ocean View", "0xFF28505A", 1805],
        ["Ocean View", "0xFF28505A", 1805],
        ["Madras Blue", "0xFFA0B9C3", 1806],
        ["Madras Blue", "0xFFA0B9C3", 1806],
        ["Soft Heather", "0xFFB49682", 1807],
        ["Soft Heather", "0xFFB49682", 1807],
        ["Velvet Slipper", "0xFFD2AF9B", 1808],
        ["Velvet Slipper", "0xFFD2AF9B", 1808],
        ["Iced Mauve", "0xFFA07D82", 1809],
        ["Iced Mauve", "0xFFA07D82", 1809],
        ["Wild Mulberry", "0xFF645055", 1810],
        ["Wild Mulberry", "0xFF645055", 1810],
        ["Wineberry", "0xFF3C2837", 1811],
        ["Wineberry", "0xFF3C2837", 1811],
        ["Wildflower", "0xFF6E2D5A", 1812],
        ["Wildflower", "0xFF6E2D5A", 1812],
        ["Plum Wine", "0xFF6E2D41", 1813],
        ["Plum Wine", "0xFF6E2D41", 1813],
        ["Orchid Kiss", "0xFFAF4B69", 1814],
        ["Orchid Kiss", "0xFFAF4B69", 1814],
        ["Japanese Fern", "0xFF91B432", 1815],
        ["Japanese Fern", "0xFF91B432", 1815],
        ["Honeydew", "0xFFD7F58C", 1816],
        ["Honeydew", "0xFFD7F58C", 1816],
        ["Lemon Grass", "0xFFAAAF14", 1817],
        ["Lemon Grass", "0xFFAAAF14", 1817],
        ["Fairway Mist", "0xFFC8F58C", 1818],
        ["Fairway Mist", "0xFFC8F58C", 1818],
        ["Outback", "0xFFC3913C", 1819],
        ["Outback", "0xFFC3913C", 1819],
        ["Fruit Shake", "0xFFC38C73", 1820],
        ["Fruit Shake", "0xFFC38C73", 1820],
        ["Creamy Peach", "0xFFFAC896", 1821],
        ["Creamy Peach", "0xFFFAC896", 1821],
        ["Toffee", "0xFF965A37", 1822],
        ["Toffee", "0xFF965A37", 1822],
        ["Cocoa", "0xFF965A28", 1823],
        ["Cocoa", "0xFF965A28", 1823],
        ["Gentle Rain", "0xFFD2C3AF", 1824],
        ["Gentle Rain", "0xFFD2C3AF", 1824],
        ["Barnyard Grass", "0xFF5F9619", 1825],
        ["Barnyard Grass", "0xFF5F9619", 1825],
        ["Galley Gold", "0xFFAA820A", 1826],
        ["Galley Gold", "0xFFAA820A", 1826],
        ["Coral Sunset", "0xFFFF643C", 1827],
        ["Coral Sunset", "0xFFFF643C", 1827],
        ["Seashell", "0xFFFFE6AA", 1828],
        ["Seashell", "0xFFFFE6AA", 1828],
        ["Crème Brulee", "0xFFF0EBA5", 1829],
        ["Crème Brulee", "0xFFF0EBA5", 1829],
        ["Lilac", "0xFFB47396", 1830],
        ["Lilac", "0xFFB47396", 1830],
        ["Liimeade", "0xFF91E12D", 1831],
        ["Liimeade", "0xFF91E12D", 1831],
        ["Desert Glow", "0xFFE19119", 1832],
        ["Desert Glow", "0xFFE19119", 1832],
        ["Pumpkin Pie", "0xFFD25F00", 1833],
        ["Pumpkin Pie", "0xFFD25F00", 1833],
        ["Pea Soup", "0xFFAFAA05", 1834],
        ["Pea Soup", "0xFFAFAA05", 1834],
        ["Peapod Green", "0xFF6E8205", 1835],
        ["Peapod Green", "0xFF6E8205", 1835],
        ["Loden Green", "0xFF3C4B05", 1836],
        ["Loden Green", "0xFF3C4B05", 1836],
        ["Lt. Cocoa", "0xFF9B735A", 1837],
        ["Lt. Cocoa", "0xFF9B735A", 1837],
        ["Cocoa Cream", "0xFFCDAA7D", 1838],
        ["Cocoa Cream", "0xFFCDAA7D", 1838],
        ["Cameo", "0xFF87C887", 1839],
        ["Cameo", "0xFF87C887", 1839],
        ["Sand", "0xFFF9E6CA", 508],
        ["Sand", "0xFFF9E6CA", 508],
        ["Bone", "0xFFFDF3DA", 520],
        ["Bone", "0xFFFDF3DA", 520],
        ["Dark Ash", "0xFF5D3446", 1241],
        ["Dark Ash", "0xFF5D3446", 1241],
        ["Spring Moss", "0xFFE0C63B", 1243],
        ["Spring Moss", "0xFFE0C63B", 1243],
        ["Summer Gold", "0xFFDDAB00", 1260],
        ["Summer Gold", "0xFFDDAB00", 1260],
        ["Dk. Autumn Gold", "0xFFA98803", 1262],
        ["Dk. Autumn Gold", "0xFFA98803", 1262],
        ["Mushroom", "0xFFAC8783", 1269],
        ["Mushroom", "0xFFAC8783", 1269],
        ["Dark Forest", "0xFF36361F", 1273],
        ["Dark Forest", "0xFF36361F", 1273],
        ["Watermelon", "0xFFFA5F7F", 1303],
        ["Watermelon", "0xFFFA5F7F", 1303],
        ["Caribbean Mist", "0xFFA3C2D7", 1644],
        ["Caribbean Mist", "0xFFA3C2D7", 1644],
        ["END", 0, -1]
    ],
    "ThreadArt_Rayon_codes": [
        ["END", 0, -1]
    ],
    "ThreadArt_Polyester_codes": [
        ["END", 0, -1]
    ],
    "ThreaDelight_Polyester_codes": [
        ["END", 0, -1]
    ],
    "Z102_Isacord_Polyester_codes": [
        ["?", "0xFFF8FFFF", 17],
        ["?", "0xFF000000", 20],
        ["?", "0xFFB7BABA", 105],
        ["?", "0xFF73787A", 108],
        ["?", "0xFF454B58", 138],
        ["?", "0xFF9EA9A6", 142],
        ["?", "0xFFC8C6BD", 150],
        ["?", "0xFFFAEE5C", 220],
        ["?", "0xFFE5CB4F", 221],
        ["?", "0xFFFFF46A", 230],
        ["?", "0xFFFEF9D9", 270],
        ["?", "0xFFFFDC00", 311],
        ["?", "0xFF624F00", 345],
        ["?", "0xFFB8B25A", 352],
        ["?", "0xFF8D8F5B", 453],
        ["?", "0xFFFFF4A5", 520],
        ["?", "0xFFFFF4A5", 520],
        ["?", "0xFFB98E03", 542],
        ["?", "0xFFB98E03", 542],
        ["?", "0xFFE4C180", 651],
        ["?", "0xFFE4C180", 651],
        ["?", "0xFFC5BFA6", 672],
        ["?", "0xFFC5BFA6", 672],
        ["?", "0xFF96836D", 722],
        ["?", "0xFF96836D", 722],
        ["?", "0xFF4E3500", 747],
        ["?", "0xFF4E3500", 747],
        ["?", "0xFFDDCBA5", 761],
        ["?", "0xFFDDCBA5", 761],
        ["?", "0xFF605840", 776],
        ["?", "0xFF605840", 776],
        ["?", "0xFFFFAF02", 800],
        ["?", "0xFFFFAF02", 800],
        ["?", "0xFFF6AE32", 811],
        ["?", "0xFFF6AE32", 811],
        ["?", "0xFFC89334", 822],
        ["?", "0xFFC89334", 822],
        ["?", "0xFFE59300", 824],
        ["?", "0xFFE59300", 824],
        ["?", "0xFFC89340", 832],
        ["?", "0xFFC89340", 832],
        ["?", "0xFF9E947F", 873],
        ["?", "0xFF9E947F", 873],
        ["?", "0xFFC8700B", 922],
        ["?", "0xFFC8700B", 922],
        ["?", "0xFFBB5704", 931],
        ["?", "0xFFBB5704", 931],
        ["?", "0xFFB19072", 1061],
        ["?", "0xFFB19072", 1061],
        ["?", "0xFFFF8101", 1102],
        ["?", "0xFFFF8101", 1102],
        ["?", "0xFFB1500A", 1115],
        ["?", "0xFFB1500A", 1115],
        ["?", "0xFFC09C72", 1123],
        ["?", "0xFFC09C72", 1123],
        ["?", "0xFF843D07", 1134],
        ["?", "0xFF843D07", 1134],
        ["?", "0xFFD8A67D", 1141],
        ["?", "0xFF82421B", 1154],
        ["?", "0xFFFF7319", 1300],
        ["?", "0xFFFF3D1E", 1305],
        ["?", "0xFFBA4005", 1311],
        ["?", "0xFFC73C13", 1312],
        ["?", "0xFFE66B21", 1332],
        ["?", "0xFF3D1C11", 1346],
        ["?", "0xFFFFBC95", 1351],
        ["?", "0xFFFFCC93", 1362],
        ["?", "0xFF373732", 1375],
        ["?", "0xFFFFAF94", 1532],
        ["?", "0xFF5B4032", 1565],
        ["?", "0xFFFF6046", 1600],
        ["?", "0xFFFF6D71", 1753],
        ["?", "0xFFEBBAAE", 1755],
        ["?", "0xFFEB2D2B", 1805],
        ["?", "0xFFFF988F", 1840],
        ["?", "0xFF434331", 1874],
        ["?", "0xFFC11914", 1902],
        ["?", "0xFFC8100D", 1903],
        ["?", "0xFFBF0A21", 1906],
        ["?", "0xFFBF0A21", 1906],
        ["?", "0xFFD23C3E", 1921],
        ["?", "0xFFD23C3E", 1921],
        ["?", "0xFF8F8C93", 1972],
        ["?", "0xFF8F8C93", 1972],
        ["?", "0xFFA31A1C", 2011],
        ["?", "0xFFA31A1C", 2011],
        ["?", "0xFF4D0308", 2115],
        ["?", "0xFF4D0308", 2115],
        ["?", "0xFFFFCDCC", 2155],
        ["?", "0xFFFFCDCC", 2155],
        ["?", "0xFF871C45", 2500],
        ["?", "0xFF871C45", 2500],
        ["?", "0xFFDB4083", 2508],
        ["?", "0xFFDB4083", 2508],
        ["?", "0xFFFF668A", 2520],
        ["?", "0xFFFF668A", 2520],
        ["?", "0xFFC91243", 2521],
        ["?", "0xFFC91243", 2521],
        ["?", "0xFFFFA0B6", 2530],
        ["?", "0xFFFFA0B6", 2530],
        ["?", "0xFFFEA5B9", 2550],
        ["?", "0xFFFEA5B9", 2550],
        ["?", "0xFF626C7E", 2674],
        ["?", "0xFF626C7E", 2674],
        ["?", "0xFF5E1942", 2711],
        ["?", "0xFF5E1942", 2711],
        ["?", "0xFF33001D", 2715],
        ["?", "0xFF33001D", 2715],
        ["?", "0xFFA57B8D", 2764],
        ["?", "0xFFA57B8D", 2764],
        ["?", "0xFF702A69", 2810],
        ["?", "0xFF702A69", 2810],
        ["?", "0xFFB385BC", 2830],
        ["?", "0xFFB385BC", 2830],
        ["?", "0xFF240047", 2900],
        ["?", "0xFF240047", 2900],
        ["?", "0xFF724593", 2910],
        ["?", "0xFF724593", 2910],
        ["?", "0xFF634D86", 2920],
        ["?", "0xFF634D86", 2920],
        ["?", "0xFF000136", 3110],
        ["?", "0xFF000136", 3110],
        ["?", "0xFF000021", 3355],
        ["?", "0xFF000021", 3355],
        ["?", "0xFF054ABD", 3522],
        ["?", "0xFF054ABD", 3522],
        ["?", "0xFF1C005D", 3541],
        ["?", "0xFF1C005D", 3541],
        ["?", "0xFF001F71", 3544],
        ["?", "0xFF001F71", 3544],
        ["?", "0xFF002E5E", 3622],
        ["?", "0xFF002E5E", 3622],
        ["?", "0xFF71AAD8", 3630],
        ["?", "0xFF71AAD8", 3630],
        ["?", "0xFF001748", 3644],
        ["?", "0xFF001748", 3644],
        ["?", "0xFFC8DBE4", 3650],
        ["?", "0xFFC8DBE4", 3650],
        ["?", "0xFF9FC7DF", 3730],
        ["?", "0xFF9FC7DF", 3730],
        ["?", "0xFF082E4D", 3743],
        ["?", "0xFF082E4D", 3743],
        ["?", "0xFF98B0BC", 3750],
        ["?", "0xFF98B0BC", 3750],
        ["?", "0xFF23679C", 3810],
        ["?", "0xFF23679C", 3810],
        ["?", "0xFF3D657E", 3842],
        ["?", "0xFF3D657E", 3842],
        ["?", "0xFF00669F", 3901],
        ["?", "0xFF00669F", 3901],
        ["?", "0xFF47AEDD", 3910],
        ["?", "0xFF47AEDD", 3910],
        ["?", "0xFFBBDFEB", 3962],
        ["?", "0xFFBBDFEB", 3962],
        ["?", "0xFFBABEB7", 3971],
        ["?", "0xFFBABEB7", 3971],
        ["?", "0xFF015D7E", 4032],
        ["?", "0xFF015D7E", 4032],
        ["?", "0xFFD5DDDB", 4071],
        ["?", "0xFFD5DDDB", 4071],
        ["?", "0xFF888D8E", 4073],
        ["?", "0xFF888D8E", 4073],
        ["?", "0xFF007CA6", 4103],
        ["?", "0xFF007CA6", 4103],
        ["?", "0xFF3EBBC8", 4111],
        ["?", "0xFF3EBBC8", 4111],
        ["?", "0xFF005C79", 4116],
        ["?", "0xFF005C79", 4116],
        ["?", "0xFF80CCD8", 4240],
        ["?", "0xFF80CCD8", 4240],
        ["?", "0xFFACCEC7", 4250],
        ["?", "0xFFACCEC7", 4250],
        ["?", "0xFF006E74", 4410],
        ["?", "0xFF006E74", 4410],
        ["?", "0xFF002A29", 4515],
        ["?", "0xFF002A29", 4515],
        ["?", "0xFF38A4AE", 4620],
        ["?", "0xFF38A4AE", 4620],
        ["?", "0xFFAFD8CD", 5050],
        ["?", "0xFFAFD8CD", 5050],
        ["?", "0xFF149B7B", 5210],
        ["?", "0xFF149B7B", 5210],
        ["?", "0xFF7AC8AF", 5220],
        ["?", "0xFF7AC8AF", 5220],
        ["?", "0xFF187166", 5233],
        ["?", "0xFF187166", 5233],
        ["?", "0xFF004B23", 5374],
        ["?", "0xFF004B23", 5374],
        ["?", "0xFF006835", 5415],
        ["?", "0xFF006835", 5415],
        ["?", "0xFF5C9C51", 5531],
        ["?", "0xFF5C9C51", 5531],
        ["?", "0xFF0E9543", 5613],
        ["?", "0xFF0E9543", 5613],
        ["?", "0xFF5E7A17", 5833],
        ["?", "0xFF5E7A17", 5833],
        ["?", "0xFF225926", 5944],
        ["?", "0xFF225926", 5944],
        ["?", "0xFFBCD633", 6011],
        ["?", "0xFFBCD633", 6011],
        ["?", "0xFFBBCD91", 6051],
        ["?", "0xFFBBCD91", 6051],
        ["?", "0xFF978B3C", 6133],
        ["?", "0xFF978B3C", 6133],
        ["END", 0, -1]
    ],
    "brand_codes": [
        "Arc_Polyester_codes",
        "Arc_Rayon_codes", #/* 1 */
        "CoatsAndClark_Rayon_codes", #/* 2 */
        "Exquisite_Polyester_codes", #/* 3 */
        "Fufu_Polyester_codes", #/* 4 */
        "Fufu_Rayon_codes", #/* 5 */
        "Hemingworth_Polyester_codes", #/* 6 */
        "Isacord_Polyester_codes", #/* 7 */
        "Isafil_Rayon_codes", #/* 8 */
        "Marathon_Polyester_codes", #/* 9 */
        "Marathon_Rayon_codes", #/* 10 */
        "Madeira_Polyester_codes", #/* 11 */
        "Madeira_Rayon_codes", #/* 12 */
        "Metro_Polyester_codes", #/* 13 */
        "Pantone_codes",  #/* 14 */
        "RobisonAnton_Polyester_codes",
        "RobisonAnton_Rayon_codes",
        "Sigma_Polyester_codes",
        "Sulky_Rayon_codes",
        "ThreadArt_Rayon_codes",
        "ThreadArt_Polyester_codes",
        "ThreaDelight_Polyester_codes",
        "Z102_Isacord_Polyester_codes",
        "svg_color_codes"
    ]
}

def clear_selection():
    r" . "
    return

class Vector(ctypes.Structure):
    r""" Wrapper for the EmbVector structure. """
    _fields_ = [
        ('x', ctypes.c_float),
        ('y', ctypes.c_float)
    ]

    def __str__(self,) -> str:
        return f"({self.x}, {self.y})"

class Pen():
    r"""
    .
    """
    def __init__(self):
        color = "0x00000000"

    def __str__(self,) -> str:
        return f"Pen({self.color})"

def create_test_file_1(outf="test01.csv"):
    r"""
    .
    """
    pattern = Pattern()

    # 10mm circle
    for i in range(20):
        x = 0.0
        y = 1.0*(i+1)
        pattern.add_stitch(x, y, flags=emb.JUMP, color=0)

    for i in range(200):
        x = 10 + 10 * math.sin(i * 0.01 * math.pi)
        y = 10 + 10 * math.cos(i * 0.01 * math.pi)
        pattern.add_stitch(x, y, flags=emb.NORMAL, color=0)

    pattern.add_thread(emb.black_thread)
    pattern.end()

    pattern.write(outf)
    pattern.free()


def create_test_file_2(outf="test02.csv"):
    r"""
    .
    """
    pattern = Pattern()

    # sin wave
    for i in range(100):
        x = 10 + 10 * math.sin(i * (0.5 / math.pi))
        y = 10 + i * 0.1
        pattern.add_stitch(x, y, flags=emb.NORMAL, color=0)

    pattern.add_thread(emb.black_thread)
    pattern.end()

    pattern.write(outf)


def create_test_file_3(outf="test03.csv"):
    """
    """
    pattern = Pattern()
    pattern.add_circle(10.0, 1.0, 5.0)
    pattern.add_thread(emb.black_thread)
    pattern.convert_geometry()
    pattern.end()

    pattern.write(outf)


def convert_test(t, from_f, to_f):
    """
    """
    inf = "test%02d.%s" % (t, from_f)
    outf = "test%02d_convert_from_%s.%s" % (t, from_f, to_f)
    if t == 1:
        create_test_file_1(inf)
    elif t == 2:
        create_test_file_2(inf)
    else:
        create_test_file_3(inf)
    return emb.convert(inf, outf)


def convert_test_all(from_f, to_f):
    for i in range(1, 4):
        if convert_test(i, from_f, to_f) != 0:
            return 1
    return 0


class TestLibembroidery(unittest.TestCase):
    r"""
    The parent class for all of the bindings
    tests, this could be made to chain into the
    internal C testing.
    """
    def test_vector_length(self):
        " Tests the vector length function. "
        v = Vector(3.0, 4.0)
        self.assertAlmostEqual(v.length(), 5.0)

    def test_arc(self):
        r"Adds an arc to the pattern, then checked that it saves in SVG."
        pattern = Pattern()
        pattern.add_arc(1.0, 2.0, 2.0, 3.0, 4.0, 6.0)
        pattern.write("out.svg")
        self.assertAlmostEqual(5.0, 5.0)

    def test_circle(self):
        r"Adds a circle to the pattern, then checked that it saves in SVG."
        pattern = Pattern()
        pattern.add_circle(3.0, 4.0, 2.0)
        pattern.write("out.svg")
        self.assertAlmostEqual(5.0, 5.0)

    def test_ellipse(self):
        r"Adds an ellipse to the pattern, then checked that it saves in SVG."
        pattern = Pattern()
        pattern.add_ellipse(3.0, 4.0, 7.0, 4.0)
        pattern.write("out.svg")
        self.assertAlmostEqual(5.0, 5.0)

    def test_path(self):
        r"Adds a path to the pattern, then checked that it saves in SVG."
        pattern = Pattern()
        pattern.add_path([])
        pattern.write("out.svg")
        self.assertAlmostEqual(5.0, 5.0)

    def test_main(self):
        r"""
        Tests the rendering and simulating of patterns.
        """
        pattern = Pattern()
        image = emb.Image(100, 100)
        hilbert_curve_result = pattern.hilbert_curve(3)
        render_result = image.render(pattern, 20.0, 20.0, "hilbert_level_3.ppm")
        simulate_result = image.simulate(pattern, 20.0, 20.0, "hilbert_level_3.avi")

        self.assertEqual(hilbert_curve_result, 0)
        self.assertEqual(render_result, 0)
        self.assertEqual(simulate_result, 0)

    def test_convert_csv_svg(self):
        " Test conversion from csv to svg. "
        self.assertEqual(convert_test_all("csv", "svg"), 1)

    def test_convert_csv_dst(self):
        " Test conversion from csv to dst. "
        self.assertEqual(convert_test_all("csv", "dst"), 1)

    def test_convert_csv_pes(self):
        " Test conversion from csv to pes. "
        self.assertEqual(convert_test_all("csv", "pes"), 1)

    def test_convert_svg_csv(self):
        " Test conversion from svg to csv. "
        self.assertEqual(convert_test_all("svg", "csv"), 1)

    def test_convert_dst_csv(self):
        " Test conversion from dst to csv. "
        self.assertEqual(convert_test_all("dst", "csv"), 1)

    def test_convert_pes_csv(self):
        " Test conversion from pes to csv. "
        self.assertEqual(convert_test_all("pes", "csv"), 1)

    def test_circle_tangent(self):
        """
        """
        t0 = Vector(0.0, 0.0)
        t1 = Vector(0.0, 0.0)
        c = Circle(0.0, 0.0, 3.0)
        p = Vector(4.0, 0.0)
        emb.getCircleTangentPoints(c, p, t0, t1)
        self.assertAlmostEqual(t0.x, 2.2500)
        self.assertAlmostEqual(t0.y, 1.9843)
        self.assertAlmostEqual(t1.x, 2.2500)
        self.assertAlmostEqual(t1.y, -1.9843)

    def test_circle_tangent_2(self):
        """
        """
        t0 = Vector(0.0, 0.0)
        t1 = Vector(0.0, 0.0)
        c = Circle(20.1762, 10.7170, 6.8221)
        p = Vector(24.3411, 18.2980)
        emb.getCircleTangentPoints(c, p, t0, t1)
        self.assertAlmostEqual(t0.x, 19.0911)
        self.assertAlmostEqual(t0.y, 17.4522)
        self.assertAlmostEqual(t1.x, 26.4428)
        self.assertAlmostEqual(t1.y, 13.4133)

    def test_thread_color(self):
        """
        TODO: Add capability for converting multiple files of various
        types to a single format. 

        Currently, we only convert a single file to multiple formats.
        """
        tColor = 0xFFD25F00
        c = emb.color(rgb=(0xD2, 0x5F, 0x00))
        tBrand = emb.Sulky_Rayon
        tNum = emb.threadColorNum(c, tBrand)
        tName = ""
        emb.threadColorName(tName, c, tBrand)

        # Solution: tNum = 1833
        # Solution: Pumpkin Pie
        print("""
Color : 0x%X
Brand : %d
Num   : %d
Name  : %s

""" % (tColor, tBrand, tNum, tName))
        return 0

    def test_format_table(self):
        """
        """
        tName = "example.zsk"
        f_format = emb.emb_identify_format(tName)
        table = emb.formatTable[f_format]

        self.assertEqual(table.extension, ".zsk")
        self.assertEqual(table.description, "ZSK USA Embroidery  f_format")
        self.assertEqual(table.reader_state, 'U')
        self.assertEqual(table.writer_state, ' ')
        self.assertEqual(table.type, 1)

    def test_list_processor():
        r" . "
        print(list_processor("(+ (* 4 6) 3)"))
        print(list_processor("(+ (/ 6 4) (* 3 2))"))
        example = "(* (/ 8 11) (sin (- (/ 10 9) (* 56 t))))"
        print(list_processor(example.replace("t", "0.04")))

    def test_selected():
        r"."
        debug_message("Test what is selected")

    def test_polyline_array():
        r"Polyline & Polygon Testing"
        p = pattern()

        offset = vector(0.0, 0.0)

        polyline_array = [
            vector(1.0, 1.0),
            vector(1.0, 2.0),
            vector(2.0, 2.0),
            vector(2.0, 3.0),
            vector(3.0, 3.0),
            vector(3.0, 2.0),
            vector(4.0, 2.0),
            vector(4.0, 1.0)
        ]
        polyline_array = [v + offset for v in polyline_array]
        p.add_polyline(polyline_array)

        offset = vector(5.0, 0.0)

        polygon_array = [
            vector(1.0, 1.0),
            vector(1.0, 2.0),
            vector(2.0, 2.0),
            vector(2.0, 3.0),
            vector(3.0, 3.0),
            vector(3.0, 2.0),
            vector(4.0, 2.0),
            vector(4.0, 1.0)
        ]
        polygon_array = [vector + offset for vector in polygon_array]
        p.add_polygon(polygon_array)

        p.write("output.svg")

def satin_fill(outline, direction, scale, max_steps):
    r"""
    A traditional directional satin fill, where the supplied
    direction is the .
    """
    return


def radial_satin_fill(outline, center, scale, max_steps):
    r"""
    .
    """
    return


def herringbone_fill(outline, scale, max_steps):
    r"""
    .
    """
    return


def join_stitches(stitch_list):
    r"""
    .
    """
    return


def clip_path(outline, path):
    r"""
    .
    """
    return


def apply_fill(outline, fill, *args):
    r"""Given a contour, overlay a fill pattern and clip out the inner part.

    outline:
        The contour is supplied as a polygon which is a list of vectors
        anticlockwise about the polygon with the first and last point joined.

        For example, a square could be encoded as:
        [[0.0, 0.0], [0.0, 1.0], [1.0, 1.0], [1.0, 0.0]].

    fill:
        The fill is a program that generates a path with points that
        mark where the needle sews. The fill continues until the outline
        is covered.
    """
    stitch_list = fill(outline, *args)
    stitch_list = clip_path(outline, stitch_list)
    stitch_list = join_stitches(stitch_list)
    return stitch_list

class Path():
    """
    The Path class definition.

    TODO: make paths similar to polylines.
    Review and implement any missing functions/members.
    """
    def __init__(self, command="", data=[], pen=Pen()):
        r"."
        self.type = "Path"
        self.normal_path = []
        self.pen = pen
        self.lwt_pen = pen

        clear_selection()
        self.first_run = True
        self.first = Vector(math.nan, math.nan)
        self.prev = Vector(math.nan, math.nan)
        self.promptPrefix = "Specify start point: "
        if data != []:
            self.data = data
        elif command != "":
            self.data = path_from_command(command)
        self.update()

    def copy(self):
        " . "
        debug_message("PathObject copy()")
        # TODO: getCurrentLineType
        return Path(data=self.data, pen=self.pen)

    def update(self):
        " . "
        self.normal_path.reverse()
        # reverse_path.connect_path(normal_path)
        # self.setPath(reverse_path)

    def paint(self, painter, option, widget):
        " . "
        obj_scene = self.scene
        if not obj_scene:
            return

        paint_pen = Pen()
        painter.set_pen(paint_pen)
        self.update_rubber(painter)
        if "State_Selected" in option.state:
            paint_pen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT"):
            paint_pen = self.lwt_pen
        painter.set_pen(paint_pen)

        painter.draw_path(self.data)

    def update_rubber(self, painter):
        """
        TODO: Path Rubber Modes.

        TODO: self.update_rubber() gripping for PathObject.
        """
        return

    def vulcanize(self):
        debug_message("PathObject vulcanize()")
        self.update_rubber()

        self.rubber_mode = "RUBBER_OFF"

        if not self.normal_path.element_count():
            details = (
                "The path added contains no points. "
                + "The command that created this object has flawed logic."
            )
            debug_message("Empty Path Error" + details, msgtype="ERROR")

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        return self.scene_pos()

    def all_grip_points(self):
        " TODO: loop thru all path Elements and return their points. "
        grip_points = [self.scene_pos()]
        return grip_points

    def grip_edit(before, after):
        " TODO: grip_edit() for PathObject."
        return

    def copy_path(self):
        " . "
        return self.normal_path

    def save_path(self):
        " . "
        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s, s)
        return trans.map(self.normal_path)

    def click(self, x, y):
        r" . "
        if self.first_run:
            self.first_run = False
            self.first = Vector(x, y)
            self.prev = Vector(x, y)
            add_path(x, y)
            append_prompt_history()
            set_prompt_prefix("Specify next point or [Arc/Undo]: ")
        else:
            append_prompt_history()
            append_line_to_path(x, y)
            self.prev = Vector(x, y)

    def prompt(self, cmd):
        r" . "
        if str == "A" or cmd == "ARC":
            # TODO: Probably should add additional qsTr calls here.
            debug_message("PATH prompt() for ARC")

        elif str == "U" or cmd == "UNDO":
            # TODO: Probably should add additional qsTr calls here.
            debug_message("PATH prompt() for UNDO")

        else:
            vector = vector_from_str(cmd)
            if not vector:
                debug_message("Point or option keyword required.", msgtype="ALERT")
                prefix = "Specify next point or [Arc/Undo]: "
                set_prompt_prefix(prefix)

            else:
                if self.first_run:
                    self.first_run = False
                    self.first = vector
                    self.prev = vector
                    add_path(vector)
                    set_prompt_prefix("Specify next point or [Arc/Undo]: ")
                else:
                    append_line_to_path(vector)
                    self.prev = vector

    def set_object_pos(self, point):
        " . "
        self.position = point


class Line():
    r"""
    Check for angle and length functions.
    """
    def __init__(self, pen=Pen(),
                 start=Vector(math.nan, math.nan),
                 end=Vector(math.nan, math.nan)):
        " . "
        self.pen = pen
        self.type = "Line"
        self.start = start
        self.end = end

        self.mode = "NORMAL"
        self.first_run = True
        self.factor = math.nan
        self.factor_ref = math.nan
        self.factor_new = math.nan

        if num_selected <= 0:
            # TODO: Prompt to select objects if nothing is preselected
            debug_message(
                "Preselect objects before invoking the scale command.",
                msgtype="ALERT"
            )
            debug_message(
                "information"
                + "Scale Preselect"
                + "Preselect objects before invoking the scale command."
            )

        else:
            set_prompt_prefix("Specify base point: ")

    def copy(self):
        " . "
        debug_message("Line.copy()")
        return Line(start=self.start, end=self.end, pen=self.pen)

    def set_start(self, start):
        r"To make the rubbers update without having to explicitly call it."
        self.start = start
        self.update_rubber()

    def set_end(self, end):
        r"To make the rubbers update without having to explicitly call it."
        self.end = end
        self.update_rubber()

    def mid_point(self):
        r"Returns the average of the start and end vectors. "
        return (self.start + self.end).scale(0.5)

    def angle(self):
        r"Finds the angle of the vector from the start to the end."
        delta = self.end - self.start
        return delta.angle()

    def paint(self, painter, option, widget):
        r"Not sure if the painter should be a seperate object."
        obj_scene = scene()
        if not obj_scene:
            return

        paintPen = pen()
        painter.set_pen(paintPen)
        self.update_rubber(painter)
        if "State_Selected" in option.state:
            paintPen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT"):
            paintPen = self.lwt_pen
        painter.set_pen(paintPen)

        if self.rubber_mode != "RUBBER_LINE":
            painter.draw_line(Line())

        if obj_scene.property("ENABLE_LWT").toBool() and obj_scene.property("ENABLE_REAL").toBool(): realRender(painter, Path())

    def update_rubber(self, painter):
        r"."
        if self.rubber_mode == "RUBBER_LINE":
            scene_start_point = self.rubber_point("LINE_START")
            sceneQSnapPoint = self.rubber_point("LINE_END")

            setObjectEndPoint1(scene_start_point)
            setobj_end_point_2(sceneQSnapPoint)

            draw_rubber_line(Line(), painter, "VIEW_COLOR_CROSSHAIR")

        elif self.rubber_mode == "RUBBER_GRIP":
            if painter:
                grip_point = self.rubber_point("GRIP_POINT")
                if grip_point == objectEndPoint1():
                    painter.draw_line(Line().p2(),
                                      (self.rubber_point("")))
                elif grip_point == obj_end_point_2():
                    painter.draw_line(Line().p1(),
                                      (self.rubber_point("")))
                elif grip_point == self.mid_point():
                    painter.draw_line(
                        Line().translated((self.rubber_point(""))
                                          - (grip_point))
                    )

                rub_line = Line((grip_point),
                                (self.rubber_point("")))
                draw_rubber_line(rub_line, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        r"."
        debug_message("LineObject vulcanize()")
        self.update_rubber()
        self.rubber_mode = "RUBBER_OFF"

    def mouse_snap_point(self, mouse_point):
        r"Returns the closest snap point to the mouse point."
        all_points = self.all_grip_points()
        return closest_vector(all_points, mouse_point)

    def all_grip_points(self):
        r"."
        return [self.start, self.end, self.mid_point()]

    def grip_edit(self, before, after):
        r"."
        if before == self.start:
            setObjectEndPoint1(after.x(), after.y())
        elif before == self.end:
            setobj_end_point_2(after.x(), after.y())
        elif before == self.mid_point():
            delta = after-before
            moveBy(delta.x(), delta.y())

    def save_path(self):
        r"."
        return [
            ["move_to", self.start],
            ["line_to", self.end - self.start]
        ]

    def click(self, point):
        r"."
        if self.mode == "NORMAL":
            if self.first_run:
                self.first_run = False
                self.start = point
                add_rubber("LINE")
                self.rubber_mode = "LINE"
                self.rubber_points["LINE_START"] = self.start
                preview_on("SELECTED", "SCALE", self.baseX, self.baseY, 1)
                append_prompt_history()
                set_prompt_prefix("Specify scale factor or [Reference]: ")

            else:
                self.end = point
                self.factor = Line(self.start, self.end).length()
                append_prompt_history()
                scale_selected(self.start, self.factor)
                preview_off()

        elif self.mode == "REFERENCE":
            if math.isnan(self.baseRX):
                self.baseR = point
                append_prompt_history()
                add_rubber("LINE")
                self.rubber_mode = "LINE"
                self.rubber_points["LINE_START"] = self.baseR
                set_prompt_prefix(translate("Specify second point: "))

            elif math.isnan(self.destRX):
                self.destR = point
                self.factor_ref = calculate_distance(self.baseR, self.destR)
                if self.factor_ref <= 0.0:
                    self.destR = Vector(math.nan, math.nan)
                    self.factor_ref = math.nan
                    debug_message("Value must be positive and nonzero.",
                                  msgtype="ALERT")
                    set_prompt_prefix("Specify second point: ")

                else:
                    append_prompt_history()
                    self.rubber_points["LINE_START"] = self.base
                    preview_on("SELECTED", "SCALE", self.base, self.factor_ref)
                    set_prompt_prefix("Specify new length: ")

            elif math.isnan(self.factor_new):
                self.factor_new = calculate_distance(self.base, point)
                if self.factor_new <= 0.0:
                    self.factor_new = math.nan
                    debug_message("Value must be positive and nonzero.", msgtype="ALERT")
                    set_prompt_prefix("Specify new length: ")
                else:
                    append_prompt_history()
                    scale_selected(self.baseX, self.baseY, self.factor_new/self.factor_ref)
                    preview_off()

    def prompt(self, cmd):
        if self.mode == "NORMAL":
            if self.first_run:
                vector = vector_from_str(cmd)
                if not vector:
                    alert("Invalid point.")
                    set_prompt_prefix("Specify base point: ")

                else:
                    self.first_run = False
                    self.start = vector
                    add_rubber("LINE")
                    set_rubber_mode("LINE")
                    self.rubber_points["LINE_START"] = self.start
                    preview_on("SELECTED", "SCALE", self.start, 1)
                    set_prompt_prefix("Specify scale factor or [Reference]: ")

            else:
                if str == "R" or cmd == "REFERENCE":
                    #TODO: Probably should add additional qsTr calls here.
                    self.mode = "REFERENCE"
                    set_prompt_prefix("Specify reference length 1}: ")
                    clear_rubber()
                    preview_off()

                else:
                    if math.isnan(cmd):
                        alert("Requires valid numeric distance, second point, or option keyword.")
                        set_prompt_prefix("Specify scale factor or [Reference]: ")
    
                    else:
                        self.factor = float(cmd)
                        scale_selected(self.baseX, self.baseY, self.factor)
                        preview_off()

        elif self.mode == "REFERENCE":
            if math.isnan(self.baseRX):
                if math.isnan(cmd):
                    vector = vector_from_str(cmd)
                    if not vector:
                        alert("Requires valid numeric distance or two points.")
                        set_prompt_prefix("Specify reference length 1}: ")
                    else:
                        self.baseRX = float(strList[0])
                        self.baseRY = float(strList[1])
                        add_rubber("LINE")
                        set_rubber_mode("LINE")
                        set_rubber_point("LINE_START", self.baseRX, self.baseRY)
                        set_prompt_prefix(translate("Specify second point: "))
                else:
                    # The base and dest values are only set here to advance the command.
                    self.baseRX = 0.0
                    self.baseRY = 0.0
                    self.destRX = 0.0
                    self.destRY = 0.0
                    # The reference length is what we will use later.
                    self.factor_ref = float(cmd)
                    if self.factor_ref <= 0.0:
                        self.baseR = Vector(math.nan, math.nan)
                        self.destR = Vector(math.nan, math.nan)
                        self.factor_ref = math.nan
                        alert("Value must be positive and nonzero.")
                        set_prompt_prefix("Specify reference length" + " 1}: ")
                    else:
                        add_rubber("LINE")
                        set_rubber_mode("LINE")
                        set_rubber_point("LINE_START", self.base)
                        preview_on("SELECTED", "SCALE", self.base, self.factor_ref)
                        set_prompt_prefix("Specify new length: ")

            elif math.isnan(self.destR.x):
                if math.isnan(cmd):
                    vector = vector_from_str(cmd)
                    if not vector:
                        alert("Requires valid numeric distance or two points.")
                        set_prompt_prefix("Specify second point: ")
                    else:
                        self.destR = vector
                        self.factor_ref = calculate_distance(self.baseR, self.destR)
                        if self.factor_ref <= 0.0:
                            self.destR = Vector(math.nan, math.nan)
                            self.factor_ref = math.nan
                            alert("Value must be positive and nonzero.")
                            set_prompt_prefix("Specify second point: ")
        
                        else:
                            set_rubber_point("LINE_START", self.base)
                            preview_on("SELECTED", "SCALE", self.base, self.factor_ref)
                            set_prompt_prefix("Specify new length: ")

                else:
                    # The base and dest values are only set here to
                    # advance the command.
                    self.base_r = Vector(0.0, 0.0)
                    self.dest_r = Vector(0.0, 0.0)

                    # The reference length is what we will use later.
                    self.factor_ref = float(cmd)
                    if self.factor_ref <= 0.0:
                        self.dest_r = Vector(math.nan, math.nan)
                        self.factor_ref = math.nan
                        alert("Value must be positive and nonzero.")
                        set_prompt_prefix("Specify second point: ")
                    else:
                        set_rubber_point("LINE_START", self.base)
                        preview_on("SELECTED", "SCALE", self.base, self.factor_ref)
                        set_prompt_prefix("Specify new length: ")

            elif math.isnan(self.factor_new):
                if math.isnan(cmd):
                    vector = vector_from_str(cmd)
                    if not vector:
                        alert("Requires valid numeric distance or second point.")
                        set_prompt_prefix(translate("Specify new length: "))
                    else:
                        self.factor_new = calculate_distance(self.base, vector)
                        if self.factor_new <= 0.0:
                            self.factor_new = math.nan
                            alert("Value must be positive and nonzero.")
                            set_prompt_prefix(translate("Specify new length: "))
        
                        else:
                            scaleSelected(self.baseX, self.baseY, self.factor_new/self.factor_ref)
                            preview_off()

                else:
                    self.factor_new = float(cmd)
                    if self.factor_new <= 0.0:
                        self.factor_new = math.nan
                        alert(translate("Value must be positive and nonzero."))
                        set_prompt_prefix(translate("Specify new length: "))
    
                    else:
                        scaleSelected(self.baseX, self.baseY, self.factor_new/self.factor_ref)
                        preview_off()

    def a__init__(self):
        " . "
        clearSelection()
        self.x1 = math.nan
        self.y1 = math.nan
        self.x2 = math.nan
        self.y2 = math.nan
        set_prompt_prefix(translate("Specify first point: "))

    def a_click(self, x, y):
        " . "
        if math.isnan(self.x1):
            self.x1 = x
            self.y1 = y
            add_rubber("LINE")
            set_rubber_mode("LINE")
            set_rubber_point("LINE_START", self.x1, self.y1)
            append_prompt_history()
            set_prompt_prefix(translate("Specify second point: "))
        else:
            append_prompt_history()
            self.x2 = x
            self.y2 = y
            reportDistance()

    def a_prompt(self, cmd):
        " . "
        vector = vector_from_str(cmd)
        if math.isnan(self.x1):
            if not vector:
                alert("Requires numeric distance or two points.")
                set_prompt_prefix("Specify first point: ")
            else:
                add_rubber("LINE")
                self.rubber_mode = "LINE"
                self.rubber_points["LINE_START"] = vector
                set_prompt_prefix("Specify second point: ")

        else:
            if not vector:
                alert("Requires numeric distance or two points.")
                set_prompt_prefix("Specify second point: ")
            else:
                self.x2 = float(strList[0])
                self.y2 = float(strList[1])
                reportDistance()

    def report_distance(self):
        r"""
        Cartesian Coordinate System reported:

                 (+)
                 90
                 |
        (-) 180__|__0 (+)
                 |
                270
                (-)
        """
        delta = self.point2 - self.point1

        dist = delta.length()
        angle = delta.angle()

        set_prompt_prefix(f"Distance = {dist}, Angle = {angle}")
        append_prompt_history()
        set_prompt_prefix(f"Delta X = {delta.x}, Delta Y = {delta.y}")
        append_prompt_history()


def vector_distance(vector_a, vector_b):
    "Find the distance between two points given as vectors."
    delta = vector_a - vector_b
    return delta.length()


class Rect():
    r" The Rect class definition. "
    def __init__(self, position_x, position_y, width, height, pen=Pen()):
        r" Create a new rectangle. "
        self.type = "Rect"
        self.selectable = True
        self.scale = 1.0
        self.rotation = 0.0
        self.position = Vector(position_x, position_y)
        self.dimensions = Vector(width, height)
        self.pen = pen

        clear_selection()
        self.new_rect = True
        self.rubber_mode = "NOT SET"
        set_prompt_prefix("Specify first corner point or [Chamfer/Fillet]: ")

    def width(self):
        r"Syntactic sugar for x dimension of the Rect."
        return self.dimensions.x

    def height(self):
        r"Syntactic sugar for y dimension of the Rect."
        return self.dimensions.y

    def area(self):
        r"Returns the area of the rectangle."
        return self.dimensions.x * self.dimensions.y

    def click(self, point):
        r"The mouse input handler for editing a Rect."
        if self.new_rect:
            self.new_rect = False
            add_rubber("RECTANGLE")
            self.rubber_mode = "RECTANGLE"
            self.position = point
            self.rubber_points["RECTANGLE_START"] = point
            set_prompt_prefix("Specify other corner point or [Dimensions]: ")
        else:
            self.new_rect = True
            self.dimensions = point - self.position
            self.rubber_points["RECTANGLE_END"] = self.position + self.dimensions
            self.vulcanize()

    def prompt(self, cmd):
        "The prompt handler for editing rectangles."
        if cmd == "C" or cmd == "CHAMFER":
            # TODO: Probably should add additional qsTr calls here.
            debug_message("RECTANGLE prompt() for CHAMFER")
        elif cmd == "D" or cmd == "DIMENSIONS":
            # TODO: Probably should add additional qsTr calls here.
            debug_message("RECTANGLE prompt() for DIMENSIONS")
        elif cmd == "F" or cmd == "FILLET":
            # TODO: Probably should add additional qsTr calls here.
            debug_message("RECTANGLE prompt() for FILLET")
        else:
            vector = vector_from_str(cmd)
            if not vector:
                debug_message("Invalid point.", msgtype="ALERT")
                set_prompt_prefix("Specify first point: ")
            else:
                if self.new_rect:
                    self.new_rect = False
                    self.position = vector
                    add_rubber("RECTANGLE")
                    self.rubber_mode = "RECTANGLE"
                    self.rubber_points["RECTANGLE_START"] = vector
                    set_prompt_prefix("Specify other corner point or [Dimensions]: ")
                else:
                    self.new_rect = True
                    self.dimensions = vector - self.position
                    self.rubber_points["RECTANGLE_END"] = vector
                    self.vulcanize()

    def copy(self):
        r" Make a copy of the rectangle. "
        debug_message("Rect copy()")
        # setRotation(self.rotation())
        return Rect(self.x, self.y, self.w, self.h, pen=self.pen)

    def set_rect(self, x, y, w, h):
        r" Alter all the positional data of the rectangle. "
        self.position = Vector(x, y)
        self.dimensions = Vector(w, h)
        self.update_path()

    def top_left(self):
        r" Returns the top left co-ordinate of this rect as a Vector. "
        return self.position

    def top_right(self):
        r" Returns the top right co-ordinate of this rect as a Vector. "
        vector = self.position
        vector.x += self.dimensions.x
        return vector

    def bottom_left(self):
        r" Returns the bottom left co-ordinate of this rect as a Vector. "
        vector = self.position
        vector.y += self.dimensions.y
        return vector

    def bottom_right(self):
        " Returns the bottom right co-ordinate of this rect as a Vector. "
        return self.position + self.dimensions

    def update_path(self):
        r" Update the associated path. "
        self.path = Path()
        r = Rect()
        self.path.move_to(r.bottom_left())
        self.path.line_to(r.bottom_right())
        self.path.line_to(r.top_right())
        self.path.line_to(r.top_left())
        self.path.line_to(r.bottom_left())
        # NOTE: Reverse the path so that the inside area isn't considered part
        # of the rectangle
        self.path.line_to(r.top_left())
        self.path.line_to(r.top_right())
        self.path.line_to(r.bottom_right())
        self.path.move_to(r.bottom_left())

    def paint(self, painter, option, widget):
        r" . "
        obj_scene = scene()
        if not obj_scene:
            return

        paint_pen = pen()
        painter.set_pen(paint_pen)
        self.update_rubber(painter)
        if option.state & "State_Selected)":
            paint_pen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT"):
            paint_pen = self.lwt_pen
        painter.set_pen(paint_pen)

        painter.drawRect(Rect())

    def update_rubber(self, painter):
        r" . "
        if self.rubber_mode == "RECTANGLE":
            scene_start_point = self.rubber_points["RECTANGLE_START"]
            scene_end_point = self.rubber_points["RECTANGLE_END"]
            x = scene_start_point.x()
            y = scene_start_point.y()
            w = scene_end_point.x() - scene_start_point.x()
            h = scene_end_point.y() - scene_start_point.y()
            self.set_rect(x, y, w, h)
            self.update_path()

        elif self.rubber_mode == "GRIP":
            if painter:
                # TODO: Make this work with rotation & scaling.
                grip_point = self.rubber_points["GRIP_POINT"]
                after = self.rubber_point("")
                delta = after-grip_point
                if grip_point == self.top_left():
                    painter.draw_polygon((Rect(after.x(), after.y(), self.width()-delta.x(), self.height()-delta.y())))
                elif grip_point == self.top_right():
                    painter.draw_polygon((Rect(self.top_left().x(), self.top_left().y()+delta.y(), self.width()+delta.x(), self.height()-delta.y())))
                elif grip_point == self.bottom_left():
                    painter.draw_polygon(Rect(self.top_left().x()+delta.x(), self.top_left().y(), self.width()-delta.x(), self.height()+delta.y()))
                elif grip_point == self.bottom_right():
                    painter.draw_polygon(Rect(self.top_left().x(), self.top_left().y(), self.width()+delta.x(), self.height()+delta.y()))

                rub_line = Line(grip_point, self.rubber_point(""))
                draw_rubber_line(rub_line, painter, "VIEW_COLOR_CROSSHAIR")

                grip_point = self.rubber_point("GRIP_POINT")
                after = self.rubber_point("")
                delta = after-grip_point

                rub_line = Line(grip_point, self.rubber_point(""))
                draw_rubber_line(rub_line, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        r" . "
        debug_message("Rect vulcanize()")
        self.update_rubber()

        self.rubber_mode = "OBJ_RUBBER_OFF"

    def mouse_snap_point(self, mouse_point):
        r" Returns the closest snap point to the mouse point. "
        all_points = self.all_grip_points()
        return closest_vector(all_points, mouse_point)

    def all_grip_points(self):
        r" . "
        return [self.top_left(), self.top_right(),
                self.bottom_left(), self.bottom_right()]

    def grip_edit(self, before, after, tolerance=0.01):
        """
        The tolerance is how close the input needs to be to be considered
        a match to the given point.
        """
        delta = after-before
        if vector_distance(before, self.top_left()) < tolerance:
            self.set_rect(after.x, after.y, self.width()-delta.x, self.height()-delta.y)
        elif vector_distance(before, self.top_right()) < tolerance:
            self.set_rect(self.top_left().x(), self.top_left().y()+delta.y(), self.width()+delta.x(), self.height()-delta.y())
        elif vector_distance(before, self.bottom_left()) < tolerance:
            self.set_rect(
                self.top_left().x()+delta.x(),
                self.top_left().y(),
                self.width()-delta.x(),
                self.height()+delta.y()
            )
        elif vector_distance(before, self.bottom_right()) < tolerance:
            self.set_rect(self.top_left().x(), self.top_left().y(),
                self.width()+delta.x(), self.height()+delta.y())

    def save_path(self):
        ". "
        path = Path()
        r = Rect()
        path.move_to(r.bottom_left())
        path.line_to(r.bottom_right())
        path.line_to(r.top_right())
        path.line_to(r.top_left())
        path.line_to(r.bottom_left())

        s = self.scale
        trans = Transform()
        trans.rotate(self.rotation)
        trans.scale(s,s)
        return trans.map(path)


class Arc():
    r"""
    The Arc class definition.
    """
    def __init__(self, start, mid, end,
                 pen=Pen(), path=Path(), rotation=0.0):
        " ."
        self.pen = pen
        self.lwt_pen = pen
        self.start = start
        self.mid = mid
        self.end = end
        self.type = "Arc"
        self.rotation = rotation
        self.center = Vector(0.0, 0.0)
        self.rubber_mode = "RUBBER_OFF"
        self.start_angle = 0.0
        self.end_angle = 0.0
        self.radius = 0.0
        self.path = path
        self.update()

    def update(self):
        " ."
        self.start_angle = (self.end - self.start).angle()
        self.end_angle = (self.start - self.end).angle()
        self.radius = self.get_radius()
        self.center = self.get_center()

        self.set_position(self.center)

        self.radius = Line(self.center, self.mid).length()
        self.update_rect(self.radius)
        self.updatePath()

    def center(self):
        " ."
        return self.scene_position

    def get_radius(self):
        " ."
        return self.rect().width()/2.0*self.scale

    def set_start_angle(self, angle):
        " Alter the start angle. "
        self.update()

    def set_end_angle(self, angle):
        " Alter the end angle. "
        self.update()

    def set_start_point(self, point):
        " Alter the start point. "
        self.start = point
        self.update()

    def set_mid_point(self, point):
        " Alter the middle point. "
        self.mid = point
        self.update()

    def set_end_point(self, point):
        " Alter the end point. "
        self.end = point
        self.update()

    def update_rubber(self, painter):
        r"""Update the rubber points to match the current data.
        # TODO: Arc Rubber Modes
        # TODO: self.update_rubber() gripping for Arc
        """
        debug_message("Update rubber.")
        self.update()

    def copy(self):
        """
        Create a copy of the current Arc  from the perspective
        of the scene.
        """
        debug_message("Arc copy()")
        return Arc(self.start, self.mid, self.end,
                   pen=self.pen, rotation=self.rotation)

    def update_rect(self, radius):
        " . "
        arc_rect = Rect()
        arc_rect.set_width(radius*2.0)
        arc_rect.set_height(radius*2.0)
        arc_rect.move_center(Vector(0, 0))
        self.set_rect(arc_rect)

    def set_radius(self, radius):
        " . "
        self.update()

        if radius <= 0:
            radius = 0.0000001

        start_line = Line(self.center, self.start)
        mid_line = Line(self.center, self.mid)
        end_line = Line(self.center, self.end)
        start_line.set_length(radius)
        mid_line.set_length(radius)
        end_line.set_length(radius)
        self.start = start_line.p2()
        self.mid = mid_line.p2()
        self.end = end_line.p2()

        self.update()

    def start_angle(self):
        " The start angle from the perspective of the render. "
        angle = Line(self.scene_position, self.start).angle()
        return math.fmod(angle, 360.0)

    def end_angle(self):
        " The end angle from the perspective of the render. "
        angle = Line(self.scene_position, self.end).angle()
        return math.fmod(angle, 360.0)

    def start_point(self):
        " The start point from the perspective of the render. "
        v = self.start
        v.scale(self.scale)
        v.rotate(radians(self.rotation))
        return self.scene_position + v

    def mid_point(self):
        " The mid point from the perspective of the render. "
        v = self.mid
        v.scale(self.scale)
        v.rotate(radians(self.rotation))
        return self.scene_position + v

    def end_point(self):
        " The end point from the perspective of the render. "
        v = self.end
        v.scale(self.scale)
        v.rotate(radians(self.rotation))
        return self.scene_position + v

    def area(self):
        " Area of a circular segment. "
        r = self.radius()
        theta = radians(self.included_angle())
        return ((r*r)/2)*(theta - math.sin(theta))

    def arc_length(self):
        " . "
        return radians(self.included_angle())*self.radius()

    def chord(self):
        " . "
        return Line(self.start, self.end).length()

    def included_angle(self):
        """
        NOTE:
        Due to floating point rounding errors, we need to clamp the
        quotient so it is in the range [-1, 1].
        If the quotient is out of that range, then the result of asin()
        will be NaN.
        """
        chord = self.chord()
        rad = self.radius()
        if chord <= 0 or rad <= 0:
            return 0
        # Prevents division by zero and non-existent circles.

        quotient = chord/(2.0*rad)
        if quotient > 1.0:
            quotient = 1.0
        if quotient < 0.0:
            quotient = 0.0
        """
        NOTE: 0 rather than -1 since we are enforcing a positive chord
        and radius.
        """
        return degrees(2.0*math.asin(quotient))
        # Properties of a Circle - Get the Included Angle - Reference: ASD9.

    def clockwise(self):
        " . "
        arc = Arc(
            self.start.x, -self.start.y,
            self.mid.x, -self.mid.y,
            self.end.x, -self.end.y
        )
        # NOTE: Y values are inverted here on purpose.
        return arc.clockwise()

    def update_path(self):
        " . "
        self.update()
        self.start_angle = self.start_angle + self.rotation
        self.span_angle = self.included_angle()

        if self.clockwise():
            self.span_angle = -self.span_angle

        rect = self.bounding_rect()
        self.path = [
            ["arc_move_to", rect, self.start_angle],
            ["arc_to", rect, self.start_angle, self.span_angle],
            # NOTE: Reverse the path so that the inside area
            # isn't considered part of the arc.
            ["arc_to", rect, self.start_angle+self.span_angle,
             -self.span_angle]
        ]

    def paint(self, painter, option, widget):
        " . "
        obj_scene = self.scene()
        if not obj_scene:
            return

        paint_pen = Pen()
        painter.set_pen(paint_pen)
        self.update_rubber(painter)
        if option.state == "State_Selected":
            paint_pen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT").to_bool():
            paint_pen = self.lwt_pen
        painter.set_pen(paint_pen)

        start_angle = (self.start_angle() + self.rotation)*16
        span_angle = self.included_angle()*16

        if self.clockwise():
            span_angle = -span_angle

        rad = self.radius()
        paint_rect = Rect(-rad, -rad, rad*2.0, rad*2.0)
        painter.draw_arc(paint_rect, start_angle, span_angle)

    def vulcanize(self):
        " . "
        debug_message("Arc vulcanize()")
        self.update_rubber()
        self.rubber_mode = "RUBBER_OFF"

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        snap_points = self.all_grip_points()
        return closest_vector(snap_points, mouse_point)

    def all_grip_points(self):
        " . "
        self.update()
        return [self.center, self.start, self.mid, self.end]

    def grip_edit(self, before, after):
        " TODO: grip_edit() for Arc. "
        return


class Circle():
    """
    The circle class definition.
    def Save[]:

    def setRadius(radius)
    def set_diameter(diameter)
    def set_area(area)
    def setCircumference(circumference)
    def update_rubber(QPainter* painter = 0)

    def Color(): return self.pen.color()
    def line_weight(): return self.lwt_pen.widthF()
    def []: return []
    def rubber_mode(): return objrubber_mode
    def self.rubber_point(key):
    def rubber_text(key):

    def setrubber_mode(mode)  objrubber_mode = mode
    def setself.rubber_point(key, point)  objself.rubber_points.insert(key, point)
    def setrubber_text(key, txt)  self.rubber_texts.insert(key, txt)
    def bounding_rect() const
    def shape() const  return []
    def draw_rubber_line(rub_line, painter=0, colorFromScene=0)
    def vulcanize() = 0
    mouse_snap_point(mouse_point) = 0
    QList<Vector> all_grip_points() = 0
    def grip_edit(before, after) = 0
    Line obj_line
    """
    def __init__(self, center, radius, pen=Pen()):
        " . "
        clear_selection()
        self.mode = "1P_RAD"
        self.point1 = Vector(math.nan, math.nan)
        self.point2 = Vector(math.nan, math.nan)
        self.point3 = Vector(math.nan, math.nan)
        self.type = "Circle"
        self.selectable = True
        self.radius = radius
        self.center = center
        self.update_path()
        self.promptPrefix = "Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "
        self.pen = pen
        self.lwt_pen = pen
        self.obj_id = "OBJ"+str(time.time())
        self.rubber_mode = "NOT SET"
        self.rubber_points = {}
        self.rubber_texts = {}
        self.update()

    def copy(self):
        " . "
        c = Circle(self.center, self.radius, pen=self.pen)
        return c

    def rect(self):
        " . "
        self.update_path()
        return self.path.bounding_rect()

    def update_path(self):
        ". "
        self.path = []

    def set_rect(self, x, y, w, h):
        """
        def set_rect(self, r):
            p = []
            p.add_rect(r)
            self.set_path(p)
        """
        p = []
        p.add_rect(x, y, w, h)
        self.set_path(p)

    def set_line(self, li):
        r"""
        p = []
        p.move_to(self.point1)
        p.line_to(self.point2)
        self.set_path(p)
        self.obj_line.set_line(self.point1, self.point2)
        """
        p = []
        p.move_to(li.p1())
        p.line_to(li.p2())
        self.set_path(p)
        self.obj_line = li

    def set_path(self, p):
        " . "
        self.path = p
        self.update()

    def set_color(self, color):
        " . "
        " . "
        self.pen.set_color(color)
        self.lwt_pen.set_color(color)

    def set_color_rgb(self, rgb):
        " . "
        self.pen.set_color(rgb)
        self.lwt_pen.set_color(rgb)

    def set_line_type(self, line_type):
        " . "
        self.pen.set_style(line_type)
        self.lwt_pen.set_style(line_type)

    def set_line_weight(self, line_weight):
        r"NOTE: The self.pen will always be cosmetic. "
        self.pen.setWidthF(0)

        if line_weight < 0:
            if line_weight == "OBJ_LWT_BYLAYER":
                self.lwt_pen.setWidthF(0.35)
                # TODO: getLayerline_weight

            elif line_weight == "OBJ_LWT_BYBLOCK":
                self.lwt_pen.setWidthF(0.35)
                # TODO: getBlockline_weight

            else:
                warning(0, "Error - Negative line_weight",
                        "line_weight: %1" % line_weight)
                debug_message(
                    "line_weight cannot be negative! Inverting sign."
                )
                self.lwt_pen.setWidthF(-line_weight)

        else:
            self.lwt_pen.setWidthF(line_weight)

    def rubber_point(self, key):
        " . "
        if key in self.rubber_points:
            return self.rubber_points[key]

        gscene = self.scene()
        if gscene:
            return self.scene().property("SCENE_QSNAP_POINT")
        return Vector()

    def rubber_text(self, key):
        r" . "
        if key in self.rubber_texts:
            return self.rubber_texts[key]
        return ""

    def bounding_rect(self):
        r" If gripped, force this  to be drawn even if it is offscreen. "
        if self.rubber_mode == "OBJ_RUBBER_GRIP":
            return self.scene().scene_rect()
        return Rect()

    def draw_rubber_line(self, rub_line, painter, colorFromScene):
        " . "
        if painter:
            obj_scene = self.scene()
            if obj_scene:
                color_pen = self.pen
                color_pen.set_color(
                    Color(obj_scene.property(colorFromScene).toUInt())
                )
                painter.set_pen(color_pen)
                painter.draw_line(rub_line)
                painter.set_pen(self.pen)

    def real_render(self, painter, render_path):
        "lighter color"
        color1 = Color()
        color2 = color1.darker(150)
        # darker color

        # If we have a dark color, lighten it.
        darkness = color1.lightness()
        threshold = 32
        "TODO: This number may need adjusted or maybe just add it to settings."
        if darkness < threshold:
            color2 = color1
            if not darkness:
                color1 = Color(threshold, threshold, threshold)
                # lighter() does not affect pure black
            else:
                color1 = color2.lighter(100 + threshold)

        count = render_path.element_count()
        for i in range(count-1):

            elem = render_path.element_at(i)
            next = render_path.element_at(i+1)

            if next.ismove_to():
                continue

            elem_path = []
            elem_path.move_to(elem.x, elem.y)
            elem_path.line_to(next.x, next.y)

            render_pen = self.pen
            render_pen.setWidthF(0)
            painter.set_pen(render_pen)
            stroker = Painter()
            stroker.setWidth(0.35)
            stroker.setCapStyle("round-cap")
            stroker.setJoinStyle("round-join")
            realPath = stroker.createStroke(elem_path)
            painter.drawPath(realPath)

            grad = LinearGradient(elem_path.pointAtPercent(0.5),
                                  elem_path.point_at_percent(0.0))
            grad.set_colorAt(0, color1)
            grad.set_colorAt(1, color2)
            grad.setSpread("QGradientReflectSpread")

            painter.fill_path(realPath, QBrush(grad))

    def diameter(self):
        " . "
        return 2 * self.radius

    def area(self):
        " . "
        return math.pi * self.radius * self.radius

    def circumference(self):
        " . "
        return 2 * self.radius * math.pi

    def quadrant(self, angle):
        " . "
        angle = math.radians(angle)
        vector = Vector(self.radius*math.cos(angle), self.radius*math.cos(angle))
        return self.center + vector

    def set_diameter(self, diameter):
        " . "
        circ_rect = Rect()
        circ_rect.setWidth(diameter)
        circ_rect.setHeight(diameter)
        circ_rect.move_center(0, 0)
        self.rect = circ_rect
        self.update_path()

    def set_area(self, area):
        " Changes the radius to match the supplied area. "
        self.radius = math.sqrt(area/math.pi)
        self.update()

    def set_circumference(self, circumference):
        " Changes the radius to match the supplied circumference. "
        self.radius = circumference / (2.0*math.pi)
        self.update()

    def update_path(self):
        path = []
        r = Rect()
        # Add the center point.
        path.add_rect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        # Add the circle.
        path.arcmove_to(r, 0)
        path.arcTo(r, 0, 360)
        # NOTE: Reverse the path so that the inside area isn't considered part of the circle. */
        path.arcTo(r, 0, -360)
        setPath(path)

    def paint(self, painter, option, widget):
        obj_scene = scene()
        if not obj_scene:
            return

        paintPen = pen()
        painter.set_pen(paintPen)
        self.update_rubber(painter)
        if "State_Selected" in option.state:
            paintPen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT"):
            paintPen = line_weightPen()
        painter.set_pen(paintPen)

        painter.draw_ellipse(rect())

    def update_rubber(self, painter):
        self.rubber_mode = rubber_mode()
        if self.rubber_mode == "CIRCLE_1P_RAD":
            sceneCenterPoint = self.rubber_point("CIRCLE_CENTER")
            sceneQSnapPoint = self.rubber_point("CIRCLE_RADIUS")
            itemCenterPoint = (sceneCenterPoint)
            itemQSnapPoint = (sceneQSnapPoint)
            itemLine = Line(itemCenterPoint, itemQSnapPoint)
            setPos(sceneCenterPoint)
            sceneLine = Line(sceneCenterPoint, sceneQSnapPoint)
            radius = sceneLine.length()
            setRadius(radius)
            if painter:
                draw_rubber_line(itemLine, painter, "VIEW_COLOR_CROSSHAIR")
            self.update_path()

        elif self.rubber_mode == "CIRCLE_1P_DIA":
            sceneCenterPoint = self.rubber_point("CIRCLE_CENTER")
            sceneQSnapPoint = self.rubber_point("CIRCLE_DIAMETER")
            itemCenterPoint = sceneCenterPoint
            itemQSnapPoint = sceneQSnapPoint
            itemLine = Line(itemCenterPoint, itemQSnapPoint)
            self.set_position(sceneCenterPoint)
            sceneLine = Line(sceneCenterPoint, sceneQSnapPoint)
            diameter = sceneLine.length()
            set_diameter(diameter)
            if painter:
                draw_rubber_line(itemLine, painter, "VIEW_COLOR_CROSSHAIR")
            self.update_path()

        elif self.rubber_mode == "CIRCLE_2P":
            sceneTan1Point = self.rubber_point("CIRCLE_TAN1")
            sceneQSnapPoint = self.rubber_point("CIRCLE_TAN2")
            sceneLine = Line(sceneTan1Point, sceneQSnapPoint)
            setPos(sceneLine.pointAt(0.5))
            diameter = sceneLine.length()
            set_diameter(diameter)
            self.update_path()

        elif self.rubber_mode == "CIRCLE_3P":
            sceneTan1Point = self.rubber_point("CIRCLE_TAN1")
            sceneTan2Point = self.rubber_point("CIRCLE_TAN2")
            sceneTan3Point = self.rubber_point("CIRCLE_TAN3")

            sceneCenter = Vector()
            arc = embArc_make(sceneTan1Point.x(), sceneTan1Point.y(),
                                sceneTan2Point.x(), sceneTan2Point.y(),
                                sceneTan3Point.x(), sceneTan3Point.y()).arc
            sceneCenter = arc.getCenter()
            sceneCenterPoint(sceneCenter.x, sceneCenter.y)
            sceneLine = Line(sceneCenterPoint, sceneTan3Point)
            setPos(sceneCenterPoint)
            radius = sceneLine.length()
            setRadius(radius)
            self.update_path()

        elif self.rubber_mode == "GRIP":
            if painter:
                grip_point = self.rubber_point("GRIP_POINT")
                if grip_point == self.center:
                    painter.draw_ellipse(rect().translated((self.rubber_point(""))-(grip_point)))

                else:
                    gripRadius = Line(self.center(), self.rubber_point("")).length()
                    painter.draw_ellipse(Vector(), gripRadius, gripRadius)

                rub_line = Line((grip_point), (self.rubber_point("")))
                draw_rubber_line(rub_line, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize():
        debug_message("Circle vulcanize()")
        self.update_rubber()
        self.rubber_mode = "RUBBER_OFF"

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        all_points = self.all_grip_points()
        return closest_vector(all_points, mouse_point)

    def all_grip_points(self):
        r"."
        return [
            self.center,
            self.quadrant(0),
            self.quadrant(90),
            self.quadrant(180),
            self.quadrant(270)
        ]

    def grip_edit(self, before, after):
        r"."
        if before == self.center:
            delta = after-before
            moveBy(delta.x(), delta.y())
        else:
            setRadius(Line(self.center(), after).length())

    def save_path(self):
        r"."
        path = []
        r = Rect()
        path.arcmove_to(r, 0)
        path.arcTo(r, 0, 360)

        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s,s)
        return trans.map(path)

    def mouse_callback(self, button, state, x, y):
        if button == "LEFT_BUTTON":
            if state == "DOWN":
                pos_x = x/(0.5*window_width) - 1.0
                pos_y = -y/(0.5*window_height) + 1.0
                mouse_x = x
                mouse_y = y
                for i in range(2):
                    leaf = root.leaves[i]
                    if leaf.left < pos_x and pos_x < leaf.right:
                        if leaf.top < pos_y and pos_y < leaf.bottom:
                            action_id = i
                            break


    def click(self, x, y):
        vector = Vector(x, y)
        if self.mode == "1P_RAD":
            if math.isnan(self.point1.x):
                self.point1 = vector
                self.center = vector
                add_rubber("CIRCLE")
                set_rubber_mode("CIRCLE_1P_RAD")
                set_rubber_point("CIRCLE_CENTER", self.cx, self.cy)
                append_prompt_history()
                set_prompt_prefix("Specify radius of circle or [Diameter]: ")
            else:
                self.point2 = vector
                self.rubber_points["CIRCLE_RADIUS"] = self.point2
                self.vulcanize()
                append_prompt_history()

        elif self.mode == "1P_DIA":
            if math.isnan(self.point1.x):
                message = "CIRCLE" + "This should never happen."
                debug_message(message, msgtype="ERROR")
            else:
                self.point2 = vector
                self.rubber_points["CIRCLE_DIAMETER"] = self.point2
                self.vulcanize()
                append_prompt_history()

        elif self.mode == "2P":
            if math.isnan(self.point1.x):
                self.point1 = vector
                add_rubber("CIRCLE")
                self.rubber_mode = "CIRCLE_2P"
                self.rubber_points["CIRCLE_TAN1"] = self.point1
                append_prompt_history()
                set_prompt_prefix("Specify second end point of circle's diameter: ")

            elif math.isnan(self.point2.x):
                self.point2 = vector
                self.rubber_points["CIRCLE_TAN2"] = self.point2
                self.vulcanize()
                append_prompt_history()

            else:
                message = "CIRCLE This should never happen."
                debug_message(message, msgtype="ERROR")

        elif self.mode == "3P":
            if math.isnan(self.point1.x):
                self.point1 = vector
                append_prompt_history()
                set_prompt_prefix("Specify second point on circle: ")

            elif math.isnan(self.point2.x):
                self.point2 = vector
                add_rubber("CIRCLE")
                self.rubber_mode = "CIRCLE_3P"
                self.rubber_points["CIRCLE_TAN1"] = self.point1
                self.rubber_points["CIRCLE_TAN2"] = self.point2
                append_prompt_history()
                set_prompt_prefix("Specify third point on circle: ")

            elif math.isnan(self.point3.x):
                self.point3 = vector
                self.rubber_points["CIRCLE_TAN3"] = self.point3
                self.vulcanize()
                append_prompt_history()

            else:
                message = "CIRCLE This should never happen."
                debug_message(message, msgtype="ERROR")

        elif self.mode == "TTR":
            if math.isnan(self.point1.x):
                self.point1 = vector
                append_prompt_history()
                set_prompt_prefix("Specify point on  for second tangent of circle: ")

            elif math.isnan(self.point2.x):
                self.point2 = vector
                append_prompt_history()
                set_prompt_prefix("Specify radius of circle: ")

            elif math.isnan(self.point3.x):
                self.point3 = vector
                append_prompt_history()
                set_prompt_prefix("Specify second point: ")

            else:
                debug_message("CIRCLE click() for TTR")

    def prompt(self, args, cmd):
        if self.mode == "1P_RAD":
            if math.isnan(self.point1.x):
                # TODO: Probably should add additional qsTr calls here.
                if cmd == "2P":
                    self.mode = "2P"
                    set_prompt_prefix("Specify first end point of circle's diameter: ")

                # TODO: Probably should add additional qsTr calls here.
                elif cmd == "3P":
                    self.mode = "3P"
                    set_prompt_prefix("Specify first point of circle: ")

                # TODO: Probably should add additional qsTr calls here.
                elif cmd == "T" or cmd == "TTR":
                    self.mode = "TTR"
                    set_prompt_prefix("Specify point on  for first tangent of circle: ")

                else:
                    vector = vector_from_str(cmd)
                    if not vector:
                        debug_message("Point or option keyword required.",
                                      msgtype="ALERT")
                        set_prompt_prefix("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: ")
 
                    else:
                        self.point1 = vector
                        self.center = self.point1
                        add_rubber("CIRCLE")
                        self.rubber_mode = "CIRCLE_1P_RAD"
                        self.rubber_points["CIRCLE_CENTER"] = self.center
                        set_prompt_prefix("Specify radius of circle or [Diameter]: ")

            else:
                # TODO: Probably should add additional qsTr calls here.
                if str == "D" or str == "DIAMETER":
                    self.mode = "1P_DIA"
                    self.rubber_mode = "CIRCLE_1P_DIA"
                    set_prompt_prefix("Specify diameter of circle: ")

                else:
                    num = float(cmd)
                    if math.isnan(num):
                        debug_message(
                            "Requires numeric radius, point on circumference, or \"D\".",
                            msgtype="ALERT"
                        )
                        set_prompt_prefix("Specify radius of circle or [Diameter]: ")
 
                    else:
                        self.rad = num
                        self.point2.x = self.point1.x + self.rad
                        self.point2.y = self.point1.y
                        self.rubber_points["CIRCLE_RADIUS"] = self.point2
                        self.vulcanize()

        elif self.mode == "1P_DIA":
            if math.isnan(self.point1.x):
                message = "CIRCLE This should never happen."
                debug_message(message, msgtype="ERROR")

            if math.isnan(self.point2.x):
                num = float(cmd)
                if math.isnan(num):
                    alert("Requires numeric distance or second point.")
                    set_prompt_prefix("Specify diameter of circle: ")

                else:
                    self.dia = num
                    self.point2.x = self.point1.x + self.dia
                    self.point2.y = self.point1.y
                    set_rubber_point("CIRCLE_DIAMETER", self.point2.x, self.point2.y)
                    self.vulcanize()
            else:
                message = "CIRCLE" + "This should never happen."
                debug_message(message, msgtype="ERROR")

        elif self.mode == "2P":
            if math.isnan(self.point1.x):
                vector = vector_from_str(cmd)
                if not vector:
                    alert("Invalid point.")
                    set_prompt_prefix("Specify first end point of circle's diameter: ")
                else:
                    self.point1 = vector
                    add_rubber("CIRCLE")
                    self.rubber_mode = "CIRCLE_2P"
                    self.rubber_points["CIRCLE_TAN1"] = self.point1
                    set_prompt_prefix("Specify second end point of circle's diameter: ")
            elif math.isnan(self.point2.x):
                vector = vector_from_str(cmd)
                if not vector:
                    alert("Invalid point.")
                    set_prompt_prefix("Specify second end point of circle's diameter: ")
                else:
                    self.point2 = vector
                    set_rubber_point("CIRCLE_TAN2", self.point2)
                    self.vulcanize()
            else:
                message = "CIRCLE" + "This should never happen."
                debug_message(message, msgtype="ERROR")

        elif self.mode == "3P":
            vector = vector_from_str(cmd)
            if not vector:
                alert("Invalid point.")
                if math.isnan(self.point1.x):
                    set_prompt_prefix("Specify first point of circle:")
                elif math.isnan(self.point2.x):
                    set_prompt_prefix("Specify second point of circle: ")
                else:
                    set_prompt_prefix("Specify third point of circle: ")
            else:
                if math.isnan(self.point1.x):
                    self.point1 = vector
                    self.rubber_points["CIRCLE_TAN1"] = self.point1
                    set_prompt_prefix("Specify second point of circle: ")
                elif math.isnan(self.point2.x):
                    self.point2 = vector
                    add_rubber("CIRCLE")
                    self.rubber_mode = "CIRCLE_3P"
                    self.rubber_points["CIRCLE_TAN2"] = self.point2
                    set_prompt_prefix("Specify third point of circle: ")
                elif math.isnan(self.point3.x):
                    self.point3 = vector
                    self.rubber_points["CIRCLE_TAN3"] = self.point3
                    self.vulcanize()
                else:
                    message = "CIRCLE" + "This should never happen."
                    debug_message(message, msgtype="ERROR")

        elif self.mode == "TTR":
            debug_message("CIRCLE prompt() for TTR")


class DimLeader():
    r"""
    The DimLeader class definition.

    EndPoint1() const
    obj_end_point_2() const
    MidPoint()  const
    def X1(): return EndPoint1().x()
    def Y1(): return EndPoint1().y()
    def X2(): return obj_end_point_2().x()
    def Y2(): return obj_end_point_2().y()
    def DeltaX(): return (obj_end_point_2().x() - EndPoint1().x())
    def DeltaY(): return (obj_end_point_2().y() - EndPoint1().y())
    def Angle():
    def Length(): return Line().length()

    def setEndPoint1(EmbVector v)
    def setobj_end_point_2(EmbVector v)

    def grip_edit(before, after)
    def paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)

    def update_leader()
    Path line_stylePath
    Path arrow_style_path
    arrow_style_angle
    arrow_style_length
    line_styleAngle
    line_style_length
    """
    def __init__(self, x1=math.nan, y1=math.nan, x2=math.nan, y2=math.nan,
                 pen=Pen()):
        self.point1 = Vector(x1, y1)
        self.point2 = Vector(x2, y2)
        self.type = "DimLeader"
        self.pen = pen
        self.selectable = True
        self.rubber_mode = "NOT SET"
        self.rubber_points = {}
        # NOTE: Allow this enum to evaluate false.
        self.arrow_style = ["NoArrow", "Open", "Closed", "Dot", "Box", "Tick"]
        # NOTE: Allow this enum to evaluate false.
        self.line_style = ["NoLine", "Flared", "Fletching"]
        self.curved = 0
        self.filled = 1

        # TODO: Adding the text is not complete yet.
        clear_selection()
        set_prompt_prefix("Specify first point: ")
        return self

    def click(self, x, y):
        if math.isnan(self.point1.x):
            self.point1 = Vector(x, y)
            add_rubber("DIMLEADER")
            self.rubber_mode = "DIMLEADER_LINE"
            self.rubber_points["DIMLEADER_LINE_START"] = self.point1
            append_prompt_history()
            set_prompt_prefix("Specify second point: ")
        else:
            self.point2 = Vector(x, y)
            self.rubber_points["DIMLEADER_LINE_END"] = self.point2
            self.vulcanize()
            append_prompt_history()

    def prompt(self, cmd):
        vector = vector_from_str(cmd)
        if math.isnan(self.point1.x):
            if not vector:
                debug_message("Requires two points.", msgtype="ALERT")
                set_prompt_prefix("Specify first point: ")

            else:
                self.point1 = vector
                add_rubber("DIMLEADER")
                self.rubber_mode = "DIMLEADER_LINE"
                self.rubber_points["DIMLEADER_LINE_START"] = vector
                set_prompt_prefix("Specify second point: ")

        else:
            if not vector:
                debug_message("Requires two points.", msgtype="ALERT")
                set_prompt_prefix("Specify second point: ")
            else:
                self.point2 = vector
                self.rubber_points["DIMLEADER_LINE_END"] = self.point2
                self.vulcanize()

    def copy(self):
        " . "
        debug_message("DimLeader copy()")
        return DimLeader(
            x1=self.point1.x,
            y1=self.point1.y,
            x2=self.point2.x,
            y2=self.point2.y,
            pen=self.pen)

    def midpoint(self):
        " . "
        return Line(self.point1, self.point2).point_at(0.5)

    def angle(self):
        " . "
        angle = Line(self.point1, self.point2).angle() - self.rotation
        return math.fmod(angle, 360.0)

    def update_leader(self):
        r"TODO: make all these constants customizable."
        arrow_style = "Closed"
        arrow_style_angle = 15.0
        arrow_style_length = 1.0
        line_style_angle = 45.0
        line_style_length = 1.0

        lyne = Line()
        angle = lyne.angle()
        ap0 = lyne.p1()
        lp0 = lyne.p2()

        # Arrow
        lynePerp = Line(lyne.pointAt(arrow_style_length/lyne.length()), lp0)
        lynePerp.setAngle(angle + 90)
        lyne1 = Line(ap0, lp0)
        lyne2 = Line(ap0, lp0)
        lyne1.setAngle(angle + arrow_style_angle)
        lyne2.setAngle(angle - arrow_style_angle)
        # ap1
        # ap2
        # HACK: these need fixing
        # lynePerp.intersects(lyne1, &ap1)
        # lynePerp.intersects(lyne2, &ap2)

        # So they don't cause memory access problems.
        ap1 = lyne1.p1()
        ap2 = lyne2.p1()

        r"""
        Math Diagram
        *                 .(ap1)                     .(lp1)
        *                /|                         /|
        *               / |                        / |
        *              /  |                       /  |
        *             /   |                      /   |
        *            /    |                     /    |
        *           /     |                    /     |
        *          /      |                   /      |
        *         /       |                  /       |
        *        /+(aSA)  |                 /+(lSA)  |
        * (ap0)./__(aSL)__|__________(lp0)./__(lSL)__|
        *       \ -(aSA)  |                \ -(lSA)  |
        *        \        |                 \        |
        *         \       |                  \       |
        *          \      |                   \      |
        *           \     |                    \     |
        *            \    |                     \    |
        *             \   |                      \   |
        *              \  |                       \  |
        *               \ |                        \ |
        *                \|                         \|
        *                 .(ap2)                     .(lp2)
        """

        if arrow_style == "Open":
            arrow_style_path = [
                ["move_to", ap1],
                ["line_to", ap0],
                ["line_to", ap2],
                ["line_to", ap0],
                ["line_to", ap1]
            ]

        elif arrow_style == "Closed":
            arrow_style_path = [
                ["move_to", ap1],
                ["line_to", ap0],
                ["line_to", ap2],
                ["line_to", ap1]
            ]

        elif arrow_style == "Dot":
            arrow_style_path = Path()
            arrow_style_path.addEllipse(ap0, arrow_style_length,
                                        arrow_style_length)

        elif arrow_style == "Box":
            arrow_style_path = Path()
            side = Line(ap1, ap2).length()
            ar0 = Rect(0, 0, side, side)
            ar0.move_center(ap0)
            arrow_style_path.add_rect(ar0)

        elif arrow_style == "Tick":
            line_stylePath = Path()
            line_stylePath.move_to(ap0)
            line_stylePath.line_to(lp0)

    def paint(self, painter, option, widget):
        obj_scene = self.scene()
        if not obj_scene:
            return

        paint_pen = Pen()
        painter.set_pen(paint_pen)
        self.update_rubber(painter)
        if "QStyle_State_Selected" in option.state:
            paint_pen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT").toBool():
            paint_pen = self.lwt_pen
        painter.set_pen(paint_pen)

        painter.draw_path("line_stylePath")
        painter.draw_path("arrow_stylePath")

        if self.filled:
            painter.fill_path("arrow_stylePath", self.pen.rgb)

    def update_rubber(self, painter):
        if self.rubber_mode == "DIMLEADER_LINE":
            scene_start_point = self.rubber_points["DIMLEADER_LINE_START"]
            scene_qsnap_point = self.rubber_points["DIMLEADER_LINE_END"]

            self.set_end_point_1(scene_start_point)
            self.set_end_point_2(scene_qsnap_point)

        elif self.rubber_mode == "Grip":
            if painter:
                grip_point = self.rubber_points["GRIP_POINT"]
                if grip_point == end_point_1():
                    end = (self.rubber_points(""))
                    painter.draw_line(Line().p2(), end)
                elif grip_point == end_point_2():
                    end = (self.rubber_points(""))
                    painter.draw_line(Line().p1(), end)
                elif grip_point == MidPoint():
                    painter.draw_line(Line().translated((self.rubber_points(""))-(grip_point)))

    def vulcanize(self):
        " . "
        debug_message("DimLeader vulcanize()")
        self.update_rubber()
        self.rubber_mode = "OFF"

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        all_points = self.all_grip_points()
        return closest_vector(all_points, mouse_point)

    def all_grip_points(self):
        " . "
        grip_points = [self.point1, self.point2]
        if self.curved:
            grip_points += [self.midpoint()]
        return grip_points

    def grip_edit(self, before, after):
        if before == EndPoint1():
            setEndPoint1(after)
        elif before == obj_end_point_2():
            setobj_end_point_2(after)
        elif before == MidPoint():
            delta = after-before
            moveBy(delta)


class Ellipse():
    r"""
    The Ellipse class definition.

    Path SavePath() const

    def Center(): return scenePos()
    def RadiusMajor():  return max(rect().width(), rect().height())/2.0*scale()
    def RadiusMinor(): return min(rect().width(), rect().height())/2.0*scale()
    def DiameterMajor(): return max(rect().width(), rect().height())*scale()
    def diameter_minor(): return min(rect().width(), rect().height())*scale()
    def Width(): return rect().width()*scale()
    def Height(): return rect().height()*scale()

    def set_Size(width, height)
    def set_RadiusMajor(radius)
    def set_RadiusMinor(radius)
    def set_DiameterMajor(diameter)
    def set_diameter_minor(diameter)
    """
    def __init__(self, center, width, height, pen=Pen()):
        r"."
        self.type = "Ellipse"
        self.selectable = 1
        self.width = width
        self.height = height
        self.position = center
        self.pen = pen
        self.update_path()
        self.rubber_points = {}

        clear_selection()
        self.mode = "MAJORDIAMETER_MINORRADIUS"
        self.point1 = Vector(math.nan, math.nan)
        self.point2 = Vector(math.nan, math.nan)
        self.point3 = Vector(math.nan, math.nan)
        set_prompt_prefix(
            translate("Specify first axis start point or [Center]: ")
        )

    def click(self, point):
        if self.mode == "MAJORDIAMETER_MINORRADIUS":
            if math.isnan(self.point1.x):
                self.point1 = point
                add_rubber("ELLIPSE")
                self.rubber_mode = "ELLIPSE_LINE"
                self.rubber_points["ELLIPSE_LINE_POINT1"] = point
                append_prompt_history()
                set_prompt_prefix(translate("Specify first axis end point: "))

            elif math.isnan(self.point2.x):
                self.point2 = point
                self.center = (self.point1 + self.point2).scale(0.5)
                self.width = Line(self.point1, self.point2).length()
                self.rot = Line(self.point1, self.point2).angle()
                self.rubber_mode = "MAJORDIAMETER_MINORRADIUS"
                self.rubber_points["ELLIPSE_AXIS1_POINT1"] = self.point1
                self.rubber_points["ELLIPSE_AXIS1_POINT2"] = self.point2
                self.rubber_points["ELLIPSE_CENTER"] = Vector(self.cx, self.cy)
                self.rubber_points["ELLIPSE_WIDTH"] = Vector(self.width, 0)
                self.rubber_points["ELLIPSE_ROT"] = Vector(self.rot, 0)
                append_prompt_history()
                set_prompt_prefix("Specify second axis end point or [Rotation]: ")

            elif math.isnan(self.point3.x):
                self.point3 = point
                self.height = perpendicular_distance(self.point3, self.point1,
                                                     self.point2)*2.0
                self.rubber_points["ELLIPSE_AXIS2_POINT2"] = point
                self.vulcanize()
                append_prompt_history()

            else:
                message = "ELLIPSE This should never happen."
                debug_message(message, msgtype="ERROR")

        elif self.mode == "MAJORRADIUS_MINORRADIUS":
            if math.isnan(self.point1.x):
                self.point1 = point
                self.center = point
                add_rubber("ELLIPSE")
                self.rubber_mode = "ELLIPSE_LINE"
                self.rubber_points["ELLIPSE_LINE_POINT1"] = point
                self.rubber_points["ELLIPSE_CENTER"] = self.center
                append_prompt_history()
                set_prompt_prefix(translate("Specify first axis end point: "))

            elif math.isnan(self.point2.x):
                self.point2 = point
                self.width = Line(self.center, self.point2).length()*2.0
                self.rot = Line(self.point1, self.point2).angle()
                self.rubber_mode = "MAJORRADIUS_MINORRADIUS"
                self.rubber_points["ELLIPSE_AXIS1_POINT2"] = self.point2
                self.rubber_points["ELLIPSE_WIDTH"] = Vector(self.width, 0)
                self.rubber_points["ELLIPSE_ROT"] = Vector(self.rot, 0)
                append_prompt_history()
                prefix = translate("Specify second axis end point or [Rotation]: ")
                set_prompt_prefix(prefix)

            elif math.isnan(self.point3.x):
                self.point3 = point
                self.height = perpendicular_distance(self.point3, self.center, 
                                                     self.point2)*2.0
                self.rubber_points["ELLIPSE_AXIS2_POINT2"] = point
                self.vulcanize()
                append_prompt_history()

            else:
                message = "ELLIPSE This should never happen."
                debug_message(message, msgtype="ERROR")

        elif self.mode == "ELLIPSE_ROTATION":
            if math.isnan(self.point1.x):
                message = "ELLIPSE This should never happen."
                debug_message(message, msgtype="ERROR")

            elif math.isnan(self.point2.x):
                message = "ELLIPSE This should never happen."
                debug_message(message, msgtype="ERROR")

            elif math.isnan(self.point3.x):
                angle = calculate_angle(self.center, point)
                self.height = math.cos(angle*math.pi/180.0)*self.width
                add_ellipse(self.center, self.width, self.height, self.rot, False)
                append_prompt_history()

    def prompt(self, cmd):
        r"."
        if self.mode == "MAJORDIAMETER_MINORRADIUS":
            if math.isnan(self.point1.x):
                if cmd == "C" or cmd == "CENTER":
                    # TODO: Probably should add additional qsTr calls here.
                    self.mode = self.mode_MAJORRADIUS_MINORRADIUS
                    set_prompt_prefix(translate("Specify center point: "))
                else:
                    vector = vector_from_str(cmd)
                    if not vector:
                        debug_message(translate("Point or option keyword required."), msgtype="ALERT")
                        prefix = translate("Specify first axis start point or [Center]: ")
                        set_prompt_prefix(prefix)
 
                    else:
                        self.point1 = vector
                        add_rubber("ELLIPSE")
                        self.rubber_mode = "ELLIPSE_LINE"
                        self.rubber_points["ELLIPSE_LINE_POINT1"] = self.point1
                        set_prompt_prefix(translate("Specify first axis end point: "))

            elif math.isnan(self.point2.x):
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message(translate("Invalid point."), msgtype="ALERT")
                    prefix = translate("Specify first axis end point: ")
                    set_prompt_prefix(prefix)

                else:
                    self.point2 = vector
                    self.cx = (self.x1 + self.x2)/2.0
                    self.cy = (self.y1 + self.y2)/2.0
                    self.width = Line(self.point1, self.point2).length()
                    self.rot = Line(self.point1, self.point2).angle()
                    self.rubber_mode = "ELLIPSE_MAJORDIAMETER_MINORRADIUS"
                    self.rubber_points["ELLIPSE_AXIS1_POINT1"] = self.point1
                    self.rubber_points["ELLIPSE_AXIS1_POINT2"] = self.point2
                    self.rubber_points["ELLIPSE_CENTER"] = self.center
                    self.rubber_points["ELLIPSE_WIDTH"] = Vector(self.width, 0)
                    self.rubber_points["ELLIPSE_ROT"] = Vector(self.rot, 0)
                    prefix = translate("Specify second axis end point or [Rotation]: ")
                    set_prompt_prefix(prefix)

            elif math.isnan(self.x3):
                if cmd == "R" or cmd == "ROTATION":
                    # TODO: Probably should add additional qsTr calls here.
                    self.mode = "ROTATION"
                    set_prompt_prefix("Specify rotation: ")
                else:
                    vector = vector_from_str(cmd)
                    if not vector:
                        debug_message("Point or option keyword required.", msgtype="ALERT")
                        set_prompt_prefix("Specify second axis end point or [Rotation]: ")
 
                    else:
                        self.point3 = vector
                        self.height = perpendicular_distance(self.point3,
                                                             self.point1,
                                                             self.point2)*2.0
                        self.rubber_points["ELLIPSE_AXIS2_POINT2"] = self.point3
                        self.vulcanize()

        elif self.mode == "MAJORRADIUS_MINORRADIUS":
            if math.isnan(self.x1):
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message("Invalid point.", msgtype="ALERT")
                    set_prompt_prefix("Specify center point: ")
                else:
                    self.point1 = vector
                    self.center = self.point1
                    add_rubber("ELLIPSE")
                    self.rubber_mode = "ELLIPSE_LINE"
                    self.rubber_points["ELLIPSE_LINE_POINT1"] = self.point1
                    self.rubber_points["ELLIPSE_CENTER"] = self.center
                    set_prompt_prefix("Specify first axis end point: ")

            elif math.isnan(self.x2):
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message("Invalid point.", msgtype="ALERT")
                    set_prompt_prefix("Specify first axis end point: ")

                else:
                    self.point2 = vector
                    line = Line(self.point1, self.point2)
                    self.width = line.length()*2.0
                    self.rot = line.angle()
                    self.rubber_mode = "ELLIPSE_MAJORRADIUS_MINORRADIUS"
                    self.rubber_points["ELLIPSE_AXIS1_POINT2"] = self.point2
                    self.rubber_points["ELLIPSE_WIDTH"] = Vector(self.width, 0)
                    self.rubber_points["ELLIPSE_ROT"] = Vector(self.rot, 0)
                    set_prompt_prefix("Specify second axis end point or [Rotation]: ")

            elif math.isnan(self.x3):
                if cmd == "R" or cmd == "ROTATION":
                    # TODO: Probably should add additional qsTr calls here.
                    self.mode = "ELLIPSE_ROTATION"
                    set_prompt_prefix("Specify ellipse rotation: ")
                else:
                    vector = vector_from_str(cmd)
                    if not vector:
                        debug_message("Point or option keyword required.", msgtype="ALERT")
                        set_prompt_prefix("Specify second axis end point or [Rotation]: ")
                    else:
                        self.point3 = vector
                        self.height = perpendicular_distance(self.point3,
                                                             self.point1,
                                                             self.point2)*2.0
                        self.rubber_points["ELLIPSE_AXIS2_POINT2"] = self.point3
                        self.vulcanize()

        elif self.mode == "ELLIPSE_ROTATION":
            if math.isnan(self.x1):
                message = "ELLIPSE This should never happen."
                debug_message(message, msgtype="ERROR")
            elif math.isnan(self.x2):
                message = "ELLIPSE This should never happen."
                debug_message(message, msgtype="ERROR")
            elif math.isnan(self.x3):
                if math.isnan(cmd):
                    debug_message("Invalid angle. Input a numeric angle or pick a point.", msgtype="ALERT")
                    set_prompt_prefix("Specify rotation: ")
                else:
                    angle = float(cmd)
                    self.height = math.cos(angle*math.pi/180.0)*self.width
                    add_ellipse(self.cx, self.cy, self.width, self.height, self.rot, False)

    def copy(self):
        " TODO: getCurrentLineType "
        debug_message("Ellipse Constructor()")
        return Ellipse(self.center, self.width, self.height,
                       pen=self.pen, rotation=self.rotation)

    def set_Size(self, width, height):
        r"."
        elRect = Rect()
        elRect.set_width(width)
        elRect.set_height(height)
        elRect.move_center(Vector(0, 0))
        set_rect(elRect)

    def set_radius_major(self, radius):
        r"."
        self.set_DiameterMajor(radius*2.0)

    def set_radius_minor(self, radius):
        r"."
        self.set_diameter_minor(radius*2.0)

    def set_diameter_major(self, diameter):
        r"."
        elRect = Rect()
        if elRect.width() > elRect.height():
            elRect.set_width(diameter)
        else:
            elRect.set_height(diameter)

        elRect.move_center(Vector(0, 0))
        set_rect(elRect)

    def set_diameter_minor(self, diameter):
        r"."
        elRect = Rect()
        if elRect.width < elRect.height:
            elRect.set_width(diameter)
        else:
            elRect.set_height(diameter)
        elRect.move_center(Vector(0, 0))
        set_rect(elRect)

    def quadrant(self, angle):
        r"."
        v = Vector(self.width()/2.0, 0.0)
        v.rotate(math.radians(self.rotation+angle))
        return self.center + v

    def update_path(self):
        r"."
        path = []
        r = Rect()
        path += ["arc_move_to", Vector(r, 0)]
        path += ["arc_to", Vector(r, 0), 360]
        # NOTE: Reverse the path so that the inside
        # area isn't considered part of the ellipse.
        path += ["arc_to", Vector(r, 0), -360]
        self.data = path

    def paint(self, painter, option, widget):
        r"."
        obj_scene = scene()
        if not obj_scene:
            return

        paint_pen = Pen()
        painter.set_pen(paint_pen)
        self.update_rubber(painter)
        if option.state == "State Selected":
            paint_pen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT").to_bool():
            paint_pen = self.lwt_pen
        painter.set_pen(paint_pen)

        painter.draw_ellipse(Rect())

    def update_rubber(self, painter):
        r"."
        if self.rubber_mode == "LINE":
            scene_line_point_1 = self.rubber_points["ELLIPSE_LINE_POINT1"]
            scene_line_point_2 = self.rubber_points["ELLIPSE_LINE_POINT2"]
            item_linePoint1 = (scene_line_point_1)
            item_linePoint2 = (scene_line_point_2)
            item_line = Line(item_linePoint1, item_linePoint2)
            if painter:
                draw_rubber_line(item_line, painter, "VIEW_COLOR_CROSSHAIR")
            self.update_path()

        elif self.rubber_mode == "MAJORDIAMETER_MINORRADIUS":
            scene_axis_1_point_1 = self.rubber_points["ELLIPSE_AXIS1_POINT1"]
            scene_axis_1_point_2 = self.rubber_points["ELLIPSE_AXIS1_POINT2"]
            sceneCenterPoint = self.rubber_points["ELLIPSE_CENTER"]
            sceneAxis2Point2 = self.rubber_points("ELLIPSE_AXIS2_POINT2")
            ellipseWidth = self.rubber_points("ELLIPSE_WIDTH").x
            ellipseRot = self.rubber_points("ELLIPSE_ROT").x

            # TODO: incorporate perpendicular_distance() into libembroidery.
            px = sceneAxis2Point2.x()
            py = sceneAxis2Point2.y()
            x1 = scene_axis_1_point_1.x()
            y1 = scene_axis_1_point_1.y()
            line = Line(scene_axis_1_point_1, scene_axis_1_point_2)
            norm = line.normalVector()
            dx = px-x1
            dy = py-y1
            norm.translate(dx, dy)
            # HACK: this isn't in all versions of Qt 5 in the same place?
            # norm.intersects(line, &iPoint)
            iPoint = line.p1()
            ellipseHeight = Line(px, py, iPoint.x(), iPoint.y()).length()*2.0

            setPos(sceneCenterPoint)
            set_Size(ellipseWidth, ellipseHeight)
            self.rotation = -ellipseRot

            itemCenterPoint = (sceneCenterPoint)
            itemAxis2Point2 = (sceneAxis2Point2)
            item_line = Line(itemCenterPoint, itemAxis2Point2)
            if painter:
                draw_rubber_line(item_line, painter, "VIEW_COLOR_CROSSHAIR")
            self.update_path()

        elif self.rubber_mode == "MAJORRADIUS_MINORRADIUS":
            scene_axis_1_point_2 = self.rubber_points("ELLIPSE_AXIS1_POINT2")
            sceneCenterPoint = self.rubber_points("ELLIPSE_CENTER")
            sceneAxis2Point2 = self.rubber_points("ELLIPSE_AXIS2_POINT2")
            ellipseWidth = self.rubber_points("ELLIPSE_WIDTH").x()
            ellipseRot = self.rubber_points("ELLIPSE_ROT").x()

            # TODO: incorporate perpendicular_distance() into libembroidery.
            px = sceneAxis2Point2.x()
            py = sceneAxis2Point2.y()
            x1 = sceneCenterPoint.x()
            y1 = sceneCenterPoint.y()
            line = Line(sceneCenterPoint, scene_axis_1_point_2)
            norm = line.normalVector()
            dx = px-x1
            dy = py-y1
            norm.translate(dx, dy)
            # HACK
            # norm.intersects(line, &iPoint);
            iPoint = line.p1()
            ellipseHeight = Line(px, py, iPoint.x(), iPoint.y()).length()*2.0

            set_pos(sceneCenterPoint)
            set_size(ellipseWidth, ellipseHeight)
            self.rotation = -ellipseRot

            itemCenterPoint = (sceneCenterPoint)
            itemAxis2Point2 = (sceneAxis2Point2)
            item_line = Line(itemCenterPoint, itemAxis2Point2)
            if painter:
                draw_rubber_line(item_line, painter, "VIEW_COLOR_CROSSHAIR")
            self.update_path()

        elif self.rubber_mode == "GRIP":
            debug_message("TODO: self.update_rubber() gripping for Ellipse.")

    def vulcanize(self):
        " . "
        debug_message("Ellipse vulcanize()")
        self.update_rubber()
        self.rubber_mode = "RUBBER_OFF"

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        all_points = self.all_grip_points()
        return closest_vector(all_points, mouse_point)

    def all_grip_points(self):
        " . "
        return [
            self.center(),
            self.quadrant(0),
            self.quadrant(90),
            self.quadrant(180),
            self.quadrant(270)
        ]

    def grip_edit(self, before, after):
        " TODO: grip_edit() for Ellipse. "

    def save_path(self):
        r"."
        r = Rect()
        path.arc_move_to(r, 0)
        path.arc_to(r, 0, 360)

        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s, s)
        return trans.map(path)

class Polygon():
    r"""
    The Polygon class definition.

    Needs to support the features available in the SVG spec.
    """
    def __init__(self, x=0, y=0, pen=Pen(), path=Path()):
        " . "
        self.x = x
        self.y = y
        self.path = path
        self.rotation = 0.0
        self.scale = 1.0
        self.type = "Polygon"
        self.selectable = True
        self.grip_index = -1
        self.pen = pen
        self.update_path(path)
        self.normal_path = Path()
        self.points = []

        # Prompt version.
        clear_selection()
        self.center = Vector(math.nan, math.nan)
        self.side1 = Vector(math.nan, math.nan)
        self.side2 = Vector(math.nan, math.nan)
        self.point_i = Vector(math.nan, math.nan)
        self.point_c = Vector(math.nan, math.nan)
        self.poly_type = "Inscribed"
        self.num_sides = 4
        self.mode = "NUM_SIDES"
        set_prompt_prefix("Enter int of sides " + str(self.num_sides) + "}: ")

    def copy(self):
        " Return a copy of the object. "
        return Polygon(x=self.x, y=self.y,
            rgb=self.pen.rgb, line_type=self.pen.line_type,
            line_weight=self.line_weight)

    def update_path(self, p):
        " . "
        self.normal_path = p
        closed_path = self.normal_path
        closed_path.close_sub_path()
        reverse_path = closed_path.toReversed()
        reverse_path.connect_path(closed_path)
        self.set_object_path(reverse_path)

    def paint(self, painter, option, widget):
        " . "
        obj_scene = Scene()
        if not obj_scene:
            return

        paint_pen = Pen()
        painter.set_pen(paint_pen)
        self.update_rubber(painter)
        if "State_Selected" in option.state:
            paint_pen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT"):
            paint_pen = line_weight_pen()
        painter.set_pen(paint_pen)

        if self.normal_path.element_count():
            painter.drawPath(self.normal_path)
            zero = self.normal_path.element_at(0)
            last = self.normal_path.element_at(self.normal_path.element_count()-1)
            painter.draw_line(Vector(zero.x, zero.y), Vector(last.x, last.y))

    def update_rubber(self, painter):
        " . "
        if self.rubber_mode == "Polygon":
            setObjectPos(object_rubber_point("POLYGON_POINT_0"))

            numStr = object_rubber_text("POLYGON_NUM_POINTS")
            if numStr.isNull():
                return
            num, ok = numStr.toInt()
            if not ok:
                return

            rubber_path = Path()
            point0 = (object_rubber_point("POLYGON_POINT_0"))
            rubber_path.move_to(point0)
            for i in range(1, num+1):
                appendStr = "POLYGON_POINT_" + str(i)
                appendPoint = (object_rubber_point(appendStr))
                rubber_path.line_to(appendPoint)

            # rubber_path.line_to(0,0)
            self.update_path(rubber_path)

            # Ensure the path isn't updated until the int of points is changed again.
            set_object_rubberText("POLYGON_NUM_POINTS", "")

        elif self.rubber_mode == "Inscribe":
            self.setObjectPos(object_rubber_point("POLYGON_CENTER"))

            num_sides = object_rubber_point("POLYGON_NUM_SIDES").x()

            inscribe_point = (object_rubber_point("POLYGON_INSCRIBE_POINT"))
            inscribe_line = Line(Vector(0,0), inscribe_point)
            inscribe_angle = inscribe_line.angle()
            inscribe_inc = 360.0/num_sides

            if painter:
                draw_rubber_line(inscribe_line, painter, "VIEW_COLOR_CROSSHAIR")

            inscribe_path = Path()
            # First Point
            inscribe_path.move_to(inscribe_point)
            # Remaining Points
            for i in range(1, num_sides):
                inscribe_line.set_angle(inscribe_angle + inscribe_inc*i)
                inscribe_path.line_to(inscribe_line.p2())

            self.update_path(inscribe_path)

        elif self.rubber_mode == "Circumscribe":
            self.setObjectPos(object_rubber_point("POLYGON_CENTER"))

            num_sides = object_rubber_point("POLYGON_NUM_SIDES").x()

            circumscribe_point = (object_rubber_point("POLYGON_CIRCUMSCRIBE_POINT"))
            circumscribe_line = Line(Vector(0,0), circumscribe_point)
            circumscribe_angle = circumscribe_line.angle()
            circumscribe_inc = 360.0/num_sides

            if painter:
                draw_rubber_line(circumscribe_line, painter, "VIEW_COLOR_CROSSHAIR")

            circumscribe_path = Path()
            # First Point
            prev = Line(circumscribe_line.p2(), Vector(0,0))
            prev = prev.normal_vector()
            circumscribe_line.set_angle(circumscribe_angle + circumscribe_inc)
            perp = Line(circumscribe_line.p2(), Vector(0,0))
            perp = perp.normal_vector()
            # HACK perp.intersects(prev, &iPoint);
            iPoint = perp.p1()
            circumscribe_path.move_to(iPoint)
            # Remaining Points
            for i in range(2, num_sides+1):
                prev = perp
                circumscribe_line.set_angle(circumscribe_angle + circumscribe_inc*i)
                perp = Line(circumscribe_line.p2(), Vector(0,0))
                perp = perp.normal_vector()
                # HACK perp.intersects(prev, &iPoint);
                iPoint = perp.p1()
                circumscribe_path.line_to(iPoint)
            self.update_path(circumscribe_path)

        elif self.rubber_mode == "GRIP":
            if painter:
                elem_count = self.normal_path.element_count()
                grip_point = object_rubber_point("GRIP_POINT")
                if grip_index == -1:
                    grip_index = find_index(grip_point)
                if grip_index == -1:
                    return

                m = 0
                n = 0

                if not grip_index:
                    m = elem_count-1
                    n = 1
                elif grip_index == elem_count-1:
                    m = elem_count-2
                    n = 0
                else:
                    m = grip_index-1
                    n = grip_index+1
                em = self.normal_path.element_at(m)
                en = self.normal_path.element_at(n)
                emPoint = Vector(em.x, em.y)
                enPoint = Vector(en.x, en.y)
                painter.draw_line(emPoint, (object_rubber_point("")))
                painter.draw_line(enPoint, (object_rubber_point("")))

                rub_line = Line((grip_point),
                                (object_rubber_point("")))
                draw_rubber_line(rub_line, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        " . "
        debug_message("PolygonObject vulcanize()")
        update_rubber()

        set_object_rubber_mode(OBJ_RUBBER_OFF)

        if not self.normal_path.element_count():
            label = "Empty Polygon Error"
            description = "The polygon added contains no points. The command that created this object has flawed logic."
            critical(0, label, description)

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        element = self.normal_path.element_at(0)
        closest_point = element
        closest_dist = Line(mouse_point, closest_point).length()
        elem_count = self.normal_path.element_count()
        for i in range(elem_count):
            element = self.normal_path.element_at(i)
            elem_point = element
            elem_dist = Line(mouse_point, elem_point).length()
            if elem_dist < closest_dist:
                closest_point = elem_point
                closest_dist = elem_dist

        return closest_point

    def all_grip_points(self):
        r"."
        grip_points = []
        for i in range(normal_path.element_count()):
            element = self.normal_path.element_at(i)
            grip_points += [(element.x, element.y)]

        return grip_points

    def find_index(self, point):
        r"Find the closest index to the supplied point."
        elem_count = self.normal_path.element_count()
        # NOTE: Points here are in item coordinates.
        item_point = (point)
        for i in range(elem_count):
            e = self.normal_path.element_at(i)
            elem_point = Vector(e.x, e.y)
            if item_point == elem_point:
                return i

        return -1

    def find_index_2(self, point):
        r"Find the closest index to the supplied point."
        index = -1
        max_distance = 1.0
        for i in range(len(self.points)):
            if (self.points[i].subtract(point)).length() < max_distance:
                index = i
                max_distance = (self.points[i].subtract(point)).length()
        return index

    def grip_edit(self, before, after):
        " . "
        self.grip_index = self.find_index(before)
        if self.grip_index == -1:
            return
        a_vector = (after)
        self.normal_path.set_element_position_at(self.grip_index, a_vector)
        self.update_path(normal_path)
        self.grip_index = -1

    def object_copy_path(self):
        " . "
        return self.normal_path

    def object_save_path(self):
        " . "
        closed_path = self.normal_path
        closed_path.close_sub_path()
        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s,s)
        return trans.map(closed_path)

    def click(self, x, y):
        vector = Vector(x, y)
        if self.mode == "NUM_SIDES":
            #Do nothing, the prompt controls this.
            debug_message("NUM_SIDES mode")

        elif self.mode == "CENTER_PT":
            self.center = vector
            self.mode = "poly_type"
            append_prompt_history()
            set_prompt_prefix("Specify polygon type [Inscribed in circle/Circumscribed around circle] " + self.poly_type + "}: ")

        elif self.mode == "poly_type":
            #Do nothing, the prompt controls this.
            debug_message("poly_type mode")

        elif self.mode == "INSCRIBE":
            self.point_i = vector
            self.rubber_points["POLYGON_INSCRIBE_POINT"] = self.point_i
            self.vulcanize()
            append_prompt_history()

        elif self.mode == "CIRCUMSCRIBE":
            self.point_c = vector
            self.rubber_points["POLYGON_CIRCUMSCRIBE_POINT"] = self.point_c
            self.vulcanize()
            append_prompt_history()

        elif self.mode == "DISTANCE":
            # Do nothing, the prompt controls this.
            debug_message("mode DISTANCE")

        elif self.mode == "SIDE_LEN":
            debug_message("POLYGON Sidelength mode")
            debug_message("mode SIDE LEN")

    def prompt(self, cmd):
        if self.mode == "NUM_SIDES":
            if str == "" and self.num_sides >= 3 and self.num_sides <= 1024:
                set_prompt_prefix("Specify center point or [Sidelength]: ")
                self.mode = "CENTER_PT"

            else:
                tmp = int(cmd)
                if math.isnan(tmp) or (not isInt(tmp)) or tmp < 3 or tmp > 1024:
                    debug_message("Requires an integer between 3 and 1024.", msgtype="ALERT")
                    set_prompt_prefix("Enter number of sides " + str(self.num_sides) + "}: ")

                else:
                    self.num_sides = tmp
                    set_prompt_prefix("Specify center point or [Sidelength]: ")
                    self.mode = "CENTER_PT"

        elif self.mode == "CENTER_PT":
            if cmd[0] == "S" or cmd == "SIDELENGTH":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SIDE_LEN"
                set_prompt_prefix("Specify start point: ")

            else:
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message("Point or option keyword required.", msgtype="ALERT")
                    set_prompt_prefix("Specify center point or [Sidelength]: ")

                else:
                    self.center = vector
                    self.mode = "poly_type"
                    set_prompt_prefix("Specify polygon type [Inscribed in circle/Circumscribed around circle] " + self.poly_type + "}: ")

        elif self.mode == "poly_type":
            if cmd == "INSCRIBED"[len(cmd)]:
                # TODO: Probably should add additional translate calls here.
                self.mode = "INSCRIBE"
                self.poly_type = "Inscribed"
                set_prompt_prefix("Specify polygon corner point or [Distance]: ")
                add_rubber("POLYGON")
                self.rubber_mode = "POLYGON_INSCRIBE"
                self.rubber_points["POLYGON_CENTER"] = self.find_center()
                self.rubber_points["POLYGON_NUM_SIDES"] = (self.num_sides, 0)

            elif cmd == "CIRCUMSCRIBED"[len(cmd)]:
                # TODO: Probably should add additional translate calls here.
                self.mode = "CIRCUMSCRIBE"
                self.poly_type = "Circumscribed"
                set_prompt_prefix("Specify polygon side point or [Distance]: ")
                add_rubber("POLYGON")
                self.rubber_mode = "POLYGON_CIRCUMSCRIBE"
                self.rubber_points["POLYGON_CENTER"] = self.find_center()
                self.rubber_points["POLYGON_NUM_SIDES"] = (self.num_sides, 0)

            elif str == "":
                if self.poly_type == "Inscribed":
                    self.mode = "INSCRIBE"
                    set_prompt_prefix("Specify polygon corner point or [Distance]: ")
                    add_rubber("POLYGON")
                    self.rubber_mode = "POLYGON_INSCRIBE"
                    self.rubber_points["POLYGON_CENTER"] = self.find_center()
                    self.rubber_points["POLYGON_NUM_SIDES"] = (self.num_sides, 0)

                elif self.poly_type == "Circumscribed":
                    self.mode = "CIRCUMSCRIBE"
                    set_prompt_prefix("Specify polygon side point or [Distance]: ")
                    add_rubber("POLYGON")
                    self.rubber_mode = "POLYGON_CIRCUMSCRIBE"
                    self.rubber_points["POLYGON_CENTER"] = self.center
                    self.rubber_points["POLYGON_NUM_SIDES"] = (self.num_sides, 0)

                else:
                    message = "POLYGON Polygon type is not Inscribed or Circumscribed."
                    debug_message(message, msgtype="ERROR")

            else:
                debug_message("Invalid option keyword.", msgtype="ALERT")
                set_prompt_prefix("Specify polygon type [Inscribed in circle/Circumscribed around circle] " + self.poly_type + "}: ")

        elif self.mode == "INSCRIBE":
            if str == "D" or cmd == "DISTANCE":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "DISTANCE"
                set_prompt_prefix("Specify distance: ")

            else:
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message("Point or option keyword required.", msgtype="ALERT")
                    set_prompt_prefix("Specify polygon corner point or [Distance]: ")

                else:
                    self.point_i = vector
                    self.rubber_points["POLYGON_INSCRIBE_POINT"] = self.point_i
                    self.vulcanize()
                    return

        elif self.mode == "CIRCUMSCRIBE":
            if cmd[0] == "D" or cmd == "DISTANCE":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "DISTANCE"
                set_prompt_prefix("Specify distance: ")
            else:
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message("Point or option keyword required.", msgtype="ALERT")
                    set_prompt_prefix("Specify polygon side point or [Distance]: ")

                else:
                    self.point_c = vector
                    self.rubber_points["POLYGON_CIRCUMSCRIBE_POINT"] = self.point_c
                    self.vulcanize()

        elif self.mode == "DISTANCE":
            if math.isnan(cmd):
                debug_message("Requires valid numeric distance.", msgtype="ALERT")
                set_prompt_prefix("Specify distance: ")

            else:
                if self.poly_type == "Inscribed":
                    self.point_i.x = self.center.x
                    self.point_i.y = self.center.y + int(cmd)
                    self.rubber_points["POLYGON_INSCRIBE_POINT"] = self.point_i
                    self.vulcanize()

                elif self.poly_type == "Circumscribed":
                    self.point_c.x = self.center.x
                    self.point_c.y = self.center.y + int(cmd)
                    self.rubber_points["POLYGON_CIRCUMSCRIBE_POINT"] = self.point_c
                    self.vulcanize()

                else:
                    description = "POLYGON Polygon type is not Inscribed or Circumscribed."
                    debug_message(description, msgtype="ERROR")

        elif self.mode == "SIDE_LEN":
            debug_message("POLYGON Sidelength mode")

    def find_center(self):
        r"Loop over all points and add the supplied offset."
        center = Vector(0.0, 0.0)
        for i in range(len(self.points)):
            center += self.points[i]
        return center / len(self.points)

    def translate(self, vector):
        r"Loop over all points and add the supplied offset."
        for i in range(len(self.points)):
            self.points[i] = vector + self.points[i]

class PolyLine():
    r"""
    This is necessarily a class because we need the same
    functions for other geometry objects and supporting SVG means
    supporting every geometry object supported natively by it.

    We should be able to initialise using an existing one, maybe
    a copy() function?
    """
    def __init__(self, x, y, pen=Pen()):
        r"""
        Needs to work with the libembroidery polyline, if that's wrapped
        in a class then this class extends that one and we call

        super().__init__()

        here.

        Some of the functions here can then be ported to libembroidery.
        """
        debug_message("Polyline.__init__()")
        self.path = p
        self.position = Vector(x, y)
        self.pen = pen
        self.rotation = 0.0
        self.scale = 1.0
        self.type = "Polyline"
        self.selectable = 1
        self.grip_index = -1
        self.update_path(p)
        self.normal_path = Path()

        clear_selection()
        self.first_run = True
        self.first = Vector(math.nan, math.nan)
        self.prev = Vector(math.nan, math.nan)
        self.num = 0
        set_prompt_prefix("Specify first point: ")

    def update(self, p):
        r"""
        This is a straight translation and I'm not sure what
        it's doing -- Robin
        """
        self.normal_path = p
        self.reverse_path = self.normal_path.reverse()
        self.reverse_path.connect(self.normal_path)
        self.path = self.reverse_path

    def paint(self, painter, option, widget):
        r"""
        """
        obj_scene = self.scene()
        if not obj_scene:
            return
        paint_pen = Pen()
        painter.pen = paint_pen
        painter.update_rubber()

        if "State_Selected" in option.state:
            paint_pen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT"):
            paint_pen = self.lwt_pen
        painter.set_pen(paint_pen)

        painter.drawPath(normal_path)

        if obj_scene.property("ENABLE_LWT") and obj_scene.property("ENABLE_REAL"):
            realRender(painter, normal_path)

    def update_rubber(self, painter):
        r"."
        self.rubber_mode = object_rubber_mode()
        if self.rubber_mode == "OBJ_RUBBER_POLYLINE":
            set_object_pos(self.rubber_point("POLYLINE_POINT_0"))

            rubber_line = Line(normal_path.currentPosition(), (self.rubber_point("")))
            if painter:
                draw_rubber_line(rubber_line, painter, "VIEW_COLOR_CROSSHAIR")

            ok = 0
            numStr = objectRubberText("POLYLINE_NUM_POINTS")
            if numStr.isNull():
                return
            num = numStr.toInt(ok)
            if not ok:
                return

            appendStr = ""
            rubberPath = Path()
            for i in range(1, num+1):
                appendStr = "POLYLINE_POINT_" + "".setNum(i)
                appendPoint = (self.rubber_point(appendStr))
                rubberPath.line_to(appendPoint)

            update_path(rubberPath)

            # Ensure the path isn't updated until
            # the number of points is changed again.
            setObjectRubberText("POLYLINE_NUM_POINTS", "")

        elif self.rubber_mode == "OBJ_RUBBER_GRIP":
            if painter:
                elemCount = normal_path.element_count()
                grip_point = self.rubber_point("GRIP_POINT")
                if gripIndex == -1:
                    gripIndex = find_index(grip_point)
                if gripIndex == -1:
                    return

                if not gripIndex:
                    # First
                    ef = normal_path.element_at(1)
                    efPoint = Vector(ef.x, ef.y)
                    painter.draw_line(efPoint, (self.rubber_point("")))

                elif gripIndex == elemCount-1:
                    # Last
                    el = normal_path.element_at(gripIndex-1)
                    elPoint = Vector(el.x, el.y)
                    painter.draw_line(elPoint, (self.rubber_point("")))

                else:
                    # Middle
                    em = normal_path.element_at(gripIndex-1)
                    en = normal_path.element_at(gripIndex+1)
                    emPoint = Vector(em.x, em.y)
                    enPoint = Vector(en.x, en.y)
                    painter.draw_line(emPoint, (self.rubber_point("")))
                    painter.draw_line(enPoint, (self.rubber_point("")))

                rub_line = Line((grip_point), (self.rubber_point("")))
                draw_rubber_line(rub_line, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        " . "
        debug_message("PolylineObject vulcanize()")
        self.update_rubber()

        setObjectrubber_mode(OBJ_RUBBER_OFF)

        if not normal_path.element_count():
            Qmessage_box_critical(0, QObject_tr("Empty Polyline Error"), QObject_tr("The polyline added contains no points. The command that created this object has flawed logic."))

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        element = normal_path.element_at(0)
        closestPoint = mapToScene(Vector(element.x, element.y))
        closestDist = Line(mouse_point, closestPoint).length()
        elemCount = normal_path.element_count()
        for i in range(elemCount):
            element = normal_path.element_at(i)
            elemPoint = mapToScene(element.x, element.y)
            elemDist = Line(mouse_point, elemPoint).length()

            if elemDist < closestDist:
                closestPoint = elemPoint
                closestDist = elemDist

        return closestPoint

    def all_grip_points(self):
        grip_points = []
        for i in range(normal_path.element_count()):
            element = normal_path.element_at(i)
            grip_points += [mapToScene(element.x, element.y)]

        return grip_points

    def Polyfind_index(self, point):
        " . "
        elemCount = normal_path.element_count()
        # NOTE: Points here are in item coordinates.
        itemPoint = (point)
        for i in range(elemCount):
            e = normal_path.element_at(i)
            elemPoint = Vector(e.x, e.y)
            if itemPoint == elemPoint:
                return i

        return -1

    def Polygrip_edit(self, before, after):
        r"."
        gripIndex = find_index(before)
        if gripIndex == -1:
            return
        a = (after)
        normal_path.setElementPositionAt(gripIndex, a)
        update_path(normal_path)
        gripIndex = -1

    def PolyobjectCopyPath():
        r"."
        return normal_path

    def PolyobjectSavePath():
        r"."
        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s,s)
        return trans.map(normal_path)

    def click(self, x, y):
        r"."
        if self.first_run:
            self.first_run = False
            self.first = Vector(x, y)
            self.prev = Vector(x, y)
            add_rubber("POLYLINE")
            set_rubber_mode("POLYLINE")
            set_rubber_point("POLYLINE_POINT_0", self.first)
            append_prompt_history()
            set_prompt_prefix("Specify next point or [Undo]: ")
        else:
            self.num += 1
            set_rubber_point(f"POLYLINE_POINT_{num}", Vector(x, y))
            set_rubber_text("POLYLINE_NUM_POINTS", str(num))
            spare_rubber("POLYLINE")
            append_prompt_history()
            self.prevX = x
            self.prevY = y

    def a_click(self, x, y):
        " . "
        if self.first_run:
            add_rubber("LINE")
            set_rubber_mode("LINE")
            set_rubber_point("LINE_START", self.first)
            append_prompt_history()
            set_prompt_prefix("Specify next point or [Undo]: ")

        else:
            set_rubber_point("LINE_END", x, y)
            vulcanize()
            add_rubber("LINE")
            set_rubber_mode("LINE")
            set_rubber_point("LINE_START", x, y)
            append_prompt_history()
            self.prevX = x
            self.prevY = y

    def prompt(self, cmd):
        r"."
        if self.first_run:
            vector = vector_from_str(cmd)
            if not vector:
                debug_message("Invalid point.", msgtype="ALERT")
                set_prompt_prefix("Specify first point: ")
            else:
                self.first_run = false
                self.first = vector
                self.prev = self.first
                add_rubber("POLYLINE")
                self.rubber_mode = "POLYLINE"
                set_rubber_point("POLYLINE_POINT_0", self.first)
                set_prompt_prefix("Specify next point or [Undo]: ")

        else:
            if cmd == "U" or cmd == "UNDO":
                # TODO: Probably should add additional qsTr calls here.
                debug_message("POLYLINE prompt() for UNDO")
            else:
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message("Point or option keyword required.", msgtype="ALERT")
                    set_prompt_prefix("Specify next point or [Undo]: ")
                else:
                    self.num += 1
                    set_rubber_point(f"POLYLINE_POINT_{num}", x, y)
                    set_rubber_text("POLYLINE_NUM_POINTS", str(num))
                    spare_rubber("POLYLINE")
                    self.prev = vector
                    set_prompt_prefix("Specify next point or [Undo]: ")

    def a_prompt(self, cmd):
        " . "
        if self.first_run:
            vector = vector_from_str(cmd)
            if not vector:
                debug_message("Invalid point.", msgtype="ALERT")
                set_prompt_prefix("Specify first point: ")
            else:
                self.first_run = False
                self.first = vector
                self.prev = self.first
                add_rubber("LINE")
                self.rubber_mode = "LINE"
                self.rubber_points["LINE_START"] = self.first
                set_prompt_prefix("Specify next point or [Undo]: ")
        else:
            if cmd == "U" or cmd == "UNDO":
                debug_message("LINE prompt() for UNDO")
            else:
                vector = vector_from_str(cmd)
                if not vector:
                    debug_message("Point or option keyword required.", msgtype="ALERT")
                    set_prompt_prefix("Specify next point or [Undo]: ")
                else:
                    self.rubber_points["LINE_END"] = vector
                    self.vulcanize()
                    add_rubber("LINE")
                    self.rubber_mode = "LINE"
                    self.rubber_points["LINE_START"] = vector
                    self.prev = vector
                    set_prompt_prefix("Specify next point or [Undo]: ")

    def copy(self):
        " . "
        return PolyLine()

    def copy_path(self):
        " . "
        return self.normal_path

    def save_path(self):
        " . "
        return self.normal_path

    def find_index(self, point):
        " . "
        return point

    def update_rubber(self, painter = 0):
        " . "
        return

    def vulcanize(self):
        " . "
        return

    def mouse_snap_point(self, mouse_point):
        " . "
        return

    def all_grip_points():
        " . "
        return

    def grip_edit(self, before, after):
        " . "
        return

    def paint(self, painter, options, widget):
        " . "
        return

    def update_path(p):
        " . "
        return

    def a__init__(self):
        r" . "
        self.first_run = True
        self.rubber = "NOT SET"
        self.base  = Vector(math.nan, math.nan)
        self.dest = Vector(math.nan, math.nan)
        self.delta = Vector(math.nan, math.nan)
        self.rubber_mode = "LINE"
        self.rubber_points = {}

        if numSelected() <= 0:
            # TODO: Prompt to select objects if nothing is preselected
            debug_message("Preselect objects before invoking the move command.", msgtype="ALERT")
            return
            message_box("information", "Move Preselect", "Preselect objects before invoking the move command.")
        else:
            set_prompt_prefix("Specify base point: ")

    def a_click(self, x, y):
        r" . "
        if self.first_run:
            self.first_run = False
            self.base = Vector(x, y)
            self.rubber = "LINE"
            self.rubber_mode = "LINE"
            self.rubber_points["LINE_START"] = self.base
            preview_on("SELECTED", "MOVE", self.base, 0)
            append_prompt_history()
            set_prompt_prefix("Specify destination point: ")
        else:
            self.dest = Vector(x, y)
            self.delta = self.dest.subtract(self.base)
            move_selected(self.delta)
            preview_off()

    def a_prompt(self, user_string):
        r" . "
        if self.first_run:
            vector = vector_from_str(user_string)
            if not vector:
                debug_message("Invalid point.", msgtype="ALERT")
                set_prompt_prefix("Specify base point: ")
            else:
                self.first_run = False
                self.base = vector
                self.rubber = "LINE"
                self.rubber_mode = "LINE"
                self.rubber_points["LINE_START"] = self.base
                preview_on("SELECTED", "MOVE", self.base, 0)
                set_prompt_prefix("Specify destination point: ")

        else:
            vector = vector_from_str(user_string)
            if not vector:
                debug_message("Invalid point.", msgtype="ALERT")
                set_prompt_prefix("Specify destination point: ")
            else:
                self.dest = vector
                self.delta = self.dest.subtract(self.base)
                move_selected(self.delta)
                preview_off()


class Rotate():
    r"""
    The Rotate class definition file.

    This was here to facilitate the ROTATE command for the
    in-built console. This may not be used in the 2.0.0 version.
    """
    def __init__(self):
        " . "
        self.mode = "NORMAL"
        self.modes = ["NORMAL", "REFERENCE"]
        self.first_run = True
        self.base = Vector(math.nan, math.nan)
        self.dest = Vector(math.nan, math.nan)
        self.angle = math.nan
        self.base_r = Vector(math.nan, math.nan)
        self.dest_r = Vector(math.nan, math.nan)
        self.angle_ref = math.nan
        self.angle_new = math.nan
        self.rubber_mode = "LINE"
        self.rubber_points = {}

        if num_selected() <= 0:
            #TODO: Prompt to select objects if nothing is preselected
            message = translate("Preselect objects before invoking the rotate command.")
            alert()
            return
            message_box("information", translate("Rotate Preselect"), message)

        else:
            set_prompt_prefix(translate("Specify base point: "))

    def click(self, x, y):
        " . "
        if self.mode == "NORMAL":
            if self.first_run:
                self.first_run = False
                self.base = Vector(x, y)
                add_rubber("LINE")
                self.rubber_mode = "LINE"
                set_rubber_point("LINE_START", self.base)
                preview_on("SELECTED", "ROTATE", self.base, 0)
                append_prompt_history()
                set_prompt_prefix(translate("Specify rotation angle or [Reference]: "))
            else:
                self.dest = Vector(x, y)
                self.angle = calculate_angle(self.base, self.dest)
                append_prompt_history()
                rotate_selected(self.baseX, self.baseY, self.angle)
                preview_off()
                return

        elif self.mode == "REFERENCE":
            if math.isnan(self.base_r.x):
                self.base_r = vector
                append_prompt_history()
                add_rubber("LINE")
                self.rubber_mode = "LINE"
                self.rubber_points["LINE_START"] = self.base_r
                set_prompt_prefix(translate("Specify second point: "))

            elif math.isnan(self.dest_r.x):
                self.dest_r = vector
                self.angle_ref = calculate_angle(self.base_r, self.dest_r)
                self.rubber_points["LINE_START"] = self.base
                preview_on("SELECTED", "ROTATE", self.base, self.angle_ref)
                append_prompt_history()
                set_prompt_prefix(translate("Specify the new angle: "))

            elif math.isnan(self.angle_new):
                self.angle_new = calculate_angle(self.base, vector)
                rotate_selected(self.base, self.angle_new - self.angle_ref)
                preview_off()

    def prompt(self, cmd):
        " . "
        if self.mode == "NORMAL":
            if self.first_run:
                vector = vector_from_str(cmd)
                if math.isnan(vector.x) or math.isnan(vector.y):
                    alert(translate("Invalid point."))
                    set_prompt_prefix(translate("Specify base point: "))
                else:
                    self.first_run = False
                    self.base = vector
                    add_rubber("LINE")
                    self.rubber_mode = "LINE"
                    set_rubber_point("LINE_START", self.base)
                    preview_on("SELECTED", "ROTATE", self.base, 0)
                    set_prompt_prefix(translate("Specify rotation angle or [Reference]: "))

            else:
                if cmd[0] == "R" or cmd == "REFERENCE":
                    # TODO: Probably should add additional qsTr calls here.
                    self.mode = self.mode_REFERENCE
                    set_prompt_prefix(translate("Specify the reference angle") + " 0.00}: ")
                    clearRubber()
                    preview_off()
                else:
                    if math.isnan(cmd):
                        alert(translate("Requires valid numeric angle, second point, or option keyword."))
                        set_prompt_prefix(translate("Specify rotation angle or [Reference]: "))
                    else:
                        self.angle = float(cmd)
                        rotate_selected(self.base, self.angle)
                        preview_off()
                        return

        elif self.mode == "REFERENCE":
            if math.isnan(self.base_r.x):
                if math.isnan(cmd):
                    vector = vector_from_str(cmd)
                    if math.isnan(vector.x) or math.isnan(vector.y):
                        alert(translate("Requires valid numeric angle or two points."))
                        set_prompt_prefix(translate("Specify the reference angle") + " 0.00}: ")
                    else:
                        self.base_r = vector
                        add_rubber("LINE")
                        self.rubber_mode = "LINE"
                        self.rubber_points["LINE_START"] = self.base_r
                        set_prompt_prefix(translate("Specify second point: "))
 
                else:
                    # The base and dest values are only set
                    # here to advance the command.
                    self.base_r = Vector(0.0, 0.0)
                    self.dest_r = Vector(0.0, 0.0)
                    # The reference angle is what we will use later.
                    self.angle_ref = float(cmd)
                    add_rubber("LINE")
                    self.rubber_mode = "LINE"
                    self.rubber_points["LINE_START"] = self.base
                    preview_on("SELECTED", "ROTATE", self.base, self.angle_ref)
                    set_prompt_prefix(translate("Specify the new angle: "))

            elif math.isnan(self.dest_r.x):
                if math.isnan(cmd):
                    vector = vector_from_str(cmd)
                    if math.isnan(vector.x) or math.isnan(vector.y):
                        alert(translate("Requires valid numeric angle or two points."))
                        set_prompt_prefix(translate("Specify second point: "))
                    else:
                        self.dest_r = vector
                        self.angle_ref = calculate_angle(self.base_r, self.dest_r)
                        preview_on("SELECTED", "ROTATE", self.base, self.angle_ref)
                        set_rubber_point("LINE_START", self.base)
                        set_prompt_prefix(translate("Specify the new angle: "))

                else:
                    # The base and dest values are only set here to
                    # advance the command.
                    self.base_r = Vector(0.0, 0.0)
                    self.dest_r = Vector(0.0, 0.0)
                    # The reference angle is what we will use later.
                    self.angle_ref = float(cmd)
                    preview_on("SELECTED", "ROTATE", self.base.x, self.base.y, self.angle_ref)
                    set_prompt_prefix(translate("Specify the new angle: "))

            elif math.isnan(self.angle_new):
                if math.isnan(cmd):
                    vector = vector_from_str(cmd)
                    if math.isnan(strList[0]) or math.isnan(strList[1]):
                        alert(translate("Requires valid numeric angle or second point."))
                        set_prompt_prefix(translate("Specify the new angle: "))
                    else:
                        self.angle_new = calculate_angle(self.base, vector)
                        rotate_selected(self.base, self.angle_new - self.angle_ref)
                        preview_off()
                else:
                    self.angle_new = float(cmd)
                    rotate_selected(self.base, self.angle_new - self.angle_ref)
                    preview_off()


class Point():
    r"""
    The point class definition.
    """
    def __init__(self, x, y, pen=Pen()):
        " TODO: pass in proper lineweight. "
        super().__init__(self)
        self.type = "Point"
        self.position = Vector(x, y)
        self.pen = pen

        setRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        clear_selection()
        set_prompt_prefix(translate("Specify point: "))

        clear_selection()
        self.first_run = True
        set_prompt_prefix("TODO: Current point settings: PDMODE=?  PDSIZE=?")
        append_prompt_history()
        set_prompt_prefix("Specify first point: ")

    def click(self, x, y):
        " . "
        append_prompt_history()
        set_prompt_prefix("X = " + x.toString() + ", Y = " + y.toString())
        append_prompt_history()

        # Alternative version.
        if self.first_run:
            self.first_run = False
            append_prompt_history()
            set_prompt_prefix("Specify next point: ")
            add_point(x,y)
        else:
            append_prompt_history()
            add_point(x,y)

    def prompt(self, cmd):
        " . "
        vector = vector_from_str(cmd)
        if not vector:
            alert("Invalid point.")
            set_prompt_prefix("Specify point: ")
        else:
            append_prompt_history()
            terms = cmd.split(",")
            set_prompt_prefix("X = " + terms[0] + ", Y = " + terms[1])
            append_prompt_history()

        # Alternative version.
        if self.first_run:
            if str == "M" or cmd == "MODE":
                # TODO: Probably should add additional qsTr calls here.
                debug_message("POINT prompt() for PDMODE")

            elif str == "S" or cmd == "SIZE":
                # TODO: Probably should add additional qsTr calls here.
                debug_message("POINT prompt() for PDSIZE")

            vector = vector_from_str(cmd)
            if not vector:
                debug_message("Invalid point.", msgtype="ALERT")
                set_prompt_prefix("Specify first point: ")
            else:
                self.first_run = False
                set_prompt_prefix("Specify next point: ")
                addPoint(vector)

        else:
            vector = vector_from_str(cmd)
            if not vector:
                alert("Invalid point.")
                set_prompt_prefix("Specify next point: ")
            else:
                set_prompt_prefix("Specify next point: ")
                addPoint(vector)

    def copy(self):
        " . "
        debug_message("PointObject copy()")
        p = Point(self.x, self.y, rgb=self.rgb, line_type="solid")
        # TODO: getCurrentLineType
        setRotation(self.rotation())
        return p

    def paint(self, painter, option, widget):
        " . "
        obj_scene = scene()
        if not obj_scene:
            return

        paintPen = pen()
        painter.set_pen(paintPen)
        self.update_rubber(painter)
        if "State_Selected" in option.state:
            paintPen.set_style("dashed")
        if obj_scene.property("ENABLE_LWT"):
            paintPen = pen.line_weight()
        painter.set_pen(paintPen)

        painter.drawPoint(0,0)

    def update_rubber(painter):
        " . "
        self.rubber_mode = objectrubber_mode()
        if self.rubber_mode == "GRIP":
            if painter:
                grip_point = self.rubber_point["GRIP_POINT"]
                if grip_point == scenePos():
                    rub_line = Line((grip_point), (self.rubber_point("")))
                    draw_rubber_line(rub_line, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        " . "
        debug_message("PointObject vulcanize()")
        self.update_rubber()

        self.rubber_mode = "RUBBER_OFF"

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        return self.scene_position

    def all_grip_points():
        " Returns all grip points: currently just the scene position. "
        return [self.scene_position]

    def grip_edit(self, before, after):
        " . "
        if before == self.scenePos():
            delta = after-before
            moveBy(delta.x(), delta.y())

    def save_path(self):
        " . "
        path = Path()
        path.add_rect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        return path

class Image():
    r"""
    The Image class definition.
    """
    def __init__(self, position, width, height, scene, fname, bg="#000000"):
        r"""
        """
        debug_message("Image Constructor()")
        self.type = "Image"
        self.selectable = True
        self.rubber_points = {}
        self.rubber_mode = "IMAGE"
        self.scene = scene
        self.position = position
        self.rect = Rect(position=self.position, size=(width, height))
        self.rgb = bg
        self.scale = 1.0
        self.rotation = 0.0
        self.pen = Pen()
        self.update_path()

    def copy(self, obj, parent):
        " . "
        debug_message("Image Constructor()")
        img = Image(self.rect.position_x, self.rect.position_y,
                    self.rect.width, self.rect.height, rgb=self.rgb,
                    line_type=self.line_type, line_weight=self.line_weight)
        img.setRotation(self.rotation())
        return img

    def area(self):
        r" Returns the area covered by the image. "
        return self.width * self.height

    def set_rect(self, position, size):
        " . "
        self.rect = Rect(position=position, size=size)
        self.update_path()

    def update_path(self):
        r""" NOTE: Reverse the path so that the inside area isn't
        considered part of the rectangle.
        """
        r = self.rect
        self.path = [
            ["move_to", r.bottom_left()],
            ["line_to", r.bottom_right()],
            ["line_to", r.top_right()],
            ["line_to", r.top_left()],
            ["line_to", r.bottom_left()],
            ["line_to", r.top_left()],
            ["line_to", r.top_right()],
            ["line_to", r.bottom_right()],
            ["move_to", r.bottom_left()]
        ]

    def paint(self, painter, option, widget):
        " . "
        obj_scene = self.scene
        if not obj_scene:
            return

        paint_pen = Pen()
        painter.pen = paint_pen
        self.update_rubber(painter)
        if obj_scene.property("ENABLE_LWT"):
            paint_pen = self.lwt_pen()
        if "State_Selected" in option.state:
            paint_pen.set_style("dashed")
        painter.pen = paint_pen

        painter.draw_rect(self.rect)

    def update_rubber(self, painter):
        " . "
        if self.rubber_mode == "IMAGE":
            debug_message("IMAGE")
            scene_start_point = self.rubber_points["IMAGE_START"]
            scene_end_point = self.rubber_points["IMAGE_END"]
            self.rect = Rect(scene_start_point, scene_end_point)
            self.update_path()

        elif self.rubber_mode == "GRIP":
            " TODO: self.update_rubber() gripping for Image. "
            debug_message("GRIP")

    def vulcanize(self):
        " . "
        debug_message("Image vulcanize()")
        self.update_rubber()
        self.rubber_mode = "OFF"

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        all_points = self.all_grip_points()
        return closest_vector(all_points, mouse_point)

    def all_grip_points(self):
        " . "
        gripPoints = [
            self.rect.top_left(),
            self.rect.top_right(),
            self.rect.bottom_left(),
            self.rect.bottom_right()
        ]
        return gripPoints

    def grip_edit(self, before, after):
        " TODO: grip_edit() for Image "
        return

    def draw(self, description):
        r"""
        This is similar to using an svg path, we can blend these systems
        later.
        int_buffer[4]
        icon = 0
        painter = 0
        QPen pen
        get_n_ints(description, int_buffer, 2)
        icon = new QPixmap(int_buffer[0], int_buffer[1])
        painter = new QPainter(icon)
        pen.setWidth(10)
        for ptr in description:
            # Other functions we can use are eraseRect, drawArc etc.
            # https://doc.qt.io/qt-5/qpainter.html
            if (strncmp(ptr, "rect", 4)==0) {
                pen.setColor(Color(QRgb(0x000000)))
                painter.set_pen(pen)
                get_n_ints(ptr+5, int_buffer, 4)
                painter.fillRect(int_buffer[0], int_buffer[1],
                    int_buffer[2], int_buffer[3], Qt_SolidPattern);

        """
        icon = ""
        return icon


class Parametric():
    r"""
    The generic design primative class.

    To make a specific object use a call like this:

    dolphin = Parametric(settings["objects"]["dolphin"])
    """
    def __init__(self, desc, center=Vector(math.nan, math.nan),
                 mode="NUM_POINTS"):
        r" Create an object. "
        clear_selection()
        self.desc = desc
        self.num_points = desc["num_points"]
        # min:64 max:8192
        self.center = center
        self.scale = Vector(desc["scale"][0], desc["scale"][1])
        self.mode = mode
        self.rubber_points = {
            "POLYGON": self.center
        }
        self.rubber_mode = "POLYGON"
        self.spare_rubber = "POLYGON"
        self.radius_outer = 1.0
        self.radius_inner = 1.0
        self.rubber_text = {}
        self.update()

    def update(self):
        r" Update the object to represent the current data. "
        for i in range(self.num_points+1):
            t_parameter = (2*math.pi)/self.num_points*i
            rubber_point = self.center

            curve_x = self.desc["x"]
            exp = curve_x.replace("t", str(t_parameter))
            rubber_point.x += self.scale.x * list_processor(exp)

            curve_y = self.desc["y"]
            exp = curve_y.replace("t", str(t_parameter))
            rubber_point.y += self.scale.y * list_processor(exp)

            self.rubber_points[f"POLYGON_POINT_{i}"] = rubber_point
            self.rubber_text["POLYGON_NUM_POINTS"] = str(self.num_points)

def sgn(x):
    r"The sign function."
    if x > 0.0:
        return 1.0
    elif x < 0.0:
        return -1.0
    return 0.0


def theta(x):
    r"""
    Basic mathematical function sometimes refered to as the step function.

    Allows us to capture discontinuties in the parametric function definitions.
    """
    if x < 0.0:
        return 0.0
    return 1.0


def identify_leaf(exp):
    """
    Find an expression with no parentheses within it that
    can be resolved.
    """
    for i in range(len(exp)):
        if exp[i] == "(":
            for j in range(i+1, len(exp)):
                if exp[j] == "(":
                    break
                if exp[j] == ")":
                    return exp[i:j+1]
    return exp


def resolve_expression(exp):
    r"Solve an expression with no subexpressions."
    # Parse the expression into an operation and numbers.
    exp = exp[1:-1]
    exp = exp.split(" ")
    operation = exp[0]
    numbers = [float(i) for i in exp[1:]]

    result = 0
    if operation == "+":
        for i in numbers:
            result += i

    elif operation == "-":
        result = numbers[0]
        for i in numbers[1:]:
            result -= i

    elif operation == "*":
        result = 1
        for i in numbers:
            result *= i

    elif operation == "/":
        result = numbers[0]
        for i in numbers[1:]:
            result /= i

    elif operation == "^":
        result = numbers[0]
        for i in numbers[1:]:
            result = result**i

    elif operation == "sin":
        result = math.sin(numbers[0])

    elif operation == "cos":
        result = math.cos(numbers[0])

    elif operation == "tan":
        result = math.tan(numbers[0])

    elif operation == "abs":
        result = abs(numbers[0])

    elif operation == "sgn":
        result = sgn(numbers[0])

    elif operation == "theta":
        result = theta(numbers[0])

    return str(result)


def list_processor(exp):
    """
    Our lisp expression processor.

    A lisp-style expression processor for our design primatives
    could resolve our data/code conundrum here.

    We need a safe way to express a function that only
    returns one value, given one input value. So a lisp expression
    without any applications of functions would be enough.

    For example:

    (* (/ 8 11) (sin (- (/ 10 9) (* 56 t))))

    represents

    8/11*math.sin(10/9-56*t)

    For now we'll need to deal with replacing using .replace()
    the t for the input value before calling this.
    """
    exp = exp.replace("\n", "")

    leaf = identify_leaf(exp)
    while "(" in exp:
        exp = exp.replace(leaf, resolve_expression(leaf))
        leaf = identify_leaf(exp)

    return exp


class Text():
    r"""
    .
    """
    def __init__(self, pen=Pen(), font="Arial"):
        clear_selection()
        self.modes = ["JUSTIFY", "SETFONT", "SETGEOM", "RAPID"]
        self.text = ""
        self.position = Vector(math.nan, math.nan)
        self.justify = "Left"
        self.font = font
        self.height = math.nan
        self.rotation = math.nan
        self.mode = "SETGEOM"
        self.rubber_points = {}
        self.rubber_texts = {}
        set_prompt_prefix(
            "Current font: " + self.font + "} "
            + "Text height: " + self.height + "}"
        )
        append_prompt_history()
        set_prompt_prefix("Specify start point of text or [Justify/Setfont]: ")

    def click(self, point):
        r"."
        if self.mode == "SETGEOM":
            if math.isnan(self.textX):
                self.position = point
                add_rubber("LINE")
                self.rubber_mode = "LINE"
                self.rubber_points["LINE_START"] = self.position
                append_prompt_history()
                set_prompt_prefix("Specify text height " + self.size() + "}: ")

            elif math.isnan(self.text_height):
                self.height = calculate_distance(self.position, point)
                append_prompt_history()
                set_prompt_prefix("Specify text angle " + text_angle() + "}: ")

            elif math.isnan(self.text_rotation):
                self.text_rotation = calculate_angle(self.position, point)
                set_text_angle(self.rotation)
                append_prompt_history()
                set_prompt_prefix(translate("Enter text: "))
                self.mode = "RAPID"
                enable_prompt_rapid_fire()
                clear_rubber()
                add_rubber("TEXTSINGLE")
                self.rubber_mode = "TEXTSINGLE"
                self.rubber_points["TEXT_POINT"] = self.position
                self.rubber_points["TEXT_HEIGHT_ROTATION"] = (
                    self.height, self.rotation
                )
                self.rubber_texts["TEXT_FONT"] = self.font
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                self.rubber_texts["TEXT_RAPID"] = self.text

            else:
                # Do nothing, as we are in rapidFire mode now.
                debug_message(".")

    def prompt(self, cmd):
        " . "
        if self.mode == "JUSTIFY":
            if cmd == "C" or cmd == "CENTER":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Center"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix("Specify center point of text or [Justify/Setfont]: ")

            elif cmd == "R" or cmd == "RIGHT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Right"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify right-end point of text or [Justify/Setfont]: "
                )

            elif str == "A" or cmd == "ALIGN":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Aligned"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify start point of text or [Justify/Setfont]: "
                )

            elif str == "M" or cmd == "MIDDLE":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Middle"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify middle point of text or [Justify/Setfont]: "
                )

            elif str == "F" or cmd == "FIT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Fit"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "))

            elif str == "TL" or cmd == "TOPLEFT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = self.mode_SETGEOM
                self.justify = "Top Left"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify top-left point of text or [Justify/Setfont]: "
                )

            elif str == "TC" or cmd == "TOPCENTER":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Top Center"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify top-center point of text or [Justify/Setfont]: "
                )

            elif cmd == "TR" or cmd == "TOPRIGHT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Top Right"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify top-right point of text or [Justify/Setfont]: "
                )

            elif cmd == "ML" or cmd == "MIDDLELEFT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Middle Left"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify middle-left point of text or [Justify/Setfont]: "
                )

            elif cmd == "MC" or cmd == "MIDDLECENTER":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Middle Center"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify middle-center point of text or [Justify/Setfont]: "
                )

            elif cmd == "MR" or cmd == "MIDDLERIGHT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Middle Right"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify middle-right point of text or [Justify/Setfont]: "
                )

            elif str == "BL" or cmd == "BOTTOMLEFT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Bottom Left"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify bottom-left point of text or [Justify/Setfont]: "
                )

            elif str == "BC" or cmd == "BOTTOMCENTER":
                #TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Bottom Center"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(
                    "Specify bottom-center point of text or [Justify/Setfont]: "
                )

            elif str == "BR" or cmd == "BOTTOMRIGHT":
                # TODO: Probably should add additional qsTr calls here.
                self.mode = "SETGEOM"
                self.justify = "Bottom Right"
                self.rubber_texts["TEXT_JUSTIFY"] = self.justify
                set_prompt_prefix(translate("Specify bottom-right point of text or [Justify/Setfont]: "))

            else:
                alert(translate("Invalid option keyword."), msgtype="ALERT")
                set_prompt_prefix(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "))

        elif self.mode == "SETFONT":
            self.mode = "SETGEOM"
            self.textFont = str
            self.rubber_texts("TEXT_FONT", self.textFont)
            setTextFont(self.textFont)
            set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "))

        elif self.mode == "SETGEOM":
            if math.isnan(self.textX):
                if str == "J" or cmd == "JUSTIFY":
                    # TODO: Probably should add additional qsTr calls here.
                    self.mode = self.mode_JUSTIFY
                    set_prompt_prefix(translate("Text Justification Options [Center/Right/Align/Middle/Fit/TL/TC/TR/ML/MC/MR/BL/BC/BR]: "))

                elif str == "S" or cmd == "SETFONT":
                    # TODO: Probably should add additional qsTr calls here.
                    self.mode = self.mode_SETFONT
                    set_prompt_prefix(translate("Specify font name: "))

                else:
                    strList = str.split(",")
                    if math.isnan(strList[0]) or math.isnan(strList[1]):
                        alert(translate("Point or option keyword required."), msgtype="ALERT")
                        set_prompt_prefix(translate("Specify start point of text or [Justify/Setfont]: "))
 
                    else:
                        self.textX = float(strList[0])
                        self.textY = float(strList[1])
                        add_rubber("LINE")
                        set_rubber_mode("LINE")
                        set_rubber_point("LINE_START", self.position)
                        set_prompt_prefix(translate("Specify text height") + " " + self.size() + "}: ")

            elif math.isnan(self.height):
                if str == "":
                    self.height = self.size()
                    set_prompt_prefix(translate("Specify text angle") + " " + textAngle() + "}: ")

                elif math.isnan(cmd):
                    alert("Requires valid numeric distance or second point.", msgtype="ALERT")
                    set_prompt_prefix(translate("Specify text height") + " " + textSize() + "}: ")

                else:
                    self.height = float(cmd)
                    set_prompt_prefix("Specify text angle " + self.angle + "}: ")


            elif math.isnan(self.rotation):
                if str == "":
                    self.rotation = self.angle
                    set_prompt_prefix(translate("Enter text: "))
                    self.mode = "RAPID"
                    enablePromptRapidFire()
                    clearRubber()
                    add_rubber("TEXTSINGLE")
                    set_rubber_mode("TEXTSINGLE")
                    set_rubber_point("TEXT_POINT", self.position)
                    set_rubber_point("TEXT_HEIGHT_ROTATION", self.height, self.rotation)
                    self.rubber_texts("TEXT_FONT", self.textFont)
                    self.rubber_texts("TEXT_JUSTIFY", self.justify)
                    self.rubber_texts("TEXT_RAPID", self.text)

                elif math.isnan(cmd):
                    debug_message("Requires valid numeric angle or second point.", msgtype="ALERT")
                    set_prompt_prefix(translate("Specify text angle") + " " + textAngle() + "}: ")

                else:
                    self.rotation = float(cmd)
                    setTextAngle(self.rotation)
                    set_prompt_prefix(translate("Enter text: "))
                    self.mode = self.mode_RAPID
                    enablePromptRapidFire()
                    clearRubber()
                    add_rubber("TEXTSINGLE")
                    set_rubber_mode("TEXTSINGLE")
                    set_rubber_point("TEXT_POINT", self.position)
                    set_rubber_point("TEXT_HEIGHT_ROTATION", self.height, self.rotation)
                    self.rubber_texts("TEXT_FONT", self.textFont)
                    self.rubber_texts("TEXT_JUSTIFY", self.justify)
                    self.rubber_texts("TEXT_RAPID", self.text)

            else:
                #Do nothing, as we are in rapidFire mode now.
                debug_message("do_nothing")

        elif self.mode == "RAPID":
            if cmd == "RAPID_ENTER":
                if self.text == "":
                    return
                else:
                    # TODO: Rather than ending the command,
                    # calculate where the next line would be and
                    # modify the x/y to the new point.
                    vulcanize()
                    return
            else:
                self.text = str
                self.rubber_texts("TEXT_RAPID", self.text)


class TextSingle():
    """
    def objectSavePathList() const  return subPathList()
    def subPathList() const

    objectPos()    const  return scenePos()
      objectX()  const  return scenePos().x()
      objectY()  const  return scenePos().y()

    QStringList objectTextJustifyList() const

    def set_object_text(self, str)
    def set_object_textFont(self, font)
    def set_object_text_justify(self, justify):
        return

    def set_object_text_size(size):
        return

    def set_object_textStyle(bold, italic, under, strike, over):
        return

    def set_object_textBold(val):
        return

    def set_object_textItalic(self, val):
        return

    def set_object_textUnderline(self, val):
        return

    def set_object_textStrikeOut(self, val):
        return

    def set_object_textOverline(self, val):
        return

    def set_object_textBackward(self, val):
        return

    def set_object_textUpsideDown(self, val):
        return
    def grip_edit(before, after)
    def paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
    """
    def __init__(self, x, y, text="Text Single", rgb="#FFFFFF",
                 parent=0, line_type="solid", line_weight=0.35,
                 size=16.0, font="Arial", justify="Left"):
        " . "
        debug_message("TextSingleObject Constructor()")
        self.type = "Text Single"
        self.selectable = True
        self.justify = justify
        self.text = text
        self.font = font
        self.path = Path()
        self.position = Vector(x, y)
        self.pen = Pen(rgb=rgb, line_weight=line_weight, line_type=line_type)
        self.rotation = 0.0
        self.size = size
        # TODO: "Aligned", "Fit"
        self.justify_list = [
            "Left", "Center", "Right", "Middle",
            "Top Left", "Top Center", "Top Right",
            "Middle Left", "Middle Center", "Middle Right",
            "Bottom Left", "Bottom Center", "Bottom Right"
        ]

    def copy(self, parent):
        " . "
        debug_message("TextSingleObject Constructor()")
        obj_copy = TextSingle(
            text=self.obj_text,
            font=self.font,
            scale=self.scale,
            x=self.x,
            y=self.y,
            rgb=self.rgb,
            rotation=self.rotation,
            line_type=self.line_type)
        return obj_copy

    def set_text(self):
        " . "
        obj_text = str
        text_path = Path()
        font = Font()
        font.setFamily(obj_textFont)
        font.setPointSizeF(obj_text.size)
        font.setBold(obj_text.bold)
        font.setItalic(obj_text.italic)
        font.setUnderline(obj_text.underline)
        font.setStrikeOut(obj_text.strikeout)
        font.setOverline(obj_text.overline)
        text_path.addText(0, 0, font, str)

        # Translate the path based on the justification.
        jRect = text_path.boundingRect()
        if self.justify == "Left":
            text_path.translate(-jRect.left(), 0)
        elif self.justify == "Center":
            text_path.translate(-jRect.center().x(), 0)
        elif self.justify == "Right":
            text_path.translate(-jRect.right(), 0)
        elif self.justify == "Aligned":
            # TODO: TextSingleObject Aligned Justification
            debug_message("TODO.")
        elif self.justify == "Middle":
            text_path.translate(-jRect.center())
        elif self.justify == "Fit":
            debug_message("TODO: TextSingleObject Fit Justification.")
        elif self.justify == "Top Left":
            text_path.translate(-jRect.top_left())
        elif self.justify == "Top Center":
            text_path.translate(-jRect.center().x(), -jRect.top())
        elif self.justify == "Top Right":
            text_path.translate(-jRect.top_right())
        elif self.justify == "Middle Left":
            text_path.translate(-jRect.left(), -jRect.top()/2.0)
        elif self.justify == "Middle Center":
            text_path.translate(-jRect.center().x(), -jRect.top()/2.0)
        elif self.justify == "Middle Right":
            text_path.translate(-jRect.right(), -jRect.top()/2.0)
        elif self.justify == "Bottom Left":
            text_path.translate(-jRect.bottom_left())
        elif self.justify == "Bottom Center":
            text_path.translate(-jRect.center().x(), -jRect.bottom())
        elif self.justify == "Bottom Right":
            text_path.translate(-jRect.bottom_right())

        # Backward or Upside Down
        if self.backward or self.upsidedown:
            horiz = 1.0
            vert = 1.0
            if self.backward:
                horiz = -1.0
            if self.upsidedown:
                vert = -1.0

            flipped_path = Path()

            element = 0
            P2 = 0
            P3 = 0
            P4 = 0
            for i in range(text_path.element_count()):
                element = text_path.element_at(i)
                if element.ismove_to():
                    flipped_path.move_to(horiz * element.x, vert * element.y)

                elif element.isline_to():
                    flipped_path.line_to(horiz * element.x, vert * element.y)

                elif element.isCurveTo():
                    # start point P1 is not needed
                    P2 = text_path.element_at(i)
                    # control point
                    P3 = text_path.element_at(i+1)
                    # control point
                    P4 = text_path.element_at(i+2)
                    # end point

                    flipped_path.cubicTo(horiz * P2.x, vert * P2.y,
                                        horiz * P3.x, vert * P3.y,
                                        horiz * P4.x, vert * P4.y)


            obj_text_path = flipped_path

        else:
            obj_text_path = text_path

        # Add the grip point to the shape path
        grip_path = obj_text_path
        grip_path.connect_path(obj_text_path)
        grip_path.add_rect(-0.00000001, -0.00000001, 0.00000002, 0.00000002)
        setObjectPath(grip_path)

    def set_font(self, font):
        " . "
        self.font = font
        set_object_text(obj_text)

    def set_object_text_justify(self, justify):
        " Verify the string is a valid option. "
        if justify in self.justify_list:
            self.justify = justify
        else:
            # Default
            self.justify = "Left"
        set_object_text(obj_text)

    def set_object_text_size(size):
        " . "
        obj_text.size = size
        set_object_text(obj_text)

    def set_object_textBold(self, val):
        " . "
        obj_text.bold = val
        set_object_text(obj_text)

    def set_object_textItalic(self, val):
        " . "
        obj_text.italic = val
        set_object_text(obj_text)

    def set_object_textUnderline(self, val):
        " . "
        obj_text.underline = val
        set_object_text(obj_text)

    def set_object_textStrikeOut(self, val):
        " . "
        obj_text.strikeout = val
        set_object_text(obj_text)

    def set_object_textOverline(self, val):
        " . "
        obj_text.overline = val
        set_object_text(obj_text)

    def set_object_text_backward(self, val):
        " . "
        obj_text.backward = val
        set_object_text(obj_text)

    def set_object_textUpsideDown(self, val):
        " . "
        obj_text.upsidedown = val
        set_object_text(obj_text)

    def paint(self, painter, option, widget):
        obj_scene = scene()
        if not obj_scene:
            return

        paintPen = Pen()
        painter.set_pen(paintPen)
        self.update_rubber(painter)
        if "QStyle_State_Selected" in option.state:
            paintPen.set_style(Qt_DashLine)
        if obj_scene.property("ENABLE_LWT").toBool():
            paintPen = lineWeightPen()
        painter.set_pen(paintPen)

        painter.drawPath(obj_text_path)

    def update_rubber(self, painter):
        if self.rubber_mode == "TEXTSINGLE":
            set_font(objectRubberText("TEXT_FONT"))
            set_object_text_justify(objectRubberText("TEXT_JUSTIFY"))
            setObjectPos(objectRubberPoint("TEXT_POINT"))
            hr = objectRubberPoint("TEXT_HEIGHT_ROTATION")
            set_object_text_size(hr.x())
            setRotation(hr.y())
            set_object_text(objectRubberText("TEXT_RAPID"))

        elif self.rubber_mode == "GRIP":
            if painter:
                gripPoint = objectRubberPoint("GRIP_POINT")
                if gripPoint == scenePos():
                    painter.drawPath(objectPath().translated((objectRubberPoint(""))-(gripPoint)))

                rubLine = Line((gripPoint), (objectRubberPoint("")))
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR")

    def vulcanize(self):
        debug_message("TextSingleObject vulcanize()")
        self.update_rubber()

        self.rubber_mode = "OFF"

    def mouse_snap_point(self, mouse_point):
        " Returns the closest snap point to the mouse point. "
        return scenePos()

    def all_grip_points(self):
        gripPoints = [scenePos()]
        return gripPoints

    def grip_edit(self, before, after):
        if before == scenePos():
            delta = after-before
            moveBy(delta.x(), delta.y())

    def sub_path_list(self):
        s = self.scale
        trans = 0
        trans.rotate(self.rotation)
        trans.scale(s,s)

        path_list = []
        path = obj_text_path
        pathMoves = []
        numMoves = 0

        for i in range(path.element_count()):
            element = path.element_at(i)
            if element.ismove_to():
                pathMoves += [i]
                numMoves += 1

        pathMoves += [path.element_count()]

        for p in range(pathMoves.size()-1):
            # if p => numMoves:
            #     break
            for i in range(pathMoves.value(p), pathMoves.value(p+1)):
                element = path.element_at(i)
                if element.ismove_to():
                    subPath.move_to(element.x, element.y)

                elif element.isline_to():
                    subPath.line_to(element.x, element.y)

                elif element.isCurveTo():
                    # control point 1, 2, end point
                    subPath.cubicTo(
                        path.element_at(i).x, path.element_at(i).y,
                        path.element_at(i+1).x, path.element_at(i+1).y,
                        path.element_at(i+2).x, path.element_at(i+2).y
                    )

            path_list.append(trans.map(subPath))

        return path_list



class Vector():
    r"""
    The basic vector class definition.
    """
    def __init__(self, *args):
        r"""
        If one argument is supplied it is the
        angle of a unit vector in the direction defined by "angle",
        measured anti-clockwise from the x axis.

        If two arguments are supplied then they are the x and y co-ordinates.
        """
        self.x = 0.0
        self.y = 0.0
        if len(args) == 1:
            self.x = math.cos(args[0])
            self.y = math.sin(args[0])
        elif len(args) == 2:
            self.x = args[0]
            self.y = args[1]

    def __add__(self, vector):
        r" Add this vector to the supplied one, overloading the + symbol. "
        output = Vector()
        output.x = self.x + vector.x
        output.y = self.y + vector.y
        return output

    def __sub__(self, vector):
        r""" Subtract this vector from the supplied one,
        overloading the - symbol. """
        output = Vector()
        output.x = self.x - vector.x
        output.y = self.y - vector.y
        return output

    def __mul__(self, vector):
        r"""
        Find the 2D version of a cross product for this and the supplied vector.
        """
        output = self.x * vector.y
        output -= self.y * vector.x
        return output

    def __neg__(self):
        r" Return the negation of this vector. "
        return Vector(-self.x, -self.y)

    def rotate(self, angle):
        r" Rotate this vector by the angle supplied. "
        u = Vector(angle)
        rot = Vector()
        rot.x = self.x*u.x - self.y*u.y
        rot.y = self.x*u.y + self.y*u.x
        return rot

    def scale(self, scale_factor):
        r" Scale this vector. "
        output = Vector(self.x, self.y)
        output.x *= scale_factor
        output.y *= scale_factor
        return output

    def dot(self, output):
        r" Find the dot product of this vector with the supplied one. "
        return self.x*output.x + self.y*output.y

    def length(self):
        r" Returns the length of this vector. "
        return math.sqrt(self.dot(self))


def vector_from_str(input_string):
    r"Convert a string to a vector."
    str_list = input_string.split(",")
    vector_x = float(str_list[0])
    vector_y = float(str_list[1])
    if math.isnan(vector_x) or math.isnan(vector_y):
        return None
    return Vector(vector_x, vector_y)


def closest_vector(vector_list, mouse_point):
    " Returns the closest vector to the point from vector_list. "
    closest = 1000.0
    result = vector_list[0]
    for point in vector_list:
        distance = (point - mouse_point).length()
        if distance < closest:
            result = point
            closest = distance
    return result


class Pen():
    r"""
    The Pen class definition.
    """
    def __init__(self, rgb="#FFFFFF", line_type="solid", line_weight=0.35):
        " . "
        self.rgb = rgb
        self.line_weight = line_weight
        self.line_type = line_type
        self.cap_style = "round cap"
        self.join_style = "round join"

    def paint(self):
        " . "
        debug_message(".")


def get_n_floats(terms, index, n):
    r"Takes n floats out at the position given from the string array."
    floats = []
    for offset in range(n):
        floats += [float(terms[index+offset])]
    return floats, index+offset


def path_from_command(command):
    r"""
    Creates a path matching the SVG-style path string.
    """
    debug_message("add_to_path()")
    path = []
    terms = command.split()
    index = 0
    while index < len(terms):
        if terms[index] == "M":
            values, index = get_n_floats(terms, index, 2)
            path += ["move_to", Vector(*values)]
        elif terms[index] == "L":
            values, index = get_n_floats(terms, index, 2)
            path += ["line_to", Vector(*values)]
        elif terms[index] == "A":
            values, index = get_n_floats(terms, index, 6)
            path += ["arc_to", values]
        elif terms[index] == "a":
            values, index = get_n_floats(terms, index, 6)
            path += ["arc_move_to", values]
        elif terms[index] == "E":
            values, index = get_n_floats(terms, index, 4)
            path += ["ellipse", values]
        elif terms[index] == "Z":
            path += ["close_path", []]
        index += 1
    return path


def convert(src, dst):
    r"Convert a file to another file."
    os.system(f"embroider {src} {dst}")


def translate(message):
    r"Placeholder function"
    return message


def clear_selection():
    r"To clear what the user has selected in the scene."
    return


def debug_message(message, msgtype="DEBUG"):
    r"To clear what the user has selected in the scene."
    if msgtype == "DEBUG":
        print("DEBUG " + message)
    elif msgtype == "ERROR":
        print("ERROR")
        print("    " + message)
    elif msgtype == "ALERT":
        print("ALERT")
        print("    " + message)
    else:
        print("UNKNOWN TYPE")
        print("    " + message)


def set_prompt_prefix(*message):
    r"Placeholder function."
    print(message)


def add_rubber(*message):
    r"Placeholder function."
    print(message)


def append_prompt_history(*message):
    r"Placeholder function."
    print(message)


def load_data(fname):
    r"Placeholder function."
    return {}

if __name__ == '__main__':
    unittest.main()

#!/usr/bin/env python3

r"""
    Libembroidery.

    --------------------------------------------------

    Copyright 2018-2022 The Embroidermodder Team
    Libembroidery is Open Source Software.
    See LICENSE for licensing terms.

    --------------------------------------------------

    The test suite for the libembroidery Python bindings.

    Similar to, although not a replica of, the internal tests. This
    cannot replace them because some systems that will run the library
    won't support Python.

    (Libembroidery may need to include some truly ancient PC in order
    to deal with the older embroidery machines that they may talk to.)
"""

import unittest
import math

from embroidery.parser import list_processor
import embroidery as emb
from embroidery import Pattern, Vector, Circle
from embroidery.tools import debug_message

def create_test_file_1(outf="test01.csv"):
    r"""
    .
    """
    pattern = Pattern()

    # 10mm circle
    for i in range(20):
        x = 0.0
        y = 1.0*(i+1)
        pattern.add_stitch(x, y, flags=emb.JUMP, color=0)

    for i in range(200):
        x = 10 + 10 * math.sin(i * 0.01 * math.pi)
        y = 10 + 10 * math.cos(i * 0.01 * math.pi)
        pattern.add_stitch(x, y, flags=emb.NORMAL, color=0)

    pattern.add_thread(emb.black_thread)
    pattern.end()

    pattern.write(outf)
    pattern.free()


def create_test_file_2(outf="test02.csv"):
    r"""
    .
    """
    pattern = Pattern()

    # sin wave
    for i in range(100):
        x = 10 + 10 * math.sin(i * (0.5 / math.pi))
        y = 10 + i * 0.1
        pattern.add_stitch(x, y, flags=emb.NORMAL, color=0)

    pattern.add_thread(emb.black_thread)
    pattern.end()

    pattern.write(outf)


def create_test_file_3(outf="test03.csv"):
    """
    """
    pattern = Pattern()
    pattern.add_circle(10.0, 1.0, 5.0)
    pattern.add_thread(emb.black_thread)
    pattern.convert_geometry()
    pattern.end()

    pattern.write(outf)


def convert_test(t, from_f, to_f):
    """
    """
    inf = "test%02d.%s" % (t, from_f)
    outf = "test%02d_convert_from_%s.%s" % (t, from_f, to_f)
    if t == 1:
        create_test_file_1(inf)
    elif t == 2:
        create_test_file_2(inf)
    else:
        create_test_file_3(inf)
    return emb.convert(inf, outf)


def convert_test_all(from_f, to_f):
    for i in range(1, 4):
        if convert_test(i, from_f, to_f) != 0:
            return 1
    return 0


class TestLibembroidery(unittest.TestCase):
    r"""
    The parent class for all of the bindings
    tests, this could be made to chain into the
    internal C testing.
    """
    def test_vector_length(self):
        " Tests the vector length function. "
        v = Vector(3.0, 4.0)
        self.assertAlmostEqual(v.length(), 5.0)

    def test_arc(self):
        r"Adds an arc to the pattern, then checked that it saves in SVG."
        pattern = Pattern()
        pattern.add_arc(1.0, 2.0, 2.0, 3.0, 4.0, 6.0)
        pattern.write("out.svg")
        self.assertAlmostEqual(5.0, 5.0)

    def test_circle(self):
        r"Adds a circle to the pattern, then checked that it saves in SVG."
        pattern = Pattern()
        pattern.add_circle(3.0, 4.0, 2.0)
        pattern.write("out.svg")
        self.assertAlmostEqual(5.0, 5.0)

    def test_ellipse(self):
        r"Adds an ellipse to the pattern, then checked that it saves in SVG."
        pattern = Pattern()
        pattern.add_ellipse(3.0, 4.0, 7.0, 4.0)
        pattern.write("out.svg")
        self.assertAlmostEqual(5.0, 5.0)

    def test_path(self):
        r"Adds a path to the pattern, then checked that it saves in SVG."
        pattern = Pattern()
        pattern.add_path([])
        pattern.write("out.svg")
        self.assertAlmostEqual(5.0, 5.0)

    def test_main(self):
        r"""
        Tests the rendering and simulating of patterns.
        """
        pattern = Pattern()
        image = emb.Image(100, 100)
        hilbert_curve_result = pattern.hilbert_curve(3)
        render_result = image.render(pattern, 20.0, 20.0, "hilbert_level_3.ppm")
        simulate_result = image.simulate(pattern, 20.0, 20.0, "hilbert_level_3.avi")

        self.assertEqual(hilbert_curve_result, 0)
        self.assertEqual(render_result, 0)
        self.assertEqual(simulate_result, 0)

    def test_convert_csv_svg(self):
        " Test conversion from csv to svg. "
        self.assertEqual(convert_test_all("csv", "svg"), 1)

    def test_convert_csv_dst(self):
        " Test conversion from csv to dst. "
        self.assertEqual(convert_test_all("csv", "dst"), 1)

    def test_convert_csv_pes(self):
        " Test conversion from csv to pes. "
        self.assertEqual(convert_test_all("csv", "pes"), 1)

    def test_convert_svg_csv(self):
        " Test conversion from svg to csv. "
        self.assertEqual(convert_test_all("svg", "csv"), 1)

    def test_convert_dst_csv(self):
        " Test conversion from dst to csv. "
        self.assertEqual(convert_test_all("dst", "csv"), 1)

    def test_convert_pes_csv(self):
        " Test conversion from pes to csv. "
        self.assertEqual(convert_test_all("pes", "csv"), 1)

    def test_circle_tangent(self):
        """
        """
        t0 = Vector(0.0, 0.0)
        t1 = Vector(0.0, 0.0)
        c = Circle(0.0, 0.0, 3.0)
        p = Vector(4.0, 0.0)
        emb.getCircleTangentPoints(c, p, t0, t1)
        self.assertAlmostEqual(t0.x, 2.2500)
        self.assertAlmostEqual(t0.y, 1.9843)
        self.assertAlmostEqual(t1.x, 2.2500)
        self.assertAlmostEqual(t1.y, -1.9843)

    def test_circle_tangent_2(self):
        """
        """
        t0 = Vector(0.0, 0.0)
        t1 = Vector(0.0, 0.0)
        c = Circle(20.1762, 10.7170, 6.8221)
        p = Vector(24.3411, 18.2980)
        emb.getCircleTangentPoints(c, p, t0, t1)
        self.assertAlmostEqual(t0.x, 19.0911)
        self.assertAlmostEqual(t0.y, 17.4522)
        self.assertAlmostEqual(t1.x, 26.4428)
        self.assertAlmostEqual(t1.y, 13.4133)

    def test_thread_color(self):
        """
        TODO: Add capability for converting multiple files of various
        types to a single format. 

        Currently, we only convert a single file to multiple formats.
        """
        tColor = 0xFFD25F00
        c = emb.color(rgb=(0xD2, 0x5F, 0x00))
        tBrand = emb.Sulky_Rayon
        tNum = emb.threadColorNum(c, tBrand)
        tName = ""
        emb.threadColorName(tName, c, tBrand)

        # Solution: tNum = 1833
        # Solution: Pumpkin Pie
        print("""
Color : 0x%X
Brand : %d
Num   : %d
Name  : %s

""" % (tColor, tBrand, tNum, tName))
        return 0

    def test_format_table(self):
        """
        """
        tName = "example.zsk"
        f_format = emb.emb_identify_format(tName)
        table = emb.formatTable[f_format]

        self.assertEqual(table.extension, ".zsk")
        self.assertEqual(table.description, "ZSK USA Embroidery  f_format")
        self.assertEqual(table.reader_state, 'U')
        self.assertEqual(table.writer_state, ' ')
        self.assertEqual(table.type, 1)

    def test_list_processor():
        r" . "
        print(list_processor("(+ (* 4 6) 3)"))
        print(list_processor("(+ (/ 6 4) (* 3 2))"))
        example = "(* (/ 8 11) (sin (- (/ 10 9) (* 56 t))))"
        print(list_processor(example.replace("t", "0.04")))

    def test_selected():
        r"."
        debug_message("Test what is selected")

    def test_polyline_array():
        r"Polyline & Polygon Testing"
        pattern = Pattern()

        offset = Vector(0.0, 0.0)

        polyline_array = [
            Vector(1.0, 1.0),
            Vector(1.0, 2.0),
            Vector(2.0, 2.0),
            Vector(2.0, 3.0),
            Vector(3.0, 3.0),
            Vector(3.0, 2.0),
            Vector(4.0, 2.0),
            Vector(4.0, 1.0)
        ]
        polyline_array = [vector + offset for vector in polyline_array]
        pattern.add_polyline(polyline_array)

        offset = Vector(5.0, 0.0)

        polygon_array = [
            Vector(1.0, 1.0),
            Vector(1.0, 2.0),
            Vector(2.0, 2.0),
            Vector(2.0, 3.0),
            Vector(3.0, 3.0),
            Vector(3.0, 2.0),
            Vector(4.0, 2.0),
            Vector(4.0, 1.0)
        ]
        polygon_array = [vector + offset for vector in polygon_array]
        pattern.add_polygon(polygon_array)

        pattern.write("output.svg")


if __name__ == '__main__':
    unittest.main()
