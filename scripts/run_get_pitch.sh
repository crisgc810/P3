#!/bin/bash

r2maxth=${1:-0.43}
clipmult=${2:-0.01}
# Put here the program (maybe with path)
GETF0="get_pitch --r2maxth=$r2maxth --clipmult=$clipmult"
DB="$HOME/PAV/P3/pitch_db/train"

for fwav in $HOME/PAV/P3/pitch_db/train/*.wav; do
    ff0=${fwav/.wav/.f0};
    $GETF0 $fwav $ff0 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1);

done

pitch_evaluate pitch_db/train/*.f0ref | fgrep TOTAL
exit 0


