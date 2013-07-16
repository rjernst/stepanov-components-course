(require 'util "./util.lisp")
(require 'binary-counter "./binary-counter.lisp")

;; ---------------------------------------------------------------
;; min2-practical
;; ---------------------------------------------------------------

;; Tail recursively iterates over the collection updating min1 and
;; min2 as it goes along. 
(defun min2-practical-helper (min1 min2 first last f-next f-value f-cmp)
  (cond ((eq first last) (values min1 min2))
        (t (let ((value (funcall f-value first)))
             (if (funcall f-cmp value min2)
                 (if (funcall f-cmp value min1)
                     (min2-practical-helper value min1 (funcall f-next first) last f-next f-value f-cmp)
                   (min2-practical-helper min1 value (funcall f-next first) last f-next f-value f-cmp))
               (min2-practical-helper min1 min2 (funcall f-next first) last f-next f-value f-cmp))))))

;; The main function deals with special cases. If the collection has more than 2 items
;; it then iterates through the rest updating min1 and min2. 
(defun min2-practical (first last f-next f-value f-cmp)
  (cond ((eq first last) (values last last))
        (t (let ((min1 (funcall f-value first)))
             (cond ((eq last (funcall f-next first)) (values min1 min1))
                   (t (let* ((first (funcall f-next first))
                             (value (funcall f-value first)))
                        (if (funcall f-cmp value min1) 
                            (min2-practical-helper value min1 (funcall f-next first) last f-next f-value f-cmp)
                          (min2-practical-helper min1 value (funcall f-next first) last f-next f-value f-cmp)))))))))

#|
;; Example 1: using lists of ints.                
(min2-practical '(10 20 334 42 53 6) nil 'cdr 'car '<)

;; Example 2: using lists of strings. 
(min2-practical '("hi" "there" "you" "sir") nil 'cdr 'car 'string<)

;; Example 3: using sequences (eg: a string, and finding the two min chars). Yikes. 
(let* ((text "hi there")
       (first 0)
       (last (length text))
       (f-next (function (lambda (x) (+ x 1))))
       (f-value (function (lambda (x) (aref text x))))
       (f-cmp 'char<))
  (min2-practical first last f-next f-value f-cmp))
|#

;;; ---------------------------------------------------------------
;;; Reusable code for min-element-1-2, all versions. This code
;;; knows nothing of state carried around in the counter. 
;;; ---------------------------------------------------------------

(defun add-many-to-counter (first last f-next f-value f-add f-node)
  (cond ((eq first last) nil)
        (t (progn
             (funcall f-add (funcall f-node (funcall f-value first)))
             (add-many-to-counter (funcall f-next first) last f-next f-value f-add f-node)))))

(defun min-element-list (min items f-cmp) 
  (cond ((null items) min)
        ((funcall f-cmp (car items) min) (min-element-list (car items) (cdr items) f-cmp))
        (t (min-element-list min (cdr items) f-cmp))))

(defun min-element-1-2 (first last f-next f-value f-cmp f-combine f-node)
  (cond ((or (eq first last) (eq (funcall f-next first) last)) (values first last))
        (t (multiple-value-bind (add reduce dump) 
                                (binary-counter 8 f-combine nil)
              (add-many-to-counter first last f-next f-value add f-node)
              (let* ((winner (funcall reduce))
                     (loosers (cadr winner)))
                (values (car winner) 
                        (min-element-list (car loosers) (cdr loosers) f-cmp)))))))

;;; ---------------------------------------------------------------
;;; min-element-1-2: not stable
;;;
;;; Each item in the counter is a list (winner, listOfLoosers)
;;; For example: (10 (23 34 45))
;;; ---------------------------------------------------------------

(defun make-combine-collect-loosers (op)
  (let ((op op))
    (function (lambda (x y) 
                (labels ((process-win (winner looser) 
                           (progn
                             (setf (cadr looser) nil)
                             (setf (cadr winner) (cons (car looser) (cadr winner)))
                             winner)))
                (if (funcall op (car y) (car x))
                    (process-win y x)
                  (process-win x y)))))))

(defun min-element-1-2-unstable (first last f-next f-value f-cmp)
  (let ((node (function (lambda (x) (list x nil)))))
    (min-element-1-2 first last f-next f-value f-cmp (make-combine-collect-loosers f-cmp) node)))

#|
(min-element-1-2-unstable '(10 20 334 42 53 6) nil 'cdr 'car '<)
|#

;;; ---------------------------------------------------------------
;;; min-element-1-2: stable
;;;
;;; Each item in the counter is a list (winner, loosersHead, loosersTail)
;;; For example: (10 (23 34 45) (45))
;;;
;;; By keeping track of both the head and tail of the loosers, we 
;;; inserts loosers at the right end of the list to maintain stability
;;; ---------------------------------------------------------------

(defun make-combine-collect-loosers-stable (op)
  (let ((op op))
    (function (lambda (x y) 
                (labels ((process-win (winner looser in-order) 
                           (progn
                             ;; Clear loosers for the looser.
                             (setf (cadr looser) nil)
                             (setf (caddr looser) nil)
                             ;; Add the looser in the right place. 
                             (cond ((null (cadr winner))
                                    ;; Special case: no loosers yet. 
                                    (setf (cadr winner) (list (car looser)))
                                    (setf (caddr winner) (cadr winner)))
                                   (t (if in-order
                                          (progn (setf (cdr (caddr winner)) (list (car looser)))
                                                 (setf (caddr winner) (cdr (caddr winner))))
                                        (setf (cadr winner) (cons (car looser) (cadr winner))))))
                             winner)))
                (if (funcall op (car y) (car x))
                    (process-win y x nil)
                  (process-win x y t)))))))

(defun min-element-1-2-stable (first last f-next f-value f-cmp)
  (let ((node (function (lambda (x) (list x nil nil)))))
    (min-element-1-2 first last f-next f-value f-cmp 
                     (make-combine-collect-loosers-stable f-cmp)
                     node)))

#|
(min-element-1-2-stable '(10 20 334 42 53 6) nil 'cdr 'car '<)
|#

(defun test-one (function first last f-next f-value f-cmp)
  (multiple-value-bind (instrumented-cmp instrumented-count) (make-instrumented f-cmp)
     (multiple-value-bind (min1 min2) (funcall function first last f-next f-value instrumented-cmp)
        (values min1 min2 (funcall instrumented-count)))))  

(defun test-mins (size start step)
  (timing
   (let ((first (gen-sequential-list size start step))
         (last nil)
         (f-next 'cdr)
         (f-value 'car)
         (f-cmp '<))
     (multiple-value-bind (min1 min2 n) (test-one 'min2-practical first last f-next f-value f-cmp)
        (format t "min2-practical          : min1=~D, min2=~D, num-ops=~D~%" min1 min2 n))
     (multiple-value-bind (min1 min2 n) (test-one 'min-element-1-2-unstable first last f-next f-value f-cmp)
        (format t "min-element-1-2-unstable: min1=~D, min2=~D, num-ops=~D~%" min1 min2 n))
     (multiple-value-bind (min1 min2 n) (test-one 'min-element-1-2-stable first last f-next f-value f-cmp)
        (format t "min-element-1-2-stable  : min1=~D, min2=~D, num-ops=~D~%" min1 min2 n))
     )))
   
#|

(test-mins 1000 1 1)
(test-mins 1000 1000 -1)
(test-mins (* 10000 10000) 1 1)

|#  
