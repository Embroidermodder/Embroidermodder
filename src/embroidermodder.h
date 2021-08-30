#ifndef EMBROIDERMODDER_H
#define EMBROIDERMODDER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "embroidery.h"

/*
 * Defines
 * -------
 */

#define DEBUG 1

#define TOOLBAR_FILE       0
#define TOOLBAR_EDIT       1
#define TOOLBAR_VIEW       2
#define TOOLBAR_ZOOM       3
#define TOOLBAR_PAN        4
#define TOOLBAR_ICON       5
#define TOOLBAR_HELP       6
#define TOOLBAR_LAYER      7
#define TOOLBAR_TEXT       8
#define TOOLBAR_PROPERTIES 9

#define ICON_3dviews                    0
#define ICON_about                      1
#define ICON_aligneddimension           2
#define ICON_aligntextangle             3
#define ICON_aligntextcenter            4
#define ICON_aligntexthome              5
#define ICON_aligntextleft              6
#define ICON_aligntextright             7
#define ICON_aligntext                  8
#define ICON_angulardimension           9
#define ICON_app                       10
#define ICON_arc3points                11
#define ICON_arccenterstartangle       12
#define ICON_arccenterstartend         13
#define ICON_arccenterstartlength      14
#define ICON_arccontinue               15
#define ICON_arcstartcenterangle       16
#define ICON_arcstartcenterend         17
#define ICON_arcstartcenterlength      18
#define ICON_arcstartendangle          19
#define ICON_arcstartenddirection      20
#define ICON_arcstartendradius         21
#define ICON_arc                       22
#define ICON_area                      23
#define ICON_array   24
#define ICON_backview    25
#define ICON_baselinedimension   26
#define ICON_bean    27
#define ICON_blank   28
#define ICON_bottomview  29
#define ICON_boundary    30
#define ICON_break2points    31
#define ICON_breakatpoint    32
#define ICON_browser 33
#define ICON_camera  34
#define ICON_centermark  35
#define ICON_chamfer 36
#define ICON_changelog   37
#define ICON_check   38
#define ICON_circle2points   39
#define ICON_circle3points   40
#define ICON_circlecenterdiameter    41
#define ICON_circlecenterradius  42
#define ICON_circletantanradius  43
#define ICON_circletantantan 44
#define ICON_circle  45
#define ICON_cloud_2 46
#define ICON_cloud   47
#define ICON_colorblue   48
#define ICON_colorbyblock    49
#define ICON_colorbylayer    50
#define ICON_colorcyan   51
#define ICON_colorgreen  52
#define ICON_colormagenta    53
#define ICON_colorother  54
#define ICON_colorred    55
#define ICON_colorselector   56
#define ICON_colorwhite  57
#define ICON_coloryellow 58
#define ICON_constructionline    59
#define ICON_continuedimension   60
#define ICON_copyobject  61
#define ICON_copy    62
#define ICON_customizekeyboard   63
#define ICON_customizemenus  64
#define ICON_customizetoolbars   65
#define ICON_customize   66
#define ICON_cut 67
#define ICON_date    68
#define ICON_day 69
#define ICON_designdetails   70
#define ICON_diameterdimension   71
#define ICON_dimensionedit   72
#define ICON_dimensionstyle  73
#define ICON_dimensiontextedit   74
#define ICON_dimensionupdate 75
#define ICON_distance    76
#define ICON_dolphin 77
#define ICON_donothing   78
#define ICON_donut_2 79
#define ICON_donut   80
#define ICON_drawing2    81
#define ICON_drawing 82
#define ICON_ellipsearc  83
#define ICON_ellipseaxisend  84
#define ICON_ellipsecenter   85
#define ICON_ellipse 86
#define ICON_erase   87
#define ICON_escape  88
#define ICON_exit    89
#define ICON_explode 90
#define ICON_extend  91
#define ICON_fillet  92
#define ICON_findandreplace  93
#define ICON_freezealllayers 94
#define ICON_frontview   95
#define ICON_gridsettings    96
#define ICON_gridsnapsettings    97
#define ICON_hatch   98
#define ICON_heart_2 99
#define ICON_heart   100
#define ICON_help_2  101
#define ICON_help    102
#define ICON_hex 103
#define ICON_hidealllayers   104
#define ICON_histogram   105
#define ICON_icon128 106
#define ICON_icon16  107
#define ICON_icon24  108
#define ICON_icon32  109
#define ICON_icon48  110
#define ICON_icon64  111
#define ICON_inquiry 112
#define ICON_insertblock 113
#define ICON_join    114
#define ICON_justifytext 115
#define ICON_layerprevious   116
#define ICON_layerselector   117
#define ICON_layers  118
#define ICON_layertranslate  119
#define ICON_leftview    120
#define ICON_lengthen    121
#define ICON_lineardimension 122
#define ICON_linetypebyblock 123
#define ICON_linetypebylayer 124
#define ICON_linetypecenter  125
#define ICON_linetypecontinuous  126
#define ICON_linetypehidden  127
#define ICON_linetypeother   128
#define ICON_linetypeselector    129
#define ICON_lineweight01    130
#define ICON_lineweight02    131
#define ICON_lineweight03    132
#define ICON_lineweight04    133
#define ICON_lineweight05    134
#define ICON_lineweight06    135
#define ICON_lineweight07    136
#define ICON_lineweight08    137
#define ICON_lineweight09    138
#define ICON_lineweight10    139
#define ICON_lineweight11    140
#define ICON_lineweight12    141
#define ICON_lineweight13    142
#define ICON_lineweight14    143
#define ICON_lineweight15    144
#define ICON_lineweight16    145
#define ICON_lineweight17    146
#define ICON_lineweight18    147
#define ICON_lineweight19    148
#define ICON_lineweight20    149
#define ICON_lineweight21    150
#define ICON_lineweight22    151
#define ICON_lineweight23    152
#define ICON_lineweight24    153
#define ICON_lineweightbyblock   154
#define ICON_lineweightbylayer   155
#define ICON_lineweightdefault   156
#define ICON_lineweightselector  157
#define ICON_lineweightsettings  158
#define ICON_line    159
#define ICON_list    160
#define ICON_locatepoint 161
#define ICON_locator_snaptoapparentintersection 162
#define ICON_locator_snaptocenter    163
#define ICON_locator_snaptoendpoint  164
#define ICON_locator_snaptoextension 165
#define ICON_locator_snaptoinsert    166
#define ICON_locator_snaptointersection  167
#define ICON_locator_snaptomidpoint  168
#define ICON_locator_snaptonearest   169
#define ICON_locator_snaptonode  170
#define ICON_locator_snaptoparallel  171
#define ICON_locator_snaptoperpendicular 172
#define ICON_locator_snaptoquadrant  173
#define ICON_locator_snaptotangent   174
#define ICON_lockalllayers   175
#define ICON_makeblock   176
#define ICON_makelayercurrent    177
#define ICON_mass    178
#define ICON_mirror  179
#define ICON_move    180
#define ICON_multilinetext   181
#define ICON_multiline   182
#define ICON_namedviews  183
#define ICON_neisometricview 184
#define ICON_new 185
#define ICON_night   186
#define ICON_nopreview   187
#define ICON_nwisometricview 188
#define ICON_obliquedimensions   189
#define ICON_offset  190
#define ICON_open    191
#define ICON_ordinatedimension   192
#define ICON_orthosettings   193
#define ICON_pandown 194
#define ICON_panleft 195
#define ICON_panpoint    196
#define ICON_panrealtime 197
#define ICON_panright    198
#define ICON_panup   199
#define ICON_pan 200
#define ICON_paste   201
#define ICON_path    202
#define ICON_pickadd 203
#define ICON_picknew 204
#define ICON_plugin  205
#define ICON_pointdivide 206
#define ICON_pointmeasure    207
#define ICON_pointmultiple   208
#define ICON_pointsingle 209
#define ICON_point   210
#define ICON_polarsettings   211
#define ICON_polygon 212
#define ICON_polyline    213
#define ICON_print   214
#define ICON_pyscript    215
#define ICON_qsnapsettings   216
#define ICON_qtracksettings  217
#define ICON_quickdimension  218
#define ICON_quickleader 219
#define ICON_quickselect 220
#define ICON_radiusdimension 221
#define ICON_ray 222
#define ICON_rectangle   223
#define ICON_redo    224
#define ICON_region  225
#define ICON_render  226
#define ICON_rgb 227
#define ICON_rightview   228
#define ICON_rotate  229
#define ICON_rulersettings   230
#define ICON_sandbox 231
#define ICON_satin   232
#define ICON_saveas  233
#define ICON_save    234
#define ICON_scale   235
#define ICON_seisometricview 236
#define ICON_settingsdialog_2    237
#define ICON_settingsdialog  238
#define ICON_shade2dwireframe    239
#define ICON_shade3dwireframe    240
#define ICON_shadeflatedges  241
#define ICON_shadeflat   242
#define ICON_shadehidden 243
#define ICON_shadesmoothedges    244
#define ICON_shadesmooth 245
#define ICON_shade   246
#define ICON_showalllayers   247
#define ICON_singlelinetext  248
#define ICON_sketch_2    249
#define ICON_sketch  250
#define ICON_snapfrom    251
#define ICON_snaptoapparentintersection  252
#define ICON_snaptocenter    253
#define ICON_snaptoendpoint  254
#define ICON_snaptoextension 255
#define ICON_snaptoinsert    256
#define ICON_snaptointersection  257
#define ICON_snaptomidpoint  258
#define ICON_snaptonearest   259
#define ICON_snaptonode  260
#define ICON_snaptonone  261
#define ICON_snaptoparallel  262
#define ICON_snaptoperpendicular 263
#define ICON_snaptoquadrant  264
#define ICON_snaptotangent   265
#define ICON_snowflake_2 266
#define ICON_snowflake   267
#define ICON_solidbox    268
#define ICON_solidcheck  269
#define ICON_solidclean  270
#define ICON_solidcoloredges 271
#define ICON_solidcolorfaces 272
#define ICON_solidcone   273
#define ICON_solidcopyedges  274
#define ICON_solidcopyfaces  275
#define ICON_solidcylinder   276
#define ICON_soliddeletefaces    277
#define ICON_solidextrudefaces   278
#define ICON_solidextrude    279
#define ICON_solidimprint    280
#define ICON_solidinterfere  281
#define ICON_solidintersect  282
#define ICON_solidmovefaces  283
#define ICON_solidoffsetfaces    284
#define ICON_solidrevolve    285
#define ICON_solidrotatefaces    286
#define ICON_solidsection    287
#define ICON_solidsediting   288
#define ICON_solidseparate   289
#define ICON_solidsetupdrawing   290
#define ICON_solidsetupprofile   291
#define ICON_solidsetupview  292
#define ICON_solidsetup  293
#define ICON_solidshell  294
#define ICON_solidslice  295
#define ICON_solidsphere 296
#define ICON_solidsubtract   297
#define ICON_solids  298
#define ICON_solidtaperfaces 299
#define ICON_solidtorus  300
#define ICON_solidunion  301
#define ICON_solidwedge  302
#define ICON_spline  303
#define ICON_star    304
#define ICON_stretch 305
#define ICON_stub    306
#define ICON_surface2dsolid  307
#define ICON_surface3dface   308
#define ICON_surface3dmesh   309
#define ICON_surfacebox  310
#define ICON_surfacecone 311
#define ICON_surfacecylinder 312
#define ICON_surfacedish 313
#define ICON_surfacedome 314
#define ICON_surfaceedgesurface  315
#define ICON_surfaceedge 316
#define ICON_surfacepyramid  317
#define ICON_surfacerevolvedsurface  318
#define ICON_surfaceruledsurface 319
#define ICON_surfacesphere   320
#define ICON_surfaces    321
#define ICON_surfacetabulatedsurface 322
#define ICON_surfacetorus    323
#define ICON_surfacewedge    324
#define ICON_swisometricview 325
#define ICON_temptrackingpoint   326
#define ICON_textbold    327
#define ICON_textitalic  328
#define ICON_textoverline    329
#define ICON_textstrikeout   330
#define ICON_textunderline   331
#define ICON_text    332
#define ICON_thawalllayers   333
#define ICON_theme   334
#define ICON_tipoftheday_2   335
#define ICON_tipoftheday 336
#define ICON_tolerance   337
#define ICON_topview 338
#define ICON_trim    339
#define ICON_undo    340
#define ICON_units   341
#define ICON_unlockalllayers 342
#define ICON_view    343
#define ICON_whatsthis   344
#define ICON_wideflange  345
#define ICON_windowcascade   346
#define ICON_windowcloseall  347
#define ICON_windowclose 348
#define ICON_windownext  349
#define ICON_windowprevious  350
#define ICON_windowtile  351
#define ICON_world   352
#define ICON_zoomall 353
#define ICON_zoomcenter  354
#define ICON_zoomdynamic 355
#define ICON_zoomextents 356
#define ICON_zoomin  357
#define ICON_zoomout 358
#define ICON_zoomprevious   359
#define ICON_zoomrealtime   360
#define ICON_zoomscale      361
#define ICON_zoomselected   362
#define ICON_zoomwindow     363
#define ICON_zoom           364

