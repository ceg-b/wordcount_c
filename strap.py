#!/usr/bin/env python3

import random

random.seed(0)



yes=0
no=0;



for p in range(0,100000):

    p1=random.random()
    p2=random.random()
    
    if (abs(p1-p2)<1/240):
        yes=yes+1
    else:
        no=no+1


print("Finally "+str(yes)+" "+str(no)+" "+str(yes/no))



    


