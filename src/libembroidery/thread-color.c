#include "thread-color.h"

#ifdef ARDUINO /* ARDUINO TODO: remove this line when thread-color.c is arduino compatible. This is a temporary arduino build fix. */
#else          /* ARDUINO TODO: remove this line when thread-color.c is arduino compatible. This is a temporary arduino build fix. */

int getNum_Arc_Polyester(unsigned int color) { return -1; } /*TODO: finish */
const char* getName_Arc_Polyester(unsigned int color) { return ""; } /*TODO: finish */

int getNum_Arc_Rayon(unsigned int color) { return -1; } /*TODO: finish */
const char* getName_Arc_Rayon(unsigned int color) { return ""; } /*TODO: finish */

int getNum_CoatsAndClark_Rayon(unsigned int color) { return -1; } /*TODO: finish */
const char* getName_CoatsAndClark_Rayon(unsigned int color) { return ""; } /*TODO: finish */

int getNum_Exquisite_Polyester(unsigned int color) { return -1; } /*TODO: finish */
const char* getName_Exquisite_Polyester(unsigned int color) { return ""; } /*TODO: finish */

int getNum_Fufu_Polyester(unsigned int color) { return -1; } /*TODO: finish */
const char* getName_Fufu_Polyester(unsigned int color) { return ""; } /*TODO: finish */

int getNum_Fufu_Rayon(unsigned int color) { return -1; } /*TODO: finish */
const char* getName_Fufu_Rayon(unsigned int color) { return ""; } /*TODO: finish */

int getNum_Hemingworth_Polyester(unsigned int color)
{
    switch(color)
    {
        case 0xFFFFFFFF: return 1001;
        case 0xFFDDE00F: return 1271;
        case 0xFFC9DD03: return 1272;
        case 0xFF60DD49: return 1273;
        case 0xFFFFCC1E: return 1274;
        case 0xFFFFED38: return 1275;
        case 0xFFFFA952: return 1276;
        case 0xFFFF9338: return 1277;
        case 0xFFFF585F: return 1278;
        case 0xFFF28CA3: return 1279;
        case 0xFFFE8A9E: return 1280;
        case 0xFFFC074F: return 1281;
        case 0xFFCFC3C3: return 1067;
        case 0xFFC9CAC8: return 1068;
        case 0xFFB2B4B3: return 1069;
        case 0xFFC6C6BC: return 1070;
        case 0xFF616365: return 1244;
        case 0xFF4D4F53: return 1245;
        case 0xFF8E908F: return 1072;
        case 0xFF747678: return 1077;
        case 0xFF6C6F70: return 1073;
        case 0xFF191D1F: return 1079;
        case 0xFF1B242A: return 1087;
        case 0xFF000000: return 1000;
        case 0xFFD5D2CA: return 1118;
        case 0xFFDAE3EA: return 1119;
        case 0xFFA6BCC6: return 1076;
        case 0xFF818A8F: return 1074;
        case 0xFF595A5C: return 1078;
        case 0xFFB9C9D0: return 1239;
        case 0xFF7D9AAA: return 1240;
        case 0xFF5E6A71: return 1085;
        case 0xFF003C69: return 1241;
        case 0xFF003946: return 1242;
        case 0xFF004250: return 1081;
        case 0xFF37424A: return 1086;
        case 0xFF005B82: return 1192;
        case 0xFF5E9CAE: return 1193;
        case 0xFF6AADE4: return 1188;
        case 0xFF4B92DB: return 1187;
        case 0xFF8FCAE7: return 1186;
        case 0xFFC2DEEA: return 1185;
        case 0xFFA0CFEB: return 1256;
        case 0xFF0098DB: return 1255;
        case 0xFF3D7EDB: return 1202;
        case 0xFF0039A6: return 1203;
        case 0xFF00338D: return 1204;
        case 0xFF0B2265: return 1205;
        case 0xFF98C6EA: return 1189;
        case 0xFFAACAE6: return 1190;
        case 0xFF8EBAE5: return 1191;
        case 0xFF0073CF: return 1198;
        case 0xFF004165: return 1201;
        case 0xFF004153: return 1200;
        case 0xFF002244: return 1199;
        case 0xFF002C5F: return 1265;
        case 0xFF002857: return 1264;
        case 0xFF003591: return 1263;
        case 0xFF002C77: return 1261;
        case 0xFF6F9AD3: return 1262;
        case 0xFF65CFE9: return 1197;
        case 0xFF0075B0: return 1195;
        case 0xFF0066A1: return 1196;
        case 0xFF006983: return 1194;
        case 0xFF003D4C: return 1258;
        case 0xFF0098C3: return 1259;
        case 0xFF00B0CA: return 1260;
        case 0xFF6FD4E4: return 1257;
        case 0xFFBBE7E6: return 1178;
        case 0xFFC1E2E5: return 1172;
        case 0xFF8FDFE2: return 1173;
        case 0xFF00AFD8: return 1174;
        case 0xFF006778: return 1181;
        case 0xFF007C92: return 1180;
        case 0xFF009AA6: return 1176;
        case 0xFF7CA295: return 1175;
        case 0xFF63CECA: return 1177;
        case 0xFF00877C: return 1179;
        case 0xFF007B69: return 1182;
        case 0xFF024E43: return 1183;
        case 0xFF004953: return 1184;
        case 0xFF156570: return 1082;
        case 0xFF00505C: return 1113;
        case 0xFF44697D: return 1084;
        case 0xFF496C60: return 1114;
        case 0xFF949D9E: return 1115;
        case 0xFF91BAA3: return 1243;
        case 0xFFB9CCC3: return 1100;
        case 0xFFA6E6BC: return 1088;
        case 0xFF00B588: return 1094;
        case 0xFF00985F: return 1106;
        case 0xFF009B74: return 1107;
        case 0xFF007D57: return 1105;
        case 0xFF006A4D: return 1104;
        case 0xFF00685B: return 1254;
        case 0xFF0D776E: return 1253;
        case 0xFFC3E76F: return 1099;
        case 0xFFCCDC00: return 1247;
        case 0xFF69BE28: return 1097;
        case 0xFF92D400: return 1091;
        case 0xFF7AB800: return 1092;
        case 0xFF3F9C35: return 1093;
        case 0xFF00AF3F: return 1248;
        case 0xFF007934: return 1095;
        case 0xFF008542: return 1108;
        case 0xFF00693C: return 1109;
        case 0xFF1C453B: return 1250;
        case 0xFF175E54: return 1249;
        case 0xFFC8E59A: return 1096;
        case 0xFF69923A: return 1251;
        case 0xFF557630: return 1252;
        case 0xFF739600: return 1089;
        case 0xFF53682B: return 1090;
        case 0xFF035642: return 1103;
        case 0xFF284E36: return 1110;
        case 0xFF004438: return 1111;
        case 0xFF004D46: return 1112;
        case 0xFF57584F: return 1121;
        case 0xFF404A29: return 1123;
        case 0xFF83847A: return 1120;
        case 0xFF827C34: return 1058;
        case 0xFFB19B00: return 1061;
        case 0xFFB5A300: return 1059;
        case 0xFFF8E498: return 1060;
        case 0xFFD7D3C7: return 1229;
        case 0xFFD5C833: return 1098;
        case 0xFF6A7029: return 1101;
        case 0xFF898F4B: return 1102;
        case 0xFF65551C: return 1246;
        case 0xFF4B452C: return 1117;
        case 0xFF4B471A: return 1116;
        case 0xFF718674: return 1083;
        case 0xFF4F4C25: return 1125;
        case 0xFF5D4F4B: return 1131;
        case 0xFF452325: return 1126;
        case 0xFF4E2E2D: return 1128;
        case 0xFF6E3219: return 1130;
        case 0xFF60351D: return 1134;
        case 0xFF6C4D23: return 1140;
        case 0xFF766A65: return 1237;
        case 0xFF5B491F: return 1137;
        case 0xFF6E5A2A: return 1135;
        case 0xFFAB8422: return 1136;
        case 0xFF856822: return 1122;
        case 0xFF675C53: return 1236;
        case 0xFF9A996E: return 1124;
        case 0xFFC2B2B5: return 1149;
        case 0xFFA5ACAF: return 1146;
        case 0xFFAE7D5B: return 1145;
        case 0xFFA76F3E: return 1133;
        case 0xFFA25022: return 1064;
        case 0xFF86431E: return 1238;
        case 0xFFB2541A: return 1163;
        case 0xFF833820: return 1164;
        case 0xFF9A3B26: return 1144;
        case 0xFF825C26: return 1142;
        case 0xFFC59217: return 1063;
        case 0xFFDDB99A: return 1062;
        case 0xFFD2C295: return 1057;
        case 0xFFC2C2A0: return 1056;
        case 0xFFB3B38C: return 1055;
        case 0xFFC7B37F: return 1054;
        case 0xFFBD9271: return 1171;
        case 0xFFB3995D: return 1138;
        case 0xFFCEA98C: return 1235;
        case 0xFFE39B6C: return 1139;
        case 0xFFBA6F2E: return 1132;
        case 0xFFBB650E: return 1141;
        case 0xFFEBE8B1: return 1042;
        case 0xFFEEEC83: return 1043;
        case 0xFFF3EC7A: return 1045;
        case 0xFFF5EC5A: return 1225;
        case 0xFFFAE700: return 1226;
        case 0xFFF2EE72: return 1044;
        case 0xFFFCD900: return 1227;
        case 0xFFFADC41: return 1046;
        case 0xFFFED100: return 1047;
        case 0xFFF3CF45: return 1048;
        case 0xFFEFBD47: return 1050;
        case 0xFFEAAB00: return 1051;
        case 0xFFDCD6B2: return 1037;
        case 0xFFC6BC89: return 1038;
     /* case 0xFFF8E498: return 1039;    TODO: duplicate case value */
        case 0xFFF8DE6E: return 1040;
        case 0xFFFADA63: return 1049;
        case 0xFFFFCB4F: return 1053;
        case 0xFFFFA100: return 1232;
        case 0xFFFFB612: return 1231;
        case 0xFFFFB652: return 1230;
        case 0xFFFFBC3D: return 1041;
        case 0xFFCE8E00: return 1052;
        case 0xFF9D5324: return 1143;
        case 0xFFE98300: return 1024;
        case 0xFFFF7000: return 1025;
        case 0xFFE37222: return 1027;
        case 0xFFFB4F14: return 1028;
        case 0xFFDD4814: return 1029;
        case 0xFFCD202C: return 1030;
        case 0xFFC30014: return 1270;
        case 0xFFA70232: return 1032;
        case 0xFF882332: return 1031;
        case 0xFFA51100: return 1002;
        case 0xFF9E3039: return 1234;
        case 0xFF783014: return 1233;
        case 0xFFD55C19: return 1065;
        case 0xFFAA272F: return 1066;
        case 0xFF5F3327: return 1129;
        case 0xFF5D3526: return 1127;
        case 0xFF592C35: return 1160;
        case 0xFF6A1A41: return 1159;
        case 0xFF6E2714: return 1158;
        case 0xFF662046: return 1157;
        case 0xFF85003C: return 1156;
        case 0xFF641F14: return 1155;
        case 0xFFFF818D: return 1166;
        case 0xFFFFA48A: return 1015;
        case 0xFFFFB0B7: return 1011;
        case 0xFFF3789B: return 1012;
        case 0xFFDB4D69: return 1014;
        case 0xFF91004B: return 1013;
        case 0xFF82240C: return 1224;
        case 0xFFF54359: return 1018;
        case 0xFFF4587A: return 1017;
        case 0xFFFF8B7C: return 1016;
        case 0xFFFFC19C: return 1022;
        case 0xFFFF8F70: return 1020;
        case 0xFFFF6D42: return 1026;
        case 0xFFFFA02F: return 1023;
        case 0xFFECC182: return 1168;
        case 0xFFFBCE92: return 1021;
        case 0xFFFDC480: return 1228;
        case 0xFFFFC2A2: return 1167;
        case 0xFFEFC5CE: return 1169;
        case 0xFFEFBE9C: return 1170;
        case 0xFF774A39: return 1162;
        case 0xFFB26F7E: return 1151;
        case 0xFFD490A8: return 1165;
        case 0xFFF6A3BB: return 1161;
        case 0xFFFFB7AE: return 1019;
        case 0xFFEFD6DB: return 1150;
        case 0xFF920075: return 1036;
        case 0xFFF375C6: return 1004;
        case 0xFFF3BBCE: return 1003;
        case 0xFFF1DBDF: return 1005;
        case 0xFFF3C9D3: return 1006;
        case 0xFFF4B2C1: return 1007;
        case 0xFFF39EBC: return 1008;
        case 0xFFF77AB4: return 1009;
        case 0xFFD71F85: return 1010;
        case 0xFF772059: return 1035;
        case 0xFFC50084: return 1034;
        case 0xFFA1006B: return 1033;
        case 0xFFD1D4D3: return 1148;
        case 0xFFCAD1E7: return 1147;
        case 0xFF9DABE2: return 1207;
        case 0xFF8884D5: return 1206;
        case 0xFF1A2155: return 1209;
        case 0xFF6459C4: return 1211;
        case 0xFF212492: return 1208;
        case 0xFF411244: return 1210;
        case 0xFF3B0083: return 1223;
        case 0xFF151C54: return 1267;
        case 0xFF490E6F: return 1269;
        case 0xFF57068C: return 1268;
        case 0xFFDCC7DF: return 1217;
        case 0xFFC2ACBE: return 1219;
        case 0xFFDC9DDD: return 1218;
        case 0xFFB382C7: return 1213;
        case 0xFF9C5FB5: return 1214;
        case 0xFF4B08A1: return 1266;
        case 0xFFC1AFE5: return 1221;
        case 0xFFC5B9E3: return 1222;
        case 0xFF6E2C6B: return 1215;
        case 0xFF7D0063: return 1216;
        case 0xFF752864: return 1220;
        case 0xFF55517B: return 1080;
        case 0xFF5C7F92: return 1075;
        case 0xFFAFADC3: return 1071;
        case 0xFFD8AAB3: return 1152;
        case 0xFF89687C: return 1153;
        case 0xFF644459: return 1154;
        case 0xFF4B306A: return 1212;
    }

    return -1;
}
const char* getName_Hemingworth_Polyester(unsigned int color)
{
    switch(color)
    {
        case 0xFFFFFFFF: return "Pure White";
        case 0xFFDDE00F: return "Lemon Ice";
        case 0xFFC9DD03: return "Neon Green";
        case 0xFF60DD49: return "Brilliant Lime";
        case 0xFFFFCC1E: return "Mango";
        case 0xFFFFED38: return "Neon Yellow";
        case 0xFFFFA952: return "Tropical Orange";
        case 0xFFFF9338: return "Neon Orange";
        case 0xFFFF585F: return "Rebel Peach";
        case 0xFFF28CA3: return "Shy Flamingo";
        case 0xFFFE8A9E: return "Neon Pink";
        case 0xFFFC074F: return "Neon Peach";
        case 0xFFCFC3C3: return "Dove Gray";
        case 0xFFC9CAC8: return "Silver Lining";
        case 0xFFB2B4B3: return "Storm Cloud";
        case 0xFFC6C6BC: return "Platinum";
        case 0xFF616365: return "Graphite";
        case 0xFF4D4F53: return "Light Charcoal";
        case 0xFF8E908F: return "Chrome";
        case 0xFF747678: return "Antique Silver";
        case 0xFF6C6F70: return "Pewter Gray";
        case 0xFF191D1F: return "Black Stallion";
        case 0xFF1B242A: return "Charcoal";
        case 0xFF000000: return "Classic Black";
        case 0xFFD5D2CA: return "Marshmallow";
        case 0xFFDAE3EA: return "Ice Blue";
        case 0xFFA6BCC6: return "Nautical Blue";
        case 0xFF818A8F: return "Sea Storm";
        case 0xFF595A5C: return "Bronzed Steel";
        case 0xFFB9C9D0: return "Silvery Gray";
        case 0xFF7D9AAA: return "Granite";
        case 0xFF5E6A71: return "Shadow";
        case 0xFF003C69: return "Dark Slate Blue";
        case 0xFF003946: return "Deep Slate Blue";
        case 0xFF004250: return "Pacific Waters";
        case 0xFF37424A: return "Dark Slate";
        case 0xFF005B82: return "Smoky Blue";
        case 0xFF5E9CAE: return "Light Slate Blue";
        case 0xFF6AADE4: return "Hyacinth";
        case 0xFF4B92DB: return "Bluebird";
        case 0xFF8FCAE7: return "Misty Blue";
        case 0xFFC2DEEA: return "Cornflower Blue";
        case 0xFFA0CFEB: return "Pale Blue";
        case 0xFF0098DB: return "Country Blue";
        case 0xFF3D7EDB: return "Azure";
        case 0xFF0039A6: return "Royal Blue";
        case 0xFF00338D: return "Brilliant Blue";
        case 0xFF0B2265: return "Deep Blue";
        case 0xFF98C6EA: return "Winter Blue";
        case 0xFFAACAE6: return "Winter Sky";
        case 0xFF8EBAE5: return "Sky Blue";
        case 0xFF0073CF: return "China Blue";
        case 0xFF004165: return "Dark Blueberry";
        case 0xFF004153: return "Salem Blue";
        case 0xFF002244: return "Navy";
        case 0xFF002C5F: return "Sailor Blue";
        case 0xFF002857: return "Dark Blue ";
        case 0xFF003591: return "Berry Blue";
        case 0xFF002C77: return "True Blue";
        case 0xFF6F9AD3: return "Periwinkle";
        case 0xFF65CFE9: return "Iceberg Blue";
        case 0xFF0075B0: return "Medium Aquamarine";
        case 0xFF0066A1: return "Dark Aquamarine";
        case 0xFF006983: return "Peacock Blue";
        case 0xFF003D4C: return "Dark Turquoise";
        case 0xFF0098C3: return "Turquoise";
        case 0xFF00B0CA: return "Caribbean Blue";
        case 0xFF6FD4E4: return "Summer Sky";
        case 0xFFBBE7E6: return "Crystal Lake";
        case 0xFFC1E2E5: return "Icicle Blue";
        case 0xFF8FDFE2: return "Frosty Blue";
        case 0xFF00AFD8: return "Blue Lagoon";
        case 0xFF006778: return "Blue Satin";
        case 0xFF007C92: return "Teal Blue";
        case 0xFF009AA6: return "Light Teal Blue";
        case 0xFF7CA295: return "Wintergreen";
        case 0xFF63CECA: return "Mint Green";
        case 0xFF00877C: return "Navajo Turquoise";
        case 0xFF007B69: return "Peacock Green";
        case 0xFF024E43: return "Forest Glen";
        case 0xFF004953: return "Deep Teal";
        case 0xFF156570: return "Deep Sea";
        case 0xFF00505C: return "Dragonfly";
        case 0xFF44697D: return "Blue Steel";
        case 0xFF496C60: return "Dark Sage";
        case 0xFF949D9E: return "Silver Green";
        case 0xFF91BAA3: return "Antique Gray";
        case 0xFFB9CCC3: return "Ocean Spray";
        case 0xFFA6E6BC: return "Sea Foam";
        case 0xFF00B588: return "Cucumber Melon";
        case 0xFF00985F: return "Light Jade";
        case 0xFF009B74: return "Jade";
        case 0xFF007D57: return "Dark Jade";
        case 0xFF006A4D: return "Caribbean";
        case 0xFF00685B: return "Dark Teal";
        case 0xFF0D776E: return "Minty Teal";
        case 0xFFC3E76F: return "Lemony Lime";
        case 0xFFCCDC00: return "Kiwi Lime";
        case 0xFF69BE28: return "Electric Green";
        case 0xFF92D400: return "Green Apple";
        case 0xFF7AB800: return "Key Lime";
        case 0xFF3F9C35: return "Kelly Green";
        case 0xFF00AF3F: return "Meadow";
        case 0xFF007934: return "Grassy Green";
        case 0xFF008542: return "Dark Kelly Green";
        case 0xFF00693C: return "Christmas Green";
        case 0xFF1C453B: return "Winter Pine ";
        case 0xFF175E54: return "Holly Leaf";
        case 0xFFC8E59A: return "Pistachio Nut";
        case 0xFF69923A: return "Dusty Green";
        case 0xFF557630: return "Bush Ivy ";
        case 0xFF739600: return "Leafy Green";
        case 0xFF53682B: return "Kentucky Grass";
        case 0xFF035642: return "Ivy";
        case 0xFF284E36: return "Evergreen";
        case 0xFF004438: return "Mountain Meadow";
        case 0xFF004D46: return "Forest Green";
        case 0xFF57584F: return "Oregano";
        case 0xFF404A29: return "Jungle Green";
        case 0xFF83847A: return "Thyme";
        case 0xFF827C34: return "Light Avocado";
        case 0xFFB19B00: return "Split Pea";
        case 0xFFB5A300: return "Spring Leaf";
        case 0xFFF8E498: return "Almond Cream";
        case 0xFFD7D3C7: return "Eggshell";
        case 0xFFD5C833: return "Cornsilk Green";
        case 0xFF6A7029: return "Avocado";
        case 0xFF898F4B: return "Seaweed";
        case 0xFF65551C: return "Olive Green";
        case 0xFF4B452C: return "Coconut Shell";
        case 0xFF4B471A: return "Parsley";
        case 0xFF718674: return "Dried Sage";
        case 0xFF4F4C25: return "Mocha";
        case 0xFF5D4F4B: return "Warm Earth";
        case 0xFF452325: return "Dark Chocolate";
        case 0xFF4E2E2D: return "Deep Walnut";
        case 0xFF6E3219: return "Teddybear Brown";
        case 0xFF60351D: return "Light Chestnut";
        case 0xFF6C4D23: return "Pecan Pie";
        case 0xFF766A65: return "Dark Alder";
        case 0xFF5B491F: return "Army Green";
        case 0xFF6E5A2A: return "Pharaoh Gold";
        case 0xFFAB8422: return "Autumn Haystack";
        case 0xFF856822: return "Sahara";
        case 0xFF675C53: return "Weathered Wood";
        case 0xFF9A996E: return "Soft Beige";
        case 0xFFC2B2B5: return "Mercury";
        case 0xFFA5ACAF: return "Old Lace";
        case 0xFFAE7D5B: return "Caramel Cappuccino";
        case 0xFFA76F3E: return "Brown Sugar";
        case 0xFFA25022: return "Light Cinnamon";
        case 0xFF86431E: return "Cinnamon";
        case 0xFFB2541A: return "Apple Cider";
        case 0xFF833820: return "Rust";
        case 0xFF9A3B26: return "Indian Paintbrush";
        case 0xFF825C26: return "Toasted Almond";
        case 0xFFC59217: return "Pale Caramel";
        case 0xFFDDB99A: return "Honey Butter";
        case 0xFFD2C295: return "Sandy Shore";
        case 0xFFC2C2A0: return "Ecru";
        case 0xFFB3B38C: return "Malt";
        case 0xFFC7B37F: return "Antique Lace";
        case 0xFFBD9271: return "Champagne";
        case 0xFFB3995D: return "Butter Taffy";
        case 0xFFCEA98C: return "Cream Soda";
        case 0xFFE39B6C: return "Conch Shell";
        case 0xFFBA6F2E: return "New Penny";
        case 0xFFBB650E: return "Pumpkin Spice";
        case 0xFFEBE8B1: return "Soft Sunlight";
        case 0xFFEEEC83: return "Lemon Drop";
        case 0xFFF3EC7A: return "Daffodil";
        case 0xFFF5EC5A: return "Lemon Citrus";
        case 0xFFFAE700: return "Sunshine Yellow";
        case 0xFFF2EE72: return "Canary Yellow";
        case 0xFFFCD900: return "Sunflower";
        case 0xFFFADC41: return "Sun";
        case 0xFFFED100: return "Dandelion";
        case 0xFFF3CF45: return "Buttercup";
        case 0xFFEFBD47: return "Ginger Root";
        case 0xFFEAAB00: return "Goldenrod";
        case 0xFFDCD6B2: return "Cornsilk";
        case 0xFFC6BC89: return "Macadamia";
     /* case 0xFFF8E498: return "Yellow Plumeria";    TODO: duplicate case value */
        case 0xFFF8DE6E: return "Maize";
        case 0xFFFADA63: return "Dried Banana";
        case 0xFFFFCB4F: return "Butternut";
        case 0xFFFFA100: return "Orange Meringue";
        case 0xFFFFB612: return "September Sunset";
        case 0xFFFFB652: return "Orange Cream";
        case 0xFFFFBC3D: return "Cantaloupe";
        case 0xFFCE8E00: return "Old Gold";
        case 0xFF9D5324: return "Auburn";
        case 0xFFE98300: return "Citrus Burst";
        case 0xFFFF7000: return "Orange Slice";
        case 0xFFE37222: return "Fiery Sunset";
        case 0xFFFB4F14: return "Hunter Orange";
        case 0xFFDD4814: return "Fall Harvest";
        case 0xFFCD202C: return "Candy Apple";
        case 0xFFC30014: return "Christmas Red";
        case 0xFFA70232: return "Pomegranate";
        case 0xFF882332: return "Rummy Raisin";
        case 0xFFA51100: return "Cardinal Red";
        case 0xFF9E3039: return "Rusty Red";
        case 0xFF783014: return "Redwood";
        case 0xFFD55C19: return "Carrot";
        case 0xFFAA272F: return "Paprika";
        case 0xFF5F3327: return "Cherrywood";
        case 0xFF5D3526: return "Burnt Sienna";
        case 0xFF592C35: return "Merlot";
        case 0xFF6A1A41: return "Loganberry";
        case 0xFF6E2714: return "Cranberry";
        case 0xFF662046: return "Mulberry";
        case 0xFF85003C: return "Magenta";
        case 0xFF641F14: return "Raspberry";
        case 0xFFFF818D: return "Salmon";
        case 0xFFFFA48A: return "Georgia Peach";
        case 0xFFFFB0B7: return "Rose Sunset";
        case 0xFFF3789B: return "Bubblegum Pink";
        case 0xFFDB4D69: return "Carnation";
        case 0xFF91004B: return "Very Berry";
        case 0xFF82240C: return "Raspberry Red";
        case 0xFFF54359: return "Dark Salmon";
        case 0xFFF4587A: return "Apricot Dream";
        case 0xFFFF8B7C: return "Coral Reef";
        case 0xFFFFC19C: return "Frosted Peach";
        case 0xFFFF8F70: return "Tangerine";
        case 0xFFFF6D42: return "Dark Mango";
        case 0xFFFFA02F: return "Marigold";
        case 0xFFECC182: return "Spun Silk";
        case 0xFFFBCE92: return "Whipped Papaya";
        case 0xFFFDC480: return "Baby Peach";
        case 0xFFFFC2A2: return "Pink Pearl";
        case 0xFFEFC5CE: return "Peaches 'n Cream";
        case 0xFFEFBE9C: return "Peach Pastel";
        case 0xFF774A39: return "Old Penny";
        case 0xFFB26F7E: return "Dusty Rose";
        case 0xFFD490A8: return "Winter Rose";
        case 0xFFF6A3BB: return "Valentine Pink";
        case 0xFFFFB7AE: return "Petal Peach";
        case 0xFFEFD6DB: return "Soft Petal";
        case 0xFF920075: return "Fuchsia";
        case 0xFFF375C6: return "Pink Kiss";
        case 0xFFF3BBCE: return "Baby Pink";
        case 0xFFF1DBDF: return "Whisper Pink";
        case 0xFFF3C9D3: return "Gentle Blush";
        case 0xFFF4B2C1: return "English Rose";
        case 0xFFF39EBC: return "Sweet Pea";
        case 0xFFF77AB4: return "Rosy Blush";
        case 0xFFD71F85: return "Passion Pink";
        case 0xFF772059: return "Mulled Wine";
        case 0xFFC50084: return "Primrose";
        case 0xFFA1006B: return "Azalea";
        case 0xFFD1D4D3: return "Snowflake";
        case 0xFFCAD1E7: return "Moonlight";
        case 0xFF9DABE2: return "Tulip";
        case 0xFF8884D5: return "Purple Iris";
        case 0xFF1A2155: return "Grape";
        case 0xFF6459C4: return "Moon Shadow";
        case 0xFF212492: return "Electric Purple";
        case 0xFF411244: return "Indigo";
        case 0xFF3B0083: return "Royal Purple";
        case 0xFF151C54: return "Eggplant";
        case 0xFF490E6F: return "Dark Purple";
        case 0xFF57068C: return "Pure Purple";
        case 0xFFDCC7DF: return "Pale Orchid";
        case 0xFFC2ACBE: return "Dusty Mauve";
        case 0xFFDC9DDD: return "Orchid";
        case 0xFFB382C7: return "Heather";
        case 0xFF9C5FB5: return "Lavender";
        case 0xFF4B08A1: return "Soft Grape";
        case 0xFFC1AFE5: return "Freesia";
        case 0xFFC5B9E3: return "Lilac";
        case 0xFF6E2C6B: return "Peony";
        case 0xFF7D0063: return "Dark Fuschia";
        case 0xFF752864: return "Grape Jelly";
        case 0xFF55517B: return "Deep Orchid";
        case 0xFF5C7F92: return "Misty Blue Gray";
        case 0xFFAFADC3: return "Iron Ore";
        case 0xFFD8AAB3: return "Light Mauve";
        case 0xFF89687C: return "Dark Mauve";
        case 0xFF644459: return "Wild Plum";
        case 0xFF4B306A: return "Huckleberry";
    }

    return "";
}