#define SYMBOL_zero          0
#define SYMBOL_one           1
#define SYMBOL_two           2
#define SYMBOL_three         3
#define SYMBOL_four          4
#define SYMBOL_five          5
#define SYMBOL_six           6
#define SYMBOL_seven         7
#define SYMBOL_eight         8
#define SYMBOL_nine          9
#define SYMBOL_minus        10
#define SYMBOL_apostrophe   11
#define SYMBOL_quote        12

/* Keys */
/* ---- */
/* value type - int: See OBJ_TYPE_VALUES */
#define OBJ_TYPE    0
/* value type - str: See OBJ_NAME_VALUES */
#define OBJ_NAME    1
/* value type - str: "USER", "DEFINED", "STRINGS", etc... */
#define OBJ_LAYER   2
/* value type - int: 0-255
  TODO: Use color chart in formats/format-dxf.h for this */
#define OBJ_COLOR   3
//value type - int: See OBJ_LTYPE_VALUES
#define OBJ_LTYPE   4
 //value type - int: 0-27
#define OBJ_LWT 5
/* value type - int: See OBJ_RUBBER_VALUES */
#define OBJ_RUBBER  6

/* Values */
/* ------ */
/* NOTE: Allow this enum to evaluate false */
#define OBJ_TYPE_NULL          0
/* NOTE: Values >= 65536 ensure compatibility with qgraphicsitem_cast() */
#define OBJ_TYPE_BASE          100000
#define OBJ_TYPE_ARC           100001
#define OBJ_TYPE_BLOCK         100002
#define OBJ_TYPE_CIRCLE        100003
#define OBJ_TYPE_DIMALIGNED    100004
#define OBJ_TYPE_DIMANGULAR    100005
#define OBJ_TYPE_DIMARCLENGTH  100006
#define OBJ_TYPE_DIMDIAMETER   100007
#define OBJ_TYPE_DIMLEADER 100008
#define OBJ_TYPE_DIMLINEAR 100009
#define OBJ_TYPE_DIMORDINATE   100010
#define OBJ_TYPE_DIMRADIUS 100011
#define OBJ_TYPE_ELLIPSE   100012
#define OBJ_TYPE_ELLIPSEARC    100013
#define OBJ_TYPE_RUBBER    100014
#define OBJ_TYPE_GRID  100015
#define OBJ_TYPE_HATCH 100016
#define OBJ_TYPE_IMAGE 100017
#define OBJ_TYPE_INFINITELINE  100018
#define OBJ_TYPE_LINE  100019
#define OBJ_TYPE_PATH  100020
#define OBJ_TYPE_POINT         100021
#define OBJ_TYPE_POLYGON       100022
#define OBJ_TYPE_POLYLINE      100023
#define OBJ_TYPE_RAY           100024
#define OBJ_TYPE_RECTANGLE     100025
#define OBJ_TYPE_SLOT          100026
#define OBJ_TYPE_SPLINE        100027
#define OBJ_TYPE_TEXTMULTI     100028
#define OBJ_TYPE_TEXTSINGLE    100029


/* CAD Linetypes */
#define OBJ_LTYPE_CONT   0
#define OBJ_LTYPE_CENTER 1
#define OBJ_LTYPE_DOT    2
#define OBJ_LTYPE_HIDDEN 3
#define OBJ_LTYPE_PHANTOM    4
#define OBJ_LTYPE_ZIGZAG 5
/* Embroidery Stitchtypes */
/* __________ */
#define OBJ_LTYPE_RUNNING    6
/* vvvvvvvvvv */
#define OBJ_LTYPE_SATIN  7
/* >>>>>>>>>> */
#define OBJ_LTYPE_FISHBONE   8

/* OBJ_LWT_VALUES
 * --------------
 */
#define OBJ_LWT_BYLAYER (-2)
#define OBJ_LWT_BYBLOCK (-1)
#define OBJ_LWT_DEFAULT   0
#define OBJ_LWT_01    1
#define OBJ_LWT_02    2 
#define OBJ_LWT_03    3 
#define OBJ_LWT_04    4 
#define OBJ_LWT_05    5 
#define OBJ_LWT_06    6 
#define OBJ_LWT_07    7 
#define OBJ_LWT_08    8 
#define OBJ_LWT_09    9 
#define OBJ_LWT_10   10 
#define OBJ_LWT_11   11 
#define OBJ_LWT_12   12 
#define OBJ_LWT_13   13 
#define OBJ_LWT_14   14 
#define OBJ_LWT_15   15 
#define OBJ_LWT_16   16 
#define OBJ_LWT_17   17 
#define OBJ_LWT_18   18 
#define OBJ_LWT_19   19 
#define OBJ_LWT_20   20 
#define OBJ_LWT_21   21 
#define OBJ_LWT_22   22 
#define OBJ_LWT_23   23 
#define OBJ_LWT_24   24

