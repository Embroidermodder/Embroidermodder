/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */


#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl2.h"
#include "TextEditor.h"
#include "stb/stb_image.h"

#include "embroidermodder.h"

#include <iostream>
#include <fstream>

#include <GLFW/glfw3.h>
#include <embroidery.h>

bool running = true;
bool debug_mode = true;
bool show_about_dialog = false;
bool show_settings_editor = false;
bool show_editor = false;
std::string language = "default";
int icon_size = 16;
ImFont *font;
ImFont *header_font;
int pattern_index = 0;
std::string current_fname = "Untitled.dst";
std::string assets_dir = "../assets/";
std::vector<Action> action_list;
string_matrix translation_table;
std::unordered_map<std::string, std::string> str_settings;
std::unordered_map<std::string, GLuint> textures;
std::unordered_map<std::string, string_matrix> menu_layout;
std::string menu_action = "";
std::string current_pattern = "";
TextEditor editor;
std::vector<std::string> file_toolbar = {
    "new",
    "open",
    "save",
    "saveas",
    "print",
    "designdetails"
};
std::vector<std::string> edit_toolbar = {
    "undo",
    "redo",
    "cut",
    "copy",
    "paste"
};
std::vector<std::string> view_toolbar = {
    "day",
    "night",
    "zoomin",
    "zoomout",
    "zoomextents"
};
std::vector<std::string> help_toolbar = {
    "help"
};
std::vector<std::string> draw_toolbar = {
    "arc",
    "circle",
    "ellipse",
    "rectangle"
};
std::vector<std::string> texture_list = {
    "new",
    "open",
    "save",
    "saveas",
    "print",
    "designdetails",
    "undo",
    "redo",
    "cut",
    "copy",
    "paste",
    "day",
    "night",
    "zoomin",
    "zoomout",
    "zoomextents",
    "help",
    "arc",
    "circle",
    "ellipse"
};
int testing = 0;


int
parse_command(int argc, char *argv[], std::vector<std::string> files)
{
    for (int i=1; i<argc; i++) {
        std::string s(argv[i]);
        if ((s == "--local-boot") || (s == "-L")) {
            assets_dir = argv[i+1];
            std::cout << "Booting from \"" << assets_dir << "\"." << std::endl;
            i++;
            continue;
        }
        if ((s == "--debug") || (s == "-d")) {
            debug_mode = 1;
            printf("DEBUG MODE\n");
            continue;
        }
        if ((s == "--help") || (s == "-h")) {
            /* Store internally so we don't need to load
             * the global state before parsing the command.
            print_string_list(help_message); */
            return 0;
        }
        if ((s == "--version") || (s == "-v")) {
            /* For scripts that need the version string */
            std::cout << VERSION << std::endl;
            return 0;
        }
        if (s == "--test") {
            testing = 1;
            continue;
        }
        files.push_back(s);
    }
    return 1;
}

GLuint
gen_gl_texture(uint8_t* data, int w, int h, int wrap)
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

int
load_textures(std::vector<std::string> texture_list)
{
    for (std::string icon : texture_list) {
        int width, height;
        std::string fname = assets_dir + "icons/default/" + icon + ".png";
        uint8_t* data = stbi_load(fname.c_str(), &width, &height, NULL, 4);
        if (!data) {
            return 1;
        }

        GLuint texture_id = gen_gl_texture(data, width, height, GL_CLAMP_TO_EDGE);
        stbi_image_free(data);

        textures[icon] = texture_id;
    }

    {
        int width, height;
        std::string fname = assets_dir + "images/texture-spirals.png";
        uint8_t* data = stbi_load(fname.c_str(), &width, &height, NULL, 4);
        if (!data) {
            return 1;
        }

        GLuint texture_id = gen_gl_texture(data, width, height, GL_REPEAT);
        stbi_image_free(data);

        textures["texture-spirals"] = texture_id;
    }

    return 0;
}

