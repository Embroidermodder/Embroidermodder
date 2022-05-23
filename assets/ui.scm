; Embroidermodder 2.
; Copyright 2013-2022 The Embroidermodder Team
; Distributed under the terms of the zlib license.
; https://www.libembroidery.org
; ------------------------------------------------------------
; Generate the user interface.
;
; Statusbar
;    make_rectangle(&rect, 0, 455, 640, 25);;
;     NULL, "interface_color",
;    create_widget(rect, "do_nothing");
;
; visibility groups
; "always"
; "file-toolbar"
; "edit-toolbar"
; "window-toolbar"
; 
; "never" (use only for debugging)
;
; ui palette
; interface-background
; button-hover
; 

; UI CONSTANTS
; ------------
(define (file-menu) 0)
(define (edit-menu) 1)
(define (view-menu) 2)
(define (settings-menu) 3)
(define (window-menu) 4)
(define (zoom-menu) 5)
(define (pan-menu) 6)
(define (recent-menu) 7)
(define (help-menu) 8)

; UI SETTINGS
; -----------
; These may change causing us to need to reload the
; file.
(define (icon-size) 24)
(define (window-width) 640)
(define (window-height) 480)
(define (menubar-height) 24)
(define (menubar-padding) 2)
(define (toolbar-padding) 4)
(define (toolbar-width) 632)
(define (icon-padding) 4)
(define (statusbar-message) "Embroidermodder 2.0.0-alpha")

(define (file-toolbar-visible) 1)
(define (edit-toolbar-visible) 1)
(define (view-toolbar-visible) 1)
(define (window-toolbar-visible) 1)

(define (menu-state) (edit-menu))


(define (toolbar-offset) (+ (menubar-height) (menubar-padding)))
(define (toolbar-height) (+ (icon-size) (toolbar-padding)))

(define (background-rect x y w h)
  (create-ui-rect x y w h 100 150 210))

(create-ui-rect 0 0 (window-width) (window-height) 50 50 50)

; Toolbar background
(create-ui-rect
  0 0
  640 (+ (* 3 (+ (toolbar-padding) (toolbar-height)))
    (toolbar-offset))
  240 240 250)

; Menubar
(create-ui-rect
  0 0
  (window-width) (menubar-height)
  200 200 250)

; Toolbars
(background-rect
  (toolbar-padding) (toolbar-offset)
  (toolbar-width) (toolbar-height))

(background-rect
  (toolbar-padding)
  (+ (toolbar-padding) (toolbar-height) (toolbar-offset))
  (toolbar-width) (toolbar-height))

(background-rect
  (toolbar-padding)
  (+ (* 2 (+ (toolbar-padding) (toolbar-height)))
    (toolbar-offset))
  (toolbar-width) (toolbar-height))

(background-rect
  (toolbar-padding)
  (+ (* 3 (+ (toolbar-padding) (toolbar-height)))
    (toolbar-offset))
  (toolbar-width) (toolbar-height))

(define (horizontal-rule x y w)
  (create-ui-rect x y w 1 0 0 0))
(define (vertical-rule x y h)
  (create-ui-rect x y 1 h 0 0 0))

(horizontal-rule 10 170 350)
(horizontal-rule 10 240 350)
(horizontal-rule 10 310 350)
(horizontal-rule 10 380 350)
(horizontal-rule 10 450 350)
(vertical-rule 10 150 300)
(vertical-rule 80 150 300)
(vertical-rule 150 150 300)
(vertical-rule 220 150 300)
(vertical-rule 290 150 300)
(vertical-rule 360 150 300)

(create-icon 1 1 "new")
(create-icon 2 1 "open")
(create-icon 3 1 "save")
(create-icon 4 1 "save-as")

(create-icon 5 1 "cut")
(create-icon 6 1 "copy")
(create-icon 7 1 "paste")
(create-icon 8 1 "delete")

(create-icon 9 1 "undo")
(create-icon 10 1 "redo")
(create-icon 11 1 "check-for-updates")
(create-icon 12 1 "select-all")
(create-icon 13 1 "whats-this")
(create-icon 14 1 "design-details")
(create-icon 15 1 "print-pattern")
(create-icon 16 1 "exit-program")

