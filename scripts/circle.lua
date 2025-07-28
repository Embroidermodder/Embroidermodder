-- Command: Circle

circle_global = {
  x1 = nil,
  y1 = nil,
  x2 = nil,
  y2 = nil,
  x3 = nil,
  y3 = nil,
  rad = nil,
  dia = nil,
  cx = nil,
  cy = nil,
  mode = nil
}

-- enums
CIRCLE_MODE_1P_RAD = 0
CIRCLE_MODE_1P_DIA = 1
CIRCLE_MODE_2P = 2
CIRCLE_MODE_3P = 3
CIRCLE_MODE_TTR = 4

-- NOTE: main() is run every time the command is started.
--       Use it to reset variables so they are ready to go.
function circle_main()
  init_command()
  clear_selection()
  circle_global.mode = CIRCLE_MODE_1P_RAD
  circle_global.x1 = nil
  circle_global.y1 = nil
  circle_global.x2 = nil
  circle_global.y2 = nil
  circle_global.x3 = nil
  circle_global.y3 = nil
  set_prompt_prefix(qsTr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "))
end

-- NOTE: click() is run only for left clicks.
--       Middle clicks are used for panning.
--       Right clicks bring up the context menu.
function click(x, y)
  if circle_global.mode == CIRCLE_MODE_1P_RAD then

    if circle_global.x1 == nil then
      global.x1 = x
      global.y1 = y
      global.cx = x
      global.cy = y
      add_rubber("CIRCLE")
      set_rubber_mode("CIRCLE_1P_RAD")
      set_rubber_point("CIRCLE_CENTER", circle_global.cx, circle_global.cy)
      append_prompt_history()
      set_prompt_prefix(qsTr("Specify radius of circle or [Diameter]: "))
    else
      circle_global.x2 = x
      circle_global.y2 = y
      setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2)
      vulcanize()
      appendPromptHistory()
      endCommand()
    end

  elseif circle_global.mode == CIRCLE_MODE_1P_DIA then

    if circle_global.x1 == nil then
      error("CIRCLE", qsTr("This should never happen."))
    else
      global.x2 = x
      global.y2 = y
      setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2)
      vulcanize()
      appendPromptHistory()
      endCommand()
    end

  elseif global.mode == CIRCLE_MODE_2P then

    if circle_global.x1 == nil then
      global.x1 = x
      global.y1 = y
      addRubber("CIRCLE")
      setRubberMode("CIRCLE_2P")
      setRubberPoint("CIRCLE_TAN1", global.x1, global.y1)
      appendPromptHistory()
      setPromptPrefix(qsTr("Specify second end point of circle's diameter: "))
    elseif global.x2 == nil then
      global.x2 = x
      global.y2 = y
      setRubberPoint("CIRCLE_TAN2", global.x2, global.y2)
      vulcanize()
      appendPromptHistory()
      endCommand()
    else
      error("CIRCLE", qsTr("This should never happen."))
    end

  else if(global.mode == global.mode_3P)
    {
        if(isNaN(global.x1))
        {
            global.x1 = x
            global.y1 = y
            appendPromptHistory()
            setPromptPrefix(qsTr("Specify second point on circle: "))
        }
        else if(isNaN(global.x2))
        {
            global.x2 = x
            global.y2 = y
            addRubber("CIRCLE")
            setRubberMode("CIRCLE_3P")
            setRubberPoint("CIRCLE_TAN1", global.x1, global.y1)
            setRubberPoint("CIRCLE_TAN2", global.x2, global.y2)
            appendPromptHistory()
            setPromptPrefix(qsTr("Specify third point on circle: "))
        }
        else if(isNaN(global.x3))
        {
            global.x3 = x
            global.y3 = y
            setRubberPoint("CIRCLE_TAN3", global.x3, global.y3)
            vulcanize()
            appendPromptHistory()
            endCommand()
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."))
        }
    }
    else if(global.mode == global.mode_TTR)
    {
        if(isNaN(global.x1))
        {
            global.x1 = x
            global.y1 = y
            appendPromptHistory()
            setPromptPrefix(qsTr("Specify point on object for second tangent of circle: "))
        }
        else if(isNaN(global.x2))
        {
            global.x2 = x
            global.y2 = y
            appendPromptHistory()
            setPromptPrefix(qsTr("Specify radius of circle: "))
        }
        else if(isNaN(global.x3))
        {
            global.x3 = x
            global.y3 = y
            appendPromptHistory()
            setPromptPrefix(qsTr("Specify second point: "))
        }
        else
        {
            todo("CIRCLE", "click() for TTR")
        }
    }
end

-- NOTE: context() is run when a context menu entry is chosen.
function circle_context (str)
  todo("CIRCLE", "context()")
end

