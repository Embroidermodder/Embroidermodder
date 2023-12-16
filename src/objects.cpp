/*
 * Embroidermodder 2.
 *
 * ------------------------------------------------------------
 *
 * Copyright 2013-2022 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software.
 * See LICENSE for licensing terms.
 *
 * ------------------------------------------------------------
 *
 * Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 *
 * For additional tips on our style see the linux Kernel style:
 *     https://www.kernel.org/doc/Documentation/process/coding-style.rst
 *
 */

#include "embroidermodder.h"

void addPath(View *view, Geometry *obj);
void saveObject(int objType, View *view, Geometry *obj);
void saveObjectAsStitches(int objType, View *view, Geometry *obj);

void toPolyline(
    View* view,
    QPointF objPos,
    QPainterPath objPath,
    QString layer,
    QColor color,
    QString lineType,
    QString lineWeight);


bool save(View *view, QString f);


const CommandData subcommand_table[MAX_COMMANDS] = {
    {
    },
    {
        .id = -1,
        .command="END"
    }
};

/* add_geometry_action args
 */
const char *
add_geometry(char **argv, int argc)
{
    View *gview = activeView();
	QGraphicsScene* gscene = gview->scene();
	QUndoStack* stack = gview->undoStack;
    int subcommand = -1;

	if (!gview) {
		return "ERROR: no active view found.";
	}
	if (!gscene) {
		return "ERROR: no graphics scene view found.";
	}
    if (!stack) {
        return "ERROR: no undo stack for scene";
    }

    for (int i=0; !string_equal(geometry_subcommands[i], "END"); i++) {
        /* FIXME */
        if (string_equal(geometry_subcommands[i], argv[0])) {
            subcommand = -1;
        }
    }

    switch (subcommand) {
    
    /* add_arc_action.
     *
     * EmbReal startX, EmbReal startY, EmbReal midX, EmbReal midY, EmbReal endX, EmbReal endY, std::string rubberMode
     */
    case SUBCOMMAND_ARC: {
        if (argc < 7) {
            return "SUBCOMMAND_ARC requires 6 arguments";
        }
		Geometry* arcObj = new Geometry(OBJ_TYPE_ARC);
		arcObj->gdata.arc.start.x = atof(argv[1]);
		arcObj->gdata.arc.start.x = -atof(argv[2]);
		arcObj->gdata.arc.mid.x = atof(argv[3]);
		arcObj->gdata.arc.mid.x = -atof(argv[4]);
		arcObj->gdata.arc.end.x = atof(argv[5]);
		arcObj->gdata.arc.end.x = -atof(argv[6]);
        /*
		arcObj->setObjectRubberMode(rubberMode);
		if (rubberMode != "OBJ_RUBBER_OFF") {
			gview->addToRubberRoom(arcObj);
		}
		scene->addItem(arcObj);
		*/
		return "";
    }

    /* add_circle_action.
     *
     * EmbReal centerX, EmbReal centerY, EmbReal radius, bool fill, std::string rubberMode
     */
    case SUBCOMMAND_CIRCLE: {
		Geometry* obj = new Geometry(OBJ_TYPE_CIRCLE);
		obj->gdata.circle.center.x = atof(argv[1]);
		obj->gdata.circle.center.y = -atof(argv[2]);
		obj->gdata.circle.radius = atof(argv[3]);
		bool fill = false;
		obj->objRubberMode = "OBJ_RUBBER_OFF";

		/*
		//TODO: circle fill
		if (rubberMode != "OBJ_RUBBER_OFF") {
			gview->addToRubberRoom(obj);
			gscene->addItem(obj);
			gscene->update();
		}
		else {
			UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
			stack->push(cmd);
		}
		*/
        return "";
    }

    /* EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot, std::string rubberMode
     */
    case SUBCOMMAND_DIM_LEADER: {
		Geometry* obj = new Geometry(OBJ_TYPE_DIMLEADER);
        /*
        AddDimLeader(std::vector<Node> a)
        _mainWin->nativeAddDimLeader(a[0].r, a[1].r, a[2].r, a[3].r, a[4].r, OBJ_RUBBER_OFF);

		Geometry* obj = new Geometry(x1, -y1, x2, -y2,_mainWin->getCurrentColor());
		obj->setRotation(-rot);
		obj->setObjectRubberMode(rubberMode);
		if (rubberMode != "OBJ_RUBBER_OFF") {
			gview->addToRubberRoom(obj);
			gscene->addItem(obj);
			gscene->update();
		}
		else {
			UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
			stack->push(cmd);
		}
		*/
        return "";
    }

    /* Add an ellipse to the scene.
     *
     * EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, EmbReal rot, bool fill, std::string rubberMode
     */
    case SUBCOMMAND_ELLIPSE: {
		Geometry* obj = new Geometry(OBJ_TYPE_ELLIPSE);
		/*
		Geometry* obj = new Geometry(centerX, -centerY, width, height,_mainWin->getCurrentColor());
		obj->setRotation(-rot);
		obj->setObjectRubberMode(rubberMode);
		//TODO: ellipse fill
		if (rubberMode != "OBJ_RUBBER_OFF") {
			gview->addToRubberRoom(obj);
			gscene->addItem(obj);
			gscene->update();
		}
		else {
			UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
			stack->push(cmd);
		}
		*/

        return "";
    }

    /*
     * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal legHeight
     */
    case SUBCOMMAND_HORIZONTAL_DIMENSION: {
        /*
		Geometry* obj = new Geometry(OBJ_TYPE_HORIZONTAL_DIMENSION);
        AddHorizontalDimension(std::vector<Node> a)
        //TODO: Node error checking
        debug_message("TODO: finish addHorizontalDimension command");
        */
        return "";
    }

    /*
     * QString img, EmbReal x, EmbReal y, EmbReal w, EmbReal h, EmbReal rot
     */
    case SUBCOMMAND_IMAGE: {
        /*
        AddImage(std::vector<Node> a)
        //TODO: Node error checking
        debug_message("TODO: finish addImage command");
        */
        return "";
    }

    /* .
     * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot
     */
    case SUBCOMMAND_INFINITE_LINE: {
        /*
        //TODO: Node error checking
        debug_message("TODO: finish addInfiniteLine command");
        */
        return "";
    }

    /*
     * .
     * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot, std::string rubberMode
     */
    case SUBCOMMAND_LINE: {
        /*
        _mainWin->nativeadd_line_action(a[0].r, a[1].r, a[2].r, a[3].r, a[4].r, OBJ_RUBBER_OFF);

		EmbLine line;
		line.start.x = x1;
		line.start.y = -y1;
		line.end.x = x2;
		line.end.y = -y2;
		LineObject* obj = new LineObject(line,_mainWin->getCurrentColor());
		obj->setRotation(-rot);
		obj->setObjectRubberMode(rubberMode);
		if (rubberMode != "OBJ_RUBBER_OFF") {
			gview->addToRubberRoom(obj);
			gscene->addItem(obj);
			gscene->update();
		}
		else {
			UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
			stack->push(cmd);
		}
        */
        return "";
    }

    /*
     * NOTE: This native is different than the rest in that
     * the Y+ is down (scripters need not worry about this).
     *
     * EmbReal startX, EmbReal startY, const QPainterPath& p, std::string rubberMode
     */
    case SUBCOMMAND_PATH: {
        /*
        AddPath(std::vector<Node> a)
        // TODO: Node error checking
        debug_message("TODO: finish addPath command");
        */
        return "";
    }

    /* add_point_action
     * args
     */
    case SUBCOMMAND_POINT: {
		/*
		EmbVector v;
		v.x = x;
		v.y = -y;

		Geometry* obj = new Geometry(v, _mainWin->getCurrentColor(), Qt::SolidLine);
		UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
		stack->push(cmd);
		*/
        return "";
    }

    /* add_polygon_action
     * args
     *
     * NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
     * EmbReal startX, EmbReal startY, const QPainterPath& p, std::string rubberMode
     */
    case SUBCOMMAND_POLYGON: {
        /*
		PolygonObject* obj = new PolygonObject(startX, startY, p,_mainWin->getCurrentColor());
		obj->setObjectRubberMode(rubberMode);
		if (rubberMode != "OBJ_RUBBER_OFF") {
			gview->addToRubberRoom(obj);
			gscene->addItem(obj);
			gscene->update();
		}
		else {
			UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
			stack->push(cmd);
		}

        QVariantList varList = a[0].toVariant().toList();
        int varSize = varList.size();
        if (varSize < 2) {
            return "TYPE ERROR: add_polygon_action(): array must contain at least two elements";
        }
        if (varSize % 2) {
            return "TYPE ERROR: add_polygon_action(): array cannot contain an odd number of elements";
        }

        bool lineTo = false;
        bool xCoord = true;
        EmbReal x = 0;
        EmbReal y = 0;
        EmbReal startX = 0;
        EmbReal startY = 0;
        QPainterPath path;
        foreach(QVariant var, varList) {
            if (var.canConvert(QVariant::Double)) {
                if (xCoord) {
                    xCoord = false;
                    x = var.toReal();
                }
                else {
                    xCoord = true;
                    y = -var.toReal();

                    if (lineTo) { path.lineTo(x,y); }
                    else { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
                }
            }
            else {
                return "TYPE ERROR: add_polygon_action(): array contains one or more invalid elements");
            }
        }

        //Close the polygon
        path.closeSubpath();

        path.translate(-startX, -startY);

        _mainWin->nativeadd_polygon_action(startX, startY, path, OBJ_RUBBER_OFF);
        */
        return "";
    }

    case SUBCOMMAND_POLYLINE: {
        return "";
    }

    /*
     * .
     * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot
     */
    case SUBCOMMAND_RAY: {
        /*
        //TODO: Node error checking
        debug_message("TODO: finish addRay command");
        */
        return "";
    }

    /* . */
    case SUBCOMMAND_RECTANGLE: {
		EmbRect rect;
		rect.left = atof(argv[0]);
		rect.right = -atof(argv[1]);
		rect.top = atof(argv[2]);
		rect.bottom = -atof(argv[3]);
		EmbReal rot = atof(argv[4]);
		bool fill = (argv[5] == "1");
		std::string rubberMode = argv[6];

		/*
		Geometry* obj = new Geometry(rect, _mainWin->getCurrentColor(), Qt::SolidLine);
		obj->setRotation(-rot);
		obj->objRubberMode = rubberMode;
		//TODO: rect fill
		if (rubberMode != "OBJ_RUBBER_OFF") {
			gview->addToRubberRoom(obj);
			gscene->addItem(obj);
			gscene->update();
		}
		else {
			UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
			stack->push(cmd);
		}
		*/
        return "";
    }

    /* AddRegularPolygon
     *
     * EmbReal centerX, EmbReal centerY, quint16 sides, uint8_t mode,
     * EmbReal rad, EmbReal rot, bool fill
     */
    case SUBCOMMAND_REGULAR_POLYGON: {
        return "";
    }

    /* add_rounded_rectangle_action.
     *
     * EmbReal x, EmbReal y, EmbReal w, EmbReal h, EmbReal rad, EmbReal rot, bool fill
     */
    case SUBCOMMAND_ROUNDED_RECTANGLE: {
        /*
        _mainWin->nativeadd_rounded_rectangle_action(
            a[0].r, a[1].r, a[2].r, a[3].r, a[4].r, a[5].r, a[6].b);
        */
        return "";
    }


    /* add_slot_action
     * args
     *
     * EmbReal centerX, EmbReal centerY, EmbReal diameter, EmbReal length, EmbReal rot, bool fill, std::string rubberMode
     */
    case SUBCOMMAND_SLOT: {
        //TODO: Use UndoableAddCommand for slots
        /*
        SlotObject* slotObj = new SlotObject(centerX, -centerY, diameter, length,_mainWin->getCurrentColor());
        slotObj->setRotation(-rot);
        slotObj->setObjectRubberMode(rubberMode);
        if (rubberMode) gview->addToRubberRoom(slotObj);
        scene->addItem(slotObj);
        //TODO: slot fill
        */
        //_mainWin->nativeAddSlot(a[0].r, a[1].r, a[2].r, a[3].r, a[4].r, a[5].b, OBJ_RUBBER_OFF);

        return "";
    }

    /* add_text_multi_action
     *
     * QString str, EmbReal x, EmbReal y, EmbReal rot, bool fill, std::string rubberMode
     */
    case SUBCOMMAND_TEXT_MULTI: {
        /*
        _mainWin->nativeadd_text_multi_action(a[0].s, a[1].r, a[2].r, a[3].r, a[4].b, OBJ_RUBBER_OFF);
        */
        return "";
    }

    /* add_text_single_action
     *
     * QString str, EmbReal x, EmbReal y, EmbReal rot, bool fill, std::string rubberMode
     */
    case SUBCOMMAND_TEXT_SINGLE: {
        /*
        _mainWin->nativeadd_text_single_action(a[0].s, a[1].r, a[2].r, a[3].r, a[4].b, OBJ_RUBBER_OFF);
        */
		/*
		TextSingleObject* obj = new TextSingleObject(str, x, -y, _mainWin->getCurrentColor());
		obj->setObjectTextFont(QString::fromStdString(settings.text_font));
		obj->setObjectTextSize(settings.text_size);
		obj->setObjectTextStyle(settings.text_style_bold,
								settings.text_style_italic,
								settings.text_style_underline,
								settings.text_style_strikeout,
								settings.text_style_overline);
		obj->setObjectTextBackward(false);
		obj->setObjectTextUpsideDown(false);
		obj->setRotation(-rot);
		//TODO: single line text fill
		obj->setObjectRubberMode(rubberMode);
		if (rubberMode != "OBJ_RUBBER_OFF") {
			gview->addToRubberRoom(obj);
			gscene->addItem(obj);
			gscene->update();
		}
		else {
			UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
			stack->push(cmd);
		}
		*/
        return "";
    }

    /* . */
    case SUBCOMMAND_TRIANGLE: {
        /*
        _mainWin->nativeadd_triangle_action(a[0].r, a[1].r, a[2].r, a[3].r, a[4].r, a[5].r, a[6].r, a[7].b);
        */
        return "";
    }

    /* TODO: Node error checking
     *
     * EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal legHeight
     */
    case SUBCOMMAND_VERTICAL_DIMENSION: {
        /*
        debug_message("TODO: finish addVerticalDimension command");
        */
        return "";
    }

    default: {
    	return "The add subcommand is not recognised.";
    }
    }

	gscene->update();
	return "";
}

