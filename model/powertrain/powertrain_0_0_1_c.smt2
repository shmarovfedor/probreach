(set-logic QF_NRA_ODE)
; declaring functions and their bounds
(declare-fun Kd () Real)
(declare-fun Kd_0_0 () Real)
(declare-fun Kd_0_t () Real)
(assert (>= Kd_0_0  0))
(assert (>= Kd_0_t  0))
(assert (<= Kd_0_0  0.5))
(assert (<= Kd_0_t  0.5))
(declare-fun Ki () Real)
(declare-fun Ki_0_0 () Real)
(declare-fun Ki_0_t () Real)
(assert (>= Ki_0_0  0))
(assert (>= Ki_0_t  0))
(assert (<= Ki_0_0  0.5))
(assert (<= Ki_0_t  0.5))
(declare-fun Kp () Real)
(declare-fun Kp_0_0 () Real)
(declare-fun Kp_0_t () Real)
(assert (>= Kp_0_0  0))
(assert (>= Kp_0_t  0))
(assert (<= Kp_0_0  0.5))
(assert (<= Kp_0_t  0.5))
(declare-fun counter () Real)
(declare-fun counter_0_0 () Real)
(declare-fun counter_0_t () Real)
(assert (>= counter_0_0  0))
(assert (>= counter_0_t  0))
(assert (<= counter_0_0  0.1))
(assert (<= counter_0_t  0.1))
(declare-fun e () Real)
(declare-fun e_0_0 () Real)
(declare-fun e_0_t () Real)
(assert (>= e_0_0 (- 1000)))
(assert (>= e_0_t (- 1000)))
(assert (<= e_0_0  1000))
(assert (<= e_0_t  1000))
(declare-fun e_der () Real)
(declare-fun e_der_0_0 () Real)
(declare-fun e_der_0_t () Real)
(assert (>= e_der_0_0 (- 1e6)))
(assert (>= e_der_0_t (- 1e6)))
(assert (<= e_der_0_0  1e6))
(assert (<= e_der_0_t  1e6))
(declare-fun e_int () Real)
(declare-fun e_int_0_0 () Real)
(declare-fun e_int_0_t () Real)
(assert (>= e_int_0_0 (- 1e6)))
(assert (>= e_int_0_t (- 1e6)))
(assert (<= e_int_0_0  1e6))
(assert (<= e_int_0_t  1e6))
(declare-fun lambda () Real)
(declare-fun lambda_0_0 () Real)
(declare-fun lambda_0_t () Real)
(assert (>= lambda_0_0 (- 1000)))
(assert (>= lambda_0_t (- 1000)))
(assert (<= lambda_0_0  1000))
(assert (<= lambda_0_t  1000))
(declare-fun lambda_prev () Real)
(declare-fun lambda_prev_0_0 () Real)
(declare-fun lambda_prev_0_t () Real)
(assert (>= lambda_prev_0_0 (- 1000)))
(assert (>= lambda_prev_0_t (- 1000)))
(assert (<= lambda_prev_0_0  1000))
(assert (<= lambda_prev_0_t  1000))
(declare-fun p () Real)
(declare-fun p_0_0 () Real)
(declare-fun p_0_t () Real)
(assert (>= p_0_0 (- 1000)))
(assert (>= p_0_t (- 1000)))
(assert (<= p_0_0  1000))
(assert (<= p_0_t  1000))
(declare-fun pe () Real)
(declare-fun pe_0_0 () Real)
(declare-fun pe_0_t () Real)
(assert (>= pe_0_0 (- 1000)))
(assert (>= pe_0_t (- 1000)))
(assert (<= pe_0_0  1000))
(assert (<= pe_0_t  1000))
(declare-fun tau () Real)
(declare-fun tau_0_0 () Real)
(declare-fun tau_0_t () Real)
(assert (>= tau_0_0  0))
(assert (>= tau_0_t  0))
(assert (<= tau_0_0  3600))
(assert (<= tau_0_t  3600))
(declare-fun the () Real)
(declare-fun the_0_0 () Real)
(declare-fun the_0_t () Real)
(assert (>= the_0_0  0))
(assert (>= the_0_t  0))
(assert (<= the_0_0  180))
(assert (<= the_0_t  180))
(declare-fun the_in () Real)
(declare-fun the_in_0_0 () Real)
(declare-fun the_in_0_t () Real)
(declare-fun u () Real)
(declare-fun u_0_0 () Real)
(declare-fun u_0_t () Real)
(assert (>= u_0_0 (- 1e6)))
(assert (>= u_0_t (- 1e6)))
(assert (<= u_0_0  1e6))
(assert (<= u_0_t  1e6))
(declare-fun w () Real)
(declare-fun w_0_0 () Real)
(declare-fun w_0_t () Real)
; assigning the values of the sample
(assert (>= the_in_0_0 30.78150543461923))
(assert (>= the_in_0_t 30.78150543461923))
(assert (<= the_in_0_0 30.78150543461924))
(assert (<= the_in_0_t 30.78150543461924))
(assert (>= w_0_0 105.1255066344554))
(assert (>= w_0_t 105.1255066344554))
(assert (<= w_0_0 105.1255066344554))
(assert (<= w_0_t 105.1255066344554))
(assert (>= Kd_0_0 0))
(assert (>= Kd_0_t 0))
(assert (<= Kd_0_0 0))
(assert (<= Kd_0_t 0))
(assert (>= Ki_0_0 0))
(assert (>= Ki_0_t 0))
(assert (<= Ki_0_0 0))
(assert (<= Ki_0_t 0))
(assert (>= Kp_0_0 0.1873141244829035))
(assert (>= Kp_0_t 0.1873141244829035))
(assert (<= Kp_0_0 0.1873141244829035))
(assert (<= Kp_0_t 0.1873141244829035))
; declaring time variable and integration bounds
(declare-fun time () Real)
(assert (>= time 0.0))
(assert (<= time 0.1))
; defining odes
(define-ode flow_1 ((= d/dt[Kd]  0)
(= d/dt[Ki]  0)
(= d/dt[Kp]  0)
(= d/dt[counter]  1)
(= d/dt[e]  0)
(= d/dt[e_der]  0)
(= d/dt[e_int]  0)
(= d/dt[lambda] (* 4(-(/(* 0.9(+(+(+(- 0.366)(*(* 0.08979 w) p))(*(*(- 0.0337) w)(^ p 2)))(*(* 0.0001(^ w 2)) p)))(*(*(* 1(/ 1 14.7))(+ 1 u))(+(+(+(- 0.366)(*(* 0.08979 w) pe))(*(*(- 0.0337) w)(^ pe 2)))(*(* 0.0001(^ w 2)) pe)))) lambda)))
(= d/dt[lambda_prev]  0)
(= d/dt[p] (* 0.41328(-(*(* 2(+(+(+ 2.821(*(- 0.05231) the))(* 0.10299(^ the 2)))(*(- 0.00063)(^ the 3))))(^(-(/ p 1)(^(/ p 1) 2)) 0.5))(* 0.9(+(+(+(- 0.366)(*(* 0.08979 w) p))(*(*(- 0.0337) w)(^ p 2)))(*(* 0.0001(^ w 2)) p))))))
(= d/dt[pe] (* 0.41328(-(*(*(* 1 2)(+(+(+ 2.821(*(- 0.05231) the))(* 0.10299(^ the 2)))(*(- 0.00063)(^ the 3))))(^(-(/ p 1)(^(/ p 1) 2)) 0.5))(+(+(+(- 0.366)(*(* 0.08979 w) pe))(*(*(- 0.0337) w)(^ pe 2)))(*(* 0.0001(^ w 2)) pe)))))
(= d/dt[tau]  1)
(= d/dt[the] (* 10(- the_in the)))
(= d/dt[the_in]  0)
(= d/dt[u]  0)
(= d/dt[w]  0)
))
; defining initial condition
(assert (and (>= counter_0_0 -4.940656458412465e-324) (<= counter_0_0 4.940656458412465e-324)))
(assert (and (>= e_0_0 -0.2854692857567027) (<= e_0_0 -0.2854578504889353)))
(assert (and (>= e_der_0_0 1.66168624088046) (<= e_der_0_0 1.661856730690836)))
(assert (and (>= e_int_0_0 -0.07371128093147256) (<= e_int_0_0 -0.07370957603336802)))
(assert (and (>= lambda_0_0 14.4145307142433) (<= lambda_0_0 14.41454214951106)))
(assert (and (>= lambda_prev_0_0 14.4145307142433) (<= lambda_prev_0_0 14.41454214951106)))
(assert (and (>= p_0_0 0.9980285739812108) (<= p_0_0 0.9980285912458806)))
(assert (and (>= pe_0_0 0.9508208636146825) (<= pe_0_0 0.9508213388131054)))
(assert (and (>= tau_0_0 0.1999977100640535) (<= tau_0_0 0.2)))
(assert (and (>= the_0_0 27.80656392602308) (<= the_0_0 27.8066322924485)))
(assert (and (>= u_0_0 -0.05347242932827658) (<= u_0_0 -0.0534702873411065)))
; defining the integral
(assert (= [Kd_0_t Ki_0_t Kp_0_t counter_0_t e_0_t e_der_0_t e_int_0_t lambda_0_t lambda_prev_0_t p_0_t pe_0_t tau_0_t the_0_t the_in_0_t u_0_t w_0_t ] (integral 0.0 time [Kd_0_0 Ki_0_0 Kp_0_0 counter_0_0 e_0_0 e_der_0_0 e_int_0_0 lambda_0_0 lambda_prev_0_0 p_0_0 pe_0_0 tau_0_0 the_0_0 the_in_0_0 u_0_0 w_0_0 ] flow_1)))
; defining invariants negations
(assert (or
(not (<(/(- lambda_0_t 14.7) 14.7) 0.05))
(not (>(/(- lambda_0_t 14.7) 14.7)(- 0.05)))
(not (>=(- 1 p_0_t) 0))
))
(check-sat)
(exit)