-- NOTE: prompt() is run when Enter is pressed.
--       appendPromptHistory is automatically called before prompt()
--       is called so calling it is only needed for erroneous input.
--       Any text in the command prompt is sent as an uppercase string.
function circle_prompt (str)
    if(global.mode == global.mode_1P_RAD)
    {
        if(isNaN(global.x1))
        {
            if(str == "2P") -- TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_2P
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "))
            }
            else if(str == "3P") -- TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_3P
                setPromptPrefix(qsTr("Specify first point of circle: "))
            }
            else if(str == "T" || str == "TTR") -- TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_TTR
                setPromptPrefix(qsTr("Specify point on object for first tangent of circle: "))
            }
            else
            {
                var strList = str.split(",")
                if(isNaN(strList[0]) || isNaN(strList[1]))
                {
                    alert(qsTr("Point or option keyword required."))
                    setPromptPrefix(qsTr("Specify center point for circle or [3P/2P/Ttr (tan tan radius)]: "))
                }
                else
                {
                    global.x1 = Number(strList[0])
                    global.y1 = Number(strList[1])
                    global.cx = global.x1
                    global.cy = global.y1
                    addRubber("CIRCLE")
                    setRubberMode("CIRCLE_1P_RAD")
                    setRubberPoint("CIRCLE_CENTER", global.cx, global.cy)
                    setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "))
                }
            }
        }
        else
        {
            if(str == "D" || str == "DIAMETER") -- TODO: Probably should add additional qsTr calls here.
            {
                global.mode = global.mode_1P_DIA
                setRubberMode("CIRCLE_1P_DIA")
                setPromptPrefix(qsTr("Specify diameter of circle: "))
            }
            else
            {
                var num = Number(str)
                if(isNaN(num))
                {
                    alert(qsTr("Requires numeric radius, point on circumference, or \"D\"."))
                    setPromptPrefix(qsTr("Specify radius of circle or [Diameter]: "))
                }
                else
                {
                    global.rad = num
                    global.x2 = global.x1 + global.rad
                    global.y2 = global.y1
                    setRubberPoint("CIRCLE_RADIUS", global.x2, global.y2)
                    vulcanize()
                    endCommand()
                }
            }
        }
    }
    else if(global.mode == global.mode_1P_DIA)
    {
        if(isNaN(global.x1))
        {
            error("CIRCLE", qsTr("This should never happen."))
        }
        if(isNaN(global.x2))
        {
            var num = Number(str)
            if(isNaN(num))
            {
                alert(qsTr("Requires numeric distance or second point."))
                setPromptPrefix(qsTr("Specify diameter of circle: "))
            }
            else
            {
                global.dia = num
                global.x2 = global.x1 + global.dia
                global.y2 = global.y1
                setRubberPoint("CIRCLE_DIAMETER", global.x2, global.y2)
                vulcanize()
                endCommand()
            }
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."))
        }
    }
    else if(global.mode == global.mode_2P)
    {
        if(isNaN(global.x1))
        {
            var strList = str.split(",")
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."))
                setPromptPrefix(qsTr("Specify first end point of circle's diameter: "))
            }
            else
            {
                global.x1 = Number(strList[0])
                global.y1 = Number(strList[1])
                addRubber("CIRCLE")
                setRubberMode("CIRCLE_2P")
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1)
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "))
            }
        }
        else if(isNaN(global.x2))
        {
            var strList = str.split(",")
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."))
                setPromptPrefix(qsTr("Specify second end point of circle's diameter: "))
            }
            else
            {
                global.x2 = Number(strList[0])
                global.y2 = Number(strList[1])
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2)
                vulcanize()
                endCommand()
            }
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."))
        }
    }
    else if(global.mode == global.mode_3P)
    {
        if(isNaN(global.x1))
        {
            var strList = str.split(",")
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."))
                setPromptPrefix(qsTr("Specify first point of circle: "))
            }
            else
            {
                global.x1 = Number(strList[0])
                global.y1 = Number(strList[1])
                setPromptPrefix(qsTr("Specify second point of circle: "))
            }
        }
        else if(isNaN(global.x2))
        {
            var strList = str.split(",")
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."))
                setPromptPrefix(qsTr("Specify second point of circle: "))
            }
            else
            {
                global.x2 = Number(strList[0])
                global.y2 = Number(strList[1])
                addRubber("CIRCLE")
                setRubberMode("CIRCLE_3P")
                setRubberPoint("CIRCLE_TAN1", global.x1, global.y1)
                setRubberPoint("CIRCLE_TAN2", global.x2, global.y2)
                setPromptPrefix(qsTr("Specify third point of circle: "))
            }
        }
        else if(isNaN(global.x3))
        {
            var strList = str.split(",")
            if(isNaN(strList[0]) || isNaN(strList[1]))
            {
                alert(qsTr("Invalid point."))
                setPromptPrefix(qsTr("Specify third point of circle: "))
            }
            else
            {                
                global.x3 = Number(strList[0])
                global.y3 = Number(strList[1])
                setRubberPoint("CIRCLE_TAN3", global.x3, global.y3)
                vulcanize()
                endCommand()
            }
        }
        else
        {
            error("CIRCLE", qsTr("This should never happen."))
        }
        
    }
    else if(global.mode == global.mode_TTR)
    {
        todo("CIRCLE", "prompt() for TTR")
    }
end

--[[
[Menu]
Name=Draw
Position=8

[ToolBar]
Name=Draw
Position=8

[Tips]
ToolTip=Circle
StatusTip=Creates a circle:  CIRCLE

[Prompt]
Alias=C, CIRCLE
--]]