/* . */
bool
save_current_file(const char *fileName)
{
    View* view = activeView();
    QGraphicsScene* gscene = activeScene();
    view->formatType = EMBFORMAT_UNSUPPORTED;
    return save(view, QString(fileName));
}

/* Find closest point to "position" from the list of points given. */
QPointF
closest_point(QPointF position, std::vector<QPointF> points)
{
    QPointF result = points[0];
    EmbReal closest = QLineF(position, points[0]).length();
    for (int i=1; i<(int)(points.size()); i++) {
        if (QLineF(position, points[i]).length() < closest) {
            closest = QLineF(position, points[i]).length();
            result = points[i];
        }
    }
    return result;
}

/* Add_polyline. */
void
add_polyline(QPainterPath p, std::string rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->undoStack;
    if (!(gview && gscene && stack)) {
        return;
    }
    Geometry* obj = new Geometry(OBJ_TYPE_POLYLINE);
    obj->normalPath = p;
    obj->updatePath();
    obj->objRubberMode = rubberMode;
    if (rubberMode != "OBJ_RUBBER_OFF") {
        gview->addToRubberRoom(obj);
        gscene->addItem(obj);
        gscene->update();
    }
    else {
        UndoableCommand* cmd = new UndoableCommand("add", obj->data(OBJ_NAME).toString(), obj, gview);
        stack->push(cmd);
    }
}

/* Construct a new Geometry object of arc type.
 * Initialize common object properties.
 *
 * WARNING
 * -------
 * DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
 * and the item is double clicked, the scene will erratically move the item while zooming.
 * All movement has to be handled explicitly by us, not by the scene.
 *
 * TODO: getCurrentLineType
 */
void
Geometry::init(int type_, QRgb rgb, Qt::PenStyle lineType, QGraphicsItem* parent)
{
    debug_message("Geometry Constructor()");
    Type = type_;
    setData(OBJ_TYPE, Type);

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    objPen.setCapStyle(Qt::RoundCap);
    objPen.setJoinStyle(Qt::RoundJoin);
    lwtPen.setCapStyle(Qt::RoundCap);
    lwtPen.setJoinStyle(Qt::RoundJoin);

    objID = QDateTime::currentMSecsSinceEpoch();

    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight

    switch (Type) {
    case OBJ_TYPE_ARC:
        setData(OBJ_NAME, "Arc");
        break;
    case OBJ_TYPE_CIRCLE:
        setData(OBJ_NAME, "Circle");
        break;
    case OBJ_TYPE_ELLIPSE:
        setData(OBJ_NAME, "Ellipse");
        break;
    case OBJ_TYPE_DIMLEADER:
        setData(OBJ_NAME, "Dimension Leader");
        break;
    case OBJ_TYPE_LINE:
        setData(OBJ_NAME, "Line");
        break;
    case OBJ_TYPE_POINT:
        setData(OBJ_NAME, "Point");
        break;
    case OBJ_TYPE_POLYGON:
        setData(OBJ_NAME, "Polygon");
        break;
    case OBJ_TYPE_POLYLINE:
        setData(OBJ_NAME, "Polyline");
        break;
    case OBJ_TYPE_RECTANGLE:
        setData(OBJ_NAME, "Rectangle");
        break;
    case OBJ_TYPE_TEXTSINGLE:
        setData(OBJ_NAME, "Single Line Text");
        break;
    case OBJ_TYPE_TEXTMULTI:
        setData(OBJ_NAME, "Multi Line Text");
        break;
    default:
        setData(OBJ_NAME, "Unknown");
        break;
    }

/*
    init_line(line); //TODO: getCurrentLineType
    init_point(vector); //TODO: getCurrentLineType
    init_path(p); //TODO: getCurrentLineType
    init_rect(rect); //TODO: getCurrentLineType
    init_text_single(str, v); //TODO: getCurrentLineType
*/

    setPen(objPen);

    update();
}

Geometry::Geometry(int type_, QGraphicsItem* parent) : QGraphicsPathItem(parent)
{
    init(type_, _mainWin->getCurrentColor(), Qt::SolidLine, parent);
}

/* TODO:
 */
void
Geometry::update(void)
{
    switch (Type) {
    case OBJ_TYPE_ARC:
        calculateArcData(gdata.arc);
        break;
    case OBJ_TYPE_CIRCLE:
        setObjectRadius(gdata.circle.radius);
        setObjectCenter(gdata.circle.center);
        break;
    case OBJ_TYPE_ELLIPSE:
        setObjectSize(gdata.ellipse.radius.x, gdata.ellipse.radius.y);
        setObjectCenter(gdata.ellipse.center);
        break;
    case OBJ_TYPE_DIMLEADER:
        setData(OBJ_NAME, "Dimension Leader");
        break;
    case OBJ_TYPE_LINE:
        setData(OBJ_NAME, "Line");
        break;
    case OBJ_TYPE_POINT:
        setData(OBJ_NAME, "Point");
        break;
    case OBJ_TYPE_POLYGON:
        setData(OBJ_NAME, "Polygon");
        break;
    case OBJ_TYPE_POLYLINE:
        setData(OBJ_NAME, "Polyline");
        break;
    case OBJ_TYPE_RECTANGLE:
        setData(OBJ_NAME, "Rectangle");
        break;
    case OBJ_TYPE_TEXTSINGLE:
        setData(OBJ_NAME, "Single Line Text");
        break;
    case OBJ_TYPE_TEXTMULTI:
        setData(OBJ_NAME, "Multi Line Text");
        break;
    default:
        setData(OBJ_NAME, "Unknown");
        break;
    }
    updatePath();
}

/* Geometry::init_line
 */
void
Geometry::init_line(EmbLine line)
{
    if (Type==OBJ_TYPE_DIMLEADER) {
        flags |= ((!PROP_CURVED) & PROP_FILLED);
    }

    setObjectEndPoint1(line.start);
    setObjectEndPoint2(line.end);
}

/* Geometry::init_point
 */
void
Geometry::init_point(EmbVector position)
{
    setRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setPos(position.x, position.y);
}


/* Geometry::init_path
 */
void
Geometry::init_path(QPainterPath p)
{
    gripIndex = -1;
    normalPath = p;
    updatePath();
    if (Type==OBJ_TYPE_POLYLINE) {
        updatePath(p);
    }

    QPainterPath::Element position = p.elementAt(0);
    setPos(position.x, position.y);
}

/* Geometry::init_rect
 */
void Geometry::init_rect(EmbRect rect)
{
    setObjectRect(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top);
}

/* Init_text_single.
 *
 * TODO: set the justification properly.
 * TODO: pass in proper lineweight
 */
void
Geometry::init_text_single(QString str, EmbVector v)
{
    objTextJustify = "Left";

    setObjectText(str);
    setPos(v.x, v.y);
}

/* Geometry::Geometry *obj *parent. */
Geometry::Geometry(Geometry* obj, QGraphicsItem* parent) : QGraphicsPathItem(parent)
{
    debug_message("Geometry Constructor()");
    if (!obj) {
        debug_message("ERROR: null obj pointer passed to Geometry contructor.");
        return;
    }
    init(obj->Type, obj->objPen.color().rgb(), Qt::SolidLine); //TODO: getCurrentLineType
    flags = obj->flags;
    Type = obj->Type;
    setRotation(obj->rotation());
    setScale(obj->scale());
    memcpy(&gdata, &(obj->gdata), sizeof(GeometryData));
    update();
}

/* Geometry::allGripPoints */
std::vector<QPointF>
Geometry::allGripPoints()
{
    std::vector<QPointF> gripPoints;
    switch (Type) {
    case OBJ_TYPE_ARC:
        gripPoints = {
            scenePos(),
            objectStartPoint(),
            objectMidPoint(),
            objectEndPoint()
        };
        break;
    case OBJ_TYPE_CIRCLE:
    case OBJ_TYPE_ELLIPSE:
        gripPoints = {
            scenePos(),
            objectQuadrant0(),
            objectQuadrant90(),
            objectQuadrant180(),
            objectQuadrant270()
        };
        break;
    case OBJ_TYPE_DIMLEADER:
        gripPoints = {
            objectEndPoint1(),
            objectEndPoint2()
        };
        if (flags & PROP_CURVED) {
            gripPoints.push_back(objectMidPoint());
        }
        break;
    case OBJ_TYPE_IMAGE:
    case OBJ_TYPE_RECTANGLE:
        gripPoints = {
            objectTopLeft(),     //Top Left Corner QSnap
            objectTopRight(),    //Top Right Corner QSnap
            objectBottomLeft(),  //Bottom Left Corner QSnap
            objectBottomRight()  //Bottom Right Corner QSnap
        };
        break;
    case OBJ_TYPE_LINE:
        gripPoints ={
            objectEndPoint1(),
            objectEndPoint2(),
            objectMidPoint()
        };
        break;
    case OBJ_TYPE_PATH:
    case OBJ_TYPE_POLYGON:
    case OBJ_TYPE_POLYLINE: {
            QPainterPath::Element element;
            for (int i = 0; i < normalPath.elementCount(); ++i) {
                element = normalPath.elementAt(i);
                gripPoints.push_back(mapToScene(element.x, element.y));
            }
        }
        break;
    default:
    case OBJ_TYPE_TEXTSINGLE:
    case OBJ_TYPE_POINT:
        gripPoints = {
            scenePos()
        };
        break;
    }

    return gripPoints;
}

/* Geometry::~Geometry
 */
Geometry::~Geometry()
{
    debug_message("Geometry Destructor()");
}

/* Set object line weight. */
void
Geometry::setObjectLineWeight(std::string lineWeight)
{
    objPen.setWidthF(0); //NOTE: The objPen will always be cosmetic

    if (lineWeight == "OBJ_LWT_BYLAYER") {
        lwtPen.setWidthF(0.35f); //TODO: getLayerLineWeight
    }
    else if (lineWeight == "OBJ_LWT_BYBLOCK") {
        lwtPen.setWidthF(0.35f); //TODO: getBlockLineWeight
    }
    else {
        lwtPen.setWidthF(std::stof(lineWeight));
    }
    /*
    else {
        QMessageBox::warning(0, translate_str("Error - Negative Lineweight"),
                                translate_str("Lineweight: %1")
                                .arg(QString().setNum(lineWeight)));
        debug_message("Lineweight cannot be negative! Inverting sign.");
        lwtPen.setWidthF(-lineWeight);
    }
    */
}

/* Geometry::objectRubberPoint
 * key */
QPointF
Geometry::objectRubberPoint(QString  key)
{
    if (objRubberPoints.contains(key)) {
        return objRubberPoints.value(key);
    }

    QGraphicsScene* gscene = scene();
    if (gscene) {
        return scene()->property("SCENE_QSNAP_POINT").toPointF();
    }
    return QPointF();
}

/* Geometry::objectRubberText
 * key
 */
QString
Geometry::objectRubberText(QString  key)
{
    if (objRubberTexts.contains(key)) {
        return objRubberTexts.value(key);
    }
    return QString();
}

/* If gripped, force this object to be drawn even if it is offscreen. */
QRectF
Geometry::boundingRect()
{
    if (objRubberMode == "OBJ_RUBBER_GRIP") {
        return scene()->sceneRect();
    }
    return path().boundingRect();
}

/* Geometry::drawRubberLine
 * rubLine, painter, colorFromScene
 */
void
Geometry::drawRubberLine(const QLineF& rubLine, QPainter* painter, const char* colorFromScene)
{
    if (!painter) {
        return;
    }
    QGraphicsScene* objScene = scene();
    if (!objScene) {
        return;
    }
    QPen colorPen = objPen;
    colorPen.setColor(QColor(objScene->property(colorFromScene).toUInt()));
    painter->setPen(colorPen);
    painter->drawLine(rubLine);
    painter->setPen(objPen);
}

/* Geometry::realRender
 * painter, renderPath
 */
void
Geometry::realRender(QPainter* painter, const QPainterPath& renderPath)
{
    QColor color1 = objPen.color();       //lighter color
    QColor color2  = color1.darker(150); //darker color

    //If we have a dark color, lighten it
    int darkness = color1.lightness();
    int threshold = 32; //TODO: This number may need adjusted or maybe just add it to settings.
    if (darkness < threshold) {
        color2 = color1;
        if (!darkness) {
            color1 = QColor(threshold, threshold, threshold);
            // lighter() does not affect pure black
        }
        else {
            color1 = color2.lighter(100 + threshold);
        }
    }

    int count = renderPath.elementCount();
    for (int i = 0; i < count-1; ++i) {
        QPainterPath::Element elem = renderPath.elementAt(i);
        QPainterPath::Element next = renderPath.elementAt(i+1);

        if (next.isMoveTo()) continue;

        QPainterPath elemPath;
        elemPath.moveTo(elem.x, elem.y);
        elemPath.lineTo(next.x, next.y);

        QPen renderPen(QColor(0,0,0,0));
        renderPen.setWidthF(0);
        painter->setPen(renderPen);
        QPainterPathStroker stroker;
        stroker.setWidth(0.35);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);
        QPainterPath realPath = stroker.createStroke(elemPath);
        painter->drawPath(realPath);

        QLinearGradient grad(elemPath.pointAtPercent(0.5), elemPath.pointAtPercent(0.0));
        grad.setColorAt(0, color1);
        grad.setColorAt(1, color2);
        grad.setSpread(QGradient::ReflectSpread);

        painter->fillPath(realPath, QBrush(grad));
    }
}

/* Return the closest snap point to the mouse point. */
QPointF
Geometry::mouseSnapPoint(const QPointF& mousePoint)
{
    return closest_point(mousePoint, allGripPoints());
}

/* . */
void
Geometry::setObjectEndPoint1(EmbVector endPt1)
{
    EmbVector endPt2 = to_EmbVector(objectEndPoint2());
    EmbVector delta = embVector_subtract(endPt2, endPt1);
    // setScale(1); ?
    setRotation(0);
    setLine(0, 0, delta.x, delta.y);
    setPos(endPt1.x, endPt1.y);
    if (Type == OBJ_TYPE_DIMLEADER) {
        updateLeader();
    }
}

