#!/bin/bash

################################################################################
## the following run type 1 instances (except those presented as infeasible) and
## all type 2 instances
################################################################################

rm expKMIS-* ;
make KMIS_E_CARD=1;
make KMIS_E_CARD=2 ;
make KMIS_E_CARD=3 ;
make KMIS_E_CARD=4 ;
make KMIS_E_CARD=5 ;
make KMIS_E_CARD=6 ;

sleep 3 ;

./expKMIS-1 & ;
./expKMIS-2 & ;
./expKMIS-3 & ;
./expKMIS-4 & ;
./expKMIS-5 & ;
./expKMIS-6 & ;