int getNum_Isacord_Polyester(unsigned int color)
{
    switch(color)
    {
        case 0xFFFFFFFF: return   10;
     /* case 0xFFFFFFFF: return   15;    TODO: duplicate case value */
     /* case 0xFFFFFFFF: return   17;    TODO: duplicate case value */
        case 0xFF000000: return   20;
        case 0xFFFFFDED: return  101;
     /* case 0xFF7D7D7D: break;          NOTE: N/A COLOR */
     /* case 0xFF515B61: break;          NOTE: N/A COLOR */
        case 0xFF6D757B: return  108;
     /* case 0xFFACACAC: break;          NOTE: N/A COLOR */
        case 0xFF515B61: return  111;
        case 0xFF5D5D5D: return  112;
        case 0xFFCFCFCF: return  124;
        case 0xFFA1A9B4: return  131;
        case 0xFF192024: return  132;
     /* case 0xFF6D757B: break;          NOTE: N/A COLOR */
        case 0xFF9EA5AA: return  142;
        case 0xFFCFD1D5: return  145;
        case 0xFFC6BDB4: return  150;
        case 0xFFD5C4B3: return  151;
        case 0xFF7C8283: return  152;
     /* case 0xFF898F94: break;          NOTE: N/A COLOR */
        case 0xFFFEF5F0: return  180;
        case 0xFFE9D7D9: return  182;
        case 0xFFEBE3DD: return  184;
        case 0xFFE0DA5F: return  221;
        case 0xFFBFBA28: return  232;
     /* case 0xFFECE9C1: break;          NOTE: N/A COLOR */
        case 0xFFFAF6CC: return  250;
     /* case 0xFFECE7A5: break;          NOTE: N/A COLOR */
     /* case 0xFFECEADB: break;          NOTE: N/A COLOR */
        case 0xFFF9F8E8: return  270;
        case 0xFFFDF76C: return  310;
        case 0xFFF5D300: return  311;
        case 0xFF797E24: return  345;
        case 0xFFB0AA76: return  352;
        case 0xFF898F2B: return  442;
        case 0xFF98996D: return  453;
     /* case 0xFF6E772E: break;          NOTE: N/A COLOR */
        case 0xFF6B7E6F: return  463;
        case 0xFF3E4F34: return  465;
        case 0xFFEDEF05: return  501;
     /* case 0xFFFAF6CC: break;          NOTE: N/A COLOR */
     /* case 0xFFF5D300: return  506;    TODO: duplicate case value */
     /* case 0xFFFFFBD1: break;          NOTE: N/A COLOR */
        case 0xFFFDE896: return  520;
        case 0xFFD7CE8A: return  532;
        case 0xFFB18B00: return  542;
     /* case 0xFFAA8D00: break;          NOTE: N/A COLOR */
        case 0xFFB28F11: return  546;
     /* case 0xFFAC9436: break;          NOTE: N/A COLOR */
        case 0xFFB69F56: return  552;
     /* case 0xFFF4EE8C: break;          NOTE: N/A COLOR */
     /* case 0xFFF1EB35: break;          NOTE: N/A COLOR */
        case 0xFFF8D73E: return  600;
     /* case 0xFFF8D73E: return  605;    TODO: duplicate case value */
        case 0xFFF7DC00: return  608;
     /* case 0xFFF7CB47: break;          NOTE: N/A COLOR */
     /* case 0xFFF7E400: break;          NOTE: N/A COLOR */
     /* case 0xFFFDE896: break;          NOTE: N/A COLOR */
     /* case 0xFFEEDB00: break;          NOTE: N/A COLOR */
        case 0xFFFEF09A: return  630;
     /* case 0xFFFDE1AF: break;          NOTE: N/A COLOR */
     /* case 0xFFFDE896: return  640;    TODO: duplicate case value */
        case 0xFFF5D2A6: return  651;
        case 0xFFFEF9EA: return  660;
        case 0xFFFAF6E7: return  670;
        case 0xFFBEBEA8: return  672;
        case 0xFFF7C35F: return  700;
        case 0xFFF5CA00: return  702;
        case 0xFFDFA200: return  704;
        case 0xFFFCF538: return  706;
        case 0xFFFADC59: return  713;
        case 0xFF8C7E6A: return  722;
     /* case 0xFF827000: break;          NOTE: N/A COLOR */
     /* case 0xFF636254: break;          NOTE: N/A COLOR */
        case 0xFF594900: return  747;
        case 0xFFD6BF94: return  761;
        case 0xFF656452: return  776;
        case 0xFFF1AF00: return  800;
     /* case 0xFFF3C200: break;          NOTE: N/A COLOR */
        case 0xFFF5BA5D: return  811;
        case 0xFFE1A23E: return  821;
        case 0xFFCCAB3F: return  822;
     /* case 0xFFDFA200: return  824;    TODO: duplicate case value */
     /* case 0xFFF3B044: break;          NOTE: N/A COLOR */
        case 0xFFD0A44F: return  832;
        case 0xFFCD944A: return  842;
        case 0xFFE3BC61: return  851;
        case 0xFF947C4A: return  853;
        case 0xFFCBBFA2: return  861;
        case 0xFFA5866A: return  862;
     /* case 0xFF001F48: break;          NOTE: N/A COLOR */
        case 0xFFEBE7DD: return  870;
        case 0xFF9FA086: return  873;
        case 0xFF9A897B: return  874;
     /* case 0xFFEE9C00: break;          NOTE: N/A COLOR */
        case 0xFFF3B259: return  904;
        case 0xFFCA832C: return  922;
        case 0xFFC07314: return  931;
        case 0xFFAC6613: return  932;
        case 0xFF744808: return  933;
        case 0xFFBD9565: return  934;
     /* case 0xFF806800: break;          NOTE: N/A COLOR */
        case 0xFFC98300: return  940;
        case 0xFFAF7D3E: return  941;
        case 0xFF483928: return  945;
     /* case 0xFFFEECD9: break;          NOTE: N/A COLOR */
        case 0xFFFEFEED: return  970;
     /* case 0xFFDD973A: break;          NOTE: N/A COLOR */
        case 0xFF6A4129: return 1055;
        case 0xFFFDE2C1: return 1060;
        case 0xFFA68A68: return 1061;
     /* case 0xFFD76814: break;          NOTE: N/A COLOR */
     /* case 0xFFED873F: break;          NOTE: N/A COLOR */
     /* case 0xFFEC870E: break;          NOTE: N/A COLOR */
        case 0xFFED9206: return 1102;
        case 0xFFEE9C00: return 1106;
     /* case 0xFFC45331: break;          NOTE: N/A COLOR */
        case 0xFFEE8751: return 1114;
        case 0xFFA35214: return 1115;
        case 0xFFF8C000: return 1120;
        case 0xFFB7976B: return 1123;
        case 0xFF9D5A21: return 1134;
        case 0xFFF3D8A8: return 1140;
        case 0xFFFACFAE: return 1141;
        case 0xFF7A4427: return 1154;
        case 0xFFDFC8AB: return 1172;
        case 0xFFE89763: return 1211;
        case 0xFFF1A236: return 1220;
     /* case 0xFF3D2723: break;          NOTE: N/A COLOR */
        case 0xFFE5571D: return 1300;
        case 0xFFD9674C: return 1301;
     /* case 0xFFE8643C: break;          NOTE: N/A COLOR */
        case 0xFFE4501E: return 1304;
        case 0xFFEA7134: return 1305;
        case 0xFFE12A23: return 1306;
        case 0xFFC14817: return 1311;
        case 0xFFC45331: return 1312;
     /* case 0xFFD7623E: break;          NOTE: N/A COLOR */
     /* case 0xFFED7C56: break;          NOTE: N/A COLOR */
     /* case 0xFF92291B: break;          NOTE: N/A COLOR */
        case 0xFFD5815E: return 1332;
        case 0xFFBB3D2E: return 1334;
        case 0xFFBE2D1A: return 1335;
        case 0xFF5F1B23: return 1342;
        case 0xFF7A3441: return 1346;
     /* case 0xFF84291D: break;          NOTE: N/A COLOR */
        case 0xFFFBBF95: return 1351;
        case 0xFFF4A773: return 1352;
        case 0xFF693920: return 1355;
     /* case 0xFFF9C6A1: break;          NOTE: N/A COLOR */
        case 0xFFF9C598: return 1362;
        case 0xFF432731: return 1366;
        case 0xFF464537: return 1375;
     /* case 0xFF4D2E18: break;          NOTE: N/A COLOR */
     /* case 0xFFD64F42: break;          NOTE: N/A COLOR */
        case 0xFFF4A782: return 1430;
        case 0xFFE22D2A: return 1501;
        case 0xFFA93121: return 1514;
        case 0xFFEC7168: return 1521;
        case 0xFFF6B08E: return 1532;
        case 0xFFF9C5B9: return 1551;
        case 0xFF806A61: return 1565;
     /* case 0xFF464537: break;          NOTE: N/A COLOR */
        case 0xFFE36C63: return 1600;
     /* case 0xFFF9C7B9: break;          NOTE: N/A COLOR */
        case 0xFFE44733: return 1701;
        case 0xFFDF0032: return 1703;
        case 0xFFE0003D: return 1704;
     /* case 0xFFE44733: break;          NOTE: N/A COLOR */
        case 0xFFCF0040: return 1725;
     /* case 0xFFDB686B: break;          NOTE: N/A COLOR */
        case 0xFFF1CDCE: return 1755;
        case 0xFFE9C9BD: return 1760;
        case 0xFFE8C0B8: return 1761;
        case 0xFFE00046: return 1800;
     /* case 0xFFE43449: break;          NOTE: N/A COLOR */
        case 0xFFD6445D: return 1805;
        case 0xFFF49E95: return 1840;
     /* case 0xFFB76663: break;          NOTE: N/A COLOR */
     /* case 0xFFE36C63: break;          NOTE: N/A COLOR */
     /* case 0xFFF0887D: break;          NOTE: N/A COLOR */
     /* case 0xFFFAC7C1: break;          NOTE: N/A COLOR */
        case 0xFFFCDAD5: return 1860;
     /* case 0xFFFDE3D3: break;          NOTE: N/A COLOR */
        case 0xFF636254: return 1874;
        case 0xFF394535: return 1876;
        case 0xFFE10057: return 1900;
        case 0xFFBD0041: return 1902;
        case 0xFFC00343: return 1903;
        case 0xFFA9023A: return 1904;
     /* case 0xFF960018: break;          NOTE: N/A COLOR */
        case 0xFFBE004F: return 1906;
        case 0xFF910230: return 1911;
        case 0xFF86023E: return 1912;
        case 0xFF9A0C3B: return 1913;
     /* case 0xFFA41F39: break;          NOTE: N/A COLOR */
        case 0xFFA33050: return 1921;
        case 0xFFF28DA6: return 1940;
        case 0xFFCE427A: return 1950;
     /* case 0xFFF2B9BE: break;          NOTE: N/A COLOR */
        case 0xFF959595: return 1972;
        case 0xFFA33145: return 2011;
        case 0xFF9F454C: return 2022;
        case 0xFFC7979B: return 2051;
     /* case 0xFFD18D89: break;          NOTE: N/A COLOR */
     /* case 0xFF970038: break;          NOTE: N/A COLOR */
        case 0xFF9F003F: return 2101;
        case 0xFF78093F: return 2113;
        case 0xFF6D0627: return 2115;
        case 0xFF432732: return 2123;
        case 0xFFE6778B: return 2152;
        case 0xFFDF8390: return 2153;
        case 0xFFF9BFC0: return 2155;
        case 0xFFFBD1D6: return 2160;
     /* case 0xFFFDE3DB: break;          NOTE: N/A COLOR */
        case 0xFFD8D5D0: return 2166;
     /* case 0xFFFEEDE2: break;          NOTE: N/A COLOR */
        case 0xFFF7DED6: return 2170;
        case 0xFFF7DEDE: return 2171;
     /* case 0xFFFCD9C4: break;          NOTE: N/A COLOR */
        case 0xFFE8418C: return 2220;
        case 0xFF8C0C4A: return 2222;
        case 0xFF883A40: return 2224;
        case 0xFFEE71A1: return 2230;
        case 0xFFA95A68: return 2241;
        case 0xFFFAC8D3: return 2250;
        case 0xFFD3007E: return 2300;
     /* case 0xFFBF006A: break;          NOTE: N/A COLOR */
        case 0xFFD20067: return 2320;
     /* case 0xFF780C38: break;          NOTE: N/A COLOR */
        case 0xFF651533: return 2333;
        case 0xFF3A212B: return 2336;
     /* case 0xFFF2E0DC: break;          NOTE: N/A COLOR */
        case 0xFFFDE5EC: return 2363;
        case 0xFF970059: return 2500;
     /* case 0xFF8B1771: break;          NOTE: N/A COLOR */
        case 0xFFAA4381: return 2504;
     /* case 0xFFB40073: break;          NOTE: N/A COLOR */
        case 0xFF820052: return 2506;
     /* case 0xFFD63C81: break;          NOTE: N/A COLOR */
        case 0xFFE20078: return 2520;
        case 0xFFBF006A: return 2521;
     /* case 0xFFEE71A1: break;          NOTE: N/A COLOR */
        case 0xFFF189AF: return 2550;
     /* case 0xFFF7B4CA: break;          NOTE: N/A COLOR */
        case 0xFFF7B4CA: return 2560;
        case 0xFF494949: return 2576;
     /* case 0xFF394248: break;          NOTE: N/A COLOR */
        case 0xFF893480: return 2600;
        case 0xFF6C0051: return 2611;
     /* case 0xFFCD73A6: break;          NOTE: N/A COLOR */
        case 0xFFD994B9: return 2640;
     /* case 0xFFDDBDD5: break;          NOTE: N/A COLOR */
        case 0xFFE6B7CF: return 2650;
        case 0xFFECD2DE: return 2655;
        case 0xFF606D8C: return 2674;
     /* case 0xFF646A6E: break;          NOTE: N/A COLOR */
        case 0xFF610051: return 2711;
     /* case 0xFF704191: break;          NOTE: N/A COLOR */
        case 0xFF490251: return 2715;
        case 0xFF89347F: return 2720;
     /* case 0xFF2F206F: break;          NOTE: N/A COLOR */
        case 0xFFC690A1: return 2764;
        case 0xFF6F067B: return 2810;
     /* case 0xFFAD85B1: break;          NOTE: N/A COLOR */
        case 0xFFA974AB: return 2830;
        case 0xFF4C0F7B: return 2900;
        case 0xFF664090: return 2905;
        case 0xFF83589D: return 2910;
        case 0xFF8C6DAA: return 2920;
        case 0xFFC9B5D4: return 3040;
        case 0xFFC790BA: return 3045;
        case 0xFF707070: return 3062;
        case 0xFF2A377E: return 3102;
     /* case 0xFF3C1F81: break;          NOTE: N/A COLOR */
        case 0xFF35247A: return 3110;
        case 0xFF260751: return 3114;
     /* case 0xFF28135B: break;          NOTE: N/A COLOR */
     /* case 0xFF6E5DA3: break;          NOTE: N/A COLOR */
        case 0xFF353A90: return 3210;
        case 0xFF524A90: return 3211;
     /* case 0xFF785FA3: break;          NOTE: N/A COLOR */
     /* case 0xFF241757: break;          NOTE: N/A COLOR */
        case 0xFF7D77AF: return 3241;
        case 0xFF9083A3: return 3251;
     /* case 0xFFB2AABD: break;          NOTE: N/A COLOR */
     /* case 0xFF392D88: break;          NOTE: N/A COLOR */
     /* case 0xFF5661A8: break;          NOTE: N/A COLOR */
     /* case 0xFF323887: break;          NOTE: N/A COLOR */
        case 0xFF14214E: return 3323;
     /* case 0xFF3A2885: break;          NOTE: N/A COLOR */
        case 0xFF7F8BC2: return 3331;
        case 0xFF1B3C78: return 3333;
        case 0xFF2E4B9B: return 3335;
     /* case 0xFFB9BDD9: break;          NOTE: N/A COLOR */
        case 0xFF11263C: return 3344;
        case 0xFF202A65: return 3353;
        case 0xFF171B4A: return 3355;
     /* case 0xFF959ACA: break;          NOTE: N/A COLOR */
     /* case 0xFF6A76B5: break;          NOTE: N/A COLOR */
     /* case 0xFF11263C: break;          NOTE: N/A COLOR */
        case 0xFF002232: return 3444;
        case 0xFF2D4491: return 3522;
        case 0xFF261257: return 3536;
     /* case 0xFF53428A: break;          NOTE: N/A COLOR */
        case 0xFF3A2885: return 3541;
        case 0xFF233B7D: return 3543;
        case 0xFF273C82: return 3544;
        case 0xFF272651: return 3554;
        case 0xFF28438C: return 3600;
        case 0xFF243A7D: return 3611;
        case 0xFF4055A1: return 3612;
        case 0xFF1A4C8D: return 3622;
     /* case 0xFF1E569F: break;          NOTE: N/A COLOR */
        case 0xFF92B1DC: return 3640;
        case 0xFF648DC7: return 3641;
        case 0xFFD0DEEE: return 3650;
        case 0xFFC8D6ED: return 3652;
     /* case 0xFFEAF0F9: break;          NOTE: N/A COLOR */
        case 0xFF00507F: return 3732;
        case 0xFF12253C: return 3743;
        case 0xFFB7D1E3: return 3750;
     /* case 0xFFD0DEEE: break;          NOTE: N/A COLOR */
        case 0xFFAFC9E5: return 3761;
        case 0xFFCED2D1: return 3770;
        case 0xFF3D6AA1: return 3810;
        case 0xFF7BA2D3: return 3815;
        case 0xFF91B9E2: return 3820;
     /* case 0xFF00779E: break;          NOTE: N/A COLOR */
        case 0xFFB4CEEB: return 3840;
        case 0xFF507193: return 3842;
     /* case 0xFFD5E3F4: break;          NOTE: N/A COLOR */
     /* case 0xFF9AB8D3: break;          NOTE: N/A COLOR */
        case 0xFF007EBA: return 3900;
        case 0xFF0082C4: return 3901;
        case 0xFF006CA5: return 3902;
        case 0xFF4ABDF0: return 3910;
        case 0xFF86AACA: return 3951;
     /* case 0xFF485E8A: break;          NOTE: N/A COLOR */
        case 0xFF697698: return 3953;
     /* case 0xFFC5E1F3: break;          NOTE: N/A COLOR */
        case 0xFFA6D8F6: return 3962;
        case 0xFFE1E1E1: return 3971;
        case 0xFF0093B9: return 4010;
     /* case 0xFF006587: break;          NOTE: N/A COLOR */
     /* case 0xFF87C7EA: break;          NOTE: N/A COLOR */
        case 0xFF507793: return 4032;
        case 0xFF265674: return 4033;
     /* case 0xFF9ED4E6: break;          NOTE: N/A COLOR */
        case 0xFFEAF0F9: return 4071;
        case 0xFF838689: return 4073;
     /* case 0xFF0096C1: break;          NOTE: N/A COLOR */
        case 0xFF2DB0CF: return 4101;
        case 0xFF0095C6: return 4103;
     /* case 0xFF0081AA: break;          NOTE: N/A COLOR */
        case 0xFF00A4D9: return 4111;
        case 0xFF00A9C9: return 4113;
        case 0xFF0082AD: return 4116;
     /* case 0xFF5DBFD2: break;          NOTE: N/A COLOR */
        case 0xFF00405D: return 4133;
     /* case 0xFF192024: return 4174;    TODO: duplicate case value */
     /* case 0xFF192024: break;          NOTE: N/A COLOR */
        case 0xFF4FB4CB: return 4220;
        case 0xFF8DCEE4: return 4230;
     /* case 0xFF2DB0CF: break;          NOTE: N/A COLOR */
     /* case 0xFF006587: break;          NOTE: N/A COLOR */
        case 0xFF8DCDDB: return 4240;
        case 0xFFD5EBF2: return 4250;
     /* case 0xFF007389: break;          NOTE: N/A COLOR */
        case 0xFF007B8D: return 4410;
     /* case 0xFF00B2CA: break;          NOTE: N/A COLOR */
        case 0xFF0091A5: return 4421;
        case 0xFF007D8C: return 4423;
        case 0xFF007986: return 4425;
        case 0xFF5FBFD1: return 4430;
     /* case 0xFF004250: break;          NOTE: N/A COLOR */
     /* case 0xFF8DCEE4: break;          NOTE: N/A COLOR */
        case 0xFF006981: return 4442;
        case 0xFF007F92: return 4452;
     /* case 0xFF008192: break;          NOTE: N/A COLOR */
     /* case 0xFF007079: break;          NOTE: N/A COLOR */
        case 0xFF002F38: return 4515;
     /* case 0xFF00646A: break;          NOTE: N/A COLOR */
        case 0xFF007389: return 4531;
     /* case 0xFF007B8D: return 4610;    TODO: duplicate case value */
        case 0xFF00A3A0: return 4620;
        case 0xFF0B7F85: return 4625;
        case 0xFF005B63: return 4643;
        case 0xFF234544: return 4644;
     /* case 0xFF8CCDD3: break;          NOTE: N/A COLOR */
     /* case 0xFF006F73: break;          NOTE: N/A COLOR */
     /* case 0xFF005B63: return 5005;    TODO: duplicate case value */
        case 0xFF00A6AD: return 5010;
     /* case 0xFF49BAC0: break;          NOTE: N/A COLOR */
     /* case 0xFFCFDDE0: break;          NOTE: N/A COLOR */
        case 0xFFB4DCD8: return 5050;
        case 0xFF00876E: return 5100;
        case 0xFF009084: return 5101;
     /* case 0xFF00B1AE: break;          NOTE: N/A COLOR */
        case 0xFF48BAB7: return 5115;
        case 0xFF00AF8C: return 5210;
        case 0xFF8CCCC2: return 5220;
        case 0xFF47B9AE: return 5230;
        case 0xFF197E6D: return 5233;
     /* case 0xFF8CCCC2: break;          NOTE: N/A COLOR */
     /* case 0xFF005B63: break;          NOTE: N/A COLOR */
        case 0xFF006E42: return 5324;
        case 0xFF004D3D: return 5326;
     /* case 0xFF002F38: return 5335;    TODO: duplicate case value */
        case 0xFF002D1F: return 5374;
     /* case 0xFF002F38: break;          NOTE: N/A COLOR */
        case 0xFF008663: return 5411;
        case 0xFF006B4E: return 5415;
     /* case 0xFF008663: return 5422;    TODO: duplicate case value */
     /* case 0xFF006B56: break;          NOTE: N/A COLOR */
     /* case 0xFF008879: break;          NOTE: N/A COLOR */
     /* case 0xFFDBE9E5: break;          NOTE: N/A COLOR */
     /* case 0xFF6AC093: break;          NOTE: N/A COLOR */
        case 0xFF52BA84: return 5500;
        case 0xFF14A363: return 5510;
        case 0xFF007848: return 5513;
     /* case 0xFF008663: return 5515;    TODO: duplicate case value */
        case 0xFF52A04F: return 5531;
     /* case 0xFF6EA293: break;          NOTE: N/A COLOR */
        case 0xFF94ADA1: return 5552;
        case 0xFF103828: return 5555;
     /* case 0xFF63BE5F: break;          NOTE: N/A COLOR */
        case 0xFF85C875: return 5610;
     /* case 0xFF2CB431: break;          NOTE: N/A COLOR */
        case 0xFF14B26D: return 5613;
     /* case 0xFF40B780: break;          NOTE: N/A COLOR */
        case 0xFF1A781E: return 5633;
        case 0xFF157436: return 5643;
        case 0xFFC9E3C5: return 5650;
        case 0xFF6B9181: return 5664;
     /* case 0xFF3A6D57: break;          NOTE: N/A COLOR */
     /* case 0xFF103828: break;          NOTE: N/A COLOR */
     /* case 0xFF02140C: break;          NOTE: N/A COLOR */
        case 0xFFA5C278: return 5822;
     /* case 0xFFB4D383: break;          NOTE: N/A COLOR */
        case 0xFF70953F: return 5833;
     /* case 0xFFA2D289: break;          NOTE: N/A COLOR */
        case 0xFF273014: return 5866;
        case 0xFF81C750: return 5912;
        case 0xFF457021: return 5933;
        case 0xFF506702: return 5934;
        case 0xFFBBDB41: return 5940;
        case 0xFF003518: return 5944;
        case 0xFFE3EB00: return 6010;
        case 0xFFBED782: return 6051;
     /* case 0xFF2D3B01: break;          NOTE: N/A COLOR */
     /* case 0xFFDCDDD1: break;          NOTE: N/A COLOR */
        case 0xFF919600: return 6133;
        case 0xFF484601: return 6156;
    }

    return -1;
}

const char* getName_Isacord_Polyester(unsigned int color) { return ""; } /*TODO: finish */