/* . */
void
Geometry::setObjectEndPoint2(EmbVector endPt2)
{
    EmbVector endPt1 = to_EmbVector(scenePos());
    EmbVector delta = embVector_subtract(endPt2, endPt1);
    setRotation(0);
    // setScale(1); ?
    setLine(0, 0, delta.x, delta.y);
    setPos(endPt1.x, endPt1.y);
    if (Type == OBJ_TYPE_DIMLEADER) {
        updateLeader();
    }
}

/* Geometry::objectEndPoint1 */
QPointF
Geometry::objectEndPoint1()
{
    return scenePos();
}

/* Geometry::objectEndPoint2 */
QPointF
Geometry::objectEndPoint2()
{
    EmbVector v;
    EmbReal rot = radians(rotation());
    v.x = objLine.x2()*scale();
    v.y = objLine.y2()*scale();
    QPointF rotEnd = to_QPointF(rotate_vector(v, rot));

    return scenePos() + rotEnd;
}

/* Geometry::updateLeader */
void
Geometry::updateLeader()
{
    int arrowStyle = ARROW_STYLE_CLOSED; //TODO: Make this customizable
    EmbReal arrowStyleAngle = 15.0; //TODO: Make this customizable
    EmbReal arrowStyleLength = 1.0; //TODO: Make this customizable
    EmbReal lineStyleAngle = 45.0; //TODO: Make this customizable
    EmbReal lineStyleLength = 1.0; //TODO: Make this customizable

    QLineF lyne = objLine;
    EmbReal angle = lyne.angle();
    QPointF ap0 = lyne.p1();
    QPointF lp0 = lyne.p2();

    //Arrow
    QLineF lynePerp(lyne.pointAt(arrowStyleLength/lyne.length()) ,lp0);
    lynePerp.setAngle(angle + 90);
    QLineF lyne1(ap0, lp0);
    QLineF lyne2(ap0, lp0);
    lyne1.setAngle(angle + arrowStyleAngle);
    lyne2.setAngle(angle - arrowStyleAngle);
    QPointF ap1;
    QPointF ap2;
    lynePerp.intersects(lyne1, &ap1);
    lynePerp.intersects(lyne2, &ap2);

    //Math Diagram
    //                 .(ap1)                     .(lp1)
    //                /|                         /|
    //               / |                        / |
    //              /  |                       /  |
    //             /   |                      /   |
    //            /    |                     /    |
    //           /     |                    /     |
    //          /      |                   /      |
    //         /       |                  /       |
    //        /+(aSA)  |                 /+(lSA)  |
    // (ap0)./__(aSL)__|__________(lp0)./__(lSL)__|
    //       \ -(aSA)  |                \ -(lSA)  |
    //        \        |                 \        |
    //         \       |                  \       |
    //          \      |                   \      |
    //           \     |                    \     |
    //            \    |                     \    |
    //             \   |                      \   |
    //              \  |                       \  |
    //               \ |                        \ |
    //                \|                         \|
    //                 .(ap2)                     .(lp2)

    switch (arrowStyle) {
    case ARROW_STYLE_OPEN: {
        arrowStylePath = QPainterPath();
        arrowStylePath.moveTo(ap1);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap2);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap1);
        break;
    }
    case ARROW_STYLE_CLOSED: {
        arrowStylePath = QPainterPath();
        arrowStylePath.moveTo(ap1);
        arrowStylePath.lineTo(ap0);
        arrowStylePath.lineTo(ap2);
        arrowStylePath.lineTo(ap1);
        break;
    }
    case ARROW_STYLE_DOT: {
        arrowStylePath = QPainterPath();
        arrowStylePath.addEllipse(ap0, arrowStyleLength, arrowStyleLength);
        break;
    }
    case ARROW_STYLE_BOX: {
        arrowStylePath = QPainterPath();
        EmbReal side = QLineF(ap1, ap2).length();
        QRectF ar0(0, 0, side, side);
        ar0.moveCenter(ap0);
        arrowStylePath.addRect(ar0);
        break;
    }
    case ARROW_STYLE_TICK:
        break;
    default:
        break;
    }

    lineStylePath = QPainterPath();
    lineStylePath.moveTo(ap0);
    lineStylePath.lineTo(lp0);
}

/* Geometry::updateRubber
 * painter
 */
