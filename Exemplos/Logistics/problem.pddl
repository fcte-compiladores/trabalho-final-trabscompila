(define (problem logistics-delivery)
  (:domain logistics-advanced)
  (:objects 
    office warehouse airport-loc - location
    truck1 - truck
    plane1 - airplane
    package1 package2 - package
  )
  (:init 
    (at package1 office)
    (at package2 office)
    (at truck1 office)
    (at plane1 airport-loc)
    (empty truck1)
    (airport airport-loc)
    (connected office warehouse)
    (connected warehouse airport-loc)
  )
  (:goal 
    (and 
      (at package1 warehouse)
      (at package2 airport-loc)
    )
  )
)