int getNum_Isafil_Rayon(unsigned int color)
{
    switch(color)
    {
        case 0xFFFFFFFF: return   10;
     /* case 0xFFFFFFFF: return   15;    TODO: duplicate case value */
     /* case 0xFFFFFFFF: return   17;    TODO: duplicate case value */
        case 0xFF000000: return   20;
        case 0xFFFFFDED: return  101;
        case 0xFF7D7D7D: return  104;
        case 0xFF515B61: return  107;
        case 0xFF6D757B: return  108;
        case 0xFFACACAC: return  109;
     /* case 0xFF515B61: return  111;    TODO: duplicate case value */
        case 0xFF5D5D5D: return  112;
        case 0xFFCFCFCF: return  124;
        case 0xFFA1A9B4: return  131;
     /* case 0xFF192024: break;          NOTE: N/A COLOR */
     /* case 0xFF6D757B: return  141;    TODO: duplicate case value */
        case 0xFF9EA5AA: return  142;
        case 0xFFCFD1D5: return  145;
        case 0xFFC6BDB4: return  150;
        case 0xFFD5C4B3: return  151;
        case 0xFF7C8283: return  152;
        case 0xFF898F94: return  156;
        case 0xFFFEF5F0: return  180;
        case 0xFFE9D7D9: return  182;
        case 0xFFEBE3DD: return  184;
        case 0xFFE0DA5F: return  221;
        case 0xFFBFBA28: return  232;
        case 0xFFECE9C1: return  241;
        case 0xFFFAF6CC: return  250;
        case 0xFFECE7A5: return  251;
        case 0xFFECEADB: return  260;
        case 0xFFF9F8E8: return  270;
        case 0xFFFDF76C: return  310;
        case 0xFFF5D300: return  311;
        case 0xFF797E24: return  345;
        case 0xFFB0AA76: return  352;
        case 0xFF898F2B: return  442;
        case 0xFF98996D: return  453;
        case 0xFF6E772E: return  454;
        case 0xFF6B7E6F: return  463;
        case 0xFF3E4F34: return  465;
        case 0xFFEDEF05: return  501;
     /* case 0xFFFAF6CC: return  505;    TODO: duplicate case value */
     /* case 0xFFF5D300: return  506;    TODO: duplicate case value */
        case 0xFFFFFBD1: return  510;
        case 0xFFFDE896: return  520;
        case 0xFFD7CE8A: return  532;
        case 0xFFB18B00: return  542;
        case 0xFFAA8D00: return  545;
        case 0xFFB28F11: return  546;
        case 0xFFAC9436: return  551;
        case 0xFFB69F56: return  552;
        case 0xFFF4EE8C: return  580;
        case 0xFFF1EB35: return  590;
        case 0xFFF8D73E: return  600;
     /* case 0xFFF8D73E: return  605;    TODO: duplicate case value */
     /* case 0xFFF7DC00: break;          NOTE: N/A COLOR */
        case 0xFFF7CB47: return  610;
        case 0xFFF7E400: return  615;
     /* case 0xFFFDE896: return  620;    TODO: duplicate case value */
        case 0xFFEEDB00: return  625;
        case 0xFFFEF09A: return  630;
        case 0xFFFDE1AF: return  635;
     /* case 0xFFFDE896: return  640;    TODO: duplicate case value */
        case 0xFFF5D2A6: return  651;
        case 0xFFFEF9EA: return  660;
        case 0xFFFAF6E7: return  670;
        case 0xFFBEBEA8: return  672;
        case 0xFFF7C35F: return  700;
        case 0xFFF5CA00: return  702;
        case 0xFFDFA200: return  704;
        case 0xFFFCF538: return  706;
        case 0xFFFADC59: return  713;
        case 0xFF8C7E6A: return  722;
        case 0xFF827000: return  726;
        case 0xFF636254: return  732;
        case 0xFF594900: return  747;
        case 0xFFD6BF94: return  761;
        case 0xFF656452: return  776;
        case 0xFFF1AF00: return  800;
        case 0xFFF3C200: return  805;
        case 0xFFF5BA5D: return  811;
        case 0xFFE1A23E: return  821;
        case 0xFFCCAB3F: return  822;
     /* case 0xFFDFA200: return  824;    TODO: duplicate case value */
        case 0xFFF3B044: return  830;
        case 0xFFD0A44F: return  832;
        case 0xFFCD944A: return  842;
        case 0xFFE3BC61: return  851;
        case 0xFF947C4A: return  853;
     /* case 0xFFCBBFA2: break;          NOTE: N/A COLOR */
     /* case 0xFFA5866A: break;          NOTE: N/A COLOR */
        case 0xFF001F48: return  866;
        case 0xFFEBE7DD: return  870;
        case 0xFF9FA086: return  873;
        case 0xFF9A897B: return  874;
        case 0xFFEE9C00: return  900;
        case 0xFFF3B259: return  904;
        case 0xFFCA832C: return  922;
        case 0xFFC07314: return  931;
        case 0xFFAC6613: return  932;
        case 0xFF744808: return  933;
        case 0xFFBD9565: return  934;
        case 0xFF806800: return  936;
        case 0xFFC98300: return  940;
        case 0xFFAF7D3E: return  941;
        case 0xFF483928: return  945;
        case 0xFFFEECD9: return  961;
        case 0xFFFEFEED: return  970;
        case 0xFFDD973A: return 1041;
        case 0xFF6A4129: return 1055;
        case 0xFFFDE2C1: return 1060;
        case 0xFFA68A68: return 1061;
        case 0xFFD76814: return 1099;
        case 0xFFED873F: return 1100;
        case 0xFFEC870E: return 1101;
        case 0xFFED9206: return 1102;
     /* case 0xFFEE9C00: return 1106;    TODO: duplicate case value */
        case 0xFFC45331: return 1113;
        case 0xFFEE8751: return 1114;
        case 0xFFA35214: return 1115;
        case 0xFFF8C000: return 1120;
        case 0xFFB7976B: return 1123;
        case 0xFF9D5A21: return 1134;
        case 0xFFF3D8A8: return 1140;
        case 0xFFFACFAE: return 1141;
     /* case 0xFF7A4427: break;          NOTE: N/A COLOR */
        case 0xFFDFC8AB: return 1172;
        case 0xFFE89763: return 1211;
        case 0xFFF1A236: return 1220;
        case 0xFF3D2723: return 1276;
        case 0xFFE5571D: return 1300;
     /* case 0xFFD9674C: break;          NOTE: N/A COLOR */
        case 0xFFE8643C: return 1302;
        case 0xFFE4501E: return 1304;
        case 0xFFEA7134: return 1305;
        case 0xFFE12A23: return 1306;
        case 0xFFC14817: return 1311;
     /* case 0xFFC45331: return 1312;    TODO: duplicate case value */
        case 0xFFD7623E: return 1313;
        case 0xFFED7C56: return 1320;
        case 0xFF92291B: return 1324;
        case 0xFFD5815E: return 1332;
        case 0xFFBB3D2E: return 1334;
        case 0xFFBE2D1A: return 1335;
        case 0xFF5F1B23: return 1342;
        case 0xFF7A3441: return 1346;
        case 0xFF84291D: return 1348;
        case 0xFFFBBF95: return 1351;
        case 0xFFF4A773: return 1352;
        case 0xFF693920: return 1355;
        case 0xFFF9C6A1: return 1361;
        case 0xFFF9C598: return 1362;
        case 0xFF432731: return 1366;
        case 0xFF464537: return 1375;
        case 0xFF4D2E18: return 1376;
        case 0xFFD64F42: return 1421;
        case 0xFFF4A782: return 1430;
        case 0xFFE22D2A: return 1501;
        case 0xFFA93121: return 1514;
        case 0xFFEC7168: return 1521;
        case 0xFFF6B08E: return 1532;
        case 0xFFF9C5B9: return 1551;
        case 0xFF806A61: return 1565;
     /* case 0xFF464537: return 1573;    TODO: duplicate case value */
        case 0xFFE36C63: return 1600;
        case 0xFFF9C7B9: return 1630;
        case 0xFFE44733: return 1701;
        case 0xFFDF0032: return 1703;
     /* case 0xFFE0003D: break;          NOTE: N/A COLOR */
     /* case 0xFFE44733: return 1705;    TODO: duplicate case value */
        case 0xFFCF0040: return 1725;
        case 0xFFDB686B: return 1750;
        case 0xFFF1CDCE: return 1755;
        case 0xFFE9C9BD: return 1760;
        case 0xFFE8C0B8: return 1761;
        case 0xFFE00046: return 1800;
        case 0xFFE43449: return 1802;
        case 0xFFD6445D: return 1805;
        case 0xFFF49E95: return 1840;
        case 0xFFB76663: return 1842;
     /* case 0xFFE36C63: return 1849;    TODO: duplicate case value */
        case 0xFFF0887D: return 1850;
        case 0xFFFAC7C1: return 1855;
        case 0xFFFCDAD5: return 1860;
        case 0xFFFDE3D3: return 1870;
     /* case 0xFF636254: return 1874;    TODO: duplicate case value */
        case 0xFF394535: return 1876;
        case 0xFFE10057: return 1900;
        case 0xFFBD0041: return 1902;
        case 0xFFC00343: return 1903;
        case 0xFFA9023A: return 1904;
        case 0xFF960018: return 1905;
        case 0xFFBE004F: return 1906;
        case 0xFF910230: return 1911;
        case 0xFF86023E: return 1912;
        case 0xFF9A0C3B: return 1913;
        case 0xFFA41F39: return 1914;
        case 0xFFA33050: return 1921;
        case 0xFFF28DA6: return 1940;
        case 0xFFCE427A: return 1950;
        case 0xFFF2B9BE: return 1960;
        case 0xFF959595: return 1972;
        case 0xFFA33145: return 2011;
        case 0xFF9F454C: return 2022;
        case 0xFFC7979B: return 2051;
        case 0xFFD18D89: return 2053;
        case 0xFF970038: return 2100;
        case 0xFF9F003F: return 2101;
        case 0xFF78093F: return 2113;
     /* case 0xFF6D0627: break;          NOTE: N/A COLOR */
        case 0xFF432732: return 2123;
        case 0xFFE6778B: return 2152;
        case 0xFFDF8390: return 2153;
        case 0xFFF9BFC0: return 2155;
        case 0xFFFBD1D6: return 2160;
        case 0xFFFDE3DB: return 2165;
        case 0xFFD8D5D0: return 2166;
        case 0xFFFEEDE2: return 2168;
        case 0xFFF7DED6: return 2170;
        case 0xFFF7DEDE: return 2171;
        case 0xFFFCD9C4: return 2180;
        case 0xFFE8418C: return 2220;
        case 0xFF8C0C4A: return 2222;
        case 0xFF883A40: return 2224;
        case 0xFFEE71A1: return 2230;
        case 0xFFA95A68: return 2241;
        case 0xFFFAC8D3: return 2250;
        case 0xFFD3007E: return 2300;
        case 0xFFBF006A: return 2310;
        case 0xFFD20067: return 2320;
        case 0xFF780C38: return 2332;
        case 0xFF651533: return 2333;
        case 0xFF3A212B: return 2336;
        case 0xFFF2E0DC: return 2360;
        case 0xFFFDE5EC: return 2363;
        case 0xFF970059: return 2500;
        case 0xFF8B1771: return 2502;
        case 0xFFAA4381: return 2504;
        case 0xFFB40073: return 2505;
        case 0xFF820052: return 2506;
        case 0xFFD63C81: return 2513;
        case 0xFFE20078: return 2520;
     /* case 0xFFBF006A: return 2521;    TODO: duplicate case value */
     /* case 0xFFEE71A1: return 2524;    TODO: duplicate case value */
        case 0xFFF189AF: return 2550;
        case 0xFFF7B4CA: return 2555;
     /* case 0xFFF7B4CA: return 2560;    TODO: duplicate case value */
        case 0xFF494949: return 2576;
        case 0xFF394248: return 2578;
        case 0xFF893480: return 2600;
        case 0xFF6C0051: return 2611;
        case 0xFFCD73A6: return 2620;
        case 0xFFD994B9: return 2640;
        case 0xFFDDBDD5: return 2645;
        case 0xFFE6B7CF: return 2650;
        case 0xFFECD2DE: return 2655;
        case 0xFF606D8C: return 2674;
        case 0xFF646A6E: return 2675;
        case 0xFF610051: return 2711;
        case 0xFF704191: return 2712;
        case 0xFF490251: return 2715;
     /* case 0xFF89347F: break;          NOTE: N/A COLOR */
        case 0xFF2F206F: return 2743;
        case 0xFFC690A1: return 2764;
        case 0xFF6F067B: return 2810;
        case 0xFFAD85B1: return 2820;
        case 0xFFA974AB: return 2830;
        case 0xFF4C0F7B: return 2900;
        case 0xFF664090: return 2905;
        case 0xFF83589D: return 2910;
        case 0xFF8C6DAA: return 2920;
        case 0xFFC9B5D4: return 3040;
        case 0xFFC790BA: return 3045;
        case 0xFF707070: return 3062;
        case 0xFF2A377E: return 3102;
        case 0xFF3C1F81: return 3103;
        case 0xFF35247A: return 3110;
        case 0xFF260751: return 3114;
        case 0xFF28135B: return 3133;
        case 0xFF6E5DA3: return 3200;
        case 0xFF353A90: return 3210;
        case 0xFF524A90: return 3211;
        case 0xFF785FA3: return 3213;
        case 0xFF241757: return 3222;
        case 0xFF7D77AF: return 3241;
        case 0xFF9083A3: return 3251;
        case 0xFFB2AABD: return 3262;
        case 0xFF392D88: return 3301;
        case 0xFF5661A8: return 3321;
        case 0xFF323887: return 3322;
        case 0xFF14214E: return 3323;
        case 0xFF3A2885: return 3330;
        case 0xFF7F8BC2: return 3331;
        case 0xFF1B3C78: return 3333;
     /* case 0xFF2E4B9B: break;          NOTE: N/A COLOR */
        case 0xFFB9BDD9: return 3340;
        case 0xFF11263C: return 3344;
        case 0xFF202A65: return 3353;
        case 0xFF171B4A: return 3355;
        case 0xFF959ACA: return 3420;
        case 0xFF6A76B5: return 3430;
     /* case 0xFF11263C: return 3443;    TODO: duplicate case value */
        case 0xFF002232: return 3444;
        case 0xFF2D4491: return 3522;
        case 0xFF261257: return 3536;
        case 0xFF53428A: return 3540;
     /* case 0xFF3A2885: return 3541;    TODO: duplicate case value */
        case 0xFF233B7D: return 3543;
        case 0xFF273C82: return 3544;
        case 0xFF272651: return 3554;
        case 0xFF28438C: return 3600;
        case 0xFF243A7D: return 3611;
        case 0xFF4055A1: return 3612;
        case 0xFF1A4C8D: return 3622;
        case 0xFF1E569F: return 3631;
        case 0xFF92B1DC: return 3640;
        case 0xFF648DC7: return 3641;
        case 0xFFD0DEEE: return 3650;
     /* case 0xFFC8D6ED: break;          NOTE: N/A COLOR */
        case 0xFFEAF0F9: return 3661;
        case 0xFF00507F: return 3732;
        case 0xFF12253C: return 3743;
        case 0xFFB7D1E3: return 3750;
     /* case 0xFFD0DEEE: return 3752;    TODO: duplicate case value */
        case 0xFFAFC9E5: return 3761;
        case 0xFFCED2D1: return 3770;
        case 0xFF3D6AA1: return 3810;
     /* case 0xFF7BA2D3: break;          NOTE: N/A COLOR */
        case 0xFF91B9E2: return 3820;
        case 0xFF00779E: return 3822;
        case 0xFFB4CEEB: return 3840;
        case 0xFF507193: return 3842;
        case 0xFFD5E3F4: return 3845;
        case 0xFF9AB8D3: return 3851;
        case 0xFF007EBA: return 3900;
        case 0xFF0082C4: return 3901;
        case 0xFF006CA5: return 3902;
        case 0xFF4ABDF0: return 3910;
        case 0xFF86AACA: return 3951;
        case 0xFF485E8A: return 3952;
        case 0xFF697698: return 3953;
        case 0xFFC5E1F3: return 3961;
        case 0xFFA6D8F6: return 3962;
        case 0xFFE1E1E1: return 3971;
        case 0xFF0093B9: return 4010;
        case 0xFF006587: return 4022;
        case 0xFF87C7EA: return 4030;
        case 0xFF507793: return 4032;
        case 0xFF265674: return 4033;
        case 0xFF9ED4E6: return 4040;
     /* case 0xFFEAF0F9: return 4071;    TODO: duplicate case value */
     /* case 0xFF838689: break;          NOTE: N/A COLOR */
        case 0xFF0096C1: return 4100;
        case 0xFF2DB0CF: return 4101;
        case 0xFF0095C6: return 4103;
        case 0xFF0081AA: return 4105;
        case 0xFF00A4D9: return 4111;
        case 0xFF00A9C9: return 4113;
     /* case 0xFF0082AD: break;          NOTE: N/A COLOR */
        case 0xFF5DBFD2: return 4121;
        case 0xFF00405D: return 4133;
     /* case 0xFF192024: return 4174;    TODO: duplicate case value */
     /* case 0xFF192024: return 4175;    TODO: duplicate case value */
        case 0xFF4FB4CB: return 4220;
        case 0xFF8DCEE4: return 4230;
     /* case 0xFF2DB0CF: return 4231;    TODO: duplicate case value */
     /* case 0xFF006587: return 4232;    TODO: duplicate case value */
        case 0xFF8DCDDB: return 4240;
        case 0xFFD5EBF2: return 4250;
        case 0xFF007389: return 4400;
        case 0xFF007B8D: return 4410;
        case 0xFF00B2CA: return 4420;
        case 0xFF0091A5: return 4421;
        case 0xFF007D8C: return 4423;
        case 0xFF007986: return 4425;
        case 0xFF5FBFD1: return 4430;
        case 0xFF004250: return 4432;
     /* case 0xFF8DCEE4: return 4440;    TODO: duplicate case value */
        case 0xFF006981: return 4442;
        case 0xFF007F92: return 4452;
        case 0xFF008192: return 4500;
        case 0xFF007079: return 4513;
        case 0xFF002F38: return 4515;
        case 0xFF00646A: return 4516;
     /* case 0xFF007389: return 4531;    TODO: duplicate case value */
     /* case 0xFF007B8D: return 4610;    TODO: duplicate case value */
        case 0xFF00A3A0: return 4620;
        case 0xFF0B7F85: return 4625;
        case 0xFF005B63: return 4643;
        case 0xFF234544: return 4644;
        case 0xFF8CCDD3: return 4840;
        case 0xFF006F73: return 5000;
     /* case 0xFF005B63: return 5005;    TODO: duplicate case value */
        case 0xFF00A6AD: return 5010;
        case 0xFF49BAC0: return 5020;
        case 0xFFCFDDE0: return 5040;
        case 0xFFB4DCD8: return 5050;
        case 0xFF00876E: return 5100;
        case 0xFF009084: return 5101;
        case 0xFF00B1AE: return 5111;
        case 0xFF48BAB7: return 5115;
        case 0xFF00AF8C: return 5210;
        case 0xFF8CCCC2: return 5220;
        case 0xFF47B9AE: return 5230;
        case 0xFF197E6D: return 5233;
     /* case 0xFF8CCCC2: return 5240;    TODO: duplicate case value */
     /* case 0xFF005B63: return 5255;    TODO: duplicate case value */
        case 0xFF006E42: return 5324;
        case 0xFF004D3D: return 5326;
     /* case 0xFF002F38: return 5335;    TODO: duplicate case value */
        case 0xFF002D1F: return 5374;
     /* case 0xFF002F38: return 5375;    TODO: duplicate case value */
        case 0xFF008663: return 5411;
        case 0xFF006B4E: return 5415;
     /* case 0xFF008663: return 5422;    TODO: duplicate case value */
        case 0xFF006B56: return 5425;
        case 0xFF008879: return 5431;
        case 0xFFDBE9E5: return 5460;
        case 0xFF6AC093: return 5470;
        case 0xFF52BA84: return 5500;
        case 0xFF14A363: return 5510;
        case 0xFF007848: return 5513;
     /* case 0xFF008663: return 5515;    TODO: duplicate case value */
        case 0xFF52A04F: return 5531;
        case 0xFF6EA293: return 5542;
        case 0xFF94ADA1: return 5552;
        case 0xFF103828: return 5555;
        case 0xFF63BE5F: return 5600;
        case 0xFF85C875: return 5610;
        case 0xFF2CB431: return 5611;
        case 0xFF14B26D: return 5613;
        case 0xFF40B780: return 5620;
        case 0xFF1A781E: return 5633;
        case 0xFF157436: return 5643;
        case 0xFFC9E3C5: return 5650;
        case 0xFF6B9181: return 5664;
        case 0xFF3A6D57: return 5765;
     /* case 0xFF103828: return 5766;    TODO: duplicate case value */
        case 0xFF02140C: return 5776;
        case 0xFFA5C278: return 5822;
        case 0xFFB4D383: return 5832;
        case 0xFF70953F: return 5833;
        case 0xFFA2D289: return 5840;
        case 0xFF273014: return 5866;
        case 0xFF81C750: return 5912;
        case 0xFF457021: return 5933;
        case 0xFF506702: return 5934;
        case 0xFFBBDB41: return 5940;
        case 0xFF003518: return 5944;
        case 0xFFE3EB00: return 6010;
        case 0xFFBED782: return 6051;
        case 0xFF2D3B01: return 6065;
        case 0xFFDCDDD1: return 6071;
        case 0xFF919600: return 6133;
        case 0xFF484601: return 6156;
    }

    return -1;
}

const char* getName_Isafil_Rayon(unsigned int color) { return ""; } /*TODO: finish */

int getNum_Marathon_Polyester(unsigned int color) { return -1; } /*TODO: finish */
const char* getName_Marathon_Polyester(unsigned int color) { return ""; } /*TODO: finish */

int getNum_Marathon_Rayon(unsigned int color) { return -1; } /*TODO: finish */
const char* getName_Marathon_Rayon(unsigned int color) { return ""; } /*TODO: finish */

int getNum_Madeira_Polyester(unsigned int color) { return -1; } /*TODO: finish */
const char* getName_Madeira_Polyester(unsigned int color) { return ""; } /*TODO: finish */

int getNum_Madeira_Rayon(unsigned int color) { return -1; } /*TODO: finish */
const char* getName_Madeira_Rayon(unsigned int color) { return ""; } /*TODO: finish */

int getNum_Metro_Polyester(unsigned int color) { return -1; } /*TODO: finish */
const char* getName_Metro_Polyester(unsigned int color) { return ""; } /*TODO: finish */