void
Geometry::updateRubber(QPainter* painter)
{
    switch (Type) {
    case OBJ_TYPE_DIMLEADER: {
        if (objRubberMode == "OBJ_RUBBER_DIMLEADER_LINE") {
            QPointF sceneStartPoint = objectRubberPoint("DIMLEADER_LINE_START");
            QPointF sceneQSnapPoint = objectRubberPoint("DIMLEADER_LINE_END");

            setObjectEndPoint1(to_EmbVector(sceneStartPoint));
            setObjectEndPoint2(to_EmbVector(sceneQSnapPoint));
        }
        else if (objRubberMode == "OBJ_RUBBER_GRIP") {
            if (painter) {
                QPointF gripPoint = objectRubberPoint("GRIP_POINT");
                if (gripPoint == objectEndPoint1()) {
                    painter->drawLine(objLine.p2(), mapFromScene(objectRubberPoint(QString())));
                }
                else if (gripPoint == objectEndPoint2()) {
                    painter->drawLine(objLine.p1(), mapFromScene(objectRubberPoint(QString())));
                }
                else if (gripPoint == objectMidPoint()) {
                    painter->drawLine(objLine.translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
                }
            }
        }
        break;
    }

    case OBJ_TYPE_ELLIPSE: {
        if (objRubberMode == "OBJ_RUBBER_ELLIPSE_LINE") {
            QPointF sceneLinePoint1 = objectRubberPoint("ELLIPSE_LINE_POINT1");
            QPointF sceneLinePoint2 = objectRubberPoint("ELLIPSE_LINE_POINT2");
            QPointF itemLinePoint1  = mapFromScene(sceneLinePoint1);
            QPointF itemLinePoint2  = mapFromScene(sceneLinePoint2);
            QLineF itemLine(itemLinePoint1, itemLinePoint2);
            if (painter) drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
            updatePath();
        }
        else if (objRubberMode == "OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS") {
            QPointF sceneAxis1Point1 = objectRubberPoint("ELLIPSE_AXIS1_POINT1");
            QPointF sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
            QPointF sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
            QPointF sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
            EmbReal ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
            EmbReal ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

            //TODO: incorporate perpendicularDistance() into libcgeometry
            EmbReal px = sceneAxis2Point2.x();
            EmbReal py = sceneAxis2Point2.y();
            EmbReal x1 = sceneAxis1Point1.x();
            EmbReal y1 = sceneAxis1Point1.y();
            QLineF line(sceneAxis1Point1, sceneAxis1Point2);
            QLineF norm = line.normalVector();
            EmbReal dx = px-x1;
            EmbReal dy = py-y1;
            norm.translate(dx, dy);
            QPointF iPoint;
            norm.intersects(line, &iPoint);
            EmbReal ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length()*2.0;

            setObjectCenter(to_EmbVector(sceneCenterPoint));
            setObjectSize(ellipseWidth, ellipseHeight);
            setRotation(-ellipseRot);

            QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
            QPointF itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
            QLineF itemLine(itemCenterPoint, itemAxis2Point2);
            if (painter) drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
            updatePath();
        }
        else if (objRubberMode == "OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS") {
            QPointF sceneAxis1Point2 = objectRubberPoint("ELLIPSE_AXIS1_POINT2");
            QPointF sceneCenterPoint = objectRubberPoint("ELLIPSE_CENTER");
            QPointF sceneAxis2Point2 = objectRubberPoint("ELLIPSE_AXIS2_POINT2");
            EmbReal ellipseWidth = objectRubberPoint("ELLIPSE_WIDTH").x();
            EmbReal ellipseRot = objectRubberPoint("ELLIPSE_ROT").x();

            //TODO: incorporate perpendicularDistance() into libcgeometry
            EmbReal px = sceneAxis2Point2.x();
            EmbReal py = sceneAxis2Point2.y();
            EmbReal x1 = sceneCenterPoint.x();
            EmbReal y1 = sceneCenterPoint.y();
            QLineF line(sceneCenterPoint, sceneAxis1Point2);
            QLineF norm = line.normalVector();
            EmbReal dx = px-x1;
            EmbReal dy = py-y1;
            norm.translate(dx, dy);
            QPointF iPoint;
            norm.intersects(line, &iPoint);
            EmbReal ellipseHeight = QLineF(px, py, iPoint.x(), iPoint.y()).length()*2.0;

            setObjectCenter(to_EmbVector(sceneCenterPoint));
            setObjectSize(ellipseWidth, ellipseHeight);
            setRotation(-ellipseRot);

            QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
            QPointF itemAxis2Point2 = mapFromScene(sceneAxis2Point2);
            QLineF itemLine(itemCenterPoint, itemAxis2Point2);
            if (painter) drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
            updatePath();
        }
        else if (objRubberMode == "OBJ_RUBBER_GRIP") {
            //TODO: updateRubber() gripping for Geometry
        }

        break;
    }

    case OBJ_TYPE_IMAGE: {
        if (objRubberMode == "OBJ_RUBBER_IMAGE") {
            QPointF sceneStartPoint = objectRubberPoint("IMAGE_START");
            QPointF sceneEndPoint = objectRubberPoint("IMAGE_END");
            EmbReal x = sceneStartPoint.x();
            EmbReal y = sceneStartPoint.y();
            EmbReal w = sceneEndPoint.x() - sceneStartPoint.x();
            EmbReal h = sceneEndPoint.y() - sceneStartPoint.y();
            setObjectRect(x,y,w,h);
            updatePath();
        }
        else if (objRubberMode == "OBJ_RUBBER_GRIP") {
            //TODO: updateRubber() gripping for ImageObject
        }
        break;
    }

    case OBJ_TYPE_LINE: {
        std::string rubberMode = objRubberMode;
        if (rubberMode == "OBJ_RUBBER_LINE") {
            QPointF sceneStartPoint = objectRubberPoint("LINE_START");
            QPointF sceneQSnapPoint = objectRubberPoint("LINE_END");

            setObjectEndPoint1(to_EmbVector(sceneStartPoint));
            setObjectEndPoint2(to_EmbVector(sceneQSnapPoint));

            drawRubberLine(objLine, painter, "VIEW_COLOR_CROSSHAIR");
        }
        else if (rubberMode == "OBJ_RUBBER_GRIP") {
            if (painter) {
                QPointF gripPoint = objectRubberPoint("GRIP_POINT");
                if (gripPoint == objectEndPoint1()) {
                    painter->drawLine(objLine.p2(), mapFromScene(objectRubberPoint(QString())));
                }
                else if (gripPoint == objectEndPoint2()) {
                    painter->drawLine(objLine.p1(), mapFromScene(objectRubberPoint(QString())));
                }
                else if (gripPoint == objectMidPoint())
                    painter->drawLine(objLine.translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));

                QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            }
        }
        break;
    }

    case OBJ_TYPE_CIRCLE: {
        if (objRubberMode == "OBJ_RUBBER_CIRCLE_1P_RAD") {
            QPointF sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
            QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_RADIUS");
            QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
            QPointF itemQSnapPoint = mapFromScene(sceneQSnapPoint);
            QLineF itemLine(itemCenterPoint, itemQSnapPoint);
            setObjectCenter(to_EmbVector(sceneCenterPoint));
            QLineF sceneLine(sceneCenterPoint, sceneQSnapPoint);
            EmbReal radius = sceneLine.length();
            setObjectRadius(radius);
            if (painter) drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
            updatePath();
        }
        else if (objRubberMode == "OBJ_RUBBER_CIRCLE_1P_DIA") {
            QPointF sceneCenterPoint = objectRubberPoint("CIRCLE_CENTER");
            QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_DIAMETER");
            QPointF itemCenterPoint = mapFromScene(sceneCenterPoint);
            QPointF itemQSnapPoint = mapFromScene(sceneQSnapPoint);
            QLineF itemLine(itemCenterPoint, itemQSnapPoint);
            setObjectCenter(to_EmbVector(sceneCenterPoint));
            QLineF sceneLine(sceneCenterPoint, sceneQSnapPoint);
            EmbReal diameter = sceneLine.length();
            setObjectDiameter(diameter);
            if (painter) drawRubberLine(itemLine, painter, "VIEW_COLOR_CROSSHAIR");
            updatePath();
        }
        else if (objRubberMode == "OBJ_RUBBER_CIRCLE_2P") {
            QPointF sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
            QPointF sceneQSnapPoint = objectRubberPoint("CIRCLE_TAN2");
            QLineF sceneLine(sceneTan1Point, sceneQSnapPoint);
            setObjectCenter(to_EmbVector(sceneLine.pointAt(0.5)));
            EmbReal diameter = sceneLine.length();
            setObjectDiameter(diameter);
            updatePath();
        }
        else if (objRubberMode == "OBJ_RUBBER_CIRCLE_3P") {
            QPointF sceneTan1Point = objectRubberPoint("CIRCLE_TAN1");
            QPointF sceneTan2Point = objectRubberPoint("CIRCLE_TAN2");
            QPointF sceneTan3Point = objectRubberPoint("CIRCLE_TAN3");

            EmbArc arc;
            arc.start = to_EmbVector(sceneTan1Point);
            arc.mid = to_EmbVector(sceneTan2Point);
            arc.end = to_EmbVector(sceneTan3Point);
            EmbVector sceneCenter = embArc_center(arc);
            QPointF sceneCenterPoint = to_QPointF(sceneCenter);
            QLineF sceneLine(sceneCenterPoint, sceneTan3Point);
            setObjectCenter(to_EmbVector(sceneCenterPoint));
            EmbReal radius = sceneLine.length();
            setObjectRadius(radius);
            updatePath();
        }
        else if (objRubberMode == "OBJ_RUBBER_GRIP") {
            if (painter) {
                QPointF gripPoint = objectRubberPoint("GRIP_POINT");
                if (gripPoint == scenePos()) {
                    painter->drawEllipse(rect().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
                }
                else {
                    EmbReal gripRadius = QLineF(scenePos(), objectRubberPoint(QString())).length();
                    painter->drawEllipse(QPointF(), gripRadius, gripRadius);
                }

                QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            }
        }
        break;
    }

    case OBJ_TYPE_POINT: {
        if (objRubberMode == "OBJ_RUBBER_GRIP") {
            if (painter) {
                QPointF gripPoint = objectRubberPoint("GRIP_POINT");
                if (gripPoint == scenePos()) {
                    QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
                    drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
                }
            }
        }
        break;
    }

    case OBJ_TYPE_POLYGON: {
        std::string rubberMode = objRubberMode;
        if (rubberMode == "OBJ_RUBBER_POLYGON") {
            setObjectPos(objectRubberPoint("POLYGON_POINT_0"));

            bool ok = false;
            QString numStr = objectRubberText("POLYGON_NUM_POINTS");
            if (numStr.isNull()) return;
            int num = numStr.toInt(&ok);
            if (!ok) return;

            QString appendStr;
            QPainterPath rubberPath;
            rubberPath.moveTo(mapFromScene(objectRubberPoint("POLYGON_POINT_0")));
            for (int i = 1; i <= num; i++) {
                appendStr = "POLYGON_POINT_" + QString().setNum(i);
                QPointF appendPoint = mapFromScene(objectRubberPoint(appendStr));
                rubberPath.lineTo(appendPoint);
            }
            //rubberPath.lineTo(0,0);
            //updatePath(rubberPath);

            //Ensure the path isn't updated until the number of points is changed again
            objRubberTexts.insert("POLYGON_NUM_POINTS", QString());
        }
        else if (rubberMode == "OBJ_RUBBER_POLYGON_INSCRIBE") {
            setObjectPos(objectRubberPoint("POLYGON_CENTER"));

            quint16 numSides = objectRubberPoint("POLYGON_NUM_SIDES").x();

            QPointF inscribePoint = mapFromScene(objectRubberPoint("POLYGON_INSCRIBE_POINT"));
            QLineF inscribeLine = QLineF(QPointF(0,0), inscribePoint);
            EmbReal inscribeAngle = inscribeLine.angle();
            EmbReal inscribeInc = 360.0/numSides;

            if (painter) drawRubberLine(inscribeLine, painter, "VIEW_COLOR_CROSSHAIR");

            QPainterPath inscribePath;
            //First Point
            inscribePath.moveTo(inscribePoint);
            //Remaining Points
            for (int i = 1; i < numSides; i++) {
                inscribeLine.setAngle(inscribeAngle + inscribeInc*i);
                inscribePath.lineTo(inscribeLine.p2());
            }
            // \todo fix this
            //updatePath(inscribePath);
        }
        else if (rubberMode == "OBJ_RUBBER_POLYGON_CIRCUMSCRIBE") {
            setObjectPos(objectRubberPoint("POLYGON_CENTER"));

            quint16 numSides = objectRubberPoint("POLYGON_NUM_SIDES").x();

            QPointF circumscribePoint = mapFromScene(objectRubberPoint("POLYGON_CIRCUMSCRIBE_POINT"));
            QLineF circumscribeLine = QLineF(QPointF(0,0), circumscribePoint);
            EmbReal circumscribeAngle = circumscribeLine.angle();
            EmbReal circumscribeInc = 360.0/numSides;

            if (painter) drawRubberLine(circumscribeLine, painter, "VIEW_COLOR_CROSSHAIR");

            QPainterPath circumscribePath;
            //First Point
            QLineF prev(circumscribeLine.p2(), QPointF(0,0));
            prev = prev.normalVector();
            circumscribeLine.setAngle(circumscribeAngle + circumscribeInc);
            QLineF perp(circumscribeLine.p2(), QPointF(0,0));
            perp = perp.normalVector();
            QPointF iPoint;
            perp.intersects(prev, &iPoint);
            circumscribePath.moveTo(iPoint);
            //Remaining Points
            for (int i = 2; i <= numSides; i++) {
                prev = perp;
                circumscribeLine.setAngle(circumscribeAngle + circumscribeInc*i);
                perp = QLineF(circumscribeLine.p2(), QPointF(0,0));
                perp = perp.normalVector();
                perp.intersects(prev, &iPoint);
                circumscribePath.lineTo(iPoint);
            }
            // \todo fix this
            // updatePath(circumscribePath);
        }
        else if (rubberMode == "OBJ_RUBBER_GRIP") {
            if (painter) {
                int elemCount = normalPath.elementCount();
                QPointF gripPoint = objectRubberPoint("GRIP_POINT");
                if (gripIndex == -1) gripIndex = findIndex(gripPoint);
                if (gripIndex == -1) return;

                int m = 0;
                int n = 0;

                if (!gripIndex) { m = elemCount-1; n = 1; }
                else if (gripIndex == elemCount-1) { m = elemCount-2; n = 0; }
                else { m = gripIndex-1; n = gripIndex+1; }
                QPainterPath::Element em = normalPath.elementAt(m);
                QPainterPath::Element en = normalPath.elementAt(n);
                QPointF emPoint = QPointF(em.x, em.y);
                QPointF enPoint = QPointF(en.x, en.y);
                painter->drawLine(emPoint, mapFromScene(objectRubberPoint(QString())));
                painter->drawLine(enPoint, mapFromScene(objectRubberPoint(QString())));

                QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            }
        }
        break;
    }

    case OBJ_TYPE_POLYLINE: {
        std::string rubberMode = objRubberMode;
        if (rubberMode == "OBJ_RUBBER_POLYLINE") {
            setObjectPos(objectRubberPoint("POLYLINE_POINT_0"));

            QLineF rubberLine(normalPath.currentPosition(), mapFromScene(objectRubberPoint(QString())));
            if (painter) drawRubberLine(rubberLine, painter, "VIEW_COLOR_CROSSHAIR");

            bool ok = false;
            QString numStr = objectRubberText("POLYLINE_NUM_POINTS");
            if (numStr.isNull()) return;
            int num = numStr.toInt(&ok);
            if (!ok) return;

            QString appendStr;
            QPainterPath rubberPath;
            for (int i = 1; i <= num; i++) {
                appendStr = "POLYLINE_POINT_" + QString().setNum(i);
                QPointF appendPoint = mapFromScene(objectRubberPoint(appendStr));
                rubberPath.lineTo(appendPoint);
            }
            // \todo fix this
            //updatePath(rubberPath);

            //Ensure the path isn't updated until the number of points is changed again
            objRubberTexts.insert("POLYLINE_NUM_POINTS", QString());
        }
        else if (rubberMode == "OBJ_RUBBER_GRIP") {
            if (painter) {
                int elemCount = normalPath.elementCount();
                QPointF gripPoint = objectRubberPoint("GRIP_POINT");
                if (gripIndex == -1) gripIndex = findIndex(gripPoint);
                if (gripIndex == -1) return;

                if (!gripIndex) {
                    // First
                    QPainterPath::Element ef = normalPath.elementAt(1);
                    QPointF efPoint = QPointF(ef.x, ef.y);
                    painter->drawLine(efPoint, mapFromScene(objectRubberPoint(QString())));
                }
                else if (gripIndex == elemCount-1) { //Last
                    QPainterPath::Element el = normalPath.elementAt(gripIndex-1);
                    QPointF elPoint = QPointF(el.x, el.y);
                    painter->drawLine(elPoint, mapFromScene(objectRubberPoint(QString())));
                }
                else { //Middle
                    QPainterPath::Element em = normalPath.elementAt(gripIndex-1);
                    QPainterPath::Element en = normalPath.elementAt(gripIndex+1);
                    QPointF emPoint = QPointF(em.x, em.y);
                    QPointF enPoint = QPointF(en.x, en.y);
                    painter->drawLine(emPoint, mapFromScene(objectRubberPoint(QString())));
                    painter->drawLine(enPoint, mapFromScene(objectRubberPoint(QString())));
                }

                QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            }
        }
        break;
    }

    case OBJ_TYPE_RECTANGLE: {
        std::string rubberMode = objRubberMode;
        if (objRubberMode == "OBJ_RUBBER_RECTANGLE") {
            QPointF sceneStartPoint = objectRubberPoint("RECTANGLE_START");
            QPointF sceneEndPoint = objectRubberPoint("RECTANGLE_END");
            EmbReal x = sceneStartPoint.x();
            EmbReal y = sceneStartPoint.y();
            EmbReal w = sceneEndPoint.x() - sceneStartPoint.x();
            EmbReal h = sceneEndPoint.y() - sceneStartPoint.y();
            setObjectRect(x,y,w,h);
            updatePath();
        }
        else if (rubberMode == "OBJ_RUBBER_GRIP") {
            if (painter) {
                //TODO: Make this work with rotation & scaling
                /*
                QPointF gripPoint = objectRubberPoint("GRIP_POINT");
                QPointF after = objectRubberPoint(QString());
                QPointF delta = after-gripPoint;
                if     (gripPoint == objectTopLeft()) { painter->drawPolygon(mapFromScene(QRectF(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y()))); }
                else if (gripPoint == objectTopRight()) {
                    painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y())));
                }
                else if (gripPoint == objectBottomLeft()) { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y()))); }
                else if (gripPoint == objectBottomRight()) { painter->drawPolygon(mapFromScene(QRectF(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y()))); }

                QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
                */

                QPointF gripPoint = objectRubberPoint("GRIP_POINT");
                QPointF after = objectRubberPoint(QString());
                QPointF delta = after-gripPoint;

                QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            }
        }
        break;
    }

    case OBJ_TYPE_TEXTSINGLE: {
        if (objRubberMode == "OBJ_RUBBER_TEXTSINGLE") {
            setObjectTextFont(objectRubberText("TEXT_FONT"));
            setObjectTextJustify(objectRubberText("TEXT_JUSTIFY"));
            setObjectPos(objectRubberPoint("TEXT_POINT"));
            QPointF hr = objectRubberPoint("TEXT_HEIGHT_ROTATION");
            setObjectTextSize(hr.x());
            setRotation(hr.y());
            setObjectText(objectRubberText("TEXT_RAPID"));
        }
        else if (objRubberMode == "OBJ_RUBBER_GRIP") {
            if (painter) {
                QPointF gripPoint = objectRubberPoint("GRIP_POINT");
                if (gripPoint == scenePos()) {
                    painter->drawPath(path().translated(mapFromScene(objectRubberPoint(QString()))-mapFromScene(gripPoint)));
                }

                QLineF rubLine(mapFromScene(gripPoint), mapFromScene(objectRubberPoint(QString())));
                drawRubberLine(rubLine, painter, "VIEW_COLOR_CROSSHAIR");
            }
        }
        break;
    }

    default:
        break;
    }
}

/* . */
EmbReal
Geometry::objectRadius()
{
    return rect().width()/2.0*scale();
}

/* . */
EmbReal
Geometry::objectDiameter()
{
    return 2.0 * objectRadius() * scale();
}

/* . */
EmbReal
Geometry::objectCircumference()
{
    return embConstantPi * objectDiameter();
}

/* QPointF. */
QPointF
Geometry::objectQuadrant0()
{
    if (Type == OBJ_TYPE_ELLIPSE) {
        EmbReal rot = radians(rotation());
        EmbVector v = embVector_scale(embVector_unit(rot), objectWidth()/2.0);
        return scenePos() + to_QPointF(v);
    }
    return scenePos() + QPointF(objectRadius(), 0);
}

/* QPointF. */
QPointF
Geometry::objectQuadrant90()
{
    if (Type == OBJ_TYPE_ELLIPSE) {
        EmbReal rot = radians(rotation()+90.0);
        EmbVector v = embVector_scale(embVector_unit(rot), objectHeight()/2.0);
        return scenePos() + to_QPointF(v);
    }
    return scenePos() + QPointF(0,-objectRadius());
}

/* QPointF. */
QPointF
Geometry::objectQuadrant180()
{
    if (Type == OBJ_TYPE_ELLIPSE) {
        EmbReal rot = radians(rotation()+180.0);
        EmbVector v = embVector_scale(embVector_unit(rot), objectWidth()/2.0);
        return scenePos() + to_QPointF(v);
    }
    return scenePos() + QPointF(-objectRadius(),0);
}

/* QPointF. */
QPointF
Geometry::objectQuadrant270()
{
    if (Type == OBJ_TYPE_ELLIPSE) {
        EmbReal rot = radians(rotation()+270.0);
        EmbVector v = embVector_scale(embVector_unit(rot), objectHeight()/2.0);
        return scenePos() + to_QPointF(v);
    }
    return scenePos() + QPointF(0, objectRadius());
}

/* EmbReal. */
EmbReal
Geometry::objectWidth()
{
    return rect().width()*scale();
}

/* EmbReal. */
EmbReal
Geometry::objectHeight()
{
    return rect().height()*scale();
}

/* EmbReal. */
EmbReal
Geometry::objectRadiusMajor()
{
    return std::max(rect().width(), rect().height())/2.0*scale();
}

/* EmbReal. */
EmbReal
Geometry::objectRadiusMinor()
{
    return std::min(rect().width(), rect().height())/2.0*scale();
}

/* EmbReal. */
EmbReal
Geometry::objectDiameterMajor()
{
    return std::max(rect().width(), rect().height())*scale();
}

/* EmbReal. */
EmbReal
Geometry::objectDiameterMinor()
{
    return std::min(rect().width(), rect().height())*scale();
}

/* . */
void
Geometry::setObjectCenter(EmbVector center)
{
    setPos(center.x, center.y);
}

/* . */
QRectF
Geometry::rect()
{
    return path().boundingRect();
}

/* . */
void
Geometry::setRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h)
{
    QPainterPath p;
    p.addRect(x, y, w, h);
    setPath(p);
}

/* . */
void
Geometry::setLine(const QLineF& li)
{
    QPainterPath p;
    p.moveTo(li.p1());
    p.lineTo(li.p2());
    setPath(p);
    objLine = li;
}

/* . */
void
Geometry::setLine(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2)
{
    QPainterPath p;
    p.moveTo(x1,y1);
    p.lineTo(x2,y2);
    setPath(p);
    objLine.setLine(x1,y1,x2,y2);
}

/* Geometry::vulcanize */
void
Geometry::vulcanize(void)
{
    debug_message("DimLeaderObject vulcanize()");
    updateRubber();

    objRubberMode = "OBJ_RUBBER_OFF";

    if (Type == OBJ_TYPE_POLYGON) {
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0, translate_str("Empty Polygon Error"), translate_str("The polygon added contains no points. The command that created this object has flawed logic."));
        }
    }
    if (Type == OBJ_TYPE_POLYLINE) {
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0,
                translate_str("Empty Polyline Error"),
                translate_str("The polyline added contains no points. The command that created this object has flawed logic."));
        }
    }
    if (Type == OBJ_TYPE_PATH) {
        if (!normalPath.elementCount()) {
            QMessageBox::critical(0,
                translate_str("Empty Path Error"),
                translate_str("The path added contains no points. The command that created this object has flawed logic."));
        }
    }
}

