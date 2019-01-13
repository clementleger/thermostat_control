#!/bin/bash
rm *.h
for i in $(seq 0 9)
do
	png_to_bytes -I -n number_$i -i $i.png -o number_$i.h
	png_to_bytes -I -n number_small_$i -i ${i}_small.png -o number_small_$i.h
	echo "#include \"number_$i.h\"" >> images.h
	echo "#include \"number_small_$i.h\"" >> images.h
done

other_files=(thermometer dot humidity home percent)

for item in ${other_files[*]}
do
	png_to_bytes -I -n $item -i $item.png -o $item.h
	echo "#include \"$item.h\"" >> images.h
done

mv *.h ../

