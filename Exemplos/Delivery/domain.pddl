(define (domain delivery)
  (:requirements :strips :typing)
  (:types
    location
    package
    truck - object
  )
  (:predicates
    (at ?obj - object ?loc - location)
    (in ?pkg - package ?truck - truck)
    (available ?truck - truck)
    (destination ?pkg - package ?loc - location)
  )
  (:action load
    :parameters (?pkg - package ?truck - truck ?loc - location)
    :precondition (and (at ?pkg ?loc) (at ?truck ?loc) (available ?truck))
    :effect (and (in ?pkg ?truck) (not (at ?pkg ?loc)) (not (available ?truck)))
  )
  (:action unload
    :parameters (?pkg - package ?truck - truck ?loc - location)
    :precondition (and (in ?pkg ?truck) (at ?truck ?loc))
    :effect (and (at ?pkg ?loc) (not (in ?pkg ?truck)) (available ?truck))
  )
  (:action drive
    :parameters (?truck - truck ?from - location ?to - location)
    :precondition (at ?truck ?from)
    :effect (and (not (at ?truck ?from)) (at ?truck ?to))
  )
)
