(provide 'util)

;; ---------------------------------------------------------------
;; Instrumented operations using Lexical Closures... 
;; ---------------------------------------------------------------

;; Makes two function: an instrumented operation and a get-counts
;; that can be called to see the total number of calls. To reset
;; the counts, just make a new set. 
(defun make-instrumented (op) 
  (let ((op-count 0))
    (values (function (lambda (x y) 
                        (progn
                          (setq op-count (+ op-count 1))
                          (funcall op x y))))
            (function (lambda () op-count)))))

#|
(multiple-value-bind (cmp get-counts) (make-instrumented '<)
    (funcall cmp 10 20)
    (funcall cmp 11 34)
    (funcall get-counts))
|#

;; ---------------------------------------------------------------
;; Generating Data Sets for Testing
;; ---------------------------------------------------------------

(defun set-car-sequential (list start step)
  (cond ((null list) nil)
        (t (setf (car list) start)
           (set-car-sequential (cdr list) (+ start step) step))))

(defun gen-sequential-list (size start step)
  (let ((result (make-list size)))
    (set-car-sequential result start step)
    result))

;; ---------------------------------------------------------------
;; Timing
;; ---------------------------------------------------------------

(defmacro timing (&body forms)
  (let ((real1 (gensym))
        (real2 (gensym))
        (run1 (gensym))
        (run2 (gensym))
        (result (gensym)))
    `(let* ((,real1 (get-internal-real-time))
            (,run1 (get-internal-run-time))
            (,result (progn ,@forms))
            (,run2 (get-internal-run-time))
            (,real2 (get-internal-real-time)))
       (format *debug-io* ";;; Computation took:~%")
       (format *debug-io* ";;;  ~f seconds of real time~%"
               (/ (- ,real2 ,real1) internal-time-units-per-second))
       (format t ";;;  ~f seconds of run time~%"
               (/ (- ,run2 ,run1) internal-time-units-per-second))
       ,result)))