/* OBJ_SNAP_VALUES */
/* --------------- */
/* NOTE: Allow this enum to evaluate false */
#define OBJ_SNAP_NULL  0
#define OBJ_SNAP_ENDPOINT  1
#define OBJ_SNAP_MIDPOINT  2
#define OBJ_SNAP_CENTER    3
#define OBJ_SNAP_NODE  4
#define OBJ_SNAP_QUADRANT  5
#define OBJ_SNAP_INTERSECTION  6
#define OBJ_SNAP_EXTENSION 7
#define OBJ_SNAP_INSERTION 8
#define OBJ_SNAP_PERPENDICULAR 9
#define OBJ_SNAP_TANGENT  10
#define OBJ_SNAP_NEAREST  11
#define OBJ_SNAP_APPINTERSECTION  12
#define OBJ_SNAP_PARALLEL 13


/* OBJ_RUBBER_VALUES
 * -----------------
 * NOTE: Allow this enum to evaluate false */
#define OBJ_RUBBER_OFF    0
/* NOTE: Allow this enum to evaluate true */
#define OBJ_RUBBER_ON 1
#define OBJ_RUBBER_CIRCLE_1P_RAD  2
#define OBJ_RUBBER_CIRCLE_1P_DIA  3
#define OBJ_RUBBER_CIRCLE_2P  4
#define OBJ_RUBBER_CIRCLE_3P  5
#define OBJ_RUBBER_CIRCLE_TTR 6
#define OBJ_RUBBER_CIRCLE_TTT 7
#define OBJ_RUBBER_DIMLEADER_LINE 8
#define OBJ_RUBBER_ELLIPSE_LINE   9
#define OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS 10
#define OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS  11
#define OBJ_RUBBER_ELLIPSE_ROTATION  12
#define OBJ_RUBBER_GRIP  13
#define OBJ_RUBBER_LINE  14
#define OBJ_RUBBER_POLYGON   15
#define OBJ_RUBBER_POLYGON_INSCRIBE  16
#define OBJ_RUBBER_POLYGON_CIRCUMSCRIBE 17
#define OBJ_RUBBER_POLYLINE  18
#define OBJ_RUBBER_IMAGE 19
#define OBJ_RUBBER_RECTANGLE 20
#define OBJ_RUBBER_TEXTSINGLE    21

/* SPARE_RUBBER_VALUES
 * -------------------
 * NOTE: Allow this enum to evaluate false */
#define SPARE_RUBBER_OFF    0
#define SPARE_RUBBER_PATH   1
#define SPARE_RUBBER_POLYGON    2
#define SPARE_RUBBER_POLYLINE   3


/* PREVIEW_CLONE_VALUES
 * --------------------
 * NOTE: Allow this enum to evaluate false */
#define PREVIEW_CLONE_NULL  0
#define PREVIEW_CLONE_SELECTED  1
#define PREVIEW_CLONE_RUBBER    2


/* PREVIEW_MODE_VALUES
 * -------------------
 * NOTE: Allow this enum to evaluate false */
#define PREVIEW_MODE_NULL   0
#define PREVIEW_MODE_MOVE   1
#define PREVIEW_MODE_ROTATE 2
#define PREVIEW_MODE_SCALE  3


/* COMMAND ACTIONS */
/* --------------- */
#define ACTION_null   0
#define ACTION_donothing  1
#define ACTION_new    2
#define ACTION_open   3
#define ACTION_save   4
#define ACTION_saveas 5
#define ACTION_print  6
#define ACTION_designdetails  7
#define ACTION_exit   8
#define ACTION_cut    9
#define ACTION_copy   10
#define ACTION_paste  11
#define ACTION_undo   12
#define ACTION_redo   13
/* Window Menu */
#define ACTION_windowclose    14
#define ACTION_windowcloseall 15
#define ACTION_windowcascade  16
#define ACTION_windowtile 17
#define ACTION_windownext 18
#define ACTION_windowprevious 19
/* Help Menu */
#define ACTION_help   20
#define ACTION_changelog  21
#define ACTION_tipoftheday    22
#define ACTION_about  23
#define ACTION_whatsthis  24
/* Icons */
#define ACTION_icon16 25
#define ACTION_icon24 26
#define ACTION_icon32 27
#define ACTION_icon48 28
#define ACTION_icon64 29
#define ACTION_icon128    30
#define ACTION_settingsdialog 31
/* Layer ToolBar */
#define ACTION_makelayercurrent 32
#define ACTION_layers   33
#define ACTION_layerselector    34
#define ACTION_layerprevious    35
#define ACTION_colorselector    36
#define ACTION_linetypeselector 37
#define ACTION_lineweightselector 38
#define ACTION_hidealllayers    39
#define ACTION_showalllayers    40
#define ACTION_freezealllayers  41
#define ACTION_thawalllayers    42
#define ACTION_lockalllayers    43
#define ACTION_unlockalllayers  44
/* Text ToolBar */
#define ACTION_textbold 45
#define ACTION_textitalic   46
#define ACTION_textunderline    47
#define ACTION_textstrikeout    48
#define ACTION_textoverline 49
/* Zoom ToolBar */
#define ACTION_zoomrealtime 50
#define ACTION_zoomprevious 51
#define ACTION_zoomwindow   52
#define ACTION_zoomdynamic  53
#define ACTION_zoomscale    54
#define ACTION_zoomcenter   55
#define ACTION_zoomin   56
#define ACTION_zoomout  57
#define ACTION_zoomselected 58
#define ACTION_zoomall  59
#define ACTION_zoomextents  60
/* Pan SubMenu */
#define ACTION_panrealtime  61
#define ACTION_panpoint 62
#define ACTION_panleft  63
#define ACTION_panright 64
#define ACTION_panup    65
#define ACTION_pandown  66
/* View */
#define ACTION_day  67
#define ACTION_night    68

/* Just added */
#define ACTION_delete   69

/*
TODO: ACTION_spellcheck 
TODO: ACTION_quickselect 
*/

#define app_folder  0
#define commands_folder 1
#define help_folder 2
#define icons_folder    3
#define images_folder   4
#define samples_folder  5
#define translations_folder 6
#define nFolders    7


#define OBJ_NAME_NULL   "Unknown"
#define OBJ_NAME_BASE   "Base"
#define OBJ_NAME_ARC    "Arc"
#define OBJ_NAME_BLOCK  "Block"
#define OBJ_NAME_CIRCLE "Circle"
#define OBJ_NAME_DIMALIGNED "Aligned Dimension"
#define OBJ_NAME_DIMANGULAR "Angular Dimension"
#define OBJ_NAME_DIMARCLENGTH   "Arc Length Dimension"
#define OBJ_NAME_DIMDIAMETER    "Diameter Dimension"
#define OBJ_NAME_DIMLEADER  "Leader Dimension"
#define OBJ_NAME_DIMLINEAR  "Linear Dimension"
#define OBJ_NAME_DIMORDINATE    "Ordinate Dimension"
#define OBJ_NAME_DIMRADIUS  "Radius Dimension"
#define OBJ_NAME_ELLIPSE    "Ellipse"
#define OBJ_NAME_ELLIPSEARC "Elliptical Arc"
#define OBJ_NAME_RUBBER "Rubber"
#define OBJ_NAME_GRID   "Grid"
#define OBJ_NAME_HATCH  "Hatch"
#define OBJ_NAME_IMAGE  "Image"
#define OBJ_NAME_INFINITELINE   "Infinite Line"
#define OBJ_NAME_LINE   "Line"
#define OBJ_NAME_PATH   "Path"
#define OBJ_NAME_POINT  "Point"
#define OBJ_NAME_POLYGON    "Polygon"
#define OBJ_NAME_POLYLINE   "Polyline"
#define OBJ_NAME_RAY    "Ray"
#define OBJ_NAME_RECTANGLE  "Rectangle"
#define OBJ_NAME_SLOT   "Slot"
#define OBJ_NAME_SPLINE "Spline"
#define OBJ_NAME_TEXTMULTI  "Multi Line Text"
#define OBJ_NAME_TEXTSINGLE "Single Line Text"

#define PATHS_MOVETO    0
#define PATHS_LINETO    1
#define PATHS_ARCTO     2
#define PATHS_ARCMOVETO 3
#define PATHS_ELLIPSE   4
#define PATHS_END       5

#define N_TEXTURES      2

/*
 * TYPEDEFS
 * --------
 */

typedef struct Path_Symbol__ {
    int type;
    float values[6];
} path_symbol;

typedef struct Action_hash_data {
    int id;
    int icon;
    char abbreviation[20];
    char menu_name[20];
    char description[50];
} action_hash_data;

typedef struct Command {
    char menu_name[15];
    int menu_position;
    char toolbar_name[15];
    int toolbar_position;
    char tooltip[15];
    char statustip[100];
    char alias[40];
    int function;
} command;

typedef struct Quad {
    int flag;
    float left;
    float right;
    float top;
    float bottom;
    float red;
    float green;
    float blue;
} quad;

typedef struct Texture_t {
    float position[8];
    float corners[8];
    int width;
    int height;
} texture_t;

/* C functions for embroidermodder
 * -------------------------------
 */

void debug_message(const char *format, ...);
void app_dir(char *string, int folder);
int file_exists(char *fname);
int parseJSON(char *fname);
int main_tex_example(int, char*argv[]);

/*
 * C++ Specific code
 * -----------------
 */
#ifdef __cplusplus
}

