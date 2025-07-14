;;;; startup environment for TinyScheme

(load "libs/init.scm")

;; load module emulation adapted from ChickenScheme
(load "libs/tinymodules.scm")

;; import general utilities (Clojure, SRFI, etc.)
(load "libs/utils.scm")