int getNum_Pantone(unsigned int color)
{
    switch(color)
    {
     /* case 0xFFF7E214: return "Process Yellow";    NOTE: N/A COLOR */
        case 0xFFFFFF7D: return 100;
        case 0xFFFFFF36: return 101;
        case 0xFFFFFC0D: return 102;
     /* case 0xFFFCE016: return "Pantone Yellow";    NOTE: N/A COLOR */
        case 0xFFD1CB00: return 103;
        case 0xFFB3AD00: return 104;
        case 0xFF807C00: return 105;
        case 0xFFFFFA4F: return 106;
        case 0xFFFFF536: return 107;
        case 0xFFFFF00D: return 108;
        case 0xFFFFE600: return 109;
        case 0xFFEDD100: return 110;
        case 0xFFBAA600: return 111;
        case 0xFF9E8E00: return 112;
        case 0xFFFFED57: return 113;
        case 0xFFFFEB45: return 114;
        case 0xFFFFE833: return 115;
        case 0xFFFFD600: return 116;
        case 0xFFD9B200: return 117;
        case 0xFFBA9900: return 118;
        case 0xFF827200: return 119;
        case 0xFFFFE86B: return 120;
        case 0xFFFFF2B0: return 1205;
        case 0xFFFFE34F: return 121;
        case 0xFFFFE88C: return 1215;
        case 0xFFFFD433: return 122;
        case 0xFFFFD461: return 1225;
        case 0xFFFFC20F: return 123;
        case 0xFFFFB517: return 1235;
        case 0xFFF0AD00: return 124;
        case 0xFFD19700: return 1245;
        case 0xFFBD8C00: return 125;
        case 0xFFA87B00: return 1255;
        case 0xFFA17800: return 126;
        case 0xFF7D5B00: return 1265;
        case 0xFFFFED80: return 127;
        case 0xFFFFE359: return 128;
        case 0xFFFFD63B: return 129;
        case 0xFFFFB300: return 130;
        case 0xFFE89E00: return 131;
        case 0xFFB38100: return 132;
        case 0xFF705A00: return 133;
        case 0xFFFFE38C: return 134;
        case 0xFFFFDB87: return 1345;
        case 0xFFFFCF66: return 135;
        case 0xFFFFCC70: return 1355;
        case 0xFFFFBA3D: return 136;
        case 0xFFFFB547: return 1365;
        case 0xFFFFA61A: return 137;
        case 0xFFFF991A: return 1375;
        case 0xFFFC9200: return 138;
        case 0xFFED8500: return 1385;
        case 0xFFC47C00: return 139;
        case 0xFFA15F00: return 1395;
        case 0xFF755600: return 140;
        case 0xFF5E3C00: return 1405;
        case 0xFFFFCF7D: return 141;
        case 0xFFFFB83D: return 142;
        case 0xFFFFA626: return 143;
        case 0xFFFF8500: return 144;
        case 0xFFEB7C00: return 145;
        case 0xFFAB6100: return 146;
        case 0xFF705100: return 147;
        case 0xFFFFD6A1: return 148;
        case 0xFFFFBA75: return 1485;
        case 0xFFFFC487: return 149;
        case 0xFFFFAB54: return 1495;
        case 0xFFFFA64D: return 150;
        case 0xFFFF943B: return 1505;
     /* case 0xFFEF6B00: return "Orange 021";    NOTE: N/A COLOR */
        case 0xFFFF850D: return 151;
        case 0xFFFC7C00: return 152;
        case 0xFFE66000: return 1525;
        case 0xFFD17100: return 153;
        case 0xFF9E4A00: return 1535;
        case 0xFFA85B00: return 154;
        case 0xFF472200: return 1545;
        case 0xFFFFE0B8: return 155;
        case 0xFFFFC7A8: return 1555;
        case 0xFFFFC794: return 156;
        case 0xFFFFA882: return 1565;
        case 0xFFFF914D: return 157;
        case 0xFFFF8C47: return 1575;
        case 0xFFFF6308: return 158;
        case 0xFFFF701A: return 1585;
        case 0xFFED5100: return 159;
        case 0xFFF26300: return 1595;
        case 0xFFAD4200: return 160;
        case 0xFFB34F00: return 1605;
        case 0xFF5C2C00: return 161;
        case 0xFF914000: return 1615;
        case 0xFFFFD9C7: return 162;
        case 0xFFFFB0A1: return 1625;
        case 0xFFFFB08F: return 163;
        case 0xFFFF9C85: return 1635;
        case 0xFFFF8A45: return 164;
        case 0xFFFF8257: return 1645;
        case 0xFFFF690A: return 165;
        case 0xFFFF5E17: return 1655;
        case 0xFFFF5C00: return 166;
        case 0xFFFF5200: return 1665;
        case 0xFFD45500: return 167;
        case 0xFFB83D00: return 1675;
        case 0xFF692D00: return 168;
        case 0xFF8F2E00: return 1685;
        case 0xFFFFCCCC: return 169;
        case 0xFFFF998F: return 170;
        case 0xFFFF7852: return 171;
        case 0xFFFF571F: return 172;
        case 0xFFF54C00: return 173;
        case 0xFFA33100: return 174;
        case 0xFF662400: return 175;
        case 0xFFFFBFD1: return 176;
        case 0xFFFF9EC9: return 1765;
        case 0xFFFFBAE0: return 1767;
        case 0xFFFF8C99: return 177;
        case 0xFFFF87B5: return 1775;
        case 0xFFFF6BA3: return 1777;
        case 0xFFFF6970: return 178;
     /* case 0xFFF93F26: return "Warm Red";    NOTE: N/A COLOR */
        case 0xFFFF5480: return 1785;
        case 0xFFFF3D66: return 1787;
     /* case 0xFFEF2B2D: return "Red 032";    NOTE: N/A COLOR */
        case 0xFFFF291F: return 1788;
        case 0xFFFF3600: return 179;
        case 0xFFFF0F00: return 1795;
        case 0xFFF50002: return 1797;
        case 0xFFE33000: return 180;
        case 0xFFC41200: return 1805;
        case 0xFFB80007: return 1807;
        case 0xFF872300: return 181;
        case 0xFF7D0C00: return 1815;
        case 0xFF570900: return 1817;
        case 0xFFFFBDE6: return 182;
        case 0xFFFF8AC9: return 183;
        case 0xFFFF5296: return 184;
        case 0xFFFF173D: return 185;
        case 0xFFF5002F: return 186;
        case 0xFFCC002B: return 187;
        case 0xFF800400: return 188;
        case 0xFFFFA1E6: return 189;
        case 0xFFFFB8ED: return 1895;
        case 0xFFFF73C7: return 190;
        case 0xFFFF96E8: return 1905;
        case 0xFFFF3D9E: return 191;
        case 0xFFFF4ACC: return 1915;
        case 0xFFFF0052: return 192;
        case 0xFFFF0073: return 1925;
        case 0xFFDE004B: return 193;
        case 0xFFF20068: return 1935;
        case 0xFFAB003E: return 194;
        case 0xFFCF005B: return 1945;
        case 0xFF73002E: return 195;
        case 0xFFA10040: return 1955;
        case 0xFFFFBFF5: return 196;
        case 0xFFFF8CE6: return 197;
        case 0xFFFF38AB: return 198;
        case 0xFFFF0061: return 199;
        case 0xFFE00053: return 200;
        case 0xFFB50043: return 201;
        case 0xFF910039: return 202;
        case 0xFFFFA8F7: return 203;
        case 0xFFFF6BF7: return 204;
        case 0xFFFF29E8: return 205;
        case 0xFFF70099: return 206;
        case 0xFFCF0076: return 207;
        case 0xFFA10067: return 208;
        case 0xFF78004F: return 209;
        case 0xFFFF9CF0: return 210;
        case 0xFFFF73EB: return 211;
        case 0xFFFF47E3: return 212;
        case 0xFFFF0DBA: return 213;
        case 0xFFEB009B: return 214;
        case 0xFFBA0079: return 215;
        case 0xFF82074E: return 216;
        case 0xFFFFB8FF: return 217;
        case 0xFFFA63FF: return 218;
        case 0xFFFC1FFF: return 219;
     /* case 0xFFD10056: return "Rubine Red";    NOTE: N/A COLOR */
        case 0xFFD400B8: return 220;
        case 0xFFB30098: return 221;
        case 0xFF69005E: return 222;
        case 0xFFFF8AFF: return 223;
        case 0xFFFC5EFF: return 224;
        case 0xFFFC2BFF: return 225;
        case 0xFFFF00FF: return 226;
        case 0xFFCF00C0: return 227;
        case 0xFF960090: return 228;
        case 0xFF660057: return 229;
        case 0xFFFFA8FF: return 230;
        case 0xFFFC7AFF: return 231;
        case 0xFFF754FF: return 232;
     /* case 0xFFED0091: return "Rhodamine Red";    NOTE: N/A COLOR */
        case 0xFFE300FF: return 233;
        case 0xFFB100BD: return 234;
        case 0xFF910099: return 235;
        case 0xFFFCB3FF: return 236;
        case 0xFFFABAFF: return 2365;
        case 0xFFF782FF: return 237;
        case 0xFFE66EFF: return 2375;
        case 0xFFF05EFF: return 238;
        case 0xFFCF36FF: return 2385;
        case 0xFFE336FF: return 239;
        case 0xFFBA0DFF: return 2395;
        case 0xFFD10FFF: return 240;
        case 0xFFA800FF: return 2405;
        case 0xFFB600FA: return 241;
        case 0xFF9D00EB: return 2415;
        case 0xFF750082: return 242;
        case 0xFF7700BD: return 2425;
        case 0xFFF2B5FF: return 243;
        case 0xFFE89EFF: return 244;
        case 0xFFDB78FF: return 245;
        case 0xFFB51AFF: return 246;
        case 0xFFA300FF: return 247;
        case 0xFF9600FA: return 248;
        case 0xFF6E00B8: return 249;
        case 0xFFF2D1FF: return 250;
        case 0xFFDE9CFF: return 251;
        case 0xFFC270FF: return 252;
     /* case 0xFFBF30B5: return "Purple";    NOTE: N/A COLOR */
        case 0xFF910DFF: return 253;
        case 0xFF8000FF: return 254;
        case 0xFF5E00BF: return 255;
        case 0xFFEDCCFF: return 256;
        case 0xFFCFA6FF: return 2562;
        case 0xFFC7ABFF: return 2563;
        case 0xFFB5A3FF: return 2567;
        case 0xFFDBA8FF: return 257;
        case 0xFFB387FF: return 2572;
        case 0xFFB391FF: return 2573;
        case 0xFF998CFF: return 2577;
        case 0xFF913DFF: return 258;
        case 0xFF8A47FF: return 2582;
        case 0xFF8A5EFF: return 2583;
        case 0xFF6957FF: return 2587;
        case 0xFF5F00D9: return 259;
        case 0xFF661AFF: return 2592;
        case 0xFF631CFF: return 2593;
        case 0xFF2600FF: return 2597;
        case 0xFF5B00BD: return 260;
        case 0xFF5C00F7: return 2602;
        case 0xFF4D00FA: return 2603;
        case 0xFF2D00ED: return 2607;
        case 0xFF500099: return 261;
        case 0xFF4F00DB: return 2612;
        case 0xFF5000D9: return 2613;
        case 0xFF2E00D9: return 2617;
        case 0xFF3F0073: return 262;
        case 0xFF3C008F: return 2622;
        case 0xFF4700AD: return 2623;
        case 0xFF2800B0: return 2627;
        case 0xFFE6DBFF: return 263;
        case 0xFFB8BAFF: return 2635;
        case 0xFFBDB8FF: return 264;
        case 0xFF99A3FF: return 2645;
        case 0xFF7570FF: return 265;
        case 0xFF7582FF: return 2655;
        case 0xFF361AFF: return 266;
        case 0xFF6166FF: return 2665;
     /* case 0xFF6607A5: return "Violet";    NOTE: N/A COLOR */
        case 0xFF1C00FF: return 267;
        case 0xFF2800E0: return 268;
        case 0xFF0900E6: return 2685;
        case 0xFF2600AB: return 269;
        case 0xFF0C0082: return 2695;
        case 0xFFB0BAFF: return 270;
        case 0xFF99B3FF: return 2705;
        case 0xFFCFE8FF: return 2706;
        case 0xFFD4F0FF: return 2707;
        case 0xFFBDE6FF: return 2708;
        case 0xFF91A1FF: return 271;
        case 0xFF6E8CFF: return 2715;
        case 0xFF8CB5FF: return 2716;
        case 0xFFB5E0FF: return 2717;
        case 0xFF5496FF: return 2718;
        case 0xFF6B85FF: return 272;
        case 0xFF3B52FF: return 2725;
        case 0xFF3657FF: return 2726;
        case 0xFF4A94FF: return 2727;
     /* case 0xFF380096: return "Blue 072";    NOTE: N/A COLOR */
        case 0xFF0A4FFF: return 2728;
        case 0xFF0009EB: return 273;
        case 0xFF000DFF: return 2735;
        case 0xFF0017FF: return 2736;
        case 0xFF0020FA: return 2738;
        case 0xFF0000B8: return 274;
        case 0xFF000BD9: return 2745;
        case 0xFF0012E6: return 2746;
        case 0xFF001ED9: return 2747;
        case 0xFF001AD9: return 2748;
        case 0xFF030091: return 275;
        case 0xFF0005B3: return 2755;
        case 0xFF000BB5: return 2756;
        case 0xFF0020B3: return 2757;
        case 0xFF0026BD: return 2758;
        case 0xFF020073: return 276;
        case 0xFF00048C: return 2765;
        case 0xFF000887: return 2766;
        case 0xFF001A75: return 2767;
        case 0xFF001F8F: return 2768;
        case 0xFFBAEDFF: return 277;
        case 0xFF9CDBFF: return 278;
        case 0xFF52A8FF: return 279;
     /* case 0xFF0C1C8C: return "Reflex Blue";    NOTE: N/A COLOR */
        case 0xFF003BD1: return 280;
        case 0xFF0031AD: return 281;
        case 0xFF002675: return 282;
        case 0xFFA6E8FF: return 283;
        case 0xFF73CFFF: return 284;
        case 0xFF1C91FF: return 285;
        case 0xFF0055FA: return 286;
        case 0xFF0048E0: return 287;
        case 0xFF0041C4: return 288;
        case 0xFF00246B: return 289;
        case 0xFFBFFAFF: return 290;
        case 0xFF96FAFF: return 2905;
        case 0xFFABF7FF: return 291;
        case 0xFF69EDFF: return 2915;
        case 0xFF82E3FF: return 292;
        case 0xFF26C2FF: return 2925;
        case 0xFF006BFA: return 293;
        case 0xFF008AFF: return 2935;
        case 0xFF0055C9: return 294;
        case 0xFF0079DB: return 2945;
        case 0xFF0045A1: return 295;
        case 0xFF0058A1: return 2955;
        case 0xFF00294D: return 296;
        case 0xFF00395C: return 2965;
        case 0xFF82FCFF: return 297;
        case 0xFFB3FFF2: return 2975;
        case 0xFF4FEDFF: return 298;
        case 0xFF69FFF0: return 2985;
        case 0xFF26CFFF: return 299;
        case 0xFF1AE3FF: return 2995;
        case 0xFF008FFF: return 300;
        case 0xFF00A0FA: return 3005;
        case 0xFF0073D1: return 301;
        case 0xFF0089CC: return 3015;
        case 0xFF006080: return 302;
        case 0xFF00687D: return 3025;
        case 0xFF003B42: return 303;
        case 0xFF004744: return 3035;
        case 0xFFB3FFEB: return 304;
        case 0xFF7DFFE8: return 305;
        case 0xFF40FFED: return 306;
     /* case 0xFF0091C9: return "Process Blue";    NOTE: N/A COLOR */
        case 0xFF009CBA: return 307;
        case 0xFF008087: return 308;
        case 0xFF004741: return 309;
        case 0xFF91FFE6: return 310;
        case 0xFF91FFE0: return 3105;
        case 0xFF5EFFE0: return 311;
        case 0xFF5EFFD1: return 3115;
        case 0xFF0AFFE3: return 312;
        case 0xFF2BFFC9: return 3125;
        case 0xFF00DECC: return 313;
        case 0xFF00E8C3: return 3135;
        case 0xFF00B3A2: return 314;
        case 0xFF00C49F: return 3145;
        case 0xFF009180: return 315;
        case 0xFF009E78: return 3155;
        case 0xFF00523C: return 316;
        case 0xFF005940: return 3165;
        case 0xFFD1FFEB: return 317;
        case 0xFF9EFFD9: return 318;
        case 0xFF7AFFCF: return 319;
        case 0xFF00EDA4: return 320;
        case 0xFF00C487: return 321;
        case 0xFF00A66F: return 322;
        case 0xFF008754: return 323;
        case 0xFFB8FFE0: return 324;
        case 0xFFA1FFD1: return 3242;
        case 0xFFA8FFCF: return 3245;
        case 0xFF91FFC2: return 3248;
        case 0xFF70FFBD: return 325;
        case 0xFF87FFC2: return 3252;
        case 0xFF82FFB8: return 3255;
        case 0xFF69FFAB: return 3258;
        case 0xFF21FF9E: return 326;
        case 0xFF4AFFAB: return 3262;
        case 0xFF4FFFA1: return 3265;
        case 0xFF1AFF82: return 3268;
        case 0xFF00D477: return 327;
        case 0xFF00FF8F: return 3272;
        case 0xFF0DFF87: return 3275;
        case 0xFF00F26D: return 3278;
        case 0xFF00AD5F: return 328;
        case 0xFF00D975: return 3282;
        case 0xFF00ED77: return 3285;
        case 0xFF00CC5E: return 3288;
        case 0xFF008A4A: return 329;
        case 0xFF008A46: return 3292;
        case 0xFF00C95F: return 3295;
        case 0xFF009440: return 3298;
        case 0xFF006635: return 330;
        case 0xFF004F24: return 3302;
        case 0xFF006327: return 3305;
        case 0xFF00471D: return 3308;
        case 0xFFC2FFD6: return 331;
        case 0xFFB3FFCC: return 332;
        case 0xFF91FFBA: return 333;
     /* case 0xFF00AF93: return "Green";    NOTE: N/A COLOR */
        case 0xFF00F763: return 334;
        case 0xFF00B33E: return 335;
        case 0xFF00872D: return 336;
        case 0xFFB0FFCC: return 337;
        case 0xFFA6FFBF: return 3375;
        case 0xFF87FFAD: return 338;
        case 0xFF8CFFAB: return 3385;
        case 0xFF29FF70: return 339;
        case 0xFF63FF8C: return 3395;
        case 0xFF00E84F: return 340;
        case 0xFF26FF59: return 3405;
        case 0xFF00B53C: return 341;
        case 0xFF00C72E: return 3415;
        case 0xFF00912A: return 342;
        case 0xFF009421: return 3425;
        case 0xFF02631C: return 343;
        case 0xFF005710: return 3435;
        case 0xFFBAFFC4: return 344;
        case 0xFF9EFFAD: return 345;
        case 0xFF73FF87: return 346;
        case 0xFF00F723: return 347;
        case 0xFF00C21D: return 348;
        case 0xFF00940D: return 349;
        case 0xFF0D4000: return 350;
        case 0xFFD4FFD6: return 351;
        case 0xFFBAFFBF: return 352;
        case 0xFF9EFFA3: return 353;
        case 0xFF33FF1A: return 354;
        case 0xFF0FFF00: return 355;
        case 0xFF09BA00: return 356;
        case 0xFF167000: return 357;
        case 0xFFBAFF9E: return 358;
        case 0xFFA3FF82: return 359;
        case 0xFF6BFF33: return 360;
        case 0xFF4FFF00: return 361;
        case 0xFF46E800: return 362;
        case 0xFF3EC200: return 363;
        case 0xFF349400: return 364;
        case 0xFFE0FFB5: return 365;
        case 0xFFCCFF8F: return 366;
        case 0xFFADFF69: return 367;
        case 0xFF6EFF00: return 368;
        case 0xFF61ED00: return 369;
        case 0xFF52BA00: return 370;
        case 0xFF407000: return 371;
        case 0xFFE6FFAB: return 372;
        case 0xFFD6FF8A: return 373;
        case 0xFFC2FF6E: return 374;
        case 0xFF96FF38: return 375;
        case 0xFF74F200: return 376;
        case 0xFF6BC200: return 377;
        case 0xFF436600: return 378;
        case 0xFFE8FF6B: return 379;
        case 0xFFDEFF47: return 380;
        case 0xFFCCFF17: return 381;
        case 0xFFB5FF00: return 382;
        case 0xFFA5CF00: return 383;
        case 0xFF90B000: return 384;
        case 0xFF686B00: return 385;
        case 0xFFF0FF70: return 386;
        case 0xFFE6FF42: return 387;
        case 0xFFDBFF36: return 388;
        case 0xFFCCFF26: return 389;
        case 0xFFB7EB00: return 390;
        case 0xFF95AB00: return 391;
        case 0xFF798200: return 392;
        case 0xFFF7FF73: return 393;
        case 0xFFFCFF52: return 3935;
        case 0xFFF0FF3D: return 394;
        case 0xFFF7FF26: return 3945;
        case 0xFFEBFF26: return 395;
        case 0xFFF0FF00: return 3955;
        case 0xFFE3FF0F: return 396;
        case 0xFFEBFF00: return 3965;
        case 0xFFCCE300: return 397;
        case 0xFFB5B500: return 3975;
        case 0xFFABB800: return 398;
        case 0xFF969200: return 3985;
        case 0xFF919100: return 399;
        case 0xFF5C5900: return 3995;
        case 0xFFD6D0C9: return 400;
        case 0xFFC4BBAF: return 401;
        case 0xFFB0A597: return 402;
        case 0xFF918779: return 403;
        case 0xFF706758: return 404;
        case 0xFF474030: return 405;
     /* case 0xFF3D332B: return "Black";    NOTE: N/A COLOR */
        case 0xFFD6CBC9: return 406;
        case 0xFFBDAEAC: return 407;
        case 0xFFA89796: return 408;
        case 0xFF8C7A77: return 409;
        case 0xFF705C59: return 410;
        case 0xFF47342E: return 411;
        case 0xFF050402: return 412;
        case 0xFFCCCCBA: return 413;
        case 0xFFB3B3A1: return 414;
        case 0xFF969684: return 415;
        case 0xFF80806B: return 416;
        case 0xFF585943: return 417;
        case 0xFF3E402C: return 418;
        case 0xFF000000: return 419;
        case 0xFFD9D9D9: return 420;
        case 0xFFBDBDBD: return 421;
        case 0xFFABABAB: return 422;
        case 0xFF8F8F8F: return 423;
        case 0xFF636363: return 424;
        case 0xFF3B3B3B: return 425;
     /* case 0xFF000000: return 426;    TODO: duplicate case value */
        case 0xFFE3E3E3: return 427;
        case 0xFFCDD1D1: return 428;
        case 0xFFA8ADAD: return 429;
        case 0xFF858C8C: return 430;
        case 0xFF525B5C: return 431;
        case 0xFF2D393B: return 432;
        case 0xFF090C0D: return 433;
        case 0xFFEDE6E8: return 434;
        case 0xFFDED6DB: return 435;
        case 0xFFC2BFBF: return 436;
        case 0xFF8A8C8A: return 437;
        case 0xFF394500: return 438;
        case 0xFF293300: return 439;
        case 0xFF202700: return 440;
        case 0xFFDAE8D8: return 441;
        case 0xFFBECFBC: return 442;
        case 0xFF9DB39D: return 443;
        case 0xFF7E947E: return 444;
        case 0xFF475947: return 445;
        case 0xFF324031: return 446;
        case 0xFF272E20: return 447;
        /* BEGIN N/A COLORS */
        /*
        case 0xFFE5DBCC: return "Warm Gray 1";
        case 0xFFDDD1C1: return "Warm Gray 2";
        case 0xFFCCC1B2: return "Warm Gray 3";
        case 0xFFC1B5A5: return "Warm Gray 4";
        case 0xFFB5A899: return "Warm Gray 5";
        case 0xFFAFA393: return "Warm Gray 6";
        case 0xFFA39687: return "Warm Gray 7";
        case 0xFF96897A: return "Warm Gray 8";
        case 0xFF8C7F70: return "Warm Gray 9";
        case 0xFF827263: return "Warm Gray 10";
        case 0xFF6D5E51: return "Warm Gray 11";
        case 0xFFE8E2D6: return "Cool Gray 1";
        case 0xFFDDD8CE: return "Cool Gray 2";
        case 0xFFD3CEC4: return "Cool Gray 3";
        case 0xFFC4C1BA: return "Cool Gray 4";
        case 0xFFBAB7AF: return "Cool Gray 5";
        case 0xFFB5B2AA: return "Cool Gray 6";
        case 0xFFA5A39E: return "Cool Gray 7";
        case 0xFF9B9993: return "Cool Gray 8";
        case 0xFF8C8984: return "Cool Gray 9";
        case 0xFF777772: return "Cool Gray 10";
        */
        /* END N/A COLORS */
        case 0xFF2D3E00: return 448;
        case 0xFF4F3A00: return 4485;
        case 0xFF3D5200: return 449;
        case 0xFF8A6E07: return 4495;
        case 0xFF506700: return 450;
        case 0xFFA38B24: return 4505;
        case 0xFFABB573: return 451;
        case 0xFFC2B061: return 4515;
        case 0xFFC2CF9C: return 452;
        case 0xFFD4C581: return 4525;
        case 0xFFDBE3BF: return 453;
        case 0xFFE3DA9F: return 4535;
        case 0xFFE8EDD6: return 454;
        case 0xFFF0E9C2: return 4545;
        case 0xFF594A00: return 455;
        case 0xFF917C00: return 456;
        case 0xFFB89C00: return 457;
        case 0xFFE6E645: return 458;
        case 0xFFF0ED73: return 459;
        case 0xFFF5F28F: return 460;
        case 0xFFF7F7A6: return 461;
        case 0xFF402600: return 462;
        case 0xFF361500: return 4625;
        case 0xFF6B3D00: return 463;
        case 0xFF8F4A06: return 4635;
        case 0xFF955300: return 464;
        case 0xFFB8743B: return 4645;
        case 0xFFCCAD6B: return 465;
        case 0xFFD19B73: return 4655;
        case 0xFFE0C791: return 466;
        case 0xFFE6BC9C: return 4665;
        case 0xFFE8D9A8: return 467;
        case 0xFFF0D5BD: return 4675;
        case 0xFFF0E8C4: return 468;
        case 0xFFF5E4D3: return 4685;
        case 0xFF4A1A00: return 469;
        case 0xFF420D00: return 4695;
        case 0xFFAB4800: return 470;
        case 0xFF823126: return 4705;
        case 0xFFD15600: return 471;
        case 0xFFA8625D: return 4715;
        case 0xFFFFA87A: return 472;
        case 0xFFBF827C: return 4725;
        case 0xFFFFC4A3: return 473;
        case 0xFFD9A9A7: return 4735;
        case 0xFFFFD9BD: return 474;
        case 0xFFE6BEBC: return 4745;
        case 0xFFFFE3CC: return 475;
        case 0xFFF0D8D3: return 4755;
        case 0xFF381C00: return 476;
        case 0xFF4F1800: return 477;
        case 0xFF6B1200: return 478;
        case 0xFFB08573: return 479;
        case 0xFFD9B5B0: return 480;
        case 0xFFE8CFC9: return 481;
        case 0xFFF2E0DE: return 482;
        case 0xFF660700: return 483;
        case 0xFFB50900: return 484;
        case 0xFFFF0D00: return 485;
        case 0xFFFF8796: return 486;
        case 0xFFFFA6B8: return 487;
        case 0xFFFFBDCF: return 488;
        case 0xFFFFD9E3: return 489;
        case 0xFF471300: return 490;
        case 0xFF7A1A00: return 491;
        case 0xFF942200: return 492;
        case 0xFFF283BB: return 493;
        case 0xFFFFABDE: return 494;
        case 0xFFFFC2E3: return 495;
        case 0xFFFFD6E8: return 496;
        case 0xFF381100: return 497;
        case 0xFF330E00: return 4975;
        case 0xFF662500: return 498;
        case 0xFF853241: return 4985;
        case 0xFF853100: return 499;
        case 0xFFA85868: return 4995;
        case 0xFFE38DB3: return 500;
        case 0xFFC47A8F: return 5005;
        case 0xFFF7B5D7: return 501;
        case 0xFFE3AAC1: return 5015;
        case 0xFFFCCFE3: return 502;
        case 0xFFEDC2D1: return 5025;
        case 0xFFFFE3EB: return 503;
        case 0xFFF7DFE1: return 5035;
        case 0xFF320000: return 504;
        case 0xFF600000: return 505;
        case 0xFF770000: return 506;
        case 0xFFDE82C4: return 507;
        case 0xFFF2A3E3: return 508;
        case 0xFFFFC2ED: return 509;
        case 0xFFFFD4F0: return 510;
        case 0xFF3D0066: return 511;
        case 0xFF2B0041: return 5115;
        case 0xFF6100CE: return 512;
        case 0xFF592482: return 5125;
        case 0xFF8A1FFF: return 513;
        case 0xFF8257B8: return 5135;
        case 0xFFD980FF: return 514;
        case 0xFFB38CE0: return 5145;
        case 0xFFED9EFF: return 515;
        case 0xFFD4B3EB: return 5155;
        case 0xFFF7BAFF: return 516;
        case 0xFFE8CFF2: return 5165;
        case 0xFFFFD1FF: return 517;
        case 0xFFF2E0F7: return 5175;
        case 0xFF2E005C: return 518;
        case 0xFF1C0022: return 5185;
        case 0xFF44009D: return 519;
        case 0xFF3D0C4E: return 5195;
        case 0xFF5C00E0: return 520;
        case 0xFF7A5E85: return 5205;
        case 0xFFBA87FF: return 521;
        case 0xFFB59EC2: return 5215;
        case 0xFFD4A1FF: return 522;
        case 0xFFD4BAD9: return 5225;
        case 0xFFE6BDFF: return 523;
        case 0xFFE6D4E6: return 5235;
        case 0xFFF0D9FF: return 524;
        case 0xFFF0E6ED: return 5245;
        case 0xFF270085: return 525;
        case 0xFF0D0B4D: return 5255;
        case 0xFF3B00ED: return 526;
        case 0xFF20258A: return 5265;
        case 0xFF4500FF: return 527;
        case 0xFF3848A8: return 5275;
        case 0xFF9673FF: return 528;
        case 0xFF7280C4: return 5285;
        case 0xFFBD99FF: return 529;
        case 0xFFA8B3E6: return 5295;
        case 0xFFD1B0FF: return 530;
        case 0xFFC7CEED: return 5305;
        case 0xFFE6CCFF: return 531;
        case 0xFFE4E4F2: return 5315;
        case 0xFF00193F: return 532;
        case 0xFF00227B: return 533;
        case 0xFF002CAA: return 534;
        case 0xFF94B3ED: return 535;
        case 0xFFB0C7F2: return 536;
        case 0xFFC7DBF7: return 537;
        case 0xFFDEE8FA: return 538;
        case 0xFF00274D: return 539;
        case 0xFF00223D: return 5395;
        case 0xFF003473: return 540;
        case 0xFF3A728A: return 5405;
        case 0xFF00449E: return 541;
        case 0xFF5A8A96: return 5415;
        case 0xFF5EC1F7: return 542;
        case 0xFF79A6AD: return 5425;
        case 0xFF96E3FF: return 543;
        case 0xFFB8CDD4: return 5435;
        case 0xFFB3F0FF: return 544;
        case 0xFFCCDCDE: return 5445;
        case 0xFFC7F7FF: return 545;
        case 0xFFDAE8E8: return 5455;
        case 0xFF02272B: return 546;
        case 0xFF002B24: return 5463;
        case 0xFF000D09: return 5467;
        case 0xFF003440: return 547;
        case 0xFF167A58: return 5473;
        case 0xFF1D4230: return 5477;
        case 0xFF00465C: return 548;
        case 0xFF43B08B: return 5483;
        case 0xFF48705D: return 5487;
        case 0xFF56ADBA: return 549;
        case 0xFF73C9AD: return 5493;
        case 0xFF829E90: return 5497;
        case 0xFF7BC1C9: return 550;
        case 0xFF9CDBC5: return 5503;
        case 0xFFA1B5A8: return 5507;
        case 0xFFA2D7DE: return 551;
        case 0xFFC7F2E1: return 5513;
        case 0xFFBED1C5: return 5517;
        case 0xFFC5E8E8: return 552;
        case 0xFFDCF7EB: return 5523;
        case 0xFFD5E3DA: return 5527;
        case 0xFF143319: return 553;
        case 0xFF102E14: return 5535;
        case 0xFF115422: return 554;
        case 0xFF327A3D: return 5545;
        case 0xFF187031: return 555;
        case 0xFF5A9E68: return 5555;
        case 0xFF66BA80: return 556;
        case 0xFF84BD8F: return 5565;
        case 0xFF98D9AD: return 557;
        case 0xFFA9D4B2: return 5575;
        case 0xFFBAE8CA: return 558;
        case 0xFFCAE6CC: return 5585;
        case 0xFFCEF0D8: return 559;
        case 0xFFDDEDDA: return 5595;
        case 0xFF0D4018: return 560;
        case 0xFF050F07: return 5605;
        case 0xFF127A38: return 561;
        case 0xFF2E522B: return 5615;
        case 0xFF1AB058: return 562;
        case 0xFF5A7D57: return 5625;
        case 0xFF79FCAC: return 563;
        case 0xFF89A386: return 5635;
        case 0xFFA1FFCC: return 564;
        case 0xFFAEBFA6: return 5645;
        case 0xFFC4FFDE: return 565;
        case 0xFFC5D1BE: return 5655;
        case 0xFFDBFFE8: return 566;
        case 0xFFDAE6D5: return 5665;
        case 0xFF0E4D1C: return 567;
        case 0xFF14A346: return 568;
        case 0xFF04D45B: return 569;
        case 0xFF85FFB5: return 570;
        case 0xFFADFFCF: return 571;
        case 0xFFC4FFDB: return 572;
     /* case 0xFFDBFFE8: return 573;    TODO: duplicate case value */
        case 0xFF314A0E: return 574;
        case 0xFF1F2E07: return 5743;
        case 0xFF243600: return 5747;
        case 0xFF3E7800: return 575;
        case 0xFF3F5410: return 5753;
        case 0xFF547306: return 5757;
        case 0xFF4F9C00: return 576;
        case 0xFF5C6E1D: return 5763;
        case 0xFF849C32: return 5767;
        case 0xFFAEE67C: return 577;
        case 0xFF909E5A: return 5773;
        case 0xFFA5B85E: return 5777;
        case 0xFFC0F090: return 578;
        case 0xFFAFBA86: return 5783;
        case 0xFFCEDE99: return 5787;
        case 0xFFCDF7A3: return 579;
        case 0xFFC9D1A5: return 5793;
        case 0xFFDCE8B0: return 5797;
        case 0xFFDCFAB9: return 580;
        case 0xFFDEE3C8: return 5803;
        case 0xFFE9F0CE: return 5807;
        case 0xFF464700: return 581;
        case 0xFF363605: return 5815;
        case 0xFF788A00: return 582;
        case 0xFF69660E: return 5825;
        case 0xFFA3D400: return 583;
        case 0xFF999632: return 5835;
        case 0xFFD3F032: return 584;
        case 0xFFB3B15F: return 5845;
        case 0xFFDEFA55: return 585;
        case 0xFFD1D190: return 5855;
        case 0xFFE8FF78: return 586;
        case 0xFFDEDEA6: return 5865;
        case 0xFFF2FF99: return 587;
        case 0xFFEBEBC0: return 5875;
        case 0xFFFFFFB5: return 600;
        case 0xFFFFFF99: return 601;
     /* case 0xFFFFFF7D: return 602;    TODO: duplicate case value */
        case 0xFFFCFC4E: return 603;
        case 0xFFF7F71E: return 604;
        case 0xFFEDE800: return 605;
        case 0xFFE0D700: return 606;
        case 0xFFFCFCCF: return 607;
        case 0xFFFAFAAA: return 608;
        case 0xFFF5F584: return 609;
        case 0xFFF0F065: return 610;
        case 0xFFE3E112: return 611;
        case 0xFFCCC800: return 612;
        case 0xFFB3AB00: return 613;
        case 0xFFF5F5C4: return 614;
        case 0xFFF0EDAF: return 615;
        case 0xFFE8E397: return 616;
        case 0xFFD4CF6E: return 617;
        case 0xFFB3AD17: return 618;
        case 0xFF918C00: return 619;
        case 0xFF787200: return 620;
        case 0xFFD9FAE1: return 621;
        case 0xFFBAF5C6: return 622;
        case 0xFF9CE6AE: return 623;
        case 0xFF72CC85: return 624;
        case 0xFF4BAB60: return 625;
        case 0xFF175E22: return 626;
        case 0xFF04290A: return 627;
        case 0xFFCFFFF0: return 628;
        case 0xFFA8FFE8: return 629;
        case 0xFF87FFE3: return 630;
        case 0xFF52FADC: return 631;
        case 0xFF13F2CE: return 632;
        case 0xFF00BFAC: return 633;
        case 0xFF00998B: return 634;
        case 0xFFADFFEB: return 635;
        case 0xFF8CFFE8: return 636;
        case 0xFF73FFE8: return 637;
        case 0xFF2BFFE6: return 638;
        case 0xFF00F2E6: return 639;
        case 0xFF00C7C7: return 640;
        case 0xFF00ABB3: return 641;
        case 0xFFD2F0FA: return 642;
        case 0xFFB8E4F5: return 643;
        case 0xFF8BCCF0: return 644;
        case 0xFF64A7E8: return 645;
        case 0xFF4696E3: return 646;
        case 0xFF0056C4: return 647;
        case 0xFF002D75: return 648;
        case 0xFFD9EDFC: return 649;
        case 0xFFBEE3FA: return 650;
        case 0xFF95C5F0: return 651;
        case 0xFF5C97E6: return 652;
        case 0xFF004ECC: return 653;
        case 0xFF00399E: return 654;
        case 0xFF002B7A: return 655;
        case 0xFFDBF5FF: return 656;
        case 0xFFC2EBFF: return 657;
        case 0xFF96CCFF: return 658;
        case 0xFF5CA6FF: return 659;
        case 0xFF1A6EFF: return 660;
        case 0xFF0048E8: return 661;
     /* case 0xFF003BD1: return 662;    TODO: duplicate case value */
        case 0xFFEDF0FF: return 663;
        case 0xFFE3E8FF: return 664;
        case 0xFFC8CFFA: return 665;
        case 0xFFA4A6ED: return 666;
        case 0xFF6970DB: return 667;
        case 0xFF3E40B3: return 668;
        case 0xFF201E87: return 669;
        case 0xFFFFDEFF: return 670;
        case 0xFFFCCCFF: return 671;
        case 0xFFF7A8FF: return 672;
        case 0xFFF082FF: return 673;
        case 0xFFE854FF: return 674;
        case 0xFFCD00F7: return 675;
        case 0xFFBB00C7: return 676;
        case 0xFFFADEFF: return 677;
        case 0xFFF7C9FF: return 678;
        case 0xFFF2BAFF: return 679;
        case 0xFFE18EFA: return 680;
        case 0xFFC15FF5: return 681;
        case 0xFFA82FE0: return 682;
        case 0xFF810091: return 683;
        case 0xFFFACFFA: return 684;
        case 0xFFF7BAF7: return 685;
        case 0xFFF2AAF2: return 686;
        case 0xFFDC7EE0: return 687;
        case 0xFFC459CF: return 688;
        case 0xFF9D27A8: return 689;
        case 0xFF690369: return 690;
        case 0xFFFCD7E8: return 691;
        case 0xFFFAC0E1: return 692;
        case 0xFFF0A8D3: return 693;
        case 0xFFE683BA: return 694;
        case 0xFFBF508A: return 695;
        case 0xFF991846: return 696;
        case 0xFF7D0925: return 697;
        case 0xFFFFD6EB: return 698;
        case 0xFFFFC2E6: return 699;
        case 0xFFFFA3DB: return 700;
        case 0xFFFF78CC: return 701;
        case 0xFFF24BA0: return 702;
        case 0xFFD62463: return 703;
        case 0xFFBA0025: return 704;
        case 0xFFFFE8F2: return 705;
        case 0xFFFFD4E6: return 706;
        case 0xFFFFB3DB: return 707;
        case 0xFFFF8AC7: return 708;
        case 0xFFFF579E: return 709;
        case 0xFFFF366B: return 710;
        case 0xFFFA0032: return 711;
        case 0xFFFFDBB0: return 712;
        case 0xFFFFCF96: return 713;
        case 0xFFFFB875: return 714;
        case 0xFFFFA14A: return 715;
        case 0xFFFF8717: return 716;
        case 0xFFFA7000: return 717;
        case 0xFFEB6300: return 718;
        case 0xFFFFE6BF: return 719;
        case 0xFFFCD7A7: return 720;
        case 0xFFF7BC77: return 721;
        case 0xFFE89538: return 722;
        case 0xFFD4740B: return 723;
        case 0xFFA14C00: return 724;
        case 0xFF823B00: return 725;
        case 0xFFFAE6C0: return 726;
        case 0xFFF2CEA0: return 727;
        case 0xFFE6B577: return 728;
        case 0xFFD19052: return 729;
        case 0xFFB56E2B: return 730;
        case 0xFF753700: return 731;
        case 0xFF5C2800: return 732;
        case 0xFFFFF5D1: return 7401;
        case 0xFFFFF0B3: return 7402;
        case 0xFFFFE680: return 7403;
     /* case 0xFFFFE833: return 7404;    TODO: duplicate case value */
     /* case 0xFFFFE600: return 7405;    TODO: duplicate case value */
        case 0xFFFFD100: return 7406;
        case 0xFFE3B122: return 7407;
        case 0xFFFFBF0D: return 7408;
        case 0xFFFFB30D: return 7409;
        case 0xFFFFB373: return 7410;
        case 0xFFFFA64F: return 7411;
        case 0xFFED8A00: return 7412;
        case 0xFFF57300: return 7413;
        case 0xFFE37B00: return 7414;
        case 0xFFFFD1D9: return 7415;
        case 0xFFFF6666: return 7416;
        case 0xFFFF4040: return 7417;
        case 0xFFF24961: return 7418;
        case 0xFFD15473: return 7419;
        case 0xFFCC2976: return 7420;
        case 0xFF630046: return 7421;
     /* case 0xFFFFE8F2: return 7422;    TODO: duplicate case value */
     /* case 0xFFFF73C7: return 7423;    TODO: duplicate case value */
        case 0xFFFF40B3: return 7424;
        case 0xFFED18A6: return 7425;
        case 0xFFD10073: return 7426;
        case 0xFFB80040: return 7427;
        case 0xFF73173F: return 7428;
        case 0xFFFFD1F7: return 7429;
        case 0xFFFAB0FF: return 7430;
        case 0xFFF296ED: return 7431;
        case 0xFFE667DF: return 7432;
        case 0xFFD936B8: return 7433;
        case 0xFFCC29AD: return 7434;
        case 0xFFA60095: return 7435;
        case 0xFFF7EBFF: return 7436;
        case 0xFFF0CCFF: return 7437;
        case 0xFFD9A6FF: return 7438;
        case 0xFFCCA6FF: return 7439;
        case 0xFFB399FF: return 7440;
        case 0xFFA380FF: return 7441;
        case 0xFF804DFF: return 7442;
        case 0xFFF0F2FF: return 7443;
        case 0xFFCCD4FF: return 7444;
        case 0xFFADC6F7: return 7445;
        case 0xFF919EFF: return 7446;
        case 0xFF5357CF: return 7447;
        case 0xFF4E4373: return 7448;
        case 0xFF270020: return 7449;
        case 0xFFCCE6FF: return 7450;
        case 0xFF99C9FF: return 7451;
        case 0xFF80ADFF: return 7452;
        case 0xFF80BDFF: return 7453;
        case 0xFF73AEE6: return 7454;
        case 0xFF3378FF: return 7455;
        case 0xFF6B9AED: return 7456;
        case 0xFFE0FFFA: return 7457;
        case 0xFF90F0E4: return 7458;
        case 0xFF5FDED1: return 7459;
        case 0xFF00F2F2: return 7460;
        case 0xFF38B8FF: return 7461;
        case 0xFF0073E6: return 7462;
        case 0xFF003359: return 7463;
        case 0xFFBFFFE6: return 7464;
        case 0xFF80FFBF: return 7465;
        case 0xFF4DFFC4: return 7466;
        case 0xFF0DFFBF: return 7467;
        case 0xFF00A5B8: return 7468;
        case 0xFF007A99: return 7469;
        case 0xFF1C778C: return 7470;
        case 0xFFB8FFDB: return 7471;
        case 0xFF7AFFBF: return 7472;
        case 0xFF46EB91: return 7473;
        case 0xFF14C78F: return 7474;
        case 0xFF59B386: return 7475;
        case 0xFF00663A: return 7476;
        case 0xFF105249: return 7477;
        case 0xFFD1FFDB: return 7478;
        case 0xFF73FF80: return 7479;
        case 0xFF66FF80: return 7480;
        case 0xFF66FF73: return 7481;
        case 0xFF33FF40: return 7482;
        case 0xFF117300: return 7483;
        case 0xFF008013: return 7484;
        case 0xFFF0FFE6: return 7485;
        case 0xFFCCFFB3: return 7486;
        case 0xFFB3FF8C: return 7487;
        case 0xFF91FF66: return 7488;
        case 0xFF5FED2F: return 7489;
        case 0xFF5BA621: return 7490;
        case 0xFF689900: return 7491;
        case 0xFFD1ED77: return 7492;
        case 0xFFC5E693: return 7493;
        case 0xFFA3D982: return 7494;
        case 0xFF86B324: return 7495;
        case 0xFF5F9E00: return 7496;
        case 0xFF738639: return 7497;
        case 0xFF263300: return 7498;
        case 0xFFFFFAD9: return 7499;
        case 0xFFF7F2D2: return 7500;
        case 0xFFF0E6C0: return 7501;
        case 0xFFE6D395: return 7502;
        case 0xFFBFA87C: return 7503;
        case 0xFF997354: return 7504;
        case 0xFF735022: return 7505;
        case 0xFFFFF2D9: return 7506;
        case 0xFFFFE6B3: return 7507;
        case 0xFFF5D093: return 7508;
        case 0xFFF2C279: return 7509;
        case 0xFFE39F40: return 7510;
        case 0xFFBF6900: return 7511;
        case 0xFFAB5C00: return 7512;
        case 0xFFF7CBB2: return 7513;
        case 0xFFF2B896: return 7514;
        case 0xFFE09270: return 7515;
        case 0xFFA65000: return 7516;
        case 0xFF8F3900: return 7517;
        case 0xFF663D2E: return 7518;
        case 0xFF423500: return 7519;
        case 0xFFFFD6CF: return 7520;
        case 0xFFE6ACB8: return 7521;
        case 0xFFD68196: return 7522;
        case 0xFFCC7A85: return 7523;
        case 0xFFBA544A: return 7524;
        case 0xFFB36259: return 7525;
        case 0xFFA63A00: return 7526;
        case 0xFFEDE8DF: return 7527;
        case 0xFFE6DFCF: return 7528;
        case 0xFFD4CBBA: return 7529;
        case 0xFFADA089: return 7530;
        case 0xFF80735D: return 7531;
        case 0xFF594A2D: return 7532;
        case 0xFF261E06: return 7533;
        case 0xFFE6E1D3: return 7534;
        case 0xFFCCC6AD: return 7535;
        case 0xFFADA687: return 7536;
        case 0xFFC6CCB8: return 7537;
        case 0xFFA2B39B: return 7538;
        case 0xFFA0A395: return 7539;
        case 0xFF474747: return 7540;
        case 0xFFEDF2F2: return 7541;
        case 0xFFC1D6D0: return 7542;
        case 0xFFA6B3B3: return 7543;
        case 0xFF8A9799: return 7544;
        case 0xFF495C5E: return 7545;
        case 0xFF304547: return 7546;
        case 0xFF0A0F0F: return 7547;
        /* BEGIN N/A COLORS */
        /*
        case 0xFF050403: return "Black";
        case 0xFFFCE216: return "Yellow 2X";
        case 0xFFF7B50C: return "116 2X";
        case 0xFFE29100: return "130 2X";
        case 0xFFEA4F00: return "165 2X";
        case 0xFFE03A00: return "Warm Red 2X";
        case 0xFFD62100: return "1788 2X";
        case 0xFFD11600: return "185 2X";
        case 0xFFCC0C00: return "485 2X";
        case 0xFFC6003D: return "Rubine Red 2X";
        case 0xFFD10572: return "Rhodamine Red 2X";
        case 0xFFC4057C: return "239 2X";
        case 0xFFAA0096: return "Purple 2X";
        case 0xFF720082: return "2592 2X";
        case 0xFF59008E: return "Violet 2X";
        case 0xFF1C007A: return "Reflex Blue 2X";
        case 0xFF0077BF: return "Process Blue 2X";
        case 0xFF007FCC: return "299 2X";
        case 0xFF00A3D1: return "306 2X";
        case 0xFF007F82: return "320 2X";
        case 0xFF008977: return "327 2X";
        case 0xFF009677: return "Green 2X";
        case 0xFF009944: return "354 2X";
        case 0xFF009E0F: return "368 2X";
        case 0xFF54BC00: return "375 2X";
        case 0xFF9EC400: return "382 2X";
        case 0xFFA34402: return "471 2X";
        case 0xFF704214: return "464 2X";
        case 0xFF0A0C11: return "433 2X";
        case 0xFF3A3321: return "Black 2";
        case 0xFF282D26: return "Black 3";
        case 0xFF3D3023: return "Black 4";
        case 0xFF422D2D: return "Black 5";
        case 0xFF1C2630: return "Black 6";
        case 0xFF443D38: return "Black 7";
        case 0xFF111111: return "Black 2 2X";
        case 0xFF111114: return "Black 3 2X";
        case 0xFF0F0F0F: return "Black 4 2X";
        case 0xFF110C0F: return "Black 5 2X";
        case 0xFF070C0F: return "Black 6 2X";
        case 0xFF33302B: return "Black 7 2X";
        */
        /* END N/A COLORS */
    }

    return -1;
}