#include <QtGlobal>
#include <QGroupBox>
#include <QToolButton>
#include <QUndoGroup>
#include <QMetaObject>
#include <QGraphicsView>
#include <QDockWidget>
#include <QStatusBar>
#include <QToolBar>
#include <QDialog>
#include <QTreeView>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QStandardItemModel>
#include <QApplication>
#include <QDialogButtonBox>
#include <QWizard>
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>
#include <QFontComboBox>
#include <QGraphicsPathItem>
#include <QSortFilterProxyModel>
#include <QMainWindow>
#include <QSignalMapper>
#include <QFileDialog>
#include <QUndoCommand>
#include <QUndoView>

class MainWindow;
class BaseObject;
class SelectBox;
class StatusBar;
class StatusBarButton;
class View;
class PropertyEditor;
class UndoEditor;
class ArcObject;
class BlockObject;
class CircleObject;
class DimAlignedObject;
class DimAngularObject;
class DimArcLengthObject;
class DimDiameterObject;
class DimLeaderObject;
class DimLinearObject;
class DimOrdinateObject;
class DimRadiusObject;
class EllipseObject;
class EllipseArcObject;
class HatchObject;
class ImageObject;
class InfiniteLineObject;
class LineObject;
class PathObject;
class PointObject;
class PolygonObject;
class PolylineObject;
class RayObject;
class RectObject;
class SplineObject;
class TextMultiObject;
class TextSingleObject;
class ImageWidget;
class StatusBarButton;

/* Closer to C code */
typedef struct Preview_wrapper {
    int general_mdi_bg_use_logo;
    int general_mdi_bg_use_texture;
    int general_mdi_bg_use_color;

    int display_show_scrollbars;

    int lwt_show_lwt;
    int lwt_real_render;

    unsigned char display_selectbox_alpha;

    QString accept_general_mdi_bg_logo;
    QString accept_general_mdi_bg_texture;
    QRgb    general_mdi_bg_color;
    QRgb    accept_general_mdi_bg_color;

    QRgb    display_crosshair_color;
    QRgb    accept_display_crosshair_color;
    QRgb    display_bg_color;
    QRgb    accept_display_bg_color;

    QRgb    display_selectbox_left_color;
    QRgb    accept_display_selectbox_left_color;
    QRgb    display_selectbox_left_fill;
    QRgb accept_display_selectbox_left_fill;
    QRgb display_selectbox_right_color;
    QRgb accept_display_selectbox_right_color;
    QRgb display_selectbox_right_fill;
    QRgb accept_display_selectbox_right_fill;

    QRgb grid_color;
    QRgb accept_grid_color;

    QRgb ruler_color;
    QRgb accept_ruler_color;
} preview_wrapper;

typedef struct Dialog_wrapper {
    int opensave_open_thumbnail;
    int opensave_save_thumbnail;
    unsigned char opensave_recent_max_files;
    unsigned char opensave_trim_dst_num_jumps;
    int printing_use_last_device;
    int printing_disable_bg;
    int grid_show_on_load;
    int grid_show_origin;
    int grid_color_match_crosshair;
    int grid_load_from_file;
    int grid_center_on_origin;
    float grid_center_x;
    float grid_center_y;
    float grid_size_x;
    float grid_size_y;
    float grid_spacing_x;
    float grid_spacing_y;
    float grid_size_radius;
    float grid_spacing_radius;
    float grid_spacing_angle;
    int ruler_show_on_load;
    int ruler_metric;
    unsigned char ruler_pixel_size;
    int qsnap_enabled;
    unsigned char qsnap_locator_size;
    unsigned char qsnap_aperture_size;
    float lwt_default_lwt;
    unsigned char selection_grip_size;
    unsigned char selection_pickbox_size;
    unsigned char display_crosshair_percent;
    int general_tip_of_the_day;
    int general_system_help_browser;
    int display_use_opengl;
    int display_renderhint_aa;
    int display_renderhint_text_aa;
    int display_renderhint_smooth_pix;
    int display_renderhint_high_aa;
    int display_renderhint_noncosmetic;
    int display_show_scrollbars;
    int display_scrollbar_widget_num;
    int general_icon_size;
    int general_mdi_bg_use_logo;
    int general_mdi_bg_use_texture;
    int general_mdi_bg_use_color;
    int qsnap_endpoint;
    int qsnap_midpoint;
    int qsnap_center;
    int qsnap_node;
    int qsnap_quadrant;
    int qsnap_intersection;
    int qsnap_extension;
    int qsnap_insertion;
    int qsnap_perpendicular;
    int qsnap_tangent;
    int qsnap_nearest;
    int qsnap_apparent;
    int qsnap_parallel;
    int lwt_show_lwt;
    int lwt_real_render;
    int selection_mode_pickfirst;
    int selection_mode_pickadd;
    int selection_mode_pickdrag;

    unsigned char display_selectbox_alpha;
    float display_zoomscale_in;
    float display_zoomscale_out;
    QString general_language;
    QString general_icon_theme;
    QString general_mdi_bg_logo;
    QString general_mdi_bg_texture;
    QRgb    general_mdi_bg_color;
    QRgb    display_crosshair_color;
    QRgb    display_bg_color;
    QRgb    display_selectbox_left_color;
    QRgb    display_selectbox_left_fill;
    QRgb    display_selectbox_right_color;
    QRgb    display_selectbox_right_fill;
    QString display_units;
    QString opensave_custom_filter;
    QString opensave_open_format;
    QString opensave_save_format;
    QString printing_default_device;
    QRgb    grid_color;
    QString grid_type;
    QRgb    ruler_color;
    QRgb    qsnap_locator_color;
    QRgb    selection_coolgrip_color;
    QRgb    selection_hotgrip_color;
} dialog_wrapper;

typedef struct Settings_wrapper {
    int window_width;
    int window_height;
    int window_x;
    int window_y;
    int general_icon_size;
    int general_mdi_bg_use_logo;
    int general_mdi_bg_use_texture;
    int general_mdi_bg_use_color;
    int qsnap_endpoint;
    int qsnap_midpoint;
    int qsnap_center;
    int qsnap_node;
    int qsnap_quadrant;
    int qsnap_intersection;
    int qsnap_extension;
    int qsnap_insertion;
    int qsnap_perpendicular;
    int qsnap_tangent;
    int qsnap_nearest;
    int qsnap_apparent;
    int qsnap_parallel;
    int grid_center_on_origin;
    float grid_center_x;
    float grid_center_y;
    float grid_size_x;
    float grid_size_y;
    float grid_spacing_x;
    float grid_spacing_y;
    float grid_size_radius;
    float grid_spacing_radius;
    float grid_spacing_angle;
    int ruler_show_on_load;
    int ruler_metric;
    int general_tip_of_the_day;
    int general_system_help_browser;
    int general_check_for_updates;
    int display_use_opengl;
    int display_renderhint_aa;
    int display_renderhint_text_aa;
    int display_renderhint_smooth_pix;
    int display_renderhint_high_aa;
    int display_renderhint_noncosmetic;
    int display_show_scrollbars;
    int display_scrollbar_widget_num;
    float display_zoomscale_in;
    float display_zoomscale_out;
    unsigned char display_selectbox_alpha;
    unsigned char display_crosshair_percent;
    int opensave_open_thumbnail;
    int opensave_save_thumbnail;
    unsigned char opensave_recent_max_files;
    unsigned char opensave_trim_dst_num_jumps;
    int printing_use_last_device;
    int printing_disable_bg;
    int grid_show_on_load;
    int grid_show_origin;
    int grid_color_match_crosshair;
    int grid_load_from_file;
    unsigned char ruler_pixel_size;
    int qsnap_enabled;
    unsigned char qsnap_locator_size;
    unsigned char qsnap_aperture_size;
    int lwt_show_lwt;
    int lwt_real_render;
    float lwt_default_lwt;
    int selection_mode_pickfirst;
    int selection_mode_pickadd;
    int selection_mode_pickdrag;
    unsigned char selection_grip_size;
    unsigned char selection_pickbox_size;
    float text_size;
    float text_angle;
    int text_style_bold;
    int text_style_italic;
    int text_style_underline;
    int text_style_overline;
    int text_style_strikeout;

    int file_toolbar[20];
    int edit_toolbar[20];
    int view_toolbar[20];
    int pan_toolbar[20];
    int icon_toolbar[20];
    int help_toolbar[20];
    int zoom_toolbar[20];
    char *folders[20];
    command command_list[20];
    quad quad_list1[20];
    quad quad_list2[20];

    int rulerPixelSize;
    int gripSize;
    int pickBoxSize;
    int crosshairSize;

    int shiftKeyPressedState;

    int grippingActive;
    int rapidMoveActive;
    int previewActive;
    int pastingActive;
    int movingActive;
    int selectingActive;
    int zoomWindowActive;
    int panningRealTimeActive;
    int panningPointActive;
    int panningActive;
    int qSnapActive;
    int qSnapToggle;

    int rulerMetric;

    QString general_language;
    QString general_icon_theme;
    QString general_mdi_bg_logo;
    QString general_mdi_bg_texture;
    QRgb general_mdi_bg_color;
    unsigned short general_current_tip;
    QRgb display_crosshair_color;
    QRgb display_bg_color;
    QRgb display_selectbox_left_color;
    QRgb display_selectbox_left_fill;
    QRgb display_selectbox_right_color;
    QRgb display_selectbox_right_fill;
    QString display_units;
    QString opensave_custom_filter;
    QString opensave_open_format;
    QString opensave_save_format;
    QStringList opensave_recent_list_of_files;
    QString opensave_recent_directory;
    QString printing_default_device;
    QRgb grid_color;
    QRgb ruler_color;
    QRgb qsnap_locator_color;
    QString grid_type;
    QRgb selection_coolgrip_color;
    QRgb selection_hotgrip_color;
    QString text_font;
} settings_wrapper;

