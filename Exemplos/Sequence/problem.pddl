(define (problem sequence-basic)
  (:domain sequence)
  (:objects 
    num1 num2 num3 - number
    pos1 pos2 pos3 - position
  )
  (:init 
    (at-position num1 pos1)
    (at-position num2 pos2)
    (next pos1 pos2)
    (next pos2 pos3)
  )
  (:goal 
    (and 
      (at-position num3 pos3)
      (generated num3)
    )
  )
)
