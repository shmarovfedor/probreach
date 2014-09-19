(set-logic QF_NRA_ODE)
(declare-fun x () Real)
(declare-fun v () Real)
(declare-fun x_0_0 () Real)
(declare-fun x_0_t () Real)
(declare-fun x_1_0 () Real)
(declare-fun x_1_t () Real)
(declare-fun x_2_0 () Real)
(declare-fun x_2_t () Real)
(declare-fun v_0_0 () Real)
(declare-fun v_0_t () Real)
(declare-fun v_1_0 () Real)
(declare-fun v_1_t () Real)
(declare-fun v_2_0 () Real)
(declare-fun v_2_t () Real)
(declare-fun time_0 () Real)
(declare-fun time_1 () Real)
(declare-fun time_2 () Real)
(declare-fun mode_0 () Real)
(declare-fun mode_1 () Real)
(declare-fun mode_2 () Real)
(define-ode flow_1 ((= d/dt[x] v) (= d/dt[v] -9.8)))
(define-ode flow_2 ((= d/dt[x] v) (= d/dt[v] (- -9.8 (/ (+ (* 5 v) (/ x 0.0025)) 7)))))
(assert (<= -411 x_0_0))
(assert (<= x_0_0 429))
(assert (<= -411 x_0_t))
(assert (<= x_0_t 429))
(assert (<= -411 x_1_0))
(assert (<= x_1_0 429))
(assert (<= -411 x_1_t))
(assert (<= x_1_t 429))
(assert (<= -411 x_2_0))
(assert (<= x_2_0 429))
(assert (<= -411 x_2_t))
(assert (<= x_2_t 429))
(assert (<= -100 v_0_0))
(assert (<= v_0_0 100))
(assert (<= -100 v_0_t))
(assert (<= v_0_t 100))
(assert (<= -100 v_1_0))
(assert (<= v_1_0 100))
(assert (<= -100 v_1_t))
(assert (<= v_1_t 100))
(assert (<= -100 v_2_0))
(assert (<= v_2_0 100))
(assert (<= -100 v_2_t))
(assert (<= v_2_t 100))
(assert (<= 0 time_0 [0.000000]))
(assert (<= time_0 10 [0.000000]))
(assert (<= 0 time_1 [0.000000]))
(assert (<= time_1 10 [0.000000]))
(assert (<= 0 time_2 [0.000000]))
(assert (<= time_2 10 [0.000000]))
(assert (<= 1 mode_0))
(assert (<= mode_0 2))
(assert (<= 1 mode_1))
(assert (<= mode_1 2))
(assert (<= 1 mode_2))
(assert (<= mode_2 2))
(assert (and (and (= v_0_0 0) (<= x_0_0 9.82423) (>= x_0_0 9.82422)) (= mode_0 1) (= [x_0_t v_0_t] (integral 0. time_0 [x_0_0 v_0_0] flow_1)) (= mode_0 1) (forall_t 1 [0 time_0] (>= x_0_t 0)) (>= x_0_t 0) (>= x_0_0 0) (= mode_1 2) (<= x_0_t 0) (= v_1_0 v_0_t) (= x_1_0 x_0_t) (= [x_1_t v_1_t] (integral 0. time_1 [x_1_0 v_1_0] flow_2)) (= mode_1 2) (forall_t 2 [0 time_1] (<= x_1_t 0)) (<= x_1_t 0) (<= x_1_0 0) (= mode_2 1) (>= x_1_t 0) (= v_2_0 v_1_t) (= x_2_0 x_1_t) (= [x_2_t v_2_t] (integral 0. time_2 [x_2_0 v_2_0] flow_1)) (= mode_2 1) (forall_t 1 [0 time_2] (>= x_2_t 0)) (>= x_2_t 0) (>= x_2_0 0) (= mode_2 1) (>= x_2_t 7)))
(check-sat)
(exit)