extern MainWindow* _mainWin;
extern path_symbol *symbol_list[20];
extern char **icon_table[300];
extern int n_toolbars, n_actions, n_menus;
extern int *toolbars[];
extern char *toolbar_label[];
extern int actions_indices[];
extern const char *actions_strings[];
extern int *menus[];
extern char *menu_label[];

/* Class based code */
class LayerManager : public QDialog
{
    Q_OBJECT

public:
    LayerManager(MainWindow* mw, QWidget *parent = 0);
    ~LayerManager();

    void addLayer(const QString& name,
  const bool visible,
  const bool frozen,
  const float zValue,
  const QRgb color,
  const QString& lineType,
  const QString& lineWeight,
  const bool print);

    QStandardItemModel*    layerModel;
    QSortFilterProxyModel* layerModelSorted;
    QTreeView* treeView;
};

// On Mac, if the user drops a file on the app's Dock icon, or uses Open As, then this is how the app actually opens the file.
class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int argc, char **argv);
    void setMainWin(MainWindow* mainWin) { _mainWin = mainWin; }
protected:
    virtual bool event(QEvent *e);
};

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    ImageWidget(const QString &filename, QWidget* parent = 0);
    ~ImageWidget();

    bool load(const QString &fileName);
    bool save(const QString &fileName);
    QImage img;

protected:
    void paintEvent(QPaintEvent* event);
};

class MdiWindow: public QMdiSubWindow
{
    Q_OBJECT

public:
    MdiWindow(const int theIndex, MainWindow* mw, QMdiArea* parent, Qt::WindowFlags wflags);
    ~MdiWindow();

    virtual QSize  sizeHint() const;
    QString    getCurrentFile()   { return curFile; }
    QString    getShortCurrentFile();
    View*  getView() { return gview; }
    QGraphicsScene*    getScene() { return gscene; }
    QString    getCurrentLayer() { return curLayer; }
    QRgb   getCurrentColor() { return curColor; }
    QString    getCurrentLineType() { return curLineType; }
    QString    getCurrentLineWeight() { return curLineWeight; }
    void   setCurrentLayer(const QString& layer) { curLayer = layer; }
    void   setCurrentColor(const QRgb& color) { curColor = color; }
    void   setCurrentLineType(const QString& lineType) { curLineType = lineType; }
    void   setCurrentLineWeight(const QString& lineWeight) { curLineWeight = lineWeight; }
    void   designDetails();
    bool   loadFile(const QString &fileName);
    bool   saveFile(const QString &fileName);
signals:
    void   sendCloseMdiWin(MdiWindow*);

private:
    MainWindow*    mainWin;
    QMdiArea*  mdiArea;
    QGraphicsScene*    gscene;
    View*  gview;

    int fileWasLoaded;

    /* QPrinter   printer; */

    QString curFile;
    void setCurrentFile(const QString& fileName);
    QString fileExtension(const QString& fileName);

    int myIndex;

    QString curLayer;
    QRgb curColor;
    QString curLineType;
    QString curLineWeight;

public slots:
    void   closeEvent(QCloseEvent* e);
    void   onWindowActivated();

    void   currentLayerChanged(const QString& layer);
    void   currentColorChanged(const QRgb& color);
    void   currentLinetypeChanged(const QString& type);
    void   currentLineweightChanged(const QString& weight);

    void   updateColorLinetypeLineweight();
    void   deletePressed();
    void   escapePressed();

    void   showViewScrollBars(bool val);
    void   setViewCrossHairColor(QRgb color);
    void   setViewBackgroundColor(QRgb color);
    void   setViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha);
    void   setViewGridColor(QRgb color);
    void   setViewRulerColor(QRgb color);

    void   print();
    void   saveBMC();
};

class EmbDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    EmbDetailsDialog(QGraphicsScene* theScene, QWidget *parent = 0);
    ~EmbDetailsDialog();

    QWidget* mainWidget;

    void getInfo();
    QWidget* createMainWidget();
    QWidget* createHistogram();

    QDialogButtonBox* buttonBox;

    unsigned int stitchesTotal;
    unsigned int stitchesReal;
    unsigned int stitchesJump;
    unsigned int stitchesTrim;
    unsigned int colorTotal;
    unsigned int colorChanges;

    QRectF boundingRect;
};

class MdiArea : public QMdiArea
{
    Q_OBJECT

public:
    MdiArea(MainWindow* mw, QWidget* parent = 0);
    ~MdiArea();

    void useBackgroundLogo(bool use);
    void useBackgroundTexture(bool use);
    void useBackgroundColor(bool use);

    void setBackgroundLogo(const QString& fileName);
    void setBackgroundTexture(const QString& fileName);
    void setBackgroundColor(const QColor& color);

    MainWindow* mainWin;

    int useLogo;
    int useTexture;
    int useColor;

    QPixmap bgLogo;
    QPixmap bgTexture;
    QColor  bgColor;

    void zoomExtentsAllSubWindows();
    void forceRepaint();

public slots:
    void cascade();
    void tile();
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* e);
    virtual void paintEvent(QPaintEvent* e);
};

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    MdiArea*    getMdiArea();
    MainWindow* getApplication();
    MdiWindow*  activeMdiWindow();
    View*   activeView();
    QGraphicsScene* activeScene();
    QUndoStack* activeUndoStack();

    void    setUndoCleanIcon(bool opened);

    virtual void    updateMenuToolbarStatusbar();

    MainWindow* mainWin;
    MdiArea*    mdiArea;
    PropertyEditor* dockPropEdit;
    UndoEditor* dockUndoEdit;
    StatusBar*  statusbar;

    QList<QGraphicsItem*> cutCopyObjectList;

    QHash<int, QAction*>    actionHash;
    QHash<QString, QToolBar*>   toolbarHash;
    QHash<QString, QMenu*>  menuHash;

    QString formatFilterOpen;
    QString formatFilterSave;

    QString platformString();

    QByteArray layoutState;

    int numOfDocs;
    int docIndex;

    QList<MdiWindow*> listMdiWin;
    QMdiSubWindow* findMdiWindow(const QString &fileName);
    QString openFilesPath;

    QAction* myFileSeparator;

    QWizard*    wizardTipOfTheDay;
    QLabel* labelTipOfTheDay;
    QCheckBox*  checkBoxTipOfTheDay;
    QStringList listTipOfTheDay;

    QComboBox* layerSelector;
    QComboBox* colorSelector;
    QComboBox* linetypeSelector;
    QComboBox* lineweightSelector;
    QFontComboBox* textFontSelector;
    QComboBox* textSizeSelector;
    void createEditMenu();
    void createViewMenu();
    void createSettingsMenu();
    void createWindowMenu();
    void createHelpMenu();

public slots:

    void enableMoveRapidFire();
    void disableMoveRapidFire();

    void    onCloseWindow();
    virtual void    onCloseMdiWin(MdiWindow*);

    void    recentMenuAboutToShow();

    void    onWindowActivated (QMdiSubWindow* w);
    void    windowMenuAboutToShow();
    void    windowMenuActivated(bool checked/*int id*/ );
    QAction*    getAction(int actionEnum);

    void    updateAllViewScrollBars(bool val);
    void    updateAllViewCrossHairColors(QRgb color);
    void    updateAllViewBackgroundColors(QRgb color);
    void    updateAllViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha);
    void    updateAllViewGridColors(QRgb color);
    void    updateAllViewRulerColors(QRgb color);

    void    updatePickAddMode(bool val);
    void    pickAddModeToggled();

    void    settingsDialog(const QString& showTab = QString());
    void    readSettings();
    void    writeSettings();

    static bool    validFileFormat(const QString &fileName);

    QAction*    createAction(const QString icon, const QString toolTip, const QString statusTip, bool scripted = false);

    void createAllToolbars();
    void createLayerToolbar();
    void createPropertiesToolbar();
    void createTextToolbar();

protected:
    virtual void    resizeEvent(QResizeEvent*);
    void    closeEvent(QCloseEvent *event);
    QAction*    getFileSeparator();
    void    loadFormats();

private slots:
    void hideUnimplemented();