(create-icon 17 1 "window-close")
(create-icon 18 1 "window-close-all")
(create-icon 19 1 "window-cascade")
(create-icon 20 1 "window-tile")
(create-icon 21 1 "window-next")
(create-icon 22 1 "window-previous")

(create-icon 1 2 "help")
(create-icon 2 2 "changelog-dialog")
(create-icon 3 2 "tip-of-the-day-dialog")
(create-icon 4 2 "about-dialog")

(create-icon 5 2 "icon-16")
(create-icon 6 2 "icon-24")
(create-icon 7 2 "icon-32")
(create-icon 8 2 "icon-48")
(create-icon 9 2 "icon-64")
(create-icon 10 2 "icon-128")

(create-icon 11 2 "settings-dialog")
(create-icon 12 2 "make-layer-current")

(create-icon 13 2 "pan-real-time")
(create-icon 14 2 "pan-point")
(create-icon 15 2 "pan-left")
(create-icon 16 2 "pan-right")
(create-icon 17 2 "pan-up")
(create-icon 18 2 "pan-down")

(create-icon 19 2 "day")
(create-icon 20 2 "night")

(create-icon 1 3 "layers")
(create-icon 2 3 "layer-selector")
(create-icon 3 3 "layer-previous")
(create-icon 4 3 "color-selector")
(create-icon 5 3 "line-type-selector")
(create-icon 6 3 "line-weight-selector")
(create-icon 7 3 "hide-all-layers")
(create-icon 8 3 "show-all-layers")
(create-icon 9 3 "freeze-all-layers")
(create-icon 10 3 "thaw-all-layers")
(create-icon 11 3 "lock-all-layers")
(create-icon 12 3 "unlock-all-layers")

(create-icon 13 3 "text-bold")
(create-icon 14 3 "text-italic")
(create-icon 15 3 "text-underline")
(create-icon 16 3 "text-strikeout")
(create-icon 17 3 "text-overline")

(create-icon 18 3 "zoom-real-time")
(create-icon 19 3 "zoom-previous")
(create-icon 20 3 "zoom-window")
(create-icon 21 3 "zoom-dynamic")
(create-icon 22 3 "zoom-scale")
(create-icon 23 3 "zoom-center")
(create-icon 24 3 "zoom-in")
(create-icon 25 3 "zoom-out")
(create-icon 26 3 "zoom-selected")
(create-icon 27 3 "zoom-all")
(create-icon 28 3 "zoom-extents")

(create-icon 1 4 "treble-clef")
(create-icon 2 4 "path")
(create-icon 3 4 "circle")
(create-icon 4 4 "line")
(create-icon 5 4 "distance")
(create-icon 6 4 "dolphin")
(create-icon 7 4 "ellipse")