const char* getName_Pantone(unsigned int color)
{
    switch(color)
    {
        case 0xFFF7E214: return "Process Yellow";

        /* SNIP 8<-------------------- */

        case 0xFFFCE016: return "Pantone Yellow";

        /* SNIP 8<-------------------- */

        case 0xFFEF6B00: return "Orange 021";

        /* SNIP 8<-------------------- */

        case 0xFFF93F26: return "Warm Red";

        /* SNIP 8<-------------------- */

        case 0xFFEF2B2D: return "Red 032";

        /* SNIP 8<-------------------- */

        case 0xFFD10056: return "Rubine Red";

        /* SNIP 8<-------------------- */

        case 0xFFED0091: return "Rhodamine Red";

        /* SNIP 8<-------------------- */

        case 0xFFBF30B5: return "Purple";

        /* SNIP 8<-------------------- */

        case 0xFF6607A5: return "Violet";

        /* SNIP 8<-------------------- */

        case 0xFF380096: return "Blue 072";

        /* SNIP 8<-------------------- */

        case 0xFF0C1C8C: return "Reflex Blue";

        /* SNIP 8<-------------------- */

        case 0xFF0091C9: return "Process Blue";

        /* SNIP 8<-------------------- */

        case 0xFF00AF93: return "Green";

        /* SNIP 8<-------------------- */

        case 0xFF3D332B: return "Black";

        /* SNIP 8<-------------------- */

        case 0xFFE5DBCC: return "Warm Gray 1";
        case 0xFFDDD1C1: return "Warm Gray 2";
        case 0xFFCCC1B2: return "Warm Gray 3";
        case 0xFFC1B5A5: return "Warm Gray 4";
        case 0xFFB5A899: return "Warm Gray 5";
        case 0xFFAFA393: return "Warm Gray 6";
        case 0xFFA39687: return "Warm Gray 7";
        case 0xFF96897A: return "Warm Gray 8";
        case 0xFF8C7F70: return "Warm Gray 9";
        case 0xFF827263: return "Warm Gray 10";
        case 0xFF6D5E51: return "Warm Gray 11";
        case 0xFFE8E2D6: return "Cool Gray 1";
        case 0xFFDDD8CE: return "Cool Gray 2";
        case 0xFFD3CEC4: return "Cool Gray 3";
        case 0xFFC4C1BA: return "Cool Gray 4";
        case 0xFFBAB7AF: return "Cool Gray 5";
        case 0xFFB5B2AA: return "Cool Gray 6";
        case 0xFFA5A39E: return "Cool Gray 7";
        case 0xFF9B9993: return "Cool Gray 8";
        case 0xFF8C8984: return "Cool Gray 9";
        case 0xFF777772: return "Cool Gray 10";

        /* SNIP 8<-------------------- */

        case 0xFF050403: return "Black";
        case 0xFFFCE216: return "Yellow 2X";
        case 0xFFF7B50C: return "116 2X";
        case 0xFFE29100: return "130 2X";
        case 0xFFEA4F00: return "165 2X";
        case 0xFFE03A00: return "Warm Red 2X";
        case 0xFFD62100: return "1788 2X";
        case 0xFFD11600: return "185 2X";
        case 0xFFCC0C00: return "485 2X";
        case 0xFFC6003D: return "Rubine Red 2X";
        case 0xFFD10572: return "Rhodamine Red 2X";
        case 0xFFC4057C: return "239 2X";
        case 0xFFAA0096: return "Purple 2X";
        case 0xFF720082: return "2592 2X";
        case 0xFF59008E: return "Violet 2X";
        case 0xFF1C007A: return "Reflex Blue 2X";
        case 0xFF0077BF: return "Process Blue 2X";
        case 0xFF007FCC: return "299 2X";
        case 0xFF00A3D1: return "306 2X";
        case 0xFF007F82: return "320 2X";
        case 0xFF008977: return "327 2X";
        case 0xFF009677: return "Green 2X";
        case 0xFF009944: return "354 2X";
        case 0xFF009E0F: return "368 2X";
        case 0xFF54BC00: return "375 2X";
        case 0xFF9EC400: return "382 2X";
        case 0xFFA34402: return "471 2X";
        case 0xFF704214: return "464 2X";
        case 0xFF0A0C11: return "433 2X";
        case 0xFF3A3321: return "Black 2";
        case 0xFF282D26: return "Black 3";
        case 0xFF3D3023: return "Black 4";
        case 0xFF422D2D: return "Black 5";
        case 0xFF1C2630: return "Black 6";
        case 0xFF443D38: return "Black 7";
        case 0xFF111111: return "Black 2 2X";
        case 0xFF111114: return "Black 3 2X";
        case 0xFF0F0F0F: return "Black 4 2X";
        case 0xFF110C0F: return "Black 5 2X";
        case 0xFF070C0F: return "Black 6 2X";
        case 0xFF33302B: return "Black 7 2X";
    }

    return "";
}

int getNum_RobisonAnton_Polyester(unsigned int color) { return -1; } /*TODO: finish */
const char* getName_RobisonAnton_Polyester(unsigned int color) { return ""; } /*TODO: finish */

int getNum_RobisonAnton_Rayon(unsigned int color)
{
    switch(color)
    {
        case 0xFFEFCCCE: return 2243;
        case 0xFFFCBFC9: return 2223;
        case 0xFFEFC6D3: return 2599;
        case 0xFFF9B2B7: return 2373;
        case 0xFFF9AFAD: return 2374;
        case 0xFFFC9BB2: return 2293;
        case 0xFFFC8C99: return 2244;
        case 0xFFF2AFC1: return 2237;
        case 0xFFE5566D: return 2246;
        case 0xFFF4476B: return 2248;
     /* case 0xFFE5566D: return 2228;    TODO: duplicate case value */
        case 0xFFF9848E: return 2412;
        case 0xFFF26877: return 2375;
        case 0xFFD8899B: return 2591;
        case 0xFF8C2633: return 2249;
        case 0xFF7C2128: return 2608;
        case 0xFF7A2638: return 2252;
        case 0xFF75263D: return 2622;
        case 0xFF772D35: return 2225;
        case 0xFF593344: return 2587;
        case 0xFF4F213A: return 2376;
        case 0xFF931638: return 2494;
     /* case 0xFFF9B2B7: return 2495;    TODO: duplicate case value */
        case 0xFF661E2B: return 2496;
     /* case 0xFFE5566D: return 2491;    TODO: duplicate case value */
        case 0xFF8E2344: return 2497;
        case 0xFF6D213F: return 2498;
     /* case 0xFF8E2344: return 2499;    TODO: duplicate case value */
        case 0xFFAD0075: return 2500;
        case 0xFFFCC9C6: return 2501;
        case 0xFFF4BFD1: return 2502;
        case 0xFFF7BFBF: return 2503;
        case 0xFFCE007C: return 2259;
        case 0xFFED72AA: return 2415;
        case 0xFFD36B9E: return 2260;
        case 0xFFD60270: return 2416;
        case 0xFFE22882: return 2261;
        case 0xFFAA004F: return 2504;
     /* case 0xFFAA004F: return 2417;    TODO: duplicate case value */
        case 0xFFEA0F6B: return 2262;
        case 0xFFAF1E2D: return 2418;
        case 0xFFBF0A30: return 2281;
     /* case 0xFFBF0A30: return 2419;    TODO: duplicate case value */
     /* case 0xFFBF0A30: return 2378;    TODO: duplicate case value */
        case 0xFFCE1126: return 2263;
        case 0xFFD62828: return 2420;
     /* case 0xFFBF0A30: return 2233;    TODO: duplicate case value */
        case 0xFFC41E3A: return 2219;
     /* case 0xFFC41E3A: return 2266;    TODO: duplicate case value */
        case 0xFFA32638: return 2267;
     /* case 0xFF8C2633: return 2268;    TODO: duplicate case value */
     /* case 0xFF75263D: return 2421;    TODO: duplicate case value */
        case 0xFF992135: return 2270;
     /* case 0xFFA32638: return 2505;    TODO: duplicate case value */
     /* case 0xFFC41E3A: return 2506;    TODO: duplicate case value */
        case 0xFFAF003D: return 2507;
        case 0xFFD81C3F: return 2508;
        case 0xFFFC5E72: return 2509;
        case 0xFFF2C4AF: return 2377;
        case 0xFFF4CCAA: return 2413;
        case 0xFFF9BAAA: return 2253;
     /* case 0xFFF7BFBF: return 2255;    TODO: duplicate case value */
        case 0xFFF9BF9E: return 2256;
        case 0xFFF9C6AA: return 2257;
        case 0xFFF98E6D: return 2294;
        case 0xFFF9A58C: return 2258;
        case 0xFFF98972: return 2414;
        case 0xFFF43F4F: return 2277;
     /* case 0xFFF9C6AA: return 2510;    TODO: duplicate case value */
     /* case 0xFFF9BAAA: return 2511;    TODO: duplicate case value */
     /* case 0xFFF98972: return 2512;    TODO: duplicate case value */
        case 0xFFE23D28: return 2513;
        case 0xFF008C82: return 2514;
        case 0xFF00B2A0: return 2515;
        case 0xFF47D6C1: return 2516;
        case 0xFF87DDD1: return 2517;
        case 0xFF008789: return 2492;
        case 0xFF2DC6D6: return 2518;
        case 0xFF00A5DB: return 2519;
        case 0xFF00A3DD: return 2520;
        case 0xFF003F54: return 2589;
        case 0xFF002D47: return 2620;
        case 0xFF006D75: return 2521;
        case 0xFF9BC4E2: return 2239;
        case 0xFFAFBCDB: return 2304;
        case 0xFF5B77CC: return 2614;
        case 0xFF6D87A8: return 2275;
        case 0xFF99D6DD: return 2305;
        case 0xFF28C4D8: return 2306;
        case 0xFF00ADC6: return 2307;
        case 0xFF00A0C4: return 2389;
        case 0xFF008ED6: return 2441;
        case 0xFF0054A0: return 2442;
     /* case 0xFF008ED6: return 2388;    TODO: duplicate case value */
     /* case 0xFF00A3DD: return 2730;    TODO: duplicate case value */
        case 0xFF00709E: return 2737;
        case 0xFF007AA5: return 2740;
        case 0xFFC9E8DD: return 2222;
        case 0xFF1E1C77: return 2210;
        case 0xFF002B7F: return 2438;
        case 0xFF335687: return 2302;
     /* case 0xFFAFBCDB: return 2522;    TODO: duplicate case value */
     /* case 0xFF335687: return 2523;    TODO: duplicate case value */
        case 0xFF26547C: return 2612;
        case 0xFF7796B2: return 2524;
        case 0xFFC1C9DD: return 2525;
     /* case 0xFF7796B2: return 2624;    TODO: duplicate case value */
        case 0xFF6689CC: return 2526;
        case 0xFF5960A8: return 2527;
        case 0xFF0051BA: return 2619;
     /* case 0xFF6689CC: return 2528;    TODO: duplicate case value */
        case 0xFF3A75C4: return 2529;
        case 0xFF75AADB: return 2530;
        case 0xFFC4D8E2: return 2531;
     /* case 0xFFC4D8E2: return 2532;    TODO: duplicate case value */
        case 0xFF60AFDD: return 2533;
        case 0xFF003D6B: return 2534;
        case 0xFF0F2B5B: return 2439;
        case 0xFF192168: return 2625;
        case 0xFF2B265B: return 2440;
        case 0xFF002654: return 2613;
        case 0xFF002649: return 2647;
        case 0xFF353F5B: return 2386;
        case 0xFF35264F: return 2450;
        case 0xFF112151: return 2303;
        case 0xFF14213D: return 2603;
     /* case 0xFF002654: return 2609;    TODO: duplicate case value */
     /* case 0xFF14213D: return 2215;    TODO: duplicate case value */
     /* case 0xFF14213D: return 2387;    TODO: duplicate case value */
        case 0xFFC6D1D6: return 2382;
     /* case 0xFFC1C9DD: return 2598;    TODO: duplicate case value */
        case 0xFFB5D1E8: return 2300;
        case 0xFFD1CEDD: return 2283;
        case 0xFF99BADD: return 2206;
     /* case 0xFFAFBCDB: return 2269;    TODO: duplicate case value */
        case 0xFFA5BAE0: return 2383;
        case 0xFF75B2DD: return 2433;
     /* case 0xFF75AADB: return 2434;    TODO: duplicate case value */
     /* case 0xFF6689CC: return 2435;    TODO: duplicate case value */
        case 0xFF7F8CBF: return 2301;
     /* case 0xFF6D87A8: return 2245;    TODO: duplicate case value */
     /* case 0xFF5B77CC: return 2384;    TODO: duplicate case value */
        case 0xFF2D338E: return 2220;
     /* case 0xFF2D338E: return 2280;    TODO: duplicate case value */
        case 0xFF00337F: return 2627;
        case 0xFF003893: return 2436;
     /* case 0xFF2D338E: return 2385;    TODO: duplicate case value */
        case 0xFF3F2893: return 2437;
        case 0xFF3A564F: return 2535;
        case 0xFF8499A5: return 2617;
        case 0xFF9BAABF: return 2536;
        case 0xFF5E99AA: return 2577;
        case 0xFFADAFAA: return 2540;
        case 0xFFBFBAAF: return 2537;
        case 0xFFBAB7AF: return 2741;
        case 0xFFC4C1BA: return 2733;
        case 0xFFCCC1B2: return 2538;
        case 0xFFD1CCBF: return 2539;
     /* case 0xFFADAFAA: return 2618;    TODO: duplicate case value */
        case 0xFF8C8984: return 2731;
        case 0xFF686663: return 2729;
        case 0xFF443D38: return 2541;
        case 0xFFDDC6C4: return 2271;
        case 0xFFD3B7A3: return 2272;
        case 0xFFB5939B: return 2314;
        case 0xFF8E6877: return 2422;
     /* case 0xFFFCBFC9: return 2423;    TODO: duplicate case value */
        case 0xFFE5BFC6: return 2379;
     /* case 0xFFEFC6D3: return 2276;    TODO: duplicate case value */
        case 0xFFEDC4DD: return 2285;
        case 0xFFB58CB2: return 2424;
        case 0xFFC9ADD8: return 2286;
        case 0xFFE29ED6: return 2588;
        case 0xFF512654: return 2600;
        case 0xFF512D44: return 2616;
        case 0xFF8E47AD: return 2425;
        case 0xFFAF72C1: return 2288;
        case 0xFF9B4F96: return 2426;
        case 0xFF66116D: return 2380;
        case 0xFF63305E: return 2490;
     /* case 0xFF1E1C77: return 2429;    TODO: duplicate case value */
        case 0xFF332875: return 2427;
        case 0xFF35006D: return 2428;
        case 0xFF2B1166: return 2736;
        case 0xFF38197A: return 2742;
     /* case 0xFF35006D: return 2628;    TODO: duplicate case value */
     /* case 0xFF8E47AD: return 2254;    TODO: duplicate case value */
        case 0xFF5B027A: return 2430;
        case 0xFF4C145E: return 2381;
        case 0xFF8977BA: return 2287;
        case 0xFF44235E: return 2431;
        case 0xFFAA0066: return 2590;
     /* case 0xFFCE007C: return 2291;    TODO: duplicate case value */
     /* case 0xFFAA004F: return 2432;    TODO: duplicate case value */
        case 0xFF9E2387: return 2292;
     /* case 0xFF5E99AA: return 2308;    TODO: duplicate case value */
        case 0xFF609191: return 2443;
     /* case 0xFF609191: return 2309;    TODO: duplicate case value */
     /* case 0xFF003F54: return 2444;    TODO: duplicate case value */
        case 0xFF93DDDB: return 2310;
        case 0xFF7FD6DB: return 2204;
        case 0xFF007272: return 2445;
        case 0xFF70CE9B: return 2311;
     /* case 0xFF70CE9B: return 2312;    TODO: duplicate case value */
        case 0xFF35C4AF: return 2390;
     /* case 0xFF006D75: return 2446;    TODO: duplicate case value */
        case 0xFF006B77: return 2621;
        case 0xFF006663: return 2447;
        case 0xFF006D66: return 2735;
     /* case 0xFF008C82: return 2391;    TODO: duplicate case value */
        case 0xFF008272: return 2448;
     /* case 0xFF006663: return 2449;    TODO: duplicate case value */
     /* case 0xFF006D66: return 2615;    TODO: duplicate case value */
        case 0xFFBCC1B2: return 2241;
        case 0xFFC6D6A0: return 2282;
        case 0xFF9EAA99: return 2221;
        case 0xFFB2D8D8: return 2313;
        case 0xFF7AA891: return 2278;
     /* case 0xFF7AA891: return 2594;    TODO: duplicate case value */
        case 0xFF006056: return 2451;
     /* case 0xFF006056: return 2743;    TODO: duplicate case value */
        case 0xFF4F6D5E: return 2392;
        case 0xFF024930: return 2315;
        case 0xFF004438: return 2734;
        case 0xFF2B4C3F: return 2631;
        case 0xFF282D26: return 2411;
        case 0xFFF2EABC: return 2316;
        case 0xFFCCC693: return 2250;
        case 0xFF5E663A: return 2317;
        case 0xFF779182: return 2202;
     /* case 0xFFC9E8DD: return 2318;    TODO: duplicate case value */
     /* case 0xFF93DDDB: return 2452;    TODO: duplicate case value */
     /* case 0xFF93DDDB: return 2393;    TODO: duplicate case value */
        case 0xFFB5E8BF: return 2238;
        case 0xFFAADD6D: return 2279;
        case 0xFFA0DB8E: return 2211;
        case 0xFFB5CC8E: return 2319;
        case 0xFF8CD600: return 2320;
        case 0xFF56AA1C: return 2738;
        case 0xFF339E35: return 2214;
        case 0xFF007A3D: return 2453;
     /* case 0xFF007A3D: return 2410;    TODO: duplicate case value */
     /* case 0xFF007A3D: return 2240;    TODO: duplicate case value */
        case 0xFF008751: return 2208;
        case 0xFF006B3F: return 2454;
        case 0xFF006854: return 2607;
        case 0xFF1EB53A: return 2578;
        case 0xFF009E49: return 2579;
     /* case 0xFF009E49: return 2580;    TODO: duplicate case value */
     /* case 0xFF006B3F: return 2284;    TODO: duplicate case value */
        case 0xFF006B54: return 2455;
        case 0xFF007C66: return 2597;
        case 0xFFA3AF07: return 2456;
        case 0xFF7FBA00: return 2321;
     /* case 0xFF7FBA00: return 2457;    TODO: duplicate case value */
     /* case 0xFF7FBA00: return 2322;    TODO: duplicate case value */
        case 0xFF568E14: return 2226;
        case 0xFF939905: return 2230;
        case 0xFF566314: return 2229;
     /* case 0xFF024930: return 2458;    TODO: duplicate case value */
        case 0xFF547730: return 2595;
        case 0xFF3F4926: return 2601;
        case 0xFF3A7728: return 2209;
        case 0xFF193833: return 2459;
        case 0xFF215B33: return 2323;
        case 0xFF265142: return 2460;
     /* case 0xFF3F4926: return 2584;    TODO: duplicate case value */
        case 0xFF99840A: return 2542;
        case 0xFFA38205: return 2543;
        case 0xFF897719: return 2544;
        case 0xFF707014: return 2545;
        case 0xFF848205: return 2546;
        case 0xFFE2E584: return 2547;
        case 0xFF998E07: return 2548;
        case 0xFF00494F: return 2549;
     /* case 0xFF4F6D5E: return 2550;    TODO: duplicate case value */
     /* case 0xFF779182: return 2551;    TODO: duplicate case value */
        case 0xFF546856: return 2554;
        case 0xFF0C3026: return 2552;
        case 0xFF233A2D: return 2553;
        case 0xFF213D30: return 2728;
        case 0xFFC9D6A3: return 2555;
        case 0xFFE0AA0F: return 2556;
        case 0xFFF4E287: return 2557;
        case 0xFFFFC61E: return 2558;
        case 0xFFF7E8AA: return 2559;
        case 0xFFF9DD16: return 2560;
        case 0xFFC6A00C: return 2561;
        case 0xFFA37F14: return 2562;
     /* case 0xFFF7E8AA: return 2324;    TODO: duplicate case value */
     /* case 0xFFF7E8AA: return 2264;    TODO: duplicate case value */
        case 0xFFEADD96: return 2461;
        case 0xFFF9E08C: return 2732;
        case 0xFFFFD87F: return 2234;
        case 0xFFFCD856: return 2408;
        case 0xFFFCA311: return 2394;
     /* case 0xFFFCA311: return 2409;    TODO: duplicate case value */
     /* case 0xFFFCA311: return 2213;    TODO: duplicate case value */
     /* case 0xFFF9DD16: return 2462;    TODO: duplicate case value */
        case 0xFFF4ED47: return 2325;
        case 0xFFF9E814: return 2326;
        case 0xFFFCB514: return 2463;
        case 0xFFFFCC49: return 2395;
     /* case 0xFFFFC61E: return 2464;    TODO: duplicate case value */
        case 0xFFFCBF49: return 2465;
        case 0xFFFCE016: return 2235;
     /* case 0xFFFFC61E: return 2626;    TODO: duplicate case value */
        case 0xFFFCD116: return 2466;
     /* case 0xFFFCD116: return 2242;    TODO: duplicate case value */
     /* case 0xFFFFCC49: return 2396;    TODO: duplicate case value */
        case 0xFFF99B0C: return 2327;
        case 0xFFF77F00: return 2328;
        case 0xFFF74902: return 2467;
     /* case 0xFFF74902: return 2397;    TODO: duplicate case value */
        case 0xFFF95602: return 2236;
        case 0xFFF93F26: return 2329;
     /* case 0xFFF95602: return 2468;    TODO: duplicate case value */
        case 0xFFF96B07: return 2218;
        case 0xFFF96302: return 2469;
        case 0xFFA53F0F: return 2581;
        case 0xFFFC8744: return 2330;
        case 0xFFBC4F07: return 2289;
        case 0xFFAF7505: return 2295;
        case 0xFFC18E60: return 2493;
        case 0xFFFCCE87: return 2398;
     /* case 0xFFFFCC49: return 2605;    TODO: duplicate case value */
        case 0xFFFCBA5E: return 2247;
     /* case 0xFFFFCC49: return 2216;    TODO: duplicate case value */
     /* case 0xFFE0AA0F: return 2331;    TODO: duplicate case value */
     /* case 0xFFE0AA0F: return 2212;    TODO: duplicate case value */
     /* case 0xFFE0AA0F: return 2470;    TODO: duplicate case value */
        case 0xFFF2BF49: return 2332;
     /* case 0xFFFCCE87: return 2399;    TODO: duplicate case value */
        case 0xFFE2D6B5: return 2630;
     /* case 0xFFC6A00C: return 2596;    TODO: duplicate case value */
     /* case 0xFFA37F14: return 2333;    TODO: duplicate case value */
     /* case 0xFFC6A00C: return 2471;    TODO: duplicate case value */
        case 0xFFBF910C: return 2201;
        case 0xFFEAB2B2: return 2299;
        case 0xFFC13828: return 2205;
        case 0xFFA03033: return 2334;
     /* case 0xFFAF1E2D: return 2623;    TODO: duplicate case value */
     /* case 0xFFAF1E2D: return 2472;    TODO: duplicate case value */
        case 0xFFFAE6CC: return 2582;
        case 0xFFFAE6CF: return 2335;
        case 0xFFF7D3B5: return 2473;
        case 0xFFEDD3BC: return 2232;
        case 0xFFAA753F: return 2489;
        case 0xFFD3A87C: return 2273;
        case 0xFFEDD3B5: return 2593;
        case 0xFFC1A875: return 2474;
        case 0xFFE2BF9B: return 2203;
     /* case 0xFFD3A87C: return 2475;    TODO: duplicate case value */
     /* case 0xFFBF910C: return 2400;    TODO: duplicate case value */
        case 0xFFD18E54: return 2401;
        case 0xFFD8B596: return 2336;
        case 0xFFD6CCAF: return 2476;
        case 0xFFF2E3C4: return 2604;
     /* case 0xFFAA753F: return 2224;    TODO: duplicate case value */
        case 0xFF6B4714: return 2477;
        case 0xFFD1BF91: return 2298;
        case 0xFF6C463D: return 2610;
        case 0xFF876028: return 2227;
        case 0xFF755426: return 2629;
     /* case 0xFF755426: return 2478;    TODO: duplicate case value */
        case 0xFFB28260: return 2488;
        case 0xFF593D2B: return 2251;
     /* case 0xFF593D2B: return 2372;    TODO: duplicate case value */
        case 0xFF3F302B: return 2337;
     /* case 0xFFC18E60: return 2338;    TODO: duplicate case value */
     /* case 0xFFAF7505: return 2479;    TODO: duplicate case value */
        case 0xFFBA7530: return 2231;
     /* case 0xFF755426: return 2402;    TODO: duplicate case value */
        case 0xFFB26B70: return 2480;
        case 0xFFA2464E: return 2611;
        case 0xFF9B4F19: return 2290;
     /* case 0xFF9B4F19: return 2481;    TODO: duplicate case value */
        case 0xFF5B2D28: return 2339;
     /* case 0xFF593D2B: return 2563;    TODO: duplicate case value */
     /* case 0xFF3F302B: return 2564;    TODO: duplicate case value */
        case 0xFF3D3028: return 2566;
        case 0xFF633A11: return 2567;
     /* case 0xFFC1A875: return 2568;    TODO: duplicate case value */
        case 0xFF7A5B11: return 2569;
     /* case 0xFFC1A875: return 2570;    TODO: duplicate case value */
     /* case 0xFFF2BF49: return 2586;    TODO: duplicate case value */
        case 0xFFF2CE68: return 2606;
        case 0xFFD88C02: return 2602;
        case 0xFFC1B5A5: return 2571;
        case 0xFF99897C: return 2739;
        case 0xFFADA07A: return 2572;
     /* case 0xFFADA07A: return 2573;    TODO: duplicate case value */
        case 0xFFF5E3CC: return 2574;
        case 0xFF66594C: return 2575;
        case 0xFF493533: return 2576;
        case 0xFFF5EBE0: return 2403;
     /* case 0xFFDDC6C4: return 2207;    TODO: duplicate case value */
        case 0xFFDBD3D3: return 2340;
        case 0xFFD8CCD1: return 2274;
        case 0xFFCCC1C6: return 2482;
        case 0xFFAFAAA3: return 2483;
     /* case 0xFFADAFAA: return 2592;    TODO: duplicate case value */
        case 0xFF919693: return 2585;
     /* case 0xFF8C8984: return 2484;    TODO: duplicate case value */
     /* case 0xFFCCC1C6: return 2404;    TODO: duplicate case value */
     /* case 0xFFDBD3D3: return 2485;    TODO: duplicate case value */
     /* case 0xFFCCC1C6: return 2405;    TODO: duplicate case value */
        case 0xFFB2A8B5: return 2486;
        case 0xFFA893AD: return 2406;
        case 0xFF666D70: return 2407;
     /* case 0xFF686663: return 2217;    TODO: duplicate case value */
     /* case 0xFF443D38: return 2565;    TODO: duplicate case value */
        case 0xFF777772: return 2265;
        case 0xFF353842: return 2487;
        case 0xFF3A4972: return 2341;
        case 0xFF1C2630: return 2296;
     /* case 0xFFF5EBE0: return 2297;    TODO: duplicate case value */
        case 0xFFF5EDDE: return 2342;
        case 0xFFF0E8D6: return 2343;
     /* case 0xFF1C2630: return 2632;    TODO: duplicate case value */

        /* BEGIN N/A COLORS */
        /*
                         return 2344;
                         return 2345;
                         return 2346;
                         return 2347;
                         return 2348;
                         return 2349;
                         return 2350;
                         return 2351;
                         return 2352;
                         return 2353;
                         return 2354;
                         return 2355;
                         return 2583;
                         return 2356;
                         return 2357;
                         return 2358;
                         return 2359;
                         return 2360;
                         return 2361;
                         return 2362;
                         return 2363;
                         return 2364;
                         return 2365;
                         return 2366;
                         return 2367;
                         return 2368;
                         return 2369;
                         return 2370;
                         return 2371;
        */
        /* END N/A COLORS */
    }

    return -1;
}

