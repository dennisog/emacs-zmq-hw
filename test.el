;; load the module
(add-to-list 'load-path "~/repos/emacs-zmq-hw")
(require 'emacs-zmq-hw)

;; connect, send a few messages, delete the connection

(let ((conn (zmq-hw-new-connection))
      (nmsgs 5))
  (while (> nmsgs 0)
    (zmq-hw-send conn "This is a message from emacs!")
    (message "%s" (zmq-hw-receive conn))
    (setq nmsgs (- nmsgs 1))
    (sleep-for 3))
  (zmq-hw-delete-connection conn))
