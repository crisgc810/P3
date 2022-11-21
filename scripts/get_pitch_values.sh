#!/bin/bash
for r2maxth in 0.395 0.40; #$(seq 0.40 0.01 0.405);
#for r2maxth in 0.61 0.62 0.63;
    do for clipmult in 0.0075 0.008; #$(seq 0.0075 0.0005 0.008);
        do for r1r0th in 0.55; #$(seq 0.52 0.015 0.55);
            do for zcrth in 30; #$(seq 20 10 50);
                do for medfilt in 1 3 5 7;
    do echo -ne "r $r2maxth \t c $clipmult \t 1 $r1r0th \t z $zcrth \t m $medfilt \t";
    run_get_pitch $r2maxth $clipmult $r1r0th $zcrth $medfilt| grep TOTAL
    #get_pitch prueba.wav prueba.f0 --r2maxth=$r2maxth --clipmult=$clipmult --division=$division --r1r0th=$r1r0th --zcrth=$zcrth; 
                done
            done
        done
    done
done
#|sort -t: -k2n

exit 0
