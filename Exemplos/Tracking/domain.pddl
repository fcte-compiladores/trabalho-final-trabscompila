(define (domain stacking)
  (:requirements :strips :typing)
  (:types
    object
    block - object
  )
  (:predicates
    (clear ?x - block)
    (ontable ?x - block)
    (holding ?x - block)
    (handempty)
    (on ?x - block ?y - block)
  )
  (:action pick-up
    :parameters (?x - block)
    :precondition (and (clear ?x) (ontable ?x) (handempty))
    :effect (and (not (ontable ?x)) (not (handempty)) (holding ?x) (not (clear ?x)))
  )
  (:action put-down
    :parameters (?x - block)
    :precondition (holding ?x)
    :effect (and (ontable ?x) (handempty) (not (holding ?x)) (clear ?x))
  )
  (:action stack
    :parameters (?x - block ?y - block)
    :precondition (and (holding ?x) (clear ?y))
    :effect (and (not (holding ?x)) (not (clear ?y)) (handempty) (on ?x ?y) (clear ?x))
  )
  (:action unstack
    :parameters (?x - block ?y - block)
    :precondition (and (on ?x ?y) (clear ?x) (handempty))
    :effect (and (holding ?x) (clear ?y) (not (handempty)) (not (on ?x ?y)) (not (clear ?x)))
  )
)
