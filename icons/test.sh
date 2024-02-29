for I in *.png
do
   xxd -i $I >$I.h
done 
