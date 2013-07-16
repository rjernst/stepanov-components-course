(provide 'binary-counter)

(defun add-to-counter (counter op zero carry)
  (cond ((null counter) carry)
        ((eq (car counter) zero) (progn (setf (car counter) carry)
                                        zero))
        (t (let ((new-carry (funcall op (car counter) carry)))
             (setf (car counter) zero)
             (add-to-counter (cdr counter) op zero new-carry)))))

(defun skip-zeros (counter zero)
  (cond ((or (null counter) (not (eq (car counter) zero))) counter)
        (t (skip-zeros (cdr counter) zero))))

(defun reduce-counter (counter op zero result)
  (cond ((null counter) result)
        ((eq zero (car counter)) (reduce-counter (cdr counter) op zero result))
        (t (reduce-counter (cdr counter) op zero (funcall op (car counter) result)))))

;;; Creates a new binary counter and returns 3 functions
;;; add, reduce and dump.
(defun binary-counter (initial-capacity op zero)
  (let ((counter (make-list initial-capacity))
        (op op)
        (zero zero))
    (values
     ;; Function to add to counter.
     (function (lambda (x) 
                 (let ((result (add-to-counter counter op zero x)))
                   (when result
                     (setf counter (append counter (list result)))))))
     ;; Function to reduce the counter.
     (function (lambda () 
                 (let ((counter (skip-zeros counter zero)))
                   (if (null counter)
                       nil
                     (reduce-counter (cdr counter) op zero (car counter))))))
     ;; Function to dump the counter.
     (function (lambda () counter)))))

;;; Creates a combine function that can be used to apply the operator
;;; to items in the binary counter. 
(defun make-combine (op)
  (let ((op op))
    (function (lambda (x y) (if (funcall op y x) y x)))))

#|

Test: 

(multiple-value-setq (add reduce dump) (binary-counter 8 (make-combine '<) nil))
(mapcar add '(1 2 3 4 5 6 7 8 9 10))
(funcall reduce)

|#                 
    
