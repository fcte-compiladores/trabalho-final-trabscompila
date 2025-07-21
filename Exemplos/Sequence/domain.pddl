;; Exemplo com tipos hierárquicos - representando sequência
(define (domain sequence)
  (:requirements :strips :typing)
  (:types 
    number - object
    position - object
  )
  (:predicates 
    (at-position ?num - number ?pos - position)
    (next ?pos1 ?pos2 - position)
    (generated ?num - number)
  )
  (:action generate-next
    :parameters (?n1 ?n2 ?result - number ?pos1 ?pos2 ?pos3 - position)
    :precondition (and 
      (at-position ?n1 ?pos1) 
      (at-position ?n2 ?pos2)
      (next ?pos1 ?pos2)
      (next ?pos2 ?pos3)
      (not (generated ?result))
    )
    :effect (and 
      (at-position ?result ?pos3)
      (generated ?result)
    )
  )
)