/* Geometry::gripEdit
 * before, after
 */
void
Geometry::gripEdit(const QPointF& before, const QPointF& after)
{
    QPointF delta = after-before;

    switch (Type) {
    case OBJ_TYPE_LINE:
    case OBJ_TYPE_DIMLEADER:
        if (before == objectEndPoint1()) {
            setObjectEndPoint1(to_EmbVector(after));
        }
        else if (before == objectEndPoint2()) {
            setObjectEndPoint2(to_EmbVector(after));
        }
        else if (before == objectMidPoint()) {
            moveBy(delta.x(), delta.y());
        }
        break;

    case OBJ_TYPE_CIRCLE:
        if (before == scenePos()) {
            moveBy(delta.x(), delta.y());
        }
        else {
            setObjectRadius(QLineF(scenePos(), after).length());
        }
        break;

    case OBJ_TYPE_POINT:
    case OBJ_TYPE_TEXTSINGLE:
        if (before == scenePos()) {
            moveBy(delta.x(), delta.y());
        }
        break;

    case OBJ_TYPE_PATH:
    case OBJ_TYPE_POLYLINE:
    case OBJ_TYPE_POLYGON: {
        gripIndex = findIndex(before);
        if (gripIndex == -1)
            return;
        QPointF a = mapFromScene(after);
        normalPath.setElementPositionAt(gripIndex, a.x(), a.y());
        updatePath();
        gripIndex = -1;
        break;
    }

    case OBJ_TYPE_RECTANGLE:
        if (before == objectTopLeft()) {
            setObjectRect(after.x(), after.y(), objectWidth()-delta.x(), objectHeight()-delta.y());
        }
        else if (before == objectTopRight()) {
            setObjectRect(objectTopLeft().x(), objectTopLeft().y()+delta.y(), objectWidth()+delta.x(), objectHeight()-delta.y());
        }
        else if (before == objectBottomLeft()) {
            setObjectRect(objectTopLeft().x()+delta.x(), objectTopLeft().y(), objectWidth()-delta.x(), objectHeight()+delta.y());
        }
        else if (before == objectBottomRight()) {
            setObjectRect(objectTopLeft().x(), objectTopLeft().y(), objectWidth()+delta.x(), objectHeight()+delta.y());
        }
        break;

    default:
        break;
    }
}

/* Geometry::objectAngle */
EmbReal
Geometry::objectAngle()
{
    EmbReal angle = objLine.angle() - rotation();
    return std::fmod(angle, 360.0);
}

/* Geometry::objectSavePath */
QPainterPath
Geometry::objectSavePath()
{
    QPainterPath path;
    switch (Type) {
    case OBJ_TYPE_CIRCLE: {
        QRectF r = rect();
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        break;
    }

    case OBJ_TYPE_ELLIPSE: {
        QRectF r = rect();
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        break;
    }

    case OBJ_TYPE_LINE: {
        /* \todo discards qualifiers error?
        QPointF delta = objectDelta();
        path.moveTo(objectStartPoint());
        path.lineTo(delta.x(), delta.y());
        */
        break;
    }

    case OBJ_TYPE_POINT: {
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
        break;
    }

    case OBJ_TYPE_POLYLINE:
    case OBJ_TYPE_PATH: {
        path = normalPath;
        break;
    }

    case OBJ_TYPE_POLYGON: {
        path = normalPath;
        path.closeSubpath();
        break;
    }

    case OBJ_TYPE_RECTANGLE: {
        QRectF r = rect();
        path.moveTo(r.bottomLeft());
        path.lineTo(r.bottomRight());
        path.lineTo(r.topRight());
        path.lineTo(r.topLeft());
        path.lineTo(r.bottomLeft());
        break;
    }

    default:
        break;
    }

    switch (Type) {
    case OBJ_TYPE_CIRCLE:
    case OBJ_TYPE_ELLIPSE:
    case OBJ_TYPE_POLYLINE:
    case OBJ_TYPE_POLYGON:
    case OBJ_TYPE_PATH:
    case OBJ_TYPE_RECTANGLE: {
        EmbReal s = scale();
        QTransform trans;
        trans.rotate(rotation());
        trans.scale(s,s);
        return trans.map(path);
    }
    default:
        break;
    }

    return path;
}

typedef std::string String;

/* Geometry::calculateArcData
 * arc
 *
 * TODO: convert this to update and make it Type sensitive.
 */
void Geometry::calculateArcData(EmbArc arc)
{
    EmbVector center = embArc_center(arc);

    arcStartPoint = to_QPointF(embVector_subtract(arc.start, center));
    arcMidPoint = to_QPointF(embVector_subtract(arc.mid, center));
    arcEndPoint = to_QPointF(embVector_subtract(arc.end, center));

    setPos(center.x, center.y);

    EmbReal radius = embVector_distance(center, arc.mid);
    updateArcRect(radius);
    updatePath();
    setRotation(0);
    setScale(1);
}

/* Update arc rect radius. */
void
Geometry::updateArcRect(EmbReal radius)
{
    QRectF elRect;
    elRect.setWidth(radius*2.0);
    elRect.setHeight(radius*2.0);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect.x(), elRect.y(), elRect.width(), elRect.height());
}

/* Set object radius to radius. */
void
Geometry::setObjectRadius(EmbReal radius)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbReal rad = std::max(radius, 0.0000001f);

        QPointF center = scenePos();
        QLineF startLine = QLineF(center, objectStartPoint());
        QLineF midLine   = QLineF(center, objectMidPoint());
        QLineF endLine   = QLineF(center, objectEndPoint());
        startLine.setLength(rad);
        midLine.setLength(rad);
        endLine.setLength(rad);
        arcStartPoint = startLine.p2();
        arcMidPoint = midLine.p2();
        arcEndPoint = endLine.p2();
        gdata.arc.start = to_EmbVector(arcStartPoint);
        gdata.arc.mid = to_EmbVector(arcMidPoint);
        gdata.arc.end = to_EmbVector(arcEndPoint);

        calculateArcData(gdata.arc);
        break;
    }

    case OBJ_TYPE_CIRCLE: {
        setObjectDiameter(radius*2.0);
        break;
    }

    default:
        break;
    }
}

/* Set object start angle to angle. */
void
Geometry::setObjectStartAngle(EmbReal angle)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        //TODO: Geometry setObjectStartAngle
        break;
    }
    default:
        break;
    }
}

/* SetObjectEndAngle to angle. */
void
Geometry::setObjectEndAngle(EmbReal angle)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        //TODO: Geometry setObjectEndAngle
        break;
    }
    default:
        break;
    }
}

/* Geometry::setObjectPoint
 * point
 */
void
Geometry::setObjectPoint(EmbVector point, int64_t point_type)
{
    switch (point_type) {
    case VECTOR_ARC_START_POINT: {;
        EmbArc arc;
        arc.start = point;
        arc.mid = to_EmbVector(objectMidPoint());
        arc.mid = to_EmbVector(objectEndPoint());
        calculateArcData(arc);
        break;
    }
    case VECTOR_ARC_MID_POINT: {
        EmbArc arc;
        arc.start = to_EmbVector(objectStartPoint());
        arc.mid = point;
        arc.end = to_EmbVector(objectEndPoint());
        calculateArcData(arc);
        break;
    }
    case VECTOR_ARC_END_POINT: {
        EmbArc arc;
        arc.start = to_EmbVector(objectStartPoint());
        arc.mid = to_EmbVector(objectMidPoint());
        arc.end = point;
        calculateArcData(arc);
        break;
    }
    default:
        break;
    }
}

/* Geometry::setObjectStartPoint
 * point
 */
void
Geometry::setObjectStartPoint(EmbVector point)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        setObjectPoint(point, VECTOR_ARC_START_POINT);
        break;
    }
    default:
        break;
    }
}

/* Geometry::setObjectMidPoint
 * point
 */
void
Geometry::setObjectMidPoint(EmbVector point)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        setObjectPoint(point, VECTOR_ARC_MID_POINT);
        break;
    }
    default:
        break;
    }
}

/* Set object endPoint to point. */
void
Geometry::setObjectEndPoint(EmbVector point)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        setObjectPoint(point, VECTOR_ARC_END_POINT);
        break;
    }
    default:
        break;
    }
}

/* ObjectStartAngle */
EmbReal
Geometry::objectStartAngle(void)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbReal angle = QLineF(scenePos(), objectStartPoint()).angle();
        return std::fmod(angle, 360.0);
    }
    default:
        break;
    }
    return 0.0f;
}

/* objectEndAngle */
EmbReal
Geometry::objectEndAngle(void)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbReal angle = QLineF(scenePos(), objectEndPoint()).angle();
        return std::fmod(angle, 360.0);
    }
    default:
        break;
    }
    return 0.0f;
}

/* . */
EmbVector
rotate_vector(EmbVector v, EmbReal alpha)
{
    EmbVector rotv;
    EmbVector u = embVector_unit(alpha);
    rotv.x = v.x*u.x - v.y*u.y;
    rotv.y = v.x*u.y + v.y*u.x;
    return rotv;
}

/* ObjectStartPoint */
QPointF
Geometry::objectStartPoint()
{
    EmbReal rot = radians(rotation());
    QPointF start_point = objLine.pointAt(0.0);
    if (Type == OBJ_TYPE_ARC) {
        start_point = arcMidPoint;
    }
    EmbVector start = embVector_scale(to_EmbVector(start_point), scale());
    QPointF rotv = to_QPointF(rotate_vector(start, rot));

    return scenePos() + rotv;
}

/* ObjectMidPoint */
QPointF
Geometry::objectMidPoint()
{
    EmbReal rot = radians(rotation());
    QPointF mid_point = objLine.pointAt(0.5);
    if (Type == OBJ_TYPE_ARC) {
        mid_point = arcMidPoint;
    }
    EmbVector mid = embVector_scale(to_EmbVector(mid_point), scale());
    QPointF rotv = to_QPointF(rotate_vector(mid, rot));

    return scenePos() + rotv;
}

/* ObjectEndPoint */
QPointF Geometry::objectEndPoint()
{
    EmbReal rot = radians(rotation());
    QPointF end_point = objLine.pointAt(1.0);
    if (Type == OBJ_TYPE_ARC) {
        end_point = arcEndPoint;
    }
    EmbVector end = embVector_scale(to_EmbVector(end_point), scale());
    QPointF rotv = to_QPointF(rotate_vector(end, rot));

    return scenePos() + rotv;
}

/* objectArea */
EmbReal
Geometry::objectArea()
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        //Area of a circular segment
        EmbReal r = objectRadius();
        EmbReal theta = radians(objectIncludedAngle());
        return ((r*r)/2)*(theta - std::sin(theta));
    }
    case OBJ_TYPE_CIRCLE:
        return embConstantPi * objectRadius() * objectRadius();
    default:
        break;
    }
    return std::fabs(objectWidth()*objectHeight());
}

EmbReal
Geometry::objectReal(int64_t real_type)
{

    return 0.0f;
}

/* ObjectArcLength. */
EmbReal
Geometry::objectArcLength(void)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        return radians(objectIncludedAngle())*objectRadius();
    }
    default:
        break;
    }
    return 0.0f;
}

/* objectChord */
EmbReal
Geometry::objectChord(void)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        return embVector_distance(
            to_EmbVector(objectStartPoint()),
            to_EmbVector(objectEndPoint()));
    }
    default:
        break;
    }
    return 0.0f;
}

/* objectIncludedAngle */
EmbReal
Geometry::objectIncludedAngle(void)
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbReal chord = objectChord();
        EmbReal rad = objectRadius();
        if (chord <= 0 || rad <= 0) return 0; //Prevents division by zero and non-existant circles

        //NOTE: Due to floating point rounding errors, we need to clamp the quotient so it is in the range [-1, 1]
        //      If the quotient is out of that range, then the result of asin() will be 0.0f.
        EmbReal quotient = chord/(2.0*rad);
        if (quotient > 1.0) quotient = 1.0;
        if (quotient < 0.0) quotient = 0.0; //NOTE: 0 rather than -1 since we are enforcing a positive chord and radius
        return degrees(2.0*asin(quotient)); //Properties of a Circle - Get the Included Angle - Reference: ASD9
    }
    default:
        break;
    }
    return 0.0f;
}

/* objectClockwise */
bool
Geometry::objectClockwise()
{
    switch (Type) {
    case OBJ_TYPE_ARC: {
        //NOTE: Y values are inverted here on purpose
        EmbArc arc;
        arc.start.x = objectStartPoint().x();
        arc.start.y = -objectStartPoint().y();
        arc.mid.x = objectMidPoint().x();
        arc.mid.y = -objectMidPoint().y();
        arc.end.x = objectEndPoint().x();
        arc.end.y = -objectEndPoint().y();
        if (embArc_clockwise(arc)) {
            return true;
        }
        break;
    }
    default:
        break;
    }
    return false;
}

