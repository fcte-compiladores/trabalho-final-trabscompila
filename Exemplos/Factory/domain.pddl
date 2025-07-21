;; Exemplo complexo - Fábrica com fluentes numéricos e recursos
(define (domain factory-complex)
  (:requirements :strips :typing :fluents :adl)
  (:types 
    machine - object
    product - object
    resource - object
    worker - object
  )
  (:predicates 
    (available ?m - machine)
    (busy ?m - machine)
    (produced ?p - product)
    (assigned ?w - worker ?m - machine)
    (can-operate ?w - worker ?m - machine)
  )
  (:functions
    (resource-level ?r - resource)
    (production-time ?m - machine ?p - product)
    (total-time)
    (capacity ?m - machine)
  )
  
  (:action assign-worker
    :parameters (?w - worker ?m - machine)
    :precondition (and 
      (available ?m)
      (can-operate ?w ?m)
      (not (exists (?m2 - machine) (assigned ?w ?m2)))
    )
    :effect (and 
      (assigned ?w ?m)
      (not (available ?m))
      (busy ?m)
    )
  )
  
  (:action produce-item
    :parameters (?m - machine ?p - product ?w - worker ?r - resource)
    :precondition (and 
      (assigned ?w ?m)
      (busy ?m)
      (>= (resource-level ?r) 10)
      (>= (capacity ?m) 1)
    )
    :effect (and 
      (produced ?p)
      (decrease (resource-level ?r) 10)
      (increase (total-time) (production-time ?m ?p))
      (decrease (capacity ?m) 1)
    )
  )
  
  (:action maintenance
    :parameters (?m - machine ?w - worker)
    :precondition (and 
      (assigned ?w ?m)
      (< (capacity ?m) 5)
    )
    :effect (and 
      (assign (capacity ?m) 10)
      (increase (total-time) 60)
    )
  )
  
  (:action release-worker
    :parameters (?w - worker ?m - machine)
    :precondition (assigned ?w ?m)
    :effect (and 
      (not (assigned ?w ?m))
      (not (busy ?m))
      (available ?m)
    )
  )
)
