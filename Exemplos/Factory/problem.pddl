(define (problem factory-production)
  (:domain factory-complex)
  (:objects 
    machine1 machine2 - machine
    product1 product2 product3 - product
    steel wood - resource
    worker1 worker2 - worker
  )
  (:init 
    (available machine1)
    (available machine2)
    (can-operate worker1 machine1)
    (can-operate worker2 machine2)
    (can-operate worker1 machine2)
    (= (resource-level steel) 100)
    (= (resource-level wood) 80)
    (= (production-time machine1 product1) 30)
    (= (production-time machine2 product2) 45)
    (= (production-time machine1 product3) 60)
    (= (total-time) 0)
    (= (capacity machine1) 10)
    (= (capacity machine2) 8)
  )
  (:goal 
    (and 
      (produced product1)
      (produced product2)
      (produced product3)
      (<= (total-time) 200)
    )
  )
)