(create-icon 8 4 "heart")
(create-icon 9 4 "locate-point")
(create-icon 10 4 "move")
;(create-icon 11 4 "export")
;(create-icon 12 4 "heart4")
;(create-icon 13 4 "heart5")
;(create-icon 14 4 "single-line-text")
;(create-icon 15 4 "spell-check"
;(create-icon 16 4 "quick-select")
;(create-icon 17 4 "rectangle")
;(create-icon 18 4 "rgb")
;(create-icon 19 4 "rotate")
;(create-icon 20 4 "sandbox")
;(create-icon 21 4 "quickleader")
;(create-icon 22 4 "snowflake")
;(create-icon 23 4 "star")
;(create-icon 24 4 "platform")
;(create-icon 25 4 "point")
;(create-icon 26 4 "polygon")
;(create-icon 27 4 "polyline")
;(create-icon 28 4 "settings-dialog")
;(create-icon 29 4 "quickleader")
;(create-icon 30 4 "locate-point")
;(create-icon 31 4 "point")



; ------------------------------------------------------------
;
; Menus

(define (menu-item-height) 20)
(define (menu-width) 100)
(define (char-width) 8)

; FILE MENU
; ---------
(define (x-offset) 10)
(create-label (x-offset) 5 "File" "file-menu")

(create-label (x-offset) (+ 5 (* (menu-item-height) 1))
    "New" "new")
(create-label (x-offset) (+ 5 (* (menu-item-height) 2))
    "Open" "open")
(create-label (x-offset) (+ 5 (* (menu-item-height) 3))
    "Save" "save")
(create-label (x-offset) (+ 5 (* (menu-item-height) 4))
    "Save as..." "save-as")
(create-label (x-offset) (+ 5 (* (menu-item-height) 5))
    "Export" "export")
(create-label (x-offset) (+ 5 (* (menu-item-height) 6))
    "Print" "print")
(create-label (x-offset) (+ 5 (* (menu-item-height) 7))
    "Design Details" "design-details")
(create-label (x-offset) (+ 5 (* (menu-item-height) 8))
    "Exit" "exit-program")

(set-visibility "New" 0)
(set-visibility "Open" 0)
(set-visibility "Save" 0)
(set-visibility "Save as..." 0)
(set-visibility "Export" 0)
(set-visibility "Print" 0)
(set-visibility "Design Details" 0)
(set-visibility "Exit" 0)

(if (= (menu-state) (file-menu))
    (begin
      (set-visibility "New" 1)
      ; (horizontal-rule )
      (set-visibility "Open" 1)
      ; (horizontal-rule )
      (set-visibility "Save" 1)
      (set-visibility "Save as..." 1)
      (set-visibility "Export" 1)
      ; (horizontal-rule )
      (set-visibility "Print" 1)
      ; (horizontal-rule )
      (set-visibility "Design Details" 1)
      ; (horizontal-rule )
      (set-visibility "Exit" 1)))

; EDIT MENU
; ---------
(define (x-offset) (+ 20 (* 4 (char-width))))

(create-label (x-offset) 5 "Edit" "edit-menu")

(create-ui-rect (x-offset) 20 (menu-width)
    (* (menu-item-height) 5)
    255 255 255)

(create-label (x-offset) (+ 5 (* (menu-item-height) 1))
    "Undo" "undo")
(create-label (x-offset) (+ 5 (* (menu-item-height) 2))
    "Redo" "redo")
(create-label (x-offset) (+ 5 (* (menu-item-height) 3))
    "Cut" "cut")
(create-label (x-offset) (+ 5 (* (menu-item-height) 4))
    "Copy" "copy")
(create-label (x-offset) (+ 5 (* (menu-item-height) 5))
    "Paste" "paste")

(set-visibility "Undo" 0)
(set-visibility "Redo" 0)
(set-visibility "Cut" 0)
(set-visibility "Copy" 0)
(set-visibility "Paste" 0)

(if (= (menu-state) (edit-menu))
    (begin
      (set-visibility "Undo" 1)
      (set-visibility "Redo" 1)
      (set-visibility "Cut" 1)
      (set-visibility "Copy" 1)
      (set-visibility "Paste" 1)
      (horizontal-rule (x-offset) (+ 5 (* (menu-item-height) 3)) (menu-width))
      (horizontal-rule (x-offset) (+ 5 (* (menu-item-height) 6)) (menu-width))
      (vertical-rule (x-offset) 20
        (* (menu-item-height) 5))
      (vertical-rule (+ (x-offset) (menu-width)) 20
        (* (menu-item-height) 5))))

; VIEW MENU
; ---------
(define (x-offset) (+ 30 (* 8 (char-width))))
(create-label (x-offset) 5 "View" "view-menu")

(if (= (menu-state) (view-menu))
  (begin
    (create-ui-rect (x-offset) 20 50 100 255 255 255)
    (create-label (x-offset) 20 "Day")
    (create-label (x-offset) 35 "Night")
    ; (horizontal-rule )
    (create-label (x-offset) 50 "Icon 16")
    (create-label (x-offset) 65 "Icon 24")
    (create-label (x-offset) 80 "Icon 32")
    (create-label (x-offset) 95 "Icon 48")
    (create-label (x-offset) 110 "Icon 64")
    (create-label (x-offset) 135 "Icon 128")
    (horizontal-rule (x-offset) (+ 5 (* (menu-item-height) 3)) (menu-width))
    (horizontal-rule (x-offset) (+ 5 (* (menu-item-height) 8)) (menu-width))
    (vertical-rule (x-offset) 5 200)
    (vertical-rule 110 5 200)))

; SETTINGS MENU
; -------------
(define (x-offset) (+ 40 (* 12 (char-width))))
(create-label (x-offset) 5 "Settings" "settings-menu")

(if (= (menu-state) (settings-menu))
  (begin
    (create-ui-rect 160 20 50 100 255 255 255)
    (create-label 160 20
      "Settings Dialog" "settings-dialog")))

; WINDOW MENU
; -----------
(define (x-offset) (+ 50 (* 20 (char-width))))
(create-label (x-offset) 5 "Window")

(if (= (menu-state) (window-menu))
  (begin
    (create-ui-rect 210 20 50 100 255 255 255)
    (create-label 210 20 "Window Cascade")))

; HELP MENU
; ---------
(define (x-offset) (+ 60 (* 26 (char-width))))
(create-label (x-offset) 5 "Help")

(if (= (menu-state) (help-menu))
  (begin
    (create-ui-rect (x-offset) 20 50 100 255 255 255)
    (create-label (x-offset) 20 "Help")
    ; (horizontal-rule )
    (create-label (x-offset) 35 "Changelog")
    ; (horizontal-rule )
    (create-label (x-offset) 50 "Tip of the Day")
    ; (horizontal-rule )
    (create-label (x-offset) 65 "About")
    ; (horizontal-rule )
    (create-label (x-offset) 80 "What's This?")
    ; (horizontal-rule )
    (vertical-rule (x-offset) 5 200)
    (vertical-rule 110 5 200)))

; RECENT MENU
; -----------
(define (x-offset) (+ 70 (* 30 (char-width))))
(create-label (x-offset) 5 "Recent")

(if (= (menu-state) (recent-menu))
  (begin
    (create-ui-rect (x-offset) 20 50 100 255 255 255)))

; ZOOM MENU
; ---------
(define (x-offset) (+ 80 (* 36 (char-width))))
(create-label (x-offset) 5 "Zoom")

(if (= (menu-state) (zoom-menu))
  (begin
    (create-ui-rect (x-offset) 20 50 100 255 255 255)
    (create-label (x-offset) 20 "Zoom Real Time")
    (create-label (x-offset) 20 "Zoom Previous")
    ; (horizontal-rule )
    (create-label (x-offset) 35 "Zoom Window")
    (create-label (x-offset) 35 "Zoom Dynamic")
    (create-label (x-offset) 35 "Zoom Scale")
    (create-label (x-offset) 35 "Zoom Center")
    ; (horizontal-rule )
    (create-label (x-offset) 50 "Zoom In")
    (create-label (x-offset) 50 "Zoom Out")
    ; (horizontal-rule )
    (create-label (x-offset) 65 "Zoom Selected")
    (create-label (x-offset) 65 "Zoom All")
    (create-label (x-offset) 65 "Zoom Extents")
    ; (horizontal-rule )
    (vertical-rule (x-offset) 5 200)
    (vertical-rule 110 5 200)))

; PAN MENU
; --------
(define (x-offset) (+ 90 (* 40 (char-width))))
(create-label (x-offset) 5 "Pan")

(if (= (menu-state) (pan-menu))
  (begin
    (create-ui-rect (x-offset) 20 50 100 255 255 255)
    (create-label (x-offset) 20 "Pan Real Time")
    (create-label (x-offset) 20 "Pan Point")
    ; (horizontal-rule )
    (create-label (x-offset) 35 "Pan Left")
    (create-label (x-offset) 35 "Pan Right")
    (create-label (x-offset) 35 "Pan Up")
    (create-label (x-offset) 35 "Pan Down")
    ; (horizontal-rule )
    (vertical-rule (x-offset) 5 200)
    (vertical-rule 110 5 200)))


; Statusbar
(create-ui-rect
  0 (- (window-height) (menubar-height))
  (window-width) (menubar-height)
  100 100 100)
(create-label
  10 (- (window-height) (menubar-height))
  (statusbar-message))

; ------------------------------------------------------------
; Crash test all basic actions.

(define (crash-test)
  (begin
    (do_nothing)

    (debug-message "File actions")
    (new-file)
    ; Set file name procedurally
    (open-file)
    (save-file)
    (save-as-file)

    (debug-message "Edit actions")
    ; Place some objects to act on.
    (cut)
    (copy)
    (paste)

    ; Crash test set icon size to x calls.
    (icon16)
    (icon24)
    (icon32)
    (icon48)
    (icon64)
    (icon128)))