void
set_style(void)
{
    std::string font_file = assets_dir + "fonts/SourceSans3-regular.ttf";
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;
    config.OversampleH = 2;
    config.OversampleV = 2;
    font = io.Fonts->AddFontFromFileTTF(font_file.c_str(), 16, &config);
    header_font = io.Fonts->AddFontFromFileTTF(font_file.c_str(), 24, &config);

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
}

void
load_menu(std::string menu_label)
{
    if (ImGui::BeginMenu(menu_label.c_str())) {
        for (auto i : menu_layout[menu_label]) {
            if (i[0] == "---") {
                ImGui::Separator();
                continue;
            }
            if (i[1] == "submenu") {
                load_menu(i[0]);
                continue;
            }
            if (ImGui::MenuItem(i[0].c_str())) {
                menu_action = i[1];
            }
        }
        ImGui::EndMenu();
    }
}

void
load_toolbar(std::vector<std::string> toolbar)
{
    int length = toolbar.size();
    for (int i=0; i<length; i++) {
        std::string icon = toolbar[i];
        if (icon == "---") {
            ImGui::Separator();
            continue;
        }
        ImVec2 size = {icon_size, icon_size};
    	if (ImGui::ImageButton((void*)(intptr_t)textures[icon], size)) {
            menu_action = icon;
        }
        if (i+1 < length) {
            ImGui::SameLine();
        }
    }
}

void
view_tab(int i)
{
    if (ImGui::BeginTabItem(views[i].filename.c_str())) {
        pattern_index = i;
        ImGui::Columns(3, "Undo History");
        ImGui::SetColumnWidth(-1, 100);
        ImGui::BeginChild("Undo History");
        ImGui::PushFont(header_font);
        ImGui::Text(translate("Undo History").c_str());
        ImGui::PopFont();
        for (std::string undo_item : views[i].undo_history) {
            ImGui::Text(undo_item.c_str());
        }
        ImGui::EndChild();
        ImGui::NextColumn();
        ImGui::SetColumnWidth(-1, 600);
        pattern_view();
        ImGui::NextColumn();
        ImGui::SetColumnWidth(-1, 200);
        property_editor();
        ImGui::Columns();
        ImGui::EndTabItem();
    }
}

