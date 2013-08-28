#ifndef MAINWINDOW_ACTIONS_H
#define MAINWINDOW_ACTIONS_H

enum FILE_FORMATS {
FILEFORMAT_NULL,
FILEFORMAT_100, //Toyota Embroidery Format
FILEFORMAT_10O, //Toyota Embroidery Format
FILEFORMAT_ART, //Bernina Embroidery Format
FILEFORMAT_BMC, //Bitmap Cache Embroidery Format
FILEFORMAT_BRO, //Bits & Volts Embroidery Format
FILEFORMAT_CND, //Melco Embroidery Format
FILEFORMAT_COL, //Embroidery Thread Color Format
FILEFORMAT_CSD, //Singer Embroidery Format
FILEFORMAT_CSV, //Comma Separated Values
FILEFORMAT_DAT, //Barudan Embroidery Format
FILEFORMAT_DEM, //Melco Embroidery Format
FILEFORMAT_DSB, //Barudan Embroidery Format
FILEFORMAT_DST, //Tajima Embroidery Format
FILEFORMAT_DSZ, //ZSK USA Embroidery Format
FILEFORMAT_DXF, //Drawing Exchange Format
FILEFORMAT_EDR, //Embird Embroidery Format
FILEFORMAT_EMD, //Elna Embroidery Format
FILEFORMAT_EXP, //Melco Embroidery Format
FILEFORMAT_EXY, //Eltac Embroidery Format
FILEFORMAT_EYS, //Sierra Expanded Embroidery Format
FILEFORMAT_FXY, //Fortron Embroidery Format
FILEFORMAT_GNC, //Great Notions Embroidery Format
FILEFORMAT_GT,  //Gold Thread Embroidery Format
FILEFORMAT_HUS, //Husqvarna Viking Embroidery Format
FILEFORMAT_INB, //Inbro Embroidery Format
FILEFORMAT_INF, //Embroidery Color Format
FILEFORMAT_JEF, //Janome Embroidery Format
FILEFORMAT_KSM, //Pfaff Embroidery Format
FILEFORMAT_MAX, //Pfaff Embroidery Format
FILEFORMAT_MIT, //Mitsubishi Embroidery Format
FILEFORMAT_NEW, //Ameco Embroidery Format
FILEFORMAT_OFM, //Melco Embroidery Format
FILEFORMAT_PCD, //Pfaff Embroidery Format
FILEFORMAT_PCM, //Pfaff Embroidery Format
FILEFORMAT_PCQ, //Pfaff Embroidery Format
FILEFORMAT_PCS, //Pfaff Embroidery Format
FILEFORMAT_PEC, //Brother Embroidery Format
FILEFORMAT_PEL, //Brother Embroidery Format
FILEFORMAT_PEM, //Brother Embroidery Format
FILEFORMAT_PES, //Brother Embroidery Format
FILEFORMAT_PHB, //Brother Embroidery Format
FILEFORMAT_PHC, //Brother Embroidery Format
FILEFORMAT_PLT, //AutoCAD Plot drawing
FILEFORMAT_RGB, //RGB Embroidery Format
FILEFORMAT_SEW, //Janome Embroidery Format
FILEFORMAT_SHV, //Husqvarna Viking Embroidery Format
FILEFORMAT_SST, //Sunstar Embroidery Format
FILEFORMAT_STX, //Data Stitch Embroidery Format
FILEFORMAT_SVG, //Scalable Vector Graphics
FILEFORMAT_T09, //Pfaff Embroidery Format
FILEFORMAT_TAP, //Happy Embroidery Format
FILEFORMAT_THR, //ThredWorks Embroidery Format
FILEFORMAT_TXT, //Text File
FILEFORMAT_U00, //Barudan Embroidery Format
FILEFORMAT_U01, //Barudan Embroidery Format
FILEFORMAT_VIP, //Pfaff Embroidery Format
FILEFORMAT_VP3, //Pfaff Embroidery Format
FILEFORMAT_XXX, //Singer Embroidery Format
FILEFORMAT_ZSK, //ZSK USA Embroidery Format
FILEFORMAT_UNKNOWN
};

enum COMMAND_ACTIONS {

ACTION_donothing,

ACTION_new,
ACTION_open,
ACTION_save,
ACTION_saveas,
ACTION_print,
ACTION_designdetails,
ACTION_exit,
ACTION_cut,
ACTION_copy,
ACTION_paste,

ACTION_undo,
ACTION_redo,
// Window Menu
ACTION_windowclose,
ACTION_windowcloseall,
ACTION_windowcascade,
ACTION_windowtile,
ACTION_windownext,
ACTION_windowprevious,
// Help Menu
ACTION_help,
ACTION_changelog,
ACTION_tipoftheday,
ACTION_about,
ACTION_aboutQt,
// Icons
ACTION_icon16,
ACTION_icon24,
ACTION_icon32,
ACTION_icon48,
ACTION_icon64,
ACTION_icon128,

ACTION_settingsdialog,

// Layer ToolBar
ACTION_makelayercurrent,
ACTION_layers,
ACTION_layerselector,
ACTION_layerprevious,
ACTION_colorselector,
ACTION_linetypeselector,
ACTION_lineweightselector,
ACTION_hidealllayers,
ACTION_showalllayers,
ACTION_freezealllayers,
ACTION_thawalllayers,
ACTION_lockalllayers,
ACTION_unlockalllayers,
//Text ToolBar
ACTION_textbold,
ACTION_textitalic,
ACTION_textunderline,
ACTION_textstrikeout,
ACTION_textoverline,
// Zoom ToolBar
ACTION_zoomrealtime,
ACTION_zoomprevious,
ACTION_zoomwindow,
ACTION_zoomdynamic,
ACTION_zoomscale,
ACTION_zoomcenter,
ACTION_zoomin,
ACTION_zoomout,
ACTION_zoomselected,
ACTION_zoomall,
ACTION_zoomextents,
// Pan SubMenu
ACTION_panrealtime,
ACTION_panpoint,
ACTION_panleft,
ACTION_panright,
ACTION_panup,
ACTION_pandown,

ACTION_spellcheck,
ACTION_quickselect,

ACTION_reduce,
ACTION_enlarge,

ACTION_null
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
