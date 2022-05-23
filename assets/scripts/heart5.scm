
x: |
    (*
        16
        (pow (sin t) 3)
    )
y: |
    (-
        (* 13 (cos t))
        (* 5 (cos (* 2 t)))
        (* 2 (cos (* 3 t)))
        (cos (* 4 t))
    )
range: [ 0.0, 2.0 ]
variable: t
num_points: 512
scale: [ 0.04, 0.04 ]