const char* getName_RobisonAnton_Rayon(unsigned int color)
{
    switch(color)
    {
        case 0xFFEFCCCE: return "Light Pink";
        case 0xFFFCBFC9: return "Pink";
        case 0xFFEFC6D3: return "Pink Bazaar";
        case 0xFFF9B2B7: return "Pink Mist";
        case 0xFFF9AFAD: return "Emily Pink";
        case 0xFFFC9BB2: return "Rose";
        case 0xFFFC8C99: return "Rose Cerise";
        case 0xFFF2AFC1: return "Carnation";
        case 0xFFE5566D: return "Shrimp";
        case 0xFFF4476B: return "Bashful Pink";
     /* case 0xFFE5566D: return "Begonia";    TODO: duplicate case value */
        case 0xFFF9848E: return "Azalea";
        case 0xFFF26877: return "Dusty Rose";
        case 0xFFD8899B: return "Rose Tint";
        case 0xFF8C2633: return "Burgundy";
        case 0xFF7C2128: return "TH Burgundy";
        case 0xFF7A2638: return "Russet";
        case 0xFF75263D: return "Pro Firebrand";
        case 0xFF772D35: return "Wine";
        case 0xFF593344: return "Intense Maroon";
        case 0xFF4F213A: return "Dark Maroon";
        case 0xFF931638: return "Carbernet";
     /* case 0xFFF9B2B7: return "Mountain Rose";    TODO: duplicate case value */
        case 0xFF661E2B: return "Warm Wine";
     /* case 0xFFE5566D: return "Primrose";    TODO: duplicate case value */
        case 0xFF8E2344: return "Perfect Ruby";
        case 0xFF6D213F: return "Brushed Burgundy";
     /* case 0xFF8E2344: return "Passion Rose";    TODO: duplicate case value */
        case 0xFFAD0075: return "New Berry";
        case 0xFFFCC9C6: return "Petal Pink";
        case 0xFFF4BFD1: return "Memphis Belle";
        case 0xFFF7BFBF: return "Desert Bloom";
        case 0xFFCE007C: return "Wild Pink";
        case 0xFFED72AA: return "Floral Pink";
        case 0xFFD36B9E: return "Hot Pink";
        case 0xFFD60270: return "Crimson";
        case 0xFFE22882: return "Ruby Glint";
        case 0xFFAA004F: return "Cherrystone";
     /* case 0xFFAA004F: return "Cherry Punch";    TODO: duplicate case value */
        case 0xFFEA0F6B: return "Cherry Blossom";
        case 0xFFAF1E2D: return "Red Berry";
        case 0xFFBF0A30: return "Jockey Red";
     /* case 0xFFBF0A30: return "Very Red";    TODO: duplicate case value */
     /* case 0xFFBF0A30: return "Red Berry";    TODO: duplicate case value */
        case 0xFFCE1126: return "Foxy Red";
        case 0xFFD62828: return "Tuxedo Red";
     /* case 0xFFBF0A30: return "Lipstick";    TODO: duplicate case value */
        case 0xFFC41E3A: return "Scarlet";
     /* case 0xFFC41E3A: return "Radiant Red";    TODO: duplicate case value */
        case 0xFFA32638: return "Wildfire";
     /* case 0xFF8C2633: return "Carolina Red";    TODO: duplicate case value */
     /* case 0xFF75263D: return "Red Jubilee";    TODO: duplicate case value */
        case 0xFF992135: return "Cranberry";
     /* case 0xFFA32638: return "Antique Red";    TODO: duplicate case value */
     /* case 0xFFC41E3A: return "Devil Red";    TODO: duplicate case value */
        case 0xFFAF003D: return "Candy Apple Red";
        case 0xFFD81C3F: return "Rosewood";
        case 0xFFFC5E72: return "Bitteroot";
        case 0xFFF2C4AF: return "Bisque";
        case 0xFFF4CCAA: return "Flesh";
        case 0xFFF9BAAA: return "Flesh Pink";
     /* case 0xFFF7BFBF: return "Opal Mist";    TODO: duplicate case value */
        case 0xFFF9BF9E: return "Tawny";
        case 0xFFF9C6AA: return "Peach";
        case 0xFFF98E6D: return "Melon";
        case 0xFFF9A58C: return "Flamingo";
        case 0xFFF98972: return "Coral";
        case 0xFFF43F4F: return "Persimmon";
     /* case 0xFFF9C6AA: return "Peach Blossom";    TODO: duplicate case value */
     /* case 0xFFF9BAAA: return "Illusion";    TODO: duplicate case value */
     /* case 0xFFF98972: return "Melonade";    TODO: duplicate case value */
        case 0xFFE23D28: return "Honeysuckle";
        case 0xFF008C82: return "Brite Jade";
        case 0xFF00B2A0: return "Bluestone";
        case 0xFF47D6C1: return "Aqua Pearl";
        case 0xFF87DDD1: return "Seafrost";
        case 0xFF008789: return "J. Turquoise";
        case 0xFF2DC6D6: return "Indian Ocean Blue";
        case 0xFF00A5DB: return "Surf Blue";
        case 0xFF00A3DD: return "Mid Windsor";
        case 0xFF003F54: return "Deep Windsor";
        case 0xFF002D47: return "Pro Dark Blue";
        case 0xFF006D75: return "Mallard Blue";
        case 0xFF9BC4E2: return "Sky Blue";
        case 0xFFAFBCDB: return "Lake Blue";
        case 0xFF5B77CC: return "Pro Lusty Blue";
        case 0xFF6D87A8: return "Slate Blue";
        case 0xFF99D6DD: return "Blue Frost";
        case 0xFF28C4D8: return "Periwinkle";
        case 0xFF00ADC6: return "Aquamarine";
        case 0xFF00A0C4: return "California Blue";
        case 0xFF008ED6: return "Baltic Blue";
        case 0xFF0054A0: return "Solar Blue";
     /* case 0xFF008ED6: return "Pacific Blue";    TODO: duplicate case value */
     /* case 0xFF00A3DD: return "Boo Boo Blue";    TODO: duplicate case value */
        case 0xFF00709E: return "Pro Band Blue";
        case 0xFF007AA5: return "Pro Peacock";
        case 0xFFC9E8DD: return "Light Blue";
        case 0xFF1E1C77: return "Royal";
        case 0xFF002B7F: return "Blue Suede";
        case 0xFF335687: return "Imperial Blue";
     /* case 0xFFAFBCDB: return "Bridgeport Blue";    TODO: duplicate case value */
     /* case 0xFF335687: return "China Blue";    TODO: duplicate case value */
        case 0xFF26547C: return "Pro Imperial";
        case 0xFF7796B2: return "Country Blue";
        case 0xFFC1C9DD: return "Heron Blue";
     /* case 0xFF7796B2: return "Pro Saxon";    TODO: duplicate case value */
        case 0xFF6689CC: return "Bright Blue";
        case 0xFF5960A8: return "Soldier Blue";
        case 0xFF0051BA: return "Pro Brilliance";
     /* case 0xFF6689CC: return "Atlantis Blue";    TODO: duplicate case value */
        case 0xFF3A75C4: return "Dolphin Blue";
        case 0xFF75AADB: return "Caribbean Blue";
        case 0xFFC4D8E2: return "Dana Blue";
     /* case 0xFFC4D8E2: return "Cadet Blue";    TODO: duplicate case value */
        case 0xFF60AFDD: return "Ozone";
        case 0xFF003D6B: return "Salem Blue";
        case 0xFF0F2B5B: return "Blue Ribbon";
        case 0xFF192168: return "Pro Navy";
        case 0xFF2B265B: return "Blue Ink";
        case 0xFF002654: return "Pro Midnight";
        case 0xFF002649: return "Pro College Blue";
        case 0xFF353F5B: return "Light Midnight";
        case 0xFF35264F: return "Fleet Blue";
        case 0xFF112151: return "Light Navy";
        case 0xFF14213D: return "Flag Blue";
     /* case 0xFF002654: return "TH Navy";    TODO: duplicate case value */
     /* case 0xFF14213D: return "Navy";    TODO: duplicate case value */
     /* case 0xFF14213D: return "Midnight Navy";    TODO: duplicate case value */
        case 0xFFC6D1D6: return "Pastel Blue";
     /* case 0xFFC1C9DD: return "Blue Hint";    TODO: duplicate case value */
        case 0xFFB5D1E8: return "Ice Blue";
        case 0xFFD1CEDD: return "Paris Blue";
        case 0xFF99BADD: return "Baby Blue";
     /* case 0xFFAFBCDB: return "Sun Blue";    TODO: duplicate case value */
        case 0xFFA5BAE0: return "Cristy Blue";
        case 0xFF75B2DD: return "Ultra Blue";
     /* case 0xFF75AADB: return "Tropic Blue";    TODO: duplicate case value */
     /* case 0xFF6689CC: return "Blue Horizon";    TODO: duplicate case value */
        case 0xFF7F8CBF: return "Oriental Blue";
     /* case 0xFF6D87A8: return "Copen";    TODO: duplicate case value */
     /* case 0xFF5B77CC: return "Jay Blue";    TODO: duplicate case value */
        case 0xFF2D338E: return "Blue";
     /* case 0xFF2D338E: return "Sapphire";    TODO: duplicate case value */
        case 0xFF00337F: return "Pro Royal";
        case 0xFF003893: return "Fire Blue";
     /* case 0xFF2D338E: return "Jamie Blue";    TODO: duplicate case value */
        case 0xFF3F2893: return "Empire Blue";
        case 0xFF3A564F: return "Enchanted Sea";
        case 0xFF8499A5: return "Pro Twinkle";
        case 0xFF9BAABF: return "Rockport Blue";
        case 0xFF5E99AA: return "Wonder Blue";
        case 0xFFADAFAA: return "Traditional Gray";
        case 0xFFBFBAAF: return "Steel";
        case 0xFFBAB7AF: return "Pro Pearl";
        case 0xFFC4C1BA: return "Pro Cool Gray";
        case 0xFFCCC1B2: return "Stainless Steel";
        case 0xFFD1CCBF: return "Chrome";
     /* case 0xFFADAFAA: return "Pro Night Sky";    TODO: duplicate case value */
        case 0xFF8C8984: return "Gull";
        case 0xFF686663: return "Mineral";
        case 0xFF443D38: return "Black Chrome";
        case 0xFFDDC6C4: return "Heather";
        case 0xFFD3B7A3: return "Grape";
        case 0xFFB5939B: return "Satin Wine";
        case 0xFF8E6877: return "Ducky Mauve";
     /* case 0xFFFCBFC9: return "Pale Orchid";    TODO: duplicate case value */
        case 0xFFE5BFC6: return "Orchid";
     /* case 0xFFEFC6D3: return "Lavender";    TODO: duplicate case value */
        case 0xFFEDC4DD: return "Violet";
        case 0xFFB58CB2: return "Cachet";
        case 0xFFC9ADD8: return "Tulip";
        case 0xFFE29ED6: return "Mid Lilac";
        case 0xFF512654: return "Port Wine";
        case 0xFF512D44: return "Pro Maroon";
        case 0xFF8E47AD: return "Laurie Lilac";
        case 0xFFAF72C1: return "Iris";
        case 0xFF9B4F96: return "Raspberry";
        case 0xFF66116D: return "Mulberry";
        case 0xFF63305E: return "Plum Wine";
     /* case 0xFF1E1C77: return "Purple Twist";    TODO: duplicate case value */
        case 0xFF332875: return "Violet Blue";
        case 0xFF35006D: return "Purple Maze";
        case 0xFF2B1166: return "Pro Brite Star";
        case 0xFF38197A: return "Pro Violet";
     /* case 0xFF35006D: return "Pro Purple";    TODO: duplicate case value */
     /* case 0xFF8E47AD: return "Purple";    TODO: duplicate case value */
        case 0xFF5B027A: return "Purple Shadow";
        case 0xFF4C145E: return "Dark Purple";
        case 0xFF8977BA: return "Mauve";
        case 0xFF44235E: return "Purple Accent";
        case 0xFFAA0066: return "Hot Peony";
     /* case 0xFFCE007C: return "Passion";    TODO: duplicate case value */
     /* case 0xFFAA004F: return "Strawberry";    TODO: duplicate case value */
        case 0xFF9E2387: return "Plum";
     /* case 0xFF5E99AA: return "Misty";    TODO: duplicate case value */
        case 0xFF609191: return "Mystic Teal";
     /* case 0xFF609191: return "Teal";    TODO: duplicate case value */
     /* case 0xFF003F54: return "Dark Teal";    TODO: duplicate case value */
        case 0xFF93DDDB: return "Mint Julep";
        case 0xFF7FD6DB: return "Turquoise";
        case 0xFF007272: return "M.D. Green";
        case 0xFF70CE9B: return "Seafoam";
     /* case 0xFF70CE9B: return "Isle Green";    TODO: duplicate case value */
        case 0xFF35C4AF: return "Peppermint";
     /* case 0xFF006D75: return "Oceanic Green";    TODO: duplicate case value */
        case 0xFF006B77: return "Pro Teal";
        case 0xFF006663: return "Garden Green";
        case 0xFF006D66: return "Pro Green";
     /* case 0xFF008C82: return "Pine Green";    TODO: duplicate case value */
        case 0xFF008272: return "Greenstone";
     /* case 0xFF006663: return "Fern Green";    TODO: duplicate case value */
     /* case 0xFF006D66: return "Pro Hunter";    TODO: duplicate case value */
        case 0xFFBCC1B2: return "Palm Leaf";
        case 0xFFC6D6A0: return "Flite Green";
        case 0xFF9EAA99: return "Willow";
        case 0xFFB2D8D8: return "Sprite";
        case 0xFF7AA891: return "Moss";
     /* case 0xFF7AA891: return "Wintergreen";    TODO: duplicate case value */
        case 0xFF006056: return "Green Forest";
     /* case 0xFF006056: return "Pro Forest";    TODO: duplicate case value */
        case 0xFF4F6D5E: return "Harbor Green";
        case 0xFF024930: return "Evergreen";
        case 0xFF004438: return "Pro Dark Green";
        case 0xFF2B4C3F: return "Lizzy Lime";
        case 0xFF282D26: return "D.H. Green";
        case 0xFFF2EABC: return "Celery";
        case 0xFFCCC693: return "Pistachio";
        case 0xFF5E663A: return "Olive Drab";
        case 0xFF779182: return "Olive";
     /* case 0xFFC9E8DD: return "Pale Green";    TODO: duplicate case value */
     /* case 0xFF93DDDB: return "Green Pearl";    TODO: duplicate case value */
     /* case 0xFF93DDDB: return "Sea Mist";    TODO: duplicate case value */
        case 0xFFB5E8BF: return "Mint";
        case 0xFFAADD6D: return "Spruce";
        case 0xFFA0DB8E: return "Nile";
        case 0xFFB5CC8E: return "Green Oak";
        case 0xFF8CD600: return "Erin Green";
        case 0xFF56AA1C: return "Pro Erin";
        case 0xFF339E35: return "Emerald";
        case 0xFF007A3D: return "Dark Emerald";
     /* case 0xFF007A3D: return "Light Kelly";    TODO: duplicate case value */
     /* case 0xFF007A3D: return "Kelly";    TODO: duplicate case value */
        case 0xFF008751: return "Dark Green";
        case 0xFF006B3F: return "Fleece Green";
        case 0xFF006854: return "TH Green";
        case 0xFF1EB53A: return "Harvest Green";
        case 0xFF009E49: return "Vibrant Green";
     /* case 0xFF009E49: return "Green Grass";    TODO: duplicate case value */
     /* case 0xFF006B3F: return "Deep Green";    TODO: duplicate case value */
        case 0xFF006B54: return "Green Bay";
        case 0xFF007C66: return "Jungle Green";
        case 0xFFA3AF07: return "Peapod";
        case 0xFF7FBA00: return "Pastoral Green";
     /* case 0xFF7FBA00: return "Green Dust";    TODO: duplicate case value */
     /* case 0xFF7FBA00: return "Ming";    TODO: duplicate case value */
        case 0xFF568E14: return "Meadow";
        case 0xFF939905: return "Tamarack";
        case 0xFF566314: return "Palmetto";
     /* case 0xFF024930: return "Green Petal";    TODO: duplicate case value */
        case 0xFF547730: return "Sage";
        case 0xFF3F4926: return "Hedge";
        case 0xFF3A7728: return "Green";
        case 0xFF193833: return "Green Sail";
        case 0xFF215B33: return "Holly";
        case 0xFF265142: return "Field Green";
     /* case 0xFF3F4926: return "Dress Green";    TODO: duplicate case value */
        case 0xFF99840A: return "Foliage Green";
        case 0xFFA38205: return "Autumn Green";
        case 0xFF897719: return "Desert Cactus";
        case 0xFF707014: return "Cypress";
        case 0xFF848205: return "Crescent Moon";
        case 0xFFE2E584: return "Pebblestone";
        case 0xFF998E07: return "Sun Shadow";
        case 0xFF00494F: return "Blue Spruce";
     /* case 0xFF4F6D5E: return "Newport";    TODO: duplicate case value */
     /* case 0xFF779182: return "Spring Garden";    TODO: duplicate case value */
        case 0xFF546856: return "Water Lilly";
        case 0xFF0C3026: return "Ivy";
        case 0xFF233A2D: return "Dark Army Green";
        case 0xFF213D30: return "Army Green";
        case 0xFFC9D6A3: return "Pastel Green";
        case 0xFFE0AA0F: return "Pollen Gold";
        case 0xFFF4E287: return "Pale Yellow";
        case 0xFFFFC61E: return "Buttercup";
        case 0xFFF7E8AA: return "Tusk";
        case 0xFFF9DD16: return "Moonbeam";
        case 0xFFC6A00C: return "Black Eyed Susie";
        case 0xFFA37F14: return "Bullion";
     /* case 0xFFF7E8AA: return "Chinese Yellow";    TODO: duplicate case value */
     /* case 0xFFF7E8AA: return "Maize";    TODO: duplicate case value */
        case 0xFFEADD96: return "Wheat";
        case 0xFFF9E08C: return "Pro Maize";
        case 0xFFFFD87F: return "Glow";
        case 0xFFFCD856: return "Star Gold";
        case 0xFFFCA311: return "Mango";
     /* case 0xFFFCA311: return "Yellow Mist";    TODO: duplicate case value */
     /* case 0xFFFCA311: return "Yellow";    TODO: duplicate case value */
     /* case 0xFFF9DD16: return "Sunflower";    TODO: duplicate case value */
        case 0xFFF4ED47: return "Lemon";
        case 0xFFF9E814: return "Daffodil";
        case 0xFFFCB514: return "Merit Gold";
        case 0xFFFFCC49: return "Cornsilk";
     /* case 0xFFFFC61E: return "Nectar";    TODO: duplicate case value */
        case 0xFFFCBF49: return "Scholastic";
        case 0xFFFCE016: return "Canary Yellow";
     /* case 0xFFFFC61E: return "Pro Gold";    TODO: duplicate case value */
        case 0xFFFCD116: return "Manila";
     /* case 0xFFFCD116: return "Goldenrod";    TODO: duplicate case value */
     /* case 0xFFFFCC49: return "Brite Yellow";    TODO: duplicate case value */
        case 0xFFF99B0C: return "Honeydew";
        case 0xFFF77F00: return "Pumpkin";
        case 0xFFF74902: return "Orangeade";
     /* case 0xFFF74902: return "Sun Orange";    TODO: duplicate case value */
        case 0xFFF95602: return "Paprika";
        case 0xFFF93F26: return "Saffron";
     /* case 0xFFF95602: return "Tex Orange";    TODO: duplicate case value */
        case 0xFFF96B07: return "Orange";
        case 0xFFF96302: return "Dark Tex Orange";
        case 0xFFA53F0F: return "Old Dark Tex Orange";
        case 0xFFFC8744: return "Golden Poppy";
        case 0xFFBC4F07: return "Rust";
        case 0xFFAF7505: return "Copper";
        case 0xFFC18E60: return "Light Bronze";
        case 0xFFFCCE87: return "Visor Gold";
     /* case 0xFFFFCC49: return "Goldenlite";    TODO: duplicate case value */
        case 0xFFFCBA5E: return "Honey";
     /* case 0xFFFFCC49: return "Marigold";    TODO: duplicate case value */
     /* case 0xFFE0AA0F: return "Mustard";    TODO: duplicate case value */
     /* case 0xFFE0AA0F: return "Sun Gold";    TODO: duplicate case value */
     /* case 0xFFE0AA0F: return "Karat";    TODO: duplicate case value */
        case 0xFFF2BF49: return "Penny";
     /* case 0xFFFCCE87: return "New Gold";    TODO: duplicate case value */
        case 0xFFE2D6B5: return "Pro Beige";
     /* case 0xFFC6A00C: return "Marine Gold";    TODO: duplicate case value */
     /* case 0xFFA37F14: return "Ginger";    TODO: duplicate case value */
     /* case 0xFFC6A00C: return "Shimmering Gold";    TODO: duplicate case value */
        case 0xFFBF910C: return "Old Gold";
        case 0xFFEAB2B2: return "Salmon";
        case 0xFFC13828: return "Dark Rust";
        case 0xFFA03033: return "Terra Cotta";
     /* case 0xFFAF1E2D: return "Pro Red";    TODO: duplicate case value */
     /* case 0xFFAF1E2D: return "Auburn";    TODO: duplicate case value */
        case 0xFFFAE6CC: return "Bone";
        case 0xFFFAE6CF: return "Ivory";
        case 0xFFF7D3B5: return "Opaline";
        case 0xFFEDD3BC: return "Ecru";
        case 0xFFAA753F: return "Wicker";
        case 0xFFD3A87C: return "Tan";
        case 0xFFEDD3B5: return "Cottage Beige";
        case 0xFFC1A875: return "Rattan";
        case 0xFFE2BF9B: return "Gold";
     /* case 0xFFD3A87C: return "Mocha Cream";    TODO: duplicate case value */
     /* case 0xFFBF910C: return "Topaz";    TODO: duplicate case value */
        case 0xFFD18E54: return "Ashley Gold";
        case 0xFFD8B596: return "Amber Beige";
        case 0xFFD6CCAF: return "Seashell";
        case 0xFFF2E3C4: return "Light Maize";
     /* case 0xFFAA753F: return "Beige";    TODO: duplicate case value */
        case 0xFF6B4714: return "Sand Dune";
        case 0xFFD1BF91: return "Taupe";
        case 0xFF6C463D: return "Pro Brown";
        case 0xFF876028: return "Chocolate";
        case 0xFF755426: return "Pro Walnut";
     /* case 0xFF755426: return "Light Cocoa";    TODO: duplicate case value */
        case 0xFFB28260: return "Cocoa Mulch";
        case 0xFF593D2B: return "Brown";
     /* case 0xFF593D2B: return "Dark Brown";    TODO: duplicate case value */
        case 0xFF3F302B: return "Espresso";
     /* case 0xFFC18E60: return "Bamboo";    TODO: duplicate case value */
     /* case 0xFFAF7505: return "Almond";    TODO: duplicate case value */
        case 0xFFBA7530: return "Toast";
     /* case 0xFF755426: return "Sienna";    TODO: duplicate case value */
        case 0xFFB26B70: return "K.A. Bronze";
        case 0xFFA2464E: return "Pro Cinnamon";
        case 0xFF9B4F19: return "Date";
     /* case 0xFF9B4F19: return "Hazel";    TODO: duplicate case value */
        case 0xFF5B2D28: return "Coffee Bean";
     /* case 0xFF593D2B: return "Dogwood";    TODO: duplicate case value */
     /* case 0xFF3F302B: return "Mahogany";    TODO: duplicate case value */
        case 0xFF3D3028: return "Best Brown";
        case 0xFF633A11: return "Mushroom";
     /* case 0xFFC1A875: return "Perfect Tan";    TODO: duplicate case value */
        case 0xFF7A5B11: return "Earthen Tan";
     /* case 0xFFC1A875: return "Golden Tan";    TODO: duplicate case value */
     /* case 0xFFF2BF49: return "14 Kt. Gold";    TODO: duplicate case value */
        case 0xFFF2CE68: return "TH Gold";
        case 0xFFD88C02: return "24 Kt. Gold";
        case 0xFFC1B5A5: return "Platinum";
        case 0xFF99897C: return "Pro Gray";
        case 0xFFADA07A: return "Grayrod";
     /* case 0xFFADA07A: return "Pewter";    TODO: duplicate case value */
        case 0xFFF5E3CC: return "Aspen White";
        case 0xFF66594C: return "Dark Taupe";
        case 0xFF493533: return "Egyptian Brown";
        case 0xFFF5EBE0: return "Oyster";
     /* case 0xFFDDC6C4: return "Gray";    TODO: duplicate case value */
        case 0xFFDBD3D3: return "Pearl Gray";
        case 0xFFD8CCD1: return "Steel Gray";
        case 0xFFCCC1C6: return "Skylight";
        case 0xFFAFAAA3: return "Cloud";
     /* case 0xFFADAFAA: return "Silver Steel";    TODO: duplicate case value */
        case 0xFF919693: return "Banner Gray";
     /* case 0xFF8C8984: return "Silvery Gray";    TODO: duplicate case value */
     /* case 0xFFCCC1C6: return "Cinder";    TODO: duplicate case value */
     /* case 0xFFDBD3D3: return "Saturn Gray";    TODO: duplicate case value */
     /* case 0xFFCCC1C6: return "Dover Gray";    TODO: duplicate case value */
        case 0xFFB2A8B5: return "Storm Gray";
        case 0xFFA893AD: return "Sterling";
        case 0xFF666D70: return "Metal";
     /* case 0xFF686663: return "Twilight";    TODO: duplicate case value */
     /* case 0xFF443D38: return "Aged Charcoal";    TODO: duplicate case value */
        case 0xFF777772: return "Charcoal";
        case 0xFF353842: return "Smokey";
        case 0xFF3A4972: return "Ash";
        case 0xFF1C2630: return "Black";
     /* case 0xFFF5EBE0: return "Snow White";    TODO: duplicate case value */
        case 0xFFF5EDDE: return "Natural White";
        case 0xFFF0E8D6: return "Eggshell";
     /* case 0xFF1C2630: return "Jet Black";    TODO: duplicate case value */

        /* BEGIN N/A COLORS */
        /*
                         return "3CC Red";
                         return "3CC Rose";
                         return "3CC Apricot";
                         return "3CC Pink";
                         return "3CC Purple";
                         return "3CC Lilac";
                         return "3CC Blue";
                         return "3CC Light Blue";
                         return "3CC Turquoise";
                         return "3CC Green";
                         return "3CC Kelly";
                         return "3CC Light Green";
                         return "3CC Olive";
                         return "3CC Yellow";
                         return "3CC Maize";
                         return "3CC Tangerine";
                         return "3CC Orange";
                         return "3CC Brown";
                         return "3CC Nugget";
                         return "3CC Gold";
                         return "3CC Gray";
                         return "3CC Flag";
                         return "3CC Horizon";
                         return "3CC Autumn";
                         return "4CC Fiesta";
                         return "4CC Mexicana";
                         return "4CC Pastel";
                         return "4CC Melody";
                         return "4CC Rainbow";
        */
        /* END N/A COLORS */
    }

    return "";
}

