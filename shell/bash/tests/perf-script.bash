#!/usr/local/bin/bash

typeset -i m2 m1 M n2 n1 N m n
typeset -i MM=5 NN=5

case $# in
  0) :
  ;;
  1) MM=$1; NN=$1
  ;;
  2) MM=$1; NN=$2
  ;;
  *) echo 1>&2 "Usage: $0 [m [n]]"
  ;;
esac

EMPTYLINE=:  # echo
echo 'a = { '    # mathematica

let "M=1"                   # for (M=1; M<=MM; M++)
while let "M <= MM"; do
  let "N=1"                 # for (N=1; N<=NN; N++)
  while let "N <= NN"; do

    let "m1 = M - 1"
    let "m2 =  M + 1"
    let "n1 = N - 1"
    let "n2 =  N + 1"


    echo -n '{ '  # math
    let "m=1"               # for(m=1; m<=MM; m++)
    while let "m <= MM"; do
      let "n=1"              # for(n=1; n<=NN; n++)
      while let "n <= NN"; do

        let "x = (m-m1)*(m-M)*(m-m2)"
        let "y = (n-n1)*(n-N)*(n-n2)"

        if let "(x*x + (n-N)*(n-N)) * ((m-M)*(m-M) + y*y)"; then
	  echo -n "0,"
        else  # neighbour
	  echo -n "1,"
        fi

	let "n=n+1"
      done
      echo -n " "; let "m=m+1"     # ". "
    done
    echo '},'


    let "N=N+1"
    $EMPTYLINE  
  done
  $EMPTYLINE  
  let "M=M+1"
done

echo '}'



echo -n 'o = { '
let "m=1"
while let "m <= MM"; do
  let "n=1"
  while let "n <= NN"; do
    echo -n "1,"
    let "n=n+1"
  done
  let "m=m+1"
done
echo " }"


echo 'x = LinearSolve[a,o]  '

exit 0



