(set-logic QF_NRA_ODE)
(declare-fun beta () Real)
(declare-fun beta_0_0 () Real)
(declare-fun beta_0_t () Real)
(assert (>= beta_0_0  -5.900000))
(assert (>= beta_0_t  -5.900000))
(assert (<= beta_0_0  5.900000))
(assert (<= beta_0_t  5.900000))
(declare-fun mu () Real)
(declare-fun mu_0_0 () Real)
(declare-fun mu_0_t () Real)
(assert (>= mu_0_0  3.9))
(assert (>= mu_0_t  3.9))
(assert (<= mu_0_0  4.1))
(assert (<= mu_0_t  4.1))
(declare-fun s () Real)
(declare-fun s_0_0 () Real)
(declare-fun s_0_t () Real)
(assert (>= s_0_0  0))
(assert (>= s_0_t  0))
(assert (<= s_0_0  1000))
(assert (<= s_0_t  1000))
(declare-fun tau () Real)
(declare-fun tau_0_0 () Real)
(declare-fun tau_0_t () Real)
(assert (>= tau_0_0  0))
(assert (>= tau_0_t  0))
(assert (<= tau_0_0  30))
(assert (<= tau_0_t  30))
(declare-fun v () Real)
(declare-fun v_0_0 () Real)
(declare-fun v_0_t () Real)
(assert (>= v_0_0  0))
(assert (>= v_0_t  0))
(assert (<= v_0_0  27.78))
(assert (<= v_0_t  27.78))
(declare-fun time_0 () Real)
(assert (>= time_0  0))
(assert (<= time_0  30))
(define-ode flow_1 ((= d/dt[beta]  0)(= d/dt[mu]  0)(= d/dt[s]  v)(= d/dt[tau]  1.0)(= d/dt[v] (-(* 0.05776(exp(+(*(- 0.05776) tau)(+(+(* beta 0.1) 0) param))))(*(* 3.028e-4 v) v)))))
(assert (and 
((and(= s_0_0 0)(= v_0_0 0)(= tau_0_0 0)))
(>= mu_0_0 3.9)
(<= mu_0_0 4.1)
(>= mu_0_t 3.9)
(<= mu_0_t 4.1)
(>= beta_0_0 -5.9)
(<= beta_0_0 -5.1625)
(>= beta_0_t -5.9)
(<= beta_0_t -5.1625)
(= [beta_0_t mu_0_t s_0_t tau_0_t v_0_t ] (integral 0.0 time_0 [beta_0_0 mu_0_0 s_0_0 tau_0_0 v_0_0 ] flow_1))
((and(= v_0_t 27.78)))))
(check-sat)
(exit)