int getNum_Sigma_Polyester(unsigned int color)
{
    switch(color)
    {
        case 0xFFFFFFFF: return   10;
        case 0xFF000000: return   20;
        case 0xFFEDFF50: return   21;
        case 0xFF96E845: return   32;
        case 0xFFFFE756: return   33;
        case 0xFFFF7824: return   43;
        case 0xFFF28DA6: return   46;
        case 0xFFC70C57: return   47;
        case 0xFFE22D2A: return  101;
        case 0xFFB8B8B8: return  102;
        case 0xFF889186: return  112;
        case 0xFF737F7F: return  115;
        case 0xFF565E5A: return  116;
        case 0xFF515250: return  117;
        case 0xFF787668: return  118;
        case 0xFFED572F: return  135;
        case 0xFF2EA59C: return  138;
        case 0xFF396276: return  142;
        case 0xFF9B3B40: return  213;
        case 0xFF6C3E47: return  216;
        case 0xFFBA6E4D: return  253;
        case 0xFFBB3D2E: return  255;
        case 0xFFF9DFCF: return  301;
        case 0xFFFBDED6: return  303;
        case 0xFFF7CDD5: return  304;
        case 0xFFF2AFB4: return  305;
        case 0xFFE8418C: return  307;
        case 0xFFE77F9D: return  309;
        case 0xFFF06F8C: return  313;
        case 0xFF008340: return  317;
        case 0xFFDF99B6: return  321;
        case 0xFF820052: return  325;
        case 0xFFB1415F: return  333;
        case 0xFFC394AE: return  345;
        case 0xFFA86E91: return  347;
        case 0xFF694169: return  348;
        case 0xFFE6CFD5: return  376;
        case 0xFFA8BED7: return  379;
        case 0xFFA0BFD7: return  380;
        case 0xFF90A6C6: return  381;
        case 0xFF8FAFC6: return  382;
        case 0xFFB1B8D3: return  383;
        case 0xFF416C9B: return  385;
        case 0xFF7D77AF: return  386;
        case 0xFFFADAF4: return  387;
        case 0xFF664090: return  390;
        case 0xFFEAF0F9: return  402;
        case 0xFFA6D8F6: return  403;
        case 0xFF7B9CB0: return  404;
        case 0xFF648DC7: return  406;
        case 0xFF3D6AA1: return  409;
        case 0xFF2D4491: return  413;
        case 0xFF143D7A: return  414;
        case 0xFF113263: return  415;
        case 0xFF0E1F38: return  423;
     /* case 0xFF0E1F38: return  432;    TODO: duplicate case value */
        case 0xFF0091A5: return  443;
        case 0xFF005B63: return  448;
        case 0xFF00474D: return  449;
        case 0xFFE5B15C: return  466;
        case 0xFFD5BF9B: return  501;
        case 0xFFFFD085: return  503;
        case 0xFFF6B08E: return  505;
        case 0xFFB3E851: return  506;
        case 0xFFF1A236: return  508;
        case 0xFF6E4337: return  513;
        case 0xFFD8493E: return  527;
        case 0xFF697698: return  541;
        case 0xFFFDE896: return  601;
        case 0xFFEDE55D: return  602;
        case 0xFFDFA200: return  609;
     /* case 0xFFFDE896: return  612;    TODO: duplicate case value */
        case 0xFFCEB24C: return  616;
        case 0xFFAD953E: return  619;
        case 0xFFFEF9EA: return  627;
        case 0xFFBD9565: return  628;
        case 0xFFFDF76C: return  632;
        case 0xFFEDEF05: return  633;
        case 0xFFF8C300: return  646;
        case 0xFFE77817: return  649;
        case 0xFFE66535: return  650;
        case 0xFFC69632: return  652;
        case 0xFF98996D: return  653;
        case 0xFFC98300: return  654;
        case 0xFF007B8D: return  688;
        case 0xFF004D3D: return  695;
        case 0xFF007EBA: return  697;
        case 0xFFCF0040: return  700;
        case 0xFF28438C: return  809;
        case 0xFFD0B478: return  812;
        case 0xFFE5BE6C: return  818;
        case 0xFF449284: return  825;
        case 0xFFCFCFCF: return  829;
        case 0xFFDC875E: return  831;
        case 0xFFB4705D: return  832;
        case 0xFF9B5C4B: return  833;
        case 0xFFA93121: return  838;
        case 0xFFB18B00: return  842;
        case 0xFF86462E: return  857;
        case 0xFF614125: return  859;
        case 0xFFB25C31: return  864;
        case 0xFF806A61: return  873;
        case 0xFF634831: return  878;
        case 0xFF1A0C06: return  891;
        case 0xFF96D5C8: return  903;
        case 0xFFB4DCD8: return  904;
        case 0xFFAF7D3E: return  905;
        case 0xFF00A3A0: return  906;
        case 0xFF00405D: return  913;
        case 0xFFC9E3C5: return  947;
        case 0xFF55AF78: return  949;
        case 0xFF858325: return  951;
        case 0xFF61601C: return  955;
        case 0xFF709188: return  961;
        case 0xFFBEDC8C: return  984;
        case 0xFFBEE678: return  985;
        case 0xFF76C850: return  988;
        case 0xFF466E64: return  448;
        case 0xFF356936: return  992;
        case 0xFF4B4884: return 1031;
        case 0xFFEDEDD2: return 1140;
        case 0xFFF3D8A8: return 1145;
        case 0xFFC8BE96: return 1148;
        case 0xFF243A7D: return 1163;
        case 0xFF86BE4E: return 1183;
        case 0xFF8E4044: return 1241;
        case 0xFF893480: return 1323;
        case 0xFF8C6DAA: return 1324;
        case 0xFFB6A36C: return 1552;
        case 0xFF2E9F76: return 1615;
        case 0xFF98C173: return 1619;
        case 0xFFCDCDCD: return 1707;
        case 0xFF2A377E: return 2031;
        case 0xFF006CA5: return 2093;
        case 0xFF834455: return 2250;
        case 0xFFD0A44F: return 2518;
        case 0xFFED9206: return 2519;
     /* case 0xFFEDEF05: return 3001;    TODO: duplicate case value */
        case 0xFFC07A46: return 3014;
        case 0xFFB43C3C: return 3015;
        case 0xFF915F46: return 3142;
        case 0xFFFFC500: return 4117;
        case 0xFFA68A68: return 4371;
        case 0xFF00A4D9: return 4419;
        case 0xFF0B7F85: return 4627;
        case 0xFF002D1F: return 4735;
        case 0xFF11263C: return 5552;
        case 0xFF91B9E2: return 5554;
        case 0xFF429648: return 5557;
        case 0xFF878C8C: return 8010;
    }

    return -1;
}
const char* getName_Sigma_Polyester(unsigned int color)
{
    switch(color)
    {
        case 0xFFFFFFFF: return "White";
        case 0xFF000000: return "Black";
        case 0xFFEDFF50: return "Light Neon Green";
        case 0xFF96E845: return "Neon Green";
        case 0xFFFFE756: return "Light Neon Orange";
        case 0xFFFF7824: return "Med Neon Orange";
        case 0xFFF28DA6: return "Neon Pink";
        case 0xFFC70C57: return "Neon Orange Pink";
        case 0xFFE22D2A: return "Silver";
        case 0xFFB8B8B8: return "Silver Diamond";
        case 0xFF889186: return "Lava Stone";
        case 0xFF737F7F: return "Medium Grey";
        case 0xFF565E5A: return "Dark Platinum";
        case 0xFF515250: return "Charcoal";
        case 0xFF787668: return "Badger Grey";
        case 0xFFED572F: return "Pumpkin Orange";
        case 0xFF2EA59C: return "Turquoise";
        case 0xFF396276: return "Dark Wedgewood";
        case 0xFF9B3B40: return "Cardinal Red";
        case 0xFF6C3E47: return "Maroon";
        case 0xFFBA6E4D: return "Rust";
        case 0xFFBB3D2E: return "Medium Rust";
        case 0xFFF9DFCF: return "Natural Pink";
        case 0xFFFBDED6: return "Baby Pink";
        case 0xFFF7CDD5: return "Piggy Pink";
        case 0xFFF2AFB4: return "Sweet Pink";
        case 0xFFE8418C: return "Blushing Pink";
        case 0xFFE77F9D: return "Pink";
        case 0xFFF06F8C: return "Rose Pink";
        case 0xFF008340: return "Green";
        case 0xFFDF99B6: return "Shocking Pink";
        case 0xFF820052: return "Ruby";
        case 0xFFB1415F: return "Garnet";
        case 0xFFC394AE: return "Light Purple";
        case 0xFFA86E91: return "Medium Purple";
        case 0xFF694169: return "Dark Grape";
        case 0xFFE6CFD5: return "Pastel Light Pink";
        case 0xFFA8BED7: return "Light Baby Blue";
        case 0xFFA0BFD7: return "Crystal Blue";
        case 0xFF90A6C6: return "Very Light Lavender";
        case 0xFF8FAFC6: return "Cornflower";
        case 0xFFB1B8D3: return "Lavender";
        case 0xFF416C9B: return "Denim";
        case 0xFF7D77AF: return "Light Violet";
        case 0xFFFADAF4: return "Misty Rose";
        case 0xFF664090: return "Grape";
        case 0xFFEAF0F9: return "Lt. Weathered Blue";
        case 0xFFA6D8F6: return "Baby Blue";
        case 0xFF7B9CB0: return "Med Baby Blue";
        case 0xFF648DC7: return "Med Pastel Blue";
        case 0xFF3D6AA1: return "Blue Raspberry";
        case 0xFF2D4491: return "Med Royal Blue";
        case 0xFF143D7A: return "Ocean Blue";
        case 0xFF113263: return "Med Navy";
        case 0xFF0E1F38: return "Dark Navy";
     /* case 0xFF0E1F38: return "Bright Sunshine";    TODO: duplicate case value */
        case 0xFF0091A5: return "Teal";
        case 0xFF005B63: return "Deep Teal";
        case 0xFF00474D: return "Dark Teal";
        case 0xFFE5B15C: return "Old Gold";
        case 0xFFD5BF9B: return "Cream";
        case 0xFFFFD085: return "Pale Salmon";
        case 0xFFF6B08E: return "Med Peach";
        case 0xFFB3E851: return "Pink Salmon";
        case 0xFFF1A236: return "Dark Peach";
        case 0xFF6E4337: return "Dark Brown";
        case 0xFFD8493E: return "Pale Red";
        case 0xFF697698: return "Heron Blue";
        case 0xFFFDE896: return "Pale Yellow";
        case 0xFFEDE55D: return "Pastel Yellow";
        case 0xFFDFA200: return "Golden Puppy";
     /* case 0xFFFDE896: return "Buttercup";    TODO: duplicate case value */
        case 0xFFCEB24C: return "Treasure Gold";
        case 0xFFAD953E: return "Old Gold";
        case 0xFFFEF9EA: return "Pale Apricot";
        case 0xFFBD9565: return "Tan";
        case 0xFFFDF76C: return "Mellow Yellow";
        case 0xFFEDEF05: return "Lemon";
        case 0xFFF8C300: return "Amber";
        case 0xFFE77817: return "Mandarina";
        case 0xFFE66535: return "Orange";
        case 0xFFC69632: return "Golden Rod";
        case 0xFF98996D: return "Light Olive";
        case 0xFFC98300: return "Bright Gold";
        case 0xFF007B8D: return "Blue-Green";
        case 0xFF004D3D: return "Forrest Green";
        case 0xFF007EBA: return "Midnight Blue";
        case 0xFFCF0040: return "Med Red";
        case 0xFF28438C: return "Med Blue";
        case 0xFFD0B478: return "Sweet Apricot";
        case 0xFFE5BE6C: return "Skin";
        case 0xFF449284: return "Jade";
        case 0xFFCFCFCF: return "Light Silver";
        case 0xFFDC875E: return "Papaya Whip";
        case 0xFFB4705D: return "Cooper";
        case 0xFF9B5C4B: return "Light Pecan";
        case 0xFFA93121: return "Burnt Rust";
        case 0xFFB18B00: return "Vegas Gold";
        case 0xFF86462E: return "Med Brown";
        case 0xFF614125: return "Med Russett";
        case 0xFFB25C31: return "Med Copper";
        case 0xFF806A61: return "Dark Driftwood";
        case 0xFF634831: return "Birch";
        case 0xFF1A0C06: return "Dark Chocolate";
        case 0xFF96D5C8: return "Sky Blue 2";
        case 0xFFB4DCD8: return "Aquamarine";
        case 0xFFAF7D3E: return "Golden Brown";
        case 0xFF00A3A0: return "Sea Blue";
        case 0xFF00405D: return "Deep Sea";
        case 0xFFC9E3C5: return "Pastel Mint";
        case 0xFF55AF78: return "True Green";
        case 0xFF858325: return "Med Olive";
        case 0xFF61601C: return "Olive";
        case 0xFF709188: return "Light Jade";
        case 0xFFBEDC8C: return "Smith Apple";
        case 0xFFBEE678: return "Light Lime";
        case 0xFF76C850: return "Grass Green";
        case 0xFF466E64: return "Deep Teal";
        case 0xFF356936: return "Med Forrest Green";
        case 0xFF4B4884: return "Deep Violet";
        case 0xFFEDEDD2: return "Light Natural";
        case 0xFFF3D8A8: return "Wheat";
        case 0xFFC8BE96: return "Desert Sand";
        case 0xFF243A7D: return "Egyptian Blue";
        case 0xFF86BE4E: return "Gecko";
        case 0xFF8E4044: return "Burgandy";
        case 0xFF893480: return "Med Orchid";
        case 0xFF8C6DAA: return "Med Purple";
        case 0xFFB6A36C: return "Very Old Gold";
        case 0xFF2E9F76: return "Light Spruce";
        case 0xFF98C173: return "Paris Green";
        case 0xFFCDCDCD: return "Timberwolf";
        case 0xFF2A377E: return "Bright Blue";
        case 0xFF006CA5: return "Turquoise Blue";
        case 0xFF834455: return "Dark Wine";
        case 0xFFD0A44F: return "Beige";
        case 0xFFED9206: return "Gold";
     /* case 0xFFEDEF05: return "Med Orange";    TODO: duplicate case value */
        case 0xFFC07A46: return "Dark Salmon";
        case 0xFFB43C3C: return "Fire Red";
        case 0xFF915F46: return "Saddle Brown";
        case 0xFFFFC500: return "Yellow Sun";
        case 0xFFA68A68: return "Deep Taupe";
        case 0xFF00A4D9: return "Sky Blue";
        case 0xFF0B7F85: return "Wild Peacock";
        case 0xFF002D1F: return "Millard Green";
        case 0xFF11263C: return "Dark Blue";
        case 0xFF91B9E2: return "Powder Blue";
        case 0xFF429648: return "Froggy Green";
        case 0xFF878C8C: return "Stone Grey";
    }

    return "";
}

int getNum_Sulky_Rayon(unsigned int color)
{
    switch(color)
    {
        case 0xFFEFC810: return  502;
        case 0xFF0C082D: return  505;
        case 0xFFB26C29: return  521;
        case 0xFFE79002: return  523;
        case 0xFF34481E: return  525;
        case 0xFF113675: return  526;
        case 0xFF111408: return  538;
        case 0xFFE10000: return  561;
        case 0xFFFFB435: return  562;
        case 0xFFF3A001: return  567;
        case 0xFFE66D00: return  568;
        case 0xFF165F28: return  569;
        case 0xFF088E6C: return  571;
        case 0xFF100A7C: return  572;
        case 0xFF35693D: return  580;
        case 0xFFE9BD96: return  619;
        case 0xFFCD3900: return  621;
        case 0xFF777113: return  630;
        case 0xFF1C6F51: return  640;
        case 0xFF262345: return  643;
        case 0xFFF9F9FF: return 1001;
        case 0xFFF9F9F4: return 1002;
        case 0xFF000000: return 1005;
        case 0xFFB7A9AC: return 1011;
        case 0xFFE1AF9A: return 1015;
        case 0xFFEC968C: return 1016;
        case 0xFFEFDFBD: return 1017;
        case 0xFFECA082: return 1019;
        case 0xFFF08278: return 1020;
        case 0xFFEB6602: return 1021;
        case 0xFFFFF7D5: return 1022;
        case 0xFFFFE669: return 1023;
        case 0xFFFFB800: return 1024;
        case 0xFFD78000: return 1025;
        case 0xFFBEC3E1: return 1028;
        case 0xFFA0C3EB: return 1029;
        case 0xFFA6A2C6: return 1030;
        case 0xFFDFBEC8: return 1031;
        case 0xFFE68CEB: return 1032;
        case 0xFFD86496: return 1033;
        case 0xFFC6323C: return 1034;
        case 0xFF790000: return 1035;
        case 0xFFF90000: return 1037;
        case 0xFFEB0000: return 1039;
        case 0xFF877375: return 1040;
        case 0xFF8C7F83: return 1041;
        case 0xFF321E50: return 1042;
        case 0xFF190525: return 1043;
        case 0xFF1D062F: return 1044;
        case 0xFFC3EFBF: return 1045;
        case 0xFF2E8359: return 1046;
        case 0xFFA6C284: return 1047;
        case 0xFF42A021: return 1049;
        case 0xFF1E6419: return 1051;
        case 0xFFEEBEAE: return 1054;
        case 0xFFEBBC80: return 1055;
        case 0xFFAF5B00: return 1056;
        case 0xFF642702: return 1057;
        case 0xFF663500: return 1058;
        case 0xFF530601: return 1059;
        case 0xFFFFF7B9: return 1061;
        case 0xFFF0F8EC: return 1063;
        case 0xFFE6B4AA: return 1064;
        case 0xFFFF9100: return 1065;
        case 0xFFFFF180: return 1066;
        case 0xFFFFFF85: return 1067;
        case 0xFFF3DBD9: return 1068;
        case 0xFFF6CE69: return 1070;
        case 0xFFF9F9EA: return 1071;
        case 0xFFD6D5E8: return 1074;
        case 0xFF5A5A8B: return 1076;
        case 0xFFBECDC8: return 1077;
        case 0xFFFF6600: return 1078;
        case 0xFF175523: return 1079;
        case 0xFFDC82A0: return 1080;
        case 0xFFF06E78: return 1081;
        case 0xFFF7E3BB: return 1082;
        case 0xFFFFC100: return 1083;
        case 0xFFE2CFC7: return 1085;
        case 0xFFF9F9E0: return 1086;
        case 0xFF16625F: return 1090;
        case 0xFF26BFCA: return 1094;
        case 0xFF10D1BD: return 1095;
        case 0xFF0F6978: return 1096;
        case 0xFFC2D37D: return 1100;
        case 0xFF098531: return 1101;
        case 0xFF02140F: return 1103;
        case 0xFFA5AF68: return 1104;
        case 0xFFFAA4A4: return 1108;
        case 0xFFDC6496: return 1109;
        case 0xFFFCCBDF: return 1111;
        case 0xFF46016E: return 1112;
        case 0xFFF0C8B4: return 1113;
        case 0xFFF0B9B9: return 1115;
        case 0xFFF5A9A0: return 1117;
        case 0xFFB46E75: return 1119;
        case 0xFFF0D6D2: return 1120;
        case 0xFFFAB9CB: return 1121;
        case 0xFF82288E: return 1122;
        case 0xFFFFEC00: return 1124;
        case 0xFFDC8C17: return 1126;
        case 0xFFFAECC6: return 1127;
        case 0xFFC39471: return 1128;
        case 0xFF6A1F06: return 1129;
        case 0xFF551602: return 1130;
        case 0xFF490002: return 1131;
        case 0xFF507DAA: return 1134;
        case 0xFFFFF072: return 1135;
        case 0xFFFFBE00: return 1137;
        case 0xFF4A5870: return 1143;
        case 0xFFB4E1EB: return 1145;
     /* case 0xFFEB0000: return 1147;    TODO: duplicate case value */
        case 0xFFFFBDBD: return 1148;
        case 0xFFE8C89C: return 1149;
        case 0xFFE2E2EB: return 1151;
        case 0xFFFA9999: return 1154;
        case 0xFF636327: return 1156;
        case 0xFFBA4500: return 1158;
        case 0xFFD39D00: return 1159;
        case 0xFF10394A: return 1162;
        case 0xFFDFE5EB: return 1165;
        case 0xFF8E7E7E: return 1166;
        case 0xFFFFD226: return 1167;
        case 0xFFF5740A: return 1168;
        case 0xFF9C0000: return 1169;
        case 0xFF975F2F: return 1170;
        case 0xFF08180E: return 1171;
        case 0xFF6E788C: return 1172;
        case 0xFF59591D: return 1173;
        case 0xFF0D2904: return 1174;
        case 0xFF152D04: return 1175;
        case 0xFF515308: return 1176;
        case 0xFF899812: return 1177;
        case 0xFF8F623D: return 1179;
        case 0xFFA58973: return 1180;
        case 0xFFCB0000: return 1181;
        case 0xFF020114: return 1182;
        case 0xFF320614: return 1183;
     /* case 0xFFFF6600: return 1184;    TODO: duplicate case value */
        case 0xFFFCBE05: return 1185;
        case 0xFF5B0000: return 1186;
        case 0xFFFFE500: return 1187;
        case 0xFFFF004B: return 1188;
        case 0xFF4B122D: return 1189;
        case 0xFFA04656: return 1190;
        case 0xFFBD1E60: return 1191;
        case 0xFFD21E82: return 1192;
        case 0xFFE6AFD2: return 1193;
        case 0xFFD274D7: return 1194;
        case 0xFF370150: return 1195;
        case 0xFF96C3E1: return 1196;
        case 0xFF220F34: return 1197;
        case 0xFF3C5075: return 1198;
        case 0xFF2A143F: return 1199;
        case 0xFF140B2D: return 1200;
        case 0xFF648BBE: return 1201;
        case 0xFF182B56: return 1202;
        case 0xFFAEB8C3: return 1203;
        case 0xFFA8C8BC: return 1204;
        case 0xFF6E90A5: return 1205;
        case 0xFF1E6E6F: return 1206;
        case 0xFF80A388: return 1207;
        case 0xFF0C3D03: return 1208;
        case 0xFFBDD163: return 1209;
        case 0xFF273B00: return 1210;
        case 0xFF95A490: return 1211;
        case 0xFF63632D: return 1212;
        case 0xFFB9A096: return 1213;
        case 0xFF642828: return 1214;
        case 0xFF500A1E: return 1215;
        case 0xFFAC1C01: return 1216;
        case 0xFF971F01: return 1217;
        case 0xFFDFDFCB: return 1218;
        case 0xFF98888C: return 1219;
        case 0xFF765960: return 1220;
        case 0xFFD1DBFF: return 1222;
        case 0xFFDCE0F1: return 1223;
        case 0xFFF0A0B9: return 1224;
        case 0xFFFACBCB: return 1225;
        case 0xFF57369E: return 1226;
        case 0xFFAF8901: return 1227;
        case 0xFF96AA8B: return 1228;
        case 0xFFE0DBDB: return 1229;
        case 0xFF0B4133: return 1230;
        case 0xFFE5326A: return 1231;
        case 0xFF193207: return 1232;
        case 0xFF0D2210: return 1233;
        case 0xFF3C1B1F: return 1234;
        case 0xFF783298: return 1235;
        case 0xFFEAE4E4: return 1236;
        case 0xFFBC3D2C: return 1237;
        case 0xFFFF8300: return 1238;
        case 0xFFFFAB57: return 1239;
        case 0xFF74586C: return 1240;
        case 0xFF543A8D: return 1242;
        case 0xFFFF0000: return 1246;
        case 0xFF660000: return 1247;
        case 0xFFD2E6F0: return 1248;
        case 0xFF62AADC: return 1249;
        case 0xFF275C70: return 1250;
        case 0xFF306F75: return 1251;
        case 0xFF09A1A8: return 1252;
        case 0xFF1B4CA4: return 1253;
        case 0xFFE6B9F5: return 1254;
        case 0xFFBE1982: return 1255;
        case 0xFFEB8296: return 1256;
        case 0xFFE60041: return 1257;
        case 0xFFF0C4A0: return 1258;
        case 0xFFE28264: return 1259;
        case 0xFFB30000: return 1263;
        case 0xFF6A0000: return 1264;
        case 0xFF9B6B2C: return 1265;
        case 0xFF9C6D45: return 1266;
        case 0xFF864C31: return 1267;
        case 0xFFEFEFE5: return 1268;
        case 0xFFB7B7AF: return 1270;
        case 0xFF3C4F31: return 1271;
        case 0xFF4A4A19: return 1272;
        case 0xFF5C9A1A: return 1274;
        case 0xFFE0E6C8: return 1275;
        case 0xFF70770F: return 1276;
        case 0xFF027602: return 1277;
        case 0xFF00AF38: return 1278;
        case 0xFF93D16C: return 1279;
        case 0xFF46B774: return 1280;
        case 0xFF483D59: return 1283;
        case 0xFF466E78: return 1284;
        case 0xFF134F45: return 1285;
        case 0xFF343213: return 1286;
        case 0xFF415545: return 1287;
        case 0xFF0FA56F: return 1288;
        case 0xFFDCEBF0: return 1289;
        case 0xFF727483: return 1291;
        case 0xFFD1DCFA: return 1292;
        case 0xFF44235D: return 1293;
        case 0xFF412044: return 1294;
        case 0xFF82878C: return 1295;
        case 0xFFD2AAF0: return 1296;
        case 0xFF735A64: return 1297;
        case 0xFF644664: return 1298;
        case 0xFF411446: return 1299;
        case 0xFF7E1E46: return 1300;
        case 0xFF320046: return 1301;
        case 0xFF6E0A96: return 1302;
        case 0xFFB47364: return 1304;
        case 0xFFAEC6BB: return 1305;
        case 0xFF7E6C7C: return 1306;
        case 0xFFDB6478: return 1307;
        case 0xFF782346: return 1309;
        case 0xFF961A32: return 1311;
        case 0xFF840000: return 1312;
        case 0xFFFC8F0C: return 1313;
     /* case 0xFFFF0000: return 1317;    TODO: duplicate case value */
        case 0xFFCBCBBD: return 1321;
        case 0xFF818901: return 1322;
        case 0xFFF8F5F1: return 1325;
        case 0xFFD5C7C3: return 1327;
        case 0xFFC0B2B7: return 1328;
        case 0xFFABA0A8: return 1329;
        case 0xFFEDF6D4: return 1331;
        case 0xFF868105: return 1332;
        case 0xFFF3B600: return 1333;
        case 0xFF349669: return 1503;
        case 0xFFC1CBB9: return 1508;
        case 0xFF7AB31D: return 1510;
        case 0xFFEE5078: return 1511;
        case 0xFF007A67: return 1513;
        case 0xFFFF8CCB: return 1515;
        case 0xFF014F3A: return 1517;
        case 0xFFCD054D: return 1533;
        case 0xFF347DCB: return 1534;
        case 0xFF23238B: return 1535;
        case 0xFF081705: return 1536;
        case 0xFFFFD6C7: return 1543;
        case 0xFF9C6484: return 1545;
        case 0xFFE6AE6F: return 1549;
        case 0xFF6C8E87: return 1550;
        case 0xFF80B0AE: return 1551;
        case 0xFF6C7C71: return 1552;
        case 0xFF8C748C: return 1554;
        case 0xFFEB7183: return 1558;
        case 0xFF68E0F8: return 1560;
        case 0xFFB58CC7: return 1561;
        /* BEGIN N/A COLORS */
        /*
                         return 2100;
                         return 2101;
                         return 2102;
                         return 2103;
                         return 2104;
                         return 2105;
                         return 2106;
                         return 2107;
                         return 2108;
                         return 2109;
                         return 2110;
                         return 2111;
                         return 2112;
                         return 2113;
                         return 2114;
                         return 2115;
                         return 2116;
                         return 2117;
                         return 2118;
                         return 2119;
                         return 2120;
                         return 2121;
                         return 2122;
                         return 2123;
                         return 2124;
                         return 2125;
                         return 2126;
                         return 2127;
                         return 2128;
                         return 2129;
                         return 2130;
                         return 2131;
                         return 2132;
                         return 2133;
                         return 2134;
                         return 2135;
                         return 2201;
                         return 2202;
                         return 2203;
                         return 2204;
                         return 2205;
                         return 2206;
                         return 2207;
                         return 2208;
                         return 2209;
                         return 2210;
                         return 2240;
                         return 2241;
                         return 2242;
                         return 2243;
                         return 2244;
                         return 2245;
                         return 2246;
                         return 2247;
        */
        /* END N/A COLORS */
        case 0xFFFAD2AA: return 1800;
        case 0xFFFADC96: return 1801;
        case 0xFFFFC896: return 1802;
        case 0xFFFF9B6E: return 1803;
        case 0xFF375A73: return 1804;
        case 0xFF28505A: return 1805;
        case 0xFFA0B9C3: return 1806;
        case 0xFFB49682: return 1807;
        case 0xFFD2AF9B: return 1808;
        case 0xFFA07D82: return 1809;
        case 0xFF645055: return 1810;
        case 0xFF3C2837: return 1811;
        case 0xFF6E2D5A: return 1812;
        case 0xFF6E2D41: return 1813;
        case 0xFFAF4B69: return 1814;
        case 0xFF91B432: return 1815;
        case 0xFFD7F58C: return 1816;
        case 0xFFAAAF14: return 1817;
        case 0xFFC8F58C: return 1818;
        case 0xFFC3913C: return 1819;
        case 0xFFC38C73: return 1820;
        case 0xFFFAC896: return 1821;
        case 0xFF965A37: return 1822;
        case 0xFF965A28: return 1823;
        case 0xFFD2C3AF: return 1824;
        case 0xFF5F9619: return 1825;
        case 0xFFAA820A: return 1826;
        case 0xFFFF643C: return 1827;
        case 0xFFFFE6AA: return 1828;
        case 0xFFF0EBA5: return 1829;
        case 0xFFB47396: return 1830;
        case 0xFF91E12D: return 1831;
        case 0xFFE19119: return 1832;
        case 0xFFD25F00: return 1833;
        case 0xFFAFAA05: return 1834;
        case 0xFF6E8205: return 1835;
        case 0xFF3C4B05: return 1836;
        case 0xFF9B735A: return 1837;
        case 0xFFCDAA7D: return 1838;
        case 0xFF87C887: return 1839;
        case 0xFFF9E6CA: return  508;
        case 0xFFFDF3DA: return  520;
        case 0xFF5D3446: return 1241;
        case 0xFFE0C63B: return 1243;
        case 0xFFDDAB00: return 1260;
        case 0xFFA98803: return 1262;
        case 0xFFAC8783: return 1269;
        case 0xFF36361F: return 1273;
        case 0xFFFA5F7F: return 1303;
        case 0xFFA3C2D7: return 1644;
    }

    return -1;
}

