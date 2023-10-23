#!/bin/bash

usage(){
    echo "usage: ${0##*/} script [-H | --help]"
    echo -e "\t-h,  print this message"
}

error(){
    echo -e "error occured: $1"
    usage
}

size_x=100
size_y=100

wx=10
wy=10

prec=100

line_width=2

data_file="data.dat"
out_file="plot.png"
# out_format="set xzeroaxis linetype 3 linewidth 2.5; set xrange [-$wx:$wx]; set yrange [-$wy:$wy]; set grid xtics ytics; set terminal png; set output '$out_file';"
# out_format="set xrange [-$wx:$wx]; set yrange [-$wy:$wy]; set grid xtics ytics; set terminal png; set output '$out_file';"
out_format="set xrange [-$wx:$wx]; set yrange [-$wy:$wy]; set grid xtics ytics;"
# cmd='function f(x) {
#     return (2.819800 + -0.047003*(x - -3.000000) + 0.000000*(x - -3.000000)^2 + -0.046199*(x - -3.000000)^3)
# };
# if(i >= -3 && i < -1)
# {
#     print i, f(i), "*", "*", "*"; printf ""
# }
# else if(i >= -1 && i < 1)
# {
#     print i, "*", g(i), "*", "*"; printf ""
# }
# else if(i >= 1 && i < 3)
# {
#     print i, "*", "*", l(i), "*"; printf ""
# }
# else if(i >= 3 && i <= 5)
# {
#     print i, "*", "*", "*", m(i); printf ""
# }

if [ $# -gt 0 ]; then
    r=""
    for ((i=1; i<=$#; i++)); 
    do
        f="function f$i(x) {
            return (${!i})
        };";
        r+=$f;
    done
else
    echo "No arguments provided!"
fi
pts="print i"
for ((i=1; i<=$#; i++)); 
do 
    pts+=", f$i(i)"; 
done
pts=${pts}'; printf ""'
echo $pts
cmd=${r}'BEGIN {for(i=-bound_x;i<=bound_x;i+=1/p){'${pts}'}}'
echo $cmd
awk -v bound_x=$size_x -v bound_y=$size_y -v p=$prec "$cmd" > $data_file

# gnuplot -e "$out_format p '$data_file' u 1:2 w l lw 2, '$data_file' u 1:3 w l lw 2, '$data_file' u 1:4 w l lw 2, '$data_file' u 1:5 w l lw 2; pause -1"

out="$out_format p" 
for ((i=1; i<=$#; i++)); 
do 
    out+=" '$data_file' u 1:$((i+1)) w l lw $line_width,"
done
out=${out::-1}"; pause -1"
echo $out
gnuplot -e "$out"