public slots:

    void stub_implement(QString txt);
    void stub_testing();

    void newFile();
    void openFile(bool recent = false, const QString& recentFile = "");
    void openFilesSelected(const QStringList&);
    void openrecentfile();
    void savefile();
    void saveasfile();
    void print();
    void designDetails();
    void exit();
    void quit();
    void checkForUpdates();
    // Help Menu
    void tipOfTheDay();
    void buttonTipOfTheDayClicked(int);
    void checkBoxTipOfTheDayStateChanged(int);
    void help();
    void changelog();
    void about();
    void whatsThisContextHelp();

    void actions();

    void cut();
    void copy();
    void paste();
    void selectAll();

    void closeToolBar(QAction*);
    void floatingChangedToolBar(bool);

    void toggleGrid();
    void toggleRuler();
    void toggleLwt();

    // Icons
    void iconResize(int iconSize);

    //Selectors
    void layerSelectorIndexChanged(int index);
    void colorSelectorIndexChanged(int index);
    void linetypeSelectorIndexChanged(int index);
    void lineweightSelectorIndexChanged(int index);
    void textFontSelectorCurrentFontChanged(const QFont& font);
    void textSizeSelectorIndexChanged(int index);

    QString textFont();
    float   textSize();
    float   textAngle();
    bool    textBold();
    bool    textItalic();
    bool    textUnderline();
    bool    textStrikeOut();
    bool    textOverline();

    void setTextFont(const QString& str);
    void setTextSize(float num);
    void setTextAngle(float num);
    void setTextBold(bool val);
    void setTextItalic(bool val);
    void setTextUnderline(bool val);
    void setTextStrikeOut(bool val);
    void setTextOverline(bool val);

    QString getCurrentLayer();
    QRgb    getCurrentColor();
    QString getCurrentLineType();
    QString getCurrentLineWeight();

    // Standard Slots
    void undo();
    void redo();

    bool isShiftPressed();
    void setShiftPressed();
    void setShiftReleased();

    void deletePressed();
    void escapePressed();

    // Layer Toolbar
    void makeLayerActive();
    void layerManager();
    void layerPrevious();
    // Zoom Toolbar
    void zoomRealtime();
    void zoomPrevious();
    void zoomWindow();
    void zoomDynamic();
    void zoomScale();
    void zoomCenter();
    void zoomIn();
    void zoomOut();
    void zoomSelected();
    void zoomAll();
    void zoomExtents();
    // Pan SubMenu
    void panrealtime();
    void panpoint();
    void panLeft();
    void panRight();
    void panUp();
    void panDown();

    void dayVision();
    void nightVision();

    void doNothing();

public:
    //Natives
    void nativeAlert  (const QString& txt);
    void nativeInitCommand    ();
    void nativeEndCommand ();

    void nativeEnableMoveRapidFire    ();
    void nativeDisableMoveRapidFire   ();

    void nativeWindowCascade  ();
    void nativeWindowTile ();
    void nativeWindowClose    ();
    void nativeWindowCloseAll ();
    void nativeWindowNext ();
    void nativeWindowPrevious ();

    QString nativePlatformString  ();

    void nativeMessageBox (const QString& type, const QString& title, const QString& text);

    void nativePrintArea  (float x, float y, float w, float h);

    void nativeSetBackgroundColor (unsigned char r, unsigned char g, unsigned char b);
    void nativeSetCrossHairColor  (unsigned char r, unsigned char g, unsigned char b);
    void nativeSetGridColor   (unsigned char r, unsigned char g, unsigned char b);

    QString nativeTextFont    ();
    float   nativeTextSize    ();
    float   nativeTextAngle   ();
    bool    nativeTextBold    ();
    bool    nativeTextItalic  ();
    bool    nativeTextUnderline   ();
    bool    nativeTextStrikeOut   ();
    bool    nativeTextOverline    ();

    void nativeSetTextFont(const QString& str);
    void nativeSetTextSize(float num);
    void nativeSetTextAngle(float num);
    void nativeSetTextBold(bool val);
    void nativeSetTextItalic(bool val);
    void nativeSetTextUnderline   (bool val);
    void nativeSetTextStrikeOut   (bool val);
    void nativeSetTextOverline    (bool val);

    void nativePreviewOn  (int clone, int mode, float x, float y, float data);
    void nativePreviewOff ();

    void nativeVulcanize  ();
    void nativeClearRubber    ();
    bool nativeAllowRubber    ();
    void nativeSpareRubber    (int id);
    //TODO: void nativeSetRubberFilter(int id); //TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once
    void nativeSetRubberMode  (int mode);
    void nativeSetRubberPoint (const QString& key, float x, float y);
    void nativeSetRubberText  (const QString& key, const QString& txt);

    void nativeAddTextMulti(const QString& str, float x, float y, float rot, bool fill, int rubberMode);
    void nativeAddTextSingle(const QString& str, float x, float y, float rot, bool fill, int rubberMode);

    void nativeAddInfiniteLine(float x1, float y1, float x2, float y2, float rot);
    void nativeAddRay(float x1, float y1, float x2, float y2, float rot);
    void nativeAddLine    (float x1, float y1, float x2, float y2, float rot, int rubberMode);
    void nativeAddTriangle    (float x1, float y1, float x2, float y2, float x3, float y3, float rot, bool fill);
    void nativeAddRectangle   (float x, float y, float w, float h, float rot, bool fill, int rubberMode);
    void nativeAddRoundedRectangle    (float x, float y, float w, float h, float rad, float rot, bool fill);
    void nativeAddArc (float startX, float startY, float midX, float midY, float endX, float endY, int rubberMode);
    void nativeAddCircle  (float centerX, float centerY, float radius, bool fill, int rubberMode);
    void nativeAddSlot    (float centerX, float centerY, float diameter, float length, float rot, bool fill, int rubberMode);
    void nativeAddEllipse (float centerX, float centerY, float width, float height, float rot, bool fill, int rubberMode);
    void nativeAddPoint   (float x, float y);
    void nativeAddRegularPolygon  (float centerX, float centerY, unsigned short sides, unsigned char mode, float rad, float rot, bool fill);
    void nativeAddPolygon (float startX, float startY, const QPainterPath& p, int rubberMode);
    void nativeAddPolyline(float startX, float startY, const QPainterPath& p, int rubberMode);
    void nativeAddPath(EmbVector start, const QPainterPath& p, int rubberMode);
    void nativeAddHorizontalDimension(EmbVector start, EmbVector end, float legHeight);
    void nativeAddVerticalDimension(EmbVector start, EmbVector end, float legHeight);
    void nativeAddImage(const QString& img, EmbRect r, float rot);

    void nativeAddDimLeader(float x1, float y1, float x2, float y2, float rot, int rubberMode);

    void  nativeSetCursorShape(const QString& str);
    float nativeCalculateAngle(float x1, float y1, float x2, float y2);
    float nativeCalculateDistance(float x1, float y1, float x2, float y2);
    float nativePerpendicularDistance (float px, float py, float x1, float y1, float x2, float y2);

    int  nativeNumSelected    ();
    void nativeSelectAll  ();
    void nativeAddToSelection (const QPainterPath path, Qt::ItemSelectionMode mode);
    void nativeClearSelection ();
    void nativeDeleteSelected ();
    void nativeCutSelected    (float x, float y);
    void nativeCopySelected   (float x, float y);
    void nativePasteSelected  (float x, float y);
    void nativeMoveSelected   (float dx, float dy);
    void nativeScaleSelected  (float x, float y, float factor);
    void nativeRotateSelected (float x, float y, float rot);
    void nativeMirrorSelected (float x1, float y1, float x2, float y2);

    float nativeQSnapX();
    float nativeQSnapY();
    float nativeMouseX();
    float nativeMouseY();
};

class PreviewDialog : public QFileDialog
{
    Q_OBJECT

public:
    PreviewDialog(QWidget* parent = 0,
  const QString& caption = QString(),
  const QString& directory = QString(),
  const QString& filter = QString());
    ~PreviewDialog();

private:
    ImageWidget* imgWidget;
};


class SaveObject : public QObject
{
    Q_OBJECT

public:
    SaveObject(QGraphicsScene* theScene, QObject* parent = 0);
    ~SaveObject();

    bool save(const QString &fileName);

    void addArc  (EmbPattern* pattern, QGraphicsItem* item);
    void addBlock    (EmbPattern* pattern, QGraphicsItem* item);
    void addCircle   (EmbPattern* pattern, QGraphicsItem* item);
    void addDimAligned   (EmbPattern* pattern, QGraphicsItem* item);
    void addDimAngular   (EmbPattern* pattern, QGraphicsItem* item);
    void addDimArcLength (EmbPattern* pattern, QGraphicsItem* item);
    void addDimDiameter  (EmbPattern* pattern, QGraphicsItem* item);
    void addDimLeader    (EmbPattern* pattern, QGraphicsItem* item);
    void addDimLinear    (EmbPattern* pattern, QGraphicsItem* item);
    void addDimOrdinate  (EmbPattern* pattern, QGraphicsItem* item);
    void addDimRadius    (EmbPattern* pattern, QGraphicsItem* item);
    void addEllipse  (EmbPattern* pattern, QGraphicsItem* item);
    void addEllipseArc   (EmbPattern* pattern, QGraphicsItem* item);
    void addGrid (EmbPattern* pattern, QGraphicsItem* item);
    void addHatch    (EmbPattern* pattern, QGraphicsItem* item);
    void addImage    (EmbPattern* pattern, QGraphicsItem* item);
    void addInfiniteLine (EmbPattern* pattern, QGraphicsItem* item);
    void addLine (EmbPattern* pattern, QGraphicsItem* item);
    void addPath (EmbPattern* pattern, QGraphicsItem* item);
    void addPoint    (EmbPattern* pattern, QGraphicsItem* item);
    void addPolygon  (EmbPattern* pattern, QGraphicsItem* item);
    void addPolyline (EmbPattern* pattern, QGraphicsItem* item);
    void addRay  (EmbPattern* pattern, QGraphicsItem* item);
    void addRectangle    (EmbPattern* pattern, QGraphicsItem* item);
    void addSlot (EmbPattern* pattern, QGraphicsItem* item);
    void addSpline   (EmbPattern* pattern, QGraphicsItem* item);
    void addTextMulti    (EmbPattern* pattern, QGraphicsItem* item);
    void addTextSingle   (EmbPattern* pattern, QGraphicsItem* item);

private:
    QGraphicsScene* gscene;
    int formatType;

