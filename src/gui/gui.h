/*
 * Embroidermodder 2 -- GUI Callbacks
 * Copyright 2011-2026 The Embroidermodder Team
 */

#ifndef __EMBROIDERMODDER_GUI_CALLBACKS__
#define __EMBROIDERMODDER_GUI_CALLBACKS__

#ifdef __cplusplus
extern "C" {
#endif

void init_command(void);
void end_command(void);
void clear_selection(void);
void set_prompt_prefix(const char *prefix);
void append_prompt_history(const char *str);
const char *translate_str(char *str);
void repaint(void);

void messagebox(const char *type, const char *title, const char *text);

void new_file(void);
void open_file(void);
void exit_program(void);

void undo_action(void);
void redo_action(void);
void repeat_action(void);

void select_all(void);
void delete_selected(void);

void about_dialog(void);
void help_dialog(void);
void tip_of_the_day_dialog(void);

void icon_resize(int size);

void window_cascade(void);
void window_close(void);
void window_close_all(void);
void window_next(void);
void window_previous(void);
void window_tile(void);

void zoom_real_time(void);
void zoom_previous(void);
void zoom_window(void);
void zoom_dynamic(void);
void zoom_scale(void);
void zoom_center(void);
void zoom_in(void);
void zoom_out(void);
void zoom_selected(void);
void zoom_all(void);
void zoom_extents(void);

void pan_real_time(void);
void pan_point(void);
void pan_left(void);
void pan_right(void);
void pan_up(void);
void pan_down(void);

void day_vision(void);
void night_vision(void);

int num_selected(void);
void delete_selected(void);
void cut_selected(float x, float y);
void copy_selected(float x, float y);
void paste_selected(float x, float y);
void move_selected(float dx, float dy);
void scale_selected(float x, float y, float factor);
void rotate_selected(float x, float y, float rot);
void mirror_selected(float x1, float y1, float x2, float y2);

float qsnapx(void);
float qsnapy(void);
float mousex(void);
float mousey(void);

#if 0
MainWindow *mainwin;

void throwError(const char *message);
void throwError(QJSValue::ErrorType type, const char *message);

/* Call a function in the command_table. */
void call(const char *command) { mainwin->call(command); }

void debug(QString msg);
void error(QString cmd, QString err);
void todo(QString cmd, QString msg);
void alert(QString message) { mainwin->alert(message); }
void blinkPrompt(void) { mainwin->blinkPrompt(); }
void setPromptPrefix(QString prefix) { mainwin->setPromptPrefix(prefix); }
void appendPromptHistory(QString message) { mainwin->appendPromptHistory(message); }
void enablePromptRapidFire(void) { mainwin->enablePromptRapidFire(); }
void disablePromptRapidFire(void) { mainwin->disablePromptRapidFire(); }
void enableMoveRapidFire(void) { mainwin->enableMoveRapidFire(); }
void disableMoveRapidFire(void) { mainwin->disableMoveRapidFire(); }
void newFile(void) { mainwin->newFile(); }
void openFile(void) { mainwin->openFile(); }
QString platformString(void) { return mainwin->platformString(); }
void messageBox(QString type, QString title, QString msg);
bool isInt(float x);
int printArea(float x, float y, float w, float h);
void dayVision(void) { mainwin->dayVision(); }
void nightVision(void) { mainwin->nightVision(); }
int setBackgroundColor(float r, float g, float b);
int setCrossHairColor(float r, float g, float b);
int setGridColor(float r, float g, float b);
QString textFont(void) { return mainwin->textFont(); }
float textSize(void) { return mainwin->textSize(); }
float textAngle(void) { return mainwin->textAngle(); }
bool textBold(void) { return mainwin->textBold(); }
bool textItalic(void) { return mainwin->textItalic(); }
bool textUnderline(void) { return mainwin->textUnderline(); }
bool textStrikeOut(void) { return mainwin->textStrikeOut(); }
bool textOverline(void) { return mainwin->textOverline(); }
void setTextFont(QString font) { mainwin->setTextFont(font); }
int setTextSize(float num);
int setTextAngle(float num);
void setTextBold(bool enabled) { mainwin->setTextBold(enabled); }
void setTextItalic(bool enabled) { mainwin->setTextItalic(enabled); }
void setTextUnderline(bool enabled) { mainwin->setTextUnderline(enabled); }
void setTextStrikeOut(bool enabled) { mainwin->setTextStrikeOut(enabled); }
void setTextOverline(bool enabled) { mainwin->setTextOverline(enabled); }
int previewOn(QString cloneStr, QString modeStr, float x, float y, float data);
void previewOff(void) { mainwin->previewOff(); }
void vulcanize(void) { mainwin->vulcanize(); }
bool allowRubber(void) { return mainwin->allowRubber(); }
int setRubberMode(QString mode);
int setRubberPoint(QString key, float x, float y);
int setRubberText(QString key, QString value);
int addRubber(QString objType);
void clearRubber(void) { mainwin->clearRubber(); }
int spareRubber(QString objID);
int addTextMulti(QString str, float x, float y, float rot, bool fill);
int addTextSingle(QString str, float x, float y, float rot, bool fill);
int addInfiniteLine(void);
int addRay(void);
int addLine(float x1, float y1, float x2, float y2, float rot);
int addTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float rot, bool fill);
int addRectangle(float x, float y, float w, float h, float rot, bool fill);
int addRoundedRectangle(float x, float y, float w, float h, float rad, float rot, bool fill);
int addArc(float startX, float startY, float midX, float midY, float endX, float endY);
int addCircle(float centerX, float centerY, float radius, bool fill);
int addSlot(float centerX, float centerY, float diameter, float length, float rot, bool fill);
int addEllipse(float centerX, float centerY, float radX, float radY, float rot, bool fill);
int addPoint(float x, float y);
int addRegularPolygon(QJSValueList args);
int addPolygon(QJSValueList args);
int addPolyline(QJSValueList args);
int addPath(void);
int addHorizontalDimension(void);
int addVerticalDimension(void);
int addImage(void);
int addDimLeader(float x1, float y1, float x2, float y2, float rot);
void setCursorShape(QString shape) { mainwin->setCursorShape(shape); }
void selectAll(void) { mainwin->selectAll(); }
int addToSelection(void);
int includeScript(QString filename);

#endif

#ifdef __cplusplus
}
#endif

#endif

