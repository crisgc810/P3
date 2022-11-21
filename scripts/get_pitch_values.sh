#!/bin/bash
for r2maxth in $(seq 0.43 0.01 0.47);
#for r2maxth in 0.61 0.62 0.63;
    do for clipmult in $(seq 0.005 0.005 0.015);
        #do for division in 1 3 5;
            #do for r1r0th in 0.8 0.0;
                #do for zcrth in 0.0;
                    do echo -ne "r $r2maxth \t c $clipmult \t";
                    run_get_pitch $r2maxth $clipmult
                    #get_pitch prueba.wav prueba.f0 --r2maxth=$r2maxth --clipmult=$clipmult --division=$division --r1r0th=$r1r0th --zcrth=$zcrth; 
                #done
            #done
        #done
    done
done
#|sort -t: -k2n

exit 0