    void toPolyline(EmbPattern* pattern, const QPointF& objPos, const QPainterPath& objPath, const QString& layer, const QColor& color, const QString& lineType, const QString& lineWeight);
};

class PropertyEditor : public QDockWidget
{
    Q_OBJECT

public:
    PropertyEditor(const QString& iconDirectory = QString(), bool pickAddMode = true, QWidget* widgetToFocus = 0, QWidget* parent = 0, Qt::WindowFlags flags = Qt::Widget);
    ~PropertyEditor();

    QGroupBox*   createGroupBoxMiscImage();
    QGroupBox*   createGroupBoxGeneral();
    QGroupBox*   createGroupBoxGeometryArc();
    QGroupBox*   createGroupBoxMiscArc();
    QGroupBox*   createGroupBoxGeometryBlock();
    QGroupBox*   createGroupBoxGeometryCircle();
    QGroupBox*   createGroupBoxGeometryDimAligned();
    QGroupBox*   createGroupBoxGeometryDimAngular();
    QGroupBox*   createGroupBoxGeometryDimArcLength();
    QGroupBox*   createGroupBoxGeometryDimDiameter();
    QGroupBox*   createGroupBoxGeometryDimLeader();
    QGroupBox*   createGroupBoxGeometryDimLinear();
    QGroupBox*   createGroupBoxGeometryDimOrdinate();
    QGroupBox*   createGroupBoxGeometryDimRadius();
    QGroupBox*   createGroupBoxGeometryEllipse();
    QGroupBox*   createGroupBoxGeometryImage();
    QGroupBox*   createGroupBoxGeometryInfiniteLine();
    QGroupBox*   createGroupBoxGeometryLine();
    QGroupBox*   createGroupBoxGeometryPath();
    QGroupBox*   createGroupBoxMiscPath();
    QGroupBox*   createGroupBoxGeometryPoint();
    QGroupBox*   createGroupBoxGeometryPolygon();
    QGroupBox*   createGroupBoxGeometryPolyline();
    QGroupBox*   createGroupBoxMiscPolyline();
    QGroupBox*   createGroupBoxGeometryRay();
    QGroupBox*   createGroupBoxGeometryRectangle();
    QGroupBox*   createGroupBoxGeometryTextMulti();
    QGroupBox*   createGroupBoxTextTextSingle();
    QGroupBox*   createGroupBoxGeometryTextSingle();
    QGroupBox*   createGroupBoxMiscTextSingle();
    
protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void pickAddModeToggled();

public slots:
    void setSelectedItems(QList<QGraphicsItem*> itemList);
    void updatePickAddModeButton(bool pickAddMode);

private slots:
    void fieldEdited(QObject* fieldObj);
    void showGroups(int objType);
    void showOneType(int index);
    void hideAllGroups();
    void clearAllFields();
    void togglePickAddMode();

private:
    QWidget* focusWidget;

    QString  iconDir;
    int  iconSize;
    Qt::ToolButtonStyle propertyEditorButtonStyle;

    int pickAdd;

    QList<QGraphicsItem*> selectedItemList;

    //Helper functions
    QToolButton*   createToolButton(const QString& iconName, const QString& txt);
    QLineEdit* createLineEdit(const QString& validatorType = QString(), bool readOnly = false);
    QComboBox* createComboBox(bool disable = false);
    QFontComboBox* createFontComboBox(bool disable = false);


    void updateLineEditStrIfVaries(QLineEdit* lineEdit, const QString& str);
    void updateLineEditNumIfVaries(QLineEdit* lineEdit, float num, bool useAnglePrecision);
    void updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, const QString& str);
    void updateComboBoxStrIfVaries(QComboBox* comboBox, const QString& str, const QStringList& strList);
    void updateComboBoxBoolIfVaries(QComboBox* comboBox, bool val, bool yesOrNoText);

    QSignalMapper* signalMapper;
    void mapSignal(QObject* fieldObj, const QString& name, QVariant value);

    QComboBox*   createComboBoxSelected();
    QToolButton* createToolButtonQSelect();
    QToolButton* createToolButtonPickAdd();

    QComboBox*   comboBoxSelected;
    QToolButton* toolButtonQSelect;
    QToolButton* toolButtonPickAdd;

    //TODO: Alphabetic/Categorized TabWidget

};

class SelectBox : public QRubberBand
{
    Q_OBJECT

public:
    SelectBox(Shape s, QWidget* parent = 0);

    EmbColor leftBrushColor;
    QColor rightBrushColor;
    QColor leftPenColor;
    QColor rightPenColor;
    unsigned char alpha;

    QBrush dirBrush;
    QBrush leftBrush;
    QBrush rightBrush;

    QPen dirPen;
    QPen leftPen;
    QPen rightPen;

    int boxDir;

public slots:
    void setDirection(int dir);
    void setColors(const QColor& colorL, const QColor& fillL, const QColor& colorR, const QColor& fillR, int newAlpha);

protected:
    void paintEvent(QPaintEvent*);

private:
    void forceRepaint();
};

class Settings_Dialog : public QDialog
{
    Q_OBJECT

public:
    Settings_Dialog(MainWindow* mw, const QString& showTab = QString(), QWidget *parent = 0);
    ~Settings_Dialog();

    MainWindow*   mainWin;

    QTabWidget*   tabWidget;

    QDialogButtonBox* buttonBox;

    void addColorsToComboBox(QComboBox* comboBox);

    /* Functions */
    QWidget* createTabGeneral();
    QWidget* createTabFilesPaths();
    QWidget* createTabDisplay();
    QWidget* createTabOpenSave();
    QWidget* createTabPrinting();
    QWidget* createTabSnap();
    QWidget* createTabGridRuler();
    QWidget* createTabOrthoPolar();
    QWidget* createTabQuickSnap();
    QWidget* createTabQuickTrack();
    QWidget* createTabLineWeight();
    QWidget* createTabSelection();

private slots:
    void comboBoxLanguageCurrentIndexChanged(const QString&);
    void comboBoxIconThemeCurrentIndexChanged(const QString&);
    void comboBoxIconSizeCurrentIndexChanged(int);
    void checkBoxGeneralMdiBGUseLogoStateChanged(int);
    void chooseGeneralMdiBackgroundLogo();
    void checkBoxGeneralMdiBGUseTextureStateChanged(int);
    void chooseGeneralMdiBackgroundTexture();
    void checkBoxGeneralMdiBGUseColorStateChanged(int);
    void chooseGeneralMdiBackgroundColor();
    void currentGeneralMdiBackgroundColorChanged(const QColor&);
    void checkBoxTipOfTheDayStateChanged(int);
    void checkBoxUseOpenGLStateChanged(int);
    void checkBoxRenderHintAAStateChanged(int);
    void checkBoxRenderHintTextAAStateChanged(int);
    void checkBoxRenderHintSmoothPixStateChanged(int);
    void checkBoxRenderHintHighAAStateChanged(int);
    void checkBoxRenderHintNonCosmeticStateChanged(int);
    void checkBoxShowScrollBarsStateChanged(int);
    void comboBoxScrollBarWidgetCurrentIndexChanged(int);
    void spinBoxZoomScaleInValueChanged(double);
    void spinBoxZoomScaleOutValueChanged(double);
    void checkBoxDisableBGStateChanged(int);
    void chooseDisplayCrossHairColor();
    void currentDisplayCrossHairColorChanged(const QColor&);
    void chooseDisplayBackgroundColor();
    void currentDisplayBackgroundColorChanged(const QColor&);
    void chooseDisplaySelectBoxLeftColor();
    void currentDisplaySelectBoxLeftColorChanged(const QColor&);
    void chooseDisplaySelectBoxLeftFill();
    void currentDisplaySelectBoxLeftFillChanged(const QColor&);
    void chooseDisplaySelectBoxRightColor();
    void currentDisplaySelectBoxRightColorChanged(const QColor&);
    void chooseDisplaySelectBoxRightFill();
    void currentDisplaySelectBoxRightFillChanged(const QColor&);
    void spinBoxDisplaySelectBoxAlphaValueChanged(int);
    void checkBoxCustomFilterStateChanged(int);
    void buttonCustomFilterSelectAllClicked();
    void buttonCustomFilterClearAllClicked();
    void spinBoxRecentMaxFilesValueChanged(int);
    void spinBoxTrimDstNumJumpsValueChanged(int);
    void checkBoxGridShowOnLoadStateChanged(int);
    void checkBoxGridShowOriginStateChanged(int);
    void checkBoxGridColorMatchCrossHairStateChanged(int);
    void chooseGridColor();
    void currentGridColorChanged(const QColor&);
    void checkBoxGridLoadFromFileStateChanged(int);
    void comboBoxGridTypeCurrentIndexChanged(const QString&);
    void checkBoxGridCenterOnOriginStateChanged(int);
    void spinBoxGridCenterXValueChanged(double);
    void spinBoxGridCenterYValueChanged(double);
    void spinBoxGridSizeXValueChanged(double);
    void spinBoxGridSizeYValueChanged(double);
    void spinBoxGridSpacingXValueChanged(double);
    void spinBoxGridSpacingYValueChanged(double);
    void spinBoxGridSizeRadiusValueChanged(double);
    void spinBoxGridSpacingRadiusValueChanged(double);
    void spinBoxGridSpacingAngleValueChanged(double);
    void checkBoxRulerShowOnLoadStateChanged(int);
    void comboBoxRulerMetricCurrentIndexChanged(int);
    void chooseRulerColor();
    void currentRulerColorChanged(const QColor&);
    void spinBoxRulerPixelSizeValueChanged(double);
    void checkBoxQSnapEndPointStateChanged(int);
    void checkBoxQSnapMidPointStateChanged(int);
    void checkBoxQSnapCenterStateChanged(int);
    void checkBoxQSnapNodeStateChanged(int);
    void checkBoxQSnapQuadrantStateChanged(int);
    void checkBoxQSnapIntersectionStateChanged(int);
    void checkBoxQSnapExtensionStateChanged(int);
    void checkBoxQSnapInsertionStateChanged(int);
    void checkBoxQSnapPerpendicularStateChanged(int);
    void checkBoxQSnapTangentStateChanged(int);
    void checkBoxQSnapNearestStateChanged(int);
    void checkBoxQSnapApparentStateChanged(int);
    void checkBoxQSnapParallelStateChanged(int);
    void buttonQSnapSelectAllClicked();
    void buttonQSnapClearAllClicked();
    void comboBoxQSnapLocatorColorCurrentIndexChanged(int);
    void sliderQSnapLocatorSizeValueChanged(int);
    void sliderQSnapApertureSizeValueChanged(int);
    void checkBoxLwtShowLwtStateChanged(int);
    void checkBoxLwtRealRenderStateChanged(int);
    void checkBoxSelectionModePickFirstStateChanged(int);
    void checkBoxSelectionModePickAddStateChanged(int);
    void checkBoxSelectionModePickDragStateChanged(int);
    void comboBoxSelectionCoolGripColorCurrentIndexChanged(int);
    void comboBoxSelectionHotGripColorCurrentIndexChanged(int);
    void sliderSelectionGripSizeValueChanged(int);
    void sliderSelectionPickBoxSizeValueChanged(int);

