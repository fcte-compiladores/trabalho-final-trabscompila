;; Exemplo básico: mover um objeto entre duas localizações
(define (domain hello-world)
  (:requirements :strips)
  (:predicates 
    (at ?obj ?loc)
  )
  (:action move
    :parameters (?obj ?from ?to)
    :precondition (at ?obj ?from)
    :effect (and (not (at ?obj ?from)) (at ?obj ?to))
  )
)