/* paint */
void
Geometry::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    QGraphicsScene* objScene = scene();
    if (!objScene) {
        return;
    }

    QPen paintPen = pen();
    painter->setPen(paintPen);

    updateRubber(painter);

    if (option->state & QStyle::State_Selected) {
        paintPen.setStyle(Qt::DashLine);
    }
    if (objScene->property("ENABLE_LWT").toBool()) {
        paintPen = lwtPen;
    }
    painter->setPen(paintPen);

    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbReal startAngle = (objectStartAngle() + rotation())*16;
        EmbReal spanAngle = objectIncludedAngle()*16;

        if (objectClockwise()) {
            spanAngle = -spanAngle;
        }

        EmbReal rad = objectRadius();
        QRectF paintRect(-rad, -rad, rad*2.0, rad*2.0);
        painter->drawArc(paintRect, startAngle, spanAngle);
        break;
    }

    case OBJ_TYPE_CIRCLE: {
        painter->drawEllipse(rect());
        break;
    }

    case OBJ_TYPE_DIMLEADER: {
        painter->drawPath(lineStylePath);
        painter->drawPath(arrowStylePath);

        if (flags & PROP_FILLED) {
            painter->fillPath(arrowStylePath, objPen.color());
        }
        break;
    }

    case OBJ_TYPE_ELLIPSE: {
        painter->drawEllipse(rect());
        break;
    }

    case OBJ_TYPE_IMAGE:
    case OBJ_TYPE_RECTANGLE: {
        painter->drawRect(rect());
        break;
    }

    case OBJ_TYPE_PATH: {
        painter->drawPath(path());
        break;
    }

    case OBJ_TYPE_POLYGON: {
        if (normalPath.elementCount()) {
            painter->drawPath(normalPath);
            QPainterPath::Element zero = normalPath.elementAt(0);
            QPainterPath::Element last = normalPath.elementAt(normalPath.elementCount()-1);
            painter->drawLine(QPointF(zero.x, zero.y), QPointF(last.x, last.y));
        }
        break;
    }

    case OBJ_TYPE_POLYLINE: {
        painter->drawPath(normalPath);

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
            realRender(painter, normalPath);
        }
        break;
    }

    case OBJ_TYPE_LINE: {
        if (objRubberMode != "OBJ_RUBBER_LINE") {
            painter->drawLine(objLine);
        }

        if (objScene->property("ENABLE_LWT").toBool()
            && objScene->property("ENABLE_REAL").toBool()) {
            realRender(painter, path());
        }
        break;
    }

    case OBJ_TYPE_POINT: {
        painter->drawPoint(0,0);
        break;
    }

    case OBJ_TYPE_TEXTSINGLE: {
        painter->drawPath(objTextPath);
        break;
    }

    default:
        break;
    }
}

/* Set object diameter. */
void
Geometry::setObjectDiameter(EmbReal diameter)
{
    QRectF elRect;
    elRect.setWidth(diameter);
    elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect.x(), elRect.y(), elRect.width(), elRect.height());
    updatePath();
}

/* Set object area. */
void
Geometry::setObjectArea(EmbReal area)
{
    EmbReal radius = sqrt(area / embConstantPi);
    setObjectRadius(radius);
}

/* Set object circumference. */
void
Geometry::setObjectCircumference(EmbReal circumference)
{
    EmbReal diameter = circumference / embConstantPi;
    setObjectDiameter(diameter);
}

/* . */
void
Geometry::setObjectSize(EmbReal width, EmbReal height)
{
    QRectF elRect = rect();
    elRect.setWidth(width);
    elRect.setHeight(height);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect.x(), elRect.y(), width, height);
}

/* . */
void
Geometry::setObjectRadiusMajor(EmbReal radius)
{
    setObjectDiameterMajor(radius*2.0);
}

/* . */
void
Geometry::setObjectRadiusMinor(EmbReal radius)
{
    setObjectDiameterMinor(radius*2.0);
}

void
Geometry::setObjectDiameterMajor(EmbReal diameter)
{
    QRectF elRect = rect();
    if (elRect.width() > elRect.height())
        elRect.setWidth(diameter);
    else
        elRect.setHeight(diameter);
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect.x(), elRect.y(), elRect.width(), elRect.height());
}

void
Geometry::setObjectDiameterMinor(EmbReal diameter)
{
    QRectF elRect = rect();
    if (elRect.width() < elRect.height()) {
        elRect.setWidth(diameter);
    }
    else {
        elRect.setHeight(diameter);
    }
    elRect.moveCenter(QPointF(0,0));
    setRect(elRect.x(), elRect.y(), elRect.width(), elRect.height());
}

/*
ImageObject::ImageObject(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, QGraphicsItem* parent) : Geometry(OBJ_TYPE_IMAGE, parent)
{
    debug_message("ImageObject Constructor()");
    init(x, y, w, h, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

ImageObject::ImageObject(ImageObject* obj, QGraphicsItem* parent) : Geometry(OBJ_TYPE_IMAGE, parent)
{
    debug_message("ImageObject Constructor()");
    if (obj) {
        QPointF ptl = obj->objectTopLeft();
        init(ptl.x(), ptl.y(), obj->objectWidth(), obj->objectHeight(), obj->objPen.color().rgb(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
    }
}

// WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
// WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
// WARNING: All movement has to be handled explicitly by us, not by the scene.
void
Geometry::ImageObject::init(EmbReal x, EmbReal y, EmbReal w, EmbReal h, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_IMAGE);
    setData(OBJ_NAME, "Image");

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setObjectRect(x, y, w, h);
    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}
*/

/* . */
void
Geometry::setObjectRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h)
{
    setPos(x, y);
    setRect(0, 0, w, h);
    updatePath();
}

/* The top left corner location as a QPointF. */
QPointF
Geometry::objectTopLeft()
{
    EmbReal rot = radians(rotation());
    QPointF tl = rect().topLeft();
    EmbVector ptl = embVector_scale(to_EmbVector(tl), scale());
    EmbVector ptlRot = rotate_vector(ptl, rot);

    return scenePos() + to_QPointF(ptlRot);
}

/* QPointF. */
QPointF
Geometry::objectTopRight()
{
    EmbReal rot = radians(rotation());
    QPointF tr = rect().topRight();
    EmbVector ptr = embVector_scale(to_EmbVector(tr), scale());
    EmbVector ptrRot = rotate_vector(ptr, rot);

    return (scenePos() + QPointF(ptrRot.x, ptrRot.y));
}

/* QPointF. */
QPointF
Geometry::objectBottomLeft()
{
    EmbReal rot = radians(rotation());
    QPointF bl = rect().bottomLeft();
    EmbVector pbl = embVector_scale(to_EmbVector(bl), scale());
    EmbVector pblRot = rotate_vector(pbl, rot);

    return scenePos() + to_QPointF(pblRot);
}

/* QPointF */
QPointF
Geometry::objectBottomRight()
{
    EmbReal rot = radians(rotation());
    QPointF br = rect().bottomRight();
    EmbVector pbr = embVector_scale(to_EmbVector(br), scale());
    EmbVector pbrRot = rotate_vector(pbr, rot);

    return scenePos() + to_QPointF(pbrRot);
}

/*
PathObject::PathObject(EmbReal x, EmbReal y, const QPainterPath p, QRgb rgb, QGraphicsItem* parent) : Geometry(OBJ_TYPE_PATH, parent)
{
    debug_message("PathObject Constructor()");
    init(x, y, p, rgb, Qt::SolidLine); //TODO: getCurrentLineType
}

PathObject::PathObject(PathObject* obj, QGraphicsItem* parent) : Geometry(OBJ_TYPE_PATH, parent)
{
    debug_message("PathObject Constructor()");
    if (obj) {
        init(obj->scenePos().x(), obj->scenePos().y(), obj->objectCopyPath(), obj->objPen.color().rgb(), Qt::SolidLine); //TODO: getCurrentLineType
        setRotation(obj->rotation());
        setScale(obj->scale());
    }
}
*/

/* WARNING: DO NOT enable QGraphicsItem::ItemIsMovable. If it is enabled,
 * WARNING: and the item is double clicked, the scene will erratically move the item while zooming.
 * WARNING: All movement has to be handled explicitly by us, not by the scene.
 */
/*
void
Geometry::init(EmbVector position, const QPainterPath& p, QRgb rgb, Qt::PenStyle lineType)
{
    setData(OBJ_TYPE, OBJ_TYPE_PATH);
    setData(OBJ_NAME, "Path");

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    normalPath = p;
    updatePath();
    setObjectPos(position.x, position.y);
    objPen.setColor(rgb);
    lwtPen.setColor(rgb);
    objPen.setStyle(lineType);
    lwtPen.setStyle(lineType);
    setObjectLineWeight("0.35"); //TODO: pass in proper lineweight
    setPen(objPen);
}
*/

/* objectCopyPath */
QPainterPath
Geometry::objectCopyPath()
{
    return normalPath;
}

/* Update path.
 * For path and polyline set normalPath before calling.
 */
void
Geometry::updatePath()
{
    QPainterPath path;
    QRectF r = rect();
    switch (Type) {
    case OBJ_TYPE_ARC: {
        EmbReal startAngle = (objectStartAngle() + rotation());
        EmbReal spanAngle = objectIncludedAngle();

        if (objectClockwise()) {
            spanAngle = -spanAngle;
        }

        QPainterPath path;
        path.arcMoveTo(rect(), startAngle);
        path.arcTo(rect(), startAngle, spanAngle);
        //NOTE: Reverse the path so that the inside area isn't considered part of the arc
        path.arcTo(rect(), startAngle+spanAngle, -spanAngle);
        setPath(path);
        break;
    }

    case OBJ_TYPE_CIRCLE: {
        //Add the center point
        path.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
        //Add the circle
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        //NOTE: Reverse the path so that the inside area isn't considered part of the circle
        path.arcTo(r, 0, -360);
        setPath(path);
        break;
    }

    case OBJ_TYPE_ELLIPSE: {
        path.arcMoveTo(r, 0);
        path.arcTo(r, 0, 360);
        //NOTE: Reverse the path so that the inside area isn't considered part of the ellipse
        path.arcTo(r, 0, -360);
        setPath(path);
        break;
    }

    case OBJ_TYPE_RECTANGLE:
    case OBJ_TYPE_IMAGE: {
        path.moveTo(r.bottomLeft());
        path.lineTo(r.bottomRight());
        path.lineTo(r.topRight());
        path.lineTo(r.topLeft());
        path.lineTo(r.bottomLeft());
        //NOTE: Reverse the path so that the inside area isn't considered part of the rectangle
        path.lineTo(r.topLeft());
        path.lineTo(r.topRight());
        path.lineTo(r.bottomRight());
        path.moveTo(r.bottomLeft());
        setPath(path);
        break;
    }

    case OBJ_TYPE_POLYGON: {
        QPainterPath closedPath = normalPath;
        closedPath.closeSubpath();
        QPainterPath reversePath = closedPath.toReversed();
        reversePath.connectPath(closedPath);
        setPath(reversePath);
        break;
    }

    case OBJ_TYPE_PATH: {
        QPainterPath reversePath = normalPath.toReversed();
        reversePath.connectPath(normalPath);
        setPath(reversePath);
        break;
    }

    default:
        break;
    }
}

/* Find index of a point within a path.
 * Return as an int, if not found return -1.
 */
int
Geometry::findIndex(const QPointF& point)
{
    int i;
    int elemCount = normalPath.elementCount();
    //NOTE: Points here are in item coordinates
    QPointF itemPoint = mapFromScene(point);
    for (i = 0; i < elemCount; i++) {
        QPainterPath::Element e = normalPath.elementAt(i);
        QPointF elemPoint = QPointF(e.x, e.y);
        if (itemPoint == elemPoint) {
            return i;
        }
    }
    return -1;
}

/* UpdatePath */
void
Geometry::updatePath(const QPainterPath& p)
{
    normalPath = p;
    QPainterPath reversePath = normalPath.toReversed();
    reversePath.connectPath(normalPath);
    setPath(reversePath);
}

/* Returns whether the save to file process was successful.
 *
 * \todo Before saving to a stitch only format, Embroidermodder needs
 * to calculate the optimal path to minimize jump stitches. Also
 * based upon which layer needs to be stitched first,
 * the path to the next object needs to be hidden beneath fills
 * that will come later. When finding the optimal path, we need
 * to take into account the color of the thread, as we do not want
 * to try to hide dark colored stitches beneath light colored fills.
 */
bool
save(View* view, QString fileName)
{
    qDebug("SaveObject save(%s)", qPrintable(fileName));
    QGraphicsScene* gscene = view->scene();
    QUndoStack* stack = view->undoStack;
    if (!(gscene && stack)) {
        return false;
    }

    bool writeSuccessful = false;

    view->formatType = formatTable[emb_identify_format(qPrintable(fileName))].type;
    if (view->formatType == EMBFORMAT_UNSUPPORTED) {
        return false;
    }

    /* Write */
    if (emb_identify_format(qPrintable(fileName)) < 0) {
        qDebug("Unsupported write file type: %s", qPrintable(fileName));
        return false;
    }

    EmbPattern* pattern = embPattern_create();
    if (!pattern) {
        debug_message("Could not allocate memory for embroidery pattern");
        return false;
    }

    QList<QGraphicsItem*> list = gscene->items(Qt::AscendingOrder);
    for (int i=0; i<(int)list.size(); i++) {
        QGraphicsItem* item = list[i];
        int objType = item->data(OBJ_TYPE).toInt();
        Geometry* obj = static_cast<Geometry*>(item);
        if (!obj) {
            continue;
        }
        if (view->formatType == EMBFORMAT_STITCHONLY) {
            saveObjectAsStitches(objType, view, obj);
        }
        else {
            saveObject(objType, view, obj);
        }
    }

    /*
    //TODO: handle EMBFORMAT_STCHANDOBJ also
    if (view->formatType == EMBFORMAT_STITCHONLY)
        embPattern_movePolylinesToStitchList(pattern); //TODO: handle all objects like this
    */

    // TODO: check the embLog for errors and if any exist, report them.
    writeSuccessful = embPattern_writeAuto(pattern, qPrintable(fileName));
    if (!writeSuccessful) {
        qDebug("Writing file %s was unsuccessful", qPrintable(fileName));
    }

    //TODO: check the embLog for errors and if any exist, report them.
    embPattern_free(pattern);

    return writeSuccessful;
}

/*
 * TODO: proper layer/lineType/lineWeight into toPolyline
 */
