set terminal pngcairo size 800,600        # Output image size
set output 'equations.png'                # Output file name

set xrange [-5:5]
set yrange [-5:5]

# Define equations
f(x) = acos(x - 2)
g(x) = sin(x) + 2

set title "System of Equations"
set xlabel "x"
set ylabel "y"
unset key

plot f(x) with lines lt 1 lc rgb "red" title "x - cos(y) = 2", \
      g(x) with lines lt 1 lc rgb "blue" title "y - sin(x) = 2"
