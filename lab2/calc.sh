a=$1; b=$2; c=$3

if [[ $# != 3 ]]; then
  echo Enter all parameters: a b c
  exit
fi
echo $a'x'^2+$b'x'+$c=0

d=$(( $b*$b-4*$a*$c ))
echo 'D= '$d

if (( $d > 0 )); then
  x1=$(echo "scale=3; 0.5*(-$b+sqrt($d))/$a" | bc)
  x2=$(echo "scale=3; 0.5*(-$b-sqrt($d))/$a" | bc)
  echo 'x1= '$x1
  echo 'x2= '$x2
elif (( $d == 0 )); then
  x12=$(echo "scale=3; 0.5*(-$b)/$a" | bc)
  echo 'x12= '$x12
else
  xr=$(echo "scale=3; 0.5*(-$b)/$a" | bc)
  xi=$(echo "scale=3; 0.5*sqrt($((-$d)))/$a" | bc)
  echo $xi
  echo 'x1= '$xr' + '$xi'i'
  echo 'x2= '$xr' - '$xi'i'
fi
