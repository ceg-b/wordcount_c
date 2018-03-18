#!/usr/bin/env python3

import sys
import pandas


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


print("50 More common words")
print(by_occurence[1:50])
print("No of different words "+str(len(by_occurence)))
print("No of singleton words "+str(len([x for x in filter(lambda x:x[1]==1,by_occurence)]))) # Python filter does not return list. 
print(sorted_pairs_of_letters)

i_pair=list(filter(lambda x:x[0][0]=='i',sorted_pairs_of_letters))
print(i_pair)


