; Command: Icon16
;

(define (icon16-main)
  ; NOTE: main() is run every time the command is started.
  ;       Use it to reset variables so they are ready to go.
  (if (= context context-main)
    (begin
      (init-command)
      (clear-selection)
      (icon16)
      (end-command))
    (begin
      (icon16)
      (end-command))))

; menu-name=None
; menu-position=0
;
; toolbar-name=None
; toolbar-position=0
;
; ToolTip=Icon&16
; StatusTip=Sets the toolbar icon size to 16x16:  ICON16
;
; alias=ICON16