void
saveObjectAsStitches(int objType, View *view, Geometry *obj)
{
    QPainterPath path = obj->objectSavePath();
    QPointF position = obj->scenePos();
    QColor color = obj->objPen.color();
    switch (objType) {
    case OBJ_TYPE_ARC: {
        debug_message("TODO: save Arc object");
        break;
    }
    case OBJ_TYPE_BLOCK: {
        debug_message("TODO: save Block object");
        break;
    }
    case OBJ_TYPE_CIRCLE: {
        // TODO: proper layer/lineType/lineWeight
        // TODO: Improve precision, replace simplified
        toPolyline(view, position, path.simplified(), "0", color, "CONTINUOUS", "BYLAYER");
        break;
    }
    case OBJ_TYPE_DIMALIGNED: {
        debug_message("TODO: save DimAligned object");
        break;
    }
    case OBJ_TYPE_DIMANGULAR: {
        debug_message("TODO: save DimAngular object");
        break;
    }
    case OBJ_TYPE_DIMARCLENGTH: {
        debug_message("TODO: save DimArcLength object");
        break;
    }
    case OBJ_TYPE_DIMDIAMETER: {
        debug_message("TODO: save DimDiameter object");
        break;
    }
    case OBJ_TYPE_DIMLEADER: {
        debug_message("TODO: save DimLeader object");
        break;
    }
    case OBJ_TYPE_DIMLINEAR: {
        debug_message("TODO: save DimLinear object");
        break;
    }
    case OBJ_TYPE_DIMORDINATE: {
        debug_message("TODO: save DimOrdinate object");
        break;
    }
    case OBJ_TYPE_DIMRADIUS: {
        debug_message("TODO: save DimRadius object");
        break;
    }
    case OBJ_TYPE_ELLIPSE: {
        // TODO: proper layer/lineType/lineWeight
        // TODO: Improve precision, replace simplified
        toPolyline(view, position, path.simplified(), "0", color, "CONTINUOUS", "BYLAYER");
        break;
    }
    case OBJ_TYPE_ELLIPSEARC: {
        debug_message("TODO: save EllipseArc object");
        break;
    }
    case OBJ_TYPE_GRID: {
        debug_message("TODO: save Grid object");
        break;
    }
    case OBJ_TYPE_HATCH: {
        debug_message("TODO: save Hatch object");
        break;
    }
    case OBJ_TYPE_IMAGE: {
        debug_message("TODO: save Image object");
        break;
    }
    case OBJ_TYPE_INFINITELINE: {
        debug_message("TODO: save InfiniteLine object");
        break;
    }

    // TODO: proper layer/lineType/lineWeight
    case OBJ_TYPE_LINE: {
        toPolyline(view, position, obj->objectSavePath(), "0", color, "CONTINUOUS", "BYLAYER");
        break;
    }

    // TODO: proper layer/lineType/lineWeight
    case OBJ_TYPE_POINT: {
        toPolyline(view, position, obj->objectSavePath(), "0", color, "CONTINUOUS", "BYLAYER");
        break;
    }
    /* PATH? */

    case OBJ_TYPE_POLYGON: {
        toPolyline(view, obj->scenePos(), obj->objectSavePath(), "0", color, "CONTINUOUS", "BYLAYER");
        break;
    }

    case OBJ_TYPE_POLYLINE: {
        toPolyline(view, obj->scenePos(), obj->objectSavePath(), "0", color, "CONTINUOUS", "BYLAYER");
        break;
    }
    case OBJ_TYPE_RAY: {
        debug_message("TODO: save Ray object");
        break;
    }

    // TODO: proper layer/lineType/lineWeight
    case OBJ_TYPE_RECTANGLE: {
        toPolyline(view, obj->scenePos(), obj->objectSavePath(), "0", color, "CONTINUOUS", "BYLAYER");
        break;
    }

    case OBJ_TYPE_SPLINE: {
        debug_message("TODO: save Spline object");
        break;
    }
    case OBJ_TYPE_TEXTMULTI: {
        debug_message("TODO: save TextMulti object");
        break;
    }

    /* Add text single "item" to "pattern".
     *
     * TODO: saving polygons, polylines and paths must be stable before we go here.
     * TODO: This needs to work like a path, not a polyline. Improve this.
     * TODO: proper layer/lineType/lineWeight
     * TODO: Improve precision, replace simplified
     */
    case OBJ_TYPE_TEXTSINGLE: {
        std::vector<QPainterPath> pathList = obj->objectSavePathList();
        foreach(QPainterPath path, pathList) {
            toPolyline(view, position, path.simplified(), "0", color, "CONTINUOUS", "BYLAYER");
        }
        break;
    }

    default: {
        break;
    }
    }
}

void
saveObject(int objType, View *view, Geometry *obj)
{
    switch (objType) {
    case OBJ_TYPE_ARC: {
        debug_message("TODO: save Arc object");
        break;
    }
    case OBJ_TYPE_BLOCK: {
        debug_message("TODO: save Block object");
        break;
    }
    case OBJ_TYPE_CIRCLE: {
        embPattern_addCircleAbs(view->pattern, obj->gdata.circle);
        break;
    }
    case OBJ_TYPE_DIMALIGNED: {
        debug_message("TODO: save DimAligned object");
        break;
    }
    case OBJ_TYPE_DIMANGULAR: {
        debug_message("TODO: save DimAngular object");
        break;
    }
    case OBJ_TYPE_DIMARCLENGTH: {
        debug_message("TODO: save DimArcLength object");
        break;
    }
    case OBJ_TYPE_DIMDIAMETER: {
        debug_message("TODO: save DimDiameter object");
        break;
    }
    case OBJ_TYPE_DIMLEADER: {
        debug_message("TODO: save DimLeader object");
        break;
    }
    case OBJ_TYPE_DIMLINEAR: {
        debug_message("TODO: save DimLinear object");
        break;
    }
    case OBJ_TYPE_DIMORDINATE: {
        debug_message("TODO: save DimOrdinate object");
        break;
    }
    case OBJ_TYPE_DIMRADIUS: {
        debug_message("TODO: save DimRadius object");
        break;
    }
    case OBJ_TYPE_ELLIPSE: {
        embPattern_addEllipseAbs(view->pattern, obj->gdata.ellipse);
        break;
    }
    case OBJ_TYPE_ELLIPSEARC: {
        debug_message("TODO: save EllipseArc object");
        break;
    }
    case OBJ_TYPE_GRID: {
        debug_message("TODO: save Grid object");
        break;
    }
    case OBJ_TYPE_HATCH: {
        debug_message("TODO: save Hatch object");
        break;
    }
    case OBJ_TYPE_IMAGE: {
        debug_message("TODO: save Image object");
        break;
    }
    case OBJ_TYPE_INFINITELINE: {
        debug_message("TODO: save InfiniteLine object");
        break;
    }
    case OBJ_TYPE_LINE: {
        embPattern_addLineAbs(view->pattern, obj->gdata.line);
        break;
    }

    case OBJ_TYPE_POINT: {
        embPattern_addPointAbs(view->pattern, obj->gdata.point);
        break;
    }

    /* PATH? */

    case OBJ_TYPE_POLYGON: {
        debug_message("TODO: save Polygon object");
        break;
    }

    case OBJ_TYPE_POLYLINE: {
        debug_message("TODO: save Polyline object");
        break;
    }

    case OBJ_TYPE_RAY: {
        debug_message("TODO: save Ray object");
        break;
    }

    case OBJ_TYPE_RECTANGLE: {
        embPattern_addRectAbs(view->pattern, obj->gdata.rect);
        break;
    }

    case OBJ_TYPE_SPLINE: {
        debug_message("TODO: save Spline object");
        break;
    }

    case OBJ_TYPE_TEXTMULTI: {
        debug_message("TODO: save TextMulti object");
        break;
    }

    /* Add text single "item" to "pattern".
     *
     * TODO: saving polygons, polylines and paths must be stable before we go here.
     *
     * TODO: This needs to work like a path, not a polyline. Improve this.
     */
    case OBJ_TYPE_TEXTSINGLE: {
        debug_message("TODO: save TextMulti object as stitches");
        break;
    }

    default: {
        break;
    }
    }
}

/* Add path "item" to "pattern".
 *
 * TODO: Reimplement addPolyline() using the libembroidery C API
 */
void
addPath(View *view, Geometry *obj)
{
    QPainterPath path = obj->path();
    EmbVector start = to_EmbVector(obj->pos());

    for (int i = 0; i < path.elementCount()-1; ++i) {
        QPainterPath::Element element = path.elementAt(i);
        /*
        if (element.isMoveTo()) {
            embPattern_addStitchAbs(view->pattern, (element.x + start.x), -(element.y + start.y), TRIM);
        }
        else if (element.isLineTo()) {
            embPattern_addStitchAbs(view->pattern, (element.x + start.x), -(element.y + start.y), NORMAL);
        }
        else if (element.isCurveTo()) {
            QPainterPath::Element P1 = path.elementAt(i-1); // start point
            QPainterPath::Element P2 = path.elementAt(i);   // control point
            QPainterPath::Element P3 = path.elementAt(i+1); // control point
            QPainterPath::Element P4 = path.elementAt(i+2); // end point

            embPattern_addStitchAbs(P4.x, -P4.y, NORMAL); //TODO: This is temporary
            //TODO: Curved Polyline segments are always arcs
        }
        */
    }
    /*
    embPattern_addStitchRel(view->pattern, 0, 0, STOP);
    QColor c = obj->pen().color();
    embPattern_addThread(view->pattern, c.red(), c.green(), c.blue(), "", "");
    */
}

/* toPolyline
 *
 * NOTE: This function should be used to interpret various object types
 * and save them as polylines for stitchOnly formats.
 */
void
toPolyline(
    View* view,
    QPointF objPos,
    QPainterPath objPath,
    QString layer,
    QColor color,
    QString lineType,
    QString lineWeight)
{
    EmbArray* pointList = 0;
    QPainterPath::Element element;
    for (int i = 0; i < objPath.elementCount(); ++i) {
        element = objPath.elementAt(i);
        if (!pointList) {
            pointList = embArray_create(EMB_POINT);
        }
        EmbPoint po;
        po.position.x = element.x + objPos.x();
        po.position.y = -(element.y + objPos.y());
        embArray_addPoint(pointList, po);
    }

    EmbColor color_out;
    color_out.r = color.red();
    color_out.g = color.green();
    color_out.b = color.blue();

    /* TODO: FIX
    EmbPolyline* polyObject = embPolyline_init(pointList, color_out, 1); //TODO: proper lineType
    embPattern_addPolylineAbs(view->pattern, polyObject);
    */
}

/* Set object text. */
void Geometry::setObjectText(QString str)
{
    objText = str;
    QPainterPath textPath;
    QFont font;
    font.setFamily(objTextFont);
    font.setPointSizeF(text_size);
    font.setBold(flags & PROP_BOLD);
    font.setItalic(flags & PROP_ITALIC);
    font.setUnderline(flags & PROP_UNDERLINE);
    font.setStrikeOut(flags & PROP_STRIKEOUT);
    font.setOverline(flags & PROP_OVERLINE);
    textPath.addText(0, 0, font, str);

    //Translate the path based on the justification
    QRectF jRect = textPath.boundingRect();
    if (objTextJustify == "Left") {
        textPath.translate(-jRect.left(), 0);
    }
    else if (objTextJustify == "Center") {
        textPath.translate(-jRect.center().x(), 0);
    }
    else if (objTextJustify == "Right") {
        textPath.translate(-jRect.right(), 0);
    }
    else if (objTextJustify == "Aligned") {
        //TODO: Geometry Aligned Justification
    }
    else if (objTextJustify == "Middle") {
        textPath.translate(-jRect.center());
    }
    else if (objTextJustify == "Fit") {
        //TODO: Geometry Fit Justification
    }
    else if (objTextJustify == "Top Left") {
        textPath.translate(-jRect.topLeft());
    }
    else if (objTextJustify == "Top Center") {
        textPath.translate(-jRect.center().x(), -jRect.top());
    }
    else if (objTextJustify == "Top Right") {
        textPath.translate(-jRect.topRight());
    }
    else if (objTextJustify == "Middle Left") {
        textPath.translate(-jRect.left(), -jRect.top()/2.0);
    }
    else if (objTextJustify == "Middle Center") {
        textPath.translate(-jRect.center().x(), -jRect.top()/2.0);
    }
    else if (objTextJustify == "Middle Right") {
        textPath.translate(-jRect.right(), -jRect.top()/2.0);
    }
    else if (objTextJustify == "Bottom Left") {
        textPath.translate(-jRect.bottomLeft());
    }
    else if (objTextJustify == "Bottom Center") {
        textPath.translate(-jRect.center().x(), -jRect.bottom());
    }
    else if (objTextJustify == "Bottom Right") {
        textPath.translate(-jRect.bottomRight());
    }

    /* Backward or Upside Down */
    if (flags & (PROP_BACKWARD + PROP_UPSIDEDOWN)) {
        EmbReal horiz = 1.0;
        EmbReal vert = 1.0;
        if (flags & PROP_BACKWARD) {
            horiz = -1.0;
        }
        if (flags & PROP_UPSIDEDOWN) {
            vert = -1.0;
        }

        QPainterPath flippedPath;

        QPainterPath::Element element;
        QPainterPath::Element P2;
        QPainterPath::Element P3;
        QPainterPath::Element P4;
        for (int i = 0; i < textPath.elementCount(); ++i) {
            element = textPath.elementAt(i);
            if (element.isMoveTo()) {
                flippedPath.moveTo(horiz * element.x, vert * element.y);
            }
            else if (element.isLineTo()) {
                flippedPath.lineTo(horiz * element.x, vert * element.y);
            }
            else if (element.isCurveTo()) {
                                              // start point P1 is not needed
                P2 = textPath.elementAt(i);   // control point
                P3 = textPath.elementAt(i+1); // control point
                P4 = textPath.elementAt(i+2); // end point

                flippedPath.cubicTo(horiz * P2.x, vert * P2.y,
                                    horiz * P3.x, vert * P3.y,
                                    horiz * P4.x, vert * P4.y);
            }
        }
        objTextPath = flippedPath;
    }
    else {
        objTextPath = textPath;
    }

    //Add the grip point to the shape path
    QPainterPath gripPath = objTextPath;
    gripPath.connectPath(objTextPath);
    gripPath.addRect(-0.00000001, -0.00000001, 0.00000002, 0.00000002);
    setPath(gripPath);
}

/* font
 */
void
Geometry::setObjectTextFont(QString font)
{
    objTextFont = font;
    setObjectText(objText);
}

/* Set the text justification.
 * Verify the string is a valid option, otherwise default to "Left".
 */
void
Geometry::setObjectTextJustify(QString justify)
{
    objTextJustify = "Left";
    String justify_ = justify.toStdString();
    for (int i=0; strcmp(justify_options[i], "END"); i++) {
        if (!strcmp(justify_.c_str(), justify_options[i])) {
            objTextJustify = justify;
        }
    }
    setObjectText(objText);
}

/* size
 */
void
Geometry::setObjectTextSize(EmbReal size)
{
    text_size = size;
    setObjectText(objText);
}

