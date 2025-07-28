; This file is part of Embroidermodder 2.
;
; NOTE: main() is run every time the command is started.
;       Use it to reset variables so they are ready to go.
(define context-main 0)

; NOTE: click() is run only for left clicks.
;       Middle clicks are used for panning.
;       Right clicks bring up the context menu.
(define context-click 1)

; NOTE: move() is called when a command is active and the user is dragging
;       the cursor, following click.
(define context-move 2)

; NOTE: context() is run when a context menu entry is chosen.
(define context-context 3)

; NOTE: prompt() is run when Enter is pressed.
;       appendPromptHistory is automatically called before prompt()
;       is called so calling it is only needed for erroneous input.
;       Any text is in the command prompt is sent as an uppercase string.
(define context-prompt 4)

; context defaults to context-main
(define context context-main)