const char* getName_Sulky_Rayon(unsigned int color)
{
    switch(color)
    {
        case 0xFFEFC810: return "Cornsilk";
        case 0xFF0C082D: return "Deep Arctic Sky";
        case 0xFFB26C29: return "Nutmeg";
        case 0xFFE79002: return "Autumn Gold";
        case 0xFF34481E: return "English Green";
        case 0xFF113675: return "Cobalt Blue";
        case 0xFF111408: return "Forest Green";
        case 0xFFE10000: return "Lipstick";
        case 0xFFFFB435: return "Spice";
        case 0xFFF3A001: return "Butterfly Gold";
        case 0xFFE66D00: return "Cinnamon";
        case 0xFF165F28: return "Garden Green";
        case 0xFF088E6C: return "Deep Aqua";
        case 0xFF100A7C: return "Blue Ribbon";
        case 0xFF35693D: return "Mint Julep";
        case 0xFFE9BD96: return "Dusty Peach";
        case 0xFFCD3900: return "Sunset";
        case 0xFF777113: return "Moss Green";
        case 0xFF1C6F51: return "Med. Aqua";
        case 0xFF262345: return "Arctic Sky";
        case 0xFFF9F9FF: return "Bright White";
        case 0xFFF9F9F4: return "Soft White";
        case 0xFF000000: return "Black";
        case 0xFFB7A9AC: return "Steel Gray";
        case 0xFFE1AF9A: return "Med. Peach";
        case 0xFFEC968C: return "Pastel Coral";
        case 0xFFEFDFBD: return "Pastel Peach";
        case 0xFFECA082: return "Peach";
        case 0xFFF08278: return "Dark Peach";
        case 0xFFEB6602: return "Maple";
        case 0xFFFFF7D5: return "Cream";
        case 0xFFFFE669: return "Yellow";
        case 0xFFFFB800: return "Goldenrod";
        case 0xFFD78000: return "Mine Gold";
        case 0xFFBEC3E1: return "Baby Blue";
        case 0xFFA0C3EB: return "Med. Blue";
        case 0xFFA6A2C6: return "Periwinkle";
        case 0xFFDFBEC8: return "Med. Orchid";
        case 0xFFE68CEB: return "Med. Purple";
        case 0xFFD86496: return "Dk. Orchid";
        case 0xFFC6323C: return "Burgundy";
        case 0xFF790000: return "Dk. Burgundy";
        case 0xFFF90000: return "Lt.Red";
        case 0xFFEB0000: return "True Red";
        case 0xFF877375: return "Med. Dk. Khaki";
        case 0xFF8C7F83: return "Med. Dk. Gray";
        case 0xFF321E50: return "Bright Navy Blue";
        case 0xFF190525: return "Dk. Navy";
        case 0xFF1D062F: return "Midnight Blue";
        case 0xFFC3EFBF: return "Lt. Teal";
        case 0xFF2E8359: return "Teal";
        case 0xFFA6C284: return "Mint Green";
        case 0xFF42A021: return "Grass Green";
        case 0xFF1E6419: return "Xmas Green";
        case 0xFFEEBEAE: return "Med. Dk. Ecru";
        case 0xFFEBBC80: return "Tawny Tan";
        case 0xFFAF5B00: return "Med Tawny Tan";
        case 0xFF642702: return "Dk Tawny Tan";
        case 0xFF663500: return "Tawny Brown";
        case 0xFF530601: return "Dk Tawny Brown";
        case 0xFFFFF7B9: return "Pale Yellow";
        case 0xFFF0F8EC: return "Pale Yellow-Green";
        case 0xFFE6B4AA: return "Pale Peach";
        case 0xFFFF9100: return "Orange Yellow";
        case 0xFFFFF180: return "Primrose";
        case 0xFFFFFF85: return "Lemon Yellow";
        case 0xFFF3DBD9: return "Pink Tint";
        case 0xFFF6CE69: return "Gold";
        case 0xFFF9F9EA: return "Off White";
        case 0xFFD6D5E8: return "Pale Powder Blue";
        case 0xFF5A5A8B: return "Royal Blue";
        case 0xFFBECDC8: return "Jade Tint";
        case 0xFFFF6600: return "Tangerine";
        case 0xFF175523: return "Emerald Green";
        case 0xFFDC82A0: return "Orchid";
        case 0xFFF06E78: return "Brick";
        case 0xFFF7E3BB: return "Ecru";
        case 0xFFFFC100: return "Spark Gold";
        case 0xFFE2CFC7: return "Silver";
        case 0xFFF9F9E0: return "Pale Sea Foam";
        case 0xFF16625F: return "Deep Peacock";
        case 0xFF26BFCA: return "Med Turquoise";
        case 0xFF10D1BD: return "Turquoise";
        case 0xFF0F6978: return "Dk Turquoise";
        case 0xFFC2D37D: return "Lt Grass Green";
        case 0xFF098531: return "True Green";
        case 0xFF02140F: return "Dk Khaki";
        case 0xFFA5AF68: return "Pastel Yellow-Grn";
        case 0xFFFAA4A4: return "Lt Mauve";
        case 0xFFDC6496: return "Hot Pink";
        case 0xFFFCCBDF: return "Pastel Orchid";
        case 0xFF46016E: return "Royal Purple";
        case 0xFFF0C8B4: return "Pastel Mauve";
        case 0xFFF0B9B9: return "Lt Pink";
        case 0xFFF5A9A0: return "Mauve";
        case 0xFFB46E75: return "Dk Mauve";
        case 0xFFF0D6D2: return "Pale Pink";
        case 0xFFFAB9CB: return "Pink";
        case 0xFF82288E: return "Purple";
        case 0xFFFFEC00: return "Sun Yellow";
        case 0xFFDC8C17: return "Tan";
        case 0xFFFAECC6: return "Med Ecru";
        case 0xFFC39471: return "Dk Ecru";
        case 0xFF6A1F06: return "Brown";
        case 0xFF551602: return "Dark Brown";
        case 0xFF490002: return "Cloister Brown";
        case 0xFF507DAA: return "Peacock Blue";
        case 0xFFFFF072: return "Pastel Yellow";
        case 0xFFFFBE00: return "Yellow Orange";
        case 0xFF4A5870: return "True Blue";
        case 0xFFB4E1EB: return "Powder Blue";
     /* case 0xFFEB0000: return "Xmas Red";    TODO: duplicate case value */
        case 0xFFFFBDBD: return "Lt Coral";
        case 0xFFE8C89C: return "Deep Ecru";
        case 0xFFE2E2EB: return "Powder Blue Tint";
        case 0xFFFA9999: return "Coral";
        case 0xFF636327: return "Lt Army Green";
        case 0xFFBA4500: return "Dk Maple";
        case 0xFFD39D00: return "Temple Gold";
        case 0xFF10394A: return "Deep Teal";
        case 0xFFDFE5EB: return "Lt Sky Blue";
        case 0xFF8E7E7E: return "Med Steel Gray";
        case 0xFFFFD226: return "Maize Yellow";
        case 0xFFF5740A: return "True Orange";
        case 0xFF9C0000: return "Bayberry Red";
        case 0xFF975F2F: return "Lt Brown";
        case 0xFF08180E: return "Weathered Blue";
        case 0xFF6E788C: return "Med Weathered Blue";
        case 0xFF59591D: return "Med Army Green";
        case 0xFF0D2904: return "Dk Pine Green";
        case 0xFF152D04: return "Dk Avocado";
        case 0xFF515308: return "Med Dk Avocado";
        case 0xFF899812: return "Avocado";
        case 0xFF8F623D: return "Dk Taupe";
        case 0xFFA58973: return "Med Taupe";
        case 0xFFCB0000: return "Rust";
        case 0xFF020114: return "Blue Black";
        case 0xFF320614: return "Black Cherry";
     /* case 0xFFFF6600: return "Orange Red";    TODO: duplicate case value */
        case 0xFFFCBE05: return "Golden Yellow";
        case 0xFF5B0000: return "Sable Brown";
        case 0xFFFFE500: return "Mimosa Yellow";
        case 0xFFFF004B: return "Red Geranium";
        case 0xFF4B122D: return "Dk Chestnut";
        case 0xFFA04656: return "Med Burgundy";
        case 0xFFBD1E60: return "Dk Rose";
        case 0xFFD21E82: return "Fuchsia";
        case 0xFFE6AFD2: return "Lavender";
        case 0xFFD274D7: return "Lt Purple";
        case 0xFF370150: return "Dk Purple";
        case 0xFF96C3E1: return "Blue";
        case 0xFF220F34: return "Med Navy";
        case 0xFF3C5075: return "Dusty Navy";
        case 0xFF2A143F: return "Admiral Navy Blue";
        case 0xFF140B2D: return "Med Dk Navy";
        case 0xFF648BBE: return "Med Powder Blue";
        case 0xFF182B56: return "Deep Turquoise";
        case 0xFFAEB8C3: return "Lt Weathered Blue";
        case 0xFFA8C8BC: return "Pastel Jade";
        case 0xFF6E90A5: return "Med Jade";
        case 0xFF1E6E6F: return "Dark Jade";
        case 0xFF80A388: return "Sea Foam Green";
        case 0xFF0C3D03: return "Mallard Green";
        case 0xFFBDD163: return "Lt Avocado";
        case 0xFF273B00: return "Dk Army Green";
        case 0xFF95A490: return "Lt Khaki";
        case 0xFF63632D: return "Khaki";
        case 0xFFB9A096: return "Taupe";
        case 0xFF642828: return "Med Chestnut";
        case 0xFF500A1E: return "Blackberry";
        case 0xFFAC1C01: return "Med Maple";
        case 0xFF971F01: return "Chestnut";
        case 0xFFDFDFCB: return "Silver Gray";
        case 0xFF98888C: return "Gray";
        case 0xFF765960: return "Charcoal Gray";
        case 0xFFD1DBFF: return "Lt Baby Blue";
        case 0xFFDCE0F1: return "Baby Blue Tint";
        case 0xFFF0A0B9: return "Bright Pink";
        case 0xFFFACBCB: return "Pastel Pink";
        case 0xFF57369E: return "Dkl Periwinkle";
        case 0xFFAF8901: return "Gold Green";
        case 0xFF96AA8B: return "Drab Green";
        case 0xFFE0DBDB: return "Lt Putty";
        case 0xFF0B4133: return "Dk Teal";
        case 0xFFE5326A: return "Med Rose";
        case 0xFF193207: return "Classic Green";
        case 0xFF0D2210: return "Ocean Teal";
        case 0xFF3C1B1F: return "Almost Black";
        case 0xFF783298: return "Deep Purple";
        case 0xFFEAE4E4: return "Lt Silver";
        case 0xFFBC3D2C: return "Deep Mauve";
        case 0xFFFF8300: return "Orange Sunrise";
        case 0xFFFFAB57: return "Apricot";
        case 0xFF74586C: return "Smokey Grey";
        case 0xFF543A8D: return "Nassau Blue";
        case 0xFFFF0000: return "Orange Flame";
        case 0xFF660000: return "Mahogany";
        case 0xFFD2E6F0: return "Med Pastel Blue";
        case 0xFF62AADC: return "Cornflower Blue";
        case 0xFF275C70: return "Duck Wing Blue";
        case 0xFF306F75: return "Bright Turquoise";
        case 0xFF09A1A8: return "Bright Peacock";
        case 0xFF1B4CA4: return "Dk Sapphire";
        case 0xFFE6B9F5: return "Dusty Lavender";
        case 0xFFBE1982: return "Deep Orchid";
        case 0xFFEB8296: return "Sweet Pink";
        case 0xFFE60041: return "Deep Coral";
        case 0xFFF0C4A0: return "Coral Reed";
        case 0xFFE28264: return "Salmon Peach";
        case 0xFFB30000: return "Red Jubilee";
        case 0xFF6A0000: return "Cognac";
        case 0xFF9B6B2C: return "Burnt Toast";
        case 0xFF9C6D45: return "Toast";
        case 0xFF864C31: return "Mink Brown";
        case 0xFFEFEFE5: return "Light Gray Khaki";
        case 0xFFB7B7AF: return "Dk Gray Khaki";
        case 0xFF3C4F31: return "Evergreen";
        case 0xFF4A4A19: return "Hedge Green";
        case 0xFF5C9A1A: return "Nile Green";
        case 0xFFE0E6C8: return "Sea Mist";
        case 0xFF70770F: return "Pistachio";
        case 0xFF027602: return "Ivy Green";
        case 0xFF00AF38: return "Bright Green";
        case 0xFF93D16C: return "Willow Green";
        case 0xFF46B774: return "Dk Willow Green";
        case 0xFF483D59: return "Slate Gray";
        case 0xFF466E78: return "Dk Winter Sky";
        case 0xFF134F45: return "Dk Sage Green";
        case 0xFF343213: return "Dk French Green";
        case 0xFF415545: return "French Green";
        case 0xFF0FA56F: return "Aqua";
        case 0xFFDCEBF0: return "Ice Blue";
        case 0xFF727483: return "Winter Sky";
        case 0xFFD1DCFA: return "Heron Blue";
        case 0xFF44235D: return "Deep Nassau Blue";
        case 0xFF412044: return "Deep Slate Gray";
        case 0xFF82878C: return "Sterling";
        case 0xFFD2AAF0: return "Hyacinth";
        case 0xFF735A64: return "Lt Plum";
        case 0xFF644664: return "Dk Plum";
        case 0xFF411446: return "Purple Shadow";
        case 0xFF7E1E46: return "Plum";
        case 0xFF320046: return "Deep Eggplant";
        case 0xFF6E0A96: return "Eggplant";
        case 0xFFB47364: return "Dewberry";
        case 0xFFAEC6BB: return "Sage Green";
        case 0xFF7E6C7C: return "Gun Metal Gray";
        case 0xFFDB6478: return "Petal Pink";
        case 0xFF782346: return "Magenta";
        case 0xFF961A32: return "Mulberry";
        case 0xFF840000: return "Wine";
        case 0xFFFC8F0C: return "Bittersweet";
     /* case 0xFFFF0000: return "Poppy";    TODO: duplicate case value */
        case 0xFFCBCBBD: return "Gray Khaki";
        case 0xFF818901: return "Chartreuse";
        case 0xFFF8F5F1: return "Whisper Gray";
        case 0xFFD5C7C3: return "Dk Whisper Gray";
        case 0xFFC0B2B7: return "Nickel Gray";
        case 0xFFABA0A8: return "Dk Nickel Gray";
        case 0xFFEDF6D4: return "Pale Green";
        case 0xFF868105: return "Deep Chartreuse";
        case 0xFFF3B600: return "Sunflower Gold";
        case 0xFF349669: return "Green Peacock";
        case 0xFFC1CBB9: return "Putty";
        case 0xFF7AB31D: return "Lime Green";
        case 0xFFEE5078: return "Deep Rose";
        case 0xFF007A67: return "Wild Peacock";
        case 0xFFFF8CCB: return "Rosebud";
        case 0xFF014F3A: return "Coachman Green";
        case 0xFFCD054D: return "Lt Rose";
        case 0xFF347DCB: return "Sapphire";
        case 0xFF23238B: return "Team Blue";
        case 0xFF081705: return "Midnight Teal";
        case 0xFFFFD6C7: return "Peach Fluff";
        case 0xFF9C6484: return "Purple Accent";
        case 0xFFE6AE6F: return "Flax";
        case 0xFF6C8E87: return "Desert Cactus";
        case 0xFF80B0AE: return "Ocean Aqua";
        case 0xFF6C7C71: return "Dk Desert Cactus";
        case 0xFF8C748C: return "Purple Passion";
        case 0xFFEB7183: return "Tea Rose";
        case 0xFF68E0F8: return "Marine Aqua";
        case 0xFFB58CC7: return "Deep Hyacinth";
        /* BEGIN N/A COLORS */
        /*
                         return "Vari-Pastel Pinks";
                         return "Vari-Pinks";
                         return "Vari-Roses";
                         return "Vari-Oranges";
                         return "Vari-Pastel Blues";
                         return "Teal Blues";
                         return "Vari-Blues";
                         return "Vari-Navy Blues";
                         return "Vari-Grays/Silvers";
                         return "Vari-Grays/Blacks";
                         return "Vari-True Greens";
                         return "Vari-Grass Greens";
                         return "Vari-Mint Greens";
                         return "Vari-Bright Greens";
                         return "Vari-Avocado Grns";
                         return "Vari-Pine Greens";
                         return "Vari-Taupes";
                         return "Vari-Yellows";
                         return "Vari-Med Brown";
                         return "Vari-Lt Browns";
                         return "Vari-Dk Browns";
                         return "Vari-Orchids";
                         return "Vari-Baby Pinks";
                         return "Vari-Reds";
                         return "Vari-Purples";
                         return "Vari-Royal Purples";
                         return "Vari-Rust Peaches";
                         return "Vari-Dk Taupes";
                         return "Vari-Willow Greens";
                         return "Vari-French Greens";
                         return "Vari-Fuchsias";
                         return "Vari-Khakis";
                         return "Vari-Aqua Teals";
                         return "Vari-Coffee Browns";
                         return "Vari-Golden Ylws";
                         return "Vari-Peaches";
                         return "Baby Blue/Pink/Mint";
                         return "Mint Greens/Pinks";
                         return "Baby Pink/Mint/Blue";
                         return "Teal/Purple/Fuchsia";
                         return "Blue/Fuchsia/Purple";
                         return "Turq/Coral/Silver";
                         return "Green/Burgundy/Tan";
                         return "Burg/Green/Blue/Tan";
                         return "Seafoam/Coralrf/Ecru";
                         return "Dk Gr/Brn Burg/Lt Br";
                         return "Grn/Coral/Blue/Ylw";
                         return "Peach/Blue/Rust/Grn";
                         return "Red/Gold/Blue";
                         return "Med Grn/Purple/Gold";
                         return "Coral/Brown/Teal";
                         return "Old Gold/Black/Red";
                         return "Med Blue/Green/Tan";
                         return "Blue/Lav/Red/Ylw/Grn";
        */
        /* END N/A COLORS */
        case 0xFFFAD2AA: return "Shrimp";
        case 0xFFFADC96: return "Flesh";
        case 0xFFFFC896: return "Soft Blush";
        case 0xFFFF9B6E: return "Island Peach";
        case 0xFF375A73: return "Bayou Blue";
        case 0xFF28505A: return "Ocean View";
        case 0xFFA0B9C3: return "Madras Blue";
        case 0xFFB49682: return "Soft Heather";
        case 0xFFD2AF9B: return "Velvet Slipper";
        case 0xFFA07D82: return "Iced Mauve";
        case 0xFF645055: return "Wild Mulberry";
        case 0xFF3C2837: return "Wineberry";
        case 0xFF6E2D5A: return "Wildflower";
        case 0xFF6E2D41: return "Plum Wine";
        case 0xFFAF4B69: return "Orchid Kiss";
        case 0xFF91B432: return "Japanese Fern";
        case 0xFFD7F58C: return "Honeydew";
        case 0xFFAAAF14: return "Lemon Grass";
        case 0xFFC8F58C: return "Fairway Mist";
        case 0xFFC3913C: return "Outback";
        case 0xFFC38C73: return "Fruit Shake";
        case 0xFFFAC896: return "Creamy Peach";
        case 0xFF965A37: return "Toffee";
        case 0xFF965A28: return "Cocoa";
        case 0xFFD2C3AF: return "Gentle Rain";
        case 0xFF5F9619: return "Barnyard Grass";
        case 0xFFAA820A: return "Galley Gold";
        case 0xFFFF643C: return "Coral Sunset";
        case 0xFFFFE6AA: return "Seashell";
        case 0xFFF0EBA5: return "Crème Brulee";
        case 0xFFB47396: return "Lilac";
        case 0xFF91E12D: return "Liimeade";
        case 0xFFE19119: return "Desert Glow";
        case 0xFFD25F00: return "Pumpkin Pie";
        case 0xFFAFAA05: return "Pea Soup";
        case 0xFF6E8205: return "Peapod Green";
        case 0xFF3C4B05: return "Loden Green";
        case 0xFF9B735A: return "Lt. Cocoa";
        case 0xFFCDAA7D: return "Cocoa Cream";
        case 0xFF87C887: return "Cameo";
        case 0xFFF9E6CA: return "Sand";
        case 0xFFFDF3DA: return "Bone";
        case 0xFF5D3446: return "Dark Ash";
        case 0xFFE0C63B: return "Spring Moss";
        case 0xFFDDAB00: return "Summer Gold";
        case 0xFFA98803: return "Dk. Autumn Gold";
        case 0xFFAC8783: return "Mushroom";
        case 0xFF36361F: return "Dark Forest";
        case 0xFFFA5F7F: return "Watermelon";
        case 0xFFA3C2D7: return "Caribbean Mist";
    }

    return "";
}

int getNum_ThreadArt_Rayon(unsigned int color) { return -1; } /*TODO: finish */
const char* getName_ThreadArt_Rayon(unsigned int color) { return ""; } /*TODO: finish */

int getNum_ThreadArt_Polyester(unsigned int color) { return -1; } /*TODO: finish */
const char* getName_ThreadArt_Polyester(unsigned int color) { return ""; } /*TODO: finish */

int getNum_ThreaDelight_Polyester(unsigned int color) { return -1; } /*TODO: finish */
const char* getName_ThreaDelight_Polyester(unsigned int color) { return ""; } /*TODO: finish */

int getNum_Z102_Isacord_Polyester(unsigned int color)
{
    switch(color)
    {
        case 0xFFF8FFFF: return 17;
        case 0xFF000000: return 20;
        case 0xFFB7BABA: return 105;
        case 0xFF73787A: return 108;
        case 0xFF454B58: return 138;
        case 0xFF9EA9A6: return 142;
        case 0xFFC8C6BD: return 150;
        case 0xFFFAEE5C: return 220;
        case 0xFFE5CB4F: return 221;
        case 0xFFFFF46A: return 230;
        case 0xFFFEF9D9: return 270;
        case 0xFFFFDC00: return 311;
        case 0xFF624F00: return 345;
        case 0xFFB8B25A: return 352;
        case 0xFF8D8F5B: return 453;
        case 0xFFFFF4A5: return 520;
        case 0xFFB98E03: return 542;
        case 0xFFE4C180: return 651;
        case 0xFFC5BFA6: return 672;
        case 0xFF96836D: return 722;
        case 0xFF4E3500: return 747;
        case 0xFFDDCBA5: return 761;
        case 0xFF605840: return 776;
        case 0xFFFFAF02: return 800;
        case 0xFFF6AE32: return 811;
        case 0xFFC89334: return 822;
        case 0xFFE59300: return 824;
        case 0xFFC89340: return 832;
        case 0xFF9E947F: return 873;
        case 0xFFC8700B: return 922;
        case 0xFFBB5704: return 931;
        case 0xFFB19072: return 1061;
        case 0xFFFF8101: return 1102;
        case 0xFFB1500A: return 1115;
        case 0xFFC09C72: return 1123;
        case 0xFF843D07: return 1134;
        case 0xFFD8A67D: return 1141;
        case 0xFF82421B: return 1154;
        case 0xFFFF7319: return 1300;
        case 0xFFFF3D1E: return 1305;
        case 0xFFBA4005: return 1311;
        case 0xFFC73C13: return 1312;
        case 0xFFE66B21: return 1332;
        case 0xFF3D1C11: return 1346;
        case 0xFFFFBC95: return 1351;
        case 0xFFFFCC93: return 1362;
        case 0xFF373732: return 1375;
        case 0xFFFFAF94: return 1532;
        case 0xFF5B4032: return 1565;
        case 0xFFFF6046: return 1600;
        case 0xFFFF6D71: return 1753;
        case 0xFFEBBAAE: return 1755;
        case 0xFFEB2D2B: return 1805;
        case 0xFFFF988F: return 1840;
        case 0xFF434331: return 1874;
        case 0xFFC11914: return 1902;
        case 0xFFC8100D: return 1903;
        case 0xFFBF0A21: return 1906;
        case 0xFFD23C3E: return 1921;
        case 0xFF8F8C93: return 1972;
        case 0xFFA31A1C: return 2011;
        case 0xFF4D0308: return 2115;
        case 0xFFFFCDCC: return 2155;
        case 0xFF871C45: return 2500;
        case 0xFFDB4083: return 2508;
        case 0xFFFF668A: return 2520;
        case 0xFFC91243: return 2521;
        case 0xFFFFA0B6: return 2530;
        case 0xFFFEA5B9: return 2550;
        case 0xFF626C7E: return 2674;
        case 0xFF5E1942: return 2711;
        case 0xFF33001D: return 2715;
        case 0xFFA57B8D: return 2764;
        case 0xFF702A69: return 2810;
        case 0xFFB385BC: return 2830;
        case 0xFF240047: return 2900;
        case 0xFF724593: return 2910;
        case 0xFF634D86: return 2920;
        case 0xFF000136: return 3110;
        case 0xFF000021: return 3355;
        case 0xFF054ABD: return 3522;
        case 0xFF1C005D: return 3541;
        case 0xFF001F71: return 3544;
        case 0xFF002E5E: return 3622;
        case 0xFF71AAD8: return 3630;
        case 0xFF001748: return 3644;
        case 0xFFC8DBE4: return 3650;
        case 0xFF9FC7DF: return 3730;
        case 0xFF082E4D: return 3743;
        case 0xFF98B0BC: return 3750;
        case 0xFF23679C: return 3810;
        case 0xFF3D657E: return 3842;
        case 0xFF00669F: return 3901;
        case 0xFF47AEDD: return 3910;
        case 0xFFBBDFEB: return 3962;
        case 0xFFBABEB7: return 3971;
        case 0xFF015D7E: return 4032;
        case 0xFFD5DDDB: return 4071;
        case 0xFF888D8E: return 4073;
        case 0xFF007CA6: return 4103;
        case 0xFF3EBBC8: return 4111;
        case 0xFF005C79: return 4116;
        case 0xFF80CCD8: return 4240;
        case 0xFFACCEC7: return 4250;
        case 0xFF006E74: return 4410;
        case 0xFF002A29: return 4515;
        case 0xFF38A4AE: return 4620;
        case 0xFFAFD8CD: return 5050;
        case 0xFF149B7B: return 5210;
        case 0xFF7AC8AF: return 5220;
        case 0xFF187166: return 5233;
        case 0xFF004B23: return 5374;
        case 0xFF006835: return 5415;
        case 0xFF5C9C51: return 5531;
        case 0xFF0E9543: return 5613;
        case 0xFF5E7A17: return 5833;
        case 0xFF225926: return 5944;
        case 0xFFBCD633: return 6011;
        case 0xFFBBCD91: return 6051;
        case 0xFF978B3C: return 6133;
    }

    return -1;
}

const char* getName_Z102_Isacord_Polyester(unsigned int color) { return ""; } /*TODO: finish */

#endif /* ARDUINO TODO: remove this line when thread-color.c is arduino compatible. This is a temporary arduino build fix. */

int threadColorNum(unsigned int color, ThreadBrand brand)
{
    switch(brand)
    {
        case Arc_Polyester:          return getNum_Arc_Polyester(color);
        case Arc_Rayon:              return getNum_Arc_Rayon(color);
        case CoatsAndClark_Rayon:    return getNum_CoatsAndClark_Rayon(color);
        case Exquisite_Polyester:    return getNum_Exquisite_Polyester(color);
        case Fufu_Polyester:         return getNum_Fufu_Polyester(color);
        case Fufu_Rayon:             return getNum_Fufu_Rayon(color);
        case Hemingworth_Polyester:  return getNum_Hemingworth_Polyester(color);
        case Isacord_Polyester:      return getNum_Isacord_Polyester(color);
        case Isafil_Rayon:           return getNum_Isafil_Rayon(color);
        case Marathon_Polyester:     return getNum_Marathon_Polyester(color);
        case Marathon_Rayon:         return getNum_Marathon_Rayon(color);
        case Madeira_Polyester:      return getNum_Madeira_Polyester(color);
        case Madeira_Rayon:          return getNum_Madeira_Rayon(color);
        case Metro_Polyester:        return getNum_Metro_Polyester(color);
        case Pantone:                return getNum_Pantone(color);
        case RobisonAnton_Polyester: return getNum_RobisonAnton_Polyester(color);
        case RobisonAnton_Rayon:     return getNum_RobisonAnton_Rayon(color);
        case Sigma_Polyester:        return getNum_Sigma_Polyester(color);
        case Sulky_Rayon:            return getNum_Sulky_Rayon(color);
        case ThreadArt_Rayon:        return getNum_ThreadArt_Rayon(color);
        case ThreadArt_Polyester:    return getNum_ThreadArt_Polyester(color);
        case ThreaDelight_Polyester: return getNum_ThreaDelight_Polyester(color);
        case Z102_Isacord_Polyester: return getNum_Z102_Isacord_Polyester(color);
    }

    return -1;
}

const char* threadColorName(unsigned int color, ThreadBrand brand)
{
    switch(brand)
    {
        case Arc_Polyester:          return getName_Arc_Polyester(color);
        case Arc_Rayon:              return getName_Arc_Rayon(color);
        case CoatsAndClark_Rayon:    return getName_CoatsAndClark_Rayon(color);
        case Exquisite_Polyester:    return getName_Exquisite_Polyester(color);
        case Fufu_Polyester:         return getName_Fufu_Polyester(color);
        case Fufu_Rayon:             return getName_Fufu_Rayon(color);
        case Hemingworth_Polyester:  return getName_Hemingworth_Polyester(color);
        case Isacord_Polyester:      return getName_Isacord_Polyester(color);
        case Isafil_Rayon:           return getName_Isafil_Rayon(color);
        case Marathon_Polyester:     return getName_Marathon_Polyester(color);
        case Marathon_Rayon:         return getName_Marathon_Rayon(color);
        case Madeira_Polyester:      return getName_Madeira_Polyester(color);
        case Madeira_Rayon:          return getName_Madeira_Rayon(color);
        case Metro_Polyester:        return getName_Metro_Polyester(color);
        case Pantone:                return getName_Pantone(color);
        case RobisonAnton_Polyester: return getName_RobisonAnton_Polyester(color);
        case RobisonAnton_Rayon:     return getName_RobisonAnton_Rayon(color);
        case Sigma_Polyester:        return getName_Sigma_Polyester(color);
        case Sulky_Rayon:            return getName_Sulky_Rayon(color);
        case ThreadArt_Rayon:        return getName_ThreadArt_Rayon(color);
        case ThreadArt_Polyester:    return getName_ThreadArt_Polyester(color);
        case ThreaDelight_Polyester: return getName_ThreaDelight_Polyester(color);
        case Z102_Isacord_Polyester: return getName_Z102_Isacord_Polyester(color);
    }

    return "";
}

/* gcc -DTEST_THREAD_COLOR -o thread-color thread-color.c */
#ifdef TEST_THREAD_COLOR

#include <stdio.h>

int main(void)
{
    unsigned int tColor = 0xFFD25F00;
    ThreadBrand  tBrand = Sulky_Rayon;
    int          tNum   = threadColorNum(tColor, tBrand);
    const char*  tName  = threadColorName(tColor, tBrand);

    printf("Color : 0x%X\n"
           "Brand : %d\n"
           "Num   : %d\n"
           "Name  : %s\n\n",
            tColor,
            tBrand,
            tNum,   /* Solution: 1833 */
            tName); /* Solution: Pumpkin Pie */
    return 0;
}

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
