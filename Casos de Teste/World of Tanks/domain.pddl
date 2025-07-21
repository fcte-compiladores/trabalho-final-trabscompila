(define (domain world-of-tanks)
  (:requirements :strips :typing)
  (:types
    tank
    location
    obstacle
    shell
  )
  (:predicates
    (at ?t - tank ?l - location)
    (adjacent ?l1 - location ?l2 - location)
    (clear ?l - location)
    (loaded ?t - tank ?s - shell)
    (can-shoot ?t - tank ?o - obstacle ?l - location)
    (obstacle-at ?o - obstacle ?l - location)
  )
  (:action move
    :parameters (?t - tank ?from - location ?to - location)
    :precondition (and (at ?t ?from) (adjacent ?from ?to) (clear ?to))
    :effect (and (not (at ?t ?from)) (at ?t ?to) (not (clear ?from)) (clear ?to))
  )
  (:action fire
    :parameters (?t - tank ?o - obstacle ?l - location ?s - shell)
    :precondition (and (at ?t ?l) (can-shoot ?t ?o ?l) (loaded ?t ?s))
    :effect (not (obstacle-at ?o ?l))
  )
  (:action load
      :parameters (?t - tank ?s - shell)
      :precondition (not (loaded ?t ?s))
      :effect (loaded ?t ?s))
)