void
main_widget(void)
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::Begin("Embroidermodder", NULL, ImGuiWindowFlags_MenuBar
        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoMove );
    ImGui::SetWindowFontScale(1.5);

    menu_action = "";

    if (ImGui::BeginMenuBar()) {
        load_menu("File");
        load_menu("Edit");
        load_menu("View");
        load_menu("Tools");
        load_menu("Draw");
        load_menu("Window");
        load_menu("Settings");
        load_menu("Help");
        ImGui::EndMenuBar();
    }

    load_toolbar(file_toolbar);
    ImGui::SameLine();
    load_toolbar(edit_toolbar);
    ImGui::SameLine();
    load_toolbar(draw_toolbar);

    load_toolbar(view_toolbar);

    if (menu_action != "") {
        actuator(menu_action);
    }

    if (views.size() > 0) {
        if (ImGui:: BeginTabBar("Tab Bar")) {
            for (int i=0; i<views.size(); i++) {
                view_tab(i);
            }
            if (show_editor) {
                if (ImGui::BeginTabItem("Text Editor")) {
                    editor.Render("Text Editor");
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }
    }
    else {
        ImVec2 size = {500, 300};
        ImGui::Image((void*)(intptr_t)textures["texture-spirals"], size);
    }

    if (show_about_dialog) {
        about_dialog();
    }

    if (show_settings_editor) {
        settings_editor();
    }

    ImGui::End();
}

void
load_text_file(std::string fname)
{
    std::string line;
    std::ifstream file;
    file.open(fname);
    while (std::getline(file, line)) {
        editor.InsertText(line + "\n");
    }
}

int
main(int argc, char* argv[])
{
    std::vector<std::string> files;
    if (!parse_command(argc, argv, files)) {
        return 0;
    }

    load_configuration();

    for (std::string file : files) {
        open_file_action(files);
    }

    int width = 1080;
    int height = 576;
    if (!glfwInit()) {
        std::cout << "ERROR: Failed to initialise GLFW." << std::endl;
        return 2;
    }
    GLFWwindow *window = glfwCreateWindow(width, height,
        "Embroidermodder " VERSION, NULL, NULL);
    if (!window) {
        std::cout << "ERROR: Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    glfwGetFramebufferSize(window, &width, &height);
    ImVec2 v = {width, height};
    ImGui::SetNextWindowPos({0.0, 0.0});
    ImGui::SetNextWindowSize(v);

    set_style();

    load_textures(texture_list);

    /* load_text_file("imgui_config.toml"); */

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        main_widget();

        ImGui::Render();

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.1, 0.1, 0.1, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
        if (!running) {
            break;
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

#if 0
double
random_uniform(void)
{
    int r = rand() % 100000;
    return 0.00001*r;
}

int
roundToMultiple(int roundUp, int numToRound, int multiple)
{
    if (multiple == 0) {
        return numToRound;
    }
    int remainder = numToRound % multiple;
    if (remainder == 0) {
        return numToRound;
    }

    if (numToRound < 0 && roundUp) {
        return numToRound - remainder;
    }
    if (roundUp) {
        return numToRound + multiple - remainder;
    }
    //else round down
    if (numToRound < 0 && !roundUp) {
        return numToRound - multiple - remainder;
    }
    return numToRound - remainder;
}

uint8_t
willUnderflowInt32(int64_t a, int64_t b)
{
    assert(LLONG_MAX>INT_MAX);
    int64_t c = (int64_t)a-b;
    return c < INT_MIN || c > INT_MAX;
}

uint8_t
willOverflowInt32(int64_t a, int64_t b)
{
    assert(LLONG_MAX>INT_MAX);
    int64_t c = (int64_t)a+b;
    return c < INT_MIN || c > INT_MAX;
}

/* Whenever the code happens across a todo call,
 * write it in a log file.
 */
void
todo(char *msg, int action)
{
    if (debug_mode) {
        std::ofstream f("todo.txt", std::ios_base::app);
        f << msg << " " << action << std::endl;
    }
}

/* Whenever the code happens across a todo call,
 * write it in a log file.
 */
void
error(char *msg, int action)
{
    if (debug_mode) {
        std::ofstream f("error.txt", std::ios_base::app);
        f << msg << " " << action << std::endl;
    }
}

/*
 * Guards against debug messages coming up during normal operation.
 *
 * Just change debug_mode to 1 to activate it. We could have a toggle
 * in the program to turn it on during operation for when something starts
 * acting weird.
 */
void
debug_message(std::string msg)
{
    if (debug_mode) {
        std::cout << msg << std::endl;
    }
}

/* .
 */
int
valid_file_format(char *fname)
{
    if (fname[0] == 0) {
        return 0;
    }
    if (emb_identify_format(fname) >= 0) {
        return 1;
    }
    return 0;
}

/*
 *  Testing.
 *  Mostly tests for actions not causing crashes that shut the program.
 *
 *  Testing actual correct application of the action would be harder.
 */

/* stores what the current error would be, should one occur */

/* Check that the translate call can return an entry from the table.
 */
int
test_translate(void)
{
    std::string translated = translate("?");
    printf("Failed to use translate.");
    return translated == "?";
}

/* .
 */
double
emb_clamp(double lower, double x, double upper)
{
    return std::max(std::min(upper, x), lower);
}

/* .
 */
int
valid_rgb(int red, int green, int blue)
{
    if ((red < 0) || (red > 255)) {
        return 0;
    }
    if ((green < 0) || (green > 255)) {
        return 0;
    }
    if ((blue < 0) || (blue > 255)) {
        return 0;
    }
    return 1;
}

bool Application_event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::FileOpen:
        if (_mainWin) {
            _mainWin->openFilesSelected(std::stringList(static_cast<QFileOpenEvent *>(event)->file()));
            return true;
        }
        // Fall through
    default:
        return QApplication::event(event);
    }
}

static void usage(void)
{
    fprintf(stderr,
    " ___ _____ ___  ___   __  _ ___  ___ ___   _____  __  ___  ___  ___ ___    ___ "           "\n"
    "| __|     | _ \\| _ \\ /  \\| |   \\| __| _ \\ |     |/  \\|   \\|   \\| __| _ \\  |__ \\" "\n"
    "| __| | | | _ <|   /| () | | |) | __|   / | | | | () | |) | |) | __|   /  / __/"           "\n"
    "|___|_|_|_|___/|_|\\_\\\\__/|_|___/|___|_|\\_\\|_|_|_|\\__/|___/|___/|___|_|\\_\\ |___|"   "\n"
    " _____________________________________________________________________________ "           "\n"
    "|                                                                             | "          "\n"
    "|                   http://embroidermodder.github.io                          | "          "\n"
    "|_____________________________________________________________________________| "          "\n"
    "                                                                               "           "\n"
    "Usage: embroidermodder [options] files ..."                                      "\n"
   //80CHARS======================================================================MAX
    "Options:"                                                                        "\n"
    "  -d, --debug      Print lots of debugging information."                         "\n"
    "  -h, --help       Print this message and exit."                                 "\n"
    "  -v, --version    Print the version number of embroidermodder and exit."        "\n"
    "\n"
           );
    exitApp = true;
}

static void version()
{
    fprintf(stdout, "%s %s\n", _appName_, _appVer_);
    exitApp = true;
}

void init_mdi_area(int i)
{
    mdi_area[i].useLogo = false;
    mdi_area[i].useTexture = false;
    mdi_area[i].useColor = false;
    mdi_area[i].bgLogo = "";
    mdi_area[i].bgTexture = "";
    mdi_area[i].bgColor = IM_COL32(40, 20, 10, 255);
}

void mdi_area_double_click(void)
{
    mainWin->openFile();
}

void MdiArea_paintEvent(QPaintEvent* /*e*/)
{
    QWidget* vport = viewport();
    QRect rect = vport->rect();

    QPainter painter(vport);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    //Always fill with a solid color first
    if (useColor) 
        painter.fillRect(rect, bgColor);
    else
        painter.fillRect(rect, background());

    //Then overlay the texture
    if (useTexture) {
        QBrush bgBrush(bgTexture);
        painter.fillRect(rect, bgBrush);
    }

    //Overlay the logo last
    if (useLogo) {
        //Center the pixmap
        int dx = (rect.width()-bgLogo.width())/2;
        int dy = (rect.height()-bgLogo.height())/2;
        painter.drawPixmap(dx, dy, bgLogo.width(), bgLogo.height(), bgLogo);
    }
}

void MdiArea_cascade()
{
    cascadeSubWindows();
    zoomExtentsAllSubWindows();
}

void MdiArea_tile()
{
    tileSubWindows();
    zoomExtentsAllSubWindows();
}

void MdiArea_zoomExtentsAllSubWindows()
{
    for (QMdiSubWindow* window : subWindowList()) {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(window);
        if (mdiWin) {
            View* v = mdiWin->gview;
            if (v) {
                v->recalculateLimits();
                v->zoomExtents();
            }
        }
    }
}

void init_MdiWindow(const int theIndex, MainWindow* mw, QMdiArea* parent, Qt::WindowFlags wflags)
{
    mainWin = mw;
    mdiArea = parent;

    myIndex = theIndex;

    fileWasLoaded = false;

    setAttribute(Qt::WA_DeleteOnClose);

    std::string aName;
    // TODO sprintf in Qt6
    //curFile = aName.sprintf("Untitled%d.dst", myIndex);
    curFile = "Untitled.dst";
    this->setWindowTitle(curFile);

    this->setWindowIcon(mainWin->load_icon("app"));

    gscene = new QGraphicsScene(0,0,0,0, this);
    gview = new View(mainWin, gscene, this);

    setWidget(gview);

    //WARNING: DO NOT SET THE QMDISUBWINDOW (this) FOCUSPROXY TO THE PROMPT
    //WARNING: AS IT WILL CAUSE THE WINDOW MENU TO NOT SWITCH WINDOWS PROPERLY!
    //WARNING: ALTHOUGH IT SEEMS THAT SETTING INTERNAL WIDGETS FOCUSPROXY IS OK.
    gview->setFocusProxy(mainWin->prompt);

    resize(sizeHint());

    promptHistory = "Welcome to Embroidermodder 2!<br/>Open some of our sample files. Many formats are supported.<br/>For help, press F1.";
    mainWin->prompt->setHistory(promptHistory);
    promptInputList << "";
    promptInputNum = 0;

    curLayer = "0";
    curColor = 0; //TODO: color ByLayer
    curLineType = "ByLayer";
    curLineWeight = "ByLayer";

    // Due to strange Qt4.2.3 feature the child window icon is not drawn
    // in the main menu if showMaximized() is called for a non-visible child window
    // Therefore calling show() first...
    show();
    showMaximized();

    setFocusPolicy(Qt::WheelFocus);
    setFocus();

    onWindowActivated();
}

MdiWindow::~MdiWindow()
{
    debug_message("MdiWindow Destructor()");
}

bool MdiWindow::saveFile(const std::string &fileName)
{
    SaveObject saveObj(gscene, this);
    return saveObj.save(fileName);
}

bool MdiWindow::loadFile(const std::string &fileName)
{
    debug_message("MdiWindow loadFile()");

    unsigned int tmpColor = getCurrentColor();

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, translate("Error reading file"),
                             translate("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    std::string ext = fileExtension(fileName);
    debug_message("ext: %s", qPrintable(ext));

    //Read
    EmbPattern* p = embPattern_create();
    if (!p) { printf("Could not allocate memory for embroidery pattern\n"); exit(1); }
    int readSuccessful = 0;
    std::string readError;
    int reader = emb_identify_format(qPrintable(fileName));
    if (reader < 0) {
        readSuccessful = 0;
        readError = "Unsupported read file type: " + fileName;
        debug_message("Unsupported read file type: " + fileName.toStdString());
    }
    else {
        readSuccessful = embPattern_read(p, qPrintable(fileName), reader);
        if (!readSuccessful) {
            readError = "Reading file was unsuccessful: " + fileName;
            debug_message("Reading file was unsuccessful: " + fileName.toStdString());
        }
    }
    if (!readSuccessful) {
        QMessageBox::warning(this, translate("Error reading pattern"), translate(qPrintable(readError)));
        QMessageBox::warning(this, translate("Error reading pattern"), translate("Cannot read pattern"));
    }
    else {
        embPattern_moveStitchListToPolylines(p); //TODO: Test more
        int stitchCount = p->stitchList->count;
        QPainterPath path;


        setCurrentFile(fileName);
        mainWin->statusbar->showMessage("File loaded.");
        std::string stitches;
        stitches.setNum(stitchCount);

        if (settings_grid_load_from_file) {
            //TODO: Josh, provide me a hoop size and/or grid spacing from the pattern.
        }
    }
    embPattern_free(p);

    //Clear the undo stack so it is not possible to undo past this point.
    gview->undoStack->clear();

    setCurrentColor(tmpColor);

    fileWasLoaded = true;
    mainWin->setUndoCleanIcon(fileWasLoaded);
    return fileWasLoaded;
}

void MdiWindow::print()
{
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        if (settings_printing_disable_bg) {
            //Save current bg
            QBrush brush = gview->backgroundBrush();
            //Save ink by not printing the bg at all
            gview->setBackgroundBrush(Qt::NoBrush);
            //Print, fitting the viewport contents into a full page
            gview->render(&painter);
            //Restore the bg
            gview->setBackgroundBrush(brush);
        }
        else {
            //Print, fitting the viewport contents into a full page
            gview->render(&painter);
        }
    }
}

//TODO: Save a Brother PEL image (An 8bpp, 130x113 pixel monochromatic? bitmap image) Why 8bpp when only 1bpp is needed?

//TODO: Should BMC be limited to ~32KB or is this a mix up with Bitmap Cache?
//TODO: Is there/should there be other embedded data in the bitmap besides the image itself?
//NOTE: Can save a Singer BMC image (An 8bpp, 130x113 pixel colored bitmap image)
void MdiWindow::saveBMC()
{
    //TODO: figure out how to center the image, right now it just plops it to the left side.
    QImage img(150, 150, QImage::Format_ARGB32_Premultiplied);
    img.fill(qRgb(255,255,255));
    EmbRect extents = gscene->itemsBoundingRect();

    QPainter painter(&img);
    EmbRect targetRect(0,0,150,150);
    // TODO: Make BMC background into it's own setting?
    if (settings_printing_disable_bg) {
        QBrush brush = gscene->backgroundBrush();
        gscene->setBackgroundBrush(Qt::NoBrush);
        gscene->update();
        gscene->render(&painter, targetRect, extents, Qt::KeepAspectRatio);
        gscene->setBackgroundBrush(brush);
    }
    else {
        gscene->update();
        gscene->render(&painter, targetRect, extents, Qt::KeepAspectRatio);
    }

    img.convertToFormat(QImage::Format_Indexed8, Qt::ThresholdDither|Qt::AvoidDither).save("test.bmc", "BMP");
}

void MdiWindow::setCurrentFile(const std::string &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowModified(false);
    setWindowTitle(getShortCurrentFile());
}

std::string MdiWindow::getShortCurrentFile()
{
    return QFileInfo(curFile).fileName();
}

std::string MdiWindow::fileExtension(const std::string& fileName)
{
    return QFileInfo(fileName).suffix().toLower();
}

void MdiWindow::closeEvent(QCloseEvent* /*e*/)
{
    debug_message("MdiWindow closeEvent()");
    emit sendCloseMdiWin(this);
}

void MdiWindow::onWindowActivated()
{
    debug_message("MdiWindow onWindowActivated()");
    gview->undoStack->setActive(true);
    mainWin->setUndoCleanIcon(fileWasLoaded);
    mainWin->statusbar->statusBarSnapButton->setChecked(gscene->property("ENABLE_SNAP").toBool());
    mainWin->statusbar->statusBarGridButton->setChecked(gscene->property("ENABLE_GRID").toBool());
    mainWin->statusbar->statusBarRulerButton->setChecked(gscene->property("ENABLE_RULER").toBool());
    mainWin->statusbar->statusBarOrthoButton->setChecked(gscene->property("ENABLE_ORTHO").toBool());
    mainWin->statusbar->statusBarPolarButton->setChecked(gscene->property("ENABLE_POLAR").toBool());
    mainWin->statusbar->statusBarQSnapButton->setChecked(gscene->property("ENABLE_QSNAP").toBool());
    mainWin->statusbar->statusBarQTrackButton->setChecked(gscene->property("ENABLE_QTRACK").toBool());
    mainWin->statusbar->statusBarLwtButton->setChecked(gscene->property("ENABLE_LWT").toBool());
    mainWin->prompt->setHistory(promptHistory);
}

QSize MdiWindow::sizeHint() const
{
    debug_message("MdiWindow sizeHint()");
    return QSize(450, 300);
}

void MdiWindow::currentLayerChanged(const std::string& layer)
{
    curLayer = layer;
}

void MdiWindow::currentColorChanged(const unsigned int& color)
{
    curColor = color;
}

void MdiWindow::currentLinetypeChanged(const std::string& type)
{
    curLineType = type;
}

void MdiWindow::currentLineweightChanged(const std::string& weight)
{
    curLineWeight = weight;
}

void MdiWindow::updateColorLinetypeLineweight()
{
}

void MdiWindow::deletePressed()
{
    gview->deletePressed();
}

void MdiWindow::escapePressed()
{
    gview->escapePressed();
}

void MdiWindow::showViewScrollBars(bool val)
{
    gview->showScrollBars(val);
}

void MdiWindow::setViewCrossHairColor(unsigned int color)
{
    gview->setCrossHairColor(color);
}

void MdiWindow::setViewBackgroundColor(unsigned int color)
{
    gview->setBackgroundColor(color);
}

void MdiWindow::setViewSelectBoxColors(unsigned int colorL, unsigned int fillL, unsigned int colorR, unsigned int fillR, int alpha)
{
    gview->setSelectBoxColors(colorL, fillL, colorR, fillR, alpha);
}

void MdiWindow::setViewGridColor(unsigned int color)
{
    gview->setGridColor(color);
}

void MdiWindow::setViewRulerColor(unsigned int color)
{
    gview->setRulerColor(color);
}

void MdiWindow::promptHistoryAppended(const std::string& txt)
{
    promptHistory.append("<br/>" + txt);
}

void MdiWindow::logPromptInput(const std::string& txt)
{
    promptInputList << txt;
    promptInputNum = promptInputList.size();
}

void MdiWindow::promptInputPrevious()
{
    promptInputPrevNext(true);
}

void MdiWindow::promptInputNext()
{
    promptInputPrevNext(false);
}

void MdiWindow::promptInputPrevNext(bool prev)
{
    if (promptInputList.isEmpty()) {
        if (prev) {
            critical_messagebox(translate("Prompt Previous Error"), translate("The prompt input is empty! Please report this as a bug!"));
        }
        else {
            critical_messagebox(translate("Prompt Next Error"), translate("The prompt input is empty! Please report this as a bug!"));
            debug_message("The prompt input is empty! Please report this as a bug!");
        }
    }
    else {
        if (prev) {
            promptInputNum--;
        }
        else {
            promptInputNum++;
        }
        int maxNum = promptInputList.size();
        if (promptInputNum < 0) {
            promptInputNum = 0;
            mainWin->prompt->setCurrentText("");
        }
        else if (promptInputNum >= maxNum) {
            promptInputNum = maxNum;
            mainWin->prompt->setCurrentText("");
        }
        else {
            mainWin->prompt->setCurrentText(promptInputList.at(promptInputNum));
        }
    }
}

void PreviewDialog(QWidget* parent,
                   std::string& caption,
                   std::string& dir,
                   std::string& filter) : QFileDialog(parent, caption, dir, filter)
{
    debug_message("PreviewDialog Constructor");

    //TODO: get actual thumbnail image from file, lets also use a size of 128x128 for now...
    //TODO: make thumbnail size adjustable thru settings dialog
    imgWidget = new ImageWidget("icons/default/nopreview.png", this);

    QLayout* lay = layout();
    if (qobject_cast<QGridLayout*>(lay))
    {
        QGridLayout* grid = qobject_cast<QGridLayout*>(lay);
        grid->addWidget(imgWidget, 0, grid->columnCount(), grid->rowCount(), 1);
    }

    setModal(true);
    setOption(QFileDialog::DontUseNativeDialog);
    setViewMode(QFileDialog::Detail);
    setFileMode(QFileDialog::ExistingFiles);

    //TODO: connect the currentChanged signal to update the preview imgWidget.
}

#endif