    void acceptChanges();
    void rejectChanges();

signals:
    void buttonCustomFilterSelectAll(bool);
    void buttonCustomFilterClearAll(bool);
    void buttonQSnapSelectAll(bool);
    void buttonQSnapClearAll(bool);
};

class StatusBarButton : public QToolButton
{
    Q_OBJECT

public:
    StatusBarButton(QString buttonText, MainWindow* mw, StatusBar* statbar, QWidget *parent = 0);

protected:
    void contextMenuEvent(QContextMenuEvent *event = 0);

private slots:
    void settingsSnap();
    void settingsGrid();
    void settingsRuler();
    void settingsOrtho();
    void settingsPolar();
    void settingsQSnap();
    void settingsQTrack();
    void settingsLwt();
    void toggleSnap(bool on);
    void toggleGrid(bool on);
    void toggleRuler(bool on);
    void toggleOrtho(bool on);
    void togglePolar(bool on);
    void toggleQSnap(bool on);
    void toggleQTrack(bool on);
    void toggleLwt(bool on);
public slots:
    void enableLwt();
    void disableLwt();
    void enableReal();
    void disableReal();

private:
    MainWindow* mainWin;
    StatusBar*  statusbar;
};

class StatusBar : public QStatusBar
{
    Q_OBJECT

public:
    StatusBar(MainWindow* mw, QWidget* parent = 0);

    void setMouseCoord(float x, float y);

};

class UndoEditor : public QDockWidget
{
    Q_OBJECT

public:
    UndoEditor(const QString& iconDirectory = QString(), QWidget* widgetToFocus = 0, QWidget* parent = 0, Qt::WindowFlags flags = Qt::Widget);
    ~UndoEditor();

    void addStack(QUndoStack* stack);

    bool canUndo() const;
    bool canRedo() const;

    QString undoText() const;
    QString redoText() const;

    QWidget*    focusWidget;

    QString iconDir;
    int iconSize;

    QUndoGroup* undoGroup;
    QUndoView*  undoView;

public slots:
    void undo();
    void redo();

    void updateCleanIcon(bool opened);

};

class View : public QGraphicsView
{
    Q_OBJECT

public:
    View(MainWindow* mw, QGraphicsScene* theScene, QWidget* parent);
    ~View();

    bool allowZoomIn();
    bool allowZoomOut();

    void recalculateLimits();
    void zoomToPoint(const QPoint& mousePoint, int zoomDir);
    void centerAt(const QPointF& centerPoint);
    QPointF center() { return mapToScene(rect().center()); }

    QUndoStack* getUndoStack() { return undoStack; }
    void addObject(BaseObject* obj);
    void deleteObject(BaseObject* obj);
    void vulcanizeObject(BaseObject* obj);

public slots:
    void zoomIn();
    void zoomOut();
    void zoomWindow();
    void zoomSelected();
    void zoomExtents();
    void panRealTime();
    void panPoint();
    void panLeft();
    void panRight();
    void panUp();
    void panDown();
    void selectAll();
    void selectionChanged();
    void clearSelection();
    void deleteSelected();
    void moveSelected(float dx, float dy);
    void cut();
    void copy();
    void paste();
    void repeatAction();
    void moveAction();
    void scaleAction();
    void scaleSelected(float x, float y, float factor);
    void rotateAction();
    void rotateSelected(float x, float y, float rot);
    void mirrorSelected(float x1, float y1, float x2, float y2);
    int  numSelected();

    void deletePressed();
    void escapePressed();

    void cornerButtonClicked();

    void showScrollBars(bool val);
    void setCornerButton();
    void setCrossHairColor(QRgb color);
    void setCrossHairSize(unsigned char percent);
    void setBackgroundColor(QRgb color);
    void setSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha);
    void toggleSnap(bool on);
    void toggleGrid(bool on);
    void toggleRuler(bool on);
    void toggleOrtho(bool on);
    void togglePolar(bool on);
    void toggleQSnap(bool on);
    void toggleQTrack(bool on);
    void toggleLwt(bool on);
    void toggleReal(bool on);
    bool isLwtEnabled();
    bool isRealEnabled();

    void setGridColor(QRgb color);
    void createGrid(const QString& gridType);
    void setRulerColor(QRgb color);

    void previewOn(int clone, int mode, float x, float y, float data);
    void previewOff();

    void enableMoveRapidFire();
    void disableMoveRapidFire();

    bool allowRubber();
    void addToRubberRoom(QGraphicsItem* item);
    void vulcanizeRubberRoom();
    void clearRubberRoom();
    void spareRubber(int id);
    void setRubberMode(int mode);
    void setRubberPoint(const QString& key, const QPointF& point);
    void setRubberText(const QString& key, const QString& txt);

protected:
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);
    void drawBackground(QPainter* painter, const QRectF& rect);
    void drawForeground(QPainter* painter, const QRectF& rect);
    void enterEvent(QEvent* event);

private:
    QHash<int, QGraphicsItem*> hashDeletedObjects;

    QList<int> spareRubberList;

    QColor gridColor;
    QPainterPath gridPath;
    void createGridRect();
    void createGridPolar();
    void createGridIso();
    QPainterPath originPath;
    void createOrigin();

    void loadRulerSettings();

    bool willUnderflowInt32(int a, int b);
    bool willOverflowInt32(int a, int b);
    int roundToMultiple(bool roundUp, int numToRound, int multiple);
    QPainterPath createRulerTextPath(float x, float y, QString str, float height);

    QList<QGraphicsItem*> previewObjectList;
    QGraphicsItemGroup* previewObjectItemGroup;
    QPointF previewPoint;
    float previewData;
    int previewMode;

    QList<QGraphicsItem*> createObjectList(QList<QGraphicsItem*> list);
    QPointF cutCopyMousePoint;
    QGraphicsItemGroup* pasteObjectItemGroup;

    QList<QGraphicsItem*> rubberRoomList;

    void copySelected();

    void startGripping(BaseObject* obj);
    void stopGripping(bool accept = false);

    BaseObject* gripBaseObj;
    BaseObject* tempBaseObj;

    MainWindow* mainWin;
    QGraphicsScene* gscene;
    QUndoStack* undoStack;

    SelectBox* selectBox;

    void updateMouseCoords(int x, int y);

    void panStart(const QPoint& point);
    int panDistance;
    int panStartX;
    int panStartY;

    void alignScenePointWithViewPoint(const QPointF& scenePoint, const QPoint& viewPoint);
};

#endif
#endif /* EMBROIDERMODDER_H */

