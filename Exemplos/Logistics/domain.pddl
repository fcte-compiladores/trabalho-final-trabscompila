;; Exemplo avançado - Sistema logístico com múltiplas ações
(define (domain logistics-advanced)
  (:requirements :strips :typing :conditional-effects)
  (:types 
    location - object
    vehicle - object
    package - object
    truck airplane - vehicle
  )
  (:predicates 
    (at ?obj - object ?loc - location)
    (in ?pkg - package ?veh - vehicle)
    (airport ?loc - location)
    (connected ?loc1 ?loc2 - location)
    (loaded ?veh - vehicle)
    (empty ?veh - vehicle)
  )
  
  (:action load-truck
    :parameters (?pkg - package ?truck - truck ?loc - location)
    :precondition (and 
      (at ?pkg ?loc) 
      (at ?truck ?loc)
      (empty ?truck)
    )
    :effect (and 
      (not (at ?pkg ?loc))
      (in ?pkg ?truck)
      (not (empty ?truck))
      (loaded ?truck)
    )
  )
  
  (:action drive-truck
    :parameters (?truck - truck ?from ?to - location)
    :precondition (and 
      (at ?truck ?from)
      (connected ?from ?to)
    )
    :effect (and 
      (not (at ?truck ?from))
      (at ?truck ?to)
    )
  )
  
  (:action unload-truck
    :parameters (?pkg - package ?truck - truck ?loc - location)
    :precondition (and 
      (in ?pkg ?truck)
      (at ?truck ?loc)
    )
    :effect (and 
      (not (in ?pkg ?truck))
      (at ?pkg ?loc)
      (empty ?truck)
      (not (loaded ?truck))
    )
  )
  
  (:action fly-airplane
    :parameters (?plane - airplane ?from ?to - location)
    :precondition (and 
      (at ?plane ?from)
      (airport ?from)
      (airport ?to)
    )
    :effect (and 
      (not (at ?plane ?from))
      (at ?plane ?to)
    )
  )
)