/* bold, italic, under, strike, over */
void
Geometry::setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over)
{
    flags |= bold * PROP_BOLD;
    flags |= italic * PROP_ITALIC;
    flags |= under * PROP_UNDERLINE;
    flags |= strike * PROP_STRIKEOUT;
    flags |= over * PROP_OVERLINE;
    setObjectText(objText);
}

/* val
 */
void
Geometry::setObjectTextBold(bool val)
{
    flags |= val * PROP_BOLD;
    setObjectText(objText);
}

/* . */
void
Geometry::setObjectTextItalic(bool val)
{
    flags |= val * PROP_ITALIC;
    setObjectText(objText);
}

/* . */
void
Geometry::setObjectTextUnderline(bool val)
{
    flags |= val * PROP_UNDERLINE;
    setObjectText(objText);
}

/* . */
void
Geometry::setObjectTextStrikeOut(bool val)
{
    flags |= val * PROP_STRIKEOUT;
    setObjectText(objText);
}

/* . */
void
Geometry::setObjectTextOverline(bool val)
{
    flags |= val * PROP_OVERLINE;
    setObjectText(objText);
}

/* . */
void
Geometry::setObjectTextBackward(bool val)
{
    flags |= val * PROP_BACKWARD;
    setObjectText(objText);
}

/* . */
void
Geometry::setObjectTextUpsideDown(bool val)
{
    flags |= val * PROP_UPSIDEDOWN;
    setObjectText(objText);
}

/* Returns std::vector<QPainterPath>. */
std::vector<QPainterPath>
Geometry::subPathList()
{
    EmbReal s = scale();
    QTransform trans;
    trans.rotate(rotation());
    trans.scale(s,s);

    std::vector<QPainterPath> pathList;

    QPainterPath path = objTextPath;

    QPainterPath::Element element;
    std::vector<int> pathMoves;
    int numMoves = 0;

    for (int i = 0; i < path.elementCount(); i++) {
        element = path.elementAt(i);
        if (element.isMoveTo()) {
            pathMoves.push_back(i);
            numMoves++;
        }
    }

    pathMoves.push_back(path.elementCount());

    for (int p = 0; p < pathMoves.size()-1 && p < numMoves; p++) {
        QPainterPath subPath;
        for (int i = pathMoves[p]; i < pathMoves[p+1]; i++) {
            element = path.elementAt(i);
            if (element.isMoveTo()) {
                subPath.moveTo(element.x, element.y);
            }
            else if (element.isLineTo()) {
                subPath.lineTo(element.x, element.y);
            }
            else if (element.isCurveTo()) {
                subPath.cubicTo(path.elementAt(i  ).x, path.elementAt(i  ).y,  //control point 1
                                path.elementAt(i+1).x, path.elementAt(i+1).y,  //control point 2
                                path.elementAt(i+2).x, path.elementAt(i+2).y); //end point
            }
        }
        pathList.push_back(trans.map(subPath));
    }

    return pathList;
}

/* Run initialisation script for this object, based on the
 * object Type.
 */
void
Geometry::script_main(void)
{
    /*
    std::vector<std::string> script = {"end"};
    switch (mode) {

    case OBJ_TYPE_CIRCLE: {
        init();
        clear_selection;
        # FIX SELECTING CURRENT OBJECT;
        select this;
        set mode CIRCLE_MODE_1P_RAD;
        set-prompt-prefix-tr Specify center point for circle or [3P/2P/TTR (tan tan radius)]: "
        break;
    }

    case OBJ_TYPE_ELLIPSE: {
        script = scripts["ellipse_init"];
        break;
    }

    case DISTANCE: {
        script = {
            init();
            clear_selection();
            set-prompt-prefix-tr Specify first point: "
        };
        break;
    }

    case DOLPHIN: {
        init();
        clear_selection();
        set mode DOLPHIN_MODE_NUM_POINTS;
        # FIX SELECTING CURRENT OBJECT;
        select this;
        numPoints 512;
        minPoints 64;
        maxPoints 8192;
        center_x 0.0f;
        center_y 0.0f;
        scale_x 0.04f;
        scale_y 0.04f;
        add-rubber-selected POLYGON;
        set-rubber-mode POLYGON;
        update-dolphin numPoints scale_x scale_y;
        spare-rubber POLYGON;
        end"
        break;
    }

    ellipse_init_script = [
        init();
        clear_selection();
        # FIX SELECTING CURRENT OBJECT
        set mode ELLIPSE_MODE_MAJORDIAMETER_MINORRADIUS;
        height = 1.0;
        width = 2.0;
        rotation = 0.0;
        "set-prompt-prefix-tr Specify first axis start point or [Center]: "
    ]

    polyline_init_script = [
        init();
        clear_selection();
        # FIX SELECTING CURRENT OBJECT
        firstRun = true;
        first_x = 0.0f;
        first.y = 0.0f;
        prev_x = 0.0f;
        prev_y = 0.0f;
        num 0;
        set-prompt-prefix(tr("Specify first point: "));
        break;
    }

    snowflake_init_script = [
        init();
        clear_selection();
        numPoints = 2048;
        minPoitns = 64;
        "maxPoints = 8192;
        "center.x = 0.0f;
        "center.y = 0.0f;
        "scale.x = 0.04f;
        "scale.y = 0.04f;
        "mode = SNOWFLAKE_MODE_NUM_POINTS;
        "add-rubber POLYGON;
        "set-rubber-mode POLYGON;
        "update-snowflake;
        "spare-rubber POLYGON;
        "end"
    ]

    default: {
        break;
    }

    }
    */
}

/* Script to change the entries in the context menu when acting
 * on this object. Also, if one is activated carry out that
 * script.
 */
void
Geometry::script_context(std::string str)
{
    switch (mode) {

    /* . */
    case MODE_CIRCLE_1P_RAD: {
        actuator("todo CIRCLE context()");
        break;
    }

    /* . */
    default: {
        break;
    }

    }
}

/* Script to run on each click for this geometry object.
 *
 * The modes are documented in detail in the reference manual.
 */
void
Geometry::script_click(EmbVector v)
{
    switch (mode) {

/*
    case MODE_CIRCLE_1P_RAD: {
        auto iter = properties.find("point1");
        if (iter == properties.end()) {
            point1 = v;
            center = v;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_1P_RAD");
            setRubberPoint("CIRCLE_CENTER", center.x, center.y);
            actuator("append-prompt");
            set_prompt_prefix(tr("Specify radius of circle or [Diameter]: ");
        }
        else {
            point2 = v;
            setRubberPoint("CIRCLE_RADIUS", v);
            vulcanize();
            actuator("append-prompt");
            end();
        }
        break;
    }

    case MODE_CIRCLE_1P_DIA: {
        auto iter = properties.find("point1");
        if (iter == properties.end()) {
            error("CIRCLE", tr("This should never happen."));
        }
        else {
            point2.x = v.x;
            point2.y = v.y;
            setRubberPoint("CIRCLE_DIAMETER", point2.x, point2.y);
            vulcanize();
            append_prompt_history();
            end();
        }
        break;
    }

    case MODE_CIRCLE_2P: {
        auto iter1 = properties.find("point1");
        auto iter2 = properties.find("point2");
        if (iter1 == properties.end()) {
            point1 = node_vector(v);
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_2P");
            setRubberPoint("CIRCLE_TAN1", v);
            append_prompt_history();
            set_prompt_prefix(tr("Specify second end point of circle's diameter: ");
        }
        else if (iter2 == properties.end()) {
            point2 = node_vector(v);
            setRubberPoint("CIRCLE_TAN2", v);
            vulcanize();
            append_prompt_history();
            end();
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
        break;
    }

    case CIRCLE_MODE_3P: {
        if (std::isnan(point1.x)) {
            point1.x = x;
            point1.y = y;
            append_prompt_history();
            set_prompt_prefix(tr("Specify second point on circle: ");
        }
        else if (std::isnan(point2.x)) {
            point2.x = x;
            point2.y = y;
            addRubber("CIRCLE");
            setRubberMode("CIRCLE_3P");
            setRubberPoint("CIRCLE_TAN1", point1.x, point1.y);
            setRubberPoint("CIRCLE_TAN2", point2.x, point2.y);
            run_script({
                append-prompt-history;
                set-prompt-prefix-tr Specify third point on circle: "
            });
        }
        else if (std::isnan(point3.x)) {
            point3.x = x;
            point3.y = y;
            setRubberPoint("CIRCLE_TAN3", point3.x, point3.y);
            run_script({
                vulcanize;
                append-prompt-history;
                end"
            });
        }
    }

    case CIRCLE_MODE_TTR: {
        point1.x = x;
        point1.y = y;
        append_prompt_history();
        set_prompt_prefix(tr("Specify point on object for second tangent of circle: ");
        mode = MODE_TTR_SET_POINT_2;
        break;
    }

    case CIRCLE_MODE_TTR_SET_POINT_2: {
        point2.x = x;
        point2.y = y;
        append_prompt_history();
        set_prompt_prefix(tr("Specify radius of circle: ");
        mode = MODE_TTR_SET_POINT_3;
        break;
    }

    case CIRCLE_MODE_TTR_SET_POINT_3: {
        point3.x = x;
        point3.y = y;
        append_prompt_history();
        set_prompt_prefix(tr("Specify second point: ");
        mode = MODE_CIRCLE_DEFAULT;
        break;
    }

    default: {
        error("CIRCLE", tr("This should never happen."));
        break;
    }
*/

    }
}

/* Script to control the behavior of the prompt when processing
 * events for this object.
 */
void
Geometry::script_prompt(std::string str)
{
    switch (mode) {

/*
    case MODE_CIRCLE_1P_RAD: {
        if (std::isnan(point1.x)) {
            if (str == "2P") {
                mode = MODE_CIRCLE_2P;
                set_prompt_prefix(tr("Specify first end point of circle's diameter: ");
            }
            else if (str == "3P") {
                mode = MODE_CIRCLE_3P;
                set_prompt_prefix(tr("Specify first point of circle: ");
            }
            else if (str == "T" || str == "TTR") {
                mode = MODE_CIRCLE_TTR;
                set_prompt_prefix(tr("Specify point on object for first tangent of circle: ");
            }
            else {
                EmbReal strList = str.split(",");
                if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                    alert(tr("Point or option keyword required."));
                    set_prompt_prefix(tr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: ");
                }
                else {
                    x1 = std::stof(strList[0]);
                    point1.y = atof(strList[1]);
                    center.x = point1.x;
                    center.y = point1.y;
                    addRubber("CIRCLE");
                    setRubberMode("CIRCLE_1P_RAD");
                    setRubberPoint("CIRCLE_CENTER", center.x, center.y);
                    set_prompt_prefix(tr("Specify radius of circle or [Diameter]: ");
                }
            }
        }
        else {
            if (str == "D" || str == "DIAMETER") {
                mode = MODE_CIRCLE_1P_DIA;
                setRubberMode("CIRCLE_1P_DIA");
                set_prompt_prefix(tr("Specify diameter of circle: ");
            }
            else {
                EmbReal num = atof(str);
                if (std::isnan(num)) {
                    alert(tr("Requires numeric radius, point on circumference, or \"D\"."));
                    set_prompt_prefix(tr("Specify radius of circle or [Diameter]: "));
                }
                else {
                    radius = num;
                    point2.x = point1.x + radius;
                    point2.y = point1.y;
                    setRubberPoint("CIRCLE_RADIUS", point2.x, point2.y);
                    vulcanize();
                    end();
                }
            }
        }
    }

    case MODE_CIRCLE_1P_DIA: {
        if (std::isnan(point1.x)) {
            error("CIRCLE", tr("This should never happen."));
        }
        if (std::isnan(point2.x)) {
            EmbReal num = atof(str);
            if (std::isnan(num)) {
                alert(tr("Requires numeric distance or second point."));
                set_prompt_prefix(tr("Specify diameter of circle: "));
            }
            else {
                diameter = num;
                point2.x = point1.x + diameter.r;
                point2.y = point1.y;
                setRubberPoint("CIRCLE_DIAMETER", point2.x, point2.y);
                vulcanize();
                end();
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }

    case MODE_CIRCLE_2P: {
        if (std::isnan(point1.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                set_prompt_prefix(tr("Specify first end point of circle's diameter: ");
            }
            else {
                x1 = atof(strList[0]);
                y1 = atof(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_2P");
                setRubberPoint("CIRCLE_TAN1", point1.x, point1.y);
                set_prompt_prefix(tr("Specify second end point of circle's diameter: ");
            }
        }
        else if (std::isnan(point2.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                set_prompt_prefix(tr("Specify second end point of circle's diameter: ");
            }
            else {
                x2 = atof(strList[0]);
                y2 = atof(strList[1]);
                setRubberPoint("CIRCLE_TAN2", point2.x, point2.y);
                vulcanize();
                end();
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }

    case MODE_CIRCLE_3P: {
        if (std::isnan(point1.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                set_prompt_prefix(tr("Specify first point of circle: ");
            }
            else {
                point1.x = atof(strList[0]);
                point1.y = atof(strList[1]);
                set_prompt_prefix(tr("Specify second point of circle: ");
            }
        }
        else if (std::isnan(point2.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                set_prompt_prefix(tr("Specify second point of circle: ");
            }
            else {
                point2.x = atof(strList[0]);
                point2.y = atof(strList[1]);
                addRubber("CIRCLE");
                setRubberMode("CIRCLE_3P");
                setRubberPoint("CIRCLE_TAN1", point1.x, point1.y);
                setRubberPoint("CIRCLE_TAN2", point2.x, point2.y);
                set_prompt_prefix(tr("Specify third point of circle: ");
            }
        }
        else if (std::isnan(point3.x)) {
            EmbReal strList = str.split(",");
            if (std::isnan(strList[0]) || std::isnan(strList[1])) {
                alert(tr("Invalid point."));
                set_prompt_prefix(tr("Specify third point of circle: ");
            }
            else {
                point3.x = atof(strList[0]);
                point3.y = atof(strList[1]);
                setRubberPoint("CIRCLE_TAN3", point3.x, point3.y);
                vulcanize();
                end();
            }
        }
        else {
            error("CIRCLE", tr("This should never happen."));
        }
    }

    case MODE_CIRCLE_TTR: {
        actuator("todo CIRCLE prompt() for TTR");
        break;
    }

    default: {
        break;
    }
*/
    }
}
