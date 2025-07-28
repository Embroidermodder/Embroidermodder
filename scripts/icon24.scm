; Command: Icon24

(define (icon24-main)
  ; NOTE: main() is run every time the command is started.
  ;       Use it to reset variables so they are ready to go.
  (if (= context context-main)
    (begin
      (init-command)
      (clear-selection)
      (icon24)
      (end-command))
    (begin
      (icon24)
      (end-command))))

;menu-name=None
;menu-position=0
;
;toolbar-name=None
;toolbar-position=0
;
;ToolTip=Icon&24
;StatusTip=Sets the toolbar icon size to 24x24:  ICON24
;
;Alias=ICON24

