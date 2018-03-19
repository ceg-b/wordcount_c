#!/usr/bin/env python3

import sys
import pandas as pd


def divide(word):
    wov="aąeęoóyui"
    out=[];

    t=0;
    part=""

    for l in word:
        if l in wov:
            out.append(part)
            out.append(l)
            part=""
        else:
            part+=l

    return out


def pairs(word):

    out=[]
    for t in range(0,len(word)-1):
        out.append(''.join(word[t:t+2]))

    return [s for s in filter(lambda p: len(p)==2,out)]



the_file=sys.argv[1];



# create the dictionary from a file
dictionary=dict();
pwords=dict();
old_w=""
new_w=""
with open(the_file) as f:
    for the_line in f:
        for w in the_line.split():
            w=w.lower()
            w=''.join(filter(lambda c:c.isalpha(),w))

            if (not w in dictionary):
                dictionary.update({w:1})
            else :
                tmp_count=dictionary[w];
                dictionary.update({w:tmp_count+1})

            if w=="the" or w=="a":
                continue
                
            old_w=new_w
            new_w=w
            the_key=(old_w,new_w)

            if (old_w and new_w):
                if (not the_key in pwords):
                    pwords.update({the_key:1})
                else:
                    tmp_count=pwords[the_key];
                    pwords.update({the_key:tmp_count+1})
                    
print(dictionary)

# 
# pairs: pairs of letters. For building Marhov chain
# 
pairs_of_letters=dict()
for k in dictionary:
    for s in pairs(k):
        if (not s in pairs_of_letters):
            pairs_of_letters.update({s:1})
        else :
            tmp_count=pairs_of_letters[s];
            pairs_of_letters.update({s:tmp_count+1})


#sorted by occurences
by_occurence=([(k, dictionary[k]) for k in sorted(dictionary, key=dictionary.get, reverse=True)])
            
#
# 
sorted_pairs_of_letters=([(k, pairs_of_letters[k]) for k in sorted(pairs_of_letters, key=pairs_of_letters.get, reverse=True)])
sorted_w_pairs=([(k, pwords[k]) for k in sorted(pwords, key=pwords.get, reverse=True)])

print("50 More common words")
print(by_occurence[1:50])
print("No of different words "+str(len(by_occurence)))
print("No of singleton words "+str(len([x for x in filter(lambda x:x[1]==1,by_occurence)]))) # Python filter does not return list. 
print(sorted_pairs_of_letters)

i_pair=list(filter(lambda x:x[0][0]=='i',sorted_pairs_of_letters))
print(i_pair)
print()
print(sorted_w_pairs[1:50])


### output for pandas:
col1=list(map(lambda x:x[0],sorted_pairs_of_letters))
col2=list(map(lambda x:x[1],sorted_pairs_of_letters))
pd_letters=pd.DataFrame(data={'pair':col1,'freq':col2})
pd_letters.to_csv("lpairs.csv");

